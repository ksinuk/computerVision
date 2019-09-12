//=============================================================================
// Copyright © 1999-2000 Point Grey Research, Inc. All Rights Reserved.
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
//
// drawimage.h
//
//=============================================================================

//=============================================================================
// Includes
//=============================================================================
#include "stdafx.h"

//=============================================================================
// Function Declarations
//=============================================================================

//=============================================================================
// Function Name : DestroyBitmapInfo
//
// Parameters :
//     oldBitmapInfo - handle to the bitmap info to be destroyed.
//
// Returned Value :
//       none.
//
// Description :
//       This function destoys a bitmap info data structure that has been 
// previously created by either CreateGreyscaleBitmapInfo or 
// CreateCompositeBitmapInfo. 
//=============================================================================
void DestroyBitmapInfo( HANDLE oldBitmapInfo );

//=============================================================================
//
// Function Name : CreateGreyscaleBitmapInfo
// Parameters :
//      numRows - number of rows in the image
//      numCols - number of columns in the image
//      pitch - the size of each row in byte
//      renderUp - a Boolean indicating the image supplied
//          is bottom up (TRUE) or top down (FALSE).
//
//  Returned Value :
//      A handle to a bitmap info data structure that is appropriate
//  for a 256 greyscale image of the specified size.
//
//  Description :
//      This function constructs a bitmap info data structure that is 
//  appropriate for a 256 greyscale image of the specified size. 
//
//=============================================================================
HANDLE CreateGreyscaleBitmapInfo(LONG numRows,
                                 LONG numCols,
                                 LONG pitch,
                                 BOOL renderUp );

//=============================================================================
//  Function Name : CreateCompositeBitmapInfo
//
//  Parameters :
//      numRows - number of rows in the image
//      numCols - number of columns in the image
//      pitch - the size of each row in byte
//      renderUp - a Boolean indicating the image supplied
//          is bottom up (TRUE) or top down (FALSE).
//
//  Returned Value :
//      A handle to a bitmap info data structure that is appropriate
//  for a composite image of the specified size.  A composite image
//  is one supports 128 greyscales and 10 color description.
//  The pixel value in this case would range from 0 to 127 for greyscale
//  and from 128 beyond will specify some special built-in color that
//  can be used for overlay purposes.
//
//  Description :
//      This function constructs a bitmap info
//  data structure that is appropriate for a composite
//  image of the specified size. 
//=============================================================================
/* 128 greyscale + 6 colors */
HANDLE CreateCompositeBitmapInfo( LONG numRows,
                                  LONG numCols,
                                  LONG pitch,
                                  BOOL renderUp );


//=============================================================================
//  Function Name : PaintImage
//
//  Parameters :
//      pDisplay - handle to a device context within
//          which the image is rendered.
//      pDispRect - pointer to a rect describing the
//          portion of the device context to refresh.
//      pImgRect - pointer to a rect describing the
//          portion of the image to be re-drawn.
//      pImgPalette - a HPALLETTE object containing
//          the palette to be used in rendering the
//          image.
//
//  Returned Value :
//      A boolean variable indicating whether the 
//  rendering was successful.
//
//  Description :
//      This function renders a raw image contents to
//  a Win32 device context.
//
//=============================================================================
BOOL PaintImage( HDC        pDisplay,
                 LPRECT     pDispRect,
                 LPRECT     pImgRect,
                 HANDLE     thisBitmapInfoHdl,
                 void*      bitmapBits );

//=============================================================================
//  Function Name : ResizeBitmapInfo
//
//  Parameters :
//      thisBitmapInfoHdl - handle the bitmap info 
//          structure to be updated.
//      numRows - number of rows in the image
//      numCols - number of columns in the image
//      pitch - the size of each row in byte
//      renderUp - a Boolean indicating the image supplied
//          is bottom up (TRUE) or top down (FALSE).
//
//  Returned Value :
//      none.
//
//  Description :
//      This function update the bitmap info to reflect
//  changes in image dimension.
//=============================================================================
void ResizeBitmapInfo( HANDLE thisBitmapInfoHdl,
                       long numRows,
                       long numCols,
                       long pitch,
                       BOOL renderUp );






HANDLE CreateColorBitmapInfo( LONG numRows,
                              LONG numCols,
                              LONG pitch,
                              BOOL renderUp );
