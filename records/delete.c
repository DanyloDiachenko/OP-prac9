void deleteRecord()
{
    char filename[MAX_FILE_NAME_LENGTH];
    FILE *file;

    if (!getFileName(filename, sizeof(filename), "Enter the name of the file to delete a record from: "))
    {
        return;
    }

    if (!openAndValidateFile(filename, &file))
    {
        return;
    }

    int signatureLength = strlen(MY_SIGNATURE);
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

    int index = 0;
    bool validInput = false;

    do
    {
        printf("Enter the index of the record to delete (1 to %d): ", count);
        if (scanf("%d", &index) == 1 && index >= 1 && index <= count)
        {
            fflush(stdin);
            validInput = true;
        }
        else
        {
            printf("Invalid input. Index must be between 1 and %d. Try again.\n", count);
            fflush(stdin);
        }
    }while(!validInput);

    index--;

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

    fwrite(MY_SIGNATURE, sizeof(char), signatureLength, file);
    fwrite(records, sizeof(Record), count - 1, file);
    fclose(file);
    free(records);

    printf("Record at index %d deleted successfully from file '%s'.\n", index + 1, filename);
}
