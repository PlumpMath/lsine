/*
lsine.cpp
This work is part of the Litestep Interop Not Emulate Project

Copyright (c) 2007, Brian Hartvigsen
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Litestep Interop Not Emulate Project nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <bbapi.h>
#include "ine.h"
#include "lsapi.h"
#include "bang.h"

typedef int (*ModuleInitExFunc) (HWND, HINSTANCE, LPCSTR);
typedef int (*ModuleQuitFunc) (HINSTANCE);

extern "C"
{
	DLL_EXPORT int beginPlugin(HINSTANCE h);
	DLL_EXPORT void endPlugin(HINSTANCE h);
	DLL_EXPORT LPCSTR    pluginInfo(int x)
    {
        switch (x)
        {
            case PLUGIN_NAME:			return "LS4BB";
            case PLUGIN_VERSION:		return "0.0.1";
            case PLUGIN_AUTHOR:			return "Brian Hartvigsen";
            case PLUGIN_RELEASEDATE:	return "2006 Feb 10";
			case PLUGIN_LINK:			return "http://tresni.crackmonkey.us";
            case PLUGIN_EMAIL:			return "tresni@crackmonkey.us";
            default:					return "LS4BB 0.0.1";
        }
    };
};

HMODULE hMod;
ModuleInitExFunc initMod;
ModuleQuitFunc quitMod;

int beginPlugin(HINSTANCE h)
{
	hMod = LoadLibrary("label.dll");
	initMod = (ModuleInitExFunc)GetProcAddress(hMod, "initModuleEx");
	quitMod = (ModuleQuitFunc)GetProcAddress(hMod, "quitModule");
	if (initMod && quitMod)
		initMod(GetBBWnd(), hMod, "label.dll");
	return TRUE;
}

void endPlugin(HINSTANCE h)
{
	quitMod(hMod);
}

// TODO: Add these calls to InteropNotEmualte
LSAPI FILE* LCOpen (LPCSTR szPath)
{
	return InteropNotEmulate.GetSettingsHandler()->LCOpen(szPath);
}

LSAPI BOOL LCClose (FILE * f)
{
	return InteropNotEmulate.GetSettingsHandler()->LCClose(f);
}

// TODO: Clean up RC reading, look for colons
LSAPI BOOL LCReadNextCommand(FILE * pFile, LPSTR pszValue, size_t cchValue)
{
	return InteropNotEmulate.GetSettingsHandler()->LCReadNextCommand(pFile, pszValue, (DWORD)cchValue);
}

LSAPI BOOL LCReadNextConfig(FILE * pFile, LPCSTR pszConfig, LPSTR pszValue, size_t cchValue)
{
	return InteropNotEmulate.GetSettingsHandler()->LCReadNextConfig(pFile, pszConfig, pszValue, cchValue);
}

LSAPI BOOL LCReadNextLine(FILE * pFile, LPSTR pszValue, size_t cchValue)
{
	return InteropNotEmulate.GetSettingsHandler()->LCReadNextCommand(pFile, pszValue, cchValue);
}

LSAPI int LCTokenize (LPCSTR szString, LPSTR * lpszBuffers, DWORD dwNumBuffers, LPSTR szExtraParameters)
{
	return InteropNotEmulate.GetSettingsHandler()->LCTokenize(szString, lpszBuffers,dwNumBuffers, szExtraParameters);
}

LSAPI int GetRCInt(LPCSTR lpKeyName, int nDefault)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCInt(lpKeyName, nDefault);
}

LSAPI BOOL GetRCString(LPCSTR lpKeyName, LPSTR value, LPCSTR defStr, int maxLen)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCString(lpKeyName, value, defStr, maxLen);
}

LSAPI BOOL GetRCBool(LPCSTR lpKeyName, BOOL ifFound)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCBool(lpKeyName, ifFound);
}

LSAPI BOOL GetRCBoolDef(LPCSTR lpKeyName, BOOL bDefault)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCBoolDef(lpKeyName, bDefault);
}

LSAPI BOOL GetRCLine(LPCSTR szKeyName, LPSTR szBuffer, UINT nBufLen, LPCSTR szDefault)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCString(szKeyName, szBuffer, szDefault, nBufLen);
}

LSAPI COLORREF GetRCColor(LPCSTR lpKeyName, COLORREF colDef)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCColor(lpKeyName, colDef);
}

// TODO: Implement proper variable support.
LSAPI BOOL LSGetVariable(LPCSTR pszKeyName, LPSTR pszValue)
{
	return InteropNotEmulate.GetSettingsHandler()->LSGetVariable(pszKeyName, pszValue);
}

LSAPI BOOL LSGetVariableEx(LPCSTR pszKeyName, LPSTR pszValue, DWORD dwLength)
{
	return InteropNotEmulate.GetSettingsHandler()->LSGetVariableEx(pszKeyName, pszValue, dwLength);
}

LSAPI void LSSetVariable(LPCSTR pszKeyName, LPCSTR pszValue)
{
	return InteropNotEmulate.GetSettingsHandler()->LSSetVariable(pszKeyName, pszValue);
}

LSAPI BOOL AddBangCommand(LPCSTR pszCommand, BangCommand pfnBangCommand)
{
	return InteropNotEmulate.GetBangHandler()->AddBang(pszCommand, new Bang(pfnBangCommand, pszCommand));
}

LSAPI BOOL AddBangCommandEx(LPCSTR pszCommand, BangCommandEx pfnBangCommand)
{
	return InteropNotEmulate.GetBangHandler()->AddBang(pszCommand, new Bang(pfnBangCommand, pszCommand));
}

LSAPI BOOL RemoveBangCommand(LPCSTR pszCommand)
{
	return InteropNotEmulate.GetBangHandler()->RemoveBang(pszCommand);
}

LSAPI BOOL ParseBangCommand (HWND hCaller, LPCSTR pszCommand, LPCSTR pszArgs)
{
	return InteropNotEmulate.GetBangHandler()->ParseBang(hCaller, pszCommand, pszArgs);
}

// TODO: All this!
LSAPI HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance, int xoffset, int yoffset)
{
	return NULL;
}

LSAPI HBITMAP BitmapFromIcon (HICON hIcon)
{
/*
This is a part of the LiteStep Shell Source code.

Copyright (C) 1997-2005 The LiteStep Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/ 
	ICONINFO infoIcon;

	if (GetIconInfo(hIcon, &infoIcon))
	{
		HDC hDC;
		HBITMAP hOldBMP;
		HBRUSH hBrush;

		hDC = CreateCompatibleDC(NULL);
		hOldBMP = (HBITMAP)SelectObject(hDC, infoIcon.hbmColor);
		hBrush = CreateSolidBrush(RGB (255, 0, 255));
		DrawIconEx(hDC, 0, 0, hIcon, 0, 0, 0, hBrush, DI_NORMAL);
		DeleteObject(hBrush);
		DeleteObject(infoIcon.hbmMask);
		SelectObject(hDC, hOldBMP);
		DeleteDC(hDC);

		return infoIcon.hbmColor;
	}

	return NULL;
/* END LITESTEP CODE */
}

