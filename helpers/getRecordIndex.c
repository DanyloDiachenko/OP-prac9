#include <stdio.h>
#include <stdbool.h>

void getRecordIndex(int *index, int recordCount, char message[])
{
    int inputIndex;
    bool validInput = false;

    do
    {
        printf("%s (1-%d): ", message, recordCount);

        if (scanf("%d", &inputIndex) == 1 && inputIndex >= 1 && inputIndex <= recordCount)
        {
            fflush(stdin);
            *index = inputIndex - 1;
            validInput = true;
        }
        else
        {
            printf("Invalid input. Please enter a number between 1 and %d.\n", recordCount);
            fflush(stdin);
        }
    } while (!validInput);
}
