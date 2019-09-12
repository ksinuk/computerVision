//=============================================================================
// Copyright © 1998-2000 Point Grey Research, Inc. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Point
// Grey Research, Inc. ("Confidential Information").  You shall not
// disclose such Confidential Information and shall use it only in
// accordance with the terms of the license agreement you entered into
// with PGR.
// 
// PGR MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, OR NON-INFRINGEMENT. PGR SHALL NOT BE LIABLE FOR ANY DAMAGES
// SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================

//=============================================================================
// Includes
//=============================================================================
#include "stdafx.h"
#include "stdlib.h"
#include "wingdi.h"
#include "drawImage.h"

//=============================================================================
// Definitions
//=============================================================================
/* shorthand for getting the width and height of a rect structure */
#define RECTWIDTH(  lpRect )    ( (lpRect)->right  - (lpRect)->left )
#define RECTHEIGHT( lpRect )    ( (lpRect)->bottom - (lpRect)->top  )

/***********************************************
*   Function Name : DestroyBitmapInfo
*
*   Parameters :
*       oldBitmapInfo - handle to the bitmap info to be
*               destroyed.
*
*   Returned Value :
*       none.
*
*   Description :
*       This function destoys a bitmap info
*   data structure that has been previously created
*   by either CreateGreyscaleBitmapInfo or
*   CreateCompositeBitmapInfo. 
*
*   Modifications :
*
************************************************/
void DestroyBitmapInfo( HANDLE oldBitmapInfo )
{
    GlobalFree( oldBitmapInfo );
}

/***********************************************
*   Function Name : CreateGreyscaleBitmapInfo
*
*   Parameters :
*       numRows - number of rows in the image
*       numCols - number of columns in the image
*       pitch - the size of each row in byte
*       renderUp - a Boolean indicating the image supplied
*           is bottom up (TRUE) or top down (FALSE).
*
*   Returned Value :
*       A handle to a bitmap info data structure that is appropriate
*   for a 256 greyscale image of the specified size.
*
*   Description :
*       This function constructs a bitmap info
*   data structure that is appropriate for a 256 greyscale
*   image of the specified size. 
*
*   Modifications :
*
************************************************/

HANDLE CreateGreyscaleBitmapInfo(LONG numRows,
                                 LONG numCols,
                                 LONG pitch,
                                 BOOL renderUp )
{
    HANDLE   tempBitInfoHdl;
    BITMAPINFO* tempBitInfoPtr;
    int      i;

    /*
     * allocate storage for the bitmapInfo structure
     */
    tempBitInfoHdl = 
            GlobalAlloc(    GHND,
                            sizeof( BITMAPINFO )
                            +  sizeof( RGBQUAD )
                            *   256 );

    if( tempBitInfoHdl == 0 )
    {
        /* not encough memory, clean up and return NULL */
        return NULL;
    }

    /* lock down memory before accessing */
    tempBitInfoPtr = (BITMAPINFO*) 
                    GlobalLock( (HGLOBAL) tempBitInfoHdl );

    /* setup the 256 greyscale ramp */
    for( i=0; i<256; i++ )
    {
        tempBitInfoPtr->bmiColors[i].rgbBlue     = i;
        tempBitInfoPtr->bmiColors[i].rgbGreen    = i;
        tempBitInfoPtr->bmiColors[i].rgbRed      = i;
        tempBitInfoPtr->bmiColors[i].rgbReserved = 0;
    }

     /* set bmi Header */
    tempBitInfoPtr->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    tempBitInfoPtr->bmiHeader.biWidth = numCols;

    tempBitInfoPtr->bmiHeader.biHeight = 
        ( renderUp ) ?
        numRows :
        numRows * -1;   

    tempBitInfoPtr->bmiHeader.biPlanes = 1;
    tempBitInfoPtr->bmiHeader.biBitCount = 8;
    tempBitInfoPtr->bmiHeader.biCompression = BI_RGB;
    tempBitInfoPtr->bmiHeader.biSizeImage = numRows * pitch;
    tempBitInfoPtr->bmiHeader.biXPelsPerMeter = 0;
    tempBitInfoPtr->bmiHeader.biYPelsPerMeter = 0;
    tempBitInfoPtr->bmiHeader.biClrUsed = 0;
    tempBitInfoPtr->bmiHeader.biClrImportant = 0;

    /* all done, unlock access */
    GlobalUnlock( (HGLOBAL) tempBitInfoHdl );

    return tempBitInfoHdl;
}


