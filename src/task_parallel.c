#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

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

// Функция для вычисления новых скоростей тел после упругого соударения
void elastic_collision(Body *b1, Body *b2) {
    double v1x, v1y, v2x, v2y;
    double m1 = b1->mass;
    double m2 = b2->mass;
    double r1 = b1->radius;
    double r2 = b2->radius;
    double dx = b2->x - b1->x;
    double dy = b2->y - b1->y;
    double d = sqrt(dx*dx + dy*dy);
    //printf("dx=%lf dy=%lf d=%lf\n");
    double nx = dx / d;
    double ny = dy / d;
    double p = 2 * (b1->vx * nx + b1->vy * ny - b2->vx * nx - b2->vy * ny) / (m1 + m2);
    v1x = b1->vx - p * m2 * nx;
    v1y = b1->vy - p * m2 * ny;
    v2x = b2->vx + p * m1 * nx;
    v2y = b2->vy + p * m1 * ny;
    b1->vx = v1x;
    b1->vy = v1y;
    b2->vx = v2x;
    b2->vy = v2y;
}

// Функция для обновления скоростей и координат тела с учетом гравитации и столкновений
void updateBody(Body *body, Body *others, int numBodies, double dt) {
    double ax = 0.0;
    double ay = 0.0;

    #pragma omp for
    for (int i = 0; i < numBodies; i++) {
        if (body != &others[i]) {
            double force = gravityForce(*body, others[i]);
            double r = distance(*body, others[i]);
            ax += force * (others[i].x - body->x) / (body->mass * r);
            ay += force * (others[i].y - body->y) / (body->mass * r);

            // Обработка столкновения между телами
            if (r <= body->radius + others[i].radius) {
                elastic_collision(body, &others[i]);
            }
        }

        //printf("Threads: %d\n", omp_get_thread_num());
        
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

    if (count <= 10) {
        printf("Initial state:\n");
        for (int i = 0;i<count;i++) {
            printf("Body %d: x=%f, y=%f, vx=%f, vy=%f, mass=%f, radius=%f\n", i+1, 
                bodies[i].x, bodies[i].y, bodies[i].vx, bodies[i].vy, bodies[i].mass, bodies[i].radius);
        }
    }

    printf("Select target body [in range 1 .. %d]: ", count);
    int target; // порядковый номер заданного тела
    if (!(scanf("%d", &target)==1 && target>=1 && target<=count)) {
        printf("Input error\n");
        return 0;
    }

    int k;  // Время в секундах
    printf("Input time in seconds: ");
    if (!(scanf("%d", &k)==1 && k >= 0)) {
        printf("Input error\n");
        return 0;
    }

    omp_set_num_threads(4);
    printf("Threads: %d\n", omp_get_thread_num());

    double start = omp_get_wtime();

    #pragma omp parallel

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < count; j++) {
            updateBody(&bodies[j], bodies, count, DT);
        }
    }

    double finish = omp_get_wtime();

    // Вывод координат заданного тела после k секунд
    printf("Body %d: x=%f, y=%f\n", target, bodies[target-1].x, bodies[target-1].y);
    printf("Time: %lf", finish - start);

    free(bodies);

    return 0;
}
