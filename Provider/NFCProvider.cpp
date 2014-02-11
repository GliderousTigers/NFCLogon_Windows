//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) 2006 Microsoft Corporation. All rights reserved.
//
// NFCProvider implements ICredentialProvider, which is the main
// interface that logonUI uses to decide which tiles to display.

#include <credentialprovider.h>
#include "NFCProvider.h"
#include "NFCCredential.h"
#include "guid.h"
#include <iostream>
#include "common.h"
#include <stdio.h>

NFCCredential* NFCProvider::_rgpCredentials[MAX_CREDENTIALS];

NFCProvider::NFCProvider():
	_cRef(1),
	_dwNumCreds(0)
{
	DllAddRef();
	ZeroMemory(_rgpCredentials, sizeof(_rgpCredentials));
}

NFCProvider::~NFCProvider()
{
	for (size_t i = 0; i < _dwNumCreds; i++)
	{
		if (_rgpCredentials[i] != NULL)
		{
			_rgpCredentials[i]->Release();
		}
	}

	DllRelease();
}


HANDLE hThread;
bool NFCThreadRunning = true;
NFCProvider *prp;
bool doAutoLogin = false;

void getCredentials(char* token){

	//HKEY hKey;
	//if(RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Tyler Menezes\\Rfid Login\\Keys", 0, KEY_READ, &hKey) != ERROR_SUCCESS){
	//	delete hKey;
	//	return; // Token not recognized.
	//}

	//std::wstring keySalt;
	//GetStringRegKey(hKey, L"Salt", keySalt, L"bad");


	//CSHA1 *sha1 = new CSHA1();
	//sha1->Update((unsigned char*)token, strlen(token));
	//sha1->Update((unsigned char*)s2cs(ws2s(keySalt)), wcslen(keySalt.c_str()));
	//sha1->Final();
	//std::wstring hash;
	//sha1->ReportHashStl(hash, CSHA1::REPORT_HEX_SHORT);
	//delete sha1;

	//std::string key = std::string("SOFTWARE\\Tyler Menezes\\Rfid Login\\Keys\\");
	//key += ws2s(hash);

	//if(RegOpenKeyExW(HKEY_LOCAL_MACHINE, s2ws(key).c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS){
	//	delete hKey;
	//	return; // Token not recognized.
	//}

	//std::wstring salt;
	//std::wstring username;
	//std::wstring password;
	//std::wstring domain;

	//char* cUsername = s2cs(GetCharRegKey(hKey, L"Username"));
	//char* cPassword = s2cs(GetCharRegKey(hKey, L"Password"));
	//char* cDomain	= s2cs(GetCharRegKey(hKey, L"Domain"));
	//
	//GetStringRegKey(hKey, L"Salt", salt, L"");
	//
	//unsigned char* cSalt = (unsigned char*)s2cs(ws2s(salt));

	//std::wstring decryptionKey;
	//sha1 = new CSHA1();
	//sha1->Update((unsigned char*)token, strlen(token));
	//sha1->Update(cSalt, strlen((char*)cSalt));
	//sha1->Final();
	//sha1->ReportHashStl(decryptionKey, CSHA1::REPORT_HEX_SHORT);
	//delete sha1;

	//char* cDecryptionKey = s2cs(ws2s(decryptionKey));

	//decrypt(cUsername, cDecryptionKey);
	//decrypt(cPassword, cDecryptionKey);
	//decrypt(cDomain, cDecryptionKey);

	//username = s2ws(std::string(cUsername));
	//password = s2ws(std::string(cPassword));
	//domain = s2ws(std::string(cDomain));

	//delete cSalt;
	//delete cUsername;
	//delete cPassword;
	//delete cDomain;
	//delete cDecryptionKey;

	//prp->_dwNumCreds -= 1;

	//wchar_t* wUsername = wcs2cs(username);
	//wchar_t* wPassword = wcs2cs(password);
	//wchar_t* wDomain = wcs2cs(domain);

	//NFCCredential::lastLoginFailed = false;

	//doAutoLogin = true;
	//prp->_EnumerateOneCredential(prp->_dwNumCreds, wUsername, wPassword, wDomain);
	//prp->Pcpe->CredentialsChanged(prp->UpAdviseContext);
}

void NFCProvider::onDataRecieved(CommResponse& response)
{

}

DWORD WINAPI _NFCReader(LPVOID lpParameter)
{
	/*Serial* s = new Serial();

	char *lastTag = new char[128];
	memset(lastTag, '\0', 128);
	while(lastTag[0] != '\0');

	int tagCount = 0;

	while(NFCThreadRunning){

		Sleep(50);

		char *id = new char[128];
		memset(id, '\0', 128);
		while(id[0] != '\0');

		if(tagCount >= 128){
			tagCount = 0;
			memset(lastTag, '\0', 128);
			while(lastTag[0] != '\0');
		}

		s->ReadData(id, 1);

		if(strlen(id) > 0){
			for(int i = 0; i < strlen(id); i++){
				lastTag[tagCount++] = id[i];
			}
		}

		delete id;


		for(int i = 0; i < strlen(lastTag); i++){
			if(lastTag[i] == 0x03){
				char* fullTag = sanatizeRfidReading(lastTag);

				printf(fullTag);

				getCredentials(fullTag);

				tagCount = 0;
				memset(lastTag, '\0', 128);
				while(lastTag[0] != '\0');

				delete fullTag;

				break;
			}
		}
	}

	delete lastTag;
	delete s;*/

	return 0;
}


