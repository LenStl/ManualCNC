#include "pch.h"
#include <cstdio>
#include <windows.h>
#include "stdlib.h"
#include <array>

#include <iostream>
#include <fstream>

constexpr double TWO_PI = 6.2831855;

using namespace System;
using namespace De::Boenigk::SMC5D::Settings;
using namespace De::Boenigk::SMC5D::Basics;
using namespace De::Boenigk::SMC5D::Move;

using namespace System::Xml;
using namespace System::Globalization;

bool initConnector(Connector^ connector, System::String^ config_loc);
/* Halts program until given argument module is ready again. */
void waitForReady(Job^ job);
void waitForReady(Reference^ reference);
void waitForReady(Connector^ connector);

StepList^ getResidualStepList(StepList^ stepList, int stepIndex, Connector^ connector);
StepList^ generateSquareStepList(float minX, float minY, float maxX, float maxY, float height, float speed, Connector^ connector);
StepList^ generateLineStepList(float startX, float startY, float endX, float endY, float height, float speed, Connector^ connector);
StepList^ generateCircleStepList(float centerX, float centerY, float radius, float height, float speed, unsigned int divisions, Connector^ connector);
StepList^ generateCircularTest(float centerX, float centerY, float radius, float height, float speed, Connector^ connector);

static int Reverse(int value)
{
    byte* p = (byte*)&value;
    return (*p << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}
int main(array<System::String^>^ args)
{
    // Reading .grf4 files. This can now read the files <Polygon>s.
    //XmlTextReader^ reader = gcnew XmlTextReader("C:\\Users\\CNC\\proj\\ManualCNC\\Working\\smc5dlib_test3_cpp\\circle.grf4");
    XmlTextReader^ reader = gcnew XmlTextReader("C:\\Projects\\C++ Projects\\ManualCNC\\Working\\smc5dlib_test3_cpp\\circle.grf4");
    Console::Write("[");
    while (reader->Read())
    {
        if (reader->Name->Equals("Polygon"))
        {
            reader->Read(); // Read in the next node.
            if (reader->Value->Length > 8)
            {
                String^ str = reader->Value->Replace(" ", "");

                String^ hex_x = str->Substring(0, 8);
                String^ hex_y = str->Substring(8, 8);
                // Be careful of endianess.
                unsigned int num = Reverse(UInt32::Parse(hex_y, System::Globalization::NumberStyles::AllowHexSpecifier));
                
                array<unsigned char>^ floatVals = BitConverter::GetBytes(num);
                float f = BitConverter::ToSingle(floatVals, 0);
                Console::WriteLine(f.ToString(CultureInfo::InvariantCulture) + ",");
            }
        }
    }
    Console::Write("]");
    Console::ReadLine();
    reader->Close();
    return 0;

    printf(" ------------------------------------------------\n");
    printf("| An attempt at using smc5d.core.dll from C++.   |\n");
    printf(" ------------------------------------------------\n\n");

    /* Initialize Connector. */
    Connector^ myConnector = gcnew Connector(".\\");
    if (!initConnector(myConnector, ".\\config_Haase1290Expert.xml")) return false;
    SpeedTact^ mySpeedTact = gcnew SpeedTact(myConnector);
    mySpeedTact->Tact(100);       // Job/ManualMove Speed: 100%
    //mySpeedTact->Tact(0);       // Job/ManualMove Speed:   0%

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

    /* Position roughly in the middle. */
    //printf("[!] Positioning for safe testing.\n");
    //myManualMove->StepXY(true, true, 150.0f);
    //Sleep(2000);
    //myManualMove->Stop();
    //waitForReady(myConnector);
    //printf("[+] Done.\n");

    /* Setup Job. */
    StepList^ myJobStepList = gcnew StepList(myConnector->SMCSettings);

    //myJobStepList = generateCircleStepList(300.0f, 300.0f, 100.0f, 100.0f, 50.0f, 800, myConnector); // Speed 30.0f smooth.
    myJobStepList = generateCircularTest(300.0f, 300.0f, 100.0f, 100.0f, 10.0f, myConnector);
    //myJobStepList = generateLineStepList(150.0f, 150.0f, 400.0f, 400.0f, 100.0f, 30.0f, myConnector);
    
    //myJobStepList = generateSquareStepList(100.0f, 100.0f, 300.0f, 300.0f, 100.0f, 50.0f, myConnector);

    //StepWriter^ myStepWriter = gcnew StepWriter("C:\\Users\\CNC\\proj\\ManualCNC\\Working\\smc5dlib_test3_cpp\\write.bin");
    //myStepWriter->Write(myJobStepList->List);
    //myStepWriter->Close();
    //
    //
    //
    //StepReader^ myStepReader = gcnew StepReader("C:\\Users\\CNC\\proj\\ManualCNC\\Working\\smc5dlib_test3_cpp\\write.bin");
    //myJobStepList->List = myStepReader->ReadToEnd();
    //myStepReader->Close();

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

StepList^ getResidualStepList(StepList^ stepList, int currentStepIndex, Connector^ connector)
{
    // For doing relative offset later
    StepList^ resStepList = gcnew StepList(connector->SMCSettings);
    StepCalc^ myStepCalc = gcnew StepCalc();
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
StepList^ generateCircularTest(float centerX, float centerY, float radius, float height, float speed, Connector^ connector)
{
    StepList^ stepList = gcnew StepList(connector->SMCSettings);
    stepList->AddXYZ(centerX + radius, centerY, height, 50.0f);
    stepList->AddXYZAB(centerX, centerY + radius, height, centerX, centerY, speed);

    return stepList;
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

    stepList->AddXYZ(centerX + radius, centerY, height, 50.0f);
    for (unsigned int i = 1; i < divisions; i++)
    {
        float radiant = (i * TWO_PI) / divisions;
        stepList->AddXYZ(centerX + radius * cos(radiant), centerY + radius * sin(radiant), height, speed);
    }
    stepList->AddXYZ(centerX + radius, centerY, height, speed);

    return stepList;
}

