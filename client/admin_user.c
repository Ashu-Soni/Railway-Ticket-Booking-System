#include "normal_user.h"
#include "../server/user_database.h"
#include "../server/train_database.h"
#include "users.h"
#include "trains.h"

#include<stdio.h>
#include<stdbool.h>

int admin_user_func(int sd, struct user_info *user)
{
    int ch;
    bool cont = true;

    struct reply rpy;
    struct train_reply trpy;

    while (1)
    {
        printf("\n\n\n1. Preview Users\n");
        printf("2. Add Train\n");
        printf("3. Preview Trains\n");
        printf("4. Modify User information\n");
        printf("5. Delete User\n");
        printf("6. Preview all bookings\n");
        printf("7. Logout\n");
        printf("enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            preview_usrs(sd);
            break;

        case 2:
            trpy = add(sd);
            if (trpy.status_code == 200)
            {
                printf("Added Successfull!\n");
            }
            else
            {
                printf("unsuccessfull with reason: %s\n", trpy.message);
            }
            break;

        case 3:
            preview_trns(sd);
            break;

        case 4:
            break;
        
        case 5:
            break;
        
        case 6:
            break;

        case 7:
            rpy = user_logout(sd, 'a', user);
            if (rpy.statusCode == 200)
            {
                printf("Successfully loged out\n");
            }
            else
            {
                printf("Invalid userid or password\n");
                admin_user_func(sd, user);
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
