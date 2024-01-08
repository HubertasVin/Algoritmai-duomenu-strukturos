#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 9

void printPuzzle(int puzzle[SIZE][SIZE]);
bool isValid(int puzzle[SIZE][SIZE], int row, int col, int num);
void printPart2(char sequence, int step, int row, int col, int num, FILE *fw);
bool solveSudoku(int puzzle[SIZE][SIZE], int row, int col, int step, FILE *fw);

int main()
{
    int puzzleOriginal[SIZE][SIZE] = {0};
    int puzzle[SIZE][SIZE] = {0};
    FILE *fr = fopen("input.txt", "r");

    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            fscanf(fr, "%d", &puzzleOriginal[i][j]);
            fscanf(fr, "%d", &puzzle[i][j]);
        }
    }

    fclose(fr);

    FILE *fw = fopen("output.txt", "w");

    fprintf(fw, "DALIS 2. Sekimas\n");
    solveSudoku(puzzle, 0, 0, 0, fw);

    fclose(fw);

    return 0;
}

// Utility function to print the puzzle
void printPuzzle(int puzzle[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", puzzle[i][j]);
        }
        printf("\n");
    }
}

// Utility function to check if a number is valid for a given cell
bool isValid(int puzzle[SIZE][SIZE], int row, int col, int num)
{
    // Check row and column
    for (int i = 0; i < SIZE; i++) {
        if (puzzle[row][i] == num || puzzle[i][col] == num) {
            return false;
        }
    }

    // Check 3x3 box
    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;
    for (int i = boxRow; i < boxRow + 3; i++) {
        for (int j = boxCol; j < boxCol + 3; j++) {
            if (puzzle[i][j] == num) {
                return false;
            }
        }
    }

    // Number is valid
    return true;
}

void printPart1(int puzzleOriginal[SIZE][SIZE], int puzzle[SIZE][SIZE])
{
    printf("DALIS 1. Duomenys\n");
    printf("  1.1. Sudoku\n");
    printPuzzle(puzzleOriginal);
}

void printPart2(char sequence, int step, int row, int col, int num, FILE *fw)
{
    // Print protocol entry
    fprintf(fw, "  %d) ", step);
    if (sequence == 's')
    {
        // printf("Step %d: (%d, %d) set to %d\n", step, row, col, num);
        for (int i = 0; i < col; ++i)
        {
            fprintf(fw, "-");
        }
        fprintf(fw, "E%d. Nustatytas. [%d; %d]:=%d\n", row, row, col, num);
    }
    else if (sequence == 'w')
    {
        // printf("Step %d: (%d, %d) set to %d\n", step, row, col, num);

        for (int i = 0; i < col; ++i)
        {
            fprintf(fw, "-");
        }
        fprintf(fw, "E%d. Uzimtas. [%d; %d]\n", row, row, col);
    }
    else if (sequence == 'b')
    {
        for (int i = 0; i < col; ++i)
        {
            fprintf(fw, "-");
        }
        fprintf(fw, "E%d. Backtrack. [%d; %d]\n", row, row, col);
    }
    else
    {
        printf("Nekorektiskas protokolas\n");
    }
    
}

// Main recursive function to solve the puzzle
bool solveSudoku(int puzzle[SIZE][SIZE], int row, int col, int step, FILE *fw)
{
    // Base case: puzzle is solved
    if (row == SIZE) {
        printf("Solution found in %d steps:\n", step);
        printPuzzle(puzzle);
        return true;
    }

    ++step;
    // Check if cell is empty
    if (puzzle[row][col] != 0) {
        // Cell is not empty, move on to next cell
        printPart2('w', step, row, col, 0, fw); // Call to print protocol backtrack
        if (col == SIZE - 1) {
            return solveSudoku(puzzle, row + 1, 0, step, fw);
        } else {
            return solveSudoku(puzzle, row, col + 1, step, fw);
        }
    } else {
        // Cell is empty, try filling with numbers 1-9
        for (int i = 1; i <= 9; i++) {
            if (isValid(puzzle, row, col, i)) {
                puzzle[row][col] = i;
                // printPart2(puzzle, 's', step, row, col, i); // Call to print protocol set

                // Move on to next cell
                if (col == SIZE - 1) {
                    printPart2('b', step, row, col, 0, fw); // Call to print protocol backtrack
                    if (solveSudoku(puzzle, row + 1, 0, step, fw)) {
                        return true;
                    }
                } else {
                    printPart2('s', step, row, col, i, fw); // Call to print protocol backtrack
                    if (solveSudoku(puzzle, row, col + 1, step, fw)) {
                        return true;
                    }
                }
                puzzle[row][col] = 0;
            }
        }
    }

    // Backtrack
    printPart2('b', step, row, col, 0, fw);
    printf("\n\n\n");
    printPuzzle(puzzle);
    // Exhausted all possibilities, backtrack
    return false;
}