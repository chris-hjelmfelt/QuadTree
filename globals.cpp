/**
 * @file
 *
 * @brief Initializations for globals.h
 */

#include "globals.h"
#include "quadTree.h"

int ScreenWidth = 1024;

int ScreenHeight = 768;

byte* BMPimage;
byte* image;
byte* image2;
byte* image3;
int nrows, ncols;
int fudge;
quadTree* ourTree = new quadTree;
bool overlay = false;

