#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include "CService.h"

#ifdef UPDATE_DEAMON
#define SERVICE_NAME _T("Ambition AutoUpdate Deamon")
#else
#define SERVICE_NAME _T("Ambition Auto Update")
#endif // UPDATE_DEAMON



int _tmain(int argc, _TCHAR* argv[])
{
	CService server(SERVICE_NAME);
	return server.ProcessMain(argc, argv);
}
