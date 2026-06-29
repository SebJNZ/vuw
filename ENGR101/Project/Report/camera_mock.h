/*
    All code written by @SebJNZ, and is licensed under the 
    GNU Affero General Public License v3.0.
    You must follow and understand all permissions, rules, 
    and limitations stated under this license.

*/


#ifndef CAMERA_MOCK_H
#define CAMERA_MOCK_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define WIDTH  640
#define HEIGHT 480
typedef unsigned char uchar;

// "extern" is used so that both main.c and tester.c have the same instances of variables
extern bool perfectRuby;
extern uchar mockR;
extern uchar mockG;
extern uchar mockB;
extern uint16_t rubyStartX;
extern uint16_t rubyEndX;
extern uint16_t rubyStartY;
extern uint16_t rubyEndY;

extern uchar mock_screen[HEIGHT][WIDTH][3];
extern bool checkRan;

void setPerfectRuby(bool state);

void setRubyLocations(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);

void setBackgroundColor(uchar r, uchar g, uchar b);

void get_pixel(int y, int x, uchar *r, uchar *g, uchar *b);

void set_pixel(int y, int x, uchar r, uchar g, uchar b);

void clear_mock_screen();

void setPixelRanFalse();

/* 
    Define other functions that are called 
    but with no functionality.
*/

int start_camera();

int take_picture();

void display_picture();

#endif