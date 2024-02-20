#include <iostream> 
#include <vector> 
#include <omp.h> 



int main() {
    const int N = 11; // Size of the vectors 
    const int num_threads = 4; // Number of threads to use 



    // Allocate memory for the vectors 
    std::vector<unsigned int> a(N);
    std::vector<unsigned int> b(N);
    std::vector<unsigned int> c(N);



    // Initialize vectors a and b with consecutive integer values 
    for (int i = 0; i < N; ++i) {
        a[i] = i;
        b[i] = i;
    }



    // Set the number of threads to use 
    omp_set_num_threads(num_threads);



    // Compute the vector addition in parallel 
#pragma omp parallel for 
    for (int i = 0; i < N; ++i) {
        c[i] = a[i] + b[i];
    }



    // Print the result 
    for (int i = 0; i < N; ++i) {
        std::cout << "a[" << i << "] + b[" << i << "] = c[" << i << "] = " << c[i] << std::endl;
    }



    return  0;
}