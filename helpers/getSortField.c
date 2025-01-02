int getSortField()
{
    int field;
    bool validity = false;

    printf("Choose the field to sort by:\n");
    printf("1 - Name\n");
    printf("2 - Area\n");
    printf("3 - Population\n");

    do
    {
        printf("Enter your choice (1-3): ");
        if (scanf("%d", &field) == 1 && field >= 1 && field <= 3)
        {
            fflush(stdin);
            validity = true;
        }
        else
        {
            printf("Invalid input. Please enter 1, 2, or 3.\n");
            fflush(stdin);
            validity = false;
        }
    } while (!validity);

    return field;
}
