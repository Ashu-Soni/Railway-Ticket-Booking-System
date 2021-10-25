#ifndef TRAINS_H
#define TRAINS_H

struct booking_reply book(int sd);
struct booking_reply edit(int sd);
struct booking_reply cancel(int sd);
void preview_bks(int sd);

struct train_booking_db take_info();

#endif