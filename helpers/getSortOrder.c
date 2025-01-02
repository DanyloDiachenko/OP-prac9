int getSortOrder()
{
    int order;
    bool validity = false;

    printf("Choose the sort order:\n");
    printf("1 - Ascending\n");
    printf("2 - Descending\n");

    do
    {
        printf("Enter your choice (1-2): ");
        if (scanf("%d", &order) == 1 && (order == 1 || order == 2))
        {
            fflush(stdin);
            validity = true;
        }
        else
        {
            printf("Invalid input. Please enter 1 or 2.\n");
            fflush(stdin);
            validity = false;
        }
    } while (!validity);

    return (order == 1);
}
