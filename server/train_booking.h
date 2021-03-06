/*
Auther and developer: 

Ashutosh Soni - MT2021026
IIIT Bangalore
email: ashutosh.soni@iiitb.ac.in
*/

#ifndef TRAIN_BOOKING_H
#define TRAIN_BOOKING_H
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

struct booking_reply{
    int status_code;
    char message[80];
    int total_bookings;
};

void book_ticket(int cfd);
void edit_ticket(int cfd);
void cancel_ticket(int cfd);
void preview_bookings(int cfd);
bool check_availability(int train_id, int req, int lastN, char type);
bool update_vacancy(int train_id, int n);

#endif