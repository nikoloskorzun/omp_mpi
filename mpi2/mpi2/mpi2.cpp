#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define VECTOR_SIZE 30

int main(int argc, char* argv[]) 
{
    int rank, size;
    short int vector[VECTOR_SIZE];
    short int total_sum[VECTOR_SIZE] = { 0 };
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(rank);

    // Генерация вектора в каждом процессе
    for (int i = 0; i < VECTOR_SIZE; i++) 
    {
        vector[i] = rand() % 100 - 50; // Генерация случайного числа от -50 до   50
    }


    printf("Vector in %d process:\t", rank);

    for (int i = 0; i < VECTOR_SIZE; i++)
    {
        printf("%d\t", vector[i]);
    }
    printf("\n");

    // Использование MPI_Reduce для поэлементной суммы
    MPI_Reduce(vector, total_sum, VECTOR_SIZE, MPI_SHORT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Вывод итоговой суммы в процессе с рангом 0
    if (rank == 0) 
    {
        printf("Total sum:\t\t");
        
        for (int i = 0; i < VECTOR_SIZE; i++) 
        {
            printf("%d\t", total_sum[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
