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
    double vx_cur;
    double vy_cur;
    double vx_fut;
    double vy_fut;
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
    double nx = dx / d;
    double ny = dy / d;
    double p = 2 * (b1->vx_cur * nx + b1->vy_cur * ny - b2->vx_cur * nx - b2->vy_cur * ny) / (m1 + m2);
    v1x = b1->vx_cur - p * m2 * nx;
    v1y = b1->vy_cur - p * m2 * ny;
    v2x = b2->vx_cur + p * m1 * nx;
    v2y = b2->vy_cur + p * m1 * ny;
    b1->vx_fut = v1x;
    b1->vy_fut = v1y;
}

// Функция для обновления скоростей и координат тела с учетом гравитации и столкновений
void gravity_influence(Body *body, Body *others, int numBodies, double dt) {
    double ax = 0.0;
    double ay = 0.0;

    int i;
    for (i = 0; i < numBodies; i++) {
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
    }

    body->vx_fut += ax * dt;
    body->vy_fut += ay * dt;
}

void update(Body* body, double dt) {
    body->vx_cur = body->vx_fut;
    body->vy_cur = body->vy_fut;

    body->x += body->vx_cur * dt;
    body->y += body->vx_cur * dt;
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
    int i;
    for (i = 0; !feof(file); i++)
    {
        fread(&bodies[i], sizeof(Body),1, file);
    }

    if (count <= 10) {
        printf("Initial state:\n");
        int i;
        for (i = 0;i<count;i++) {
            printf("Body %d: x=%.2f, y=%.2f, vx=%.2f, vy=%.2f, mass=%.2f, radius=%.2f\n", i+1, 
                bodies[i].x, bodies[i].y, bodies[i].vx_cur, bodies[i].vy_cur, bodies[i].mass, bodies[i].radius);
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

    double start = omp_get_wtime();

    for (i = 0; i < k / DT; i++) {
        int j;
        for (j = 0; j < count; j++) {
            gravity_influence(&bodies[j], bodies, count, DT);
        }
        for (j = 0; j < count; j++) {
            update(&bodies[j], DT);
        }
    }

    double finish = omp_get_wtime();

    // Вывод координат заданного тела после k секунд
    printf("Body %d: x=%lf, y=%lf\n", target, bodies[target-1].x, bodies[target-1].y);
    printf("Time: %lf\n", finish - start);

    free(bodies);

    return 0;
}
