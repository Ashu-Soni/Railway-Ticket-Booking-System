# Online Railway Ticket Booking System

## Description
--------------

The project aims to develop a ticket booking system that is user-friendly and multi-functional. The project provides following functionalities:

- All account transactional details and customer information are **stored in files**
- Login system
- **Password-protected** administrative access
- Three types of login: normal account, agent account(multiple
simultaneous logins) , administrator
- Use of **system call** rather using the library functions wherever possible 
- Two tier architecture: Server<->Client using **socket programming**


## Design & Architecture
--------------------------

System is implemented using the following mechanisms:

- Process Management
- File Management
- File Locking
- Multithreading
- Inter Process Communication(IPC)

Architecture diagram for the system:

![Architecture](documents/diagram.png "Architecture")


## Pre-requisites
-----------------
- C language


## Quick Start
--------------

Clone this repository [**Ignore this step if you already have the repository on you local machine**]
```sh
$ git clone https://github.com/Ashu-Soni/Railway-Ticket-Booking-System.git
```

Go into the directory
```sh
$ cd Railway-Ticket-Booking-System
```

### Either use existing build:

#### Run the server:
```sh
$ ./serv 
```

#### Run the client:
```sh
$ ./cli
```

### Create your own builds:

#### Generate the build for server
```sh
$ gcc server/*.c -o <server_name>
```

#### Generate the build for client
```sh
$ gcc client/*.c -o <client_name>
```
You can use these new builds by running ```./<server_name>``` for server and ```./<client_name>``` for client.

### Sample user-id/password

- Normal User => guest/guest123
- Administrator => admin/admin

**Note:** you can always create new user by following the registration procedure.


## User-wise system functionalities
-----------------------------
### Normal user/Agent user
- Book Ticket
- Preview Trains
- Update Booking
- Cancel Booking
- Preview Booking
- Logout

### Administrator
- Add Train
- Preview Trains
- Modify Train
- Preview Users
- Modify User information
- Preview all bookings
- Logout

## Troubleshooting

There is a possibility of vulnerability in the system because of use of file system for dabase management with locking mechanisms. In this type of cases, remove the database files ```$ rm db/*``` and try again by registering new users.