/***********************************************
*   Function Name : CreateCompositeBitmapInfo
*
*   Parameters :
*       numRows - number of rows in the image
*       numCols - number of columns in the image
*       pitch - the size of each row in byte
*       renderUp - a Boolean indicating the image supplied
*           is bottom up (TRUE) or top down (FALSE).
*
*   Returned Value :
*       A handle to a bitmap info data structure that is appropriate
*   for a composite image of the specified size.  A composite image
*   is one supports 128 greyscales and 10 color description.
*   The pixel value in this case would range from 0 to 127 for greyscale
*   and from 128 beyond will specify some special built-in color that
*   can be used for overlay purposes.
*
*   Description :
*       This function constructs a bitmap info
*   data structure that is appropriate for a composite
*   image of the specified size. 
*
*   Modifications :
*
************************************************/

/* 128 greyscale + 6 colors */
HANDLE CreateCompositeBitmapInfo( LONG numRows,
                                 LONG numCols,
                                 LONG pitch,
                                 BOOL renderUp )
{
    HANDLE   tempBitInfoHdl;
    BITMAPINFO* tempBitInfoPtr;
    int      i;

    /*
     * allocate storage for the bitmapInfo structure
     */
    tempBitInfoHdl = 
            GlobalAlloc(    GHND,
                            sizeof( BITMAPINFO )
                            +  sizeof( RGBQUAD )
							*	256 );  

    if( tempBitInfoHdl == 0 )
    {
        /* not encough memory, clean up and return NULL */
        return NULL;
    }

    /* lock down memory before accessing */
    tempBitInfoPtr = (BITMAPINFO*) 
                    GlobalLock( (HGLOBAL) tempBitInfoHdl );

    /* setup the 128 greyscale ramp */
    for( i=0; i<128; i++ )
    {
        tempBitInfoPtr->bmiColors[i].rgbBlue     = i*2;
        tempBitInfoPtr->bmiColors[i].rgbGreen    = i*2;
        tempBitInfoPtr->bmiColors[i].rgbRed      = i*2;
        tempBitInfoPtr->bmiColors[i].rgbReserved = 0;
    }
    /* setup the remaining 6 colors */
    for( ; i<134; i++ )
    {
        tempBitInfoPtr->bmiColors[i].rgbBlue     = 0;
        tempBitInfoPtr->bmiColors[i].rgbGreen    = 0;
        tempBitInfoPtr->bmiColors[i].rgbRed      = 0;
        tempBitInfoPtr->bmiColors[i].rgbReserved = 0;
    }

    tempBitInfoPtr->bmiColors[128].rgbBlue       = 255;

    tempBitInfoPtr->bmiColors[129].rgbGreen      = 255;

    tempBitInfoPtr->bmiColors[130].rgbRed        = 255;

    tempBitInfoPtr->bmiColors[131].rgbBlue       = 255;
    tempBitInfoPtr->bmiColors[131].rgbGreen      = 255;

    tempBitInfoPtr->bmiColors[132].rgbBlue       = 255;
    tempBitInfoPtr->bmiColors[132].rgbRed        = 255;

    tempBitInfoPtr->bmiColors[133].rgbGreen      = 255;
    tempBitInfoPtr->bmiColors[133].rgbRed        = 255;

     /* set bmi Header */
    tempBitInfoPtr->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    tempBitInfoPtr->bmiHeader.biWidth = numCols;

    tempBitInfoPtr->bmiHeader.biHeight = 
        ( renderUp ) ?
        numRows :
        numRows * -1;   

    tempBitInfoPtr->bmiHeader.biPlanes = 1;
    tempBitInfoPtr->bmiHeader.biBitCount = 8;
    tempBitInfoPtr->bmiHeader.biCompression = BI_RGB;
    tempBitInfoPtr->bmiHeader.biSizeImage = numRows * pitch;
    tempBitInfoPtr->bmiHeader.biXPelsPerMeter = 0;
    tempBitInfoPtr->bmiHeader.biYPelsPerMeter = 0;
    tempBitInfoPtr->bmiHeader.biClrUsed = 0;
    tempBitInfoPtr->bmiHeader.biClrImportant = 0;

    /* all done, unlock access */
    GlobalUnlock( (HGLOBAL) tempBitInfoHdl );

    return tempBitInfoHdl;
}


