#include <QCoreApplication>
#include "DSApp.h"


#if 0
#include <windows.h>
#include <stdio.h>
static wchar_t ServiceName[] = L"DSService";
DSApp* gApp = NULL;

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;
void ServiceMain(int argc, char** argv);
void ControlHandler(DWORD request);

void ServiceMain(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    ServiceStatus.dwServiceType =
            SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
    ServiceStatus.dwCurrentState =
            SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted =
            SERVICE_ACCEPT_STOP;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 0;

    hStatus = RegisterServiceCtrlHandler(ServiceName,
                                          (LPHANDLER_FUNCTION)ControlHandler);
    if(hStatus == (SERVICE_STATUS_HANDLE)0)
    {
        qWarning() << "RegisterServiceCtrl Error";
        return;
    }

    // report the running status to SCM
    ServiceStatus.dwCurrentState =
            SERVICE_START_PENDING;
    SetServiceStatus(hStatus, &ServiceStatus);
    gApp = new DSApp;

    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &ServiceStatus);

    ServiceStatus.dwWin32ExitCode = app.exec();
    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(hStatus, &ServiceStatus);
}

void ServiceStop()
{
    // report the running status to SCM
    ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
    SetServiceStatus(hStatus, &ServiceStatus);
    if(gApp)
    {
        delete gApp;
        gApp = NULL;
    }

    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(hStatus, &ServiceStatus);
}

void ControlHandler(DWORD request)
{
    switch(request)
    {
    case SERVICE_CONTROL_STOP:
        ServiceStop();
    default:
        break;
    }
    return;
}
int main(int, char*[])
{
    SERVICE_TABLE_ENTRY ST[2];
    ST[0].lpServiceName = ServiceName;
    ST[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

    ST[1].lpServiceName = NULL;
    ST[1].lpServiceProc = NULL;

    StartServiceCtrlDispatcher(ST);
    return 0;
}
#else
int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    DSUtil::installDebug();

    new DSApp;
    return app.exec();
}
#endif
