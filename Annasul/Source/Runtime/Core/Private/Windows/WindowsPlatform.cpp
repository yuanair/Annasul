#include "Windows/WindowsPlatform.hpp"

#include "Windows/Windows.hpp"
#include "Container/StringView.hpp"

namespace Annasul
{
	
	
	bool FWindowsPlatform::IsDarkMode()
	{
		uint32 value;
		ReadFromRegedit(
			KeyCurrentUser, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"),
			TEXT("AppsUseLightTheme"),
			value
		);
		return value == 0;
	}
	
	bool FWindowsPlatform::IsLightMode()
	{
		uint32 value;
		ReadFromRegedit(
			KeyCurrentUser, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"),
			TEXT("AppsUseLightTheme"),
			value
		);
		return value == 1;
	}
	
	bool FWindowsPlatform::IsUserAnAdmin()
	{
		return ::IsUserAnAdmin();
	}
	
	bool FWindowsPlatform::ReadFromRegedit(Key hKey, FStringView subKey, FStringView subKeyValue, uint32 &value)
	{
		// HKEY openKey;
		DWORD valueSize;
		// if (::RegOpenKeyEx((HKEY) hKey, subKey.GetNullTerminatedData(), 0, KEY_READ, &openKey) != ERROR_SUCCESS) return false;
		if (::RegGetValue((HKEY) hKey, subKey.GetNullTerminatedData(), subKeyValue.GetNullTerminatedData(),
		                  RRF_RT_REG_DWORD, nullptr, &value, &valueSize
		) !=
		    ERROR_SUCCESS)
			return false;
		//::RegCloseKey((HKEY) openKey);
		return true;
	}

//	bool
//	FWindowsPlatform::ReadFromRegedit(Key hKey, FStringView subKey, FStringView subKeyValue, StringW &value)
//	{
//		DWORD valueSize;
//		if (::RegGetValue((HKEY) hKey, subKey.GetNullTerminatedData(), subKeyValue.GetNullTerminatedData(), RRF_RT_REG_SZ, nullptr, nullptr, &valueSize
//		) !=
//		    ERROR_SUCCESS)
//			return false;
//		value.resize(valueSize / sizeof(FStringView::ElementType) - 2);
//		if (::RegGetValue((HKEY) hKey, subKey.GetNullTerminatedData(), subKeyValue.GetNullTerminatedData(), RRF_RT_REG_SZ, nullptr, &value[0], &valueSize
//		) !=
//		    ERROR_SUCCESS)
//			return false;
//		return true;
//	}
	
	bool
	FWindowsPlatform::WriteToRegedit(Key hKey, FStringView subKey, FStringView subKeyValue, FStringView value)
	{
		HKEY createKey;
		if (::RegCreateKeyEx(
			(HKEY) hKey,
			subKey.GetNullTerminatedData(),
			0,
			nullptr,
			REG_OPTION_NON_VOLATILE,
			KEY_WRITE,
			nullptr,
			&createKey,
			nullptr
		) != ERROR_SUCCESS)
			return false;
		//
		if (::RegSetValueEx(
			createKey,
			subKeyValue.GetNullTerminatedData(),
			0,
			REG_SZ,
			reinterpret_cast<LPBYTE>(const_cast<TCHAR *>(value.GetNullTerminatedData())),
			sizeof(FStringView::ElementType) * value.GetSize()) != ERROR_SUCCESS)
			return false;
		::RegCloseKey(createKey);
		return true;
	}
	
	bool FWindowsPlatform::DeleteKeyInRegedit(Key hKey, FStringView subKey)
	{
		return ::RegDeleteKeyEx((HKEY) hKey, subKey.GetNullTerminatedData(), 0, 0) == ERROR_SUCCESS;
	}
	
	bool FWindowsPlatform::DeleteKeyValueInRegedit(Key hKey, FStringView subKey, FStringView subKeyValue)
	{
		return ::RegDeleteKeyValue((HKEY) hKey, subKey.GetNullTerminatedData(), subKeyValue.GetNullTerminatedData()) ==
		       ERROR_SUCCESS;
	}
	
	bool FWindowsPlatform::SetBootStartUp(FStringView key, FStringView command)
	{
		return WriteToRegedit(KeyCurrentUser, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), key, command);
	}

//	bool FWindowsPlatform::GetBootStartUp(FStringView key, StringW &command)
//	{
//		return ReadFromRegedit(KeyCurrentUser, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run", key, command);
//	}
	
	bool FWindowsPlatform::DeleteBootStartUp(FStringView key)
	{
		return DeleteKeyValueInRegedit(KeyCurrentUser, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), key);
	}
	
}
