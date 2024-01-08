#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct
{
    bool isAvailable;
    int clientID;
    char cleintType;
    int timeWithClient;
} Cashier;

typedef struct
{
    bool isFilled;
    int QueueFree[10000];
    int QueuePrem[10000];
    int nf;
    int np;
    int timeSegment;
} DataByTimeSegment;

void beginWork(Cashier *Cashier, DataByTimeSegment *Data, int NewClientsFree[], int NewClientsPrem[], int QueueFree[][2], int QueuePrem[][2], int nnf, int nnp, int nf, int np, int nc, FILE *fw);
void beginWork2(Cashier *Cashier, DataByTimeSegment *Data, int NewClientsFree[], int NewClientsPrem[], int QueueFree[][2], int QueuePrem[][2], int nnf, int nnp, int nf, int np, int nc, FILE *fw);
void createRandomClient(DataByTimeSegment *Data, int NewClientsFree[], int NewClientsPrem[], int QueueFree[][2], int QueuePrem[][2], int *nnf, int *nnp, int *nf, int *np, int *nc);
void updateCashierStatus(Cashier Cashier[]);
void moveClientsToQueue(int NewClientsFree[], int NewClientsPrem[], int QueueFree[][2], int QueuePrem[][2], int *nnf, int *nnp, int *nf, int *np, int *nc);
void moveClientsToCashiers(int QueueFree[][2], int QueuePrem[][2], Cashier Cashier[], int *nf, int *np, int *nc, FILE *fw);
void moveClientsToCashiers2(int QueueFree[][2], int QueuePrem[][2], Cashier Cashier[], int *nf, int *np, int *nc, FILE *fw);
void printPart1(FILE *fw, FILE *fw2);
void printPart2(int NewClientsFree[], int NewClientsPrem[], int QueueFree[][2], int QueuePrem[][2], Cashier Cashier[], int nnf, int nnp, int nf, int np, int nc, FILE *fw);
void printPart3(int QueueFree[][2], int QueuePrem[][2], int nf, int np, int nc, FILE *fw, FILE *fw2);
int numberOfDigits(int number);
bool keepGoing(int QueueFree[][2], int QueuePrem[][2], int nf, int np, int nc, Cashier Cashier[]);
void calculateScoreElements(int QueueFree[][2], int QueuePrem[][2], int nf, int np, int nc, Cashier Cashier[], bool isOvertime);
void printToFiles(char text[], FILE *fw, FILE *fw2);
int calculateEfficiencyScore();

int cashiers = 5; // Kasininku skaicius
int vipCashiers = 1; // VIP kasininku skaicius

const int maxNewClients = 1; // Maksimalus nauju klientu skaicius

int chanceFree = 100; // Naujo paprastojo kliento tikimybe
int chancePrem = 0; // Naujo priritytetinio kliento tikimybe

int timeSegment = 0; // Laiko segmentas

const int timeInterval = 60; // Vieno laiko segmento trukme sekundemis
int timeWork = 100; // Banko darbo trukme laiko segmentais
int timeCashierSpeed = 6; // Kliento aptarnavimo trukme segmentais

int totalFreeClients = 0; // Paprastuju klientu skaicius
int timeAtQueueFreeProc1 = 0; // Paprastojo kliento laikas praleistas eileje
int timeAtQueueFree = 0; // Paprastojo kliento laikas praleistas eileje
int maxTimeAtQueuePremProc1 = 0; // VIP kliento laikas praleistas eileje
int maxTimeAtQueuePrem = 0; // VIP kliento laikas praleistas eileje
int timeDoingNothingProc1 = 0; // Kasininko laikas praleistas be kliento
int timeDoingNothing = 0; // Kasininko laikas praleistas be kliento
int timeOvertimeProc1 = 0; // Kasininko laikas praleistas po darbo
int timeOvertime = 0; // Kasininko laikas praleistas po darbo

int process1Score = 0; // Proceso 1 rezultatas
int process2Score = 0; // Proceso 2 rezultatas

int wordWrapCount; // Teksto permetimo skaicius

#define _GNU_SOURCE

