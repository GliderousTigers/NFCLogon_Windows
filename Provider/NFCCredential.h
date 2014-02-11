/** @file NFCCredential.h
Defines the behavior of a credential Tile : UI, user input, etc.
@date 8/2/2014
@version 0.1
*/

#pragma once

#include <windows.h>
#include <credentialprovider.h>
#include "helpers.h"
#include "dll.h"
#include "resource.h"

class NFCCredential : public ICredentialProviderCredential
{
public:
	STDMETHOD_(ULONG, AddRef)()
	{
		return _cRef++;
	}

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
	* @copydoc NFCProvider::QueryInterface()
	*/
	STDMETHOD (QueryInterface)(REFIID riid, void** ppv)
	{
		HRESULT hr;
		if (ppv != NULL)
		{
			if (IID_IUnknown == riid ||
				IID_ICredentialProviderCredential == riid)
			{
				*ppv = static_cast<IUnknown*>(this);
				reinterpret_cast<IUnknown*>(*ppv)->AddRef();
				hr = S_OK;
			}
			else
			{
				*ppv = NULL;
				hr = E_NOINTERFACE;
			}
		}
		else
		{
			hr = E_INVALIDARG;
		}
		return hr;
	}
public:
	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776019(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP Advise(ICredentialProviderCredentialEvents* pcpce);

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776036(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP UnAdvise();

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776034(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP SetSelected(BOOL* pbAutoLogon);

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776033(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP SetDeselected();

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776025(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP GetFieldState(DWORD dwFieldID,
		CREDENTIAL_PROVIDER_FIELD_STATE* pcpfs,
		CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE* pcpfis);

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776027(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP GetStringValue(DWORD dwFieldID, PWSTR* ppwsz);

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776021(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP GetBitmapValue(DWORD dwFieldID, HBITMAP* phbmp);

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776022(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP GetCheckboxValue(DWORD dwFieldID, BOOL* pbChecked, PWSTR* ppwszLabel);

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776024(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP GetComboBoxValueCount(DWORD dwFieldID, DWORD* pcItems, DWORD* pdwSelectedItem);

	/**<a href="externalfile.html">Microsoft doc</a>
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776023(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP GetComboBoxValueAt(DWORD dwFieldID, DWORD dwItem, PWSTR* ppwszItem);

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776028(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP GetSubmitButtonValue(DWORD dwFieldID, DWORD* pdwAdjacentTo);

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776035(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP SetStringValue(DWORD dwFieldID, PCWSTR pwz);

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776031(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP SetCheckboxValue(DWORD dwFieldID, BOOL bChecked);

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776032(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP SetComboBoxSelectedValue(DWORD dwFieldID, DWORD dwSelectedItem);

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776020(v=vs.85).aspx">Microsoft doc</a>
	*/
	IFACEMETHODIMP CommandLinkClicked(DWORD dwFieldID);

	/**
	*  Collect the username and password into a serialized credential for the correct usage scenario 
	* (logon/unlock).  LogonUI then passes these credentials 
	* back to the system to log on.
	*/
	IFACEMETHODIMP GetSerialization(
		CREDENTIAL_PROVIDER_GET_SERIALIZATION_RESPONSE* pcpgsr, 
		CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcs, 
		PWSTR* ppwszOptionalStatusText, 
		CREDENTIAL_PROVIDER_STATUS_ICON* pcpsiOptionalStatusIcon);

	/**
	* Not implemented
	*/
	IFACEMETHODIMP ReportResult(
		NTSTATUS ntsStatus, 
		NTSTATUS ntsSubstatus,
		PWSTR* ppwszOptionalStatusText, 
		CREDENTIAL_PROVIDER_STATUS_ICON* pcpsiOptionalStatusIcon);

public:
	/**
	* Initializes one credential with the field information passed in.
	*/
	HRESULT Initialize(
		const CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR* rgcpfd,
		const FIELD_STATE_PAIR* rgfsp,
		PWSTR pwzUsername,
		PWSTR pwzPassword = NULL,
		PWSTR pwzDomain = NULL);

	NFCCredential();

	virtual ~NFCCredential();

	/**
	* Boolean to keep track of the failed logins.
	*/
	static bool lastLoginFailed;

	/**
	* Determines in wich scenario 
	*/
	static KERB_LOGON_SUBMIT_TYPE lockType;

	LONG                                  _cRef;

	/**
	* An array holding the type and 
	* name of each field in the tile.
	*/
	CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR  _rgCredProvFieldDescriptors[SFI_NUM_FIELDS];
	/**
	* An array holding the state of 
	* each field in the tile.
	*/
	FIELD_STATE_PAIR                      _rgFieldStatePairs[SFI_NUM_FIELDS];

	/**
	* An array holding the string value of each field. This is 
	* different from the name of the field held in _rgCredProvFieldDescriptors
	*/
	PWSTR                                 _rgFieldStrings[SFI_NUM_FIELDS];         

	/**
	* <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/bb776010(v=vs.85).aspx">Microsoft doc</a>
	*/                                                                                 
	ICredentialProviderCredentialEvents* _pCredProvCredentialEvents;

	PWSTR username;
	PWSTR password;
	PWSTR domain;

};