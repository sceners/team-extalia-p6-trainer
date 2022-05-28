#include "dibEngine.h"

///////////////////////////////////////////////////////////////////////////
//
// dibEngine.cpp
// Copyright(c) 2007, Wiccaan
//
///////////////////////////////////////////////////////////////////////////
// This class is a wrapper around some basic DIB functions to
// allow the user to easily create interesting effects on their
// programs window as a canvas. This wrapper also has some extensions
// that I wrote, and others can be writen with ease.
//
// Special thanks to: Mircal
///////////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////

/* -- Pixel Info --
 * color[0]  Blue
 * color[1]  Green
 * color[2]  Red
 * color[3]  Alpha
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Init / Class DeInit
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
dibEngine::dibEngine()
{ 
	hFont		= NULL;
	hWnd		= NULL;
	canvasBmp	= NULL;
	canvashDC	= NULL;
}

dibEngine::~dibEngine()
{
	hFont		= NULL;
	hWnd		= NULL;
	canvasBmp	= NULL;
	canvashDC	= NULL;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : SetHwnd()
// Parameters : HWND hwnd
//
// Purpose    : Allows the user to send the hwnd of their program to
//              the class to be used with the DIB functions.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dibEngine::SetHwnd( HWND hwnd )
{
	hWnd = hwnd;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : GetHwnd()
// Parameters : N/A
//
// Purpose    : Allows the user to retrieve the hwnd that is currently
//              being used by the class.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HWND dibEngine::GetHwnd()
{
	return hWnd;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : InitCanvas()
// Parameters : int iWidth, int iHeight
//
// Purpose    : Creates the DIB section canvas of the given size.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL dibEngine::InitCanvas( int iWidth, int iHeight )
{
	int result = 0;

	/* Get Dialog Width/Height */
	ScreenWidth		= iWidth;
	ScreenHeight	= iHeight;

	/* Setup Canvas */
	canvas.bmiHeader.biSize		= sizeof(canvas.bmiHeader);
	canvas.bmiHeader.biWidth	= ScreenWidth;
	canvas.bmiHeader.biHeight	= -(signed int(ScreenHeight));
	canvas.bmiHeader.biPlanes	= 1;
	canvas.bmiHeader.biBitCount = 32;

	/* Create DIB Section */
	canvashDC = CreateCompatibleDC( GetWindowDC(hWnd) );
	canvasBmp = CreateDIBSection( canvashDC,&canvas,DIB_RGB_COLORS, (LPVOID*)&lpCanvasBuffer, 0, 0 );
	if( !canvashDC || !canvasBmp )
		return false;

	/* Set The Background As Transparent */
	hObj = SelectObject( canvashDC, canvasBmp );	// Select Object To Edit
	result = SetBkMode( canvashDC, TRANSPARENT );	// Set Background Transparent
	cfColor = SetTextColor( canvashDC, 0xFFFFFF );	// Set Text To White
	if( !hObj || !result || cfColor == CLR_INVALID )
		return false;

	/* Create Font */
	hFont = CreateFont( -12, 1, 0, 0, 0, 0, 0, 0, OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH|FF_MODERN, "Terminal");
	hObj = SelectObject( canvashDC, hFont );
	if( !hFont || !hObj )
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : DestroyCanvas()
// Parameters : N/A
//
// Purpose    : Deletes the DIB canvas.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL dibEngine::DestroyCanvas()
{
	BOOL r1 = DeleteObject(canvasBmp);
	BOOL r2 = DeleteDC(canvashDC);
	BOOL r3 = DeleteObject(hFont);

	if( r1 && r2 && r3 )
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : ClearCanvas()
// Parameters : N/A
//
// Purpose    : Emptiess the canvas buffer.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dibEngine::ClearCanvas()
{
	memset( (void*)lpCanvasBuffer, 25, ScreenWidth*ScreenHeight*4 );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : UpdateCanvas()
// Parameters : N/A
//
// Purpose    : Blits the canvas buffer to the screen.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL dibEngine::UpdateCanvas()
{
	HDC DisplayHDC = BeginPaint(hWnd,&ps);
	BOOL r1 = BitBlt(DisplayHDC,0,0,ScreenWidth,ScreenHeight,canvashDC,0,0,SRCCOPY);
	BOOL r2 = EndPaint(hWnd,&ps);

	if( !DisplayHDC || !r1 || !r2 )
		return false;
	else
		return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : PutPixel()
// Parameters : unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b
//
// Purpose    : Plots a solid colored pixel into the canvas buffer at the given position.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dibEngine::PutPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b )
{
	unsigned int iOffset = 0;
	if( x >= 0 && x < ScreenWidth && y >= 0 && y < ScreenHeight )
	{
		iOffset = ( (y * ScreenWidth) + x );
		lpCanvasBuffer[(iOffset * 4)  ] = b;
		lpCanvasBuffer[(iOffset * 4)+1] = g;
		lpCanvasBuffer[(iOffset * 4)+2] = r;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : PutPixelTrans()
// Parameters : unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, float a
//
// Purpose    : Plots a transparent colored pixel into the canvas buffer at the given position.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dibEngine::PutPixelTrans( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, float a )
{
	unsigned int iOffset;
	if( x >= 0 && x < ScreenWidth && y >= 0 && y < ScreenHeight )
	{
		iOffset = ( y * ScreenWidth + x );
		lpCanvasBuffer[(iOffset * 4)  ]	= (unsigned char)(a*b+(1-a)*lpCanvasBuffer[(iOffset * 4)  ]);
		lpCanvasBuffer[(iOffset * 4)+1] = (unsigned char)(a*g+(1-a)*lpCanvasBuffer[(iOffset * 4)+1]);
		lpCanvasBuffer[(iOffset * 4)+2] = (unsigned char)(a*r+(1-a)*lpCanvasBuffer[(iOffset * 4)+2]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : PutBitmap()
// Parameters : unsigned char PicArray[], unsigned char Palette[], int DestX, int DestY, int iWidth, int iHeight, unsigned char TransColorIndex, bool bFlip
//
// Purpose    : Plots a solid bitmap into the canvas buffer at the given position of the given size.
// Comments   : TransColorIndex is the color index used to show transparent pixels. (0=black,255=white)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dibEngine::PutBitmap( unsigned char PicArray[], unsigned char Palette[], int DestX, int DestY, int iWidth, int iHeight, COLORREF cRef, bool bFlip )
{
	unsigned char color[4];
	unsigned char color_index;

	for( int y=0; y<iHeight; y++ )
		for( int x=0; x<iWidth; x++ )	
		{
			color_index = PicArray[y*iWidth+x];
			memcpy( &color, (char*)(&Palette[color_index*4]), 4 );

			if( color[0] == GetRValue(cRef) && 
				color[1] == GetGValue(cRef) && 
				color[2] == GetBValue(cRef) )
			{ /* Do Nothing :) */ }
			else
			{
				if( !bFlip )
					PutPixel( DestX+x, DestY+y, color[2], color[1], color[0] );
				else
					PutPixel( DestX+(iWidth-x), DestY+y, color[2], color[1], color[0] );
			}
		}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : PutBitmapTrans()
// Parameters : unsigned char PicArray[], unsigned char Palette[], int DestX, int DestY, int iWidth, int iHeight, unsigned char TransColorIndex, bool bFlip
//
// Purpose    : Plots a transparent bitmap into the canvas buffer at the given position of the given size.
// Comments   : TransColorIndex is the color index used to show transparent pixels. (0=black,255=white)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dibEngine::PutBitmapTrans( unsigned char PicArray[], unsigned char Palette[], int DestX, int DestY, int iWidth, int iHeight, unsigned char TransColorIndex, bool bFlip, float Transparency )
{
	unsigned char color[4];
	unsigned char color_index;
	unsigned int screenx,screeny;
	int r,g,b;

	if(Transparency<=0) return;
	if(Transparency>=1)
	{
		PutBitmap(PicArray, Palette,DestX,DestY,iWidth,iHeight,TransColorIndex,bFlip);
		return;
	}
	for( int y=0; y<iHeight; y++ )
	{
		for( int x=0; x<iWidth; x++ )
		{
			if( !bFlip ){
				screenx=DestX+x;
				screeny=DestY+y;
			} else {
				screenx=DestX+iWidth-x;
				screeny=DestY+y;
			}

			color_index = PicArray[y*iWidth+x];
			unsigned char Shit = PicArray[470*480+290];
			
			//if( color_index != TransColorIndex && screenx>=0 && screenx<ScreenWidth && screeny>=0 && screeny<ScreenHeight )
			if( color_index != TransColorIndex )
			{
				memcpy( &color, (char*)(&Palette[color_index*4]), 4 );
				b = (unsigned char)(color[0]*Transparency+(1-Transparency)*lpCanvasBuffer[(screeny*ScreenWidth+screenx)*4]);
				g = (unsigned char)(color[1]*Transparency+(1-Transparency)*lpCanvasBuffer[(screeny*ScreenWidth+screenx)*4+1]);
				r = (unsigned char)(color[2]*Transparency+(1-Transparency)*lpCanvasBuffer[(screeny*ScreenWidth+screenx)*4+2]);
				PutPixel(screenx,screeny,r,g,b);
			}else{
				OutputDebugString("FUCK");
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : PutBitmapChars()
// Parameters : unsigned char PicArray[], unsigned char Palette[], int DestX, int DestY, int iWidth, int iHeight, unsigned char TransColorIndex, bool bFlip, float Transparency, int iScroll
//
// Purpose    : Plots a bitmap font to the canvas buffer.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dibEngine::PutBitmapChars(unsigned char PicArray[], unsigned char Palette[], int DestX, int DestY, int iWidth, int iHeight, unsigned char TransColorIndex, bool bFlip, float Transparency, int iScroll)
{        
	unsigned char color[4];
	unsigned char color_index;
	unsigned int ScreenX, ScreenY;
	int iR, iG, iB;

	if (Transparency<=0) return;

    for (int y=0; y<iHeight; y++) {
        for (int x=0; x<iWidth; x++) {

            if (!bFlip) {
               ScreenX=DestX+x;
               ScreenY=DestY+y;
            } else {
              ScreenX=DestX+iWidth-x;
              ScreenY=DestY+y;
            }

            color_index = PicArray[y*iWidth+x];
            
            if ( color_index!=TransColorIndex
               && ScreenX>=0 && ScreenX<ScreenWidth &&
               ScreenY>=0 && ScreenY<ScreenHeight ) {

				   memcpy(&color,(char*)(&Palette[((ScreenY*5+iScroll)%256)*4]),4);
               
                iB = (unsigned char) (
                   color[0]*Transparency + (1-Transparency)*lpCanvasBuffer[(ScreenY*ScreenWidth+ScreenX)*4]
                ) ;
                iG = (unsigned char) (
                   Transparency*color[1] + (1-Transparency)*lpCanvasBuffer[(ScreenY*ScreenWidth+ScreenX)*4+1]
                ) ;
                iR = (unsigned char) (
                   Transparency*color[2] + (1-Transparency)*lpCanvasBuffer[(ScreenY*ScreenWidth+ScreenX)*4+2]
                ) ;

                PutPixel(ScreenX,ScreenY,iR,iG,iB);
            }    
        }   
    }    
}  

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : DrawButton()
// Parameters : int DestX, int DestY, char* Text, bool bHover
//
// Purpose    : Draws a button into the canvas buffer at the given location with the given text.
// Comments   : bHover is used to tell the function if the mouse pointer is over the button.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dibEngine::DrawButton( int DestX, int DestY, TCHAR* Text, bool bHover )
{
	float fAlpha = 1.0f;

	for( int y=0; y<15; y++ )
		for( int x=0; x<40; x++ )
		{
			if( x==0 || y==0 || x==39 || y==14 )
				PutPixelTrans( DestX+x, DestY+y, 255, 255, 255, fAlpha );
			else
				if(bHover)
					PutPixelTrans( DestX+x, DestY+y, (unsigned char)(255-10*y), 0, 0, fAlpha );
				else
					PutPixelTrans( DestX+x, DestY+y ,0, 0, (unsigned char)(255-10*y), fAlpha );
		}
	TextOut( canvashDC, DestX+5, DestY+2, Text, int(strlen(Text)) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : DrawButtonEx()
// Parameters : RECT rcPosition, TCHAR *szText, bool bHover, PIXELCOLOR pxBorderColor, PIXELCOLOR pxButtonColor, PIXELCOLOR pxButtonHoverColor
//
// Purpose    : Draws a button into the canvas buffer at the given location with the given text. (With Extended Options)
// Comments   : bHover is used to tell the function if the mouse pointer is over the button.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dibEngine::DrawButtonEx(RECT rcPosition, TCHAR *szText, bool bHover, PIXELCOLOR pxBorderColor, PIXELCOLOR pxButtonColor, PIXELCOLOR pxButtonHoverColor)
{
	double dbPercent;
	for( int y=0; y<rcPosition.bottom; y++ )
		for( int x=0; x<rcPosition.right; x++ )
			if( x==0 || x==rcPosition.right-1 || y==0 || y==rcPosition.bottom-1 )
				PutPixel( rcPosition.left+x, rcPosition.top+y, pxBorderColor.r, pxBorderColor.g, pxBorderColor.b );
			else
			{
				dbPercent = 1-(double)y / (double)(rcPosition.bottom);
				if( bHover )
					PutPixelTrans(rcPosition.left+x, rcPosition.top+y, ((unsigned char)(pxButtonHoverColor.r*dbPercent)+(unsigned char)(0*(dbPercent))), ((unsigned char)(pxButtonHoverColor.g*dbPercent)+(unsigned char)(0*(dbPercent))), ((unsigned char)(pxButtonHoverColor.b*dbPercent)+(unsigned char)(0*(dbPercent))), pxButtonHoverColor.a);
				else
					PutPixelTrans(rcPosition.left+x, rcPosition.top+y, ((unsigned char)(pxButtonColor.r*dbPercent)+(unsigned char)(0*(dbPercent))), ((unsigned char)(pxButtonColor.g*dbPercent)+(unsigned char)(0*(dbPercent))), ((unsigned char)(pxButtonColor.b*dbPercent)+(unsigned char)(0*(dbPercent))), pxButtonColor.a);
			}

	SIZE sz;
	GetTextExtentPoint32( canvashDC, szText, (unsigned int)strlen(szText), &sz );
	TextOut( canvashDC, ((rcPosition.left+(rcPosition.left+rcPosition.right))/2)-(sz.cx/2), ((rcPosition.top+(rcPosition.top+rcPosition.bottom))/2)-(sz.cy/2), szText, int(strlen(szText)) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : MakeGradient()
// Parameters : unsigned char Palette[], unsigned char StartIndex, unsigned char EndIndex, unsigned char rS, unsigned char gS, unsigned char bS, unsigned char rE, unsigned char gE, unsigned char bE
//
// Purpose    : Dumps a color range into a palette.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dibEngine::MakeGradient( unsigned char Palette[], unsigned char StartIndex, unsigned char EndIndex, unsigned char rS, unsigned char gS, unsigned char bS, unsigned char rE, unsigned char gE, unsigned char bE )
{
	unsigned char iIndex;
	unsigned char iMax = (EndIndex - StartIndex);

	float rInc, gInc, bInc;

	/* Fill Start Of Palette */
	Palette[StartIndex*4+0] = bS;
	Palette[StartIndex*4+1] = gS;
	Palette[StartIndex*4+2] = rS;
	Palette[StartIndex*4+3] = 0;

	/* Fill End Of Palette */
	Palette[EndIndex*4+0] = bE;
	Palette[EndIndex*4+1] = gE;
	Palette[EndIndex*4+2] = rE;
	Palette[EndIndex*4+3] = 0;

	/* Compute RGB Increments */
	rInc = (rE-rS) / ((float)(iMax));
	gInc = (gE-gS) / ((float)(iMax));
	bInc = (bE-bS) / ((float)(iMax));

	/* Fill In Gradiant Colors */
	for( iIndex = 1; iIndex<iMax; iIndex++ )
	{
		Palette[(StartIndex+iIndex)*4+0] = (int)(bS+bInc*iIndex);
		Palette[(StartIndex+iIndex)*4+1] = (int)(gS+gInc*iIndex);
		Palette[(StartIndex+iIndex)*4+2] = (int)(rS+rInc*iIndex);
		Palette[(StartIndex+iIndex)*4+3] = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : FadeToBlack()
// Parameters : None
//
// Purpose    : Fade the current canvas to black. (Best used in a stepping loop.)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL dibEngine::FadeToBlack()
{
	BOOL bNeedToLoop = FALSE;
	for( unsigned int i=0; i<ScreenWidth*ScreenHeight*4; i++ )
		if( lpCanvasBuffer[i] > 0 ){
			bNeedToLoop = TRUE;
			lpCanvasBuffer[i]--;
		}
	return bNeedToLoop;
}