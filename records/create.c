void createRecord()
{
    char filename[256];
    FILE *file;

    if (!getFileName(filename, sizeof(filename), "Enter the name of the file to add record: "))
    {
        return;
    }

    if (!openAndValidateFile(filename, &file))
    {
        return;
    }

    Record record;
    bool validInput = false;

    do 
    {
        printf("Enter name of the region (max %d characters): ", MAX_RECORD_NAME_SIZE);
        if (fgets(record.name, sizeof(record.name), stdin) != NULL)
        {
            size_t len = strlen(record.name);
            if (len > 0 && record.name[len - 1] == '\n')
            {
                record.name[len - 1] = '\0';
            }
            validInput = true;
        }
        else
        {
            printf("Error reading input. Please try again.\n");
        }
    } while(!validInput);

    validInput = false;
    do
    {
        printf("Enter area: ");
        if (scanf("%f", &record.area) == 1 && record.area > 0 && record.area <= MAX_RECORD_AREA)
        {
            fflush(stdin);
            validInput = true;
        }
        else
        {
            printf("Invalid input. Area must be a positive number and less than or equal to %.2f.\n", MAX_RECORD_AREA);
            fflush(stdin);
        }
    } while(!validInput);

    validInput = false;
    do
    {
        printf("Enter population: ");
        if (scanf("%f", &record.population) == 1 && record.population > 0 && record.population <= MAX_RECORD_POPULATION)
        {
            fflush(stdin);
            validInput = true;
        }
        else
        {
            printf("Invalid input. Population must be a positive number and less than or equal to %.2f.\n", MAX_RECORD_POPULATION);
            fflush(stdin);
        }
    }while(!validInput);

    fwrite(&record, sizeof(Record), 1, file);
    printf("Record added successfully.\n");

    fclose(file);
}
