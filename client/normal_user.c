#include "normal_user.h"
#include "../server/user_database.h"
#include "../server/train_booking.h"
#include "users.h"
#include "trains.h"

#include<stdio.h>
#include<stdbool.h>

int normal_user_func(int sd, struct user_info *user)
{
    int ch;
    bool cont = true;

    struct reply rpy;
    struct booking_reply brpy;

    while (1)
    {
        printf("\n\n\n1. Book Ticket\n");
        printf("2. Preview Trains\n");
        printf("3. Update Booking\n");
        printf("4. Cancel Booking\n");
        printf("5. Preview Booking\n");
        printf("6. Logout\n");
        printf("enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            brpy = book(sd, user);
            if (brpy.status_code == 200)
            {
                printf("Booking Successfull!\n");
            }
            else
            {
                printf("Booking unsuccessfull with reason: %s\n", brpy.message);
            }
            break;

        case 2:
            preview_trns(sd);
            break;

        case 3:
            brpy = edit(sd, user);
            if (brpy.status_code == 200)
            {
                printf("Booking editted Successfull!\n");
            }
            else
            {
                printf("Edit process unsuccessfull with reason: %s\n", brpy.message);
            }
            break;

        case 4:
            brpy = cancel(sd, user);
            if (brpy.status_code == 200)
            {
                printf("Booking cancelled Successfull!\n");
            }
            else
            {
                printf("cancelling process unsuccessfull with reason: %s\n", brpy.message);
            }
            break;

        case 5:
            preview_bks(sd, user);
            break;

        case 6:
            rpy = user_logout(sd, 'n', user);
            if (rpy.statusCode == 200)
            {
                printf("Successfully loged out\n");
            }
            else
            {
                printf("Invalid userid or password\n");
                normal_user_func(sd, user);
            }
            cont = false;
            break;

        default:
            printf("Invalid choice\n");
            break;
        }

        if (cont == false)
        {
            break;
        }
    }
    return 0;
}