#include <mpi.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

double f(double x)
{
    return std::cos(x);
}

/**
* 将积分区间 [−1,1] 划分为多段，每个进程负责计算自己负责区间的积分值
* 最后一个进程负责汇总其他进程的计算结果，并输出最终积分值、误差和运行时间
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[])
{
    double start = -1.0, end = 1.0; // 积分区间上下限
    int n = 100;                   // 每个进程分成的小区间数
    double w;                      // 每个区间宽度
    int myrank, size;              // 进程编号和进程总数

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // 区间被划分为 size× n 个小区间，每个小区间宽度为w
    // 每个进程负责计算其中的 n 个区间
    w = (end - start) / (size * n); // 每个小区间宽度

    // 开始计时
    auto start_time = std::chrono::high_resolution_clock::now();

    // 当前进程计算的积分值
    double local_integral = 0.0;
    for (int i = 0; i < n; ++i)
    {
        double midpoint = start + (myrank * n + i + 0.5) * w;
        local_integral += f(midpoint) * w;
    }

    // 汇总结果
    double total_integral = 0.0;
    if (myrank == size - 1) // 最后一个进程负责汇总
    {
        total_integral = local_integral;
        for (int i = 0; i < size - 1; ++i)
        {
            double temp;
            MPI_Recv(&temp, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_integral += temp;
        }

        // 结束计时
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;

        // 输出结果
        double exact_value = 1.6829419696158; // 精确积分值
        std::cout << "计算进程数: " << size << "\n"
                  << "积分值: " << total_integral << "\n"
                  << "误差: " << std::fabs(exact_value - total_integral) << "\n"
                  << "耗时: " << elapsed.count() << " 秒\n";
    }
    else // 其他进程发送结果
    {
        MPI_Send(&local_integral, 1, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
/**
 *计算进程数: 6
积分值: 1.68294
误差: 7.7914e-07
耗时: 0.000471397 秒
*/
