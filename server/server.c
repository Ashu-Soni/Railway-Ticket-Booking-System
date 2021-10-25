#include "user_database.h"
#include "train_database.h"

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
    int sfd, cfd;

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

    while(1){
        int clientS = sizeof(client);

        cfd=accept(sfd, (struct sockaddr *)&client, (socklen_t *)&clientS);
        if(cfd==-1){
            printf("Error accepting the client\n");
            return 0;
        }
        printf("Connected to the client\n");

        pthread_t t;
        int pret = pthread_create(&t, NULL, client_main, &cfd);
        if (pret == -1)
        {
            perror("Thread creation:");
            exit(1);
        }

        pthread_join(t, NULL);
        printf("Connection closed with client\n");
        close(cfd);

        break;
    }

    close(sfd);
    printf("Success!!\n");
    return 0;
}

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
    }

    return NULL;
}