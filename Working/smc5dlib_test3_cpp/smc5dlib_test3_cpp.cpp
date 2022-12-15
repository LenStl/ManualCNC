#include "pch.h"
#include <cstdio>
#include <windows.h>
#include "stdlib.h"
#include <array>

using namespace System;
using namespace System::Collections::Generic;
using namespace De::Boenigk::SMC5D::Settings;
using namespace De::Boenigk::SMC5D::Basics;
using namespace De::Boenigk::SMC5D::Move;


int main(array<System::String ^> ^args)
{
    printf("An attempt at using smc5d.core.dll from C++.\n");
    Connector ^myConnector = gcnew Connector(".\\");
    if (!(myConnector->Load(".\\config_Haase1290Expert.xml")))
    {
        printf("Config loading failed.\n");
        return false;
    }

    ManualMove ^myManualMove = gcnew ManualMove(myConnector);

    printf("Configuration loaded. Trying to connect.\n");

    SMCStatus result = myConnector->Enable();

    int timeout = 5000;

    while (myConnector->Status != SMCStatus::Ready && timeout > 0)
    {
        Sleep(50);
        timeout -= 50;
        printf(".");
    }
    if (myConnector->Status != SMCStatus::Ready)
    {
        printf("\nTimeout with error %d.\n", myConnector->Status);
        return false;
    }

    // New
    // Job: Draw square with 10mm/s. After 2.5 seconds stop and do a manual move for 1 second. Then continue.
    SMCSettings ^smcSettings = gcnew SMCSettings();
    StepList ^stepList = gcnew StepList(smcSettings);
    stepList->AddXY( 0.0f,  0.0f, 10.0f);
    stepList->AddXY(100.0f,  0.0f, 10.0f);
    stepList->AddXY(100.0f, 100.0f, 10.0f);
    stepList->AddXY( 0.0f, 100.0f, 10.0f);
    stepList->AddXY( 0.0f, 00.0f, 10.0f);

    Job^ job = gcnew Job(myConnector);
    job->SetStepList(stepList->List);

    job->Start(100);        // Start job with 100% speed
    Sleep(2500);
    job->Pause();
    myManualMove->Step(MoveAxis::X, true, 10.0f);
    Sleep(1000);
    myManualMove->Stop();

    job->Continue(100);

    printf("\nOK!!\n");

    return true;
}
