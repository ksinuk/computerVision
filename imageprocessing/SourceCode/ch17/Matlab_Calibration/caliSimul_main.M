% 평면모델의 서로 다른 3개의 view를 이용하여 카메라의 내부행렬과 외부행렬을 계산
%
% (1) 공간 내 어떤 평면의 영상좌표를 생성
% (2) Zhang의 알고리즘으로 내부행렬 A를 구함
% (3) 카메라행렬 A를 이용하여 외부행렬 R, T를 구함
% 2009년 9월 22일
% PNU, VISLab, D.J. Kang

clear all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%  (1) 공간 내 어떤 평면의 영상좌표를 생성
%%%% -- 임의로 가정한 카메라 내부행렬 C, 세개의 카메라 위치를
%%%% -- 나타내는 R, T를 가정
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Build a Planar scale pattern
Xw = [0 0 0; 200 0 0; 200 300 0; 0 300 0]';  % Four points in world space
X1 = [Xw; ones(1,4) ];  % homogeneous coord.

% 카메라의 내부파라메터 행렬 (We assume a Intrinsic Matrix here)
C=[1000 1 512;0 1000 384;0 0 1;]; 


% Projection matrices for poses simulated to project the world points on image plane
% Three matrices for 3 different camera poses 
P1 = CoordTrans(C,[60 -10 10], [-250 -230 800]');
P2 = CoordTrans(C,[50 30 -30], [10 -20 700]');
P3 = CoordTrans(C,[-60 10 40], [-40 50 1000]');

% Get image coordinate from each projection matrix
imc = P1*X1; % image coordinate for first camera position   
ipc1 = [imc(1,:)./imc(3,:); imc(2,:)./imc(3,:)];
imc = P2*X1; % image coordinate for second camera position
ipc2 = [imc(1,:)./imc(3,:); imc(2,:)./imc(3,:)];
imc = P3*X1; % image coordinate for third camera position
ipc3 = [imc(1,:)./imc(3,:); imc(2,:)./imc(3,:)];

% Add random noises for original image data
distort = 0;  % noises = 0
ipc1 = ipc1 + distort*(rand(1)-0.5);
ipc2 = ipc2 + distort*(rand(1)-0.5);
ipc3 = ipc3 + distort*(rand(1)-0.5);

% Plot three different poses of the projected plane on image plane
plot(ipc1(1,:),ipc1(2,:)); line([ipc1(1,4) ipc1(1,1)],[ipc1(2,4) ipc1(2,1)]); hold on;
plot(ipc2(1,:),ipc2(2,:)); line([ipc2(1,4) ipc2(1,1)],[ipc2(2,4) ipc2(2,1)]); hold on;
plot(ipc3(1,:),ipc3(2,:)); line([ipc3(1,4) ipc3(1,1)],[ipc3(2,4) ipc3(2,1)]);
axis([0 1024 0 768]);
axis ij

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%  (2) Zhang의 알고리즘으로 카메라 내부행렬 A를 구함
%%%%   -- 앞에서 발생시킨 영상점을 이용하여 역으로 
%%%    -- 좌표발생에 사용된 R, T, 카메라행렬 C를 추정
%%%    -- A와 C가 같아지는지를 확인함
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% From here, 
% The code is implemented from Zhang Calibration algorithm
XYZ = Xw(1:2,:)';  % The 3D coordinate in world coordinate sysem
NP  = 4; % number of points
NOP = 3; % number of planes

% Plane homograph matrix H for each camera poses
[hh1,h1,uv1,err1]=Homograp(XYZ,NP,ipc1'); 
[hh2,h2,uv2,err2]=Homograp(XYZ,NP,ipc2');
[hh3,h3,uv3,err3]=Homograp(XYZ,NP,ipc3');

% Matrix normalization
h1=h1/h1(9); h2=h2/h2(9); h3=h3/h3(9);  
ha=[h1 h2 h3]';
avrErr = [err1 err2 err3];

% Calculation of vv matrix
for i=1:NOP,
    vv(2*i-1,1)=ha(i,1)*ha(i,2);
    vv(2*i-1,2)=ha(i,1)*ha(i,5)+ha(i,2)*ha(i,4);
    vv(2*i-1,3)=ha(i,4)*ha(i,5);
    vv(2*i-1,4)=ha(i,2)*ha(i,7)+ha(i,1)*ha(i,8);
    vv(2*i-1,5)=ha(i,5)*ha(i,7)+ha(i,8)*ha(i,4);
    vv(2*i-1,6)=ha(i,7)*ha(i,8);
    
    vv(2*i,1)  = ha(i,1)*ha(i,1)-ha(i,2)*ha(i,2);
    vv(2*i,2)  =(ha(i,1)*ha(i,4)+ha(i,1)*ha(i,4))-(ha(i,2)*ha(i,5)+ha(i,2)*ha(i,5));
    vv(2*i,3)  = ha(i,4)*ha(i,4)-ha(i,5)*ha(i,5);
    vv(2*i,4)  =(ha(i,1)*ha(i,7)+ha(i,1)*ha(i,7))-(ha(i,2)*ha(i,8)+ha(i,2)*ha(i,8));
    vv(2*i,5)  =(ha(i,4)*ha(i,7)+ha(i,4)*ha(i,7))-(ha(i,5)*ha(i,8)+ha(i,5)*ha(i,8));
    vv(2*i,6)  = ha(i,7)*ha(i,7)-ha(i,8)*ha(i,8);
end

% Singular value decomposition from vv matrix
% Solution is eigenvector of matrix v corresponding 
% to smallest singular value of matrix d
[u,d,v] = svd(vv);
bb = v(:,end);

% B matrix (=K(-T)*K(-1))
BB = [bb(1) bb(2) bb(4); bb(2) bb(3) bb(5); bb(4) bb(5) bb(6);];

% Get the intrinsic parameter of camera to make camera matrix
v0 = (bb(2)*bb(4)-bb(1)*bb(5))/(bb(1)*bb(3)-bb(2)*bb(2));
lambda = bb(6)-( bb(4)*bb(4) + v0*(bb(2)*bb(4)-bb(1)*bb(5)) )/bb(1);
au = sqrt(lambda/bb(1));
av = sqrt( (lambda*bb(1)) / (bb(1)*bb(3)-bb(2)*bb(2)) );
gamma = -bb(2)*(au*au*av/lambda);
u0 = gamma*v0/au - bb(4)*au*au/lambda; 

% Intrinsic matrix of camera
A = [au gamma u0;0 av v0; 0 0 1;];

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%  (3) 카메라행렬과 H행렬을 이용하여 외부행렬 R, T를 구함
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
% Extrinsic parameters from Homography     
[R1,t1,sP1] = ExtHomo(A,h1);
[R2,t2,sP2] = ExtHomo(A,h2);
[R3,t3,sP3] = ExtHomo(A,h3);


