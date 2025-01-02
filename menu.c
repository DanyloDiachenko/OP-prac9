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
        case MENU_CREATE_FILE:
            createFile();
            break;
        case MENU_READ_FILE:
            readFile();
            break;
        case MENU_DELETE_FILE:
            deleteFile();
            break;
        case MENU_ADD_RECORD:
            createRecord();
            break;
        case MENU_READ_SINGLE_RECORD:
            readSingleRecord();
            break;
        case MENU_UPDATE_RECORD:
            updateRecord();
            break;
        case MENU_SORT_RECORDS:
            sortRecords();
            break;
        case MENU_INSERT_RECORD:
            insertRecord();
            break;
        case MENU_DELETE_RECORD:
            deleteRecord();
            break;
        case MENU_EXIT:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}