int main()
{
    int QueueFree[10000][2] = { { 0 }, { 0 } }, nf = 0; // Paprastosios eiles klientu ID ir praleistu laiko segmentu masyvas
    int QueuePrem[10000][2] = { { 0 }, { 0 } }, np = 0; // VIP eiles klientu ID ir praleistu laiko segmentu masyvas
    int NewClientsFree[10000] = { 0 }, nnf = 0; // Nauju paprastu klientu ID masyvas
    int NewClientsPrem[10000] = { 0 }, nnp = 0; // Nauju VIP klientu ID masyvas
    int nc = 0; // Klientu skaicius
    Cashier Cashier[cashiers]; // Kasininku masyvas
    DataByTimeSegment Data[timeWork];

    char buffer1[60]; // The filename buffer.
    char buffer2[60]; // The filename buffer.

    
    printf("Iveskite paprasto kliento atejimo tikimybe: ");
    scanf("%d", &chanceFree);
    printf("Iveskite VIP kliento atejimo tikimybe: ");
    scanf("%d", &chancePrem);
    snprintf(buffer1, sizeof(char) * 60, "protokolas1-1-procesas1-%dmin-%dproc-%dProc.txt", timeWork, chanceFree, chancePrem);
    snprintf(buffer2, sizeof(char) * 60, "protokolas2-1-procesas2-%dmin-%dproc-%dProc.txt", timeWork, chanceFree, chancePrem);
    FILE *fw = fopen(buffer1, "w");
    FILE *fw2 = fopen(buffer2, "w");
    // chancePrem = 100 - chanceFree;
    // printf("Iveskite kasininku skaiciu: ");
    // scanf("%d", &cashiers);
    // printf("Iveskite kiek siu kasininku bus VIP kasininkai (2 procesas): ");
    // scanf("%d", &vipCashiers);
    // printf("Iveskite kiek laiko kasininkas praleidzia su vienu klientu: ");
    // scanf("%d", &timeCashierSpeed);
    // printf("Iveskite kiek laiko dirba bankas: ");
    // scanf("%d", &timeWork);
    printPart1(fw, fw2);
    fprintf(fw, "DALIS 2. Vykdymas.\n");
    fprintf(fw2, "DALIS 2. Vykdymas.\n");
    beginWork(Cashier, Data, NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, nnf, nnp, nf, np, nc, fw);
    timeAtQueueFreeProc1 = timeAtQueueFree;
    maxTimeAtQueuePremProc1 = maxTimeAtQueuePrem;
    timeDoingNothingProc1 = timeDoingNothing;
    timeOvertimeProc1 = timeOvertime;
    process1Score = calculateEfficiencyScore();
    beginWork2(Cashier, Data, NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, nnf, nnp, nf, np, nc, fw2);
    process2Score = calculateEfficiencyScore();
    printPart3(QueueFree, QueuePrem, nf, np, nc, fw, fw2);

    return 0;
}

// Pradeda darba bankas procesu 1
void beginWork(Cashier *Cashier, DataByTimeSegment *Data, int NewClientsFree[], int NewClientsPrem[], int QueueFree[][2], int QueuePrem[][2], int nnf, int nnp, int nf, int np, int nc, FILE *fw)
{
    srand(time(NULL));
    for(int i = 0; i < cashiers; ++i)
    {
        Cashier[i].clientID = 0;
        Cashier[i].isAvailable = true;
    }

    for(int i = 0; i < timeWork; ++i)
    {
        createRandomClient(Data, NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, &nnf, &nnp, &nf, &np, &nc);
        printPart2(NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, Cashier, nnf, nnp, nf, np, nc, fw);
        
        calculateScoreElements(QueueFree, QueuePrem, nf, np, nc, Cashier, false);

        moveClientsToQueue(NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, &nnf, &nnp, &nf, &np, &nc);
        updateCashierStatus(Cashier);
        moveClientsToCashiers(QueueFree, QueuePrem, Cashier, &nf, &np, &nc, fw);
        ++timeSegment;
    }
    while (keepGoing(QueueFree, QueuePrem, nf, np, nc, Cashier))
    {
        printPart2(NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, Cashier, nnf, nnp, nf, np, nc, fw);
        
        calculateScoreElements(QueueFree, QueuePrem, nf, np, nc, Cashier, true);

        moveClientsToQueue(NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, &nnf, &nnp, &nf, &np, &nc);
        updateCashierStatus(Cashier);
        moveClientsToCashiers(QueueFree, QueuePrem, Cashier, &nf, &np, &nc, fw);
        ++timeSegment;
    }
    printPart2(NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, Cashier, nnf, nnp, nf, np, nc, fw);
}

// Pradeda darba kasininkai procesu 2
void beginWork2(Cashier *Cashier, DataByTimeSegment *Data, int NewClientsFree[], int NewClientsPrem[], int QueueFree[][2], int QueuePrem[][2], int nnf, int nnp, int nf, int np, int nc, FILE *fw)
{
    timeSegment = 0;
    for(int i = 0; i < cashiers; ++i)
    {
        Cashier[i].clientID = 0;
        Cashier[i].isAvailable = true;
    }

    for(int i = 0; i < timeWork; ++i)
    {
        createRandomClient(Data, NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, &nnf, &nnp, &nf, &np, &nc);
        printPart2(NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, Cashier, nnf, nnp, nf, np, nc, fw);
        
        calculateScoreElements(QueueFree, QueuePrem, nf, np, nc, Cashier, false);

        moveClientsToQueue(NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, &nnf, &nnp, &nf, &np, &nc);
        updateCashierStatus(Cashier);
        moveClientsToCashiers2(QueueFree, QueuePrem, Cashier, &nf, &np, &nc, fw);
        ++timeSegment;
    }
    while (keepGoing(QueueFree, QueuePrem, nf, np, nc, Cashier))
    {
        printPart2(NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, Cashier, nnf, nnp, nf, np, nc, fw);
        
        calculateScoreElements(QueueFree, QueuePrem, nf, np, nc, Cashier, true);

        moveClientsToQueue(NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, &nnf, &nnp, &nf, &np, &nc);
        updateCashierStatus(Cashier);
        moveClientsToCashiers2(QueueFree, QueuePrem, Cashier, &nf, &np, &nc, fw);
        ++timeSegment;
    }
    printPart2(NewClientsFree, NewClientsPrem, QueueFree, QueuePrem, Cashier, nnf, nnp, nf, np, nc, fw);
}

