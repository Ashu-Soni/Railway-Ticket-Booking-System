#ifndef USERS_H
#define USERS_H
#include<stdbool.h>

struct user_info {
    int user_id;
    int agent_id;
    int admin_id;
};

struct reply user_login(int sd, char type);
struct reply user_logout(int sd, char type, struct user_info *user);
struct reply user_register(int sd, char type);
void preview_usrs(int sd);

#endif