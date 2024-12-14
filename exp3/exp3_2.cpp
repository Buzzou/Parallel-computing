#include <mpi.h>
#include "iostream"

using namespace std;

int main(int argc, char* argv[])
{
    int myrank, size;
    string message1, message2, message3;
    MPI_Status status;

    //初始化MPI环境
    MPI_Init(&argc, &argv);

    //标识相应进程组中有多少个进程
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //用来标识各个MPI进程，给出调用该函数的进程的进程号
    //参数1：MPI_Comm类型的通信域，标识参与计算的MPI进程组
    //参数2：&rank返回调用进程中的标识号
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    cout << "process " << myrank << " of " << size << " is alive " << endl;

    // 确保所有进程在 MPI_Barrier 前的代码执行完成后，才会继续执行屏障后的代码
    MPI_Barrier(MPI_COMM_WORLD);

    if (0 == myrank)
    {
        message1 = "Hello, process 1";
        MPI_Send(&message1, 20, MPI_CHAR, 1, 99, MPI_COMM_WORLD);
        message2 = "Hello, process 2";
        MPI_Send(&message2, 20, MPI_CHAR, 2, 99, MPI_COMM_WORLD);
        message3 = "Hello, process 3";
        MPI_Send(&message3, 20, MPI_CHAR, 3, 99, MPI_COMM_WORLD);
    }
    else if (myrank == 1)
    {
        MPI_Recv(&message1, 20, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
        cout << message1 << endl;
    }
    else if (myrank == 2)
    {
        MPI_Recv(&message2, 20, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
        cout << message2 << endl;
    }
    else if (myrank == 3)
    {
        MPI_Recv(&message3, 20, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
        cout << message3 << endl;
    }

    //结束MPI执行环境
    MPI_Finalize();

    return 0;
}
/**
*process 3 of 6 is alive
process 2 of 6 is alive
process 1 of 6 is alive
process 4 of 6 is alive
process 0 of 6 is alive
process 5 of 6 is alive
Hello, process 2
Hello, process 3
Hello, process 1
*/
