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
void waitForReady(Job^ job);
void waitForReady(Reference^ reference);
void waitForReady(Connector^ connector);
StepList^ getResidualStepList(StepList^ stepList, int stepIndex, Connector^ connector);
StepList^ generateSquareStepList(float minX, float minY, float maxX, float maxY, float height, float speed, Connector^ connector);
StepList^ generateLineStepList(float startX, float startY, float endX, float endY, float height, float speed, Connector^ connector);
StepList^ generateCircleStepList(float centerX, float centerY, float radius, float height, float speed, unsigned int divisions, Connector^ connector);

int main(array<System::String^>^ args)
{
    printf(" ------------------------------------------------\n");
    printf("| An attempt at using smc5d.core.dll from C++.   |\n");
    printf(" ------------------------------------------------\n\n");

    /* Initialize Connector. */
    Connector^ myConnector = gcnew Connector(".\\");
    if (!initConnector(myConnector, ".\\config_Haase1290Expert.xml")) return false;
    SpeedTact^ speedTact = gcnew SpeedTact(myConnector);
    speedTact->Tact(100);       // Job/ManualMove Speed: 100%
    //speedTact->Tact(0);       // Job/ManualMove Speed:   0%

    /* Do reference run. */
    printf("[!] Doing reference run.\n");
    Reference^ myReference = gcnew Reference(myConnector);
    myReference->Start();
    waitForReady(myReference);
    printf("[+] Done.\n");

    /* Position roughly in the middle. */
    ManualMove^ myManualMove = gcnew ManualMove(myConnector);
    //printf("[!] Positioning for safe testing.\n");
    //myManualMove->StepXY(true, true, 150.0f);
    //Sleep(2000);
    //myManualMove->Stop();
    //waitForReady(myConnector);
    //printf("[+] Done.\n");

    /* Setup Job. */
    StepList^ myJobStepList;
    //myJobStepList = generateCircleStepList(300.0f, 300.0f, 100.0f, 100.0f, 50.0f, 40, myConnector);
    myJobStepList = generateSquareStepList(100.0f, 100.0f, 300.0f, 300.0f, 100.0f, 50.0f, myConnector);
    StepList^ currentJobStepList = gcnew StepList(myConnector->SMCSettings);
    for (int i = 0; i < myJobStepList->List->Count; i++)
        currentJobStepList->Add(myJobStepList->List[i]->GetCopy());

    //myJobStepList = generateLineStepList(150.0f, 150.0f, 400.0f, 400.0f, 100.0f, 30.0f, myConnector);
    
    waitForReady(myConnector);

    StepCalc^ stepCalc = gcnew StepCalc();
    Switch^ mySwitch = gcnew Switch(myConnector);

    printf("[!] Starting job.\n");
    Job^ myJob = gcnew Job(myConnector);
    myJob->SetStepList(currentJobStepList->List);
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
                    /* Pausing and continuing job .*/
                    //if (!isStopped) myJob->Pause();
                    //else myJob->Continue(255);
                    
                    /* Current: Abort job, do movement, calculate residual job and execute it.*/
                    int lastIndex = myJob->GetPosition();
                    myJob->Pause();

                    printf(" [!] Job interrupted.\n");
                    //waitForReady(myConnector);
                    //myManualMove->Step(MoveAxis::X, false, 30.0f);
                    Sleep(2000);
                    //myManualMove->Stop();
                    //waitForReady(myConnector);
                    printf(" [!] Continuing job...\n");

                    myJob = gcnew Job(myConnector);
                    
                    // TODO
                    currentJobStepList->List->RemoveRange(0, lastIndex);
                    //currentJobStepList = getResidualStepList(currentJobStepList, lastIndex, myConnector);
                    myJob->SetStepList(currentJobStepList->List);
                    waitForReady(myConnector);
                    
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

StepList^ getResidualStepList(StepList^ stepList, int currentStepIndex, Connector^ connector)
{
    // For doing relative offset later
    StepList^ resStepList = gcnew StepList(connector->SMCSettings);
    StepCalc^ stepCalc = gcnew StepCalc();
    for (int i = currentStepIndex; i < stepList->List->Count; i++)
        resStepList->Add(stepList->List[i]->GetCopy());

    //resStepList->List[0]->X = stepCalc->GetMMX(connector);
    //resStepList->List[0]->Y = stepCalc->GetMMY(connector);
    //resStepList->List[0]->Z = stepCalc->GetMMZ(connector);
    //resStepList->List[0]->A = stepCalc->GetMMA(connector);
    //resStepList->List[0]->B = stepCalc->GetMMB(connector);

    return resStepList;
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
StepList^ generateSquareStepList(float minX, float minY, float maxX, float maxY, float height, float speed, Connector^ connector)
{
    StepList^ stepList = gcnew StepList(connector->SMCSettings);

    stepList->AddXYZ(minX, minY, height, speed);
    stepList->AddXYZ(maxX, minY, height, speed);
    stepList->AddXYZ(maxX, maxY, height, speed);
    stepList->AddXYZ(minX, maxY, height, speed);
    stepList->AddXYZ(minX, minY, height, speed);

    return stepList;
}
StepList^ generateLineStepList(float startX, float startY, float endX, float endY, float height, float speed, Connector^ connector)
{
    StepList^ stepList = gcnew StepList(connector->SMCSettings);

    stepList->AddXYZ(startX, startY, height, speed);
    stepList->AddXYZ(endX, endY, height, speed);

    return stepList;
}
StepList^ generateCircleStepList(float centerX, float centerY, float radius, float height, float speed, unsigned int divisions, Connector^ connector)
{
    StepList^ stepList = gcnew StepList(connector->SMCSettings);

    for (unsigned int i = 0; i < divisions; i++)
    {
        float radiant = (i * TWO_PI) / divisions;
        stepList->AddXYZ(centerX + radius * cos(radiant), centerY + radius * sin(radiant), height, speed);
    }
    stepList->AddXYZ(centerX + radius, centerY, height, speed);

    return stepList;
}

