#include "users.h"
#include "../server/user_database.h"

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

struct reply user_login(int sd, char type)
{
    struct reply rpy;

    write(sd, "login", sizeof("login"));

    struct user_db usr;
    printf("Enter user ID: ");
    scanf("%d", &usr.user_id);
    printf("Enter password: ");
    scanf("%s", usr.user_password);
    usr.type = type;

    write(sd, &usr, sizeof(usr));

    read(sd, &rpy, sizeof(rpy));
    printf("Reply from server: %d\n", rpy.statusCode);

    return rpy;
}

struct reply user_logout(int sd, char type)
{
    struct reply rpy;

    write(sd, "logout", sizeof("logout"));

    struct user_db usr;
    printf("Enter user ID: ");
    scanf("%d", &usr.user_id);
    printf("Enter password: ");
    scanf("%s", usr.user_password);
    usr.type = type;

    write(sd, &usr, sizeof(usr));

    read(sd, &rpy, sizeof(rpy));
    printf("Reply from server: %d\n", rpy.statusCode);
    return rpy;
}

struct reply user_register(int sd, char type)
{
    struct reply rpy;

    write(sd, "register", sizeof("register"));

    struct user_db usr;
    printf("Enter Username: ");
    scanf("%s", usr.user_name);
    printf("Enter email id: ");
    scanf("%s", usr.user_email);
    printf("Enter password: ");
    scanf("%s", usr.user_password);
    usr.type = type;

    write(sd, &usr, sizeof(usr));

    read(sd, &rpy, sizeof(rpy));
    printf("Reply from server: %d\n", rpy.statusCode);

    return rpy;
}

void preview_usrs(int sd)
{
    struct reply rpy;
    write(sd, "preview users", sizeof("preview users"));

    read(sd, &rpy, sizeof(rpy));
    if (rpy.statusCode != 200)
    {
        printf("Error getting users information\n");
    }
    else
    {
        struct user_db users[rpy.total_users];
        read(sd, &users, sizeof(users));

        for (int i = 0; i < rpy.total_users; i++)
        {
            printf("User ID\tUser Name\tUser Email\tUser Password\tUser type\tLogged in/not\n");
            printf("%d\t%s\t%s\t%s\t%c\t%d\n", (users[i]).user_id, (users[i]).user_name, (users[i]).user_email, (users[i]).user_password, (users[i]).type, ((users[i]).loggen_in == true));
        }
    }
}