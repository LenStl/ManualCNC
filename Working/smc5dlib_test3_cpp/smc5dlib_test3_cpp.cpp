#include "pch.h"
#include <cstdio>
#include <windows.h>
#include "stdlib.h"
#include <array>

using namespace System;
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
        _sleep(50);
        timeout -= 50;
        printf(".");
    }
    if (myConnector->Status != SMCStatus::Ready)
    {
        printf("\nTimeout with error %d.\n", myConnector->Status);
        return false;
    }

    printf("\nOK!!\n");

    return true;

}