LSAPI HBITMAP LoadLSImage(LPCSTR szFile, LPCSTR szImage)
{
	char dimensions[MAX_LINE_LENGTH];
	char colorTo[MAX_LINE_LENGTH];
	char colorFrom[MAX_LINE_LENGTH];
	char colorBorder[MAX_LINE_LENGTH];
	char borderSize[MAX_LINE_LENGTH];
	char gradient[MAX_LINE_LENGTH];
	char *tokens[5];
	tokens[0] = dimensions;
	tokens[1] = colorFrom;
	tokens[2] = colorTo;
	tokens[3] = colorBorder;
	tokens[4] = borderSize;
	BBTokenize(szFile, tokens, 5, gradient);
	char* width = strtok(dimensions, "x");

	StyleItem si;

	si.Color = ReadColor(extensionsrcPath(), "i.do.not.exist!:",colorFrom);
	si.ColorTo = ReadColor(extensionsrcPath(), "i.do.not.exist!:",colorTo);
	si.borderColor = ReadColor(extensionsrcPath(), "i.do.not.exist!:",colorBorder);
	si.borderWidth = atoi(borderSize);

	ParseItem(gradient, &si);
	HDC hDC = CreateCompatibleDC(NULL);
	BITMAPINFO alpha;
	ZeroMemory( &alpha.bmiHeader, sizeof(BITMAPINFOHEADER) );
	alpha.bmiHeader.biWidth=atoi(width);      // Set size you need
	alpha.bmiHeader.biHeight=atoi(dimensions);    // Set size you need
	alpha.bmiHeader.biPlanes=1;
	alpha.bmiHeader.biBitCount=32;      // Can be 8, 16, 32 bpp or other number
	alpha.bmiHeader.biSizeImage=0;
	alpha.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	alpha.bmiHeader.biClrUsed= 0;
	alpha.bmiHeader.biClrImportant= 0;
	VOID *pvBits;

	HBITMAP bufferBitmap = CreateDIBSection( hDC,
												&alpha,
												DIB_RGB_COLORS,
												&pvBits,
												NULL,
												0 );
	HBITMAP oldBMP = (HBITMAP) SelectObject(hDC, bufferBitmap);
	RECT r = {0,0,atoi(width), atoi(dimensions)};
	MakeStyleGradient(hDC, &r, &si, false);
	bufferBitmap = (HBITMAP) SelectObject(hDC, oldBMP);
	ReleaseDC(NULL, hDC);
	DeleteObject(oldBMP);
	return bufferBitmap;
}

