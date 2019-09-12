// C++ implementation of Plane Homography
// Using ezMTL C++ library for handling matrices
// Made by DJKang, 2008 PNU
#include <stdio.h>
#include "./ezmtl/Matrix.h"

void main()
{
	Matrix<double> a(10,8), x1(5,2), x2(5,2), b(10,1);
	int i;

	x1(1,1) =   0;  x1(1,2)  =   0;
	x1(2,1) = 140;  x1(2,2)  =   0;
	x1(3,1) = 140;  x1(3,2)  =  90;
	x1(4,1) =   0;  x1(4,2)  =  90;


	x2(1,1) = 295.6825;  x2(1,2)  =   24.6023;
	x2(2,1) =  49.2821;  x2(2,2)  =  303.4359;
	x2(3,1) =  87.9020;  x2(3,2)  =  574.4706;
	x2(4,1) = 424.3918;  x2(4,2)  =  465.4169;



	for(i=0; i<4; i++)
	{
		a(2*i+1,3) = 1.0;
		a(2*i+1,4) = a(2*i+1,5) = a(2*i+1,6) = 0.0;

		a(2*i+2,1) = a(2*i+2,2) = a(2*i+2,3) = 0.0;
		a(2*i+2,6) = 1.0;
	}


	for(i=0; i<4; i++)
	{
		a(2*i+1,1) =  x1(i+1,1);
		a(2*i+1,2) =  x1(i+1,2);
		a(2*i+1,7) = -x2(i+1,1)*x1(i+1,1);
		a(2*i+1,8) = -x2(i+1,1)*x1(i+1,2);

		a(2*i+2,4) =  x1(i+1,1);
		a(2*i+2,5) =  x1(i+1,2);
		a(2*i+2,7) = -x2(i+1,2)*x1(i+1,1);
		a(2*i+2,8) = -x2(i+1,2)*x1(i+1,2);

		b(2*i+1,1) =  x2(i+1,1);
		b(2*i+2,1) =  x2(i+1,2);
	}

 //	a.Print(cout,"h");

	int retCode;
	Matrix<double> x(8,1);
	x = Inv(Transpose(a)*a,&retCode)*Transpose(a)*b;
//	x.Print(cout,"Val ");

	Matrix<double> hh(3,3), w(3,1), c(3,1);
	hh(1,1) = x(1,1); hh(1,2) = x(2,1); hh(1,3) = x(3,1);
	hh(2,1) = x(4,1); hh(2,2) = x(5,1); hh(2,3) = x(6,1);
	hh(3,1) = x(7,1); hh(3,2) = x(8,1); hh(3,3) = 1.0;

	hh.Print(cout, "H matrix: ");


	for(i=0; i<4; i++)
	{
		 w(1,1) = x1(i+1,1);
		 w(2,1) = x1(i+1,2);
		 w(3,1) = 1.0;

		 c = hh*w;

		 double row = c(1,1)/c(3,1) ;
		 double col = c(2,1)/c(3,1) ;

		 double dist = hypot(row-x2(i+1,1),col-x2(i+1,2));
			
		 printf("Dist error: %7.4f\n",dist);
	}
}

