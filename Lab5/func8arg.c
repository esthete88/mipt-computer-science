#include <stdio.h>
#include <stdlib.h>

int foo(int a, int b, int c, int d, int e, int f, int g, int h)
{
	return a+b+c+d+e+f+g+h;
}

int main()
{
	return foo(1, 2, 3, 4, 5, 6, 7, 8);
}