// Atsitiktinai sugeneruoja nauja klienta (20% - VIP, 80% - paprastas)
void createRandomClient(DataByTimeSegment *Data, int NewClientsFree[], int NewClientsPrem[], int QueueFree[][2], int QueuePrem[][2], int *nnf, int *nnp, int *nf, int *np, int *nc)
{
    if (!Data[timeSegment].isFilled)
    {
        Data[timeSegment].nf = 0;
        Data[timeSegment].np = 0;

        int chance = rand() % 100 + 1;
        int randomClientAmount = rand() % maxNewClients + 1; // Sugeneruojama kiek ateis nauju klientu
        for (int i = 0; i < randomClientAmount; ++i)
        {
            chance = rand() % 100 + 1;
            if(chance <= chancePrem)
            {
                ++*nc;
                NewClientsPrem[*nnp] = *nc;
                ++*nnp;
                Data[timeSegment].QueuePrem[Data[timeSegment].np] = *nc;
                ++Data[timeSegment].np;
                /* else
                {
                    QueuePrem[*np][0] = *nc;
                    QueuePrem[*np][1] = 0;
                    ++*np;
                } */
            }
        }
        for (int i = 0; i < randomClientAmount; ++i)
        {
            chance = rand() % 100 + 1;
            if (chance > chancePrem && chance <= chanceFree + chancePrem)
            {
                ++*nc;
                NewClientsFree[*nnf] = *nc;
                ++*nnf;
                ++totalFreeClients;
                if (!Data[timeSegment].isFilled)
                {
                    Data[timeSegment].QueueFree[Data[timeSegment].nf] = *nc;
                    ++Data[timeSegment].nf;
                }
                
                /* else
                {
                    QueueFree[*nf][0] = *nc;
                    QueueFree[*nf][1] = 0;
                    ++*nf;
                } */
            }
        }
        
        Data[timeSegment].isFilled = true;
    }
    else
    {
        for (int i = 0; i < Data[timeSegment].np; ++i)
        {
            NewClientsPrem[*nnp] = Data[timeSegment].QueuePrem[*nnp];
            ++*nnp;
        }
        for (int i = 0; i < Data[timeSegment].nf; ++i)
        {
            NewClientsFree[*nnf] = Data[timeSegment].QueueFree[*nnf];
            ++*nnf;
        }
    }
}

void updateCashierStatus(Cashier Cashier[])
{
    for(int i = 0; i < cashiers; ++i)
    {
        if(!Cashier[i].isAvailable)
        {
            if(Cashier[i].timeWithClient == timeCashierSpeed)
            {
                Cashier[i].timeWithClient = 0;
                Cashier[i].isAvailable = true;
            }
            ++Cashier[i].timeWithClient;
        }
    }
}

void moveClientsToQueue(int NewClientsFree[], int NewClientsPrem[], int QueueFree[][2], int QueuePrem[][2], int *nnf, int *nnp, int *nf, int *np, int *nc)
{
    for(int i = 0; i < *nnf; ++i)
    {
        QueueFree[*nf][0] = NewClientsFree[i];
        QueueFree[*nf][1] = 0;
        ++*nf;
    }
    for(int i = 0; i < *nnp; ++i)
    {
        QueuePrem[*np][0] = NewClientsPrem[i];
        QueuePrem[*np][1] = 0;
        ++*np;
    }
    *nnf = 0;
    *nnp = 0;
}

void moveClientsToCashiers(int QueueFree[][2], int QueuePrem[][2], Cashier Cashier[], int *nf, int *np, int *nc, FILE *fw)
{
    for(int i = 0; i < cashiers; ++i)
    {
        if(Cashier[i].isAvailable)
        {
            if(*np > 0)
            {
                Cashier[i].clientID = QueuePrem[0][0];
                Cashier[i].cleintType = 'P';
                Cashier[i].timeWithClient = 0;
                Cashier[i].isAvailable = false;
                for(int j = 0; j < *np; ++j)
                {
                    QueuePrem[j][0] = QueuePrem[j + 1][0];
                    QueuePrem[j][1] = QueuePrem[j + 1][1];
                }
                --*np;
            }
            else if(*nf > 0)
            {
                Cashier[i].clientID = QueueFree[0][0];
                Cashier[i].cleintType = 'F';
                Cashier[i].timeWithClient = 0;
                Cashier[i].isAvailable = false;
                for(int j = 0; j < *nf; ++j)
                {
                    QueueFree[j][0] = QueueFree[j + 1][0];
                    QueueFree[j][1] = QueueFree[j + 1][1];
                }
                --*nf;
            }
        }
    }
}

