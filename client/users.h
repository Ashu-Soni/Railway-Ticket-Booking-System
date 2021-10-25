#ifndef USERS_H
#define USERS_H
#include<stdbool.h>

struct reply user_login(int sd, char type);
struct reply user_logout(int sd, char type);
struct reply user_register(int sd, char type);
void preview_usrs(int sd);

#endif