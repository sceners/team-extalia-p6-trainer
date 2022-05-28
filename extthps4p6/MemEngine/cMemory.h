///////////////////////////////////////////////////////////////////////////
//
// cMemory.h
// Copyright(c) 2007, Wiccaan
//
///////////////////////////////////////////////////////////////////////////
// This memory class is a wrapper around some API to
// allow the user to easily use functions to be used
// inside a game trainer that accesses another procs
// memory.
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
#include "tlhelp32.h"

class cMemory
{
public:
	cMemory();
	~cMemory();

	HRESULT OpenProc		(TCHAR *tszProcName);
	HRESULT OpenProc		(DWORD dwProcId);
	HRESULT CloseProc		();

	HRESULT GetModBase		(TCHAR *tszModName, BYTE** pStorage);

	BOOL	CalcPtr			(BYTE *pBaseAddress, DWORD iPtrOffset, DWORD* pStorage);
	BOOL	ReadPtr			(BYTE *pBaseAddress, DWORD iPtrOffset, DWORD* pStorage);

	BOOL	ReadMem			(BYTE* pAddress, LPVOID pStorage, DWORD dwSize);

	BOOL	WriteMem		(BYTE* pAddress, LPVOID pBuffer, DWORD dwSize);
	BOOL	WriteNops		(BYTE* pAddress, int iNopsToWrite);

	DWORD	MakeCave		(LPVOID pCaveBytes, DWORD dwCaveSize);
	BOOL	DeleteCave		(BYTE* pCaveAddr, DWORD dwCaveSize);

	DWORD	MakeJump		(BYTE* pOrigAddress, int iNopsNeeded, LPVOID pCaveBytes, DWORD dwCaveSize);

	HANDLE	GetOpenHandle	(){ return m_hProc; }		// Obtain Current Process Handle
	DWORD	GetOpenProcId	(){ return m_dwProcId; }	// Obtain Current Process Id

protected:
	PROCESSENTRY32	ProcessInfo	(TCHAR *tszProcName);
	MODULEENTRY32	ModuleInfo	(TCHAR *tszModName);

private:
	HANDLE	m_hProc;
	DWORD	m_dwProcId;
};