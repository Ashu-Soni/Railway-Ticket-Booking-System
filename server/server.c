/*
Auther and developer: 

Ashutosh Soni - MT2021026
IIIT Bangalore
email: ashutosh.soni@iiitb.ac.in
*/

#include "user_database.h"
#include "train_database.h"
#include "train_booking.h"

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>

void *client_main(void *arg);

int main(){
    struct sockaddr_in server, client;
    int sfd;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd==-1){
        printf("Error while creation of server socket\n");
        return 0;
    }

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(5050);

    bind(sfd, (struct sockaddr *)&server, sizeof(server));

    listen(sfd, 5);

    pthread_t ts[10];
    int cfd[10];
    int i=0;
    while(1){
        int clientS = sizeof(client);

        printf("Waiting for new client to join\n");
        cfd[i]=accept(sfd, (struct sockaddr *)&client, (socklen_t *)&clientS);
        if(cfd[i]==-1){
            printf("Error accepting the client\n");
            return 0;
        }
        printf("Connected to the new client\n");

        int pret = pthread_create(&ts[i], NULL, client_main, &cfd[i]);
        if (pret == -1)
        {
            perror("Thread creation:");
            exit(1);
        }

        i++;
        if(i>=8){
            printf("Already %d clients are connected, so waiting for some to disconnect!\n", i);
            i=0;
            while(i<8){
                pthread_join(ts[i++], NULL);
            }
            i=0;
            printf("Connection closed with all previous client\n");
            printf("New clients can connect now!\n\n");
        }

    }

    close(sfd);
    printf("Success!!\n");
    return 0;
}

// Server endpoints which works as the database functionalities for clients
void *client_main(void *arg){
    int *cfd = (int *)arg;

    char msg[80];

    while(1){
        read(*cfd, &msg, sizeof(msg));

        if(strcmp(msg, "quit")==0){
            break;
        }

        if(strcmp(msg, "register")==0){
            registration(*cfd);
        }

        if(strcmp(msg, "login")==0){
            login(*cfd);
        }

        if(strcmp(msg, "logout")==0){
            logout(*cfd);
        }

        if(strcmp(msg, "preview users")==0){
            preview_users(*cfd);
        }

        if(strcmp(msg, "book ticket")==0){
            book_ticket(*cfd);
        }

        if(strcmp(msg, "edit ticket")==0){
            edit_ticket(*cfd);
        }

        if(strcmp(msg, "cancel ticket")==0){
            cancel_ticket(*cfd);
        }

        if(strcmp(msg, "preview bookings")==0){
            preview_bookings(*cfd);
        }

        if(strcmp(msg, "add new train")==0){
            add_train(*cfd);
        }

        if(strcmp(msg, "preview all train")==0){
            preview_trains(*cfd);
        }

        if(strcmp(msg, "update user info")==0){
            edit_user(*cfd);
        }

        if(strcmp(msg, "update train")==0){
            edit_trn(*cfd);
        }
    }

    return NULL;
}