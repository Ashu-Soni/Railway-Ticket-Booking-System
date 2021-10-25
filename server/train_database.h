#ifndef TRAIN_DATABASE_H
#define TRAIN_DATABASE_H
#include<stdbool.h>

struct train_booking_db{
    int booking_id;
    int user_id;
    int agent_id;
    int train_id;
    int total_passanger;
    char source[80];
    char destination[80];
    char booking_status;
};

struct train{
    int train_id;
    char train_name[80];
    int capacity;
    int vacancy;
    char status;
};

struct booking_reply{
    int status_code;
    char message[80];
    int total_bookings;
};

void book_ticket(int cfd);
void edit_ticket(int cfd);
void cancel_ticket(int cfd);
void preview_bookings(int cfd);

#endif