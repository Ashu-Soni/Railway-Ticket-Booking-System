#include "trains.h"
#include "../server/train_database.h"

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

struct booking_reply book(int sd){
    struct booking_reply brpy;

    write(sd, "book ticket", sizeof("book ticket"));

    struct train_booking_db tb;
    tb=take_info();

    write(sd, &tb, sizeof(tb));

    read(sd, &brpy, sizeof(brpy));
    
    return brpy;
}

struct booking_reply edit(int sd){
    struct booking_reply brpy;

    write(sd, "edit ticket", sizeof("edit ticket"));

    struct train_booking_db tb;
    tb=take_info();
    printf("Enter booking ID: ");
    scanf("%d", &tb.booking_id);

    write(sd, &tb, sizeof(tb));

    read(sd, &brpy, sizeof(brpy));
    
    return brpy;
}

struct booking_reply cancel(int sd){
    struct booking_reply brpy;

    write(sd, "cancel ticket", sizeof("cancel ticket"));

    struct train_booking_db tb;
    
    printf("Enter booking ID: ");
    scanf("%d", &tb.booking_id);

    write(sd, &tb, sizeof(tb));

    read(sd, &brpy, sizeof(brpy));
    return brpy;
}

void preview_bks(int sd){
    struct booking_reply brpy;
    struct train_booking_db tb;

    write(sd, "preview bookings", sizeof("preview bookings"));

    printf("Enter user ID(if through agent give 0): ");
    scanf("%d", &tb.user_id);
    printf("Enter agen ID(if not agent give 0): ");
    scanf("%d", &tb.agent_id);

    write(sd, &tb, sizeof(tb));

    read(sd, &brpy, sizeof(brpy));
    if (brpy.status_code != 200)
    {
        printf("Error getting booking information\n");
    }
    else if(brpy.total_bookings>0)
    {
        struct train_booking_db bookings[brpy.total_bookings];
        read(sd, &bookings, sizeof(bookings));

        for (int i = 0; i < brpy.total_bookings; i++)
        {
            printf("Booking ID\tUser ID\tAgent ID\tTrain ID\tTotal tickets\tSource\tDestination\n");
            printf("%d\t%d\t%d\t%d\t%d\t%s\t%s\n", bookings[i].booking_id, bookings[i].user_id, bookings[i].agent_id, bookings[i].train_id, bookings[i].total_passanger, bookings[i].source, bookings[i].destination);
        }
    }
}

struct train_booking_db take_info(){
    struct train_booking_db tb;

    printf("Enter user ID(if not then give 0): ");
    scanf("%d", &tb.user_id);
    printf("Enter agent ID(if not then give 0): ");
    scanf("%d", &tb.agent_id);
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