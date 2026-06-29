/*
    All code written by @SebJNZ, and is licensed under the 
    GNU Affero General Public License v3.0.
    You must follow and understand all permissions, rules, 
    and limitations stated under this license.

*/

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "camera_mock.h"
#include <stdint.h>
#include <time.h>
#include <string.h>

struct pointLocations {
    uint16_t topY;
    uint16_t bottomY;
    uint16_t leftX;
    uint16_t rightX;
};

struct detectionPoints {
    uint16_t x[4];
    uint16_t y[4];
    uint8_t stolen[4];
};

bool locateRuby(struct pointLocations *locationsPtr, int *redpx);

void calculateDetectionPoints(struct pointLocations *rubyLoc, struct detectionPoints *points);

int displayRed();

int getTime(char *timeV);

int drawLine(int x1, int x2, int y1, int y2, char num);

int displayTime(char *time);

int createLog(char *time);

int start(void);

#endif