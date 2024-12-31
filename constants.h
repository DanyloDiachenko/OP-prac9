#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MY_SIGNATURE "MY_SIGNATURE"

#define RECORD_NAME_SIZE 50

typedef struct
{
    char name[RECORD_NAME_SIZE];
    float area;
    float population;
} Record;

#endif // CONSTANTS_H