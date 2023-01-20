#include "pch.h"
#include <cstdio>
#include <windows.h>
#include "stdlib.h"
#include "step_io.h"
#include <array>

#include <iostream>
#include <fstream>

using namespace System;
using namespace De::Boenigk::SMC5D::Settings;
using namespace De::Boenigk::SMC5D::Basics;
using namespace De::Boenigk::SMC5D::Move;

bool initConnector(Connector^ connector, System::String^ config_loc);

/* Halts program until given argument module is ready again. */
void waitForReady(Job^ job);
void waitForReady(Reference^ reference);
void waitForReady(Connector^ connector);

int main(array<System::String^>^ args)
{
    printf(" ------------------------------------------------\n");
    printf("| An attempt at using smc5d.core.dll from C++.   |\n");
    printf(" ------------------------------------------------\n\n");

    /* Initialize Connector. */
    Connector^ myConnector = gcnew Connector(".\\");
    //if (!initConnector(myConnector, ".\\config_Haase1290Expert.xml")) return false;

    StepList^ mySteps = StepIO::load_grf4("C:\\Projects\\C++ Projects\\ManualCNC\\Working\\smc5dlib_test3_cpp\\circle.grf4", 100.0f, 30.0f, myConnector->SMCSettings);
    Console::WriteLine(mySteps->List->Count);
    Console::ReadLine();
    return 0;

    SpeedTact^ mySpeedTact = gcnew SpeedTact(myConnector);
    mySpeedTact->Tact(100);       // Job/ManualMove Speed: 100%

    /* Do reference run. */
    printf("[!] Doing reference run.\n");
    Reference^ myReference = gcnew Reference(myConnector);
    myReference->Start();
    waitForReady(myReference);
    printf("[+] Done.\n");

    /* Initialize Utilities. */
    StepCalc^ myStepCalc = gcnew StepCalc();
    Switch^ mySwitch = gcnew Switch(myConnector);
    ManualMove^ myManualMove = gcnew ManualMove(myConnector);

    /* Setup Job. */
    StepList^ myJobStepList = gcnew StepList(myConnector->SMCSettings);
    //myJobStepList = StepIO::generate_circle(300.0f, 300.0f, 100.0f, 100.0f, 50.0f, 800, myConnector->SMCSettings); // Speed 30.0f smooth.
    //myJobStepList = StepIO::generate_line(150.0f, 150.0f, 400.0f, 400.0f, 100.0f, 30.0f, myConnector->SMCSettings);
    myJobStepList = StepIO::generate_square(100.0f, 100.0f, 300.0f, 300.0f, 100.0f, 50.0f, myConnector->SMCSettings);

    StepList^ currentJobStepList = gcnew StepList(myConnector->SMCSettings);
    for (int i = 0; i < myJobStepList->List->Count; i++)
        currentJobStepList->Add(myJobStepList->List[i]->GetCopy());

    waitForReady(myConnector);
    printf("[!] Starting job.\n");
    Job^ myJob = gcnew Job(myConnector);
    myJob->SetStepList(currentJobStepList->List);
    waitForReady(myJob);
    myJob->Start(100);
    /* Test Area */
    {
        bool isStopped = false;
        bool wasPressed = false;

        while (myJob->Status != SMCStatus::Ready)
        {
            if (GetKeyState(VK_SPACE) & 0x8000)
            {
                if (!wasPressed)
                {
                    wasPressed = true;
                    /* Stopping with setting pin. (Doesnt seem to work)*/
                    //if (!isStopped) mySwitch->SetPinPermanently(Output::Con2Pin8MotorCurrent, false);
                    //else mySwitch->SetPinPermanently(Output::Con2Pin8MotorCurrent, true);
                    /* Speed control with SpeedTact. (Slows down, also affects Manual Move, doesnt stop the job)*/
                    //if (!isStopped) speedTact->Tact(25);
                    //else speedTact->Tact(100);
                    /* Pausing and continuing job. (Continue doesnt seem to work.*/
                    //if (!isStopped) myJob->Pause();
                    //else myJob->Continue(255);


                    /* Abort/Pause job, do movement, calculate residual job and execute it.*/
                    int currentIndex = myJob->GetPosition();
                    //myJob->Pause();                               // Slow stopping, while still following path.
                    myJob->Abort();
                    //myJob->Dispose();
                    printf(" [!] Job interrupted at step %d.\n", currentIndex);
                    waitForReady(myConnector);
                    myManualMove->Step(MoveAxis::X, true, 0.0f);    // Somehow resets the connector from "Abort" to "Ready".
                    myManualMove->Stop();                           //
                    Sleep(2000);
                    myJob = gcnew Job(myConnector);
                    
                    if (currentIndex > 0)
                    {
                        currentIndex--; // Slight Non-Linearity in the step indices: GetPosition() == 0: Initializing, moving to the first coordinate. GetPosition() == 2: Going the first distance. GetPosition() == 1 doesnt exists.
                        StepList^ newJobStepList = gcnew StepList(myConnector->SMCSettings);
                        for (int i = 0; i < currentJobStepList->List->Count - currentIndex; i++)
                            newJobStepList->Add(currentJobStepList->List[i + currentIndex]->GetCopy());
                        currentJobStepList = newJobStepList;
                    }
                    myJob->SetStepList(currentJobStepList->List);
                    printf(" [!] Continuing job...\n");
                    waitForReady(myConnector);
                    waitForReady(myJob);
                    myJob->Start(100);
                    
                    wasPressed = false;
                }
            }
        }
    }
    waitForReady(myConnector);
    printf("[+] Job completed.\n");
    printf("\n[+] OK!!\n");

    return true;
}

void manualControl(float speed, ManualMove^ manualMove)
{
    // Space, Left, Up, Right, Down
    bool wasPressed[] = {false, false, false, false, false};
    while (!wasPressed[0])
    {
        if (GetKeyState(VK_SPACE) & 0x8000)
        {
            wasPressed[0] = false;
        }
        if (GetKeyState(VK_LEFT) & 0x8000)
        {
            if (!wasPressed[1])
            {
                wasPressed[1] = true;
            }
        }
        else
        {
            if (wasPressed)
            {
                wasPressed[1] = false;
                manualMove->Stop();
            }
        }
    }
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
void waitForReady(Job^ job)
{
    while (job->Status != SMCStatus::Ready)
    {
        Sleep(1);
    }
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