void createFile()
{
    char filename[MAX_FILE_NAME_LENGTH];
    FILE *file;
    bool validity;

    do
    {
        printf("Enter the name of the file to create (letters, numbers, dots, underscores, and hyphens only) including '.dat' extension: ");
        validity = validateFileName(filename, sizeof(filename));

        if (!validity)
            continue;

        if (validateFileExisting(filename))
        {
            printf("File '%s' already exists. Please choose a different name.\n", filename);
            validity = false;
            continue;
        }

        file = fopen(filename, "wb");
        if (file == NULL)
        {
            printf("Error creating file: %s\n", strerror(errno));
            validity = false;
            continue;
        }

        if (fwrite(MY_SIGNATURE, sizeof(char), strlen(MY_SIGNATURE), file) != strlen(MY_SIGNATURE))
        {
            printf("Error writing to file.\n");
            validity = false;
        }
        else
        {
            printf("File '%s' created successfully.\n", filename);
            validity = true;
        }

        fclose(file);
    } while (!validity);
}