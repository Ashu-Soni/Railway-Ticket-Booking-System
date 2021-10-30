/*
Auther and developer: 

Ashutosh Soni - MT2021026
IIIT Bangalore
email: ashutosh.soni@iiitb.ac.in
*/

#ifndef TRAINS_H
#define TRAINS_H
#include "users.h"

struct booking_reply book(int sd, struct user_info *user);
struct booking_reply edit(int sd, struct user_info *user);
struct booking_reply cancel(int sd, struct user_info *user);
void preview_bks(int sd, struct user_info *user);

struct train_reply add(int sd);
struct train_reply edit_train(int sd);
void preview_trns(int sd);

struct train_booking_db take_info();
struct train take_train_info();

#endif