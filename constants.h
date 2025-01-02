#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MY_SIGNATURE "MY_SIGNATURE"

#define MAX_RECORD_NAME_SIZE 50
#define MAX_RECORD_AREA 1e9
#define MAX_RECORD_POPULATION 1e12

#define MAX_FILE_NAME_LENGTH 256

typedef struct
{
    char name[MAX_RECORD_NAME_SIZE];
    float area;
    float population;
} Record;

typedef enum
{
    MENU_EXIT = 0,
    MENU_CREATE_FILE,
    MENU_READ_FILE,
    MENU_DELETE_FILE,
    MENU_ADD_RECORD,
    MENU_READ_SINGLE_RECORD,
    MENU_UPDATE_RECORD,
    MENU_SORT_RECORDS,
    MENU_INSERT_RECORD,
    MENU_DELETE_RECORD
} MenuOption;

typedef enum
{
    SORT_NAME = 1,
    SORT_AREA,
    SORT_POPULATION
}

#endif // CONSTANTS_H