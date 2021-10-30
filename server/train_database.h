/*
Auther and developer: 

Ashutosh Soni - MT2021026
IIIT Bangalore
email: ashutosh.soni@iiitb.ac.in
*/

#ifndef TRAIN_DATABASE_H
#define TRAIN_DATABASE_H
#include<stdbool.h>

struct train {
    int id;
    char name[80];
    char from_[80];
    char to_[80];
    int capacity;
    int vacancy;
};

struct train_reply{
    int status_code;
    char message[80];
    int total_trains;
};

void add_train(int cfd);
void edit_trn(int cfd);
void preview_trains(int cfd);

#endif