/*************************************************************************//**
 * @file 
 *
 * @brief The main file to run the program and display functions
 *
 * @mainpage Program #2 - Quadtree encoding
 * 
 * @section course_section Course Information 
 *
 * @author Cheldon Coughlen
 * @author Chris Hjelmfelt
 * 
 * @date Oct. 26, 2015
 * 
 * @par Professor: 
 *         Dr. Hinker
 * 
 * @par Course: 
 *         CSC300 - M001 -  10:00 a.m.
 * 
 * @par Location: 
 *         McLaury - 313
 *
 * @section program_section Program Information 
 * 
 * @details This program uses a quadTree class to encode an image by finding 
 * regions of pixels within an image that are of similar light intensity and
 * then prints a decoded image where those regions are all at the same mean 
 * value. By changing the quality factor we can compress an image to different
 * sizes
 *
 * This program takes a 24 bit color bmp image and converts it to 8 bit 
 * monochrome image which is then converted into a quadtree. The quadtree is 
 * then decoded into a new image. Pressing the spacebar 
 * toggles an overlay outlining the regions where pixels of similar intensity 
 * have been averaged together
 *
 * As the quality factor increases, we soon see a significant drop in file size.
 * With very low quality factors, the compression method creates a larger file
 * because of the quadtree overhead. There is a range at which the picture
 * quality is mostly retained, while producing a smaller file size as a result
 * having more compression. At some point, the image is degraded to such a 
 * degree where the quality is too low. Using this program, we can find the
 * optimal balance between quality and compression.
 *
 * @section compile_section Compiling and Usage 
 *
 * @par Compiling Instructions: 
 *      g++ -o quadTree globals.cpp BMPdisplay.cpp BMPload.cpp quadTree.cpp
 *  -lglut -lGLU -lGL -lm -std=c++11 -g
 * 
 * Makefile included to compile the program. Type "make" in the directory of
 * all the files.
 * 
 * @par Usage: 
   @verbatim  
   c:\> ./quadTree [bmp image filename] [fudge factor] 
   d:\> c:\bin\./quadTree [bmp image filename] [fudge factor]

   Where filename is the name of the image file, fudge factor is the 
   tolerance level or compression factor. Both arguments without brackets.
   
   Spacebar toggles the quadtree overlay
   Escape exits the program
   @endverbatim 
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 * 
 * @bug none
 * 
 * @todo none
 * 
 * @par Modifications and Development Timeline: 
   @verbatim 
   Date          Modification 
   ------------  -------------------------------------------------------------- 
   Oct  8, 2011  Started project, made Node structure and program layout
   Oct 10, 2015  Planned out valueMatch and fillTree functions
   Oct 13, 2015  Wrote quadTree class and functions
   Oct 18, 2015  Troubleshooting through output missign regions
   Oct 19, 2015  Troubleshooting with Dr. Hinker about filling
   Oct 20, 2015  Program draws overlay and output, documentation written
   @endverbatim
 *
 *****************************************************************************/

//Includes
#include <iostream>
#include <GL/glut.h>
#include <cstdlib>
#include <GL/freeglut.h>
#include "quadTree.h"
#include "globals.h"

using namespace std;

///Key constant used for keyboard input
const int EscapeKey = 27;

// OpenGL callback function prototypes
void display( void );
void reshape( int w, int h );
void keyboard( unsigned char key, int x, int y );

// other function prototypes
void initOpenGL( const char *filename, int nrows, int ncols );
bool LoadBmpFile( const char* filename, int &nrows, int &ncols, byte* &image );
void displayColor( int x, int y, int w, int h, byte *image );
void displayMonochrome( int x, int y, int w, int h, byte *image );
void imageInfo( char *argv);
void DrawTextString (char *string, int x, int y, const float color[]);

/**************************************************************************//** 
 * @author John M. Weiss, Ph.D.
 * @author Cheldon Coughlen
 * @author Chris Hjelmfelt
 *
 * @par Description: 
 * Checks for proper arguments, converts color array to monochrome,
 * initializes openGL and glut, calls functions: LoadBmpFile, ourTree->fillTree,
 * imageInfo, glutInit, initOpenGL, glutMainLoop
 * 
 * @param[in]	argc - number of arguments
 * @param[in]	*argv[] - 2 arguments: image name and quality factor
 * 
 * @returns 0 program ran successful.
 * 
 *****************************************************************************/
