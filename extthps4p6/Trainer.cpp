///////////////////////////////////////////////////////////////////////////
//
// Trainer.cpp
// Copyright(c) 2007, Wiccaan
//
///////////////////////////////////////////////////////////////////////////
// Main trainer code to handle the cheats. This file uses
// the cMemory engine also written by Wiccaan to handle
// memory manipulation.
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
#include "resource.h"
#include "MemEngine/cMemory.h"
cMemory Mem;

// == SCORE 999,999,999 =============================================
BOOL	bScore999		= FALSE;
DWORD	dwScoreCave		= NULL;
BYTE	btScoreCave[]	= {0xC7,0x46,0x04,0xFF,0xC9,0x9A,0x3B,0x8B,0x3D,0x48,0x5B,0xAB,0x00};
BYTE	btScoreOrig[]	= {0x89,0x46,0x04,0x8B,0x3D,0x48,0x5B,0xAB,0x00};
// ==================================================================

// == MONEY 999,999,999 =============================================
BOOL	bMoney999		= FALSE;
DWORD	dwMoneyCave		= NULL;
BYTE	btMoneyCave[]	= {0xC7,0x46,0x24,0xFF,0xC9,0x9A,0x3B,0x8B,0x76,0x24,0x8B,0x44,0x24,0x0C};
BYTE	btMoneyOrig[]	= {0x8B,0x76,0x24,0x8B,0x44,0x24,0x0C};
// ==================================================================

// == UNLIMITED TIME ================================================
BOOL	bUnlimitTime	= FALSE;
BYTE	btTimeOrig[]	= {0x01,0x46,0x40};
// ==================================================================

// == PRO POINTS 190 ================================================
BOOL	bProPoints		= FALSE;
DWORD	dwPointCave		= NULL;
BYTE	btPointCave[]	= {0x89,0x57,0x1C,0xB9,0xBE,0x00,0x00,0x00,0x89,0x4F,0x20};
BYTE	btPointOrig[]	= {0x89,0x57,0x1C,0x89,0x4F,0x20};
// ==================================================================

// == PERFECT BALANCE ===============================================
BOOL	bPerfectBalance	= FALSE;
DWORD	dwBalanceStore	= NULL; // Fake Stack Pop
DWORD	dwBalanceCave1	= NULL;
DWORD	dwBalanceCave2	= NULL;
BYTE	btBalanceCave1[] = {0x81,0xC1,0x80,0x07,0x00,0x00,0xD9,0x1D,0x00,0x00,0x00,0x00};
BYTE	btBalanceCave2[] = {0xD9,0x1D,0x00,0x00,0x00,0x00,0xC7,0x46,0x24,0x00,0x00,0x00,0x00,0xD9,0x44,0x24,0x14};
BYTE	btBalanceOrig1[] = {0x81,0xC1,0x80,0x07,0x00,0x00,0xD9,0x5E,0x24};
BYTE	btBalanceOrig2[] = {0xD9,0x5E,0x24,0xD9,0x44,0x24,0x14};
// ==================================================================

// == ALWAYS SPECIAL ================================================
BOOL	bAlwaysSpecial	= FALSE;
DWORD	dwSpecialCave1	= NULL;
DWORD	dwSpecialCave2	= NULL;
BYTE	btSpecialCave1[] = {0x39,0xD8,0xC7,0x46,0x5C,0xB8,0x0B,0x00,0x00};
BYTE	btSpecialCave2[] = {0xC7,0x46,0x5C,0xB8,0x0B,0x00,0x00};
BYTE	btSpecialOrig1[] = {0x39,0xD8,0x89,0x46,0x5C};
BYTE	btSpecialOrig2[] = {0xC7,0x46,0x5C,0x00,0x00,0x00,0x00};
BYTE	btForceSpecialE[] = {0xC6,0x46,0x60,0x01};
BYTE	btFixFallReset[] = {0x89,0x6E,0x5C};
BYTE	btFixObjReset1[] = {0x89,0x5D,0x5C,0xC6,0x45,0x60,0x00};
BYTE	btFixObjReset2[] = {0x89,0x46,0x5C};
BYTE	btForceSpecialD[] = {0xC6,0x46,0x60,0x00};
// ==================================================================



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//================================================================================
// @Function: CheatBeep(...)
// @Purpose : Tells the application to send a beep message to the system
//            based on the param bActivate.
//================================================================================
void CheatBeep( BOOL bActivate )
{
	if( bActivate ){
		Beep( 750, 50 );
		Beep( 1000, 50 );
	}else{
		Beep( 300, 50 );
		Beep( 150, 50 );
	}
}

