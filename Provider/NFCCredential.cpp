#ifndef WIN32_NO_STATUS
#include <ntstatus.h>
#define WIN32_NO_STATUS
#endif

#include <tchar.h>
#include "NFCCredential.h"
#include "guid.h"
#include <iostream>
#include <ntstatus.h>
#include <subauth.h>

extern HINSTANCE hInstance;
typedef int (WINAPI * PFRUNGATEFRAMEWORKPWDRESET) (HWND, PWSTR, PWSTR, PWSTR);


NFCCredential::NFCCredential():
	_cRef(1),
	_pCredProvCredentialEvents(NULL)
{
	DllAddRef();

	ZeroMemory(_rgCredProvFieldDescriptors, sizeof(_rgCredProvFieldDescriptors));
	ZeroMemory(_rgFieldStatePairs, sizeof(_rgFieldStatePairs));
	ZeroMemory(_rgFieldStrings, sizeof(_rgFieldStrings));
}

NFCCredential::~NFCCredential()
{
	for (int i = 0; i < ARRAYSIZE(_rgFieldStrings); i++)
	{
		CoTaskMemFree(_rgFieldStrings[i]);
		CoTaskMemFree(_rgCredProvFieldDescriptors[i].pszLabel);
	}

	DllRelease();
}

bool NFCCredential::lastLoginFailed;
KERB_LOGON_SUBMIT_TYPE NFCCredential::lockType;

HRESULT NFCCredential::Initialize(
	const CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR* rgcpfd,
	const FIELD_STATE_PAIR* rgfsp,
	PWSTR pwzUsername,
	PWSTR pwzPassword,
	PWSTR pwzDomain
	)
{

	UNREFERENCED_PARAMETER(pwzPassword);

	HRESULT hr = S_OK;

	for (DWORD i = 0; SUCCEEDED(hr) && i < ARRAYSIZE(_rgCredProvFieldDescriptors); i++)
	{
		_rgFieldStatePairs[i] = rgfsp[i];
		hr = FieldDescriptorCopy(rgcpfd[i], &_rgCredProvFieldDescriptors[i]);
	}

	username = pwzUsername;
	password = pwzPassword;
	domain = pwzDomain;
	LPWSTR loginString;
	if(username && domain){
		loginString = new wchar_t[10 + wcslen(username) + wcslen(password)];
		wsprintf(loginString, L"Login as %s\\%s", domain, username);
	}
	if (SUCCEEDED(hr))
	{
		hr = SHStrDupW(L"NFCLogon", &_rgFieldStrings[SFI_TITLE]);
	}
	if (SUCCEEDED(hr))
	{
		if(!username){
			hr = SHStrDupW(L"Swipe your tag with the cellphone.", &_rgFieldStrings[SFI_DETAILS]);
		}else{
			hr = SHStrDupW(loginString, &_rgFieldStrings[SFI_DETAILS]);
		}
	}
	if (SUCCEEDED(hr))
	{
		if(!username){
			hr = SHStrDupW(L"Swipe your tag", &_rgFieldStrings[SFI_MINIDETAILS]);
		}else{
			hr = SHStrDupW(loginString, &_rgFieldStrings[SFI_MINIDETAILS]);
		}
	}

	return S_OK;
}

HRESULT NFCCredential::Advise(
	ICredentialProviderCredentialEvents* pcpce
	)
{
	if (_pCredProvCredentialEvents != NULL)
	{
		_pCredProvCredentialEvents->Release();
	}
	_pCredProvCredentialEvents = pcpce;
	_pCredProvCredentialEvents->AddRef();

	return S_OK;
}

HRESULT NFCCredential::UnAdvise()
{
	if (_pCredProvCredentialEvents)
	{
		_pCredProvCredentialEvents->Release();
	}
	_pCredProvCredentialEvents = NULL;

	return S_OK;
}