void moveClientsToCashiers2(int QueueFree[][2], int QueuePrem[][2], Cashier Cashier[], int *nf, int *np, int *nc, FILE *fw)
{
    for(int i = cashiers - vipCashiers; i < cashiers; ++i)
    {
        if(Cashier[i].isAvailable)
        {
            if(*np > 0)
            {
                Cashier[i].clientID = QueuePrem[0][0];
                Cashier[i].cleintType = 'P';
                Cashier[i].timeWithClient = 0;
                Cashier[i].isAvailable = false;
                for(int j = 0; j < *np; ++j)
                {
                    QueuePrem[j][0] = QueuePrem[j + 1][0];
                    QueuePrem[j][1] = QueuePrem[j + 1][1];
                }
                --*np;
            }
        }
    }
    for(int i = 0; i < cashiers - vipCashiers; ++i)
    {
        if(Cashier[i].isAvailable)
        {
            if(*nf > 0)
            {
                Cashier[i].clientID = QueueFree[0][0];
                Cashier[i].cleintType = 'F';
                Cashier[i].timeWithClient = 0;
                Cashier[i].isAvailable = false;
                for(int j = 0; j < *nf; ++j)
                {
                    QueueFree[j][0] = QueueFree[j + 1][0];
                    QueueFree[j][1] = QueueFree[j + 1][1];
                }
                --*nf;
            }
        }
    }
}