LSAPI HICON LoadLSIcon (LPCSTR pszIconPath, LPCSTR pszFile)
{
/*
This function's code is a part of the LiteStep Shell Source code.

Copyright (C) 1997-2005 The LiteStep Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/ 
	HICON hIcon = ExtractIcon(NULL, pszIconPath, 0);
	if (hIcon == NULL)
		hIcon = ExtractIcon(NULL, pszFile, 0);
	return hIcon;
}

LSAPI void GetLSBitmapSize(HBITMAP hBitmap, LPINT nWidth, LPINT nHeight)
{
/*
This function's code is a part of the LiteStep Shell Source code.

Copyright (C) 1997-2005 The LiteStep Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/ 
	BITMAP hbmBitmap;

	if (!GetObject(hBitmap, sizeof(hbmBitmap), (LPSTR) & hbmBitmap))
	{
		*nWidth = 0;
		*nHeight = 0;
	}
	else
	{
		*nWidth = hbmBitmap.bmWidth;
		*nHeight = hbmBitmap.bmHeight;
	}
}
LSAPI void TransparentBltLS (HDC dc, int nXDest, int nYDest, int nWidth, int nHeight, HDC tempDC, int nXSrc, int nYSrc, COLORREF colorTransparent) {}

LSAPI int CommandTokenize (LPCSTR szString, LPSTR * lpszBuffers, DWORD dwNumBuffers, LPSTR szExtraParameters)
{
	return 0;
}

LSAPI void CommandParse(LPCSTR pszCommand, LPSTR pszOutCommand, LPSTR pszOutArgs, size_t cchOutCommand, size_t cchOutArgs) {}

LSAPI HINSTANCE LSExecute(HWND Owner, LPCSTR szCommand, int nShowCmd)
{
	return BBExecute(Owner, "", szCommand, "", "", nShowCmd, true);
}

LSAPI HINSTANCE LSExecuteEx(HWND Owner, LPCSTR szOperation, LPCSTR szCommand, LPCSTR szArgs, LPCSTR szDirectory, int nShowCmd)
{
	return BBExecute(Owner, szOperation, szCommand, szArgs, szDirectory, nShowCmd, true);
}

LSAPI HWND GetLitestepWnd(void)
{
	return GetBBWnd();
}

LSAPI BOOL WINAPI LSGetLitestepPath(LPSTR pszPath, size_t cchPath)
{
	GetBlackboxPath(pszPath, (int)cchPath);
	return TRUE;
}

LSAPI BOOL WINAPI LSGetImagePath(LPSTR pszPath, size_t cchPath)
{
	GetBlackboxPath(pszPath, (int)cchPath);
	strncat(pszPath, "images//", cchPath - lstrlen(pszPath));
	return TRUE;
}

LSAPI void VarExpansion(LPSTR pszExpandedString, LPCSTR pszTemplate){}
LSAPI void VarExpansionEx(LPSTR pszExpandedString, LPCSTR pszTemplate, size_t cchExpandedString){}

LSAPI BOOL GetToken(LPCSTR szString, LPSTR szToken, LPCSTR * szNextToken, BOOL useBrackets)
{
	return FALSE;
}

LSAPI void Frame3D(HDC dc, RECT rect, COLORREF TopColor, COLORREF BottomColor, int Width){}
LSAPI void SetDesktopArea(int left, int top, int right, int bottom){}

LSAPI BOOL match(LPCSTR pattern, LPCSTR text)
{
	return FALSE;
}

LSAPI int matche(LPCSTR pattern, LPCSTR text)
{
	return 0;
}

LSAPI BOOL is_valid_pattern(LPCSTR p, LPINT error_type)
{
	return FALSE;
}

LSAPI void GetResStr(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText){}
LSAPI void GetResStrEx(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText, ...){}

LSAPI int LSGetSystemMetrics(int)
{
	return 0;
}

LSAPI HMONITOR LSMonitorFromWindow(HWND, DWORD)
{
	return NULL;
}

LSAPI HMONITOR LSMonitorFromRect(LPCRECT, DWORD)
{
	return NULL;
}

LSAPI HMONITOR LSMonitorFromPoint(POINT, DWORD)
{
	return NULL;
}

LSAPI BOOL LSGetMonitorInfo(HMONITOR, LPMONITORINFO)
{
	return NULL;
}

LSAPI BOOL LSEnumDisplayMonitors(HDC, LPCRECT, MONITORENUMPROC, LPARAM)
{
	return NULL;
}

LSAPI BOOL LSEnumDisplayDevices(PVOID, DWORD, PDISPLAY_DEVICE, DWORD)
{
	return NULL;
}

LSAPI BOOL WINAPI LSLog(int nLevel, LPCSTR pszModule, LPCSTR pszMessage)
{
	return FALSE;
}

LSAPI BOOL WINAPIV LSLogPrintf(int nLevel, LPCSTR pszModule, LPCSTR pszFormat, ...)
{
	return FALSE;
}

LSAPI int GetRCCoordinate(LPCSTR pszKeyName, int nDefault, int nMaxVal)
{
	return 0;
}

LSAPI int ParseCoordinate(LPCSTR szString, int nDefault, int nMaxVal)
{
	return 0;
}

LSAPI HRESULT EnumLSData(UINT uInfo, FARPROC pfnCallback, LPARAM lParam)
{
	return NULL;
}