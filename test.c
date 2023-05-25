#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define NUM_STARS 2

typedef struct {
    int x;
    int y;
    int blinkState;
} Star;

void initializeStars(Star stars[], int numStars) {
    srand(time(NULL));

    for (int i = 0; i < numStars; i++) {
        stars[i].x = rand() % 80;
        stars[i].y = rand() % 24;
        stars[i].blinkState = 0;
    }
}

void printStars(Star stars[], int numStars) {
    for (int i = 0; i < numStars; i++) {
        if (stars[i].blinkState == 1) {
            printf("\033[%d;%dH*", stars[i].y, stars[i].x);
        }
        else {
            printf("\033[%d;%dH ", stars[i].y, stars[i].x);
        }
    }

    fflush(stdout);
}

void updateStars(Star stars[], int numStars) {
    for (int i = 0; i < numStars; i++) {
        if (rand() % 2 == 0) {
            stars[i].blinkState = !stars[i].blinkState;
        }
    }
}

int main() {
    Star stars[NUM_STARS];

    initializeStars(stars, NUM_STARS);

    while (1) {
        system("cls");

        updateStars(stars, NUM_STARS);
        printStars(stars, NUM_STARS);

        Sleep(500);
    }

    return 0;
}
