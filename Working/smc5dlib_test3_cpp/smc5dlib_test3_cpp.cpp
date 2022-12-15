#include "pch.h"
#include <cstdio>
#include <windows.h>
#include "stdlib.h"
#include <array>

using namespace System;
using namespace De::Boenigk::SMC5D::Settings;
using namespace De::Boenigk::SMC5D::Basics;
using namespace De::Boenigk::SMC5D::Move;

// Add "waitForReady(connector, etc.)" function that sleeps until status is ready

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

    // Create Reference, SpeedTact, Job with Steps from 50-100, z = 100, speed, about 20, 30


    // Do Reference
    // Do Positioning (with Manual Move and Sleep)
    // Create Job with z= 100, move speed = 20, square between 50 - 100
    // Manipulate with Key Inputs. Speed control via SpeedTact. Job also has .Pause and .Continue, but dont work properly.

    myManualMove->Step(MoveAxis::Y, false, 10.0f);
    Sleep(5000);
    myManualMove->Stop();

    job->Continue(100);

    printf("\nOK!!\n");

    return true;
}
