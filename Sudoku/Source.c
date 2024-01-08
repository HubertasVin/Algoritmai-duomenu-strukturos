#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 9

int step = 0;
int deepness = 0;
int wordWrap = 0;

void printPuzzle(int puzzle[SIZE][SIZE], FILE *fw);
bool isPuzzleSolvable(int puzzle[SIZE][SIZE]);
bool isValid(int puzzle[SIZE][SIZE], int row, int col, int num);
void printPart1(int puzzle[SIZE][SIZE], FILE *fw);
void printPart2(char sequence, bool isNewLine, int row, int col, int num, FILE *fw);
void printPart3(int puzzle[SIZE][SIZE], FILE *fw);
bool solveSudoku(int puzzle[SIZE][SIZE], int row, int col, FILE *fw);

int main()
{
    int puzzle[SIZE][SIZE] = {0};
    char inputFileName[100];

    printf("Iveskite ivesties failo pavadinima su jo pletiniu: ");
    scanf("%s", inputFileName);
    FILE *fr = fopen(inputFileName, "r");
    if (fr == NULL)
    {
        printf("Nepavyko atidaryti failo.\n");
        return 1;
    }

    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            fscanf(fr, "%d", &puzzle[i][j]);
        }
    }

    fclose(fr);

    FILE *fw = fopen("2-Uzd-08-Var-Protokolas-Vindzigalskis-20230419.txt", "w");

    // fprintf(fw, "Hubertas Vindzigalskis. 3 grupe, 2 pogrupis.\n");
    printf("Hubertas Vindzigalskis. 3 grupe, 2 pogrupis.\n");
    // fprintf(fw, "Salyga: 8. Parasyti programa, sprendziancia Sudoku.\n");
    printf("Salyga: 8. Parasyti programa, sprendziancia Sudoku.\n");
    printPart1(puzzle, fw);

    fprintf(fw, "\nDALIS 2. Vykdymas\n");

    if (!isPuzzleSolvable(puzzle))
    {
        fprintf(fw, "Ivestis yra nekorektiska, neimanoma isspresti sudoku.\n");
        printf("Ivestis yra nekorektiska, neimanoma isspresti sudoku.\n");
    }
    else
    {
        printPart3(puzzle, fw);
    }

    fclose(fw);

    return 0;
}

// write a function to check if the input sudoku puzzle is solvable
bool isPuzzleSolvable(int puzzle[SIZE][SIZE])
{
    // Check rows
    for (int i = 0; i < SIZE; i++) {
        bool used[SIZE + 1] = {false};
        for (int j = 0; j < SIZE; j++) {
            if (puzzle[i][j] != 0 && used[puzzle[i][j]]) {
                return false;
            }
            used[puzzle[i][j]] = true;
        }
    }
    
    // Check columns
    for (int j = 0; j < SIZE; j++) {
        bool used[SIZE + 1] = {false};
        for (int i = 0; i < SIZE; i++) {
            if (puzzle[i][j] != 0 && used[puzzle[i][j]]) {
                return false;
            }
            used[puzzle[i][j]] = true;
        }
    }
    
    // Check sub-grids
    for (int k = 0; k < SIZE; k++) {
        bool used[SIZE + 1] = {false};
        int row = (k / 3) * 3;
        int col = (k % 3) * 3;
        for (int i = row; i < row + 3; i++) {
            for (int j = col; j < col + 3; j++) {
                if (puzzle[i][j] != 0 && used[puzzle[i][j]]) {
                    return false;
                }
                used[puzzle[i][j]] = true;
            }
        }
    }
    
    return true;
}

// Utility function to print the puzzle
void printPuzzle(int puzzle[SIZE][SIZE], FILE *fw)
{
    fprintf(fw, "\n");
    printf("\n");
    fprintf(fw, "  Y ^\n");
    printf("  Y ^\n");
    for (int i = 0; i < SIZE; i++)
    {
        if (i % 3 == 0)
        {
            fprintf(fw, "    | -------------------------------\n");
            printf("    | -------------------------------\n");
        }
        fprintf(fw, "  %2d| ", 10 - (i + 1));
        printf("  %2d| ", 10 - (i + 1));
        for (int j = 0; j < SIZE; j++)
        {
            if (j % 3 == 0)
            {
                fprintf(fw, "|");
                printf("|");
            }
            fprintf(fw, " %d ", puzzle[i][j]);
            printf(" %d ", puzzle[i][j]);
        }
        fprintf(fw, "|\n");
        printf("|\n");
    }
    fprintf(fw, "    | -------------------------------\n");
    printf("    | -------------------------------\n");
    fprintf(fw, "    0--------------------------------->\n");
    printf("    0--------------------------------->\n");
    fprintf(fw, "        1  2  3   4  5  6   7  8  9   X\n");
    printf("        1  2  3   4  5  6   7  8  9   X\n");
}

