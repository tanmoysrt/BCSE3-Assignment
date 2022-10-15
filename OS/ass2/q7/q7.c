#include <stdio.h>

int max[10][10];
int allocation[10][10];
int need[10][10];
int available[10];
int request[10];
int finish[10];

int main()
{
    int i, j, k, n, m, p, flag, count = 0;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the number of resources: ");
    scanf("%d", &m);
    printf("Enter the allocation matrix:", n, m);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            scanf("%d", &allocation[i][j]);
        }
    }
    printf("Enter the max matrix:", n, m);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            scanf("%d", &max[i][j]);
        }
    }
    printf("Enter the available matrix:", m);
    for (i = 0; i < m; i++)
    {
        scanf("%d", &available[i]);
    }
    printf("Enter the request matrix:", m);
    for (i = 0; i < m; i++)
    {
        scanf("%d", &request[i]);
    }
    for (i = 0; i < n; i++)
    {
        finish[i] = 0;
    }
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    do
    {
        flag = 0;
        for (i = 0; i < n; i++)
        {
            if (finish[i] == 0)
            {
                p = 0;
                for (j = 0; j < m; j++)
                {
                    if (need[i][j] > available[j])
                    {
                        p = 1;
                        break;
                    }
                }
                if (p == 0)
                {
                    for (j = 0; j < m; j++)
                    {
                        available[j] += allocation[i][j];
                    }
                    finish[i] = 1;
                    flag = 1;
                }
            }
        }
        count++;
    } while (flag != 0 && count < n);
    if (count < n)
    {
        printf("The system is in safe state.");
    }
    else
    {
        printf("The system is in unsafe state.");
    }
    return 0;
}