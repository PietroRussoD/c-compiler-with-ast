# C-like Compiler with Abstract Syntax Tree

The realised compiler implements operations of mathematics, assignment and relational, furthermore it is possible to implement selection and iteration statements.
It is created a lexical analyser with FLEX that recognises tokens. The grammar is defined by the rules recognised by the compiler. The syntax analyser, created with BISON, is specialised in this.
It is implemented also a symbol table for keep in memory the variables that are used by the other statements.
In the compile step, there are checks for inform if there are a logic error. If there are not errors the Abstract Syntax Tree (AST) is realised. In the end, the AST is read from top to bottom for write the quadruples.

## Installing

It is necessary to have installed *bison* and *flex*.

Use the following commands to build the compiler:

```
cd src
make install && make clean
```
The executable is created in the *bin* directory.

## Usage

```
Usage: scanner file
```
After that the compiler has terminated the process without errors, it creates a text file with the set of quadruples, and it prints out the symbol tables.

## Examples

In the *examples* directory there are some c files.
The following code:

```c
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
```
It is converted in the following set of quadruples
```assembly
	MOVI #0, , a
	MOVI #0, , b
	MOVI #0, , res
	MOVI #5, , a
	JMP , ,cond_for_1
for_1:
cond_for_1:
	JMP , , istr_for_1
	JMP , , end_for_1
istr_for_1:
	ADDI #1, a, a
	JMPGTE a, #10, if_1
	JMP , , end_if_1
if_1:
	JMP , , end_for_1
end_if_1:
	JMP , , for_1
end_for_1:
	MOVI #0, , c
	JMP , ,cond_for_2
for_2:
	ADDI #1, c, c
cond_for_2:
	JMP , , istr_for_2
	JMP , , end_for_2
istr_for_2:
	JMPE c, #10, if_2
	JMP , , end_if_2
if_2:
	JMP , , end_for_2
end_if_2:
	JMP , , for_2
end_for_2:
	MOVI #0, , b
	JMP , ,cond_for_3
for_3:
	ADDI #1, b, b
cond_for_3:
	JMPGT b, #5, istr_for_3
	JMP , , end_for_3
istr_for_3:
	ADDI b, a, TEMPI1
	MOVI TEMPI1, , res
	JMP , , for_3
end_for_3:
	JMP , ,cond_for_4
for_4:
cond_for_4:
	CMPIGTE a, #8, TEMPI1
	CMPILTE b, #10, TEMPI2
	CMPNZ TEMPI1, , TEMPI3
	CMPNZ TEMPI2, , TEMPI4
	ORL TEMPI3, TEMPI4, TEMPI5
	JMPNZ TEMPI5, , istr_for_4
	JMP , , end_for_4
istr_for_4:
	DIVI b, a, TEMPI1
	MOVI TEMPI1, , res
	ADDI #1, b, b
	ADDI a, 2, a
	JMP , , for_4
end_for_4:
```

## Built With

* [GCC](https://gcc.gnu.org/) - The GNU Compiler Collection
* [Flex](https://github.com/westes/flex) - The Fast Lexical Analyser
* [Bison](https://www.gnu.org/software/bison/) - GNU Bison

## Authors

* **Pietro Russo**
* **Antonio Specchia**

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