HRESULT NFCCredential::SetSelected(BOOL* pbAutoLogon)  
{
	if(username && password && domain && !this->lastLoginFailed){
		*pbAutoLogon = TRUE;
	}else{
		*pbAutoLogon = FALSE;

		if(this->lastLoginFailed){
			if(_pCredProvCredentialEvents != NULL){
				_pCredProvCredentialEvents->SetFieldString(this, SFI_MINIDETAILS, L"Login credentials were invalid.");
				_pCredProvCredentialEvents->SetFieldString(this, SFI_DETAILS, L"Login details were invalid. You will need to login with a different credential to update them.");
				_pCredProvCredentialEvents->SetFieldBitmap(this, SFI_TILEIMAGE, LoadBitmap(HINST_THISDLL, MAKEINTRESOURCE(104)));
			}
		}

	}

	return S_OK;
}

HRESULT NFCCredential::SetDeselected()
{
	HRESULT hr = S_OK;
	return hr;
}

HRESULT NFCCredential::GetFieldState(
	DWORD dwFieldID,
	CREDENTIAL_PROVIDER_FIELD_STATE* pcpfs,
	CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE* pcpfis
	)
{
	HRESULT hr;

	if ((dwFieldID < ARRAYSIZE(_rgFieldStatePairs)) && pcpfs && pcpfis)
	{
		*pcpfs = _rgFieldStatePairs[dwFieldID].cpfs;
		*pcpfis = _rgFieldStatePairs[dwFieldID].cpfis;

		hr = S_OK;
	}
	else
	{
		hr = E_INVALIDARG;
	}

	return hr;
}

HRESULT NFCCredential::GetStringValue(
	DWORD dwFieldID, 
	PWSTR* ppwsz
	)
{
	HRESULT hr;

	if (dwFieldID < ARRAYSIZE(_rgCredProvFieldDescriptors) && ppwsz) 
	{
		hr = SHStrDupW(_rgFieldStrings[dwFieldID], ppwsz);
	}
	else
	{
		hr = E_INVALIDARG;
	}

	return hr;
}

HRESULT NFCCredential::GetBitmapValue(
	DWORD dwFieldID, 
	HBITMAP* phbmp
	)
{
	HRESULT hr;

	if ((SFI_TILEIMAGE == dwFieldID) && phbmp)
	{
		HBITMAP hbmp;

		if(username && password && domain && !this->lastLoginFailed){
			hbmp = LoadBitmap(HINST_THISDLL, MAKEINTRESOURCE(102));
		}else{
			hbmp = LoadBitmap(HINST_THISDLL, MAKEINTRESOURCE(101));
		}

		if (hbmp != NULL)
		{
			hr = S_OK;
			*phbmp = hbmp;
		}
		else
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
	}
	else
	{
		hr = E_INVALIDARG;
	}

	return hr;
}

HRESULT NFCCredential::GetSubmitButtonValue(
	DWORD dwFieldID,
	DWORD* pdwAdjacentTo
	)
{
	return E_INVALIDARG;
}

HRESULT NFCCredential::SetStringValue(
	DWORD dwFieldID, 
	PCWSTR pwz      
	)
{
UNREFERENCED_PARAMETER(dwFieldID);
	UNREFERENCED_PARAMETER(dwFieldID);
	UNREFERENCED_PARAMETER(pwz);

	return E_NOTIMPL;
}

HRESULT NFCCredential::GetCheckboxValue(
	DWORD dwFieldID, 
	BOOL* pbChecked,
	PWSTR* ppwszLabel
	)
{
	UNREFERENCED_PARAMETER(dwFieldID);
	UNREFERENCED_PARAMETER(pbChecked);
	UNREFERENCED_PARAMETER(ppwszLabel);

	return E_NOTIMPL;
}

HRESULT NFCCredential::GetComboBoxValueCount(
	DWORD dwFieldID, 
	DWORD* pcItems, 
	DWORD* pdwSelectedItem
	)
{
	UNREFERENCED_PARAMETER(dwFieldID);
	UNREFERENCED_PARAMETER(pcItems);
	UNREFERENCED_PARAMETER(pdwSelectedItem);

	return E_NOTIMPL;
}