void printPart1(FILE *fw, FILE *fw2)
{
    char *buffer = malloc((sizeof(char) * 200));

    sprintf(buffer, "3 uzduotis, 5 variantas. 3 grupe, 2 pogrupis. Hubertas Vindzigalskis.\n");
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "Salyga: 8. VIP bankas.\n");
    printToFiles(buffer, fw, fw2);
    fprintf(fw, "Proceso 1 protokolas.\n");
    fprintf(fw2, "Proceso 2 protokolas.\n");
    sprintf(buffer, "DALIS 1. Duomenys.\n");
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "  1.1. Salyga: VIP bankas.\n");
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "  1.2. Pradiniai duomenys:\n");
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "    1) Kasininku skaicius:% 37d.\n", cashiers);
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "    2) Naujo VIP kliento tikimybe: % 28d %%.\n", chancePrem);
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "    3) Naujo paprasto kliento tikimybe: % 23d %%.\n", chanceFree);
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "    4) Kasininko darbo atlikimo greitis su vienu klientu: % 5d min.\n", timeCashierSpeed);
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "    5) Banko darbo trukme laiko segmentais: T=% 17d min.\n", timeWork);
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "  1.3. Algoritmas:\n");
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "    1) Funkcijoje beginWork() paleidziamas ciklas, kuris sukasi iki %d min.\n", timeWork);
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "    2) Cikle kiekviena karta sukuriamas skirtingas skaicius nauju VIP ir paprastu\n       klientu.\n");
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "    3) Cikle atnaujinamos kasininku busenos.\n");
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "    4) Cikle naujiems kasininkams priskiriami nauji klientai, jei jie laisvi.\n");
    printToFiles(buffer, fw, fw2);
    sprintf(buffer, "    5) Kai kasininkas praleistas laikas su klientu pasiekia %d reiksme,\n       klientas iseina is banko ir kasininkas pradeda darba su nauju klientu.\n", timeCashierSpeed);
    printToFiles(buffer, fw, fw2);


    /* printf("Salyga: 8. VIP bankas.\n");
    fprintf(fw, "Salyga: 8. VIP bankas.\n");
    fprintf(fw2, "Salyga: 8. VIP bankas.\n");
    fprintf(fw, "Proceso 1 protokolas.\n");
    fprintf(fw2, "Proceso 2 protokolas.\n");
    printf("DALIS 1. Duomenys.\n");
    fprintf(fw, "DALIS 1. Duomenys.\n");
    fprintf(fw2, "DALIS 1. Duomenys.\n");
    printf("  1.1. Salyga: VIP bankas.\n");
    fprintf(fw, "  1.1. Salyga: VIP bankas.\n");
    fprintf(fw2, "  1.1. Salyga: VIP bankas.\n");
    printf("  1.2. Pradiniai duomenys:\n");
    fprintf(fw, "  1.2. Pradiniai duomenys:\n");
    fprintf(fw2, "  1.2. Pradiniai duomenys:\n");
    printf("    1) Kasininku skaicius:% 36d.\n", cashiers);
    fprintf(fw, "    1) Kasininku skaicius: % 36d.\n", cashiers);
    fprintf(fw2, "    1) Kasininku skaicius: % 36d.\n", cashiers);
    printf("    2) Naujo VIP kliento tikimybe: % 28d%%.\n", chancePrem);
    fprintf(fw, "    2) Naujo VIP kliento tikimybe: % 28d%%.\n", chancePrem);
    fprintf(fw2, "    2) Naujo VIP kliento tikimybe: % 28d%%.\n", chancePrem);
    printf("    3) Naujo paprasto kliento tikimybe: % 23d%%.\n", chanceFree);
    fprintf(fw, "    3) Naujo paprasto kliento tikimybe: % 23d%%.\n", chanceFree);
    fprintf(fw2, "    3) Naujo paprasto kliento tikimybe: % 23d%%.\n", chanceFree);
    // printf("    4) Laiko segmento trukme sekundemis: %d.\n", timeInterval);
    // fprintf(fw, "    4) Laiko segmento trukme sekundemis: %d.\n", timeInterval);
    // fprintf(fw2, "    4) Laiko segmento trukme sekundemis: %d.\n", timeInterval);
    sprintf(buffer, "", );
    printf("    4) Kasininko darbo atlikimo greitis su vienu klientu: % 5d min.\n", timeCashierSpeed);
    fprintf(fw, "    4) Kasininko darbo atlikimo greitis su vienu klientu: % 5d min.\n", timeCashierSpeed);
    fprintf(fw2, "    4) Kasininko darbo atlikimo greitis su vienu klientu: % 5d min.\n", timeCashierSpeed);
    printf("    5) Banko darbo trukme laiko segmentais: T=% 17d min.\n", timeWork);
    fprintf(fw, "    5) Banko darbo trukme laiko segmentais: T=% 17d min.\n", timeWork);
    fprintf(fw2, "    5) Banko darbo trukme laiko segmentais: T=% 17d min.\n", timeWork);
    printf("  1.3. Algoritmas:\n");
    fprintf(fw, "  1.3. Algoritmas:\n");
    fprintf(fw2, "  1.3. Algoritmas:\n");
    printf("    1) Funkcijoje beginWork() paleidziamas ciklas, kuris sukasi iki %d min.\n", timeWork);
    fprintf(fw, "    1) Funkcijoje beginWork() paleidziamas ciklas, kuris sukasi iki %d min.\n", timeWork);
    fprintf(fw2, "    1) Funkcijoje beginWork() paleidziamas ciklas, kuris sukasi iki %d min.\n", timeWork);
    printf("    2) Cikle kiekviena karta sukuriamas skirtingas skaicius nauju VIP ir paprastu\n       klientu.\n");
    fprintf(fw, "    2) Cikle kiekviena karta sukuriamas skirtingas skaicius nauju VIP ir paprastu\n       klientu.\n");
    fprintf(fw2, "    2) Cikle kiekviena karta sukuriamas skirtingas skaicius nauju VIP ir paprastu\n       klientu.\n");
    printf("    3) Cikle atnaujinamos kasininku busenos.\n");
    fprintf(fw, "    3) Cikle atnaujinamos kasininku busenos.\n");
    fprintf(fw2, "    3) Cikle atnaujinamos kasininku busenos.\n");
    printf("    4) Cikle naujiems kasininkams priskiriami nauji klientai, jei jie laisvi.\n");
    fprintf(fw, "    4) Cikle naujiems kasininkams priskiriami nauji klientai, jei jie laisvi.\n");
    fprintf(fw2, "    4) Cikle naujiems kasininkams priskiriami nauji klientai, jei jie laisvi.\n");
    printf("    5) Kai kasininkas praleistas laikas su klientu pasiekia %d reiksme,\n       klientas iseina is banko ir kasininkas pradeda darba su nauju klientu.\n", timeCashierSpeed);
    fprintf(fw, "    5) Kai kasininkas praleistas laikas su klientu pasiekia %d reiksme,\n       klientas iseina is banko ir kasininkas pradeda darba su nauju klientu.\n", timeCashierSpeed);
    fprintf(fw2, "    5) Kai kasininkas praleistas laikas su klientu pasiekia %d reiksme,\n       klientas iseina is banko ir kasininkas pradeda darba su nauju klientu.\n", timeCashierSpeed); */
}

