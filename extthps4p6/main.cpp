///////////////////////////////////////////////////////////////////////////
//
// main.cpp
// Copyright(c) 2007, Wiccaan
//
///////////////////////////////////////////////////////////////////////////
// Main program source file. This file handles the window
// creation, DIB effects, etc. This is the heart of the prog.
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

#pragma 

#include <windows.h>
#include <cmath>
#include <stdio.h>
#include <tchar.h>
#include "resource.h"

// == HOTKEY EXTERN ============================================
extern void CheckHotkeys(); // In Trainer.cpp
// =============================================================

// == DIB IMAGES ===============================================
#include "ExtaliaSplash.h"
#include "ExtaliaHeader.h"
#include "NFOText.h"
// =============================================================

// == DIB ENGINE + EXTENSIONS ==================================
#include "dibEngine/dibEngine.h"
#include "Starfield/Starfield.h"
#include "SineText/SineText.h"

dibEngine	dibE;
Starfield	star;
SineText	sine;
// =============================================================

// == miniFMod ENGINE ==========================================
#pragma comment(lib,"winmm.lib")
#include "miniFMod/miniF.h"

miniF fMod;
bool bIsMusicPaused = false;
// =============================================================

// == WINDOW INFORMATION =======================================
#define ScreenWidth		480
#define ScreenHeight	300

TCHAR tszWindowName[]	= _T(" .: tONY hAWKS pRO sKATER 4 :. ");
TCHAR tszClassName[]	= _T("clsExtEng_THPS4");
// =============================================================

// == TRAINER INFO =============================================
RECT rectInfo = {20,150,ScreenWidth-rectInfo.left,250};
TCHAR tszTrainerInfo[] = _T(
	"tONY hAWKS pRO sKATER 4 +6 tRAINER\n\r" \
	"tRainer........................Wiccaan\n\r"\
	"cOding.........................Wiccaan\n\r"\
	"gFx............................Wiccaan\n\r"\
	"tUnez............................Radix\n\r"
);
// =============================================================

// == FUNCTION CALLBACKS =======================================
LRESULT CALLBACK WinProc	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
void OnCreate				( HWND );
void OnDestroy				( HWND );
void OnPaint				( HWND );
void OnTimer				( HWND, WPARAM, LPARAM );
void OnMouseMove			( HWND, WPARAM, LPARAM );
void OnLMBDown				( HWND, WPARAM, LPARAM );
bool IsOverButton			( RECT rectButton, LPARAM lParam );
void SceneManager			( HWND, WPARAM, LPARAM );
// =============================================================

// == BUTTON INFO ==============================================
RECT rectBt_Nfo		= { 435, 10, 40, 15 };
RECT rectBt_Close	= { 435, 30, 40, 15 };
RECT rectBt_Back	= { 435, 10, 40, 15 };
RECT rectNfoArea	= { 0, 0, ScreenWidth, ScreenHeight };


bool bIsOverNfo		= false;
bool bIsOverClose	= false;
bool bIsOverBack	= false;
// =============================================================

// == SCENE VARIABLES ==========================================
BOOL bIsQuiting		= FALSE;
BOOL bIsInNfo		= FALSE;
// =============================================================

// == NFO SCENE VARIABLES ======================================
unsigned char lpNfoPallet[1024];
void MakeGradientPal();
void CreateColorOverlay();
// =============================================================

// == SINETEXT STRING ==========================================
TCHAR tszSineText[] = _T(
	"Greetz Fly Out To :: Extalia Team, Micral, SunBeam, Labyrnth, Elissaios, Raven, ILA, Deranged, Elzevir" \
	" -- Tony Hawks Pro Skater 4 +6 Trainer By Wiccaan -- Coded in C -- Team Extalia -- www.extalia.com"
);
// =============================================================





