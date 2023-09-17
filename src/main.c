#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[])
{
    int sum = 0;
    int private_sum = 0;
    omp_set_num_threads(4); //Установить количество нитей
    #pragma omp parallel shared(sum) private(private_sum) //В этот момент создается 4 нити, переменная num у них будет общей
    {
        printf("%d\n", omp_get_thread_num()); //Вывести номер нити
        sum++; //Функцию кинутся выполнять все нити параллельно
        private_sum++;
    }
    printf("sum: %d\n", sum);
    printf("private_sum: %d\n", private_sum);
}