#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <mpi.h>

int main(int argc, char *argv[]) {
    const int niter = 100000; // Number of iterations per process
    int myid;                // Holds process's rank id
    int count = 0;           // Local count of points inside the circle
    double x, y, z;          // Coordinates and distance to the origin
    int reducedcount = 0;    // Total number of "good" points across all processes
    int reducedniter = 0;    // Total number of points across all processes
    double pi;               // Approximation of Pi
    double tic, toc;         // Timing variables

    MPI_Init(&argc, &argv);                  // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);    // Get rank of the current process

    // Start timing
    tic = MPI_Wtime();

    // 每个进程都需要做的 - Generate random points and count the ones inside the circle
    // 每个进程（包括 myid == 0 的主进程）都有自己的 niter 和 count，并独立完成抛点的计算
    srand48(time(nullptr) + myid); // Seed the random generator with unique seed per process
    for (int i = 0; i < niter; ++i) {
        x = drand48(); // Generate random x coordinate in [0, 1]
        y = drand48(); // Generate random y coordinate in [0, 1]
        z = std::sqrt(x * x + y * y); // Check if the point is inside the unit circle
        if (z <= 1) {
            ++count;
        }
    }

    // Perform reduction to sum counts and iterations across all processes
    MPI_Reduce(&count, &reducedcount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&niter, &reducedniter, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Compute and print results in the root process
    if (myid == 0) {
        pi = static_cast<double>(reducedcount) / reducedniter * 4.0;
        toc = MPI_Wtime();
        std::cout << "Estimated Pi: " << pi << "\n"
                  << "Reduced Count: " << reducedcount << "\n"
                  << "Reduced niter: " << reducedniter << "\n"
                  << "Error: " << std::fabs(M_PI - pi) << "\n"
                  << "Execution Time: " << toc - tic << " seconds" << std::endl;
    }

    MPI_Finalize(); // Finalize MPI
    return 0;
}
/**
*Estimated Pi: 3.13851
Reduced Count: 470777
Reduced niter: 600000
Error: 0.00307932
Execution Time: 0.003251 seconds
*/
