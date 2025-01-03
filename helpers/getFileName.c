bool getFileName(char *filename, size_t size, char message[])
{
    printf("%s", message);
    if (fgets(filename, size, stdin) == NULL)
    {
        printf("Error reading input. Please try again.\n");
        return false;
    }

    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n')
    {
        filename[len - 1] = '\0';
    }

    if (!validateCreatingFileName(filename))
    {
        printf("Invalid file name. Only letters, numbers, dots, underscores, and hyphens are allowed.\n");
        return false;
    }

    return true;
}