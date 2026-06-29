/*
    All code written by @SebJNZ, and is licensed under the 
    GNU Affero General Public License v3.0.
    You must follow and understand all permissions, rules, 
    and limitations stated under this license.

*/

#include "main.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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

int totalTestsPass = 0;
int totalTests = 0;

int testLocateRuby() {
    int pxNum = 0;
    struct pointLocations locations;

    // Test 1 - white background - red square in center.
    printf("--- RUNNING T1: White background with Red Square in center ---\n");
    setBackgroundColor(255, 255, 255);
    setPerfectRuby(true);
    bool found = locateRuby(&locations, &pxNum);
    totalTests++;
    if (found) {
        printf("T1 PASSED, Ruby location:\nTopY: %d\nBottomY: %d\nLeftX: %d\nRightX: %d\nRed Pixels: %d\n", locations.topY, locations.bottomY, locations.leftX, locations.rightX, pxNum);
        totalTestsPass++;
    } else {
        printf("T1 FAILED, Red Pixels: %d\n", pxNum);
    }

    // Test 2 - white background - no red square.
    printf("\n--- RUNNING T2: White background with no Red Square ---\n");
    pxNum = 0;
    setPerfectRuby(false);
    found = locateRuby(&locations, &pxNum);
    totalTests++;
    if (found) {
        printf("T2 FAILED, Ruby location:\nTopY: %d\nBottomY: %d\nLeftX: %d\nRightX: %d\nRed Pixels: %d\n", locations.topY, locations.bottomY, locations.leftX, locations.rightX, pxNum);
    } else {
        printf("T2 PASSED, Red Pixels: %d\n", pxNum);
        totalTestsPass++;
    }

    // Test 3 - locations as a null pointer.
    printf("\n--- RUNNING T3: Null Pointer Check (locations) ---\n");
    setPerfectRuby(true);
    found = locateRuby(NULL, &pxNum);
    totalTests++;
    if (!found) {
        printf("T3 PASSED, detected null pointer.\n");
        totalTestsPass++;
    } else {
        printf("T3 FAILED - didn't catch null pointer.\n");   
    }
    
    // Test 4 - pxNum as a null pointer.
    printf("\n--- RUNNING T4: Null Pointer Check (pxNum) ---\n");
    printf("STATUS: SKIPPED (KNOWN VULNERABILITY)\n");
    printf("REASON: locateRuby() lacks a safety check for a null redpx pointer.\n");
    printf("Running this test currently causes a fatal Segmentation Fault.\n");
    printf("To verify the crash, uncomment the block at line 72\n");
    totalTests++;

    /* === UNCOMMENT BELOW TO TRIGGER SEGFAULT ===
    found = locateRuby(&locations, NULL);
    if (!found) {
        printf("\n\nT4 PASSED, detected null pointer.\n");
    } else {
        printf("\n\nT4 FAILED - didn't catch null pointer.\n");   
    }
    */

    // Test 5 - Extreme ruby locations (Part 1)
    printf("\n--- RUNNING T5: Extreme ruby locations (1, 1)->(2, 2) - \"The Ghost Ruby\" ---\n");
    setPerfectRuby(true);
    setRubyLocations(1, 1, 2, 2);
    found = locateRuby(&locations, &pxNum);
    totalTests++;
    if (!found) {
        printf("T5 FAILED: Ruby missed due to += 4 stride optimization.\n");
    } else {
        printf("T5 PASSED: Somehow found a ruby it should have skipped.\n");
        totalTestsPass++;
    }

    // Test 6 - Extreme ruby locations (Part 2)
    printf("\n--- RUNNING T6: Extreme ruby locations (636, 476)->(639, 479) ---\n");
    setRubyLocations(636, 476, 639, 479);
    found = locateRuby(&locations, &pxNum);
    totalTests++;
    if (found) {
        printf("T6 PASSED, Ruby location:\nTopY: %d\nBottomY: %d\nLeftX: %d\nRightX: %d\nRed Pixels: %d\n", locations.topY, locations.bottomY, locations.leftX, locations.rightX, pxNum);
        totalTestsPass++;
    } else {
        printf("T6 FAILED, Red Pixels: %d\n", pxNum);
    }
    return 0;
}