// Utility function to check if a number is valid for a given cell
bool isValid(int puzzle[SIZE][SIZE], int row, int col, int num)
{
    // Check row and column
    for (int i = 0; i < SIZE; i++)
    {
        if (puzzle[row][i] == num || puzzle[i][col] == num)
        {
            return false;
        }
    }

    // Check 3x3 box
    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;
    for (int i = boxRow; i < boxRow + 3; i++)
    {
        for (int j = boxCol; j < boxCol + 3; j++)
        {
            if (puzzle[i][j] == num)
            {
                return false;
            }
        }
    }

    // Number is valid
    return true;
}

void printPart1(int puzzle[SIZE][SIZE], FILE *fw)
{
    fprintf(fw, "2 uzduotis, 8 variantas. 3 grupe, 2 pogrupis. Hubertas Vindzigalskis.\n");
    fprintf(fw, "\nSalyga: 8. Parasyti programa, sprendziancia Sudoku. Randamas pirmasis sprendinys.\nAlgoritmas pradeda nuo langelio X=1 Y=9 ir keliauja iki X=9.\nTuomet sprendimas tesiamas zemesneje eiluteje Y=8 ir taip\nkeliaujame kol pasiekiame Y=1 eilutes pabaiga, kuomet atrandamas sprendinys.\n");
    fprintf(fw, "\nAlgoritmas:\n");
    fprintf(fw, "  1) Is eiles tikrinamas kiekvienas langelis ar jis nera uzimtas.\n");
    fprintf(fw, "  2) Jei langelis uzimtas ir yra paskutinis eiluteje, tai einame i zemensne\n     eilute. Jei nera paskutinis eiluteje, tai einame i sekanti langeli eiluteje.\n");
    fprintf(fw, "  3) Jei langelis tuscias, tai sukame cikla nuo 1 iki 9 ir bandome istatyti\n     kiekviena skaiciu tikrindami jo validuma sudoku sprendime su funkcija isValid().\n");
    fprintf(fw, "  4) Istacius skaiciu i langeli tikriname ar langelis buvo paskutinis\n     eiluteje, jei taip, keliaujame i zemesne eilute, jei ne, keliaujame\n     i sekanti langeli eiluteje.\n");
    fprintf(fw, "  5) Kartojame visus siuos veiksmus, kol pasiekiame zemiausios eilutes (Y=1) pabaiga.\n");
    fprintf(fw, "\nDALIS 1. Duomenys.\n");
    printf("\nDALIS 1. Duomenys.\n");
    fprintf(fw, "  1.1. Pradiniai duomenys:\n");
    printf("  1.1. Pradiniai duomenys:\n");
    printPuzzle(puzzle, fw);
}

void printPart2(char sequence, bool isNewLine, int row, int col, int num, FILE *fw)
{
    // Print protocol entry
    ++step;
    fprintf(fw, "% 6d) ", step);
    if (sequence == 's')
    {
        for (int i = 0; i < deepness; ++i)
        {
            fprintf(fw, "-");
        }
        // fprintf(fw, "bandoma");
        fprintf(fw, " X=%d,Y=%d.", col + 1, 10 - (row + 1));
        fprintf(fw, " Bandoma statyti skaiciu %d.", num);
        if (!isNewLine && col == 8)
        {
            fprintf(fw, " Sprendimas tesiamas zemesneje eiluteje.");
        }
        
        fprintf(fw, "\n");
        ++deepness;
    }
    else if (sequence == 'w')
    {
        // printf("Step %d: (%d, %d) set to %d\n", step, row, col, num);

        for (int i = 0; i < deepness; ++i)
        {
            fprintf(fw, "-");
        }
        // fprintf(fw, "bandoma");
        fprintf(fw, " X=%d,Y=%d.", col + 1, 10 - (row + 1));
        fprintf(fw, " Langelis yra uzimtas skaiciumi %d.", num);
        if (!isNewLine && col == 8)
        {
            fprintf(fw, " Sprendimas tesiamas zemesneje eiluteje.");
        }
        
        fprintf(fw, "\n");
        ++deepness;
    }
    else if (sequence == 'c')
    {
        for (int i = 0; i < deepness; ++i)
        {
            fprintf(fw, "-");
        }
        fprintf(fw, " X=%d,Y=%d.", col + 1, 10 - (row + 1));
        fprintf(fw, " Nerastas tinkamas skaicius. Backtrack. Fail.\n", num);
        --deepness;
    }
    else if (sequence == 'b')
    {
        for (int i = 0; i < deepness; ++i)
        {
            fprintf(fw, "-");
        }
        // fprintf(fw, "bandoma");
        fprintf(fw, " X=%d,Y=%d. ", col + 1, 10 - (row + 1));
        fprintf(fw, "Bandoma statyti skaiciu %d.", num);
        if (!isNewLine && col == 8)
        {
            fprintf(fw, " Sprendimas tesiamas zemesneje eiluteje.");
        }
        fprintf(fw, "\n");
        ++deepness;
    }
    else
    {
        fprintf(fw, "Neisprendziamas pradinis sudoku.\n");
        printf("Neisprendziamas pradinis sudoku.\n");
    }
}

