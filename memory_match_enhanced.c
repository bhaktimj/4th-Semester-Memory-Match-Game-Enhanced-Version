#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 4

void shuffle(char board[SIZE][SIZE]) {
    char values[SIZE * SIZE];
    int k = 0;

    for (char c = 'A'; c < 'A' + (SIZE * SIZE) / 2; c++) {
        values[k++] = c;
        values[k++] = c;
    }

    for (int i = SIZE * SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = values[i];
        values[i] = values[j];
        values[j] = temp;
    }

    k = 0;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = values[k++];
}

void display(char board[SIZE][SIZE], int found[SIZE][SIZE]) {
    printf("\n    1 2 3 4\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d | ", i + 1);
        for (int j = 0; j < SIZE; j++)
            printf("%c ", found[i][j] ? board[i][j] : '*');
        printf("\n");
    }
}

int main(void) {
    srand((unsigned)time(NULL));

    char board[SIZE][SIZE];
    int found[SIZE][SIZE] = {0};
    int pairs = 0, score = 0, attempts = 0;
    int maxAttempts = 30;

    printf("=== MEMORY MATCH GAME: ENHANCED ===\n");
    printf("Difficulty: 1-Easy  2-Normal  3-Hard\n");

    int difficulty;
    scanf("%d", &difficulty);

    if (difficulty == 1) maxAttempts = 40;
    else if (difficulty == 3) maxAttempts = 20;

    shuffle(board);

    while (pairs < (SIZE * SIZE) / 2 && attempts < maxAttempts) {
        display(board, found);

        int r1, c1, r2, c2;
        printf("\nChoose first card (row col): ");
        scanf("%d %d", &r1, &c1);
        printf("Choose second card (row col): ");
        scanf("%d %d", &r2, &c2);

        r1--; c1--; r2--; c2--;

        if (r1 < 0 || r1 >= SIZE || c1 < 0 || c1 >= SIZE ||
            r2 < 0 || r2 >= SIZE || c2 < 0 || c2 >= SIZE ||
            found[r1][c1] || found[r2][c2] ||
            (r1 == r2 && c1 == c2)) {
            printf("Invalid selection. Try again.\n");
            continue;
        }

        attempts++;

        printf("Cards: %c and %c\n", board[r1][c1], board[r2][c2]);

        if (board[r1][c1] == board[r2][c2]) {
            found[r1][c1] = found[r2][c2] = 1;
            pairs++;
            score += 10;
            printf("Match found! +10 points.\n");
        } else {
            score -= 1;
            printf("Not a match. -1 point.\n");
        }
    }

    display(board, found);

    if (pairs == (SIZE * SIZE) / 2)
        printf("\nCongratulations! You completed the game.\n");
    else
        printf("\nGame over. Better luck next time!\n");

    printf("Final score: %d\n", score);
    printf("Attempts: %d\n", attempts);

    return 0;
}
