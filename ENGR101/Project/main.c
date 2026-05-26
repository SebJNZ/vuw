/*
 * The code below is meant to detect 
 * whether the Red Ruby is present. 
*/

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "camera_x11.h"
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

bool locateRuby(struct pointLocations *locationsPtr, int *redpx) {
    if (!locationsPtr) {return false;} // Safety check if it's null.
    take_picture();
    // Define Point Variables.
    int minX = 640;
    int maxX = -1;
    int minY = 480;
    int maxY = -1;
    bool foundAny = false;
    // Find Min and Max points (left, right, top bottom.)
    for (int row = 0 ; row < 480 ; row += 4) {	
        for (int col = 0; col < 640; col += 4) {
            uchar r, g ,b;
            get_pixel(row, col, &r, &g ,&b);
            if ( (r  > b * 1.5) && ( r  > g * 1.5) ) {
                foundAny = true;
                if (col < minX) { minX = col; }
                if (col > maxX) { maxX = col; }
                if (row < minY) { minY = row; }
                if (row > maxY) { maxY = row; }
                *redpx = *redpx + 1;
            }
        }
    }

    // If we find a ruby, set the points in the struct.
    if (foundAny) {
        locationsPtr->leftX = minX;
        locationsPtr->rightX = maxX;
        locationsPtr->topY = minY;
        locationsPtr->bottomY = maxY;
        return true;
    }
    return false;
}

void calculateDetectionPoints(struct pointLocations *rubyLoc, struct detectionPoints *points) {
    if (!rubyLoc || !points) {return;}
    int centralX = (rubyLoc->leftX + rubyLoc->rightX) / 2;
    int centralY = (rubyLoc->topY + rubyLoc->bottomY) / 2;
    int width = rubyLoc->rightX - rubyLoc->leftX;
    int height = rubyLoc->bottomY - rubyLoc->topY;
    int widthIncrement = (width / 8);
    int heightIncrement = (height / 8);
    // Left inline with dot (on y axis)
    points->x[0] = centralX - widthIncrement;
    points->y[0] = centralY;
    // Right inline with dot (on y axis)
    points->x[1] = centralX + widthIncrement;
    points->y[1] = centralY;
    // Top inline with dot (on x axis)
    points->x[2] = centralX;
    points->y[2] = centralY - heightIncrement;
    // Bottom inline with dot (on x axis)
    points->x[3] = centralX;
    points->y[3] = centralY + heightIncrement;
}

int displayRed() {
    for (int row = 0; row < 480; row++) {
        for (int col = 0; col < 640; col++) {
            set_pixel(row, col, 255, 0, 0);
        }
    }
    display_picture();
    return 0;
}

int getTime(char *timeV) {
    time_t currentTime = time(NULL);
    char *timeString = ctime(&currentTime);
    int len = strlen(timeString);
    int index = len - 10;
    for (int i = 0; i < 5; i++) {
        timeV[4-i] = timeString[index - i];
    }
    timeV[5] = '\0';
    return 0;
}

