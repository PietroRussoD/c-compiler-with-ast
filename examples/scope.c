main()
/* First Compound Statement */
{
	int a;
	a=3;
	/* Second Compound Statement */
	{
		int b=4;
		a=5;
	}
	/* End of the Second Compound Statement */
	b=5; /* Error!! */
}
/* End of the First Compound Statement */