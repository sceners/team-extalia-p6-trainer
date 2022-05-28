///////////////////////////////////////////////////////////////////////////
//
// SineText.cpp
// Copyright(c) 2007, Wiccaan
//
///////////////////////////////////////////////////////////////////////////
// An extension wrapper for dibEngine written by Wiccaan.
// This extension creates a snake-like text effect at
// the given location.
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

#include "SineText.h"

SineText::SineText()	{ }
SineText::~SineText()	{ }

void SineText::SetupSineText( dibEngine* dib, int iWidth,int iHeight, int iPlacementY, char* TextToScroll )
{
	dibEng			= dib;
	ScreenWidth		= iWidth;
	ScreenHeight	= iHeight;
	posOffset		= iPlacementY;

	strcpy_s( txtScroll, ARRAYSIZE(txtScroll), TextToScroll );

	MakeSineTable();
}

void SineText::DrawSineText()
{
	static int ScreenX	= ScreenWidth, iScroll1 = 359, iScroll2 = 0;
	int i, pos;
	SIZE charsize;

	ScreenX--;
	iScroll1--; if( iScroll1 <   0 ) iScroll1 = 359;
	iScroll2++; if( iScroll2 > 255 ) iScroll2 = 0;
	TextOut( dibEng->GetCanvasDC(), ScreenX, (int)SineScroll[(ScreenX+iScroll1)%360]+posOffset, &txtScroll[0], 1 );

	pos = ScreenX;
	for( i=1; i<signed(strlen(txtScroll)); i++ ){
		GetTextExtentPoint32( dibEng->GetCanvasDC(), &txtScroll[i], 1, &charsize );
		pos += charsize.cx;
		TextOut( dibEng->GetCanvasDC(), pos, (int)SineScroll[(pos+iScroll1)%360]+posOffset, &txtScroll[i], 1 );
	}
	if( pos<0 ) ScreenX = ScreenWidth;
}

void SineText::MakeSineTable()
{
	for( int i=0; i<360; i++ ){
		SineScroll[i] = int ( 8 * (sin((float)(PI/180*i)) + sin((float)(PI/180*i*3))) );
	}
}