void printPart2(int NewClientsFree[], int NewClientsPrem[], int QueueFree[][2], int QueuePrem[][2], Cashier Cashier[], int nnf, int nnp, int nf, int np, int nc, FILE *fw)
{
    // Spausdiname protokola
    fprintf(fw, "  T=%d min.\n", timeSegment);
    if (nnp != 0)
    {
        if (nnp == 1)
        {
            fprintf(fw, "    Atejo VIP klientas");
        }
        else
        {
            fprintf(fw, "    Atejo VIP klientai:");
        }

        for (int i = 0; i < nnp; ++i)
        {
            fprintf(fw, " VIP%d", NewClientsPrem[i]);
        }
        fprintf(fw, ".\n");
    }
    if (nnf != 0)
    {
        if (nnf == 1)
        {
            fprintf(fw, "    Atejo paprastas klientas");
        }
        else
        {
            fprintf(fw, "    Atejo paprasti klientai:");
        }
        for (int i = 0; i < nnf; ++i)
        {
            fprintf(fw, " KL%d", NewClientsFree[i]);
        }
        fprintf(fw, ".\n");
    }

    if (np != 0)
    {
        wordWrapCount = 33;
        if (np == 1)
        {
            fprintf(fw, "    Eileje stovi VIP klientas");
        }
        else
        {
            fprintf(fw, "    Eileje stovi VIP klientai:");
        }
        
        for (int i = 0; i < np; ++i)
        {
            wordWrapCount += numberOfDigits(QueueFree[i][0]) + 3;
            if (wordWrapCount > 80)
            {
                fprintf(fw, "\n   ");
                wordWrapCount = 8 + numberOfDigits(QueueFree[i][0]) + 3;
            }
            fprintf(fw, " VIP%d", QueuePrem[i][0]);
            
        }
        fprintf(fw, ".\n");
    }
    else
    {
        // fprintf(fw, "    VIP eile tuscia.\n");
    }
    
    if (nf != 0)
    {
        wordWrapCount = 39;
        if (nf == 1)
        {
            fprintf(fw, "    Eileje stovi paprastas klientas");
        }
        else
        {
            fprintf(fw, "    Eileje stovi paprasti klientai:");
        }
        
        for (int i = 0; i < nf; ++i)
        {
            if (QueueFree[i][1] == 0)
            {
                wordWrapCount += numberOfDigits(QueueFree[i][0]) + 3;
                if (wordWrapCount > 80)
                {
                    fprintf(fw, "\n   ");
                    wordWrapCount = 8 + numberOfDigits(QueueFree[i][0]) + 3;
                }
                fprintf(fw, " KL%d", QueueFree[i][0]);
            }
        }
        fprintf(fw, ".\n");
    }
    else
    {
        // fprintf(fw, "    Paprasta eile tuscia.\n");
    }


    for (int i = 0; i < cashiers; ++i)
    {
        if (Cashier[i].isAvailable == true)
        {
            if (timeSegment < timeWork)
            {
                if (Cashier[i].cleintType == 'P' && Cashier[i].clientID != 0)
                {
                    fprintf(fw, "    Kasininkas KA%d yra baiges aptarnauti VIP klienta VIP%d.\n", i + 1, Cashier[i].clientID);
                }
                else if (Cashier[i].cleintType == 'F' && Cashier[i].clientID != 0)
                {
                    fprintf(fw, "    Kasininkas KA%d yra baiges aptarnauti paprasta klienta KL%d.\n", i + 1, Cashier[i].clientID);
                }
                else
                {
                    fprintf(fw, "    Kasininkas KA%d laisvas.\n", i + 1);
                }
            }
            else
            {
                fprintf(fw, "    Kasininkas KA%d baige darba.\n", i + 1);
            }
        }
        else
        {
            if (Cashier[i].cleintType == 'P')
            {
                fprintf(fw, "    Kasininkas KA%d aptarnauja VIP klienta VIP%d.", i + 1, Cashier[i].clientID);
            }
            else if (Cashier[i].cleintType == 'F')
            {
                fprintf(fw, "    Kasininkas KA%d aptarnauja paprasta klienta KL%d.", i + 1, Cashier[i].clientID);
            }
            // if (Cashier[i].timeWithClient == 0)
            // {
            //     fprintf(fw, " Aptarnavimas jau trunka %d minuciu is %d minuciu.\n", Cashier[i].timeWithClient, timeCashierSpeed);
            // }
            // else if (Cashier[i].timeWithClient == 1)
            // {
            //     fprintf(fw, " Aptarnavimas jau trunka %d minute is %d minuciu.\n", Cashier[i].timeWithClient, timeCashierSpeed);
            // }
            // else
            // {
                fprintf(fw, " Aptarnavimas jau trunka %d min. is %d min.\n", Cashier[i].timeWithClient, timeCashierSpeed);
            // }
        }
    }
    fprintf(fw, "\n");
}

