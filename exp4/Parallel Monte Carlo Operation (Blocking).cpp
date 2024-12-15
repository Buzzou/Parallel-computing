#include <iostream>
#include <random>
#include <cmath>
#include <mpi.h>

#define SEED 35791246

int main(int argc, char* argv[])
{
    const uint64_t niter = 1000000; // 「每个进程」的迭代次数（投点数）
    int myid, nodenum; // 当前进程的 ID 和总进程数
    uint64_t count = 0; // 当前进程内落在圆内的点数
    double pi = 0.0; // π 值估算结果
    double tic, toc; // 计时器变量

    // 初始化 MPI 环境
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &nodenum);

    // 随机数生成器
    std::mt19937_64 rng(SEED + myid); // 使用 Mersenne Twister 64 位引擎，确保每个进程种子不同
    std::uniform_real_distribution<double> dist(0.0, 1.0); // 均匀分布 [0, 1)

    // 记录起始时间
    tic = MPI_Wtime();

    // 非主进程执行蒙特卡洛计算
    if (myid != 0)
    {
        for (uint64_t i = 0; i < niter; ++i)
        {
            double x = dist(rng); // 随机生成 x 坐标
            double y = dist(rng); // 随机生成 y 坐标
            double z = x * x + y * y; // 判断是否落在单位圆内
            if (z <= 1.0)
            {
                ++count; // 落在圆内的点数累加
            }
        }
        // 将结果发送给主进程
        MPI_Send(&count, 1, MPI_INT64_T, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        // 主进程负责收集和计算结果
        uint64_t global_count = 0; // 全局落圆点数
        uint64_t global_niter = niter * (nodenum - 1); // 总投点数（不包括主进程）

        for (int source = 1; source < nodenum; ++source)
        {
            uint64_t temp_count;
            MPI_Recv(&temp_count, 1, MPI_INT64_T, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_count += temp_count;
        }

        // 根据公式计算 π 的值
        pi = (4.0 * (double) global_count) / (double) global_niter;

        // 记录结束时间
        toc = MPI_Wtime();

        // 输出结果
        std::cout << "进程数: " << nodenum << '\n';
        std::cout << "Pi: " << pi << '\n';
        std::cout << "误差: " << std::abs(M_PI - pi) << '\n'; // M_PI 是 <cmath> 提供的 π 的高精度常量
        std::cout << "计算时间: " << toc - tic << " 秒\n";
    }

    // 结束 MPI 环境
    MPI_Finalize();
    return 0;
}
/**
*进程数: 6
Pi: 3.14126
误差: 0.000333454
计算时间: 0.123828 秒
*/
