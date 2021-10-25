#include "normal_user.h"
#include "../server/user_database.h"
#include "users.h"

#include<stdio.h>
#include<stdbool.h>

int admin_user_func(int sd)
{
    int ch;
    bool cont = true;

    struct reply rpy;

    while (1)
    {
        printf("\n\n\n1. Preview Users\n");
        printf("2. Preview Trains\n");
        printf("3. Modify User information\n");
        printf("4. Delete User\n");
        printf("5. Logout\n");
        printf("enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            preview_usrs(sd);
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            rpy = user_logout(sd, 'n');
            if (rpy.statusCode == 200)
            {
                printf("Successfully loged out\n");
            }
            else
            {
                printf("Invalid userid or password\n");
                normal_user_func(sd);
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