// Helper function
int verifyPointsInRuby(struct pointLocations *loc, struct detectionPoints *dPoints) {
    printf("Point Locations: leftX: %u rightX: %u topY: %u bottomY: %u\n", loc->leftX, loc->rightX, loc->topY, loc->bottomY);

    int count = 0;
    int minX = (loc->leftX < loc->rightX) ? loc->leftX : loc->rightX;
    int maxX = (loc->leftX > loc->rightX) ? loc->leftX : loc->rightX;
    int minY = (loc->topY < loc->bottomY) ? loc->topY : loc->bottomY;
    int maxY = (loc->topY > loc->bottomY) ? loc->topY : loc->bottomY;
    for (int i = 0; i < 4; i++) {
        printf("Detection Point %d: [%u, %u]\n", i+1, dPoints->x[i], dPoints->y[i]);
        if (dPoints->x[i] >= minX && dPoints->x[i] <= maxX && dPoints->y[i] >= minY && dPoints->y[i] <= maxX) {
            count++;
            continue;
        }
    }
    totalTests++;
    return count;
}

int testCalculateDetectionPoints() {
    struct pointLocations locations;
    struct detectionPoints dPoints;

    // Test 1 - Standard Rectangle Ruby
    printf("--- RUNNING T1: Standard Rectangle Ruby ---\n");
    int count = 0;
    locations.leftX = 100;
    locations.rightX = 300;
    locations.topY = 50;
    locations.bottomY = 130;
    calculateDetectionPoints(&locations, &dPoints);

    if (verifyPointsInRuby(&locations, &dPoints) == 4){
        printf("T1 PASSED - all points in ruby.\n");
        totalTestsPass++;
    } else {
        printf("T1 FAILED - not all points were in ruby.\n");
    }

    // Test 2 - Odd Center Square
    printf("--- RUNNING T2: Odd Center Square ---\n");
    count = 0;
    locations.leftX = 10;
    locations.rightX = 25;
    locations.topY = 10;
    locations.bottomY = 25;
    calculateDetectionPoints(&locations, &dPoints);

    if (verifyPointsInRuby(&locations, &dPoints) == 4){
        printf("T2 PASSED - all points in ruby.\n");
        totalTestsPass++;
    } else {
        printf("T2 FAILED - not all points were in ruby.\n");
    }

    // Test 3 - Fractional Increment
    printf("--- RUNNING T3: Fractional Increment ---\n");
    count = 0;
    locations.leftX = 10;
    locations.rightX = 97;
    locations.topY = 10;
    locations.bottomY = 97;
    calculateDetectionPoints(&locations, &dPoints);

    if (verifyPointsInRuby(&locations, &dPoints) == 4){
        printf("T3 PASSED - all points in ruby.\n");
        totalTestsPass++;
    } else {
        printf("T3 FAILED - not all points were in ruby.\n");
    }

    // Test 4 - Inverted Box
    printf("--- RUNNING T4: Inverted Box ---\n");
    count = 0;
    locations.leftX = 50;
    locations.rightX = 20;
    locations.topY = 50;
    locations.bottomY = 20;
    calculateDetectionPoints(&locations, &dPoints);

    if (verifyPointsInRuby(&locations, &dPoints) == 4){
        printf("T4 PASSED - all points in ruby.\n");
        totalTestsPass++;
    } else {
        printf("T4 FAILED - not all points were in ruby.\n");
    }

    // Test 5 - Massive Values
    printf("--- RUNNING T5: Massive Values ---\n");
    count = 0;
    locations.leftX = 0;
    locations.rightX = 65535;
    locations.topY = 0;
    locations.bottomY = 65535;
    calculateDetectionPoints(&locations, &dPoints);

    if (verifyPointsInRuby(&locations, &dPoints) == 4){
        printf("T5 PASSED - all points in ruby.\n");
        totalTestsPass++;
    } else {
        printf("T5 FAILED - not all points were in ruby.\n");
    }

    // Test 6 - Massive Values
    printf("--- RUNNING T6: Null Structs ---\n");
    printf("We will get a segmentation fault if this fails...\n");
    calculateDetectionPoints(NULL, NULL);
    printf("T6 PASSED - null pointers mitigated\n");
    totalTests++;
    totalTestsPass++;
    return 0;
}