int drawLine(int x1, int x2, int y1, int y2, char num) {
    for (int col = x1; col < x2; col++) {
        int y20 = y1 + 20;
        int y10 = y1 + 10;
        if (num == '0'){
            set_pixel(y1, col, 255, 0, 0);
            set_pixel(y20, col, 255, 0, 0);
        } if (num == '2'){
            set_pixel(y1, col, 255, 0, 0);
            set_pixel(y20, col, 255, 0, 0);
            set_pixel(y10, col, 255, 0, 0);
        } if (num =='3'){
            set_pixel(y1, col, 255, 0, 0);
            set_pixel(y20, col, 255, 0, 0);
            set_pixel(y10, col, 255, 0, 0);
        } if (num == '4'){
            set_pixel(y10, col, 255, 0, 0);
        } if (num == '5'){
            set_pixel(y1, col, 255, 0, 0);
            set_pixel(y20, col, 255, 0, 0);
            set_pixel(y10, col, 255, 0, 0);
        } if (num == '6'){
            set_pixel(y20, col, 255, 0, 0);
            set_pixel(y10, col, 255, 0, 0);
        } if (num == '7'){
            set_pixel(y1, col, 255, 0, 0);
        }
        if (num == '8'){
            set_pixel(y1, col, 255, 0, 0);
            set_pixel(y20, col, 255, 0, 0);
            set_pixel(y10, col, 255, 0, 0);
        } if (num == '9'){
            set_pixel(y1, col, 255, 0, 0);
            set_pixel(y10, col, 255, 0, 0);
        }
    }
    for (int row = y1; row < y2; row++) {
        int x0 = x1 + 10;
        int newRow = 20;
        if (num == '0'){
            set_pixel(row, x1, 255, 0, 0);
            set_pixel(row, x0, 255, 0, 0);
        } if (num == '1'){
            set_pixel(row, x1, 255, 0, 0);
        } if (num =='2'){
            if (row > 60){
                break;
            }
            set_pixel(row, x0, 255, 0, 0);
            set_pixel(row+10, x1, 255, 0, 0);
        } if (num == '3'){
            set_pixel(row, x0, 255, 0, 0);
            set_pixel(newRow, x1, 255, 0, 0);
        } if (num == '4'){
            set_pixel(row, x0, 255, 0, 0);
        if (row < 60){
            set_pixel(row, x1, 255, 0, 0); }
        } if (num == '5'){
            if (row < 60){
                set_pixel(row, x1, 255, 0, 0); }
            if (row > 60){
                set_pixel(row, x0, 255, 0, 0);
            }
        } if (num == '6'){
            if (row > 60){
                set_pixel(row, x0, 255, 0, 0);
            }
            set_pixel(row, x1, 255, 0, 0);
        } if (num == '7'){
            set_pixel(row, x0, 255, 0, 0);
        }
        if (num == '8'){
            set_pixel(row, x0, 255, 0, 0);
            set_pixel(row, x1, 255, 0, 0);
        } if (num == '9'){
            set_pixel(row, x0, 255, 0, 0);
            if (row < 60){
                set_pixel(row, x1, 255, 0, 0); 
            }
        }
    }
}

int displayTime(char *time) {
    take_picture();
    for (int i = 0; i < 5; i++) {
        drawLine(15 + 30 * i, 25 + 30 * i, 50, 70, time[i]);
    }
    display_picture();
    sleep(10);
}


int createLog(char *time) {
    FILE *fp;
    char filename[] = {"rubylog.txt"};

    fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Error opening log file due to system error.");
        return 0; // Keep the program running even though file failed to write.
    }

    fprintf(fp, "Ruby was stolen at: %s\n", time);

    printf("Log file of stolen time written to: '%s'.\n", filename);

    fclose(fp);
    return 0;
}

int main(void) {
    if (start_camera() != 0) {
		printf("Failed to start camera\n");
		return 1;
	}

    for(int i = 0; i < 10; i++){
        take_picture();
        display_picture();
    }

    struct pointLocations rubyLocation;
    struct detectionPoints points;
    bool rubyStolen = false;

    char timeV[10];
    int pxNum = 0;
    int cNum = 0;

    if(!locateRuby(&rubyLocation, &pxNum)) {
        displayRed();
	printf("Ruby not present\n");
        sleep(5);
        return 0;
    } else {
        calculateDetectionPoints(&rubyLocation, &points);
        // Loop through detection - need to add showing green, and actual pictutre
        while (true) {
            take_picture();
            memset(points.stolen, 0, sizeof(points.stolen)); // Set points to zero
            cNum = 0;
            for (int row = 0 ; row < 480 ; row += 4) {	
                for (int col = 0; col < 640; col += 4) {
                    uchar r, g ,b;
                    get_pixel(row, col, &r, &g ,&b);
                    if ( (r  > b * 1.5) && ( r  > g * 1.5) ) {
                        cNum++;
                    }
                }
            }

            for (int i = 0; i < 4; i++) {
                uchar r, g ,b;
                get_pixel(points.y[i], points.x[i], &r, &g, &b);
                if ( (r  < b * 1.5) || ( r  < g * 1.5) ) {
                    points.stolen[i] = 1;
                }
            }

            if (points.stolen[0] == 1 && points.stolen[1] == 1 && points.stolen[2] == 1 && points.stolen[3] == 1 ||  cNum > pxNum * 1.5) {
                getTime(timeV);
                displayTime(timeV);
                createLog(timeV);
                sleep(10);
                break;
            } else {
                for (int y = 0; y < 50; y++) {
                    for (int x = 0; x < 50; x++) {
                        set_pixel(y, x, 0, 255, 0);
                    }
                }
            }
            display_picture();
        }
    }
    return 0;
}
