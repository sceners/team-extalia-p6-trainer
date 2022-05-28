///////////////////////////////////////////////////////////////////////////
//
// Starfield.cpp
// Copyright(c) 2007, Wiccaan
//
///////////////////////////////////////////////////////////////////////////
// An extension for dibEngine written by Wiccaan.
// This extension creates a starfield effect with some
// customizable options. A quick toss-together though.
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

#include "Starfield.h"

Starfield::Starfield()		{ }
Starfield::~Starfield()		{ }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : CreateStarField()
// Parameters : dibEngine* dib, int iWidth,int iHeight
//
// Purpose    : Creates a starfield with the given width and size.
//
// Comments   : dibEngine* dib is a pointer to a predefined dibEngine class.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Starfield::CreateStarField( dibEngine* dib, int iWidth, int iHeight, BOOL bFadeTrail ) 
{
	ScreenWidth		= iWidth;
	ScreenHeight	= iHeight;
	dibEng			= dib;
	bFadeStars		= bFadeTrail;

	for( int i = 0; i < NUM_OF_STARS; i++ ){
		Star[i].x		= rand()%ScreenWidth;
		Star[i].y		= rand()%ScreenHeight;
		Star[i].Plane	= rand()%16;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : MoveStars()
// Parameters : N/A
//
// Purpose    : Repositions the stars on the field.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Starfield::MoveStars() 
{
	for( int i = 0; i < NUM_OF_STARS; i++ ){
		Star[i].x -= (Star[i].Plane >> 1)+1;
		if (Star[i].x < 0){
			Star[i].x = ScreenWidth;
			Star[i].y = rand()%ScreenHeight;
			Star[i].Plane = rand()%16;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : DrawStars()
// Parameters : N/A
//
// Purpose    : Plots the pixel stars to the screen.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Starfield::DrawStars() 
{
	unsigned char cColor = 0;

	for( int i = 0; i < NUM_OF_STARS; i++ ){
		cColor = Star[i].Plane*10+105;
		if( !bFadeStars ){
			for (int j=0;j<3;j++)
				dibEng->PutPixel(Star[i].x+j,Star[i].y,cColor,cColor,cColor);
		}else{
			for (int j=0;j<10;j++)
				dibEng->PutPixelTrans(Star[i].x+j,Star[i].y,cColor,cColor,cColor,1.0f/(j+1));
		}
	}
}  

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function   : UpdateStarField()
// Parameters : N/A
//
// Purpose    : Used to minimize code length of using MoveStars + DrawStars in actual program.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Starfield::UpdateStarField()
{
	MoveStars();
	DrawStars();
}