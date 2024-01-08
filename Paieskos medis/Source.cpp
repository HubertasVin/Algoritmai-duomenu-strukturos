#include <iostream>
#include <string.h>

using namespace std;

struct BST
{
    int element = 0;
    bool isOccupied = false;
    BST *lesser, *greater;
};

void interface(BST A[], int *bstCount);
int createBSTOption(BST A[], int *bstCount);
int deleteBSTOption(BST A[], int *bstCount);
void sort(int A[], int n);
void optimiseBSTOption(BST A[], int bstCount);
void optimiseBST(BST &root, int *arr, int start, int end);
void insertOption(BST A[], int *bstCount);
void insert(BST &A, int element);
void searchOption(BST A[], int bstCount);
int search(BST A, int query);
void printBSTOption(BST A[], int bstCount);
void printBST(BST root, char direction, int length);
void bstEmptyOption(BST A[], int bstCount);
void swap(int *a, int *b);
int selectBST(int bstCount);
bool isNumeric(string input);
bool isExit(string input);
void bstToArr(BST &root, int *Arr, int *size);

int count = 0;
int bstMax = 3;

int main(int argc, const char **argv)
{
    BST *A = (BST *)malloc(sizeof(BST) * bstMax);
    
    int bstCount = 0;

    interface(A, &bstCount);

    return 0;
}

void interface(BST A[], int *bstCount)
{
    int bstSelection;
    int *treeElements, size = 0;
    int errorCode;
    char selection, garbage[80];
    char inputStr[80];

    system("clear");
    printf("Select one of these options\n");
    printf("1. Create Binary Search Tree\n");
    printf("0. Exit program\n");
    switch (getchar())
    {
    case '1':
    system("clear");
        if ((errorCode = createBSTOption(A, bstCount)) == 1)
        {
            printf("Created a new search tree.\n");
        }
        else
        {
            printf("The program exit with error code %d.\n", errorCode);
            printf("The limit of search trees has been exceeded (maximum number of trees is %d). No more search trees can be created unless you delete one.\n", bstMax);
        }
        break;

    case '0':
        system("clear");
        printf("Quitting application.\n");
        return;
        break;

    default:
        return;
        break;
    }

    cin.getline(garbage, 80);

    while (selection != '0')
    {
        // fflush(stdin);
        printf("Select one of these options:\n");
        printf("1. Create Binary Search Tree\n");
        printf("2. Insert a value into a search tree\n");
        printf("3. Search for a value in a search tree\n");
        printf("4. Optimise a search tree\n");
        printf("5. Delete a search tree\n");
        printf("6. Print a search tree\n");
        printf("7. Check if search tree is empty\n");
        printf("0. Exit program\n");

        // scanf("%c", &selection);
        cin >> selection;
        // cin.getline(garbage, 80);
        system("clear");
        switch (selection)
        {
        case '1':   // Create BST tree
            if ((errorCode = createBSTOption(A, bstCount)) == 1)
            {
                printf("Created a new search tree.\n\n");
            }
            else
            {
                printf("The task failed with error code %d.\n", errorCode);
                printf("The limit of search trees has been exceeded (maximum number of trees is %d). No more search trees can be created unless you delete one.\n\n", bstMax);
            }
            break;

        case '2':   // Insert value
            insertOption(A, bstCount);
            break;

        case '3':   // Search value
            searchOption(A, *bstCount);
            break;

        case '4':   // Optimise BST
            optimiseBSTOption(A, *bstCount);
            break;

        case '5':   //Delete BST
            if((errorCode = deleteBSTOption(A, bstCount)) == 1)
            {
                printf("Deleted the BST.\n\n");
            }
            else
            {
                printf("The task failed with error code %d.\n", errorCode);
                printf("There are no BSTs available.\n\n");
            }
            break;

        case '6':
            printBSTOption(A, *bstCount);
            break;
        
        case '7':
            bstEmptyOption(A, *bstCount);
            break;

        case '0':   // Exit application
            printf("Quitting application.\n");
            return;
            break;

        default:
            printf("Wrong user input, please try again.\n\n");
            printf("%c", selection);
            break;
        }
    }
}

int createBSTOption(BST A[], int *bstCount)
{
    if (*bstCount < bstMax)
    {
        ++*bstCount;
        return 1;
    }
    else
    {
        return 0;
    }
}

int deleteBSTOption(BST A[], int *bstCount)
{
    if (*bstCount > 0)
    {
        int bstSelection = selectBST(*bstCount);   // terminal BST selection interface
        for (int i = bstSelection; i < *bstCount - 1; ++i)
        {
            A[i] = A[i + 1];
        }
        --*bstCount;
        
        return 1;
    }
    else
    {
        return 2;   // There are no BSTs
    }
    
}

void sort(int A[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (A[i] > A[j])
                swap(A[i], A[j]);
        }
    }
}

void optimiseBSTOption(BST A[], int bstCount)
{
    int *treeElements = (int *)malloc(sizeof(int));
    int bstSelection;
    int size = 0;

    if ((bstSelection = selectBST(bstCount)) == -1)
    {
        printf("The task failed with error code %d.\n", -1);
        printf("There are no BSTs available.\n\n");
        return;
    }

    bstToArr(A[bstSelection], treeElements, &size);
    sort(treeElements, size);
    A[bstSelection].isOccupied = false;
    A[bstSelection].lesser = new BST();
    A[bstSelection].greater = new BST();
    optimiseBST(A[bstSelection], treeElements, 0, size - 1);

    printf("Optimised search tree #%d.\n\n", bstSelection);

    size = 0;
}

