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
#include <sstream>

NFCCredential* NFCProvider::_rgpCredentials[MAX_CREDENTIALS];

NFCProvider::NFCProvider():
	_cRef(1),
	_dwNumCreds(0)
{
	DllAddRef();
	this->comm = new UDPComm(41337);
	this->comm->addListener(this);
	ZeroMemory(_rgpCredentials, sizeof(_rgpCredentials));
	Journal::log("Provider::Provider constructor\n");
}

NFCProvider::~NFCProvider()
{
	Journal::log("Provider::Provider destructor\n");
	for (size_t i = 0; i < _dwNumCreds; i++)
	{
		if (_rgpCredentials[i] != NULL)
		{
			_rgpCredentials[i]->Release();
		}
	}

	DllRelease();
	delete this->comm;
}


HANDLE hThread;
bool NFCThreadRunning = true;
NFCProvider *prp;
bool doAutoLogin = false;

void NFCProvider::onDataRecieved(CommResponse& response)
{
	Journal::log("Provider::Data received: " + string(response.getData()) + "\n");
	if (string(response.getData()) == "unlock")
	{
		prp->_dwNumCreds -= 1;

		wchar_t* username = wcs2cs(s2ws(std::string("")));
		wchar_t* password = wcs2cs(s2ws(std::string("")));
		wchar_t* domain = wcs2cs(s2ws(std::string("")));

		NFCCredential::lastLoginFailed = false;
		doAutoLogin = true;
		
		prp->_EnumerateOneCredential(prp->_dwNumCreds, username, password, domain);
		prp->Pcpe->CredentialsChanged(prp->UpAdviseContext);
	}
	else
	{
		Journal::log("Provider::Can't unlock\n");
	}
	
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
		Journal::log("Provider::Set usage KerbInteractiveLogon\n");
		break;
	case CPUS_UNLOCK_WORKSTATION:    
		NFCCredential::lockType = KerbWorkstationUnlockLogon;
		Journal::log("Provider::Set usage KerbWorkstationUnlockLogon\n");
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

	Journal::log("Provider::SetSerialization\n");

	return E_NOTIMPL;
}

ICredentialProviderEvents* Pcpe;
UINT_PTR UpAdviseContext;

HRESULT NFCProvider::Advise(
	ICredentialProviderEvents* pcpe,
	UINT_PTR upAdviseContext
	)
{
	Journal::log("Provider::advise\n");
	this->comm->start();

	Pcpe=pcpe;
	Pcpe->AddRef();
	UpAdviseContext=upAdviseContext;

	return S_OK;

}

HRESULT NFCProvider::UnAdvise()
{
	Journal::log("Provider::Unadvise\n");
	this->comm->close();

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
	Journal::log("Provider::Field count\n");
	*pdwCount = SFI_NUM_FIELDS;

	return S_OK;
}

HRESULT NFCProvider::GetFieldDescriptorAt(
	DWORD dwIndex, 
	CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR** ppcpfd
	)
{   
	std::stringstream ss;
	ss << dwIndex;
	Journal::log("Provider::Get field at" + ss.str() + "\n");
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

	Journal::log("Provider::Credential count\n");
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
	std::stringstream ss;
	ss << dwIndex;
	Journal::log("Provider::GetCredentialAt" + ss.str() + "\n");

	HRESULT hr;

	if((dwIndex < _dwNumCreds) && ppcpc)
	{
		hr = _rgpCredentials[dwIndex]->QueryInterface(IID_ICredentialProviderCredential, reinterpret_cast<void**>(ppcpc));
	}
	else
	{

		Journal::log("Provider::can't get credentails at" + ss.str() + "\n");
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
	Journal::log("Provider::_EnumerateOneCredential\n");
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
	Journal::log("Provider::_EnumerateCredentials");
	HRESULT hr = _EnumerateOneCredential(0, NULL, NULL, NULL);

	return hr;
}
HRESULT NFCProvider_CreateInstance(REFIID riid, void** ppv)
{
	Journal::log("Provider::NFCProvider_CreateInstance\n");
	HRESULT hr;

	NFCProvider* pProvider = new NFCProvider();

	if (pProvider)
	{
		hr = pProvider->QueryInterface(riid, ppv);
		pProvider->Release();
	}
	else
	{
		Journal::log("failed\n");
		hr = E_OUTOFMEMORY;
	}

	return hr;
}