int main( int argc, char *argv[] )
{
    if ( argc < 2 )
    {
        cerr << "Usage: BMPdisplay image.bmp\n";
        return -1;
    }

    // read image file
    if ( !LoadBmpFile( argv[1], nrows, ncols, BMPimage ) )
    {
        cerr << "Error: unable to load " << argv[1] << endl;
        return -1;
    }
    cerr << "reading " << argv[1] << ": " << nrows << " x " << ncols << endl;
	fudge = atoi(argv[2]);
	
    // convert 24-bit color BMP image to 8-bit monochrome image
    image = new byte [nrows * ncols ];
    image2 = new byte [ nrows * ncols ];
    image3 = new byte [ nrows * ncols ];
    byte* BMPptr = BMPimage, *imageptr = image;
    for ( int row = 0; row < nrows; row++ )
        for ( int col = 0; col < ncols; col++ )
        {
            *imageptr++ = 0.30 * BMPptr[0] + 0.59 * BMPptr[1] + 0.11 * BMPptr[2] + 0.5;
            BMPptr += 3;
        }
        
    //Fill the tree and print out the image information
	ourTree->fillTree(ourTree->root, 0, 0, nrows);
	imageInfo( argv[1]);
	
    // perform various OpenGL initializations
    glutInit( &argc, argv );
    initOpenGL( argv[1], nrows, ncols );

    // go into OpenGL/GLUT main loop, never to return
    glutMainLoop();

    return 0;
}

 /**************************************************************************//** 
 * @author Cheldon Coughlen
 * @author Chris Hjelmfelt
 * 
 * @par Description: 
 * Print out information about the images and nodes for the user
 *
 * 
 * 
 * @param[in]   argv - array holding the name of the image passed in
 * 
 *****************************************************************************/
void imageInfo( char *argv)
{
	cout << ncols * nrows << " 8-bit pixels in image (" << ncols * nrows <<
		 " bytes)." << endl;
	cout << ourTree->nodes() << " nodes and " << ourTree->leaves() << 
		" leaves in quadtree (" << 2 * ourTree->leaves() << " bytes)." << endl;
	cout << "The quadtree size is about " << 
		int(100 * float(2 * ourTree->leaves()) / (nrows * ncols)) 
		<< "% of the uncompressed image size." << endl;
}

/**************************************************************************//** 
 * @author John M. Weiss, Ph.D.
 * @author Cheldon Coughlen
 * @author Chris Hjelmfelt
 * 
 * @par Description: 
 * Various commands to initialize OpenGL and GLUT
 * 
 * @param[in]   *filename - the imagefile that was passed into the program
 * @param[in]   nrows - the number of pixils high
 * @param[in]   ncols - the number of pixils wide
 * 
 * 
 *****************************************************************************/
void initOpenGL( const char *filename, int nrows, int ncols )
{
	// 32-bit graphics and single buffering
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE );	        

    ScreenWidth = 2 * ncols;
    ScreenHeight = nrows;
    glutInitWindowSize( ScreenWidth, ScreenHeight + 24 );	// initial window size
    glutInitWindowPosition( 100, 50 );			    // initial window  position
    glutCreateWindow( filename );			            // window title

    glClearColor( 0.0, 0.0, 0.0, 0.0 );   // use black for glClear command

    // callback routines
    glutDisplayFunc( display );				// how to redisplay window
    glutReshapeFunc( reshape );				// how to resize window
    glutKeyboardFunc( keyboard );			// how to handle key presses
}

/******************************************************************************/
/*                          OpenGL callback functions                         */
/******************************************************************************/
 
/**************************************************************************//** 
 * @author John M. Weiss, Ph.D.
 * @author Cheldon Coughlen
 * @author Chris Hjelmfelt
 * 
 * @par Description: 
 * Callback function that tells OpenGL how to redraw window
 *
 *****************************************************************************/
