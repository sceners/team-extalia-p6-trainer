///////////////////////////////////////////////////////////////////////////
//
// cMemory.cpp
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

#include "cMemory.h"


///////////////////////////////////////////////////////////////////////////
//
// cMemory Construct / Deconstruct
//
///////////////////////////////////////////////////////////////////////////
cMemory::cMemory()
{
	m_hProc		= NULL;
	m_dwProcId	= NULL;
}
cMemory::~cMemory()
{
	if( m_hProc != INVALID_HANDLE_VALUE )
		CloseHandle( m_hProc );

	m_hProc		= NULL;
	m_dwProcId	= NULL;
}


///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::ProcessInfo(...)
// @Parameters : TCHAR*
// @Purpose    : Obtains the process info of the given process name.
///////////////////////////////////////////////////////////////////////////
PROCESSENTRY32 cMemory::ProcessInfo(TCHAR *tszProcName)
{
	PROCESSENTRY32 pe32		= {0};
	HANDLE hSnapshot		= NULL;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if( Process32First(hSnapshot, &pe32) )
	{
		do{
			if( _tcsicmp(pe32.szExeFile, tszProcName) == 0 ){
				CloseHandle( hSnapshot );
				return pe32;
			}
		}while( Process32Next(hSnapshot, &pe32) );
	}
	if( hSnapshot != INVALID_HANDLE_VALUE )
		CloseHandle( hSnapshot );

	memset( &pe32, -1, sizeof(PROCESSENTRY32) );
	return pe32;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::ModuleInfo(...)
// @Parameters : TCHAR*
// @Purpose    : Obtains the module info of the given module name.
//
// This function is to be used after you have attached to a process.
///////////////////////////////////////////////////////////////////////////
MODULEENTRY32 cMemory::ModuleInfo(TCHAR *tszModName)
{
	MODULEENTRY32 me32 = {0};
	HANDLE hSnapshot = NULL;
	me32.dwSize = sizeof(MODULEENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcId);
	if( Module32First(hSnapshot, &me32) )
	{
		do{
			if( _tcsicmp(me32.szModule, tszModName) == 0 ){
				CloseHandle( hSnapshot );
				return me32;
			}
		}while( Module32Next(hSnapshot, &me32) );
	}
	if( hSnapshot != INVALID_HANDLE_VALUE )
		CloseHandle( hSnapshot );

	memset( &me32, -1, sizeof(MODULEENTRY32) );
	return me32;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::OpenProc(...)
// @Parameters : TCHAR*
// @Purpose    : Opens a process for access by its name.
//
// This function is a wrapper to itself. If this is called, it will call
// the second OpenProc function after obtaining the process info.
///////////////////////////////////////////////////////////////////////////
HRESULT cMemory::OpenProc(TCHAR *tszProcName)
{
	PROCESSENTRY32 pe32 = ProcessInfo(tszProcName);
	if( pe32.th32ProcessID != -1 )
		return OpenProc( pe32.th32ProcessID );
	return E_FAIL;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::OpenProc(...)
// @Parameters : DWORD
// @Purpose    : Obtains the process info of the given process id.
//
// Mainly not used by the user, instead, use the other OpenProc for easy
// use. This is used after the process id has been found.
///////////////////////////////////////////////////////////////////////////
HRESULT cMemory::OpenProc(DWORD dwProcId)
{
	if( (dwProcId == 0) || (dwProcId == -1) )
		return E_FAIL;

	m_hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcId);
	if( m_hProc == INVALID_HANDLE_VALUE )
		return E_FAIL;

	m_dwProcId = dwProcId;
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::CloseProc(...)
// @Parameters : NONE
// @Purpose    : Closes the open process handle if valid.
///////////////////////////////////////////////////////////////////////////
HRESULT cMemory::CloseProc()
{
	if( m_hProc == INVALID_HANDLE_VALUE ){
		return E_FAIL;
	}else{
		CloseHandle( m_hProc );
		m_hProc = INVALID_HANDLE_VALUE;
	}
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::GetModBase(...)
// @Parameters : TCHAR*, BYTE**
// @Purpose    : Obtains the module base address of the given module
//               and returns it into pStorage.
//
// You must first be attached to a process for this to work.
///////////////////////////////////////////////////////////////////////////
HRESULT cMemory::GetModBase(TCHAR *tszModName, BYTE** pStorage)
{
	MODULEENTRY32 me32 = ModuleInfo(tszModName);
	if( _tcsicmp(me32.szModule, tszModName) != 0 )
		return E_FAIL;

	*pStorage = me32.modBaseAddr;
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::CalcPtr(...)
// @Parameters : BYTE*, DWORD, DWORD*
// @Purpose    : Calculates a pointer to be read later.
//
// The base address is the first inital pointer to be read. The offset
// is then added to the value read from the first pointer.
///////////////////////////////////////////////////////////////////////////
BOOL cMemory::CalcPtr(BYTE *pBaseAddress, DWORD iPtrOffset, DWORD* pStorage)
{
	BOOL	bRead		= FALSE;
	DWORD	dwPointer	= NULL;

	bRead = ReadProcessMemory( m_hProc, pBaseAddress, &dwPointer, sizeof(DWORD), NULL );
	if( !bRead ){
		*pStorage = -1;
		return FALSE;
	}

	if( IsBadReadPtr( &dwPointer, 4 ) ){
		*pStorage = -1;
		return FALSE;
	}

	*pStorage = (dwPointer+iPtrOffset);
	return bRead;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::ReadPtr(...)
// @Parameters : BYTE*, DWORD, DWORD*
// @Purpose    : Reads a pointer for its value.
///////////////////////////////////////////////////////////////////////////
BOOL cMemory::ReadPtr(BYTE *pBaseAddress, DWORD iPtrOffset, DWORD* pStorage)
{
	BOOL	bRead		= FALSE;
	DWORD	dwPointer	= NULL;
	DWORD	dwValue		= NULL;

	bRead = ReadProcessMemory( m_hProc, pBaseAddress, &dwPointer, sizeof(DWORD), NULL );
	if( !bRead )
		return FALSE;

	if( IsBadReadPtr( &dwPointer, 4 ) )
		return FALSE;

	bRead = ReadProcessMemory( m_hProc, (LPCVOID)(DWORD_PTR)(dwPointer+iPtrOffset), &dwValue, sizeof(DWORD), NULL );
	if( !bRead )
		return FALSE;

	*pStorage = dwValue;
	return bRead;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::ReadMem(...)
// @Parameters : BYTE*, LPVOID, DWORD
// @Purpose    : Reads an address and returns the value to pStorage.
//
// pStorage is a pointer, so you can use any datatype. If needed, you
// can template this function instead.
///////////////////////////////////////////////////////////////////////////
BOOL cMemory::ReadMem(BYTE* pAddress, LPVOID pStorage, DWORD dwSize)
{
	if( m_hProc == INVALID_HANDLE_VALUE )
		return E_FAIL;

	DWORD dwOldProtect = NULL;
	VirtualProtectEx( m_hProc, pAddress, dwSize, PAGE_EXECUTE_READWRITE, &dwOldProtect );
	BOOL bRead = ReadProcessMemory( m_hProc, pAddress, pStorage, dwSize, NULL );
	VirtualProtectEx( m_hProc, pAddress, dwSize, dwOldProtect, &dwOldProtect );
	return bRead;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::WriteMem(...)
// @Parameters : BYTE*, LPVOID, DWORD
// @Purpose    : Writes a value to the given address.
//
// pBuffer is a pointer to the value(s) to be written. This can be any
// datatype as well.
///////////////////////////////////////////////////////////////////////////
BOOL cMemory::WriteMem(BYTE *pAddress, LPVOID pBuffer, DWORD dwSize)
{
	if( m_hProc == INVALID_HANDLE_VALUE )
		return FALSE;

	DWORD dwOldProtect = NULL;
	BOOL bProt = VirtualProtectEx( m_hProc, pAddress, dwSize, PAGE_EXECUTE_READWRITE, &dwOldProtect );
	BOOL bWrite = WriteProcessMemory( m_hProc, pAddress, pBuffer, dwSize, NULL );
	VirtualProtectEx( m_hProc, pAddress, dwSize, dwOldProtect, &dwOldProtect );

	return bWrite;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::WriteNops(...)
// @Parameters : BYTE*, int
// @Purpose    : Writes a series of NOPs that starts at the given address
//				 and steps 1 for each NOP to write.
///////////////////////////////////////////////////////////////////////////
BOOL cMemory::WriteNops(BYTE* pAddress, int iNopsToWrite)
{
	if( m_hProc == INVALID_HANDLE_VALUE )
		return FALSE;

	DWORD dwOldProtect = NULL;
	VirtualProtectEx( m_hProc, pAddress, iNopsToWrite, PAGE_EXECUTE_READWRITE, &dwOldProtect );

	BYTE bNOP[] = { 0x90 };
	for( int x = 0; x < iNopsToWrite; x++ )
		if( !WriteProcessMemory( m_hProc, pAddress+x, &bNOP, 1, NULL ) )
			return FALSE;
	VirtualProtectEx( m_hProc, pAddress, iNopsToWrite, dwOldProtect, &dwOldProtect );
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::MakeCave(...)
// @Parameters : LPVOID, DWORD
// @Purpose    : Creates a code cave based on the cave bytes and size.
//
// This function returns the cave address! It is important that you
// store this address to destroy the cave later on if needed!
///////////////////////////////////////////////////////////////////////////
DWORD cMemory::MakeCave(LPVOID pCaveBytes, DWORD dwCaveSize)
{
	if( m_hProc == INVALID_HANDLE_VALUE )
		return NULL;

	DWORD dwCaveAddress = NULL;
	dwCaveAddress = (DWORD)VirtualAllocEx( m_hProc, NULL, dwCaveSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE );

	if( pCaveBytes != NULL )
		WriteMem( (BYTE*)dwCaveAddress, pCaveBytes, dwCaveSize );

	return dwCaveAddress;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::DeleteCave(...)
// @Parameters : BYTE*, DWORD
// @Purpose    : Deletes a cave based on the address and size.
//
// The size is important to ensure the whole cave is deleted and loose
// bytes are not left behind.
///////////////////////////////////////////////////////////////////////////
BOOL cMemory::DeleteCave(BYTE* pCaveAddr, DWORD dwCaveSize)
{
	if( m_hProc == INVALID_HANDLE_VALUE )
		return FALSE;

	BOOL bRet = VirtualFreeEx( m_hProc, pCaveAddr, dwCaveSize, MEM_DECOMMIT );
	return bRet;
}

///////////////////////////////////////////////////////////////////////////
// @Function   : cMemory::MakeJump(...)
// @Parameters : BYTE*, int, LPVOID, DWORD
// @Purpose    : Creates a jump to a code cave.
//
// This function also returns the cave address! It is important that you
// store this address later incase you need to delete the cave.
///////////////////////////////////////////////////////////////////////////
DWORD cMemory::MakeJump(BYTE* pOrigAddress, int iNopsNeeded, LPVOID pCaveBytes, DWORD dwCaveSize)
{
	//
	// Ensure the handle is valid.
	//
	if( m_hProc == INVALID_HANDLE_VALUE )
		return NULL;

	//
	// Create the code cave before doing anything. This is to ensure
	// when the jump is created it has something to jump to.
	//
	DWORD dwCaveAddress = NULL;
	dwCaveAddress = MakeCave( pCaveBytes, (dwCaveSize+5) );

	//
	// Calculate the return jump address for the cave.
	//
	BYTE* dwReturnJmp = (PBYTE)(((DWORD)pOrigAddress+5)+iNopsNeeded);
	dwReturnJmp = dwReturnJmp - (dwCaveAddress+(dwCaveSize+5));

	//
	// Write the return jump inside the cave. The cave size
	// is +5 to hold the jump address. E9 is the byte for JMP
	//
	BYTE bJmp[] = {0xE9};
	WriteMem( (BYTE*)((dwCaveAddress+dwCaveSize)), &bJmp, 1 );
	WriteMem( (BYTE*)((dwCaveAddress+dwCaveSize)+1), &dwReturnJmp, 4 );

	//
	// Write the main jump at the original address to our cave
	// and write nops if needed. (NOPs are needed if you are
	// replacing code that is longer then 5 bytes!)
	//
	DWORD dwJmpAddr = dwCaveAddress - ((DWORD)pOrigAddress+5);
	if( iNopsNeeded != 0 )
		WriteNops((pOrigAddress+5), iNopsNeeded);

	WriteMem( pOrigAddress, &bJmp, 1 );
	WriteMem( (pOrigAddress+1), &dwJmpAddr, 4 ); 

	//
	// Return the created cave address.
	//
	return dwCaveAddress;
}