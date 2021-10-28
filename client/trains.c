#include "trains.h"
#include "../server/train_booking.h"
#include "../server/train_database.h"
#include "users.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

struct booking_reply book(int sd, struct user_info *user){
    struct booking_reply brpy;

    write(sd, "book ticket", sizeof("book ticket"));

    struct train_booking_db tb;
    tb=take_info();
    tb.agent_id=(*user).agent_id;
    tb.user_id=(*user).user_id;

    write(sd, &tb, sizeof(tb));

    read(sd, &brpy, sizeof(brpy));
    
    return brpy;
}

struct booking_reply edit(int sd, struct user_info *user){
    struct booking_reply brpy;

    write(sd, "edit ticket", sizeof("edit ticket"));

    struct train_booking_db tb;
    tb=take_info();
    tb.agent_id=(*user).agent_id;
    tb.user_id=(*user).user_id;
    printf("Enter booking ID: ");
    scanf("%d", &tb.booking_id);

    write(sd, &tb, sizeof(tb));

    read(sd, &brpy, sizeof(brpy));
    
    return brpy;
}

struct booking_reply cancel(int sd, struct user_info *user){
    struct booking_reply brpy;

    write(sd, "cancel ticket", sizeof("cancel ticket"));

    struct train_booking_db tb;
    
    printf("Enter booking ID: ");
    scanf("%d", &tb.booking_id);
    tb.agent_id=(*user).agent_id;
    tb.user_id=(*user).user_id;

    write(sd, &tb, sizeof(tb));

    read(sd, &brpy, sizeof(brpy));
    return brpy;
}

void preview_bks(int sd, struct user_info *user){
    struct booking_reply brpy;
    struct train_booking_db tb;

    write(sd, "preview bookings", sizeof("preview bookings"));

    tb.agent_id=(*user).agent_id;
    tb.user_id=(*user).user_id;

    write(sd, &tb, sizeof(tb));

    read(sd, &brpy, sizeof(brpy));
    if (brpy.status_code != 200)
    {
        printf("Error getting booking information\n");
    }
    else if(brpy.total_bookings>0)
    {
        printf("In the preview\n");
        struct train_booking_db bookings[brpy.total_bookings];
        read(sd, &bookings, sizeof(bookings));

        printf("---------------------------------------------------------------------------------------------------\n");
        printf("|                                 All Booking Information                                          |\n");
        printf("---------------------------------------------------------------------------------------------------\n");
        printf("|  Booking ID | User ID | Agent ID | Train ID | Total tickets |   Source   | Destination | status  |\n");
        printf("---------------------------------------------------------------------------------------------------\n");
        for (int i = 0; i < brpy.total_bookings; i++)
        {
            printf("|  %10d | %7d | %8d | %8d | %13d | %10s | %11s | %6c  |\n", bookings[i].booking_id, bookings[i].user_id, bookings[i].agent_id, bookings[i].train_id, bookings[i].total_passanger, bookings[i].source, bookings[i].destination, bookings[i].booking_status);
        }
        printf("---------------------------------------------------------------------------------------------------\n");
    }
}

struct train_booking_db take_info(){
    struct train_booking_db tb;

    printf("Enter train ID: ");
    scanf("%d", &tb.train_id);
    printf("Enter number of tickets: ");
    scanf("%d", &tb.total_passanger);
    printf("Enter source: ");
    scanf("%s", tb.source);
    printf("Enter destination: ");
    scanf("%s", tb.destination);

    return tb;
}

struct train_reply add(int sd){
    struct train_reply trpy;

    write(sd, "add new train", sizeof("add new train"));

    struct train trn=take_train_info();
    trn.vacancy=trn.capacity;

    write(sd, &trn, sizeof(trn));

    read(sd, &trpy, sizeof(trpy));

    return trpy;
}

void preview_trns(int sd){
    struct train_reply trpy;

    write(sd, "preview all train", sizeof("preview all train"));

    read(sd, &trpy, sizeof(trpy));
    if(trpy.status_code!=200){
        printf("Error getting train information\n");
    } else if(trpy.total_trains>0){
        struct train trains[trpy.total_trains];

        read(sd, &trains, sizeof(trains));

        printf("---------------------------------------------------------------------------\n");
        printf("|                       All Train Information                              |\n");
        printf("---------------------------------------------------------------------------\n");
        printf("|  Train ID |   Name   |     From     |      To      | Capacity | vacancy  |\n");
        printf("---------------------------------------------------------------------------\n");
        for(int i=0;i<trpy.total_trains;i++){
            printf("|  %8d | %8s | %12s | %12s | %8d | %7d  |\n", trains[i].id, trains[i].name, trains[i].from_, trains[i].to_, trains[i].capacity, trains[i].vacancy);
        }
        printf("---------------------------------------------------------------------------\n");
    }
}

struct train take_train_info(){
    struct train trn;

    printf("Enter name of the train: ");
    scanf("%s", trn.name);
    printf("Train from: ");
    scanf("%s", trn.from_);
    printf("Train to: ");
    scanf("%s", trn.to_);
    printf("Enter the capacity: ");
    scanf("%d", &trn.capacity);

    return trn;
}