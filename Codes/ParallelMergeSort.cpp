#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include <ctime>
using namespace std;
//Merge function for merge sort
void merge(vector<int> &arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[m + 1 + j];
    }
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k++] = L[i++];
        }
        else
        {
            arr[k++] = R[j++];
        }
    }
    while (i < n1)
    {
        arr[k++] = L[i++];
    }
    while (j < n2)
    {
        arr[k++] = R[j++];
    }
}
//Sequential Merge Sort
void mergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
//Parallel Merge Sort
void parallelMergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
#pragma omp parallel sections
        {
#pragma omp section
            parallelMergeSort(arr, l, m);
#pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}
int main(){
   // Sequential Merge Sort
    srand(time(0));
    int n = 10000; // Change the size of the array as needed
    vector<int> arr(n), arr_copy;
    // Fill the array with random numbers
    for (int i = 0; i < n; ++i)
    {
        arr[i] = rand() % 1000;
    }
    arr_copy = arr;
    double start = omp_get_wtime();
    start = omp_get_wtime();
    mergeSort(arr_copy, 0, n - 1);
    double end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << end - start << " seconds" << endl;
    //Parallel Merge Sort
    arr_copy = arr;
    start = omp_get_wtime();
    parallelMergeSort(arr_copy, 0, n - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << end - start << " seconds" << endl;
    return 0;
}
