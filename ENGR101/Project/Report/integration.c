/*
    All code written by @SebJNZ, and is licensed under the 
    GNU Affero General Public License v3.0.
    You must follow and understand all permissions, rules, 
    and limitations stated under this license.

*/


#include "main.h"
#include "camera_mock.h"
#include <stdio.h>
#include <unistd.h>

/* Had to find some way to multithread, for stealing the ruby,
   while main.c runs, found out there was a library, pthread.h
   from https://www.geeksforgeeks.org/c/multithreading-in-c/
*/ 
#include <pthread.h>

void *stealRuby(void *args) {
    printf("Giving camera 7 seconds to start, and ruby to be detected\n");
    sleep(7);

    printf("Stealing ruby\n");
    setPerfectRuby(false);
    return NULL;
}

bool openFile() {
    FILE *fp = fopen("rubylog.txt", "r");
    if (fp == NULL) {
        printf("TEST FAILED, log file was not created\n");
        return false;
    }

    char time[10];

    int items_read = fscanf(fp, "Ruby was stolen at: %[^\n]", time);
    fclose(fp);

    if (items_read != 1) {
        printf("Error, file not formatted correctly.\n");
        return false;
    }
    return true;
}

int main() {
    setBackgroundColor(255, 255, 255);
    setPerfectRuby(true);

    printf("Starting integration testing...\n");
    pthread_t thread;

    pthread_create(&thread, NULL, stealRuby, NULL);

    printf("Starting ruby program start()...\n");
    start();
    printf("start() exited successfully\n");

    pthread_join(thread, NULL);

    if(openFile() == true) {
        printf("Ruby stolen detected successfully!\n");
    }
    return 0;
}