#include <stdio.h>
#include <stdlib.h>

#define INITIAL_POINTS 10

typedef struct {
    int *x;
    int *y;
    int currentArraySize;
    int pointer;
} Points;

int calculateOctant(Points *points, int initX, int initY, int radius) {
    if (!points || radius <= 0) { return 1; }
    
    int relX = 0;
    int relY = radius;
    int p = 1 - radius; // decision point

    while (relX <= relY) {
        if (points->pointer > points->currentArraySize - 1) {
            int newSize = (points->currentArraySize + INITIAL_POINTS);
            
            int *tempX = realloc(points->x, newSize * sizeof(int));
            if (!tempX) { return 1; }
            points->x = tempX;
            
            int *tempY = realloc(points->y, newSize * sizeof(int));
            if (!tempY) { return 1; }
            points->y = tempY;

            points->currentArraySize = newSize;
        }

        points->x[points->pointer] = relX + initX;
        points->y[points->pointer] = relY + initY;
        relX++;

        if (p < 0) {
            // Y does not change, move east.
            p += 2 * relX + 1; // modify decision point
        } else {
            // Y changes, move southeast.
            relY--;
            p += 2 * (relX - relY) + 1;
        }

        points->pointer++;
    }
    return 0;
}

int main(void) {
    Points *points = malloc(sizeof(Points));
    if (!points) {
        perror("Failed to allocate memory for struct.");
        return 1;
    }

    points -> x = malloc(INITIAL_POINTS * sizeof(int));
    points -> y = malloc(INITIAL_POINTS * sizeof(int));

    if (!points->x || !points->y) {
        free(points->x);
        free(points->y);
        free(points);
        perror("Failed to allocate memory for Points in struct.");
        return 1;
    }

    points->currentArraySize = INITIAL_POINTS;
    points->pointer = 0;

    // Define midpoints of circle and radius.
    int x = 0;
    int y = 0;
    int radius = 90;
    
    if (calculateOctant(points, x, y, radius) == 0) {
        for (int i = 0; i < points->pointer; i++) {
            printf("(%d, %d)\n", points->x[i], points->y[i]);
        }
    } else {
        printf("There was an error when calculating or reallocating memory.");
    }

    free(points->x);
    free(points->y);
    free(points);

    return 0;
}
