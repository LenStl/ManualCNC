#include "pch.h"
#include <cstdio>
#include <windows.h>
#include "stdlib.h"
#include <array>

constexpr double TWO_PI = 6.2831855;

using namespace System;
using namespace De::Boenigk::SMC5D::Settings;
using namespace De::Boenigk::SMC5D::Basics;
using namespace De::Boenigk::SMC5D::Move;

bool initConnector(Connector^ connector, System::String^ config_loc);
void waitForReady(Reference^ reference);
void waitForReady(Connector^ connector);
Job^ generateCircleJob(float centerX, float centerY, float radius, float height, float speed, unsigned int divisions, Connector^ connector);

int main(array<System::String ^> ^args)
{
    printf(" ------------------------------------------------\n");
    printf("| An attempt at using smc5d.core.dll from C++.   |\n");
    printf(" ------------------------------------------------\n\n");

    /* Initialize Connector. */
    Connector^ myConnector = gcnew Connector(".\\");
    if (!initConnector(myConnector, ".\\config_Haase1290Expert.xml")) return false;

    /* Do reference run. */
    printf("[!] Doing reference run.\n");
    Reference^ myReference = gcnew Reference(myConnector);
    myReference->Start();
    waitForReady(myReference);
    printf("[+] Done.\n");

    /* Position roughly in the middle. */
    printf("[!] Positioning for safe testing.\n");
    ManualMove^ myManualMove = gcnew ManualMove(myConnector);
    myManualMove->StepXY(true, true, 30.0f);
    Sleep(5000);
    myManualMove->Stop();
    waitForReady(myConnector);
    printf("[+] Done.\n");

    // Manipulate with Key Inputs. Speed control via SpeedTact. Job also has .Pause and .Continue, but dont work properly.
    /* Setup Job. */
    Job^ job = generateCircleJob(40.0f, 40.0f, 15.0f, 100.0f, 30.0f, 20, myConnector);
    //float step_speed = 20.0f;
    //float glob_z = 100.0f;
    //StepList^ stepList = gcnew StepList(smcSettings);
    //stepList->AddXYZ(0.0f, 0.0f, glob_z, step_speed);
    //stepList->AddXYZ(50.0f, 0.0f, glob_z, step_speed);
    //stepList->AddXYZ(50.0f, 50.0f, glob_z, step_speed);
    //stepList->AddXYZ(0.0f, 50.0f, glob_z, step_speed);
    //stepList->AddXYZ(0.0f, 0.0f, glob_z, step_speed);

    SpeedTact^ speedTact = gcnew SpeedTact(myConnector);
    speedTact->Tact(100);   // Job/ManualMove Speed: 100%
    speedTact->Tact(0);   // Job/ManualMove Speed:   0%
    job->Start(100);

    printf("\n[+] OK!!\n");

    return true;
}

bool initConnector(Connector^ connector, System::String^ config_loc)
{
    if (!(connector->Load(config_loc)))
    {
        printf("[-] Config loading failed.\n");
        return false;
    }
    printf("[!] Configuration loaded. Trying to connect .");

    SMCStatus result = connector->Enable();

    int timeout = 5000;
    while (connector->Status != SMCStatus::Ready && timeout > 0)
    {
        Sleep(50);
        timeout -= 50;
        printf(".");
    }
    if (connector->Status != SMCStatus::Ready)
    {
        printf("\n[-] Timeout with error %d.\n", connector->Status);
        return false;
    }
    printf("\n[+] Connection successfully established with %s.\n", connector->GetControllerName());
    return true;
}
void waitForReady(Reference^ reference)
{
    while (reference->Status != SMCStatus::Ready)
    {
        Sleep(1);
    }
}
void waitForReady(Connector^ connector)
{
    while (connector->Status != SMCStatus::Ready)
    {
        Sleep(1);
    }
}
Job^ generateCircleJob(float centerX, float centerY, float radius, float height, float speed, unsigned int divisions, Connector^ connector)
{
    StepList^ stepList = gcnew StepList(gcnew SMCSettings());

    for (unsigned int i = 0; i < divisions; i++)
    {
        float radiant = (i * TWO_PI) / divisions;
        stepList->AddXYZ(centerX + radius * cos(radiant), centerY + radius * sin(radiant), height, speed);
    }
    stepList->AddXYZ(centerX + radius, centerY, height, speed);

    Job^ job = gcnew Job(connector);
    job->SetStepList(stepList->List);

    return job;
}