HRESULT NFCCredential::GetComboBoxValueAt(
	DWORD dwFieldID, 
	DWORD dwItem,
	PWSTR* ppwszItem
	)
{
	UNREFERENCED_PARAMETER(dwFieldID);
	UNREFERENCED_PARAMETER(dwItem);
	UNREFERENCED_PARAMETER(ppwszItem);

	return E_NOTIMPL;
}

HRESULT NFCCredential::SetCheckboxValue(
	DWORD dwFieldID, 
	BOOL bChecked
	)
{
	UNREFERENCED_PARAMETER(dwFieldID);
	UNREFERENCED_PARAMETER(bChecked);

	return E_NOTIMPL;
}

HRESULT NFCCredential::SetComboBoxSelectedValue(
	DWORD dwFieldId,
	DWORD dwSelectedItem
	)
{
	UNREFERENCED_PARAMETER(dwFieldId);
	UNREFERENCED_PARAMETER(dwSelectedItem);

	return E_NOTIMPL;
}

HRESULT NFCCredential::CommandLinkClicked(DWORD dwFieldID)
{
	UNREFERENCED_PARAMETER(dwFieldID);

	return E_NOTIMPL;
}

HRESULT NFCCredential::GetSerialization(
	CREDENTIAL_PROVIDER_GET_SERIALIZATION_RESPONSE* pcpgsr,
	CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcs, 
	PWSTR* ppwzOptionalStatusText, 
	CREDENTIAL_PROVIDER_STATUS_ICON* pcpsiOptionalStatusIcon
	)
{

	UNREFERENCED_PARAMETER(ppwzOptionalStatusText);
	UNREFERENCED_PARAMETER(pcpsiOptionalStatusIcon);

	KERB_INTERACTIVE_LOGON kil;
	ZeroMemory(&kil, sizeof(kil));

	HRESULT hr;

	this->lastLoginFailed = true;

	WCHAR wsz[MAX_COMPUTERNAME_LENGTH+1];
	DWORD cch = ARRAYSIZE(wsz);
	GetComputerNameW(wsz, &cch);
	hr = UnicodeStringInitWithString(domain, &kil.LogonDomainName);
	hr = UnicodeStringInitWithString(username, &kil.UserName);
	hr = UnicodeStringInitWithString(password, &kil.Password);

			if (SUCCEEDED(hr))
			{
				kil.MessageType = lockType;
				hr = KerbInteractiveLogonPack(kil, &pcpcs->rgbSerialization, &pcpcs->cbSerialization);

				if (SUCCEEDED(hr))
				{
					ULONG ulAuthPackage;
					hr = RetrieveNegotiateAuthPackage(&ulAuthPackage);
					if (SUCCEEDED(hr))
					{
						pcpcs->ulAuthenticationPackage = ulAuthPackage;
						pcpcs->clsidCredentialProvider = CLSID_NFCProvider;

						*pcpgsr = CPGSR_RETURN_CREDENTIAL_FINISHED;
					}
				}
			}
			else
			{
				DWORD dwErr = GetLastError();
				hr = HRESULT_FROM_WIN32(dwErr);
			}

	return hr;
}

HRESULT NFCCredential::ReportResult(
	NTSTATUS ntsStatus, 
	NTSTATUS ntsSubstatus,
	PWSTR* ppwszOptionalStatusText, 
	CREDENTIAL_PROVIDER_STATUS_ICON* pcpsiOptionalStatusIcon
	)
{
	UNREFERENCED_PARAMETER(ntsSubstatus);
	UNREFERENCED_PARAMETER(ppwszOptionalStatusText);
	UNREFERENCED_PARAMETER(pcpsiOptionalStatusIcon);
	
	return E_NOTIMPL;
}