void InitNFCReader(){
	NFCThreadRunning = true;
	hThread = ::CreateThread(NULL, 0, _NFCReader, NULL , 0, NULL);
}

void StopNFCReader(){
	NFCThreadRunning = false;
}

HRESULT NFCProvider::SetUsageScenario(
	CREDENTIAL_PROVIDER_USAGE_SCENARIO cpus,
	DWORD dwFlags
	)
{
	UNREFERENCED_PARAMETER(dwFlags);

	HRESULT hr;
	static bool s_bCredsEnumerated = false;
	switch (cpus)
	{
	case CPUS_LOGON:
	case CPUS_CREDUI:
		NFCCredential::lockType = KerbInteractiveLogon;
		break;
	case CPUS_UNLOCK_WORKSTATION:    
		NFCCredential::lockType = KerbWorkstationUnlockLogon;
		break;
	case CPUS_CHANGE_PASSWORD:
		hr = E_NOTIMPL;
		break;

	default:
		hr = E_INVALIDARG;
		break;
	}

		if (!s_bCredsEnumerated)
		{
			prp = this;
			hr = this->_EnumerateCredentials();
			s_bCredsEnumerated = true;
		}
		else
		{
			hr = S_OK;
		}

	return hr;
}

STDMETHODIMP NFCProvider::SetSerialization(
	const CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcs
	)
{
	UNREFERENCED_PARAMETER(pcpcs);

	return E_NOTIMPL;
}

ICredentialProviderEvents* Pcpe;
UINT_PTR UpAdviseContext;

HRESULT NFCProvider::Advise(
	ICredentialProviderEvents* pcpe,
	UINT_PTR upAdviseContext
	)
{
	InitNFCReader();

	Pcpe=pcpe;
	Pcpe->AddRef();
	UpAdviseContext=upAdviseContext;

	return S_OK;

}

HRESULT NFCProvider::UnAdvise()
{
	StopNFCReader();

	 if(Pcpe)
	{
		Pcpe->Release();
		Pcpe=NULL;
	}
	UpAdviseContext=NULL;


	return S_OK;
}

HRESULT NFCProvider::GetFieldDescriptorCount(
	DWORD* pdwCount
	)
{
	*pdwCount = SFI_NUM_FIELDS;

	return S_OK;
}

HRESULT NFCProvider::GetFieldDescriptorAt(
	DWORD dwIndex, 
	CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR** ppcpfd
	)
{    
	HRESULT hr;
	if ((dwIndex < SFI_NUM_FIELDS) && ppcpfd)
	{
		hr = FieldDescriptorCoAllocCopy(s_rgCredProvFieldDescriptors[dwIndex], ppcpfd);
	}
	else
	{ 
		hr = E_INVALIDARG;
	}

	return hr;
}

HRESULT NFCProvider::GetCredentialCount(
	DWORD* pdwCount,
	DWORD* pdwDefault,
	BOOL* pbAutoLogonWithDefault
	)
{
	HRESULT hr;

	if(doAutoLogin && _dwNumCreds > 0 && !NFCCredential::lastLoginFailed){
		*pdwDefault = _dwNumCreds - 1;
		*pbAutoLogonWithDefault = TRUE;
	}else{
		*pdwDefault = CREDENTIAL_PROVIDER_NO_DEFAULT;
		*pbAutoLogonWithDefault = FALSE;
	}

	*pdwCount = _dwNumCreds; 
	if (*pdwCount > 0)
	{
		hr = S_OK;
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}

HRESULT NFCProvider::GetCredentialAt(
	DWORD dwIndex, 
	ICredentialProviderCredential** ppcpc
	)
{
	HRESULT hr;

	if((dwIndex < _dwNumCreds) && ppcpc)
	{
		hr = _rgpCredentials[dwIndex]->QueryInterface(IID_ICredentialProviderCredential, reinterpret_cast<void**>(ppcpc));
	}
	else
	{
		hr = E_INVALIDARG;
	}

	return hr;
}

HRESULT NFCProvider::_EnumerateOneCredential(
	DWORD dwCredentialIndex,
	PWSTR pwzUsername,
	PWSTR pwzPassword,
	PWSTR pwzDomain
	)
{
	HRESULT hr;
	NFCCredential* ppc = new NFCCredential();

	if (ppc)
	{
		hr = ppc->Initialize(s_rgCredProvFieldDescriptors, s_rgFieldStatePairs, pwzUsername, pwzPassword, pwzDomain);
		ppc->lastLoginFailed = false;

		if (SUCCEEDED(hr))
		{
			_rgpCredentials[dwCredentialIndex] = ppc;
			_dwNumCreds++;
		}
		else
		{
			ppc->Release();
		}
	}
	else
	{
		hr = E_OUTOFMEMORY;
	}

	return hr;
}
HRESULT NFCProvider::_EnumerateCredentials()
{
	HRESULT hr = _EnumerateOneCredential(0, NULL, NULL, NULL);

	return hr;
}
HRESULT NFCProvider_CreateInstance(REFIID riid, void** ppv)
{
	HRESULT hr;

	NFCProvider* pProvider = new NFCProvider();

	if (pProvider)
	{
		hr = pProvider->QueryInterface(riid, ppv);
		pProvider->Release();
	}
	else
	{
		hr = E_OUTOFMEMORY;
	}

	return hr;
}
