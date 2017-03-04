main()
{
	int a,b,c,d,sum;
	char ac,bc,cc,dc,sumc;
	double ad,bd,cd,dd,sumd;
	sum=a*b+c*d;
	sum=a*2+5*d;
	sum=-c;

	sumc=ad*b+c*dc;
	sum=ad*'x'+5.0*d;
	sumd=-(c*bd);

	//Unary Arithmetic Operators
	a=++b;
	a=b++;
	a=!b;

	//Relational Operators
	a=(b==1);
	a=(bd!=0)+(ad<=ac);

	//Logical Operators
	a=(cd+cc)&&(c>=9);
	a=90||(cc+'y');

	//Compound Assignment Operators
	a+=5;
	b=a*=a/=90;

	//Conditional Operator
	a=(ac>d)?(sum=a+b):(sum=a/b);

	//Shift Operators
	a=a>>2;
	a=a<<3;
}