#include <omp.h>
#include <iostream>

using namespace std;

const int N = 100000;   // Array length
int tmp_nums[N];        // Holds unsorted numbers
int nums[N];            // Holds sorted numbers

/**
 * @brief Resets the nums array to its previous random numbers
 */
void reset_nums()
{
    for (int i = 0; i < N; i++)
    {
        nums[i] = tmp_nums[i];
    }
}

/**
 * @brief Swaps elements at lo and hi in nums
 * 
 * @param lo First index
 * @param hi Second index
 */
void swap(int lo, int hi)
{
    int tmp = nums[lo];
    nums[lo] = nums[hi];
    nums[hi] = tmp;
}

/**
 * @brief Partitions nums around a random number between two indexes
 * 
 * @param lo First index
 * @param hi Second index
 * @return int Partition index
 */
int partition(int lo, int hi)
{
    int pivot = lo + rand() % (hi - lo);
    swap(lo, pivot);

    int i = lo + 1, j = hi;
    while (i <= j)
    {
        while (i <= j && nums[i] <= nums[lo])
        {
            i++;
        }
        while (i <= j && nums[j] >= nums[lo])
        {
            j--;
        }
        if (i < j)
        {
            swap(i++, j--);
        }
    }

    swap(lo, j);
    return j;
}

/**
 * @brief Performs a sequential quicksort on nums between two indexes
 * 
 * @param lo First index
 * @param hi Second index
 */
void sequential_quicksort(int lo = 0, int hi = N-1)
{
    // Base case
    if (lo >= hi)
    {
        return;
    }

    // Recursive sort
    int k = partition(lo, hi);
    sequential_quicksort(lo, k-1);
    sequential_quicksort(k+1, hi);
}

/**
 * @brief Performs a parallel quicksort on nums between two indexes
 * 
 * @param lo First index
 * @param hi Second index
 */
void parallel_quicksort(int lo = 0, int hi = N-1)
{
    // Base case
    if (lo < hi)
    {
        // Recursive sort
        int k = partition(lo, hi);

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallel_quicksort(lo, k-1);
            }

            #pragma omp section
            {
                parallel_quicksort(k+1, hi);
            }
        }
    }
}

/**
 * @brief Perform a quicksort on nums and return the time taken (nanoseconds)
 * 
 * @tparam func Any function
 * @param fn Function used to perform quicksort
 * @return int Time taken to perform quicksort
 */
template<typename func>
int time_quicksort(func &fn)
{
    auto start = chrono::high_resolution_clock::now();
    fn(0, N-1);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(end - start).count();
}

int main()
{
    // Create array to hold unsorted values
    for (int i = 0; i < N; i++)
    {
        tmp_nums[i] = rand() % 100;
    }

    // Sequential quicksort
    reset_nums();
    int duration = time_quicksort(sequential_quicksort);
    cout << "Sequntial: " << duration << endl;

    // Parallel quicksort
    reset_nums();
    duration = time_quicksort(parallel_quicksort);
    cout << "Parallel:  " << duration << endl;

    return 0;
}