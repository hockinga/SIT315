#include <chrono>
#include <fstream>
#include <pthread.h>
#include <math.h>
#include <omp.h>

using namespace std;
using chrono::high_resolution_clock, chrono::duration_cast, chrono::nanoseconds;

const int N = 100;                                  // Matrix size
const int THREADS = 16;                             // Number of threads
const int THREAD_ROWS = ceil((float)N / THREADS);   // Number of rows to compute per thread

int A[N][N], B[N][N], C[N][N];

/**
 * @brief Writes a matrix to a file
 * 
 * @param M Matrix to write
 * @param file Name of the file to write to
 */
void write_matrix(int (&M)[N][N], ofstream &file)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            file << M[i][j] << " ";
        }
        file << endl;
    }
}

/**
 * @brief Writes a matrix product experiment to file
 * 
 * @param duration Time taken to perform AxB
 * @param fname Name of the file to write to
 */
void write(int duration, string fname)
{
    // Create file
    ofstream file(fname);

    // Write Duration, A, B and C to file
    file << "Duration: " << duration << " nanoseconds" << endl;
    file << "\nA =" << endl;
    write_matrix(A, file);
    file << "\nB =" << endl;
    write_matrix(B, file);
    file << "\nC =" << endl;
    write_matrix(C, file);

    // Close file
    file.close();
}

/**
 * @brief Fill a matrix with random integers
 * 
 * @param matrix Matrix to fill with random integers
 */
void rand_matrix(int (&M)[N][N])
{
    // Fill with random numbers from 0-9
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            M[i][j] = (rand() % 10);
        }
    }
}

/**
 * @brief Compute and store row i in C=AxB
 * 
 * @param i Row number
 */
void row_multiply(int i)
{
    for (int j = 0; j < N; j++)
    {
        // Compute entry i, j
        int tmp = 0;
        for (int k = 0; k < N; k++)
        {
            tmp += A[i][k] * B[k][j];
        }
        C[i][j] = tmp;
    }
}

/**
 * @brief Compute and store C=AxB and return the time taken (nanoseconds)
 * 
 * @tparam func Any function
 * @param fn Function used to compute AxB
 * @return int Time taken to compute AxB
 */
template<typename func>
int time_multiply(func &fn)
{
    auto start = high_resolution_clock::now();
    fn();
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end - start).count();
}

/**
 * @brief Compute and store C=AxB sequentially
 */
void sequential_multiply()
{
    for (int i = 0; i < N; i++)
    {
        row_multiply(i);
    }
}

/**
 * @brief Compute and store a nummber of rows in C=AxB
 * 
 * @param arg Thread id
 * @return void* Null
 */
void *pthread_rows(void *arg)
{
    // Rows to compute
    long id = (long) arg;
    int lo = id * THREAD_ROWS;
    int hi = min(lo + THREAD_ROWS, N);

    // Compute rows
    for (int i = lo; i < hi; i++)
    {
        row_multiply(i);
    }

    pthread_exit(NULL);
}

/**
 * @brief Compute and store C=AxB using pthread
 */
void pthread_multiply()
{
    int thread_rows = N / THREADS;
    pthread_t threads[THREADS];

    // Create threads
    for (long i = 0; i < THREADS; i++)
    {
        pthread_create(&threads[i], NULL, pthread_rows, (void *)i);
    }

    // Merge with main thread
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

/**
 * @brief Compute and store C=AxB using OpenMP
 */
void openmp_multiply()
{
    omp_set_num_threads(THREADS);
    #pragma omp parallel
    {
        // Rows to compute
        int id = omp_get_thread_num();
        int lo = id * THREAD_ROWS;
        int hi = min(lo + THREAD_ROWS, N);

        // Compute rows
        for (int i = lo; i < hi; i++)
        {
            row_multiply(i);
        }
    }
}

int main()
{
    // Randomly fill A and B
    rand_matrix(A);
    rand_matrix(B);

    // Store time taken to multiply
    int duration;

    // Sequential multiplication
    duration = time_multiply(sequential_multiply);
    write(duration, "sequential.txt");

    // P Thread multiplication
    duration = time_multiply(pthread_multiply);
    write(duration, "pthread.txt");

    // OpenMP multiplication
    duration = time_multiply(openmp_multiply);
    write(duration, "openmp.txt");

    return 0;
}