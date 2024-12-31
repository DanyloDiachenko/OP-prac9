void createFile()
{
    char filename[256];
    bool valid;
    FILE *file;

    do
    {
        printf("Enter the name of the file to create (letters, numbers, dots, underscores, and hyphens only) including"
               " '.dat' extension: ");
        if (fgets(filename, sizeof(filename), stdin) == NULL)
        {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n')
        {
            filename[len - 1] = '\0';
        }

        valid = validateFileName(filename);
        if (!valid)
        {
            printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
            continue;
        }

        if (access(filename, F_OK) == 0)
        {
            printf("File '%s' already exists. Please choose a different name.\n", filename);
            valid = false;
            continue;
        }

        file = fopen(filename, "wb");
        if (file == NULL)
        {
            printf("Error creating file: %s\n", strerror(errno));
            valid = false;
        }
    } while (!valid);

    if (fwrite(MY_SIGNATURE, sizeof(char), strlen(MY_SIGNATURE), file) != strlen(MY_SIGNATURE))
    {
        printf("Error writing to file.\n");
    }
    else
    {
        printf("File '%s' created successfully.\n", filename);
    }

    fclose(file);
}
