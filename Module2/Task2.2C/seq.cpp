#include <iostream>
#include <chrono>

#define N 1000000

using namespace std;
using namespace chrono;

int partition(int *A, int lo, int hi);
void quicksort(int *A, int lo = 0, int hi = N-1);

int main()
{
    // Initialise array
    int *A;
    A = (int *) malloc(N * sizeof(int *));

    // Fill with random elements
    for (int i = 0; i < N; i++)
        A[i] = rand() % 100;

    // Start time
    auto start = high_resolution_clock::now();

    // Sort
    quicksort(A);

    // Stop time
    auto stop = high_resolution_clock::now();

    // Total duration
    int duration = duration_cast<microseconds>(stop - start).count();
    cout << "Duration: " << duration << " microseconds" << endl;

    return 0;
}

void quicksort(int *A, int lo, int hi)
{
    // Base case
    if (lo >= hi)
        return;

    // Recursive case
    int k = partition(A, lo, hi);
    quicksort(A, lo, k-1);  // Sort left half
    quicksort(A, k+1, hi);  // Sort right half
}

int partition(int *A, int lo, int hi)
{
    // Random pivot element
    int pivot = lo + rand() % (hi - lo);

    // Move pivot aside
    swap(A[lo], A[pivot]);

    // Partition above and below pivot element
    int i = lo + 1, j = hi;
    while (i <= j)
    {
        while (i <= j && A[i] <= A[lo])
            i++;
        while (i <= j && A[j] >= A[lo])
            j--;
        if (i < j)
            swap(A[i++], A[j--]);
    }

    // Replace pivot
    swap(A[lo], A[j]);

    return j;
}