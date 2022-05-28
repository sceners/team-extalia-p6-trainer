#pragma once

///////////////////////////////////////////////////////////////////////////
//
// dibEngine.h
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

#include <windows.h>
#include <tchar.h>

typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} PIXELCOLOR;

class dibEngine
{
	public:
		dibEngine				( VOID );
		virtual ~dibEngine		( VOID );

		void	SetHwnd			( HWND hwnd );
		HWND	GetHwnd			( VOID );

		HDC		GetCanvasDC		( VOID ) { return canvashDC; }
		HBITMAP GetCanvasBmp	( VOID ) { return canvasBmp; }
		unsigned char* GetCanvasBuffer( VOID ){ return lpCanvasBuffer; }

		BOOL	InitCanvas		( int,int );
		BOOL	DestroyCanvas	( VOID );
		void	ClearCanvas		( VOID );
		BOOL	UpdateCanvas	( VOID );

		void	PutPixel		( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b );
		void	PutPixelTrans	( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, float a );
		void	PutBitmap		( unsigned char PicArray[], unsigned char Palette[], int DestX, int DestY, int iWidth, int iHeight, COLORREF cRef, bool bFlip );
		void	PutBitmapTrans	( unsigned char PicArray[], unsigned char Palette[], int DestX, int DestY, int iWidth, int iHeight, unsigned char TransColorIndex, bool bFlip, float Transparency );
		void    PutBitmapChars	( unsigned char PicArray[], unsigned char Palette[], int DestX, int DestY, int iWidth, int iHeight, unsigned char TransColorIndex, bool bFlip, float Transparency, int iScroll );

		void	DrawButton		( int x, int y, TCHAR* szText, bool bHover );
		void	DrawButtonEx	( RECT rcPosition, TCHAR* szText, bool bHover, PIXELCOLOR pxBorderColor, PIXELCOLOR pxGradTop, PIXELCOLOR pxGradBottom );

		void	MakeGradient	( unsigned char Palette[], unsigned char StartIndex, unsigned char EndIndex, unsigned char rS, unsigned char gS, unsigned char bS, unsigned char rE, unsigned char gE, unsigned char bE );

		BOOL	FadeToBlack		( VOID );
	
	protected:
		unsigned int	ScreenWidth;
		unsigned int	ScreenHeight;
		unsigned char*	lpCanvasBuffer;

	private:
		HWND			hWnd;
		HFONT			hFont;
		PAINTSTRUCT		ps;
		BITMAPINFO		canvas;
		HBITMAP			canvasBmp;
		HDC				canvashDC;
		HGDIOBJ			hObj;
		COLORREF		cfColor;
};