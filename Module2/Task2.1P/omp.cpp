#include <iostream>
#include <chrono>
#include <fstream>
#include <omp.h>

#define N 200
#define THREADS 4

using namespace std;
using namespace chrono;

void fill_matrix(int (&A)[N][N], int (&B)[N][N], int (&C)[N][N], unsigned int seed);
void write(string fname, int duration, int (&C)[N][N]);

int main()
{
    // Initialise A, B and C
    int A[N][N], B[N][N], C[N][N];

    // Fill initial values
    fill_matrix(A, B, C, 0);

    // Setup Open MP
    omp_set_num_threads(THREADS);

    // Start time
    auto start = high_resolution_clock::now();

    // Multiply matrices
    #pragma omp parallel
    {
        // Reduce shared updates to C by using a private variable
        int tmp;

        #pragma omp for
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                // Store C[i][j] in private variable to reduce number of shared updates
                tmp = 0;
                for (int k = 0; k < N; k++)
                {
                    tmp += A[i][k] * B[k][j];
                }

                // Merge update for each entry
                #pragma omp critical
                C[i][j] += tmp;
            }
        }
    }

    // Stop time
    auto stop = high_resolution_clock::now();

    // Total duration
    int duration = duration_cast<nanoseconds>(stop - start).count();
    cout << "Duration: " << duration << " nanoseconds" << endl;

    // Write to file
    write("omp.txt", duration, C);

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