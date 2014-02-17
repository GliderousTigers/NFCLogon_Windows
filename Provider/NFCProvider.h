/** @file NFCProvider.h
This clases manages one or more Credentials.

@date 8/2/2014
@version 0.1
*/
#pragma once

#include <tchar.h>
#include <credentialprovider.h>
#include <strsafe.h>

#include "NFCCredential.h"
#include "helpers.h"
#include "Crypto.h"
#include "CommListener.h"
#include "Comm.h"
#include "UDPComm.h"

#define MAX_CREDENTIALS 1

class NFCProvider : public ICredentialProvider, public CommListener
{
public:
	/**
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/ms680509">Microsoft doc</a>
	*/
	STDMETHOD_(ULONG, AddRef)()
	{
		return _cRef++;
	}
	/**
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/ms680509">Microsoft doc</a>
	*/
	STDMETHOD_(ULONG, Release)()
	{
		LONG cRef = _cRef--;
		if (!cRef)
		{
			delete this;
		}
		return cRef;
	}
	/**
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/ms682521">Microsoft doc</a>
	*/
	STDMETHOD (QueryInterface)(REFIID riid, void** ppv)
	{
		HRESULT hr;
		if (IID_IUnknown == riid || 
			IID_ICredentialProvider == riid)
		{
			*ppv = this;
			reinterpret_cast<IUnknown*>(*ppv)->AddRef();
			hr = S_OK;
		}
		else
		{
			*ppv = NULL;
			hr = E_NOINTERFACE;
		}
		return hr;
	}

public:
	/**
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776044">Microsoft doc</a>
	*/
	IFACEMETHODIMP SetUsageScenario(CREDENTIAL_PROVIDER_USAGE_SCENARIO cpus, DWORD dwFlags);

	/**
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776043">Microsoft doc</a>
	*/
	IFACEMETHODIMP SetSerialization(const CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcs);

	/**
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776037">Microsoft doc</a>
	*/
	IFACEMETHODIMP Advise(__in ICredentialProviderEvents* pcpe, UINT_PTR upAdviseContext);

	/**
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776045">Microsoft doc</a>
	*/
	IFACEMETHODIMP UnAdvise();

	/**
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776045">Microsoft doc</a>
	*/
	IFACEMETHODIMP GetFieldDescriptorCount(__out DWORD* pdwCount);

	/**
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776040">Microsoft doc</a>
	*/
	IFACEMETHODIMP GetFieldDescriptorAt(DWORD dwIndex,  __deref_out CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR** ppcpfd);

	/**
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776039">Microsoft doc</a>
	*/
	IFACEMETHODIMP GetCredentialCount(
		__out DWORD* pdwCount,
		__out DWORD* pdwDefault,
		__out BOOL* pbAutoLogonWithDefault);

	/**
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776038">Microsoft doc</a>
	*/
	IFACEMETHODIMP GetCredentialAt(
		DWORD dwIndex, 
		__out ICredentialProviderCredential** ppcpc);

	/**
	* Creates a NFCProvider and retrieves pointers 
	* to the supported interfaces on an object and hads it's reference.
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/ms682521">Microsoft doc</a>
	*/
	friend HRESULT NFCProvider_CreateInstance(REFIID riid, __deref_out void** ppv);

	/**
	*  Sets up all the credentials for this provider.
	*/
	HRESULT _EnumerateCredentials();

	/**
	*  Releases all the credentials for this provider.
	*/
	void _ReleaseEnumeratedCredentials();

	/**
	* Creates a credential using the login informations
	*/
	HRESULT _EnumerateOneCredential(
		__in DWORD dwCredientialIndex,
		__in PWSTR pwzUsername,
		__in PWSTR pwzPassword,
		__in PWSTR pwzDomain
	);
	
	/*
	* The number of credentials enumerated
	*/
	DWORD	_dwNumCreds;

	/*
	* Pointer to an interface that manager events
	*  <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776007">Microsoft doc</a>
	*/
	ICredentialProviderEvents* Pcpe;

	UINT_PTR UpAdviseContext;

	virtual void onDataRecieved(CommResponse& response);
protected:
	NFCProvider();
	__override ~NFCProvider();
private:
	LONG              _cRef;

	/**
	* Pointers to the credentials which will be enumerated by the provider
	*/
	static NFCCredential* _rgpCredentials[MAX_CREDENTIALS]; 

	Comm* comm;
};