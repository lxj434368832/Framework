
#include <sapi.h>

#pragma comment(lib, "SAPI.lib")

int TestTextToSpeak()
{
	ISpVoice * pVoice = NULL;

	if (FAILED(::CoInitialize(NULL)))
		return -1;

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	if (SUCCEEDED(hr))
	{
		hr = pVoice->Speak(L"Hello world", 0, NULL);
		pVoice->Release();
		pVoice = NULL;
	}

	::CoUninitialize();
	system("pause");
	return 0;
}
