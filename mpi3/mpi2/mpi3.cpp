
#include <iostream>
#include <vector>
#include <algorithm>
#include <mpi.h>
#include <cstdlib> // Для функции rand()
#include <ctime>   // Для функции time()

// Функция для сортировки массива
void sortSubarrays(std::vector<unsigned>& array, int M) 
{
    for (int i = 0; i < array.size(); i += M) {
        // Ensure we don't go past the end of the array
        int end = std::min(i + M, static_cast<int>(array.size()));
        std::sort(array.begin() + i, array.begin() + end, [](const auto& a, const auto& b) {
            return a > b;
            });
        //std::sort(array.begin() + i, array.begin() + end);
    }
}

// Функция для заполнения массива случайными числами
void fill_array_with_random(std::vector<unsigned>& array, int size) {
    for (int i = 0; i < size; ++i) {
        array[i]= (rand() % 10); 
    }
}


void print_matrix(std::vector<unsigned>& array, int n, int m)
{
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < m; j++)
        {
            std::cout<<array[i*m+j]<<"\t";
        }

        std::cout << "\n";
    }

}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Инициализация генератора случайных чисел
    srand(world_rank);

    // Предположим, что у нас есть N массивов, каждый из которых имеет размер M
    int N = 10; // Количество массивов
    int M = 3; // Размер каждого массива

    // Заполнение массивов случайными числами
    std::vector<unsigned> all_arrays(N*M);

    fill_array_with_random(all_arrays, N*M);

    
    std::vector<int> sendcounts(world_size);

    std::vector<int> displs(world_size);

    int residue = N % world_size;
    displs[0] = 0;
    for (int i = 0; i < world_size; i++)
    {
        displs[i] = N / world_size;
        if (i && residue)
        {
            displs[i] += 1;
            residue--;
        }

        
        displs[i] *= M;
        sendcounts[i] = displs[i];
    }

    
    for (int i = world_size; i > 1; i--)
    {
        displs[i-1] = displs[i-2];

    }
    displs[0] = 0;

    for (int i = 1; i < world_size; i++)
    {
        displs[i] += displs[i - 1];

    }
    


    if(!world_rank)
    { 
        std::cout <<"BEFORE" << std::endl;

        print_matrix(all_arrays, N, M);
    std::cout << std::endl;
    }


    std::vector<unsigned> local_array(sendcounts[world_rank]);


   

    MPI_Scatterv(all_arrays.data(), sendcounts.data(), displs.data(), MPI_UNSIGNED, local_array.data(), sendcounts[world_rank], MPI_UNSIGNED,     0, MPI_COMM_WORLD);
    
    



    sortSubarrays(local_array, M);

#if 0
    
    for (int num : local_array) {
        std::cout << num<< " " << world_rank << "\t";
    }
    std::cout << std::endl;

    std::cout << std::endl << sendcounts[world_rank]<<"; " << displs[world_rank] << "\n";

#endif
    MPI_Gatherv(local_array.data(), sendcounts[world_rank], MPI_UNSIGNED, all_arrays.data(), sendcounts.data(), displs.data(), MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    if (!world_rank)
    {
        std::cout << "AFTER" << std::endl;

        print_matrix(all_arrays, N, M);

        std::cout << std::endl;
    }


    MPI_Finalize();

    return   0;
}




