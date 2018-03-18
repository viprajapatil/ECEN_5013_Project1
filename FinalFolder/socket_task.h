/*
@file - socket_task.h
@brief - Header file for the program which accepts a socket request from another task in order to make API calls to the sensors 
@author - Nikhil Divekar & Vipraja Patil
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

void socket_server();
