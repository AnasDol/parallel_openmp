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

// Функция для вычисления расстояния между двумя телами
double distance(Body a, Body b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

// Функция для вычисления силы гравитационного притяжения между двумя телами
double gravityForce(Body a, Body b) {
    double r = distance(a, b);
    return G * a.mass * b.mass / (r * r);
}

// Функция для обновления скоростей и координат тела с учетом гравитации и столкновений
void updateBody(Body *body, Body *others, int numBodies, double dt) {
    double ax = 0.0;
    double ay = 0.0;

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
    }

    body->vx += ax * dt;
    body->vy += ay * dt;
    body->x += body->vx * dt;
    body->y += body->vy * dt;
}


int main() {
    Body bodies[] = {
        { 0, 0, 2, 0, 1, 1 },
        { 10, 0, -2, 0, 1, 1 },
    };
    int count = sizeof(bodies) / sizeof(Body);
    double dt = 0.01;
    Body *target = &bodies[0];
    int k;  // Время в секундах
    printf("Input time in seconds: ");
    if (scanf("%d", &k)!=1 || k < 0) {
        printf("Input error\n");
        return 0;
    }
    int steps = 100;
    int i;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < count; j++) {
            updateBody(&bodies[j], bodies, count, DT);
        }
    }
    // for (i = 0; i < steps; i++) {
    //     simulate(b, count, dt, target, k);
    // }
    //printf("Target body coordinates at time t=%f:\n", k);
    //printf("x=%f, y=%f, vx=%f, vy=%f\n", target->x, target->y, target->vx, target->vy);
   for (int i = 0; i < count; i++) {
        printf("Body %d: x=%f, y=%f, vx=%f, vy=%f\n", i+1, bodies[i].x, bodies[i].y, bodies[i].vx, bodies[i].vy);
    }
    return 0;
}
