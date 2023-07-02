#pragma comment (lib, "Setupapi.lib")

#include <windows.h>
#include <setupapi.h>
#include <iostream>

int main() {
	GUID guid;
	HRESULT hResult =
		CLSIDFromString(
			L"{4d36e96b-e325-11ce-bfc1-08002be10318}",
			(LPCLSID) &guid);

	if (hResult == CO_E_CLASSSTRING) {
		std::cerr << "ERROR: Bad GUID string.\n";
		return EXIT_FAILURE;
	}

	HDEVINFO hDeviceInfo =
		SetupDiGetClassDevsExA(
			&guid,
			NULL,
			NULL,
			DIGCF_PRESENT,
			NULL,
			NULL,
			NULL);

	if (hDeviceInfo == INVALID_HANDLE_VALUE) {
		std::cerr << "ERROR: Could not obtain HDEVINFO.\n";
		return EXIT_FAILURE;
	}
	
	SP_DEVINFO_DATA deviceInfoData;
	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	deviceInfoData.ClassGuid = guid;
	
	BOOL deviceEnumerated = 
		SetupDiEnumDeviceInfo(
			hDeviceInfo, 
			0,
			&deviceInfoData);

	if (!deviceEnumerated) {
		std::cerr << "ERROR: Could not enumerate the SP_DEVINFO_DATA.\n";
		return EXIT_FAILURE;
	}

	BOOL removeStatus =
		SetupDiCallClassInstaller(
			DIF_REMOVE,
			hDeviceInfo, 
			&deviceInfoData);

	if (!removeStatus) {
		std::cerr << "ERROR: Could not remove the device.\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}