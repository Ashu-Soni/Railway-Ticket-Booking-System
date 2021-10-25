#include "users.h"
#include "../server/user_database.h"
#include "admin_user.h"
#include "normal_user.h"
#include "agent_user.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

int main_util(int sd, char type);
int main_user_choice(int sd);

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
    printf("Online Railway Ticket Booking System");

    main_user_choice(sd);

    write(sd, "quit", sizeof("quit"));
    close(sd);

    return 0;
}

int main_user_choice(int sd)
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
        return 0;
    }
    main_util(sd, type);
    main_user_choice(sd);
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
        if (rpy.statusCode == 200)
        {
            printf("Successfully loged in\n");
            if (type == 'n')
            {
                normal_user_func(sd);
            }
            else if (type == 'm')
            {
                agent_user_func(sd);
            }
            else
            {
                admin_user_func(sd);
            }
        }
        else
        {
            printf("Invalid username or password\n");
            main_util(sd, type);
        }
        break;

    case 2:
        rpy = user_register(sd, type);
        if (rpy.statusCode == 200)
        {
            printf("Registration successfull, you can login into stystem now!\n");
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