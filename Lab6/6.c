#include <stdio.h>
extern void sort(long long*, long long);

int main() {
	long long A[3] = {2, 56, 1};
	sort(A, 3);
	printf("%lld%lld%lld\n", A[0], A[1], A[2]);
	return 0;
}