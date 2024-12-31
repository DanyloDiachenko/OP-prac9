#include "./includes.h";


void deleteRecord()
{
    char filename[256];
    int index;
    FILE *file;

    printf("Enter the name of the file to delete a record from: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL)
    {
        printf("Error reading input. Operation aborted.\n");
        return;
    }

    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n')
    {
        filename[len - 1] = '\0';
    }

    if (!isValidFileName(filename))
    {
        printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
        return;
    }

    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    const char *expectedSignature = "MY_SIGNATURE";
    size_t signatureLength = strlen(expectedSignature);
    char fileSignature[signatureLength + 1];
    memset(fileSignature, 0, sizeof(fileSignature));

    fread(fileSignature, sizeof(char), signatureLength, file);
    if (strcmp(fileSignature, expectedSignature) != 0)
    {
        printf("Invalid file format. Operation aborted.\n");
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = (ftell(file) - signatureLength) / sizeof(Record);
    if (count == 0)
    {
        printf("The file contains no records. Nothing to delete.\n");
        fclose(file);
        return;
    }
    fseek(file, signatureLength, SEEK_SET);

    Record *records = (Record *)malloc(count * sizeof(Record));
    if (records == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(records, sizeof(Record), count, file);
    fclose(file);

    // Изменяем вывод так, чтобы индексация начиналась с 1
    printf("Enter the index of the record to delete (1 to %d): ", count); // Индексация от 1
    while (scanf("%d", &index) != 1 || index < 1 || index > count)        // Ввод индекса от 1 до count
    {
        printf("Invalid input. Index must be between 1 and %d. Try again: ", count);
        fflush(stdin);
    }
    fflush(stdin);

    // Преобразуем индекс пользователя в индекс для массива (индексация с 0)
    index--; // Уменьшаем на 1, чтобы работать с индексами массива (начиная с 0)

    for (int i = index; i < count - 1; i++)
    {
        records[i] = records[i + 1];
    }

    file = fopen(filename, "wb");

    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        free(records);
        return;
    }

    fwrite(expectedSignature, sizeof(char), signatureLength, file);
    fwrite(records, sizeof(Record), count - 1, file);
    fclose(file);
    free(records);

    printf("Record at index %d deleted successfully from file '%s'.\n", index + 1, filename); // Выводим индекс с 1
}

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
        printf("6. Edit record\n");
        printf("7. Sort records\n");
        printf("8. Insert record\n");
        printf("9. Delete record\n");
        printf("0. Exit\n");

        int validChoice;
        do
        {
            printf("Enter your choice: ");
            validChoice = scanf("%d", &choice);
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

int main()
{
    menu();

    return 0;
}
