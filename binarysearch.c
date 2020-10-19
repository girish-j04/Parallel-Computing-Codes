#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int left = m - l + 1;
    int right = r - m;
    int L[left], R[right];
    for (i = 0; i < left; i++)
        L[i] = arr[l + i];
    for (j = 0; j < right; j++)
        R[j] = arr[m + 1 + j];
    i = 0;
    j = 0;
    k = l;
    while (i < left && j < right) 
    {
        if (L[i] <= R[j]) 
        {
            arr[k] = L[i];
            i++;
        }
        else 
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < left) 
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < right) 
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergesort(int arr[], int l, int r)
{
    if (l < r) 
    {
        int mid = l + (r - l) / 2;
        mergesort(arr, l, mid);
        mergesort(arr, mid + 1, r);
        merge(arr, l, mid, r);
    }

}
int binary_search_sequential(int arr[], int n, int element)
{
    int l = 0, r = n-1, mid;
    while (l < r)
    {
        mid = l + (r-l)/2;
        if (arr[mid] == element)
        {
            return mid;
        }
        if (arr[mid] > element)
        {
            r = mid-1;
        }
        else
        {
            l = mid+1;
        }
    }
    if (arr[l] == element)
    {
        return l;
    }
    return -1;
}
int binary_search_parallel(int arr[], int l, int r, int element)
{
    if (l > r)
    {
        return -1;
    }
    if (l == r)
    {
        if (arr[l] == element)
        {
        return l;
        }
        return -1;
    }
    int mid = l + (r-l)/2;
    if (arr[mid] == element)
    {
    return mid;
    }
    int res = -1;
    #pragma omp parallel num_threads(4)
    {
    #pragma omp single
    {
    if (arr[mid] > element)
    {
    #pragma omp task   
    {
    res = binary_search_parallel(arr, l, mid-1, element);
    }
    }
    else
    {
    #pragma omp task
    {
    res = binary_search_parallel(arr, mid+1, r, element);
    }
    }
    #pragma omp taskwait
    }
    }
    return res;
}
int main()
{
    srand(time(0));
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);
    int arr[n];
    for (int a = 0; a < n; a++)
    {
    arr[a] = rand()%n;
    }
    int element = n+100, res;
    printf("\tSearching for: %d\n", element);
    struct timeval TimeValue_Start;
    struct timezone TimeZone_Start;
    struct timeval TimeValue_Final;
    struct timezone TimeZone_Final;
    long time_start, time_end;
    double time_overhead;
    mergesort(arr, 0, n-1);
    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    res = binary_search_sequential(arr, n, element);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    printf("TIME TAKEN FOR SEQUENTIAL BINARY SEARCH: %lf\n", time_overhead);
    if (res == -1)
    {
    printf("\telementment not found\n");
    }
    else
    {
    printf("\telementment found at: %d\n", res);
    }
    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    res = binary_search_parallel(arr, 0, n-1, element);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    printf("TIME TAKEN FOR PARALLEL BINARY SEARCH: %lf\n", time_overhead);
    if (res == -1)
    {
    printf("\telementment not found\n");
    }
    else
    {
    printf("\telementment found at: %d\n", res);
}
 
}