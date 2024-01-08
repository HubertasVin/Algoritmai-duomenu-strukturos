#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_NODES 100

struct Node {
    int label;
    struct Node** neighbors;
    int numNeighbors;
    int visited;
};

struct Node* createNode(int label, int numNeighbors) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->label = label;
    newNode->numNeighbors = numNeighbors;
    newNode->visited = 0;

    newNode->neighbors = (struct Node**)malloc(numNeighbors * sizeof(struct Node*));
    for (int i = 0; i < numNeighbors; i++) {
        newNode->neighbors[i] = NULL;
    }

    return newNode;
}

int T = 0;

void addNeighbor(struct Node* node, struct Node* neighbor) {
    for (int i = 0; i < node->numNeighbors; ++i) {
        if (node->neighbors[i] == NULL) {
            node->neighbors[i] = neighbor;
            return;
        }
    }
}

void BFS(struct Node* startNode, int *Result, int *rn, FILE *fw) {
    struct Node* queue[MAX_NODES];
    int front = 0, rear = 0;

    printf("DALIS 2. Vykdymas:\n");
    fprintf(fw, "DALIS 2. Vykdymas:\n");

    printf("  T=%d. Veiksmas %d:\n", T, T);
    fprintf(fw, "  T=%d. Veiksmas %d:\n", T, T);
    ++T;
    printf("    Atidaryta virsune: %d\n", startNode->label);
    fprintf(fw, "    Atidaryta virsune: %d\n", startNode->label);
    startNode->visited = 1;

    queue[rear++] = startNode;
    printf("    Eile: ");
    fprintf(fw, "    Eile: ");
    printf("%d ", startNode->label);
    fprintf(fw, "%d ", startNode->label);
    printf("\n");
    fprintf(fw, "\n");

    while (front < rear) {
        struct Node* currentNode = queue[front++];
        Result[*rn] = currentNode->label;
        ++*rn;

        for (int i = 0; i < currentNode->numNeighbors; i++) {
            struct Node* neighbor = currentNode->neighbors[i];
            if (neighbor != NULL && !neighbor->visited) {
                queue[rear++] = neighbor;
                neighbor->visited = 1;
            }
        }

        if (front < rear) {
            printf("  T=%d. Veiksmas %d:\n", T, T);
            fprintf(fw, "  T=%d. Veiksmas %d:\n", T, T);
            ++T;
            printf("    Aplankytos virsunes: ");
            fprintf(fw, "    Aplankytos virsunes: ");
            for (int i = 0; i < rear; i++) {
                printf("%d ", queue[i]->label);
                fprintf(fw, "%d ", queue[i]->label);
            }
            printf("\n");
            fprintf(fw, "\n");
            printf("    Eile: ");
            fprintf(fw, "    Eile: ");
            for (int i = front; i < rear; i++) {
                printf("%d ", queue[i]->label);
                fprintf(fw, "%d ", queue[i]->label);
            }
            printf("\n");
            fprintf(fw, "\n");
        }
    }
    printf("  T=%d. Veiksmas %d:\n", T, T);
    fprintf(fw, "  T=%d. Veiksmas %d:\n", T, T);
    ++T;
    printf("    Aplankytos virsunes: ");
    fprintf(fw, "    Aplankytos virsunes: ");
    for (int i = 0; i < rear; i++) {
        printf("%d ", queue[i]->label);
        fprintf(fw, "%d ", queue[i]->label);
    }
    printf("\n");
    fprintf(fw, "\n");
    startNode->visited = 1;

    queue[rear++] = startNode;
    printf("    Eile: ");
    fprintf(fw, "    Eile: ");
    printf("tuscia");
    fprintf(fw, "tuscia");
    printf("\n\n");
    fprintf(fw, "\n\n");
}

bool isEmpty(char *text) {
    for (int i = 0; i < strlen(text); ++i)
    {
        if (!isspace(text[i]))
            return false;
    }
    return true;
}

void printPart1(FILE *fw, int nodeN, char Graph[][100], int graphSize) {
    printf("ADS 4 uzduotis. 14 variantas.\nHubertas Vindzigalskis. 3 grupe, 2 pogrupis.\n");
    fprintf(fw, "ADS 4 uzduotis. 14 variantas.\nHubertas Vindzigalskis. 3 grupe, 2 pogrupis.\n");

    printf("\nSalyga: Is duoto grafo suformuoti medi. (Grafo realizacija grindziama kaiminystes\nsarysiais; naudoti paieskos i ploti metoda.)\n\n");
    fprintf(fw, "\nSalyga: Is duoto grafo suformuoti medi. (Grafo realizacija grindziama kaiminystes\nsarysiais; naudoti paieskos i ploti metoda.)\n\n");

    printf("DALIS 1. Pradiniai duomenys:\n");
    fprintf(fw, "DALIS 1. Pradiniai duomenys:\n");
    printf("  1) Virsuniu skaicius = %d\n", nodeN - 1);
    fprintf(fw, "  1) Virsuniu skaicius = %d\n", nodeN - 1);
    printf("  2) Grafo brezinys:\n");
    fprintf(fw, "  2) Grafo brezinys:\n");
    for (int i = 1; i < graphSize; ++i)
    {
        if (!isEmpty(Graph[i]))
        {
            printf("     %s", Graph[i]);
            fprintf(fw, "     %s", Graph[i]);
            if (strchr(Graph[i], '\n') == NULL)
            {
                printf("\n");
                fprintf(fw, "\n");
            }
        }
    }
    printf("\n");
    fprintf(fw, "\n");
}