void printPart2Proc2(int NewClientsFree[], int NewClientsPrem[], int QueueFree[][2], int QueuePrem[][2], Cashier Cashier[], int nnf, int nnp, int nf, int np, int nc, FILE *fw)
{
    // Spausdiname protokola
    fprintf(fw, "  Momentas T=%d\n", timeSegment);
    if (nnf != 0)
    {
        fprintf(fw, "    Atejo nauji paprasti klientai:");
        for (int i = 0; i < nnf; ++i)
        {
            fprintf(fw, " %d", NewClientsFree[i]);
        }
        fprintf(fw, ".\n");
    }
    if (nnp != 0)
    {
        fprintf(fw, "    Atejo nauji VIP klientai:");
        for (int i = 0; i < nnp; ++i)
        {
            fprintf(fw, " %d", NewClientsPrem[i]);
        }
        fprintf(fw, ".\n");
    }

    if (nf != 0)
    {
        wordWrapCount = 39;
        fprintf(fw, "    Eileje stovi paprasti klientai:");
        for (int i = 0; i < nf; ++i)
        {
            if (QueueFree[i][1] == 0)
            {
                wordWrapCount += numberOfDigits(QueueFree[i][0]) + 1;
                if (wordWrapCount > 80)
                {
                    fprintf(fw, "\n    ");
                    wordWrapCount = 8 + numberOfDigits(QueueFree[i][0]) + 1;
                }
                fprintf(fw, " %d", QueueFree[i][0]);
            }
        }
        fprintf(fw, ".\n");
    }
    else
    {
        // fprintf(fw, "    Paprasta eile tuscia.\n");
    }

    if (np != 0)
    {
        fprintf(fw, "    Eileje stovi VIP klientai:");
        for (int i = 0; i < np; ++i)
        {
            wordWrapCount += numberOfDigits(QueuePrem[i][0]) + 1;
            if (wordWrapCount > 80)
            {
                fprintf(fw, "\n    ");
                wordWrapCount = 8 + numberOfDigits(QueuePrem[i][0]) + 1;
            }
            fprintf(fw, " %d", QueuePrem[i][0]);
            
        }
        fprintf(fw, ".\n");
    }
    else
    {
        // fprintf(fw, "    VIP eile tuscia.\n");
    }

    for (int i = 0; i < cashiers - vipCashiers; ++i)
    {
        if (Cashier[i].isAvailable == true)
        {
            if (timeSegment < timeWork)
            {
                fprintf(fw, "    Paprastu klientu kasininkas #%d laisvas.\n", i + 1);
            }
            else
            {
                fprintf(fw, "    Paprastu klientu kasininkas #%d baige darba.\n", i + 1);
            }
        }
        else
        {
            if (Cashier[i].cleintType == 'F')
            {
                fprintf(fw, "    Paprastu klientu kasininkas #%d aptarnauja klienta #%d.\n    Aptarnavimas jau trunka %d minutes.\n", i + 1, Cashier[i].clientID, Cashier[i].timeWithClient);
            }
            
        }
    }

    for (int i = cashiers - vipCashiers; i < cashiers; ++i)
    {
        if (Cashier[i].isAvailable == true)
        {
            if (timeSegment < timeWork)
            {
                fprintf(fw, "    VIP klientu kasininkas #%d laisvas.\n", i + 1);
            }
            else
            {
                fprintf(fw, "    VIP klientu kasininkas #%d baige darba.\n", i + 1);
            }
        }
        else
        {
            if (Cashier[i].cleintType == 'P')
            {
                fprintf(fw, "    VIP klientu kasininkas #%d aptarnauja klienta #%d.\n    Aptarnavimas jau trunka %d minutes.\n", i - cashiers + vipCashiers + 1, Cashier[i].clientID, Cashier[i].timeWithClient);
            }
        }
    }
}

