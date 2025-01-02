void sortRecordArray(Record *records, int count, int sortField, int isAscending)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (compareRecords(&records[i], &records[j], sortField, isAscending) > 0)
            {
                Record temp = records[i];
                records[i] = records[j];
                records[j] = temp;
            }
        }
    }
}