int main() {
    int Result[100], rn = 0;
    int tempNode1, tempNode2;
    int nodeN = 0;
    int *NeighborN;

    char Graph[100][100], graphSize = 0;
    char fileName[100];

    printf("Iveskite duomenu failo pavadinima: ");
    scanf("%s", fileName);
    FILE *fr = fopen(fileName, "r");
    FILE *fw = fopen("protokolas.txt", "w");

    // Get number of nodes
    while (fscanf(fr, "%d", &tempNode1) != 0)
    {
        if (nodeN < tempNode1)
            nodeN = tempNode1;
    }
    ++nodeN;
    NeighborN = malloc(nodeN * sizeof(int));

    // Get number of neighbors for each node
    fseek(fr, 0, SEEK_SET);
    while (fscanf(fr, "%d %d", &tempNode1, &tempNode2) != 0)
    {
        ++NeighborN[tempNode1];
    }
    
    // Create nodes
    struct Node** Nodes = malloc(nodeN * sizeof(struct Node*));
    for (int i = 0; i < nodeN; i++) {
        Nodes[i] = createNode(i, NeighborN[i]);
    }

    fseek(fr, 0, SEEK_SET);
    while (fscanf(fr, "%d %d", &tempNode1, &tempNode2) != 0)
    {
        addNeighbor(Nodes[tempNode1], Nodes[tempNode2]);
    }

    // Print the neighbors of each node
    /* for (int i = 0; i < nodeN; ++i) {
        printf("Node %d neighbors: ", Nodes[i]->label);
        fprintf(fw, "Node %d neighbors: ", Nodes[i]->label);
        for (int j = 0; j < Nodes[i]->numNeighbors; ++j) {
            printf("%d ", Nodes[i]->neighbors[j]->label);
            fprintf(fw, "%d ", Nodes[i]->neighbors[j]->label);
        }
        printf("\n");
        fprintf(fw, "\n");
    } */

    int size;
    while (fgets(Graph[graphSize], 100, fr) != NULL)
        ++graphSize;

    printPart1(fw, nodeN, Graph, graphSize);

    /* struct Node* A = createNode(0, 2);
    struct Node* B = createNode(1, 3);
    struct Node* C = createNode(2, 3);
    struct Node* D = createNode(3, 2);
    struct Node* E = createNode(4, 2);

    addNeighbor(A, B);
    addNeighbor(A, C);
    addNeighbor(B, A);
    addNeighbor(B, C);
    addNeighbor(B, D);
    addNeighbor(C, A);
    addNeighbor(C, B);
    addNeighbor(C, E);
    addNeighbor(D, B);
    addNeighbor(D, E);
    addNeighbor(E, C);
    addNeighbor(E, D); */

    /* struct Node* A = createNode(2, 2);
    struct Node* B = createNode(0, 2);
    struct Node* C = createNode(1, 1);
    struct Node* D = createNode(3, 1);

    addNeighbor(A, B);
    addNeighbor(A, D);
    addNeighbor(B, A);
    addNeighbor(B, C);
    addNeighbor(C, A);
    addNeighbor(A, D);
    addNeighbor(D, D); */

    BFS(Nodes[0], Result, &rn, fw);

    /* free(A->neighbors);
    free(A);
    free(B->neighbors);
    free(B);
    free(C->neighbors);
    free(C);
    free(D->neighbors);
    free(D); */
    
    /* free(A->neighbors);
    free(A);
    free(B->neighbors);
    free(B);
    free(C->neighbors);
    free(C);
    free(D->neighbors);
    free(D);
    free(E->neighbors);
    free(E); */
    
    printf("DALIS 3. Rezultatas:\n");
    fprintf(fw, "DALIS 3. Rezultatas:\n");
    printf("  ");
    fprintf(fw, "  ");
    for (int i = 0; i < rn; i++) {
        printf("%d ", Result[i]);
        fprintf(fw, "%d ", Result[i]);
    }
    printf("\n");
    fprintf(fw, "\n");

    return 0;
}
