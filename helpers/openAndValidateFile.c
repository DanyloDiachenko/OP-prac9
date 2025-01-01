bool openAndValidateFile(const char *filename, FILE **file)
{
    *file = fopen(filename, "rb+");
    if (*file == NULL)
    {
        printf("Error opening file: %s\n", strerror(errno));
        return false;
    }

    int signatureLength = strlen(MY_SIGNATURE);
    char signature[signatureLength + 1];

    if (fread(signature, sizeof(char), signatureLength, *file) != signatureLength)
    {
        printf("Invalid file format or file is corrupted.\n");
        fclose(*file);
        return false;
    }
    signature[signatureLength] = '\0';

    if (strcmp(signature, MY_SIGNATURE) != 0)
    {
        printf("Invalid file format.\n");
        fclose(*file);
        return false;
    }

    fseek(*file, 0, SEEK_END);
    
    return true;
}