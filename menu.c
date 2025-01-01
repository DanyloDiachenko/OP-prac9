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
        bool validInput = false;

        do
        {
            printf("Enter your choice: ");
            validChoice = getchar();
            fflush(stdin);

            if (validChoice >= '0' && validChoice <= '9')
            {
                validInput = true;
                choice = validChoice - '0';
            }
            else
            {
                printf("Invalid input. Please enter a number between 0 and 9.\n");
            }
        } while (!validInput);

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
            createRecord();
            break;
        case 5:
            readSingleRecord();
            break;
        case 6:
            updateRecord();
            break;
        case 7:
            sortRecords();
            break;
        case 8:
            insertRecord();
            break;
        case 9:
            deleteRecord();
            break;
        case 0:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}