// =============================================================
//                     MAIN PROGRAM ENTRY
// @Function: WinMain(...)
// @Purpose : Creates the window for our process.
// =============================================================
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	WNDCLASSEX	wc		= {0};
	MSG			uMsg	= {0};
	HWND		hWnd	= NULL;

	wc.hInstance		= hInstance;
	wc.lpszClassName	= tszClassName;
	wc.lpfnWndProc		= WinProc;
	wc.style			= CS_DBLCLKS;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.hbrBackground	= (HBRUSH)COLOR_WINDOW;
	wc.hIcon			= LoadIcon( hInstance, NULL );
	wc.hIconSm			= LoadIcon( hInstance, NULL );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.cbClsExtra		= NULL;
	wc.cbWndExtra		= NULL;
	wc.lpszMenuName		= NULL;

	// Register The Window Class
	if( !RegisterClassEx( &wc ) )
		return 0;

	// Create The Window
	hWnd = CreateWindowEx( 0, tszClassName, tszWindowName, WS_POPUP, (GetSystemMetrics(SM_CXSCREEN)-ScreenWidth)/2, (GetSystemMetrics(SM_CYSCREEN)-ScreenHeight)/2, ScreenWidth, ScreenHeight, (HWND)HWND_DESKTOP, (HMENU)NULL, hInstance, (LPVOID)NULL );
	if( !hWnd )
		return 0;

	// Show The Window
	ShowWindow( hWnd, nShowCmd );

	// Handle Any Messages
	while( GetMessage( &uMsg, 0, 0, 0 ) )
	{
		TranslateMessage( &uMsg );
		DispatchMessage( &uMsg );
	}

	// When No More Messages Unregister Our Window And Return
	UnregisterClass( tszClassName, 0 );
	return 0;
}

// =============================================================
//                       MAIN MESSAGE LOOP
// @Function: WinProc(...)
// @Purpose : Handles messages passed in and out of our window.
// =============================================================
LRESULT CALLBACK WinProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_CREATE:
		OnCreate( hWnd );
		break;
	case WM_DESTROY:
		OnDestroy( hWnd );
		break;
	case WM_ACTIVATE:
		{
			if( wParam == WA_INACTIVE ){
				CloseWindow( hWnd );
				bIsMusicPaused = true;
				fMod.Stop();
			}
			if( wParam == WA_ACTIVE ){
				if( bIsMusicPaused == true ){
					bIsMusicPaused = false;
					fMod.Play();
				}
			}
		}
		break;
	case WM_PAINT:
		OnPaint( hWnd );
		break;
	case WM_TIMER:
		OnTimer( hWnd, wParam, lParam );
		break;
	case WM_KEYDOWN:
		if( bIsInNfo )
			if( (wParam == VK_UP) && (rectNfoArea.top < 0) )
				rectNfoArea.top += 5;
			if( (wParam == VK_DOWN) && (rectNfoArea.top > -1180) )
				rectNfoArea.top -= 5;
			if( (wParam == VK_HOME) )
				rectNfoArea.top = 0;
			if( (wParam == VK_END) )
				rectNfoArea.top = -1180;
		break;
	case WM_MOUSEMOVE:
		OnMouseMove( hWnd, wParam, lParam );
		break;
	case WM_LBUTTONDOWN:
		OnLMBDown( hWnd, wParam, lParam );
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}




// =============================================================
//
//              MESSAGE LOOP EXTENDED CALLBACKS
//
// =============================================================

//================================================================================
// @Function: OnCreate(...)
// @Purpose : Extended version of WM_CREATE command to make code easier to read.
//================================================================================
void OnCreate( HWND hWnd )
{
	// Setup dibEngine And Canvas
	dibE.SetHwnd( hWnd );
	if( !dibE.InitCanvas( ScreenWidth, ScreenHeight ) )
		PostQuitMessage( 0 );

	// Create Main DIB Timer
	SetTimer( hWnd, 1, 10, 0 );

	// Init The Starfield and Sinetext
	star.CreateStarField( &dibE, ScreenWidth, ScreenHeight, TRUE );
	sine.SetupSineText( &dibE, ScreenWidth, 100, ScreenHeight-25, tszSineText );

	// Load The XM File For Tunez
	fMod.LoadMusic( IDR_MODFILE1 );

	// Init The Gradient Pallet For Nfo Text And SineText
	MakeGradientPal();
}

//================================================================================
// @Function: OnDestroy(...)
// @Purpose : Extended version of WM_DESTROY command to make code easier to read.
//================================================================================
void OnDestroy( HWND hWnd )
{
	// Kill Our Timers
	KillTimer( hWnd, 1 );
	KillTimer( hWnd, 2 );

	// Stop The Music
	fMod.Stop();

	// Tell The Program We Are Ready To Exit
	PostQuitMessage( 0 );
}

//================================================================================
// @Function: OnPaint(...)
// @Purpose : Extended version of WM_PAINT command to make code easier to read.
//================================================================================
void OnPaint( HWND hWnd )
{
	// Attempt To Update Our Window Canvas
	if( !dibE.UpdateCanvas() )
		PostQuitMessage( 0 );
}

