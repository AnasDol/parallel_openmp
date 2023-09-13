#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define G 6.67430e-11 // гравитационная постоянная
#define DT 1.0        // шаг интегрирования (в секундах)

typedef struct {
    double x;
    double y;
    double vx;
    double vy;
    double mass;
    double radius;
} Body;

// Функция для вычисления расстояния между двумя телами
double distance(Body a, Body b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

// Функция для вычисления силы гравитационного притяжения между двумя телами
double gravityForce(Body a, Body b) {
    double r = distance(a, b);
    return G * a.mass * b.mass / (r * r);
}

// Функция для обновления скоростей и координат тела с учетом гравитации
void updateBody(Body* body, Body* others, int numBodies, double dt) {
    double ax = 0.0;
    double ay = 0.0;

    for (int i = 0; i < numBodies; i++) {
        if (body != &others[i]) {
            double force = gravityForce(*body, others[i]);
            double r = distance(*body, others[i]);
            ax += force * (others[i].x - body->x) / (body->mass * r);
            ay += force * (others[i].y - body->y) / (body->mass * r);
        }
    }

    body->vx += ax * dt;
    body->vy += ay * dt;
    body->x += body->vx * dt;
    body->y += body->vy * dt;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Enter filename as command prompt argument\n");
        return 0;
    }

    char filename[255];
    strncpy(filename, argv[1], 255);

    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("File not found\n");
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long count = ftell(file) / sizeof(Body);

    Body* bodies = (Body*)malloc(count * sizeof(Body));

    fseek(file, 0, SEEK_SET);
    for (int i = 0; !feof(file); i++)
    {
        fread(&bodies[i], sizeof(Body),1, file);
    }

    int k;  // Время в секундах
    printf("Input time in seconds: ");
    if (scanf("%d", &k)!=1 || k < 0) {
        printf("Input error\n");
        return 0;
    }

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < count; j++) {
            updateBody(&bodies[j], bodies, count, DT);
        }
    }

    // Вывод координат тел после k секунд
    for (int i = 0; i < count; i++) {
        printf("Body %d: x=%f, y=%f\n", i+1, bodies[i].x, bodies[i].y);
    }

    free(bodies);

    return 0;
}