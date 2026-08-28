#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ROWS 4
#define MAX_COLS 6

/* ---------- Input Handling ---------- */

void clearInput()
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int getNumber(const char message[], int min, int max)
{
    int value;

    while (1)
    {
        printf("%s", message);

        if (scanf("%d", &value) == 1 &&
            value >= min && value <= max)
        {
            clearInput();
            return value;
        }

        printf("Invalid input. Please enter a number between %d and %d.\n",
               min, max);

        clearInput();
    }
}

/* ---------- Instructions ---------- */

void showInstructions()
{
    printf("\n========================================\n");
    printf("              HOW TO PLAY\n");
    printf("========================================\n");
    printf("1. Choose a difficulty level.\n");
    printf("2. The cards are randomly arranged.\n");
    printf("3. Select two cards using row and column.\n");
    printf("4. Matching cards remain visible.\n");
    printf("5. Non-matching cards are hidden again.\n");
    printf("6. Find all pairs to finish the game.\n");
    printf("7. Try to get the highest score with\n");
    printf("   the fewest attempts.\n");
    printf("========================================\n");
}

/* ---------- Shuffle Cards ---------- */

void shuffleBoard(int board[MAX_ROWS][MAX_COLS],
                  int rows,
                  int cols)
{
    int total = rows * cols;

    for (int i = total - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        int r1 = i / cols;
        int c1 = i % cols;

        int r2 = j / cols;
        int c2 = j % cols;

        int temp = board[r1][c1];

        board[r1][c1] = board[r2][c2];
        board[r2][c2] = temp;
    }
}

/* ---------- Display Board ---------- */

void displayBoard(int board[MAX_ROWS][MAX_COLS],
                  int matched[MAX_ROWS][MAX_COLS],
                  int rows,
                  int cols,
                  int firstRow,
                  int firstCol,
                  int secondRow,
                  int secondCol)
{
    printf("\n       ");

    for (int j = 0; j < cols; j++)
    {
        printf("%3d ", j + 1);
    }

    printf("\n");

    printf("     +");

    for (int j = 0; j < cols; j++)
    {
        printf("----+");
    }

    printf("\n");

    for (int i = 0; i < rows; i++)
    {
        printf(" %3d |", i + 1);

        for (int j = 0; j < cols; j++)
        {
            if (matched[i][j] ||
                (i == firstRow && j == firstCol) ||
                (i == secondRow && j == secondCol))
            {
                printf(" %2d |", board[i][j]);
            }
            else
            {
                printf("  ? |");
            }
        }

        printf("\n");

        printf("     +");

        for (int j = 0; j < cols; j++)
        {
            printf("----+");
        }

        printf("\n");
    }
}

/* ---------- Calculate Score ---------- */

int calculateScore(int totalPairs, int attempts)
{
    int extraAttempts = attempts - totalPairs;

    int score = (totalPairs * 100) -
                (extraAttempts * 10);

    if (score < 0)
    {
        score = 0;
    }

    return score;
}

/* ---------- Play Game ---------- */

