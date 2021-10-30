/*
Auther and developer: 

Ashutosh Soni - MT2021026
IIIT Bangalore
email: ashutosh.soni@iiitb.ac.in
*/

#include "user_database.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

// User registration function
void registration(int cfd)
{
    struct user_db usr;
    struct user_db prev;
    struct reply rpy;

    read(cfd, &usr, sizeof(usr));
    usr.loggen_in=0;

    struct flock lock;

    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();
    lock.l_type = F_WRLCK;

    int user_fd = open("db/users", O_RDWR | O_CREAT, 0744);
    if (user_fd == -1)
    {
        rpy.statusCode = 400;
        write(cfd, &rpy, sizeof(rpy));
    }
    else
    {
        int loc = lseek(user_fd, 0, SEEK_END);

        fcntl(user_fd, F_SETLKW, &lock);

        if (loc == 0)
        {
            usr.user_id = 1;
            write(user_fd, &usr, sizeof(usr));
        }
        else
        {
            loc = lseek(user_fd, -1 * sizeof(struct user_db), SEEK_END);
            read(user_fd, &prev, sizeof(prev));
            usr.user_id = prev.user_id + 1;
            lseek(user_fd, 0, SEEK_END);
            write(user_fd, &usr, sizeof(usr));
        }

        lock.l_type = F_UNLCK;
        fcntl(user_fd, F_SETLK, &lock);

        rpy.statusCode = 200;
        rpy.user_id = usr.user_id;
        write(cfd, &rpy, sizeof(rpy));
    }

    close(user_fd);
}

// login for all type of users
void login(int cfd)
{
    struct user_db usrAct;
    struct user_db usrGet;
    struct reply rpy;

    read(cfd, &usrAct, sizeof(usrAct));

    struct flock lock;

    lock.l_start = (usrAct.user_id - 1) * sizeof(struct user_db);
    lock.l_len = sizeof(struct user_db);
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();
    lock.l_type = F_WRLCK;

    int user_fd = open("db/users", O_RDWR | O_CREAT, 0744);
    if (user_fd == -1)
    {
        rpy.statusCode = 404;
        write(cfd, &rpy, sizeof(rpy));
    }
    else
    {
        fcntl(user_fd, F_SETLKW, &lock);

        lseek(user_fd, (usrAct.user_id - 1) * sizeof(struct user_db), SEEK_SET);
        read(user_fd, &usrGet, sizeof(usrGet));

        if (usrGet.user_id == usrAct.user_id && strcmp(usrGet.user_password, usrAct.user_password) == 0 && usrAct.type==usrGet.type)
        {
            if (usrGet.loggen_in == 1)
            {
                printf("user type %c and logged_in %d\n", usrGet.type, usrGet.loggen_in);
                if (usrGet.type == 'n' || usrGet.type == 'a')
                {
                    rpy.statusCode = 400;
                } else {
                    rpy.statusCode =200;
                }
            }
            else
            {
                rpy.statusCode = 200;
                lseek(user_fd, (usrAct.user_id - 1) * sizeof(struct user_db), SEEK_SET);
                usrGet.loggen_in = 1;
                write(user_fd, &usrGet, sizeof(usrGet));
            }
        }
        else
        {
            rpy.statusCode = 400;
        }
        write(cfd, &rpy, sizeof(rpy));
    }

    lock.l_type = F_UNLCK;
    fcntl(user_fd, F_SETLK, &lock);

    close(user_fd);
}

// logout method for all users
void logout(int cfd)
{
    struct user_db usrAct;
    struct user_db usrGet;
    struct reply rpy;

    read(cfd, &usrAct, sizeof(usrAct));

    struct flock lock;

    lock.l_start = (usrAct.user_id - 1) * sizeof(struct user_db);
    lock.l_len = sizeof(struct user_db);
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();
    lock.l_type = F_WRLCK;

    int user_fd = open("db/users", O_RDWR | O_CREAT, 0744);
    if (user_fd == -1)
    {
        rpy.statusCode = 404;
        write(cfd, &rpy, sizeof(rpy));
    }
    else
    {
        fcntl(user_fd, F_SETLKW, &lock);

        lseek(user_fd, (usrAct.user_id - 1) * sizeof(struct user_db), SEEK_SET);
        read(user_fd, &usrGet, sizeof(usrGet));

        if (strcmp(usrGet.user_password, usrAct.user_password) == 0)
        {
            usrGet.loggen_in = 0;
            lseek(user_fd, (usrAct.user_id - 1) * sizeof(struct user_db), SEEK_SET);
            write(user_fd, &usrGet, sizeof(usrGet));
            rpy.statusCode = 200;
        }
        else
        {
            rpy.statusCode = 400;
        }

        write(cfd, &rpy, sizeof(rpy));
    }

    lock.l_type = F_UNLCK;
    fcntl(user_fd, F_SETLK, &lock);

    close(user_fd);
}

// preview_users give information related to all the users
// This function can only be invoked by administrator
void preview_users(int cfd)
{
    struct reply rpy;

    struct flock lock;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();
    lock.l_type = F_RDLCK;

    int user_fd = open("db/users", O_RDWR | O_CREAT, 0744);
    if (user_fd == -1)
    {
        rpy.statusCode = 404;
        write(cfd, &rpy, sizeof(rpy));
    }
    else
    {
        fcntl(user_fd, F_SETLKW, &lock);

        lseek(user_fd, (-1) * sizeof(struct user_db), SEEK_END);
        struct user_db tp;
        read(user_fd, &tp, sizeof(tp));

        int total = tp.user_id;
        struct user_db users[total];
        for (int i = 0; i < total; i++)
        {
            lseek(user_fd, (i) * sizeof(struct user_db), SEEK_SET);
            struct user_db tp;
            read(user_fd, &tp, sizeof(tp));
            users[i] = tp;
        }

        rpy.statusCode = 200;
        rpy.total_users = total;
        write(cfd, &rpy, sizeof(rpy));
        write(cfd, &users, sizeof(users));

        lock.l_type = F_UNLCK;
        fcntl(user_fd, F_SETLK, &lock);
    }

    close(user_fd);
}

// edit_user edit the user information
// This function can only be invoked by administrator
void edit_user(int cfd)
{
    struct user_db usr;
    struct reply rpy;

    read(cfd, &usr, sizeof(usr));

    struct flock lock;
    lock.l_start = (usr.user_id - 1) * sizeof(struct user_db);
    lock.l_len = sizeof(struct user_db);
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();
    lock.l_type = F_WRLCK;

    int user_fd = open("db/users", O_RDWR | O_CREAT, 0744);
    if (user_fd == -1)
    {
        rpy.statusCode = 404;
        write(cfd, &rpy, sizeof(rpy));
    }
    else
    {
        fcntl(user_fd, F_SETLKW, &lock);

        int loc = lseek(user_fd, (usr.user_id - 1) * sizeof(struct user_db), SEEK_SET);
        if (loc == -1)
        {
            rpy.statusCode = 400;
        }
        else
        {
            write(user_fd, &usr, sizeof(usr));
            rpy.statusCode = 200;
        }

        lock.l_type = F_UNLCK;
        fcntl(user_fd, F_SETLK, &lock);

        write(cfd, &rpy, sizeof(rpy));
    }

    close(user_fd);
}