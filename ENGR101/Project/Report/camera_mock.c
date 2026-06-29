/*
    All code written by @SebJNZ, and is licensed under the 
    GNU Affero General Public License v3.0.
    You must follow and understand all permissions, rules, 
    and limitations stated under this license.

*/

#include "camera_mock.h"
#include <stdio.h>

bool perfectRuby = false;
uchar mockR = 0;
uchar mockG = 0;
uchar mockB = 0;
uint16_t rubyStartX = 295;
uint16_t rubyEndX = 345;
uint16_t rubyStartY = 215;
uint16_t rubyEndY = 265;

uchar mock_screen[HEIGHT][WIDTH][3] = {0};
bool checkRan = false;

void setPerfectRuby(bool state) {
    perfectRuby = state;
}

void setRubyLocations(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
    rubyStartX = xStart;
    rubyEndX = xEnd;
    rubyStartY = yStart;
    rubyEndY = yEnd;
}

void setBackgroundColor(uchar r, uchar g, uchar b) {
    mockR = r;
    mockG = g;
    mockB = b;
}

void get_pixel(int y, int x, uchar *r, uchar *g, uchar *b) {
    if (perfectRuby) {
        if (y >= rubyStartY && y <= rubyEndY) {
            if (x >= rubyStartX && x <= rubyEndX) {
                *r = (uchar)255;
                *g = (uchar)0;
                *b = (uchar)0;
                return;
            }
        }
    }
    *r = (uchar)mockR;
    *g = (uchar)mockG;
    *b = (uchar)mockB;
}

void set_pixel(int y, int x, uchar r, uchar g, uchar b) {
    checkRan = true;
    if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
        mock_screen[y][x][0] = r;
        mock_screen[y][x][1] = g;
        mock_screen[y][x][2] = b;
    }
}

void clear_mock_screen() {
    memset(mock_screen, 0, sizeof(mock_screen));
}

void setPixelRanFalse() {
    checkRan = false;
}

/* 
    Define other functions that are called 
    but with no functionality.
*/

int start_camera() {
    return 0;
}

int take_picture() {
    return 0;
}

void display_picture() {}

