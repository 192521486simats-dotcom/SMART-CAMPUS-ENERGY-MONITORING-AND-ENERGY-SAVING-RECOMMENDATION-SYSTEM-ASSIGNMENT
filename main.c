```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 50
#define DAYS 7

typedef struct
{
    char name[50];
    float energy[DAYS];
    float limit;
    float total;
    float average;
    float savings;
    float score;
    char status[20];
    char recommendation[200];
} Facility;


/* Function Prototypes */
float recursiveSum(float arr[], int index);
void calculateDetails(Facility *f, float reductionTarget);
void classifyFacility(Facility *f);
void generateRecommendation(Facility *f);
void displayFacility(Facility *f);
void displayAll(Facility facilities[], int n);
int searchFacility(Facility facilities[], int n, char name[]);
void sortFacilities(Facility facilities[], int n);
void mergeFacilities(Facility a[], int n1,
                     Facility b[], int n2,
                     Facility merged[], int *n3);
void saveToFile(Facility facilities[], int n);
void loadFromFile(Facility facilities[], int *n);
void generateReport(Facility facilities[], int n);
void findTopConsumers(Facility facilities[], int n);


/* Recursive function to calculate total */
float recursiveSum(float arr[], int index)
{
    if (index == DAYS)
        return 0;

    return arr[index] + recursiveSum(arr, index + 1);
}


/* Classify energy status */
void classifyFacility(Facility *f)
{
    float percentage = (f->total / f->limit) * 100;

    if (percentage <= 80)
        strcpy(f->status, "NORMAL");
    else if (percentage <= 100)
        strcpy(f->status, "HIGH");
    else
        strcpy(f->status, "EXCESSIVE");
}


/* Generate recommendation */
void generateRecommendation(Facility *f)
{
    if (strcmp(f->status, "EXCESSIVE") == 0)
    {
        strcpy(f->recommendation,
               "Conduct an energy audit, reduce unnecessary equipment usage, "
               "and replace inefficient appliances.");
    }
    else if (strcmp(f->status, "HIGH") == 0)
    {
        strcpy(f->recommendation,
               "Switch off unused equipment, improve lighting efficiency, "
               "and monitor peak-hour consumption.");
    }
    else
    {
        strcpy(f->recommendation,
               "Maintain current energy practices and continue regular monitoring.");
    }
}


/* Calculate all details */
void calculateDetails(Facility *f, float reductionTarget)
{
    int i;
    float max, min;
    int maxDay = 0, minDay = 0;

    f->total = recursiveSum(f->energy, 0);
    f->average = f->total / DAYS;

    max = f->energy[0];
    min = f->energy[0];

    for (i = 1; i < DAYS; i++)
    {
        if (f->energy[i] > max)
        {
            max = f->energy[i];
            maxDay = i;
        }

        if (f->energy[i] < min)
        {
            min = f->energy[i];
            minDay = i;
        }
    }

    classifyFacility(f);
    generateRecommendation(f);

    f->savings = f->total * reductionTarget / 100.0;

    f->score = 100 - ((f->total - f->limit) / f->limit * 100);

    if (f->score > 100)
        f->score = 100;

    if (f->score < 0)
        f->score = 0;

    printf("\nHighest Consumption Day : Day %d (%.2f kWh)",
           maxDay + 1, max);

    printf("\nLowest Consumption Day  : Day %d (%.2f kWh)",
           minDay + 1, min);
}


/* Display one facility */
void displayFacility(Facility *f)
{
    int i;

    printf("\n---------------------------------------------");
    printf("\nFacility              : %s", f->name);
    printf("\nPermitted Limit       : %.2f kWh", f->limit);

    printf("\nDaily Consumption:");
    for (i = 0; i < DAYS; i++)
        printf("\n  Day %d : %.2f kWh", i + 1, f->energy[i]);

    printf("\nTotal Consumption     : %.2f kWh", f->total);
    printf("\nAverage Consumption   : %.2f kWh", f->average);
    printf("\nEnergy Status         : %s", f->status);
    printf("\nPotential Savings     : %.2f kWh", f->savings);
    printf("\nSustainability Score  : %.2f / 100", f->score);
    printf("\nRecommendation        : %s", f->recommendation);
    printf("\n---------------------------------------------\n");
}


/* Display all facilities */
void displayAll(Facility facilities[], int n)
{
    int i;

    for (i = 0; i < n; i++)
        displayFacility(&facilities[i]);
}


/* Search facility */
int searchFacility(Facility facilities[], int n, char name[])
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (strcmp(facilities[i].name, name) == 0)
            return i;
    }

    return -1;
}


/* Sort facilities in descending order */
void sortFacilities(Facility facilities[], int n)
{
    int i, j;
    Facility temp;

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (facilities[j].total < facilities[j + 1].total)
            {
                temp = facilities[j];
                facilities[j] = facilities[j + 1];
                facilities[j + 1] = temp;
            }
        }
    }
}


/* Merge two datasets */
void mergeFacilities(Facility a[], int n1,
                     Facility b[], int n2,
                     Facility merged[], int *n3)
{
    int i;

    *n3 = 0;

    for (i = 0; i < n1; i++)
    {
        merged[*n3] = a[i];
        (*n3)++;
    }

    for (i = 0; i < n2; i++)
    {
        merged[*n3] = b[i];
        (*n3)++;
    }
}


/* Save records to file */
void saveToFile(Facility facilities[], int n)
{
    FILE *fp;
    int i, j;

    fp = fopen("energy_records.txt", "w");

    if (fp == NULL)
    {
        printf("\nError opening file.");
        return;
    }

    fprintf(fp, "SMART CAMPUS ENERGY REPORT\n");
    fprintf(fp, "==========================\n\n");

    for (i = 0; i < n; i++)
    {
        fprintf(fp, "Facility: %s\n", facilities[i].name);

        for (j = 0; j < DAYS; j++)
            fprintf(fp, "Day %d: %.2f kWh\n",
                    j + 1, facilities[i].energy[j]);

        fprintf(fp, "Limit: %.2f kWh\n", facilities[i].limit);
        fprintf(fp, "Total: %.2f kWh\n", facilities[i].total);
        fprintf(fp, "Average: %.2f kWh\n", facilities[i].average);
        fprintf(fp, "Status: %s\n", facilities[i].status);
        fprintf(fp, "Savings: %.2f kWh\n", facilities[i].savings);
        fprintf(fp, "Score: %.2f\n", facilities[i].score);
        fprintf(fp, "Recommendation: %s\n\n",
                facilities[i].recommendation);
    }

    fclose(fp);

    printf("\nData successfully saved to energy_records.txt\n");
}


/* Load basic records from binary file */
void loadFromFile(Facility facilities[], int *n)
{
    FILE *fp;

    fp = fopen("energy_data.dat", "rb");

    if (fp == NULL)
    {
        printf("\nNo stored binary data found.\n");
        return;
    }

    fread(n, sizeof(int), 1, fp);
    fread(facilities, sizeof(Facility), *n, fp);

    fclose(fp);

    printf("\nData successfully loaded from energy_data.dat\n");
}


/* Generate sustainability report */
void generateReport(Facility facilities[], int n)
{
    int i;
    float campusTotal = 0;
    float campusAverage;

    for (i = 0; i < n; i++)
        campusTotal += facilities[i].total;

    campusAverage = campusTotal / n;

    printf("\n=============================================");
    printf("\n        CAMPUS SUSTAINABILITY REPORT");
    printf("\n=============================================");

    printf("\nNumber of Facilities : %d", n);
    printf("\nCampus Total Energy  : %.2f kWh", campusTotal);
    printf("\nCampus Average       : %.2f kWh", campusAverage);

    printf("\n\nFacility Status:");

    for (i = 0; i < n; i++)
    {
        printf("\n%-25s %-12s Score: %.2f",
               facilities[i].name,
               facilities[i].status,
               facilities[i].score);
    }

    printf("\n=============================================\n");
}


/* Find top energy-consuming facilities */
void findTopConsumers(Facility facilities[], int n)
{
    int i;
    Facility copy[MAX];

    for (i = 0; i < n; i++)
        copy[i] = facilities[i];

    sortFacilities(copy, n);

    printf("\nTOP ENERGY-CONSUMING FACILITIES\n");
    printf("---------------------------------------------");

    for (i = 0; i < n && i < 3; i++)
    {
        printf("\n%d. %s - %.2f kWh",
               i + 1,
               copy[i].name,
               copy[i].total);
    }

    printf("\n");
}


/* Main function */
int main()
{
    Facility facilities[MAX];
    Facility secondSet[10];
    Facility merged[MAX];

    int n = 0;
    int n2 = 0;
    int mergedCount;
    int choice;
    int i, j;
    int index;

    char searchName[50];
    float reductionTarget;

    printf("=============================================\n");
    printf(" SMART CAMPUS ENERGY MONITORING SYSTEM\n");
    printf("=============================================\n");

    printf("\nEnter number of facilities: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX)
    {
        printf("Invalid number of facilities.\n");
        return 0;
    }

    printf("\nEnter target reduction percentage: ");
    scanf("%f", &reductionTarget);

    /* Input data */
    for (i = 0; i < n; i++)
    {
        printf("\nEnter facility %d name: ", i + 1);
        scanf(" %[^\n]", facilities[i].name);

        printf("Enter permitted weekly energy limit: ");
        scanf("%f", &facilities[i].limit);

        printf("Enter energy consumption for 7 days:\n");

        for (j = 0; j < DAYS; j++)
        {
            printf("Day %d: ", j + 1);
            scanf("%f", &facilities[i].energy[j]);
        }

        calculateDetails(&facilities[i], reductionTarget);
    }


    /* Menu */
    do
    {
        printf("\n\n=============================================");
        printf("\n                 MAIN MENU");
        printf("\n=============================================");
        printf("\n1. Display All Facilities");
        printf("\n2. Search Facility");
        printf("\n3. Sort by Energy Consumption");
        printf("\n4. Find Top Energy Consumers");
        printf("\n5. Generate Sustainability Report");
        printf("\n6. Save Report to File");
        printf("\n7. Merge Another Dataset");
        printf("\n8. Exit");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                displayAll(facilities, n);
                break;

            case 2:
                printf("\nEnter facility name to search: ");
                scanf(" %[^\n]", searchName);

                index = searchFacility(facilities, n, searchName);

                if (index != -1)
                    displayFacility(&facilities[index]);
                else
                    printf("\nFacility not found.\n");

                break;

            case 3:
                sortFacilities(facilities, n);

                printf("\nFacilities sorted by energy consumption:\n");
                displayAll(facilities, n);
                break;

            case 4:
                findTopConsumers(facilities, n);
                break;

            case 5:
                generateReport(facilities, n);
                break;

            case 6:
                saveToFile(facilities, n);
                break;

            case 7:
                printf("\nEnter number of facilities in second dataset: ");
                scanf("%d", &n2);

                if (n2 > 0 && n2 <= 10)
                {
                    for (i = 0; i < n2; i++)
                    {
                        printf("\nEnter facility name: ");
                        scanf(" %[^\n]", secondSet[i].name);

                        printf("Enter weekly limit: ");
                        scanf("%f", &secondSet[i].limit);

                        for (j = 0; j < DAYS; j++)
                        {
                            printf("Day %d: ", j + 1);
                            scanf("%f", &secondSet[i].energy[j]);
                        }

                        calculateDetails(&secondSet[i],
                                         reductionTarget);
                    }

                    mergeFacilities(facilities, n,
                                    secondSet, n2,
                                    merged, &mergedCount);

                    printf("\nMerged dataset:\n");
                    displayAll(merged, mergedCount);
                }
                else
                {
                    printf("\nInvalid dataset size.\n");
                }

                break;

            case 8:
                printf("\nThank you for using Smart Campus Energy Monitoring System.\n");
                break;

            default:
                printf("\nInvalid menu choice.\n");
        }

    } while (choice != 8);

    return 0;
}
```
