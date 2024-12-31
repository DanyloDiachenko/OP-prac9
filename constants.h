#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MY_SIGNATURE "MY_SIGNATURE"

#define MAX_RECORD_NAME_SIZE 50
#define MAX_RECORD_AREA 1e9
#define MAX_RECORD_POPULATION 1e12

typedef struct
{
    char name[MAX_RECORD_NAME_SIZE];
    float area;
    float population;
} Record;

#endif // CONSTANTS_H