void optimiseBST(BST &root, int *arr, int start, int end)
{
    if (start <= end)
    {
        int mid = (start + end) / 2;
        insert(root, arr[mid]);
        optimiseBST(root, arr, start, mid - 1);
        optimiseBST(root, arr, mid + 1, end);
    }
}

void insertOption(BST A[], int *bstCount)
{
    char inputStr[80];
    int bstSelection;

    if ((bstSelection = selectBST(*bstCount)) == -1)
    {
        return;
    }

    printf("Selected search tree #%d.\n", bstSelection);
    printf("Enter the elements you want to insert (enter \"exit\" to exit):\n");
    scanf("%s", inputStr);
    while (!isExit(inputStr))
    {
        if (isNumeric(inputStr) == 1)
        {
            insert(A[bstSelection], stoi(inputStr));
        }
        else
        {
            printf("Wrong user input, please try again.\n");
        }
        scanf("%s", inputStr);
    }
    system("clear");
}

void insert(BST &root, int element)
{
    if (root.isOccupied == false)
    {
        root.element = element;
        root.isOccupied = true;
        root.lesser = new BST();
        root.greater = new BST();
    }
    else
    {
        if (element < root.element)
        {
            insert(*(root.lesser), element);
        }
        else
        {
            insert(*(root.greater), element);
        }
    }
}

void searchOption(BST A[], int bstCount)
{
    char inputStr[80];
    int bstSelection;

    if ((bstSelection = selectBST(bstCount)) == -1)
    {
        return;
    }

    printf("Selected search tree #%d.\n", bstSelection);
    printf("Enter the element you want to search for (enter \"exit\" to exit):\n");
    scanf("%s", inputStr);
    while (!isNumeric(inputStr))
    {
        printf("Wrong user input, please try again.\n");
        scanf("%s", inputStr);
    }

    system("clear");

    if (search(A[bstSelection], stoi(inputStr)) == 0)
    {
        printf("Found the element %s on node number %d.\n\n", inputStr, count);
    }
    else
    {
        printf("The search query %s was not found.\n\n", inputStr);
    }
    count = 0;
}

int search(BST root, int query)
{
    if (root.element == query)
    {
        return 0;
    }
    else
    {
        if (query < root.element && root.isOccupied == true)
        {
            ++count;
            return search(*(root.lesser), query);
        }
        else if (query > root.element && root.isOccupied == true)
        {
            ++count;
            return search(*(root.greater), query);
        }
    }
    return 1;
}

void printBSTOption(BST A[], int bstCount)
{
    int *treeElements = (int *)malloc(sizeof(int));
    int bstSelection;
    int size = 0;

    if ((bstSelection = selectBST(bstCount)) == -1)
    {
        printf("The task failed with error code %d.\n", -1);
        printf("There are no BSTs available.\n\n");
        return;
    }

    bstToArr(A[bstSelection], treeElements, &size);

    if (size == 0)
    {
        printf("The BST #%d is empty.\n\n", bstSelection);
    }
    else
    {
        printf("Printing BST #%d:\n", bstSelection);
        printBST(A[bstSelection], 'N', 0);
        printf("\n\n");
    }
}

void printBST(BST root, char direction, int length)
{
    if (root.isOccupied != false)
    {
        for (int i = 0; i < length - 1; ++i)
        {
            printf("-");
        }
        if (direction != 'N')
        {
            printf("%c", direction);
        }
        printf("%d\n", root.element);
        ++length;
        printBST(*root.lesser, 'K', length);
        printBST(*root.greater, 'D', length);
    }
    else
    {
        for (int i = 0; i < length - 1; ++i)
        {
            printf("-");
        }
        if (direction != 'N')
        {
            printf("%c", direction);
        }
        printf("*\n");
        return;
    }
}

void bstEmptyOption(BST A[], int bstCount)
{
    int *treeElements = (int *)malloc(sizeof(int));
    int bstSelection;
    int size = 0;

    if ((bstSelection = selectBST(bstCount)) == -1)
    {
        printf("The task failed with error code %d.\n", -1);
        printf("There are no BSTs available.\n\n");
        return;
    }

    bstToArr(A[bstSelection], treeElements, &size);

    if (size == 0)
    {
        printf("The BST #%d is empty\n\n", bstSelection);
    }
    else
    {
        printf("The BST #%d is not empty\n\n", bstSelection);
    }
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int selectBST(int bstCount)
{
    int bstSelection;

    if (bstCount == 0)
    {
        printf("There are no BSTs available.\n\n");
        return -1;
    }
    else if (bstCount == 1)
    {
        return 0;
    }
    do
    {
        printf("Select one search tree from interval [0;%d].\n", bstCount);
        scanf("%d", &bstSelection);
        system("clear");
        if (bstSelection < 0 || bstCount <= bstSelection)
        {
            printf("Wrong user input, please try again.\n");
        }
    } while (bstSelection < 0 || bstCount <= bstSelection);
    return bstSelection;
}

bool isNumeric(string input)
{
    for (int i = 0; i < input.length(); ++i)
    {
        if (!isdigit(input[i]))
        {
            return 0;
        }
    }
    if (input.length() > 9)
    {
        return -1;
    }
    
    return 1;
}

bool isExit(string input)
{
    return ((input[0] == 'e') + (input[1] == 'x') + (input[2] == 'i') + (input[3] == 't')) == 4;
}

void bstToArr(BST &root, int *Arr, int *size)
{
    if (root.isOccupied == true)
    {
        ++*size;
        Arr = (int *)realloc(Arr, *size * sizeof(int));
        Arr[*size - 1] = root.element;
        if (root.lesser != NULL)
        {
            bstToArr(*root.lesser, Arr, size);
        }
        if (root.greater != NULL)
        {
            bstToArr(*root.greater, Arr, size);
        }
    }
}