void printPart3(int puzzle[SIZE][SIZE], FILE *fw)
{
    if (solveSudoku(puzzle, 0, 0, fw))
    {
        fprintf(fw, "% 6d) Sudoku sprendinys rastas.\n", step + 1);
        fprintf(fw, "\nDALIS 3. Rezultatai.\n");
        printf("\nDALIS 3. Rezultatai.\n");
        fprintf(fw, "  3.1. Sprendinys atrastas zingsnyje %d.\n", step + 1);
        printf("  3.1. Sprendinys atrastas zingsnyje %d.\n", step + 1);
        fprintf(fw, "  3.2. Issprestas sudoku:\n");
        printf("  3.2. Issprestas sudoku:\n");
        printPuzzle(puzzle, fw);
    }
    else
    {
        fprintf(fw, "% 6d) Sudoku sprendinys nerastas.\n", step + 1);
        fprintf(fw, "DALIS 3. Rezultatai.\n");
        printf("DALIS 3. Rezultatai.\n");
        fprintf(fw, "  3.1. Sudoku sprendimo neturi.\n");
        printf("  3.1. Sudoku sprendimo neturi.\n");
    }
}

// Main recursive function to solve the puzzle
bool solveSudoku(int puzzle[SIZE][SIZE], int row, int col, FILE *fw)
{
    bool temp;
    // Base case: puzzle is solved
    if (row == SIZE)
    {
        return true;
    }

    // Check if cell is empty
    if (puzzle[row][col] != 0)
    {
        // Cell is not empty, move on to next cell
        // fprintf(fw, "1\n");
        // printPuzzle(puzzle, fw);
        printf("%d\n", (row == SIZE - 1) ? true : false);
        printPart2('w', (row == SIZE - 1) ? true : false, row, col, puzzle[row][col], fw); // Call to print protocol taken
        if (col == SIZE - 1)
        {
            temp = solveSudoku(puzzle, row + 1, 0, fw);
            if (temp == false)
            {
                // fprintf(fw, "2\n");
                // printPuzzle(puzzle, fw);
                printPart2('c', false, row, 0, puzzle[row][0], fw); // Call to print protocol backtrack
            }
            return temp;
        }
        else
        {
            temp = solveSudoku(puzzle, row, col + 1, fw);
            if (temp == false)
            {
                // fprintf(fw, "3\n");
                // printPuzzle(puzzle, fw);
                printPart2('c', false, row, col, puzzle[row][col], fw); // Call to print protocol backtrack
            }
            return temp;
        }
    }
    else
    {
        // Cell is empty, try filling with numbers 1-9
        for (int i = 1; i <= 9; i++)
        {
            if (isValid(puzzle, row, col, i))
            {
                puzzle[row][col] = i;
                // Move on to next cell
                if (col == SIZE - 1)
                {
                    // fprintf(fw, "4\n");
                    // printPuzzle(puzzle, fw);
                    printPart2('b', false, row, col, i, fw); // Call to print protocol backtrack
                    if (solveSudoku(puzzle, row + 1, 0, fw))
                    {
                        return true;
                    }
                }
                else
                {
                    // fprintf(fw, "5\n");
                    // printPuzzle(puzzle, fw);
                    printPart2('s', (col == SIZE - 1) ? true : false, row, col, i, fw); // Call to print protocol set
                    if (solveSudoku(puzzle, row, col + 1, fw))
                    {
                        return true;
                    }
                }
            }
        }
    }
    // Backtrack
    printPart2('c', false, row, col, puzzle[row][col], fw);
    puzzle[row][col] = 0;
    return false;
}