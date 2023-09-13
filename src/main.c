#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[])
{
    int nthreads, tid;
    {
        tid = omp_get_thread_num();
        printf("welcome to GFG from thread = %d\n", tid);
        if (tid == 0){
            nthreads = omp_get_num_threads();
            printf("number of threads = %d\n", nthreads);
        }
    }
}