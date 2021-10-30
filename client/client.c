/*
Auther and developer: 

Ashutosh Soni - MT2021026
IIIT Bangalore
email: ashutosh.soni@iiitb.ac.in
*/

#include "users.h"
#include "../server/user_database.h"
#include "admin_user.h"
#include "normal_user.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

int main_util(int sd, char type);
int main_user_choice(int sd);

// main entry point for the client
int main()
{
    struct sockaddr_in server;
    int sd;
    char buf[80];

    sd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5050);

    printf("Connecting with the server...\n");
    connect(sd, (struct sockaddr *)&server, sizeof(server));
    printf("connected with server\n");

    int ch;
    bool flag = false;
    char type;
    printf("\n---------------------------------------------------------");
    printf("\n|                                                        |");
    printf("\n|        Online Railway Ticket Booking System            |");
    printf("\n|                                                        |");
    printf("\n---------------------------------------------------------");

    main_user_choice(sd);

    write(sd, "quit", sizeof("quit"));
    close(sd);

    return 0;
}

int main_user_choice(int sd)
{
    while (1)
    {
        int ch;
        char type;
        bool cont = true;
        printf("\n\n1. Normal User\n");
        printf("2. Agent\n");
        printf("3. Administrator\n");
        printf("4. Quit\n");
        printf("enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            type = 'n';
            break;

        case 2:
            type = 'm';
            break;

        case 3:
            type = 'a';
            break;

        case 4:
            cont = false;
            break;

        default:
            printf("Invalid choice, Try again!!\n");
            main_user_choice(sd);
            break;
        }

        if (cont == false)
        {
            break;
        }
        main_util(sd, type);
    }
    
    return 0;
}

int main_util(int sd, char type)
{
    int ch;
    printf("\n\n\n1. Login\n");
    printf("2. Registration\n");
    printf("enter your choice: ");
    scanf("%d", &ch);

    struct reply rpy;
    switch (ch)
    {
    case 1:
        rpy = user_login(sd, type);
        struct user_info user;
        if (rpy.statusCode == 200)
        {
            printf("Successfully loged in\n");
            if (type == 'n' || type == 'm')
            {
                if (type == 'n')
                {
                    user.user_id = rpy.user_id;
                    user.agent_id = 0;
                    user.admin_id = 0;
                }
                else
                {
                    user.agent_id = rpy.user_id;
                    user.user_id = 0;
                    user.admin_id = 0;
                }
                normal_user_func(sd, type, &user);      // All functionalities for Normal/Agent user
            }
            else
            {
                user.admin_id = rpy.user_id;
                user.agent_id = -1;
                user.user_id = -1;
                admin_user_func(sd, &user);             // All functionalities for administrator
            }
        }
        else
        {
            printf("Invalid username, password or type\n");
            main_util(sd, type);
        }
        break;

    case 2:
        rpy = user_register(sd, type);
        if (rpy.statusCode == 200)
        {
            printf("Registration successfull, you can login into stystem now!\n\n");
            printf("Your ID is: %d\nPlease save it privately\n", rpy.user_id);
        }
        else
        {
            printf("Registration unsuccessfull with error: %s\n", rpy.message);
        }
        main_util(sd, type);
        break;

    default:
        printf("Invalid choice\n");
        main_util(sd, type);
        break;
    }

    return 0;
}