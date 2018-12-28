#include <fstream>
#include <iostream>
#include <stdio.h>
#include <ctime>

using namespace std;

const int size = 10000;

int A[10000];
int i, sort, j, k, minim, index, temp;

int main() {
	clock_t time1, time2;
	ofstream fout;
	fout.open("/home/esthete/2sem/ArrayMy.csv");
	fout << "N,time" << endl;
	for (k = 500; k <= size; k += 100)
	{
		for (sort = 0; sort < k; ++sort)
		{
			A[sort] = k - sort;
		}
		j = i - 1;
		time1 = clock();
		asm(
			"pushq %rbx\n"
			"xor %ecx, %ecx\n"
			"for1:\n"
				"movl A(,%ecx,4), %eax\n"
				"movl %ecx, %edx\n"
				"movl %ecx, %edi\n"
				"incl %edx\n"
				"for2:\n"
					"cmpl %eax, A(,%edx,4)\n"
						"jnl endfor2\n"
						"movl %edx, %edi\n"
						"movl A(,%edx,4), %eax\n"
					"endfor2: incl %edx\n"
						"cmpl %edx, i\n"
							"jne for2\n"
				"xchg %eax, A(,%ecx,4)\n"
				"movl %eax, A(,%edi,4)\n"
				"incl %ecx\n"
				"cmpl %ecx, j\n"
					"jne for1\n"
			"popq %rbx"
		);
		// Below you can see the realization on C
		//
		// for (i = 0; i < 29; ++i)
		// {
		// 	minim = A[i];
		// 	index = i;
		// 	for (j = i + 1; j < 30; ++j)
		// 	{
		// 		if (A[j] < minim)
		// 		{
		// 			minim = A[j];
		// 			index = j;
		// 		}
		// 	}
		// 	temp = A[index];
		// 	A[index] = A[i];
		// 	A[i] = temp;
		// }
		time2 = clock() - time1;
		fout << i << "," << (double)time2 / CLOCKS_PER_SEC << endl;
	}
	fout.close();
	return 0;
}
