bool validateFileSignature(FILE *file)
{
    const int signatureLength = strlen(MY_SIGNATURE);
    char signature[signatureLength + 1];

    if (fread(signature, sizeof(char), signatureLength, file) != signatureLength)
    {
        printf("Error reading file signature.\n");
        return false;
    }
    signature[signatureLength] = '\0';

    if (strcmp(signature, MY_SIGNATURE) != 0)
    {
        printf("Invalid file format.\n");
        return false;
    }

    return true;
}