void printPart3(int QueueFree[][2], int QueuePrem[][2], int nf, int np, int nc, FILE *fw, FILE *fw2)
{
    char *temp = malloc((sizeof(char) * 200));
    printToFiles("DALIS 3. Rezultatai.\n", fw, fw2);
    sprintf(temp, "  3.1. Procesu 1 ir 2 rezultatai:\n");
    printToFiles(temp, fw, fw2);
    sprintf(temp, "\n                         Duomuo                       |  Proceso 1 balas  |  Proceso 2 balas  |  Efektyvesnis procesas\n");
    printToFiles(temp, fw, fw2);
    sprintf(temp, "  --------------------------------------------------------------------------------------------------------------------\n");
    printToFiles(temp, fw, fw2);
    sprintf(temp, "  Kasininku stoviniavimo laikas                       | % 11d min.  | % 11d min.  | % 15s\n", timeDoingNothingProc1, timeDoingNothing, timeDoingNothingProc1 > timeDoingNothing ? "Procesas 2" : "Procesas 1");
    printToFiles(temp, fw, fw2);
    sprintf(temp, "  Virsvalandziu laikas                                | % 11d min.  | % 11d min.  | % 15s\n", timeOvertimeProc1, timeOvertime, timeOvertimeProc1 > timeOvertime ? "Procesas 2" : "Procesas 1");
    printToFiles(temp, fw, fw2);
    sprintf(temp, "  Vidutinis paprastu klientu praleistas laikas eileje | % 11d min.  | % 11d min.  | % 15s\n", timeAtQueueFreeProc1 / totalFreeClients, timeAtQueueFree / totalFreeClients, timeAtQueueFreeProc1 > timeAtQueueFree ? "Procesas 2" : "Procesas 1");
    printToFiles(temp, fw, fw2);
    sprintf(temp, "  Maksimalus VIP praleistas laikas eileje             | % 11d min.  | % 11d min.  | % 15s\n\n", maxTimeAtQueuePremProc1, maxTimeAtQueuePrem, maxTimeAtQueuePremProc1 > maxTimeAtQueuePrem ? "Procesas 2" : "Procesas 1");
    printToFiles(temp, fw, fw2);
    // printf("  3.1. Proceso rezultatas skaiciuojamas pagal formule: (Suminis kasininku\n       prastovu laikas) + (suminis kasininku virsvalandziu laikas) * 2 +\n       + (vidutinis paprasto kliento laukimo laikas) * 3 + (VIP kliento\n       maksimalus laukimo laikas) * 10.\n");
    // fprintf(fw, "  3.1. Proceso rezultatas skaiciuojamas pagal formule: (Suminis kasininku\n       prastovu laikas) + (suminis kasininku virsvalandziu laikas) * 2 +\n       + (vidutinis paprasto kliento laukimo laikas) * 3 + (VIP kliento\n       maksimalus laukimo laikas) * 10.\n");
    // fprintf(fw2, "  3.1. Proceso rezultatas skaiciuojamas pagal formule: (Suminis kasininku\n       prastovu laikas) + (suminis kasininku virsvalandziu laikas) * 2 +\n       + (vidutinis paprasto kliento laukimo laikas) * 3 + (VIP kliento\n       maksimalus laukimo laikas) * 10.\n");

    // printf("  3.3. Proceso 2 rezultatas: %d\n", process2Score);
    // fprintf(fw, "  3.3. Proceso 2 rezultatas: %d\n", process2Score);
    // fprintf(fw2, "  3.3. Proceso 2 rezultatas: %d\n", process2Score);

    /* if (process1Score < process2Score)
    {
        sprintf(temp, "  3.2. Greitesnis buvo procesas 1.\n");
        printToFiles(temp, fw, fw2);
    }
    else if (process1Score > process2Score)
    {
        sprintf(temp, "  3.2. Greitesnis buvo procesas 2.\n");
        printToFiles(temp, fw, fw2);
    }
    else
    {
        sprintf(temp, "  3.2. Abu procesai buvo vienodai efektyvus.\n");
        printToFiles(temp, fw, fw2);
    } */

    /* printf("Paprastu klientu vidutinis praleistas laikas eileje: %d.\n", timeAtQueueFree / totalFreeClients);
    printf("VIP klientu maksimalus praleistas laikas eileje: %d.\n", maxTimeAtQueuePrem);
    printf("Kasininku praleistas laikas nieko neveikiant: %d.\n", timeDoingNothing);
    printf("Kasininku darbo laikas praleistas po darbo valandu: %d\n", timeOvertime);

    printf("Bendras banko proceso efektyvumo balas procesu 1: %d\n", calculateEfficiencyScore()); */
}

int numberOfDigits(int number)
{
    int digits = 0;
    while (number != 0)
    {
        number /= 10;
        ++digits;
    }
    return digits;
}

bool keepGoing(int QueueFree[][2], int QueuePrem[][2], int nf, int np, int nc, Cashier Cashier[])
{
    if (nf != 0 || np != 0)
    {
        return true;
    }
    for (int i = 0; i < cashiers; ++i)
    {
        if (Cashier[i].isAvailable == false)
        {
            return true;
        }
    }
    return false;
}

void calculateScoreElements(int QueueFree[][2], int QueuePrem[][2], int nf, int np, int nc, Cashier Cashier[], bool isOvertime)
{
    timeAtQueueFree += nf;
    for (int i = 0; i < np; ++i)
    {
        ++QueuePrem[i][1];
        if (QueuePrem[i][1] > maxTimeAtQueuePrem)
        {
            maxTimeAtQueuePrem = QueuePrem[i][1];
        }
    }
    if (!isOvertime)
    {
        for (int i = 0; i < cashiers; ++i)
        {
            if (Cashier[i].isAvailable)
            {
                ++timeDoingNothing;
            }
        }
    }
    else
    {
        for (int i = 0; i < cashiers; ++i)
        {
            if (!Cashier[i].isAvailable)
            {
                ++timeOvertime;
            }
        }
    }
    
}

void printToFiles(char text[], FILE *fw, FILE *fw2)
{
    printf("%s", text);
    fprintf(fw, "%s", text);
    fprintf(fw2, "%s", text);
}

int calculateEfficiencyScore()
{
    return timeDoingNothing + 2 * timeOvertime + 3 * timeAtQueueFree / totalFreeClients + 10 * maxTimeAtQueuePrem;
}