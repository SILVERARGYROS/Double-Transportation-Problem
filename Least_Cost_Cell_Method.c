// Algorithm Sources
// Algorithm example - GeeksforGeeks: https://www.geeksforgeeks.org/transportation-problem-set-3-least-cost-cell-method/?ref=lbp 

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Debug constants
#define DEBUG 1
#define CLEAR_SCREEN 0
#define DEBUG_SHOW_TABLES 1
#define DEBUG_CORE_DUMP 1
#define DEBUG_VALUES 1

// Color palette, source:https://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
#define RED		"\x1B[31m"
#define BLUE	"\x1B[36m"
#define GREEN	"\x1B[32m"
#define DEF		"\x1B[0m"

// Global variables
int i;
int j;
int n;
int m;
int step = 0;
int* supply;
int* demand;
int** table1;
int** table2;
int** record;
int total = 0;
clock_t start, end;
double cpu_time_used;

// Supporting functions
void printTable(int** table);
void leastCostMethod(int** table);
int MoreDemand();

int main(void)
{
    if(CLEAR_SCREEN){system("tput clear");}
    srand(time(NULL));

    // Asking for input
    printf("Please enter value for n: ");
    do{
        scanf("%d", &n);
        if(n <= 0){
            printf("Please enter a proper positive integer: ");
        }
    }while(n <= 0);

    printf("Please enter value for m: ");
    do{
        scanf("%d", &m);
        if(m <= 0){
            printf("Please enter a proper positive integer: ");
        }
    }while(m <= 0);
    // Initializing all necessary arrays
    supply = (int*)malloc(sizeof(int)*n);
    demand = (int*)malloc(sizeof(int)*n);
    table1 = (int**)malloc(sizeof(int*)*n);
    table2 = (int**)malloc(sizeof(int*)*n);

    record = (int**)malloc(sizeof(int*)*n*n);
    for(i = 0; i < n*n; i++)
    {
        record[i] = (int*)malloc(sizeof(int)*2);
    }

    for(i = 0; i < n; i++)
    {
        table1[i] = (int*)malloc(sizeof(int)*n);
        table2[i] = (int*)malloc(sizeof(int)*n);
    }

    for(i = 0; i < n; i++)
    {
        supply[i] = m;
        demand[i] = m;
        for(j = 0; j < n; j++)
        {
            table1[i][j] = rand() % 21 + 1;
            table2[i][j] = rand() % 21 + 1;
        }
    }

    if(DEBUG_SHOW_TABLES){printf("PRINTING TABLES\n"); printf("TABLE 1\n"); printTable(table1); printf("TABLE 2\n"); printTable(table2);}


    // Setting up timer to measure execution time of Dikjstra. 
    start = clock();

    leastCostMethod(table1);

    // Re-Initialize those arrays in particular
    for(i = 0; i < n; i++)
    {
        supply[i] = m;
        demand[i] = m;
    }
    record[step][0] = -1;
    record[step][1] = -1;
    step++;
    leastCostMethod(table2);

    if(DEBUG){for(i = 0; i < step; i++){printf(RED "record = %d , %d\n " DEF, record[i][0], record[i][1]);}}

    int switchedTable = 0;
    printf(GREEN "\nTotal cost is %d.\n\n" DEF, total);
    printf(BLUE "Steps in order:\n");
    for(i = 0; i < step; i++)
    {
        if(record[i][0] == -1 && record[i][1] == -1)
        {
            switchedTable = 1;
            continue;
        }

        if(switchedTable)
        {
            printf("Step: %d, warehouse %d supplied shop %d with cost %d\n", i, record[i][0], record[i][1], table2[record[i][0]][record[i][1]]);
        }
        else
        {
            printf("Step: %d, factory %d supplied warehouse %d with cost %d\n", i, record[i][0], record[i][1], table1[record[i][0]][record[i][1]]);
        }
    }
    printf("\n" DEF);

    if(DEBUG_CORE_DUMP){printf(RED "Debug at the end core dump\n" DEF);}

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Least Cell Cost algorithm was successfully executed in %lf seconds.\n", cpu_time_used);

    return 0;
}

void printTable(int** table){
    printf("    STEP = %d\n", step);
    j = 0;
    for(i = 0; i < n; i++)
    {
        if(j == 0)
        {
            printf(GREEN "%d   " DEF, supply[i]);
        }

        for(j = 0; j < n; j++)
        {
            printf(" %d ", table[i][j]);
        }
        printf("\n");   
        j = 0;
    }
    printf("    ");
    for(i = 0; i < n; i++)
    {
        printf(RED " %d " DEF, demand[i]);
    }
    printf("\n");   
}

void leastCostMethod(int** table){
    int mini;
    int minj;
    int min;

    do{
        mini = 0;
        minj = 0;
        int foundAnything = 0;

        printTable(table);
        

        min = 99999;
        // finding smallest value
        for(i = 0; i < n; i++)
        {
            for(j = 0; j < n; j++)
            {
                if(table[i][j] < min && supply[i] > 0 && demand[j] > 0)
                {
                    min = table[i][j];
                    mini = i;
                    minj = j;
                    foundAnything = 1;
                    if(DEBUG_VALUES){printf("DEBUG mini = %d minj = %d\n", mini, minj);}
                }
            }
        }

        if(foundAnything)
        {
            // computing the total, supply, demand, record
            total += supply[mini] * table[mini][minj];
            supply[mini] = 0;
            demand[minj] = 0;
            record[step][0] = mini;
            record[step][1] = minj;
            step++;

            if(DEBUG_VALUES){printf(GREEN "DEBUG total = %d\n" DEF, total);}
        }

        if(!MoreDemand())
        {
            break;
            if(DEBUG_CORE_DUMP){printf(RED "Debug after break;\n" DEF);}
        }

    }while(1);
}

int MoreDemand()
{
    int k, count = 0;
    for(k = 0; k < n; k++)
    {
        count += demand[k];
    }
    return count;
}