/***********************************************
*   Function Name : PaintImage
*
*   Parameters :
*       pDisplay - handle to a device context within
*           which the image is rendered.
*       pDispRect - pointer to a rect describing the
*           portion of the device context to refresh.
*       pImgRect - pointer to a rect describing the
*           portion of the image to be re-drawn.
*       pImgPalette - a HPALLETTE object containing
*           the palette to be used in rendering the
*           image.
*
*   Returned Value :
*       A boolean variable indicating whether the 
*   rendering was successful.
*
*   Description :
*       This function renders a raw image contents to
*   a Win32 device context.
*
*
************************************************/

BOOL PaintImage( 
                    HDC        pDisplay,
                    LPRECT     pDispRect,
                    LPRECT     pImgRect,
                    HANDLE     thisBitmapInfoHdl,
                    void*      bitmapBits
                )
{
    BOOL        successCode = FALSE;
    BITMAPINFO* thisBitmapInfoPtr;


    /*
     *  lock down the bitmap info header
     */
    thisBitmapInfoPtr = (BITMAPINFO*) 
                        GlobalLock( (HGLOBAL) thisBitmapInfoHdl );

    /*
     * Make sure to use the stretching mode best for color pictures
     */
    SetStretchBltMode( pDisplay, COLORONCOLOR );

    /*
     * Determine whether to call StretchDIBits() or SetDIBitsToDevice()
     */
    if( ( RECTWIDTH( pDispRect ) == RECTWIDTH( pImgRect ) ) &&
        ( RECTHEIGHT(pDispRect ) == RECTHEIGHT(pImgRect ) ) )
    {
        LONG numRows = thisBitmapInfoPtr->bmiHeader.biHeight;

        if( numRows < 0 ) 
            numRows *= -1;

        successCode = SetDIBitsToDevice(
                            pDisplay,
                            pDispRect->left,
                            pDispRect->top,
                            RECTWIDTH( pDispRect ),
                            RECTHEIGHT( pDispRect ),
                            pImgRect->left,
                            numRows - pImgRect->bottom,
                            0,
                            numRows,
                            bitmapBits,
                            thisBitmapInfoPtr,
                            DIB_RGB_COLORS 
                          );
    }
    else
    {
        successCode = StretchDIBits(
                            pDisplay,
                            pDispRect->left,
                            pDispRect->top,
                            RECTWIDTH( pDispRect ),
                            RECTHEIGHT( pDispRect ),
                            pImgRect->left,
                            pImgRect->top,
                            RECTWIDTH( pImgRect ),
                            RECTHEIGHT( pImgRect ),
                            bitmapBits,
                            thisBitmapInfoPtr,
                            DIB_RGB_COLORS,
                            SRCCOPY
                          );
    }

    GlobalUnlock( (HGLOBAL) thisBitmapInfoHdl );

    return successCode;

}



