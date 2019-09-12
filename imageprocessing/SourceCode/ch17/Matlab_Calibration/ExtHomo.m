function [R,t,P] = ExtHomo(A,h)

hh = [h(1) h(2) h(3); h(4) h(5) h(6); h(7) h(8) h(9);];

r1=inv(A)*hh(:,1); r1 = r1/norm(r1);
r2=inv(A)*hh(:,2); r2 = r2/norm(r2);
t =inv(A)*hh(:,3)/norm(inv(A)*hh(:,1));
r3(1) = r1(2)*r2(3)-r1(3)*r2(2);
r3(2) = r1(3)*r2(1)-r1(1)*r2(3);
r3(3) = r1(1)*r2(2)-r1(2)*r2(1);
R = [r1 r2 r3'];
P = A*[R t];


