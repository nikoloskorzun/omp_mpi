//mpiexec -n 15 mpi1.exe
#define TIME_OUTPUT 1
//#define UNICAST
//#define BROADCAST
//#define Scatter_Gather
#define All_gather
//#define Alltoall


#include <iostream>
#include <vector>
#include <mpi.h>
#include <ctime>

int main(int argc, char** argv) 
{
#if TIME_OUTPUT == 1
    clock_t start_time = clock();
#endif
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int M = 10; // Размерность вектора





    #ifdef Scatter_Gather
        int recv_count = 2;
        
        M = recv_count * world_size;

        std::vector<int> mini_vector(recv_count);
        // Создаем массив данных для распределения
    #endif

    #ifdef All_gather
        int send_count = 5;

        M = send_count * world_size;

        std::vector<int> mini_vector(send_count);

        std::cout << "Process " << world_rank << " send data: ";

        for (int i = 0; i < send_count; ++i)
        {

            mini_vector[i] = (world_rank + 1) * (i + 1);
            std::cout << mini_vector[i] << " ";

        }
        std::cout << std::endl;

    #endif

#ifdef Alltoall
        int send_count = 2;
        M = send_count * world_size;
        std::vector<int> vector_(M);



        std::cout << "Process " << world_rank << " send data: ";

        for (int i = 0; i < world_size * send_count; ++i) {
            vector_[i] = world_rank * send_count + i * world_rank;
            std::cout << vector_[i] << " ";

        }
        std::cout << std::endl;

#endif



        std::vector<int> vector(M);
    #ifdef Alltoall

        MPI_Alltoall(vector_.data(), send_count, MPI_INT, vector.data(), send_count, MPI_INT, MPI_COMM_WORLD);

        std::cout << "Process " << world_rank << " received data: ";
        for (int i = 0; i < vector.size(); i++)
        {
            std::cout << vector[i] << " ";
        }
        std::cout << std::endl;
    #endif


    #ifdef All_gather
        MPI_Allgather(mini_vector.data(), send_count, MPI_INT, vector.data(), send_count, MPI_INT, MPI_COMM_WORLD);
        std::cout << "Process " << world_rank << " recieve data: ";
        for (int i = 0; i < vector.size(); ++i)
        {
            std::cout << vector[i] << " ";
        }
        std::cout << std::endl;
    #endif
    
    if (!world_rank) 
    {
        #ifdef Scatter_Gather
            std::cout << "Scatter_Gather MPI_Scatter - MPI_Scatter, MPI_Gather - MPI_Gather test.\n\n";
        #endif
        #ifdef BROADCAST
            std::cout << "BROASCAST MPI_Bcast - MPI_Bcast test.\n\n";
        #endif


        #ifdef All_gather
            std::cout << "All_gather MPI_Allgather - MPI_Allgather test.\n\n";

        #endif
        #ifdef Alltoall
            std::cout << "Alltoall MPI_Alltoall - MPI_Alltoall test.\n\n";

        #endif

        // Заполняем вектор в процессе 0
        srand(time(0));
        for (int i = 0; i < M; ++i) 
        {
            vector[i] = rand() % 100;
        }

        #ifdef UNICAST

            std::cout << "UNICAST MPI_Send - MPI_Recv test.\n\n";
            // Отправляем вектор на другие процессы
            for (int dest = 1; dest < world_size; ++dest) 
            {
                vector[0] = dest; //меняем первый элемент для наглядности
                MPI_Send(vector.data(), M, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }
        #endif
    }
    else 
    {
        #ifdef UNICAST
            // Получаем вектор на других процессах
            MPI_Recv(vector.data(), M, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        #endif
    }
    #ifdef Scatter_Gather
        if (!world_rank)
        {
            std::cout << "Process sender: vector: ";

            for (int i = 0; i < M; ++i)
            {
                std::cout << vector[i] << " ";
            }
            std::cout << std::endl;
        }
        MPI_Scatter(vector.data(), recv_count, MPI_INT, mini_vector.data(), recv_count, MPI_INT, 0, MPI_COMM_WORLD);
        std::cout << "Process " << world_rank << " recieved part: ";
        for (int i = 0; i < recv_count; i++) 
        {
            std::cout << mini_vector[i] << " ";
            mini_vector[i] *= -1;
        }
        std::cout << std::endl;

        MPI_Gather(mini_vector.data(), recv_count, MPI_INT, vector.data(), recv_count, MPI_INT, 0, MPI_COMM_WORLD);

        if (!world_rank)
        {
            std::cout << "Process reciever(old sender): vector: ";

            for (int i = 0; i < M; ++i)
            {
                std::cout << vector[i] << " ";
            }
            std::cout << std::endl;
        }


    #endif





    #ifdef BROADCAST
        start_time = clock();

        MPI_Bcast(vector.data(), M, MPI_INT, 0, MPI_COMM_WORLD);
    #endif


    #ifndef Scatter_Gather 
    #ifndef All_gather
    #ifndef Alltoall
        // Выводим элементы вектора и время выполнения
        if (world_rank == 0) 
        {
            std::cout << "Process sender: vector: ";
        
        }
        else 
        {
            std::cout << "Process reciever #" << world_rank << " vector: ";

        }

        for (int i = 0; i < M; ++i)
        {
            std::cout << vector[i] << " ";
        }
        std::cout << std::endl;

    #endif
    #endif
    #endif


    #if TIME_OUTPUT == 1
        clock_t end_time = clock();
        double time_spent = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
        std::cout <<world_rank<<". Execution time: " << time_spent << " seconds" << std::endl;
    #endif
    MPI_Finalize();
    return  0;
}
