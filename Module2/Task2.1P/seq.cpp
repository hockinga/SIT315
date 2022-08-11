#include <iostream>
#include <chrono>
#include <fstream>

#define N 200

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

    // Start time
    auto start = high_resolution_clock::now();

    // Multiply matrices
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Stop time
    auto stop = high_resolution_clock::now();

    // Total duration
    int duration = duration_cast<nanoseconds>(stop - start).count();
    cout << "Duration: " << duration << " nanoseconds" << endl;

    // Write to file
    write("seq.txt", duration, C);

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