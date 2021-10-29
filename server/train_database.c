#include "train_database.h"

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

// These functions can only be invoked by the administrator

// add_train adds new train in the train listing
void add_train(int cfd)
{
    struct train_reply trpy;

    struct train trn;
    struct train prev;

    read(cfd, &trn, sizeof(trn));

    struct flock lock;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();
    lock.l_type = F_WRLCK;

    int train_fd = open("db/trains", O_RDWR | O_CREAT, 0744);
    if (train_fd == -1)
    {
        trpy.status_code = 400;
        write(cfd, &trpy, sizeof(trpy));
    }
    else
    {
        fcntl(train_fd, F_SETLKW, &lock);

        int loc = lseek(train_fd, 0, SEEK_END);
        if (loc == 0)
        {
            trn.id = 1;
            write(train_fd, &trn, sizeof(trn));
        }
        else
        {
            loc = lseek(train_fd, (-1) * sizeof(trn), SEEK_END);

            read(train_fd, &prev, sizeof(prev));
            trn.id = prev.id + 1;

            lseek(train_fd, 0, SEEK_END);
            write(train_fd, &trn, sizeof(trn));
        }

        lock.l_type = F_UNLCK;
        fcntl(train_fd, F_SETLK, &lock);

        trpy.status_code = 200;
        write(cfd, &trpy, sizeof(trpy));
    }

    close(train_fd);
}

// edit_trn edits the existing train information
void edit_trn(int cfd)
{
    struct train_reply trpy;

    struct train trn;

    read(cfd, &trn, sizeof(trn));

    struct flock lock;
    lock.l_start = (trn.id - 1) * sizeof(struct train);
    lock.l_len = sizeof(struct train);
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();
    lock.l_type = F_WRLCK;

    int train_fd = open("db/trains", O_RDWR | O_CREAT, 0744);
    if (train_fd == -1)
    {
        trpy.status_code = 400;
        write(cfd, &trpy, sizeof(trpy));
    }
    else
    {
        int loc = lseek(train_fd, (trn.id - 1) * sizeof(struct train), SEEK_SET);
        if (loc == -1)
        {
            trpy.status_code = 400;
        }
        else
        {
            fcntl(train_fd, F_SETLKW, &lock);

            struct train tp;
            read(train_fd, &tp, sizeof(tp));
            tp.vacancy += (trn.capacity - tp.capacity);
            if (tp.vacancy >= 0)
            {
                trn.vacancy = tp.vacancy;
                lseek(train_fd, (trn.id - 1) * sizeof(struct train), SEEK_SET);
                write(train_fd, &trn, sizeof(trn));

                trpy.status_code = 200;
            }
            else
            {
                trpy.status_code = 400;
            }

            lock.l_type = F_UNLCK;
            fcntl(train_fd, F_SETLK, &lock);
        }

        write(cfd, &trpy, sizeof(trpy));
    }

    close(train_fd);
}

void preview_trains(int cfd)
{
    struct train_reply trpy;

    struct train prev;

    struct flock lock;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();
    lock.l_type = F_RDLCK;

    int train_fd = open("db/trains", O_RDWR | O_CREAT, 0744);
    if (train_fd == -1)
    {
        trpy.status_code = 400;
        write(cfd, &trpy, sizeof(trpy));
    }
    else
    {
        fcntl(train_fd, F_SETLKW, &lock);

        int loc = lseek(train_fd, 0, SEEK_END);
        if (loc == 0)
        {
            trpy.status_code = 200;
            trpy.total_trains = 0;
            write(cfd, &trpy, sizeof(trpy));
        }
        else
        {
            lseek(train_fd, (-1) * sizeof(struct train), SEEK_END);
            read(train_fd, &prev, sizeof(prev));

            trpy.total_trains = prev.id;
            struct train trains[prev.id];
            for (int i = 0; i < trpy.total_trains; i++)
            {
                lseek(train_fd, (i) * sizeof(struct train), SEEK_SET);
                read(train_fd, &trains[i], sizeof(trains[i]));
            }

            trpy.status_code = 200;
            write(cfd, &trpy, sizeof(trpy));

            write(cfd, &trains, sizeof(trains));
        }

        lock.l_type = F_UNLCK;
        fcntl(train_fd, F_SETLK, &lock);
    }

    close(train_fd);
}