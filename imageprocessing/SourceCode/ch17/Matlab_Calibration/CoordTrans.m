function P = CoordTrans(C, angle, trans) 
% Projection matrix from C, Angle, Translational vector

d2r = pi/180;

r0 = [angle(1)*d2r  angle(2)*d2r angle(3)*d2r]; % Rotation para. of the model  
Rz = [cos(r0(3)) -sin(r0(3)) 0; sin(r0(3)) cos(r0(3)) 0; 0 0 1];
Ry = [cos(r0(2)) 0 sin(r0(2)); 0 1 0; -sin(r0(2)) 0 cos(r0(2))];
Rx = [1 0 0; 0 cos(r0(1)) -sin(r0(1)); 0 sin(r0(1)) cos(r0(1))];
R  = Rz*Ry*Rx;   
R0 = R;

P  = C*[R0 trans];


