#include "train_booking.h"
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

void book_ticket(int cfd)
{
    struct booking_reply brpy;
    struct train_booking_db tb;
    struct train_booking_db prev;

    read(cfd, &tb, sizeof(tb));

    struct flock lock;

    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();
    lock.l_type = F_WRLCK;

    int train_b_fd = open("db/train_booking", O_RDWR | O_CREAT, 0744);
    if (train_b_fd == -1)
    {
        brpy.status_code = 400;
    }
    else
    {
        int loc = lseek(train_b_fd, 0, SEEK_END);

        fcntl(train_b_fd, F_SETLKW, &lock);

        if (check_availability(tb.train_id, tb.total_passanger, 0, 'b') == true)
        {

            tb.booking_status = 'b';
            if (loc == 0)
            {
                tb.booking_id = 1;
                write(train_b_fd, &tb, sizeof(tb));
                printf("Booking ID: %d\n", tb.booking_id);
            }
            else
            {
                loc = lseek(train_b_fd, (-1) * sizeof(struct train_booking_db), SEEK_END);
                printf("location: %d\n", loc);

                read(train_b_fd, &prev, sizeof(prev));
                tb.booking_id = prev.booking_id + 1;

                loc = lseek(train_b_fd, 0, SEEK_END);
                printf("location: %d\n", loc);
                write(train_b_fd, &tb, sizeof(tb));
                printf("Booking ID: %d\n", tb.booking_id);
            }
            brpy.status_code = 200;
        }
        else
        {
            brpy.status_code = 400;
        }

        lock.l_type = F_UNLCK;
        fcntl(train_b_fd, F_SETLK, &lock);
    }

    write(cfd, &brpy, sizeof(brpy));
    close(train_b_fd);
}

void edit_ticket(int cfd)
{
    struct booking_reply brpy;
    struct train_booking_db tb;

    read(cfd, &tb, sizeof(tb));
    tb.booking_status = 'e';

    struct flock lock;
    lock.l_len = sizeof(tb);
    lock.l_pid = getpid();
    lock.l_start = (tb.booking_id - 1) * sizeof(struct train_booking_db);
    lock.l_type = F_WRLCK;

    int train_b_fd = open("db/train_booking", O_RDWR | O_CREAT, 0744);
    if (train_b_fd == -1)
    {
        brpy.status_code = 400;
        write(cfd, &brpy, sizeof(brpy));
    }
    else
    {
        fcntl(train_b_fd, F_SETLKW, &lock);

        printf("%lu\n", (tb.booking_id - 1) * sizeof(struct train_booking_db));
        int loc = lseek(train_b_fd, (tb.booking_id - 1) * sizeof(struct train_booking_db), SEEK_SET);
        if (loc == -1)
        {
            brpy.status_code = 400;
        }
        else
        {
            printf("Location in update: %d\n", loc);

            struct train_booking_db tp;
            read(train_b_fd, &tp, sizeof(tp));

            if (check_availability(tb.train_id, tb.total_passanger, tp.total_passanger, 'e') == true)
            {
                lseek(train_b_fd, (tb.booking_id - 1) * sizeof(struct train_booking_db), SEEK_SET);
                write(train_b_fd, &tb, sizeof(tb));
                brpy.status_code = 200;
            }
            else
            {
                brpy.status_code = 400;
            }
        }
        lock.l_type = F_ULOCK;
        fcntl(train_b_fd, F_SETLK, &lock);

        write(cfd, &brpy, sizeof(brpy));
    }

    close(train_b_fd);
}

void cancel_ticket(int cfd)
{
    struct booking_reply brpy;
    struct train_booking_db tb;

    read(cfd, &tb, sizeof(tb));

    struct flock lock;
    lock.l_len = sizeof(tb);
    lock.l_pid = getpid();
    lock.l_start = (tb.booking_id - 1) * sizeof(struct train_booking_db);
    lock.l_type = F_WRLCK;

    int train_b_fd = open("db/train_booking", O_RDWR | O_CREAT, 0744);
    if (train_b_fd == -1)
    {
        brpy.status_code = 400;
        write(cfd, &brpy, sizeof(brpy));
    }
    else
    {
        fcntl(train_b_fd, F_SETLKW, &lock);
        printf("cancel location: %lu\n", (tb.booking_id - 1) * sizeof(struct train_booking_db));
        lseek(train_b_fd, (tb.booking_id - 1) * sizeof(struct train_booking_db), SEEK_SET);
        struct train_booking_db tp;
        read(train_b_fd, &tp, sizeof(tp));

        if (tp.agent_id == tb.agent_id && tp.user_id == tb.user_id && update_vacancy(tp.train_id, tp.total_passanger)==true)
        {
            lseek(train_b_fd, (tb.booking_id - 1) * sizeof(struct train_booking_db), SEEK_SET);
            tp.booking_status = 'c';
            write(train_b_fd, &tp, sizeof(tp));

            lock.l_type = F_ULOCK;
            fcntl(train_b_fd, F_SETLK, &lock);

            brpy.status_code = 200;
        }
        else
        {
            brpy.status_code = 400;
        }
        write(cfd, &brpy, sizeof(brpy));
    }

    close(train_b_fd);
}

