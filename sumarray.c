#include <stdio.h>
#include <omp.h>
void main()
{
int sum = 0;
int lsum = 0;
int n;
printf("Enter number of elements in the array : \n");
scanf("%d", &n);
int A[100] = {0};
printf("Enter elements : \n");
for (int a = 0; a < n; a++)
{
scanf("%d", &A[a]);
}
#pragma omp parallel private(lsum)
{
#pragma omp for
for (int i = 0; i < n; i++)
{
lsum = lsum + A[i];
}
#pragma omp critical
{
sum += lsum;
}
}
printf("Sum is : %d\n", sum);}