void playGame(int rows,
              int cols,
              const char difficulty[])
{
    int board[MAX_ROWS][MAX_COLS] = {0};
    int matched[MAX_ROWS][MAX_COLS] = {0};

    int totalCards = rows * cols;
    int totalPairs = totalCards / 2;

    /* Create pairs */

    int values[24];

    for (int i = 0; i < totalPairs; i++)
    {
        values[i * 2] = i + 1;
        values[i * 2 + 1] = i + 1;
    }

    /* Copy values into board */

    int index = 0;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            board[i][j] = values[index];
            index++;
        }
    }

    /* Randomize cards */

    shuffleBoard(board, rows, cols);

    int pairsFound = 0;
    int attempts = 0;

    printf("\n========================================\n");
    printf("              %s MODE\n", difficulty);
    printf("========================================\n");

    while (pairsFound < totalPairs)
    {
        displayBoard(board,
                     matched,
                     rows,
                     cols,
                     -1,
                     -1,
                     -1,
                     -1);

        printf("\nPairs found : %d/%d\n",
               pairsFound,
               totalPairs);

        printf("Attempts    : %d\n",
               attempts);

        /* First card */

        int row1 = getNumber(
            "\nEnter first card row: ",
            1,
            rows) - 1;

        int col1 = getNumber(
            "Enter first card column: ",
            1,
            cols) - 1;

        if (matched[row1][col1])
        {
            printf("\nThat card is already matched!\n");
            continue;
        }

        /* Show first selected card */

        displayBoard(board,
                     matched,
                     rows,
                     cols,
                     row1,
                     col1,
                     -1,
                     -1);

        /* Second card */

        int row2 = getNumber(
            "\nEnter second card row: ",
            1,
            rows) - 1;

        int col2 = getNumber(
            "Enter second card column: ",
            1,
            cols) - 1;

        /* Same card check */

        if (row1 == row2 && col1 == col2)
        {
            printf("\nYou cannot select the same card twice!\n");
            continue;
        }

        /* Already matched check */

        if (matched[row2][col2])
        {
            printf("\nThat card is already matched!\n");
            continue;
        }

        attempts++;

        /* Show both cards */

        displayBoard(board,
                     matched,
                     rows,
                     cols,
                     row1,
                     col1,
                     row2,
                     col2);

        /* Match checking */

        if (board[row1][col1] == board[row2][col2])
        {
            printf("\n*** MATCH FOUND! ***\n");

            matched[row1][col1] = 1;
            matched[row2][col2] = 1;

            pairsFound++;
        }
        else
        {
            printf("\nNot a match!\n");
        }

        if (pairsFound < totalPairs)
        {
            printf("\nPress ENTER to continue...");
            getchar();
        }
    }

    /* Final score */

    int score = calculateScore(
        totalPairs,
        attempts);

    displayBoard(board,
                 matched,
                 rows,
                 cols,
                 -1,
                 -1,
                 -1,
                 -1);

    printf("\n========================================\n");
    printf("          CONGRATULATIONS!\n");
    printf("========================================\n");

    printf("Difficulty    : %s\n", difficulty);
    printf("Pairs found   : %d/%d\n",
           pairsFound,
           totalPairs);

    printf("Total attempts: %d\n",
           attempts);

    printf("Final score   : %d\n",
           score);

    printf("========================================\n");
}

/* ---------- Main Menu ---------- */

int main()
{
    /* Random seed */

    srand((unsigned int)time(NULL));

    while (1)
    {
        printf("\n\n========================================\n");
        printf("          MEMORY MATCH GAME\n");
        printf("           4TH SEM VERSION\n");
        printf("========================================\n");

        printf("1. Start Game\n");
        printf("2. Instructions\n");
        printf("3. Exit\n");

        printf("========================================\n");

        int choice = getNumber(
            "Enter your choice: ",
            1,
            3);

        /* Start Game */

        if (choice == 1)
        {
            printf("\nChoose Difficulty:\n");

            printf("1. Easy   (3 x 4)\n");
            printf("2. Medium (4 x 4)\n");
            printf("3. Hard   (4 x 6)\n");

            int difficulty = getNumber(
                "Enter difficulty: ",
                1,
                3);

            if (difficulty == 1)
            {
                playGame(
                    3,
                    4,
                    "EASY");
            }
            else if (difficulty == 2)
            {
                playGame(
                    4,
                    4,
                    "MEDIUM");
            }
            else
            {
                playGame(
                    4,
                    6,
                    "HARD");
            }
        }

        /* Instructions */

        else if (choice == 2)
        {
            showInstructions();
        }

        /* Exit */

        else
        {
            printf("\nThanks for playing Memory Match!\n");
            printf("Goodbye!\n");

            break;
        }
    }

    return 0;
}