//================================================================================
// @Function: OnTimer(...)
// @Purpose : Extended version of WM_TIMER command to make code easier to read.
//================================================================================
void OnTimer( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	//
	// If Not Quiting, Update Canvas And Starfield
	// These are placed here because we want these to always
	// happen no matter what scene is occuring.
	//
	if( !bIsQuiting )
	{
		dibE.ClearCanvas();
		star.UpdateStarField();
	}

	// Check Our Hotkeys For Trainer Actions
	CheckHotkeys();

	// Send Message Params To Our SceneManager
	SceneManager( hWnd, wParam, lParam );

	// Redraw Our Window
	RedrawWindow( hWnd, 0, 0, RDW_INVALIDATE );
}

//================================================================================
// @Function: OnCreate(...)
// @Purpose : Extended version of WM_MOUSEMOVE command to make code easier to read.
//================================================================================
void OnMouseMove( HWND, WPARAM wParam, LPARAM lParam )
{
	//
	// Check if the mouse is over any of the
	// buttons located on our window. If so,
	// set the boolean to true.
	//
	bIsOverNfo = IsOverButton( rectBt_Nfo, lParam );
	bIsOverClose = IsOverButton( rectBt_Close, lParam );
	
	//
	// Back button is only displayed while in the
	// nfo screen. Just like the others, detects if
	// the mouse is over the button and sets the boolean.
	//
	if( bIsInNfo )
		bIsOverBack = IsOverButton( rectBt_Back, lParam );
}

//================================================================================
// @Function: OnLMBDown(...)
// @Purpose : Extended version of WM_LBUTTONDOWN command to make code easier to read.
//================================================================================
void OnLMBDown( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	//
	// If we have clicked the mouse and are over
	// the Close button, set our quit boolean to true
	// and create the timer to fade the window and quit.
	//
	if( bIsOverClose == true ){
		bIsQuiting = TRUE;
		SetTimer( hWnd, 2, 3500, 0 );
		return;
	}

	//
	// If we are in the nfo scene and are over the
	// back button, set the window to go back to the
	// main scene and out of the nfo scene.
	//
	if( bIsOverBack ){
		bIsOverBack = false;
		bIsInNfo = FALSE;
		return;
	}	

	//
	// If we are in the main scene and are over the
	// nfo button, set the window to the nfo scene.
	//
	if( bIsOverNfo ){
		bIsInNfo = TRUE;
		return;
	}

	// If nothing handled the message, drag the window on click.
	SendMessage( hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0 );
}




// =============================================================
// @Function: IsOverButton(...)
// @Purpose : Detects the mouse position on the window
//            and returns if over the given rect.
// =============================================================
bool IsOverButton( RECT rectButton, LPARAM lParam )
{
	// Obtain coords of mouse pointer.
	LONG MouseX = (LOWORD(lParam));
	LONG MouseY = (HIWORD(lParam));

	// Check the coords against the given rect.
	if( (rectButton.left <= MouseX) && ((rectButton.right+rectButton.left) >= MouseX) )
		if( (rectButton.top <= MouseY) && ((rectButton.bottom+rectButton.top) >= MouseY) )
			return true; // Mouse is over given rect!
	return false; // Mouse is not over given rect!
}




// =============================================================
//
//                     NFO SCENE FUNCTIONS
//
// =============================================================

//================================================================================
// @Function: MakeGradientPal(...)
// @Purpose : Creates a gradient in the given pallet at the given position.
//================================================================================
void MakeGradientPal()
{
	dibE.MakeGradient( lpNfoPallet,   0,  25,    0, 0xe4, 0xff,    0,    0, 0xff );
	dibE.MakeGradient( lpNfoPallet,  25,  75,    0,    0, 0xff, 0xff,    0,    0 );
	dibE.MakeGradient( lpNfoPallet,  75, 125, 0xff,    0,    0, 0xff, 0xb4,    0 );
	dibE.MakeGradient( lpNfoPallet, 125, 175, 0xff, 0xb4,    0, 0xff, 0xff,    0 );
	dibE.MakeGradient( lpNfoPallet, 175, 225, 0xff, 0xff,    0,    0, 0xff,    0 );
	dibE.MakeGradient( lpNfoPallet, 225, 255,    0, 0xff,    0,    0, 0xe4, 0xff );
}

//================================================================================
// @Function: CreateColorOverlay(...)
// @Purpose : Creates a color overlay with the lpNfoPallet.
//================================================================================
void CreateColorOverlay()
{
	DWORD		dwColor			= NULL;
	int			iColorIndex		= NULL;
	static int	iColorScroll	= NULL;

	for( int y=0; y<ScreenHeight; y++ )
		for( int x=0; x<ScreenWidth; x++ )
		{
			memcpy( &dwColor, dibE.GetCanvasBuffer()+(y*ScreenWidth+x)*4, 4 );
			if( dwColor == 0x00FFFFFF ){
				iColorIndex = (((y+x+iColorScroll)%256)*4);
				dibE.PutPixel( x, y, lpNfoPallet[iColorIndex+2], lpNfoPallet[iColorIndex+1], lpNfoPallet[iColorIndex] );
			}
		}
	iColorScroll++;
	if( iColorScroll >= 255 )
		iColorScroll = 0;
}