void preview_bookings(int cfd)
{
    struct booking_reply brpy;
    struct train_booking_db tb;

    read(cfd, &tb, sizeof(tb));

    struct flock lock;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();
    lock.l_type = F_RDLCK;

    int train_b_fd = open("db/train_booking", O_RDWR | O_CREAT, 0744);
    if (train_b_fd == -1)
    {
        brpy.status_code = 404;
        write(cfd, &brpy, sizeof(brpy));
    }
    else
    {
        fcntl(train_b_fd, F_SETLKW, &lock);

        int loc = lseek(train_b_fd, 0, SEEK_END);
        printf("train_booking location: %d\n", loc);
        if (loc > 0)
        {
            loc = lseek(train_b_fd, (-1) * sizeof(struct train_booking_db), SEEK_END);
            printf("train_booking location updated: %d\n", loc);
            struct train_booking_db tp;
            read(train_b_fd, &tp, sizeof(tp));

            int total = tp.booking_id;
            printf("last booking ID: %d\n", tp.booking_id);
            struct train_booking_db bookings[total];
            int total_cnt = 0;
            for (int i = 0; i < total; i++)
            {
                lseek(train_b_fd, (i) * sizeof(struct train_booking_db), SEEK_SET);
                struct train_booking_db tp;
                read(train_b_fd, &tp, sizeof(tp));
                printf("%d %d and %d\n", tb.user_id, tp.agent_id, tp.booking_id);
                if (tb.agent_id == -1 && tb.user_id == -1)
                {
                    bookings[total_cnt++] = tp;
                }
                else if (tp.agent_id == tb.agent_id && tp.user_id == tb.user_id && tp.booking_status != 'c')
                {
                    bookings[total_cnt++] = tp;
                }
            }

            printf("Total count: %d\n", total_cnt);
            brpy.status_code = 200;
            brpy.total_bookings = total_cnt;
            write(cfd, &brpy, sizeof(brpy));
            if (total_cnt > 0)
            {
                write(cfd, &bookings, sizeof(bookings));
            }
        }
        else
        {
            brpy.status_code = 200;
            brpy.total_bookings = 0;
            write(cfd, &brpy, sizeof(brpy));
        }

        lock.l_type = F_UNLCK;
        fcntl(train_b_fd, F_SETLK, &lock);
    }

    close(train_b_fd);
}

bool check_availability(int train_id, int req, int lastN, char type)
{
    struct flock lock1;
    lock1.l_len = sizeof(struct train);
    lock1.l_start = (train_id - 1) * sizeof(struct train);
    lock1.l_pid = getpid();
    lock1.l_type = F_WRLCK;
    lock1.l_whence = SEEK_SET;

    struct train tp;
    int train_fd = open("db/trains", O_RDWR | O_CREAT, 0744);
    if (train_fd == -1)
    {
        return false;
    }
    fcntl(train_fd, F_SETLKW, &lock1);

    int loc = lseek(train_fd, (train_id - 1) * sizeof(struct train), SEEK_SET);
    if (loc == -1)
    {
        return false;
    }
    read(train_fd, &tp, sizeof(tp));
    if (type == 'b' && tp.vacancy >= req)
    {
        tp.vacancy -= req;
        lseek(train_fd, (train_id - 1) * sizeof(struct train), SEEK_SET);
        write(train_fd, &tp, sizeof(tp));
    }
    else if (type == 'e' && tp.vacancy + lastN >= req)
    {
        tp.vacancy = tp.vacancy + lastN - req;
        lseek(train_fd, (train_id - 1) * sizeof(struct train), SEEK_SET);
        write(train_fd, &tp, sizeof(tp));
    }
    else
    {
        return false;
    }

    lock1.l_type = F_UNLCK;
    fcntl(train_fd, F_SETLK, &lock1);

    return true;
}

bool update_vacancy(int train_id, int n){
    struct flock lock1;
    lock1.l_len = sizeof(struct train);
    lock1.l_start = (train_id - 1) * sizeof(struct train);
    lock1.l_pid = getpid();
    lock1.l_type = F_WRLCK;
    lock1.l_whence = SEEK_SET;

    struct train tp;
    int train_fd = open("db/trains", O_RDWR | O_CREAT, 0744);
    if (train_fd == -1)
    {
        return false;
    }
    fcntl(train_fd, F_SETLKW, &lock1);   

    lseek(train_fd, (train_id - 1) * sizeof(struct train), SEEK_SET);
    read(train_fd, &tp, sizeof(tp));
    tp.vacancy += n;
    lseek(train_fd, (train_id - 1) * sizeof(struct train), SEEK_SET);
    write(train_fd, &tp, sizeof(tp));

    lock1.l_type = F_UNLCK;
    fcntl(train_fd, F_SETLK, &lock1);

    return true;
}