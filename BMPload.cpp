/**
* @file	BMPload.cpp
*
* @brief Read in a 24-bit BMP image file. CSC300 Data Structures, Fall 2015
* Ref: Based on code from RgbImage.cc (Buss, 3-D Computer Graphics, 2003).
*
* @author John M. Weiss, Ph.D.
* @author Cheldon Coughlen
* @author Chris Hjelmfelt
*/

#include <cstdio>

// prototypes
bool LoadBmpFile( const char* filename, int &NumRows, int &NumCols, unsigned char* &ImagePtr );
static short readShort( FILE* infile );
static int readLong( FILE* infile );
static void skipChars( FILE* infile, int numChars );
static inline int GetNumBytesPerRow( int NumCols );

 /**************************************************************************//** 
 * @author Sam Buss December 2001.
 * 
 * @par Description: 
 * LoadBmpFile Read into memory an RGB image from an uncompressed BMP file.
 * Return true for success, false for failure.
 * 
 * @param[in]       filename - the name of the file
 * @param[in,out]   &NumRows - number of rows
 * @param[in,out]   &NumCols - number of columns
 * @param[in,out]   &ImagePtr - empty image array to fil
 * 
 * @returns true    if file was successfully read in
 * @returns false   if there was an error
 * 
 *****************************************************************************/

bool LoadBmpFile( const char* filename, int &NumRows, int &NumCols, unsigned char* &ImagePtr )
{
    FILE* infile = fopen( filename, "rb" );		// open binary BMP file for reading
    if ( !infile )
    {
        fprintf( stderr, "Unable to open file: %s\n", filename );
        return false;
    }

    // process BMP file header
    bool fileFormatOK = false;
    int bChar = fgetc( infile );
    int mChar = fgetc( infile );
    if ( bChar == 'B' && mChar == 'M' )			// should start with "BM" for "BitMap"
    {
        skipChars( infile, 4 + 2 + 2 + 4 + 4 );		// Skip 4 fields we don't care about
        NumCols = readLong( infile );
        NumRows = readLong( infile );
        skipChars( infile, 2 );				// Skip one field
        int bitsPerPixel = readShort( infile );
        skipChars( infile, 4 + 4 + 4 + 4 + 4 + 4 );	// Skip 6 more fields

        // check for valid image metadata
        if ( NumCols > 0 && NumCols <= 100000 && NumRows > 0 && NumRows <= 100000
                && bitsPerPixel == 24 && !feof( infile ) )
        {
            fileFormatOK = true;
        }
    }

    if ( !fileFormatOK )
    {
        fclose ( infile );
        fprintf( stderr, "Not a valid 24-bit bitmap file: %s.\n", filename );
        return false;
    }

    // allocate memory
    ImagePtr = new unsigned char[ NumRows * GetNumBytesPerRow( NumCols ) ];
    if ( !ImagePtr )
    {
        fclose ( infile );
        fprintf( stderr, "Unable to allocate memory for %i x %i bitmap: %s.\n",
                 NumRows, NumCols, filename );
        return false;
    }

    // read image RGB data
    unsigned char* cPtr = ImagePtr;
    for ( int i = 0; i < NumRows; i++ )
    {
        int j;
        for ( j = 0; j < NumCols; j++ )
        {
            *( cPtr + 2 ) = fgetc( infile );	// Blue color value
            *( cPtr + 1 ) = fgetc( infile );	// Green color value
            *cPtr = fgetc( infile );		// Red color value
            cPtr += 3;
        }
        int k = 3 * NumCols;			// number of bytes already read
        for ( ; k < GetNumBytesPerRow( NumCols ); k++ )
        {
            fgetc( infile );			// ignore padding
            *( cPtr++ ) = 0;
        }
    }

    // and... we should be done
    if ( feof( infile ) )
    {
        fclose ( infile );
        fprintf( stderr, "Premature end of file: %s.\n", filename );
        return false;
    }

    fclose( infile );	// Close the file

    return true;
}

/* ********************************************************************
 *  @par GetNumBytesPerRow rows are word aligned
 *  @author John M. Weiss, Ph.D.
 *
 *  @param[in]       NumCols - number of columns in file
 *  @returns  	     number of bytes per row
 **********************************************************************/ 
static inline int GetNumBytesPerRow( int NumCols )
{
    return ( ( 3 * NumCols + 3 ) >> 2 ) << 2;
}

/* ********************************************************************
 *  @par readShort read a 16-bit integer from the input file
 *  @author John M. Weiss, Ph.D.
 *
 *  @param[in]       infile - the file we are reading from
 *  @returns  	     ret
 **********************************************************************/  
short readShort( FILE* infile )
{
    unsigned char lowByte, hiByte;
    lowByte = fgetc( infile );			// Read the low order byte (little endian form)
    hiByte = fgetc( infile );			// Read the high order byte

    // Pack together
    short ret = hiByte;
    ret <<= 8;
    ret |= lowByte;
    return ret;
}

/* ********************************************************************
 *  @par readLong read a 32-bit integer from the input file
 *  @author John M. Weiss, Ph.D.
 *
 *  @param[in]       infile - the file we are reading from
 *  @returns  	     ret
 **********************************************************************/   
int readLong( FILE* infile )
{
    unsigned char byte0, byte1, byte2, byte3;
    byte0 = fgetc( infile );			// Read bytes, low order to high order
    byte1 = fgetc( infile );
    byte2 = fgetc( infile );
    byte3 = fgetc( infile );

    // Pack together
    int ret = byte3;
    ret <<= 8;
    ret |= byte2;
    ret <<= 8;
    ret |= byte1;
    ret <<= 8;
    ret |= byte0;
    return ret;
}

/* ********************************************************************
 *  @par skipChars skip over given number of bytes in input file
 *  @author John M. Weiss, Ph.D.
 *
 *  @param[in]       infile - the file we are reading from
 *  @param[in]       numChars - number of characters to skip
 **********************************************************************/
void skipChars( FILE* infile, int numChars )
{
    for ( int i = 0; i < numChars; i++ )
    {
        fgetc( infile );
    }
}