//================================================================================
// @Function: DrawBottomBox(...)
// @Purpose : Creates a solid box at the bottom of the nfo window which
//            is used to display how to scroll the nfo file.
//================================================================================
void DrawBottomBox()
{
	for(int y=(ScreenHeight-25);y<ScreenHeight;y++){
		for(int x=0;x<ScreenWidth;x++){
			dibE.PutPixel(x,y,25,25,25);
		}
	}
}

//================================================================================
// @Function: DrawNfoFile(...)
// @Purpose : Called inside the scene manager when the user has selected
//            to view the nfo file.
//================================================================================
void DrawNfoFile()
{
	TCHAR tszScrollNfo[] = _T("Up/Down Arrows To Scroll -- Home/End To Skip");
	SIZE szSize = {0};
	GetTextExtentPoint32(dibE.GetCanvasDC(),tszScrollNfo,(unsigned int)_tcslen(tszScrollNfo),&szSize);
	RECT rectScrollInfo = {((ScreenWidth/2)-(szSize.cx/2)),ScreenHeight-15,ScreenWidth,ScreenHeight};
	DrawText(dibE.GetCanvasDC(),(const char*)&nfo,sizeof(nfo)-1,&rectNfoArea,DT_NOCLIP);
	DrawBottomBox();
	CreateColorOverlay();
	DrawText(dibE.GetCanvasDC(),tszScrollNfo,sizeof(tszScrollNfo)-1,&rectScrollInfo,DT_NOCLIP);
}





// =============================================================
//
//                         SCENE MANAGER
//  Main rendering function for our DIB effects.
// =============================================================
void SceneManager( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	static float	fAlpha			= 1.0f;	// Used To Fade Splash
	static int		introState		= 0;	// Handles Switch Case Position
	static int		iCounter		= 0;	// Timer For Splash Screen

	PIXELCOLOR		btnColor_Border	= { 255, 255, 255, 001 };
	PIXELCOLOR		btnColor_Inner	= { 175, 255, 000, 001 };
	PIXELCOLOR		btnColor_Hover	= { 175, 255, 255, 001 };
	COLORREF		PixelColor		= RGB( 255, 000, 255 );

// == HANDLE QUITTING TIMER ==================================================================================
	if( wParam == 2 ){
		PostQuitMessage( 0 );
		return;
	}
// ===========================================================================================================
// == HANDLE FADE EFFECT =====================================================================================
	if( bIsQuiting ){
		dibE.FadeToBlack();
		return;
	}
// ===========================================================================================================
// == MAIN TRAINER DRAWING ===================================================================================
	if( introState >= 1 )
	{
		if( bIsInNfo ){
			DrawNfoFile();
			dibE.DrawButtonEx( rectBt_Back, "Back", bIsOverBack, btnColor_Border, btnColor_Inner, btnColor_Hover );
			return;
		}else{
			sine.DrawSineText();
			DrawText( dibE.GetCanvasDC(), tszTrainerInfo, sizeof(tszTrainerInfo)-1, &rectInfo, DT_CENTER|DT_NOCLIP );
			CreateColorOverlay();
			dibE.PutBitmap( Header, Header_Pallet, 0, 0, 480, 145, PixelColor, false );
			dibE.DrawButtonEx( rectBt_Nfo, "Nfo", bIsOverNfo, btnColor_Border, btnColor_Inner, btnColor_Hover );
			dibE.DrawButtonEx( rectBt_Close, "Close", bIsOverClose, btnColor_Border, btnColor_Inner, btnColor_Hover );
		}
	}
// ===========================================================================================================
// == HANDLE MAIN TIMER ======================================================================================
	if( wParam == 1 )
	{
		switch( introState )
		{
		case 0: // == Splash Screen ==========================================================================
			dibE.PutBitmap( Splash, Splash_Pallet, 0, 0, ScreenWidth, ScreenHeight, -1, false );
			iCounter++;
			if( iCounter > 100 ){ iCounter = 0; introState++; }
			return;
		case 1: // == Fade Splash Screen =====================================================================
			fAlpha -= 0.05f;
			dibE.PutBitmapTrans( Splash, Splash_Pallet, 0, 0, ScreenWidth, ScreenHeight, 0, false, fAlpha );
			if( fAlpha <= 0 ){ fAlpha = 0; introState++; fMod.Play(); }
			return;
		}
	}
}

// Fin!