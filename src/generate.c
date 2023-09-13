#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
    double vx;
    double vy;
    double mass;
    double radius;
} Body;

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

    Body bodies[2] = {
        {0.0, 0.0, 0.0, 0.0, 0, 10.0},  // Тело 1
        {100.0, 0.0, -1.0, 0.0, 0, 10.0} // Тело 2
    };

    fwrite((void*)&bodies[0], sizeof(Body), 1, file);
    fwrite((void*)&bodies[1], sizeof(Body), 1, file);

    fclose(file);

    

    return 0;
}