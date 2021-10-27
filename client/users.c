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
    rpy.user_id=usr.user_id;

    return rpy;
}

struct reply user_logout(int sd, char type, struct user_info *user)
{
    struct reply rpy;

    write(sd, "logout", sizeof("logout"));

    struct user_db usr;
    if(type=='n'){
        usr.user_id=(*user).user_id;
    } else if(type=='m'){
        usr.user_id=(*user).agent_id;
    } else {
        usr.user_id=(*user).admin_id;
    }
    printf("user ID: %d\n", usr.user_id);
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

        printf("-------------------------------------------------------------------------------------\n");
        printf("|                           All User Information                                     |\n");
        printf("-------------------------------------------------------------------------------------\n");
        printf("|  User ID |  User Name  |  User Email  | User Password | User type | Logged in/not  |\n");
        printf("-------------------------------------------------------------------------------------\n");
        for (int i = 0; i < rpy.total_users; i++)
        {
            printf("|  %7d |%13s|%14s|%15s| %9c | %13d  |\n", (users[i]).user_id, (users[i]).user_name, (users[i]).user_email, (users[i]).user_password, (users[i]).type, ((users[i]).loggen_in == true));
        }
        printf("------------------------------------------------------------------------------------\n");
    }
}

struct reply update_user(int sd){
    struct reply rpy;

    write(sd, "update user info", sizeof("update user info"));

    struct user_db usr;
    printf("Enter the type of user: ");
    getchar();
    scanf("%c", &usr.type);
    printf("Enter the user ID: ");
    scanf("%d", &usr.user_id);
    printf("Enter Username: ");
    scanf("%s", usr.user_name);
    printf("Enter email id: ");
    scanf("%s", usr.user_email);
    printf("Enter password: ");
    scanf("%s", usr.user_password);
    usr.loggen_in=false;

    write(sd, &usr, sizeof(usr));

    read(sd, &rpy, sizeof(rpy));
    
    return rpy;    
}