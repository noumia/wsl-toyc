#include <windows.h>

static inline const WCHAR* lookup_tail(const WCHAR* args, const WCHAR* end)
{
	int st = 0;

	for (const WCHAR* p = args; p < end; p++) {
		WCHAR ch = *p;

		switch (st) {
		case 0:
			if (ch == L' ') {
				st = 2;
			} else if (ch == L'\"') {
				st = 1;
			}
			break;

		case 1:
			if (ch == L'\"') {
				st = 0;
			}
			break;

		case 2:
			if (ch != L' ') {
				return p;
			}
			break;
		}
	}

	return end;
}

static inline const WCHAR* lookup_s(const WCHAR* arg0, const WCHAR* end)
{
	const WCHAR* s = (*arg0 == L'\"') ? arg0 + 1 : arg0;

	for (const WCHAR* p = end - 1; p >= s; --p) {
		if (*p == '\\') {
			return p + 1;
		}
	}

	return s;
}

static inline const WCHAR* lookup_e(const WCHAR* s, const WCHAR* end)
{
	for (const WCHAR* p = s; p < end; p++) {
		if (*p == ' ' || *p == '.' || *p == '\"') {
			return p;
		}
	}

	return end;
}

typedef HRESULT WINAPI (*WslLaunchInteractive_t)(PCWSTR, PCWSTR, BOOL, DWORD*);

void _start(void)
{
	DWORD code = 1;
	WCHAR name[128];

	HMODULE hWSL = LoadLibraryW(L"wslapi.dll");
	if (hWSL != NULL) {
		WslLaunchInteractive_t pfn = (WslLaunchInteractive_t) GetProcAddress(hWSL, "WslLaunchInteractive");
		if (pfn != NULL) {
			const WCHAR* args = GetCommandLineW();
			SIZE_T       agsz = lstrlenW(args);
			const WCHAR* tail = lookup_tail(args, args + agsz);

			const WCHAR* ds = lookup_s(args, tail);
			const WCHAR* de = lookup_e(ds,   tail);

			if (de - ds + 1 <= sizeof(name) / sizeof(WCHAR)) {
				if (lstrcpynW(name, ds, de - ds + 1) != NULL) {
					HRESULT hr = pfn(
						name,
						tail,
						TRUE,
						&code);
					if (FAILED(hr)) {
						code = (DWORD) hr;
					}
				}
			}
		}

		FreeLibrary(hWSL);
	}

	ExitProcess(code);
}

