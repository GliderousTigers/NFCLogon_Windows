/** @file helpers.h
Provides utility methods for working with
UNICODE strings and auth packages.

@date 8/2/2014
@version 0.1
*/

#pragma once
#include "common.h"
#include <windows.h>
#include <strsafe.h>
#include <string>
#include <vector>
#include <cstring>

#pragma warning(push)
#pragma warning(disable : 4995)
#include <shlwapi.h>
#pragma warning(pop)

/**
* Copies the field descriptor pointed to by rcpfd into a buffer allocated 
* using CoTaskMemAlloc. Returns that buffer in ppcpfd.
*/
HRESULT FieldDescriptorCoAllocCopy(
    const CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR& rcpfd,
    CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR** ppcpfd
    );

/** Copies rcpfd into the buffer pointed to by pcpfd.The caller is responsible for
* allocating pcpfd. This function uses CoTaskMemAlloc to allocate memory for 
* pcpfd->pszLabel.
*/
HRESULT FieldDescriptorCopy(
    const CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR& rcpfd,
    CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR* pcpfd
    );
/** This function copies the length of pwz and the pointer pwz into the UNICODE_STRING structure
* This function is intended for serializing a credential in GetSerialization only.
* Note that this function just makes a copy of the string pointer. It DOES NOT ALLOCATE storage!
*/
HRESULT UnicodeStringInitWithString(
    PWSTR pwz, 
    UNICODE_STRING* pus
    );
/*WinLogon and LSA consume "packed" KERB_INTERACTIVE_LOGONs.In these, the PWSTR members of each
* UNICODE_STRING are not actually pointers but byte offsets into the overall buffer represented
* by the packed KERB_INTERACTIVE_LOGON.  For example:
*
* kil.LogonDomainName.Length = 14                             -> Length is in bytes, not characters
* kil.LogonDomainName.Buffer = sizeof(KERB_INTERACTIVE_LOGON) -> LogonDomainName begins immediately
*                                                                after the KERB_... struct in the buffer
* kil.UserName.Length = 10
* kil.UserName.Buffer = sizeof(KERB_INTERACTIVE_LOGON) + 14   -> UNICODE_STRINGS are NOT null-terminated
*
* kil.Password.Length = 16
* kil.Password.Buffer = sizeof(KERB_INTERACTIVE_LOGON) + 14 + 10
*
* @link http://msdn.microsoft.com/msdnmag/issues/05/06/SecurityBriefs/#void
*/
HRESULT KerbInteractiveLogonPack(
    const KERB_INTERACTIVE_LOGON& rkil,
    BYTE** prgb,
    DWORD* pcb
    );
/**
* Retrieves the 'negotiate' AuthPackage from the LSA. In this case, Kerberos.
*/
HRESULT RetrieveNegotiateAuthPackage(
    ULONG * pulAuthPackage
    );
/**
* converts a utf-8 string to a wide string (utf-16)
* @return the convered string.
* @see ws2s
*/
std::wstring s2ws(const std::string& s);

/**
* converts a utf-16 wide string to a utf-8 string 
* @return the convered string.
* @see s2ws
*/
std::string ws2s(const std::wstring& si);

/**
* converts a utf-16 wide const string to a utf-8 const string
* @return the convered string.
*/
wchar_t* wcs2cs(std::wstring& s);

/**
* converts a string into a const string
* @return the convered string.
*/
char* s2cs(std::string& s);

/**
* converts a string into a unsigned const string
* @return the convered string.
*/
unsigned char* s2ucs(std::string& s);