main()
{
	int a,b,res;
	a=5;
	
	for( ; ; )
	{
		a++;
		if(a>=10)
			break;
	}
	for(int c=0 ; ; c++)
	{
		if(c==10)
			break;
	}
	for(b=0 ; b>5 ; b++)
	{
		res=b+a;
	}
	for( ; (a>=8) || (b<=10); )
	{
		res=b/a;
		++b;
		a+=2;
	}
}