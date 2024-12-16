#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char name[50];
    float area;
    float population;
} Record;

void createFile(const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error creating file.\n");

        return;
    }

    printf("File '%s' created successfully.\n", filename);

    fclose(file);
}

void readFile(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    Record record;
    printf("\nRecords in file:\n");
    while (fread(&record, sizeof(Record), 1, file))
    {
        printf("Name: %s, Area: %.2f, Population: %.2f\n", record.name, record.area, record.population);
    }

    fclose(file);
}

void addRecord(const char *filename)
{
    FILE *file = fopen(filename, "ab");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    Record record;
    printf("Enter name of the region: ");
    scanf("%49s", record.name);
    printf("Enter area: ");
    scanf("%f", &record.area);
    printf("Enter population: ");
    scanf("%f", &record.population);

    fwrite(&record, sizeof(Record), 1, file);
    printf("Record added successfully.\n");
    fclose(file);
}

void deleteFile(const char *filename)
{
    if (remove(filename) == 0)
    {
        printf("File '%s' deleted successfully.\n", filename);
    }
    else
    {
        printf("Error deleting file or file does not exist.\n");
    }
}

void readSingleRecord(const char *filename, int index)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    Record record;
    fseek(file, index * sizeof(Record), SEEK_SET);
    if (fread(&record, sizeof(Record), 1, file))
    {
        printf("Name: %s, Area: %.2f, Population: %.2f\n", record.name, record.area, record.population);
    }
    else
    {
        printf("No record found at index %d.\n", index);
    }

    fclose(file);
}

void editRecord(const char *filename, int index)
{
    FILE *file = fopen(filename, "r+b");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    Record record;
    fseek(file, index * sizeof(Record), SEEK_SET);
    if (fread(&record, sizeof(Record), 1, file))
    {
        printf("Editing record at index %d:\n", index);
        printf("Current Name: %s, Area: %.2f, Population: %.2f\n", record.name, record.area, record.population);

        printf("Enter new name: ");
        scanf("%49s", record.name);
        printf("Enter new area: ");
        scanf("%f", &record.area);
        printf("Enter new population: ");
        scanf("%f", &record.population);

        fseek(file, index * sizeof(Record), SEEK_SET);
        fwrite(&record, sizeof(Record), 1, file);
        printf("Record updated successfully.\n");
    }
    else
    {
        printf("No record found at index %d.\n", index);
    }

    fclose(file);
}

void sortRecords(const char *filename, int ascending)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = ftell(file) / sizeof(Record);
    fseek(file, 0, SEEK_SET);

    Record *records = (Record *)malloc(count * sizeof(Record));
    if (records == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(records, sizeof(Record), count, file);
    fclose(file);

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            int condition = ascending ? (strcmp(records[j].name, records[j + 1].name) > 0) : (strcmp(records[j].name, records[j + 1].name) < 0);
            if (condition)
            {
                Record temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }

    file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        free(records);
        return;
    }

    fwrite(records, sizeof(Record), count, file);
    fclose(file);
    free(records);

    printf("Records sorted %s successfully.\n", ascending ? "in ascending order" : "in descending order");
}

void insertRecord(const char *filename, Record newRecord)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = ftell(file) / sizeof(Record);
    fseek(file, 0, SEEK_SET);

    Record *records = (Record *)malloc((count + 1) * sizeof(Record));
    if (records == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(records, sizeof(Record), count, file);
    fclose(file);

    int i;
    for (i = count - 1; i >= 0 && strcmp(records[i].name, newRecord.name) > 0; i--)
    {
        records[i + 1] = records[i];
    }
    records[i + 1] = newRecord;

    file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        free(records);
        return;
    }

    fwrite(records, sizeof(Record), count + 1, file);
    fclose(file);
    free(records);

    printf("Record inserted successfully.\n");
}

void deleteRecord(const char *filename, int index)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = ftell(file) / sizeof(Record);
    fseek(file, 0, SEEK_SET);

    if (index < 0 || index >= count)
    {
        printf("Invalid index.\n");
        fclose(file);
        return;
    }

    Record *records = (Record *)malloc(count * sizeof(Record));
    if (records == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return;
    }

    fread(records, sizeof(Record), count, file);
    fclose(file);

    for (int i = index; i < count - 1; i++)
    {
        records[i] = records[i + 1];
    }

    file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        free(records);
        return;
    }

    fwrite(records, sizeof(Record), count - 1, file);
    fclose(file);
    free(records);

    printf("Record deleted successfully.\n");
}

void menu()
{
    const char *filename = "regions.dat";
    int choice;

    do
    {
        printf("\nMenu:\n");
        printf("1. Create file\n");
        printf("2. Read file\n");
        printf("3. Delete file\n");
        printf("4. Add record\n");
        printf("5. Read single record\n");
        printf("6. Edit record\n");
        printf("7. Sort records\n");
        printf("8. Insert record\n");
        printf("9. Delete record\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createFile(filename);
            break;
        case 2:
            readFile(filename);
            break;
        case 3:
            deleteFile(filename);
            break;
        case 4:
            addRecord(filename);
            break;
        case 5:
        {
            int index;
            printf("Enter record index: ");
            scanf("%d", &index);
            readSingleRecord(filename, index);
            break;
        }
        case 6:
        {
            int index;
            printf("Enter record index to edit: ");
            scanf("%d", &index);
            editRecord(filename, index);
            break;
        }
        case 7:
        {
            int ascending;
            printf("Enter 1 for ascending or 0 for descending order: ");
            scanf("%d", &ascending);
            sortRecords(filename, ascending);
            break;
        }
        case 8:
        {
            Record newRecord;
            printf("Enter name of the region: ");
            scanf("%49s", newRecord.name);
            printf("Enter area: ");
            scanf("%f", &newRecord.area);
            printf("Enter population: ");
            scanf("%f", &newRecord.population);
            insertRecord(filename, newRecord);
            break;
        }
        case 9:
        {
            int index;
            printf("Enter record index to delete: ");
            scanf("%d", &index);
            deleteRecord(filename, index);
            break;
        }
        case 0:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

int main()
{
    menu();
    return 0;
}
