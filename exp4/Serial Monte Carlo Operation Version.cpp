//
// Created by RuoHao Li on 6/17/21.
//

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>

double monteCarloPi(size_t niter) {
    // 使用 C++11 随机数生成器
    std::random_device rd; // random_device 生成真正的随机数（若硬件支持）或者伪随机数（当硬件不支持时）
    std::mt19937 gen(rd()); // rd() 是一个随机数，用于初始化其他随机数生成器；gen() 生成一系列伪随机数
    std::uniform_real_distribution<> dis(0.0, 1.0); // 在 [0.0, 1.0] 间生成随机数

    size_t count = 0;
    for (size_t i = 0; i < niter; ++i) {
        // 每次调用 dis(gen) 时，gen 生成一个伪随机数，dis 将这个伪随机数映射到 [0.0, 1.0] 范围内，返回一个浮点数
        double x = dis(gen);
        double y = dis(gen);
        // 计算点 (x, y) 到原点的距离
        if ((x * x + y * y) <= 1.0) {
            ++count;
        }
    }
    return 4.0 * static_cast<double>(count) / static_cast<double>(niter);
}

/**
* 这段代码实现了通过蒙特卡洛法估算 π 的值，其基本原理是：
* 在单位正方形内随机生成点，检查这些点是否落在单位圆内；
* 圆的面积为 πr^2，正方形面积为 4r^2，所以单位圆与单位正方形的面积比为 π/4；
* 当随机点的数量足够大时，圆内点占总点的比例趋近于 π/4 ，由此可以计算出 π
*/
int main() {
    const size_t niter = 10000000; // 迭代次数
    const double pi_reference = 3.14159265358979323846; // π 的参考值

    // 记录开始时间
    auto start_time = std::chrono::high_resolution_clock::now();

    // 计算 π 值
    double pi = monteCarloPi(niter);

    // 记录结束时间
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;

    // 输出结果
    std::cout << std::fixed << std::setprecision(15);
    std::cout << "Estimated Pi: " << pi << std::endl;
    std::cout << "Reference Pi: " << pi_reference << std::endl;
    std::cout << "Absolute Error: " << std::fabs(pi - pi_reference) << std::endl;
    std::cout << "Elapsed Time: " << elapsed_time.count() << " seconds" << std::endl;

    return 0;
}
/**
*Estimated Pi: 3.141502800000000
Reference Pi: 3.141592653589793
Absolute Error: 0.000089853589793
Elapsed Time: 1.419414488000000 seconds
*/
