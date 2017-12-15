#include <windows.h>
#include <shlwapi.h>

#include <stdio.h>

/* */

typedef HRESULT WINAPI (*WslRegisterDistribution_t)(PCWSTR, PCWSTR);

static int wsl_register(PCWSTR name, PCWSTR root)
{
	int code = 1;

	HMODULE hWSL = LoadLibraryW(L"wslapi.dll");
	if (hWSL != NULL) {
		WslRegisterDistribution_t pfn = (WslRegisterDistribution_t) GetProcAddress(hWSL, "WslRegisterDistribution");
		if (pfn != NULL) {
			HRESULT hr = pfn(name, root);
			if (FAILED(hr)) {
				fwprintf_s(stderr, L"Error: 0x%08X\n", hr);
				code = (int) hr;
			} else {
				code = 0;
			}
		}

		FreeLibrary(hWSL);
	}

	return code;
}

/* */

int main()
{
	int code = 1;

	WCHAR name[MAX_PATH];
	WCHAR tarp[MAX_PATH];
	PCWSTR distro;

	if (GetModuleFileNameW(NULL, name, MAX_PATH) == 0) {
		return 1;
	}

	if (!PathRemoveFileSpecW(name)) {
		return 1;
	}

	if (lstrcpynW(tarp, name, MAX_PATH) == NULL) {
		return 1;
	}

	if (!PathAppendW(tarp, L"rootfs.tar.gz")) {
		return 1;
	}

	distro = PathFindFileNameW(name);
	if (distro == NULL) {
		return 1;
	}

	fwprintf_s(stderr, L"distro: %s\n", distro);
	fwprintf_s(stderr, L"rootfs: %s\n", tarp);

	code = wsl_register(distro, tarp);
	if (code != 0) {
		MessageBoxW(NULL, L"Error!", L"WslRegisterDistribution", MB_OK);
	}

	return code;
}

