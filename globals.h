/**
 *  @file
 *  @brief This file contains global variables for various functions, 
 *  three image arrays, and our quadtree
 *
 *  @author Cheldon Coughlen
 *  @author Chris Hjelmfelt
 */
#ifndef _GLobals_H_
#define _GLobals_H_

#include "quadTree.h"

/// Definition of a new type for holding pixel values
typedef unsigned char byte;

// Global Variables
/// width of the window we create
extern int ScreenWidth;
/// height of the window we create
extern int ScreenHeight;
/// array of bytes to store RGB pixel values (range 0 to 255)
extern byte* BMPimage;
/// array of bytes to store monochrome pixel values before quadtree encoding	
extern byte* image; 
/// array of bytes to store monochrome pixel values after quadtree encoding	      
extern byte* image2;
/// array of bytes to store monochrome pixel values with overlay
extern byte* image3;
/// image dimensions in rows
extern int nrows;
/// image dimensions in columns
extern int ncols;  
/// quality factor 
extern int fudge;
/// the quadtree
extern quadTree* ourTree;
///Bool to toggle overlay
extern bool overlay;
#endif
