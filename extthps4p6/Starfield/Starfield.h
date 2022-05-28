///////////////////////////////////////////////////////////////////////////
//
// Starfield.h
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

#pragma once

#include <windows.h>
#include "../dibEngine/dibEngine.h"

#define NUM_OF_STARS 75

struct STAR{
    int x, y;
    char Plane;
};

class Starfield
{
	public:
		Starfield				( VOID );
		~Starfield				( VOID );

		void CreateStarField	( dibEngine*,int,int,BOOL );
		void UpdateStarField	( VOID );

	protected:
		void MoveStars			( VOID );
		void DrawStars			( VOID );

		dibEngine*		dibEng;
		unsigned int	ScreenWidth;
		unsigned int	ScreenHeight;
		BOOL			bFadeStars;

	private:
		STAR			Star[NUM_OF_STARS];
};