function [Homo,h0,uv,err]= Homography(XYZ,NP,mm)

for i=1:NP,
 	    cc(2*i-1,:)=[XYZ(i,1) XYZ(i,2) 1 0 0 0 -XYZ(i,1)*mm(i,1) -XYZ(i,2)*mm(i,1) -mm(i,1)];
  	    cc(2*i,:)  =[0 0 0 XYZ(i,1) XYZ(i,2) 1 -XYZ(i,1)*mm(i,2) -XYZ(i,2)*mm(i,2) -mm(i,2)];
end
      
[u,d,v]=svd(cc);
h0 = v(:,end);

Homo=[h0(1) h0(2) h0(3);
   h0(4) h0(5) h0(6);
   h0(7) h0(8) h0(9);];

hx = Homo*[XYZ ones(NP,1)]';
uv=[hx(1,:)./hx(3,:); hx(2,:)./hx(3,:)];

err = sum(sum((uv-mm').*(uv-mm')));

