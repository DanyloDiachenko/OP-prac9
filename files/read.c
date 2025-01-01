void readFile()
{
    char filename[256];
    FILE *file;

    while (true)
    {
        printf("Enter the name of the file to read: ");
        if (!validateAndSanitizeFileName(filename, sizeof(filename)))
            continue;

        file = fopen(filename, "rb");
        if (file == NULL)
        {
            printf("Error opening file or file does not exist: %s\n", strerror(errno));
            continue;
        }

        if (!checkFileSignature(file))
        {
            fclose(file);
            return;
        }

        Record record;
        printf("\nRecords in file:\n");
        while (fread(&record, sizeof(Record), 1, file) == 1)
        {
            printf("Name: %s, Area: %.2f, Population: %.2f\n", record.name, record.area, record.population);
        }

        if (feof(file))
        {
            printf("End of file reached.\n");
        }
        else
        {
            printf("Error reading file.\n");
        }

        fclose(file);
        break;
    }
}