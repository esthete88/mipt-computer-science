#include <stdio.h>

void sort(int* A, int n) {
	int i, j, minim, index, temp;
	for (i = 0; i < n-1; ++i)
	{
		minim = A[i];
		index = i;
		for (j = i + 1; j < n; ++j)
		{
			if (A[j] < minim)
			{
				minim = A[j];
				index = j;
			}
		}
		temp = A[index];
		A[index] = A[i];
		A[i] = temp;
	}
}

int main() {
	int A[4] = {3, 3478, 1, 0};
	sort(A, 4);
	printf("%d%d%d%d", A[0], A[1], A[2], A[3]);
}