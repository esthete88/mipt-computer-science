#include <stdio.h>
#include <stdlib.h>

int foo(int a, int b, int c, int d, int e, int f)
{
	return a+b+c+d+e+f;
}

int main()
{
	return foo(1, 2, 3, 4, 5, 6);
}