int testGetTime() {
    char time[10];
    getTime(time);

    bool failed = false;

    for (int i = 0; i < 5; i++) {
        printf("%c", time[i]);
        if (time[i] == ':') {
            continue;
        }

        if (!isdigit(time[i])) {
            failed = true;
            break;
        }
    }
    totalTests++;

    if (time[5] != '\0') {
        failed = true;
    }

    if (failed == true) {
        printf("\nTest Failed\n");
    } else {
        printf("\nTest Passed\n");
        totalTestsPass++;
    }
    return 0;
}

int testDrawLine() {
    printf("--- RUNNING T1: Draw Number '1' ---\n");
    totalTests++;
    clear_mock_screen();
    
    drawLine(10, 20, 30, 50, '1');
    
    bool passed = true;

    if (mock_screen[35][10][0] != 255) {
        printf("T1 FAILED: Expected Red pixel at (Y:35, X:10) was missing.\n");
        passed = false;
    }

    if (mock_screen[35][11][0] == 255) {
        printf("T1 FAILED: Found Red pixel at (Y:35, X:11) where it should be blank.\n");
        passed = false;
    }

    if (passed) {
        printf("T1 PASSED - red pixel found at correct location.\n");
        totalTestsPass++;
    }

    printf("--- RUNNING T2: Checking out of bounds ---\n");
    totalTests++;
    clear_mock_screen();

    drawLine(-1, 640, -1, 480, '8');

    passed = true;
    
    if (mock_screen[19][0][0] != 255) {
        printf("T2 FAILED: Expected Red pixel at (Y:19, X:0) was missing.\n");
        passed = false;
    }

    if (mock_screen[0][0][0] == 255) {
        printf("T2 FAILED: Found Red pixel at (Y:0, X:0) where it should be blank.\n");
        passed = false;
    }

    if (passed) {
        printf("T2 PASSED: red pixel found at correct location.\n");
        totalTestsPass++;
    }

    printf("--- RUNNING T3: Checking random characters ---\n");
    totalTests++;
    clear_mock_screen();
    setPixelRanFalse();

    drawLine(0, 50, 0, 50, 'r');

    if(checkRan) {
        printf("T3 FAILED: Function attempted to draw something.]n");
    } else {
        totalTestsPass++;
        printf("T3 PASSED: Function avoided illegal character\n");
    }
}

int readLog(char *time) {
    FILE *fp;
    char filename[] = {"rubylog.txt"};

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("TEST FAILED, unable to read file due to system error.\n");
        return 1;
    }

    int items_read = fscanf(fp, "Ruby was stolen at: %[^\n]", time);

    fclose(fp);
    if (items_read != 1) {
        printf("Error, file not formatted correctly.\n");
        return 1;
    }
    return 0;
}

int testCreateLog() {
    char time[] = {"10:00"};
    
    createLog(time);

    char time_result[6]; 
    if (readLog(time_result) == 0) {
        if (strcmp(time, time_result) == 0) {
            printf("T1 PASSED - times are the same\n");
            totalTestsPass++;
        } else {
            printf("Times not the same\n");
        }
    }
    totalTests++;
    return 0;
}
 
int main(void) {
    printf("--- FUNCTION 1 - locateRuby() ---\n");
    testLocateRuby();
    printf("\n\n--- FUNCTION 2 - calculateDetectionPoints() ---\n");
    testCalculateDetectionPoints();
    printf("\n\n--- FUNCTION 3 - getTime() ---\n");
    testGetTime();
    printf("\n\n--- FUNCTION 4 - createLog() ---\n");
    testCreateLog();
    printf("\n\n--- FUNCTION 5 - drawLine() ---\n");
    testDrawLine();

    double percent = ((double)totalTestsPass / totalTests) * 100;
    printf("\nTest Pass Rate: %d/%d = %.2f%%\n", totalTestsPass, totalTests, percent);
    return 0;
}