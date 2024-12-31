#include "./includes.h"

void menu()
{
    int choice;

    do
    {
        printf("\nMenu:\n");
        printf("1. Create file\n");
        printf("2. Read file\n");
        printf("3. Delete file\n");
        printf("4. Add record\n");
        printf("5. Read single record\n");
        printf("6. Update record\n");
        printf("7. Sort records\n");
        printf("8. Insert record\n");
        printf("9. Delete record\n");
        printf("0. Exit\n");

        int validChoice;
        do
        {
            printf("Enter your choice: ");
            validChoice = getchar();
            if (validChoice != 1)
            {
                printf("Invalid input. Please enter a number.\n");
                fflush(stdin);
                continue;
            }
            fflush(stdin);
        } while (validChoice != 1);

        switch (choice)
        {
        case 1:
            createFile();
            break;
        case 2:
            readFile();
            break;
        case 3:
            deleteFile();
            break;
        case 4:
            addRecord();
            break;
        case 5:
        {
            readSingleRecord();
            break;
        }
        case 6:
        {
            editRecord();
            break;
        }
        case 7:
        {
            sortRecords();
            break;
        }
        case 8:
        {
            insertRecord();
            break;
        }
        case 9:
        {
            deleteRecord();
            break;
        }
        case 0:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}
