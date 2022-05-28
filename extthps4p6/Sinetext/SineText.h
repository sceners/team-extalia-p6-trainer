///////////////////////////////////////////////////////////////////////////
//
// SineText.h
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

#include <windows.h>
#include <cmath>
#include "../dibEngine/dibEngine.h"
#define PI 3.14159

class SineText
{
	public:
					SineText		( VOID );
					~SineText		( VOID );

		void		SetupSineText	( dibEngine* dib,int,int,int,char* );
		void		DrawSineText	( VOID );

	protected:
		void		MakeSineTable			( VOID );

		dibEngine*	dibEng;
		int			ScreenWidth;
		int			ScreenHeight;
		int			posOffset;

	private:
		char		txtScroll[360];
		int			SineScroll[360];
};