/***********************************************
*   Function Name : ResizeBitmapInfo
*
*   Parameters :
*       thisBitmapInfoHdl - handle the bitmap info 
*           structure to be updated.
*       numRows - number of rows in the image
*       numCols - number of columns in the image
*       pitch - the size of each row in byte
*       renderUp - a Boolean indicating the image supplied
*           is bottom up (TRUE) or top down (FALSE).
*
*   Returned Value :
*       none.
*
*   Description :
*       This function update the bitmap info to reflect
*   changes in image dimension.
*
*
************************************************/

void ResizeBitmapInfo( HANDLE thisBitmapInfoHdl,
                       long numRows,
                       long numCols,
                       long pitch,
                       BOOL renderUp )
{
    BITMAPINFO* thisBitmapInfoPtr;


    /*
     *  lock down the bitmap info
     */
    thisBitmapInfoPtr = (BITMAPINFO*) 
                        GlobalLock( (HGLOBAL) thisBitmapInfoHdl );

    /* update the image size related items */
    thisBitmapInfoPtr->bmiHeader.biWidth = numCols;

    thisBitmapInfoPtr->bmiHeader.biHeight = 
        ( renderUp ) ?
        numRows :
        numRows * -1;   
    thisBitmapInfoPtr->bmiHeader.biSizeImage = numRows * pitch;

    /* done - release lock on bitmap info */
    GlobalUnlock( (HGLOBAL) thisBitmapInfoHdl );
}





/***********************************************
*   Function Name : CreateColorBitmapInfo
*
*   Parameters :
*       numRows - number of rows in the image
*       numCols - number of columns in the image
*       pitch - the size of each row in byte
*       renderUp - a Boolean indicating the image supplied
*           is bottom up (TRUE) or top down (FALSE).
*
*   Returned Value :
*       A handle to a bitmap info data structure that is appropriate
*   for a color image of the specified size.
*
*   Description :
*       This function constructs a bitmap info
*   data structure that is appropriate for a color
*   image of the specified size. 
*
*   Modifications :
*
************************************************/

HANDLE CreateColorBitmapInfo(LONG numRows,
                             LONG numCols,
                             LONG pitch,
                             BOOL renderUp )
{
    HANDLE   tempBitInfoHdl;
    BITMAPINFO* tempBitInfoPtr;

    /*
     * allocate storage for the bitmapInfo structure
     */
    tempBitInfoHdl = 
            GlobalAlloc(    GHND,
                            sizeof( BITMAPINFO )
                            +  sizeof( RGBQUAD ) );

    if( tempBitInfoHdl == 0 )
    {
        /* not encough memory, clean up and return NULL */
        return NULL;
    }

    /* lock down memory before accessing */
    tempBitInfoPtr = (BITMAPINFO*) 
                    GlobalLock( (HGLOBAL) tempBitInfoHdl );

    /* set bmi Header */
    tempBitInfoPtr->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    tempBitInfoPtr->bmiHeader.biWidth = numCols;
    tempBitInfoPtr->bmiHeader.biHeight = ( renderUp ) ?  numRows : numRows * -1;   
    tempBitInfoPtr->bmiHeader.biPlanes = 1;
    tempBitInfoPtr->bmiHeader.biBitCount = 24;
    tempBitInfoPtr->bmiHeader.biCompression = BI_RGB;
    tempBitInfoPtr->bmiHeader.biSizeImage = 0;
    tempBitInfoPtr->bmiHeader.biXPelsPerMeter = 0;
    tempBitInfoPtr->bmiHeader.biYPelsPerMeter = 0;
    tempBitInfoPtr->bmiHeader.biClrUsed = 0;
    tempBitInfoPtr->bmiHeader.biClrImportant = 0;

    /* all done, unlock access */
    GlobalUnlock( (HGLOBAL) tempBitInfoHdl );

    return tempBitInfoHdl;
}
