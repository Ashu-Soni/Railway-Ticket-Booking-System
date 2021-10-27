#ifndef USER_DATABASE_H
#define USER_DATABASE_H
#include<stdbool.h>

struct user_db{
    int user_id;
    char user_name[80];
    char user_email[80];
    char user_password[80];
    char type;
    bool loggen_in;
};

struct reply
{
    int statusCode;
    char message[80];
    int total_users;
    int user_id;
};

void registration(int cfd);
void login(int cfd);
void logout(int cfd);
void preview_users(int cfd);
void edit_user(int cfd);

#endif