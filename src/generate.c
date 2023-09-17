#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define count 1000

typedef struct {
    double x;
    double y;
    double vx;
    double vy;
    double mass;
    double radius;
} Body;

double drand ( double low, double high )
{
    return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Enter filename as command prompt argument\n");
        return 0;
    }

    char filename[255];
    strncpy(filename, argv[1], 255);

    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error\n");
        return 0;
    }

    /*Body bodies[2] = {
        {0.0, 0.0, 0.0, 0.0, 1.0e12, 10.0},  // Тело 1
        {100.0, 0.0, -1.0, 0.0, 1.0e12, 10.0} // Тело 2
    };*/

    Body bodies[count];
    for (int i = 0;i<count;i++) {
        bodies[i].x = drand(-100,100);
        bodies[i].y = drand(-100,100);
        bodies[i].vx = drand(-10,10);
        bodies[i].vy = drand(-10,10);
        bodies[i].mass = drand(1,1.0e12);
        bodies[i].radius = drand(1,50);
    }

    for (int i = 0;i<count;i++) {
        fwrite((void*)&bodies[i], sizeof(Body), 1, file);
    }

    fclose(file);

    

    return 0;
}