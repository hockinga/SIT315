#include <iostream>
#include <chrono>
#include <fstream>
#include <pthread.h>

#define N 200
#define THREADS 4

using namespace std;
using namespace chrono;

void fill_matrix(int (&A)[N][N], int (&B)[N][N], int (&C)[N][N], unsigned int seed);
void write(string fname, int duration, int (&C)[N][N]);

// Struct to pass into threads
struct Task
{
    int id;
    int (*A)[N], (*B)[N], (*C)[N];
};

void *arr_multiply(void *args)
{
    // Cast args to task
    Task *task = ((struct Task *)args);

    // Complete multiplication for N/THREADS rows
    for (int i = task->id; i < N; i += THREADS)
    {
        for (int j = 0; j < N; j++)
        {
            // Set entry C[i][j]
            int tmp = 0;
            for (int k = 0; k < N; k++)
            {
                tmp += task->A[i][k] * task->B[k][j];
            }
            task->C[i][j] = tmp;
        }
    }
}

int main()
{
    // Initialise A, B and C
    int A[N][N], B[N][N], C[N][N];

    // Fill initial values
    fill_matrix(A, B, C, 0);

    // Create threads
    pthread_t threads[THREADS];

    // Start time
    auto start = high_resolution_clock::now();

    // Multiply matrices
    for (int i = 0; i < THREADS; i++)
    {
        // Create struct to pass to thread
        struct Task *task = (struct Task *)malloc(sizeof(struct Task));
        task->id = i;
        task->A = A;
        task->B = B;
        task->C = C;

        // Begin thread
        pthread_create(&threads[i], NULL, arr_multiply, (void *)task);
    }

    // Wait for all threads to finish
    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    // Stop time
    auto stop = high_resolution_clock::now();

    // Total duration
    int duration = duration_cast<nanoseconds>(stop - start).count();
    cout << "Duration: " << duration << " nanoseconds" << endl;

    // Write to file
    write("pth.txt", duration, C);

    return 0;
}

void fill_matrix(int (&A)[N][N], int (&B)[N][N], int (&C)[N][N], unsigned int seed)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            A[i][j] = rand_r(&seed) % 5;
            B[i][j] = rand_r(&seed) % 5;
            C[i][j] = 0;
        }
}

void write(string fname, int duration, int (&C)[N][N])
{
    // Create file
    ofstream stream(fname);

    // Write duration
    stream << "Duration: " << duration << " nanoseconds" << endl;

    // Write matrix
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            stream << C[i][j] << ' ';
        }
        stream << endl;
    }

    // Close file
    stream.close();
}