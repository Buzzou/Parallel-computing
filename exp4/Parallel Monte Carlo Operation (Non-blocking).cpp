#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <mpi.h>

constexpr int SEED = 35791246;

/**
* 非阻塞函数允许程序在通信时继续执行其他任务，而无需等待传输完成，提高性能（异步）
* 代价是需要手动管理通信请求和状态
*/
int main(int argc, char* argv[])
{
    const unsigned long niter = 1000000;
    int myid, nodenum;
    double tic, toc;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &nodenum);

    std::mt19937 gen(SEED + myid); // Mersenne Twister generator for reproducible results
    std::uniform_real_distribution<> dis(0.0, 1.0);

    uint count = 0;
    if (myid != 0)
    {
        // 投点计算落在圆内的次数
        for (unsigned long i = 0; i < niter; ++i)
        {
            double x = dis(gen);
            double y = dis(gen);
            if (x * x + y * y <= 1.0)
            {
                ++count;
            }
        }

        // 非阻塞发送数据 - 调用后函数会立即返回，而不等待通信操作完成；数据的实际传输会在后台进行
        MPI_Request reqs[2];
        MPI_Isend(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &reqs[0]);
        MPI_Isend(&niter, 1, MPI_LONG, 0, 2, MPI_COMM_WORLD, &reqs[1]);
        MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE); // 用来等待所有非阻塞发送完成
    }
    else
    {
        // 0号节点负责接收数据 - 使用 std::vector 动态管理从节点接收的数据，避免了手动管理数组的复杂性
        std::vector<int> received_counts(nodenum);
        std::vector<long> received_niters(nodenum);

        MPI_Request reqs[2 * (nodenum - 1)];
        for (int i = 1; i < nodenum; ++i)
        {
            MPI_Irecv(&received_counts[i], 1, MPI_INT, i, 1, MPI_COMM_WORLD, &reqs[2 * (i - 1)]);
            MPI_Irecv(&received_niters[i], 1, MPI_LONG, i, 2, MPI_COMM_WORLD, &reqs[2 * (i - 1) + 1]);
        }

        MPI_Waitall(2 * (nodenum - 1), reqs, MPI_STATUSES_IGNORE);

        // 汇总计算 π 值
        int final_count = 0;
        long final_niter = 0;
        for (int i = 1; i < nodenum; ++i)
        {
            final_count += received_counts[i];
            final_niter += received_niters[i];
        }

        double pi = (static_cast<double>(final_count) / final_niter) * 4.0;

        toc = MPI_Wtime();
        std::cout << "Pi: " << pi << "\n";
        std::cout << "误差: " << std::fabs(M_PI - pi) << "\n";
        std::cout << "所用时间: " << toc - tic << " 秒\n";
    }

    MPI_Finalize();
    return 0;
}
/**
*Pi: 3.14228
误差: 0.000684146
所用时间: 0 秒
*/