void display( void )
{
	const float color[3] = {1.0, 1.0, 1.0};
	char original[20] = "Original Image";
	char quad[40] = "Quadtree Image (spacebar toggles quads)";
    // clear the display
    glClear( GL_COLOR_BUFFER_BIT );
    

    // display image in color and monochrome
    //displayColor( 0, 0, ncols, nrows, BMPimage );
    displayMonochrome( 0, 0, ncols, nrows, image );
    displayMonochrome(ncols, 0, ncols, nrows, (overlay ? image3 : image2));
    
    //Title bar to label the images
    DrawTextString( original , 20, nrows + 5, color);
    DrawTextString( quad, ncols + 20, nrows + 5, color);
    
    // flush graphical output
    glFlush();
}

/**************************************************************************//** 
 * @author John M. Weiss, Ph.D.
 * 
 * @par Description: 
 * Callback function that tells OpenGL how to resize window.
 * Note that this is called when the window is first created.
 * 
 * @param[in]   w - window width
 * @param[in]   h - window height
 * 
 *****************************************************************************/
void reshape( int w, int h )
{
    // store new window dimensions globally
    ScreenWidth = w;
    ScreenHeight = h;

    // how to project 3-D scene onto 2-D
    glMatrixMode( GL_PROJECTION );		// use an orthographic projection
    glLoadIdentity();				// initialize transformation matrix
    gluOrtho2D( 0.0, w, 0.0, h );
    glViewport( 0, 0, w, h );			// adjust viewport to new window
}

/**************************************************************************//** 
 * @author John M. Weiss, Ph.D.
 * @author Chris Hjelmfelt
 * @author Cheldon Coughlen
 * 
 * @par Description: 
 * Callback function that tells OpenGL how to handle keystrokes
 * 
 * @param[in]   key - key that was pressed
 * @param[in]   x - x coordinate
 * @param[in]   y - y coordinate
 * 
 *****************************************************************************/
void keyboard( unsigned char key, int x, int y )
{
	static bool flag = false;
    switch ( key )
    {
            // Escape quits program
        case EscapeKey:
        	delete [] image;
        	delete [] image2;
        	delete [] image3;
        	ourTree->~quadTree();
            exit( 0 );
            break;
        case 32: // Spacebar was pressed
        	overlay = !overlay;
        	glutPostRedisplay();
			break;
            // anything else redraws window
        default:
            glutPostRedisplay();
            break;
    }
}

/**************************************************************************//** 
 * @author John M. Weiss, Ph.D.
 * 
 * @par Description: 
 * Display 24-bit color image
 * 
 * @param[in]   x - x coordinate
 * @param[in]   y - y coordinate
 * @param[in]   w - window width
 * @param[in]   h - window height
 * @param[in]   image - the color image array
 * 
 *****************************************************************************/
void displayColor( int x, int y, int w, int h, byte* image )
{
    glRasterPos2i( x, y );
    glDrawPixels( w, h, GL_RGB, GL_UNSIGNED_BYTE, BMPimage );
}

/**************************************************************************//** 
 * @author John M. Weiss, Ph.D.
 * 
 * @par Description: 
 * Display 8-bit monochrome image
 * 
 * @param[in]   x - x coordinate
 * @param[in]   y - y coordinate
 * @param[in]   w - window width
 * @param[in]   h - window height
 * @param[in]   image - the color image array
 * 
 *****************************************************************************/
void displayMonochrome( int x, int y, int w, int h, byte* image )
{
    glRasterPos2i( x, y );
    glDrawPixels( w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, image );
}

/**************************************************************************//** 
 * @author John M. Weiss, Ph.D.
 * 
 * @par Description: 
 * Draws a text string at the x, y, coordinates in the color
 * passed in.
 * 
 * @param[in]   string - the text array 
 * @param[in]   x - x coordinate
 * @param[in]   y - y coordinate
 * @param[in]   color - the color to draw text
 * 
 *****************************************************************************/
void DrawTextString (char *string, int x, int y, const float color[])
{
	glColor3fv(color);
	glRasterPos2i (x, y);
	while ( *string )
		glutBitmapCharacter (GLUT_BITMAP_8_BY_13, *string++);
}
