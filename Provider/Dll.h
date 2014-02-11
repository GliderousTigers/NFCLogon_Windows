/** @file Dll.h
* Fulfills baseline support for COM server.
* and DLL requirements.
* Standard dll required functions and class factory implementation.
*
@date 8/2/2014
*/

#pragma once

extern HINSTANCE g_hinst;
#define HINST_THISDLL g_hinst

void DllAddRef();
void DllRelease();
