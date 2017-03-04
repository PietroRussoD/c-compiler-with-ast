main()
{
	int a,b,res;
	a=5;

	switch(a)
	{
		case 1:
		case 2:
		{
			res=a/2;
			break;
		}
		case 3:
		{
			b=10;
		}
		case 4:
		{
			res=a+b;
			break;
		}
		case 5:
		{
			break;
		}
		default:
		{
			res=0;
		}
	}
}