//================================================================================
// @Function: CheckIfAttached(...)
// @Purpose : Determins if the given proc name is valid, in memory,
//            and that we were able to attach to it.
//================================================================================
BOOL CheckIfAttached()
{
	if( Mem.OpenProc( "Skate4.exe" ) == S_OK )
		return TRUE;

	MessageBeep( 0x10 );
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void HackScore()
{
	if( !CheckIfAttached() )
		return;

	if( !bScore999 ){
		dwScoreCave = Mem.MakeJump( (BYTE*)0x4F6ECC, 4, &btScoreCave, sizeof(btScoreCave) );
		CheatBeep( 1 );
		bScore999 = !bScore999;
	}else{
		Mem.WriteMem( (BYTE*)0x4F6ECC, &btScoreOrig, sizeof(btScoreOrig) );
		Mem.DeleteCave( (BYTE*)dwScoreCave, sizeof(btScoreCave)+5 );
		CheatBeep( 0 );
		bScore999 = !bScore999;
	}
}

void HackMoney()
{
	if( !CheckIfAttached() )
		return;

	if( !bMoney999 ){
		dwMoneyCave = Mem.MakeJump( (BYTE*)0x4EF9B5, 2, &btMoneyCave, sizeof(btMoneyCave) );
		CheatBeep( 1 );
		bMoney999 = !bMoney999;
	}else{
		Mem.WriteMem( (BYTE*)0x4EF9B5, &btMoneyOrig, sizeof(btMoneyOrig) );
		Mem.DeleteCave( (BYTE*)dwMoneyCave, sizeof(btMoneyCave)+5 );
		CheatBeep( 0 );
		bMoney999 = !bMoney999;
	}
}

void HackTime()
{
	if( !CheckIfAttached() )
		return;

	if( !bUnlimitTime ){
		Mem.WriteNops( (BYTE*)0x4E6A13, 3 );
		CheatBeep( 1 );
		bUnlimitTime = !bUnlimitTime;
	}else{
		Mem.WriteMem( (BYTE*)0x4E6A13, &btTimeOrig, sizeof(btTimeOrig) );
		CheatBeep( 0 );
		bUnlimitTime = !bUnlimitTime;
	}
}

void HackProPoints()
{
	if( !CheckIfAttached() )
		return;

	if( !bProPoints ){
		dwPointCave = Mem.MakeJump( (BYTE*)0x4E9B70, 1, &btPointCave, sizeof(btPointCave) );
		CheatBeep( 1 );
		bProPoints = !bProPoints;
	}else{
		Mem.WriteMem( (BYTE*)0x4E9B70, &btPointOrig, sizeof(btPointOrig) );
		Mem.DeleteCave( (BYTE*)dwPointCave, sizeof(btPointCave)+5 );
		CheatBeep( 0 );
		bProPoints = !bProPoints;
	}
}

void HackBalance()
{
	if( !CheckIfAttached() )
		return;

	if( !bPerfectBalance ){
		dwBalanceStore = Mem.MakeCave( NULL, 4 );
		dwBalanceCave1 = Mem.MakeJump( (BYTE*)0x49CD37, 4, &btBalanceCave1, sizeof(btBalanceCave1) );
		dwBalanceCave2 = Mem.MakeJump( (BYTE*)0x49CD1B, 2, &btBalanceCave2, sizeof(btBalanceCave2) );
		DWORD* dwNewAddr = &dwBalanceStore;
		BYTE* bpStack1Start = (BYTE*)((char*)dwBalanceCave1);
		Mem.WriteMem( (bpStack1Start+8), dwNewAddr, 4 );
		BYTE* bpStack2Start = (BYTE*)((char*)dwBalanceCave2);
		Mem.WriteMem( (bpStack2Start+2), dwNewAddr, 4 );
		CheatBeep( 1 );
		bPerfectBalance = !bPerfectBalance;
	}else{
		Mem.WriteMem( (BYTE*)0x49CD37, &btBalanceOrig1, sizeof(btBalanceOrig1) );
		Mem.WriteMem( (BYTE*)0x49CD1B, &btBalanceOrig2, sizeof(btBalanceOrig2) );
		Mem.DeleteCave( (BYTE*)dwBalanceCave1, sizeof(btBalanceCave1)+5 );
		Mem.DeleteCave( (BYTE*)dwBalanceCave2, sizeof(btBalanceCave2)+5 );
		Mem.DeleteCave( (BYTE*)dwBalanceStore, 4 );
		CheatBeep( 0 );
		bPerfectBalance = !bPerfectBalance;
	}
}

void HackSpecial()
{
	if( !CheckIfAttached() )
		return;

	if( !bAlwaysSpecial ){
		// Make Caves
		dwSpecialCave1 = Mem.MakeJump( (BYTE*)0x4F46C1, 0, &btSpecialCave1, sizeof(btSpecialCave1) );
		dwSpecialCave2 = Mem.MakeJump( (BYTE*)0x4F66E4, 2, &btSpecialCave2, sizeof(btSpecialCave2) );
		// Fall Reset
		Mem.WriteNops( (BYTE*)0x4F6267, 3 );
		// Objective Reset
		Mem.WriteNops( (BYTE*)0x4F63D3, 7 );
		Mem.WriteNops( (BYTE*)0x4F66FB, 3 );
		// Force Special Bar
		Mem.WriteMem( (BYTE*)0x4F626A, &btForceSpecialE, sizeof(btForceSpecialE) );
		Mem.WriteMem( (BYTE*)0x4F66E0, &btForceSpecialE, sizeof(btForceSpecialE) );
		CheatBeep( 1 );
		bAlwaysSpecial = !bAlwaysSpecial;
	}else{
		// Fix Cave Jumps
		Mem.WriteMem( (BYTE*)0x4F46C1, &btSpecialOrig1, sizeof(btSpecialOrig1) );
		Mem.WriteMem( (BYTE*)0x4F66E4, &btSpecialOrig2, sizeof(btSpecialOrig2) );
		// Delete Caves
		Mem.DeleteCave( (BYTE*)dwSpecialCave1, sizeof(btSpecialCave1)+5 );
		Mem.DeleteCave( (BYTE*)dwSpecialCave2, sizeof(btSpecialCave2)+5 );
		// Fix Fall Reset
		Mem.WriteMem( (BYTE*)0x4F6267, &btFixFallReset, sizeof(btFixFallReset) );
		// Fix Objective Reset
		Mem.WriteMem( (BYTE*)0x4F63D3, &btFixObjReset1, sizeof(btFixObjReset1) );
		Mem.WriteMem( (BYTE*)0x4F66FB, &btFixObjReset2, sizeof(btFixObjReset2) );
		// Fix Force Special
		Mem.WriteMem( (BYTE*)0x4F626A, &btForceSpecialD, sizeof(btForceSpecialD) );
		Mem.WriteMem( (BYTE*)0x4F66E0, &btForceSpecialD, sizeof(btForceSpecialD) );
		CheatBeep( 0 );
		bAlwaysSpecial = !bAlwaysSpecial;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// @Function   : CheckHotkeys()
// @Parameters : None
// @Purpose    : Handles key presses for hotkeys.
///////////////////////////////////////////////////////////////////////////
void CheckHotkeys()
{
	if( GetAsyncKeyState(VK_F1)&1 )
		HackScore();

	if( GetAsyncKeyState(VK_F2)&1 )
		HackMoney();

	if( GetAsyncKeyState(VK_F3)&1 )
		HackTime();

	if( GetAsyncKeyState(VK_F4)&1 )
		HackProPoints();

	if( GetAsyncKeyState(VK_F5)&1 )
		HackBalance();

	if( GetAsyncKeyState(VK_F6)&1 )
		HackSpecial();

	Mem.CloseProc();
}