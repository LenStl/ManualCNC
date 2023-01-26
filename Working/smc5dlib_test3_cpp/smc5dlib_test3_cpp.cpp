#include "pch.h"
#include <cstdio>
#include <windows.h>
//#include "stdlib.h"
//#include "step_io.h"
#include <array>
#include "job_controller.h"

#include <iostream>
#include <fstream>
#include <string>

//using namespace System;
//using namespace De::Boenigk::SMC5D::Settings;
//using namespace De::Boenigk::SMC5D::Basics;
//using namespace De::Boenigk::SMC5D::Move;

/* Halts program until given argument module is ready again. */
//void waitForReady(Job^ job);
//void waitForReady(Reference^ reference);
//void waitForReady(Connector^ connector);
//
//bool initConnector(Connector^ connector, System::String^ config_loc);

int main()
{
    // TODO: Make pch.h work.
    printf("+------------------------------------------------+\n");
    printf("|  An attempt at using smc5d.core.dll from C++.  |\n");
    printf("+--------------------------+---------------------+\n");
    printf("|  SPACE  - Halt/Continue  |\n");
    printf("| PERIOD  - Increase speed |\n");
    printf("|  COMMA  - Decrease speed |\n");
    printf("|   ESC   - Abort Job      |\n");
    printf("+--------------------------+\n\n");

    std::string directory = "C:\\Users\\CNC\\proj\\ManualCNC\\Working\\smc5dlib_test3_cpp\\";
    std::string fileName = "circle.grf5";

    JobController jobController = JobController(".\\config_Haase1290Expert.xml", ".\\");
    jobController.setBaseTravelSpeed(30.0f);
    jobController.setTravelHeight(100.0f);

    /* Initialize Connector. */
    //Connector^ connector = gcnew Connector(".\\");
    //if (!initConnector(connector, ".\\config_Haase1290Expert.xml")) return false;

    /* Initialize utilities. */
    //Reference^ reference = gcnew Reference(connector);
    //ManualMove^ manual_move = gcnew ManualMove(connector);
    //SpeedTact^ speed_tact = gcnew SpeedTact(connector);
    //speed_tact->Tact(100);

    jobController.doReference();
    /* Do reference run. */
    //printf("[!] Doing reference run.\n");
    //reference->Start();
    //waitForReady(reference);
    //printf("[+] Done.\n");

    jobController.loadGRF(directory.append(fileName).c_str());
    /* Setup Job. */
    //StepList^ job_step_list = gcnew StepList(connector->SMCSettings);
    //job_step_list = StepIO::loadGRF(String::Concat(directory, file_name), 100.0f, 20.0f, connector->SMCSettings);
    //job_step_list = StepIO::generateCircle(300.0f, 300.0f, 100.0f, 100.0f, 50.0f, 800, connector->SMCSettings); // Speed 30.0f smooth.
    //job_step_list = StepIO::generateLine(150.0f, 150.0f, 400.0f, 400.0f, 100.0f, 30.0f, connector->SMCSettings);
    //job_step_list = StepIO::generateSquare(100.0f, 100.0f, 300.0f, 300.0f, 100.0f, 50.0f, connector->SMCSettings);

    jobController.beginJob();
    //StepList^ current_job_step_list = gcnew StepList(connector->SMCSettings);
    //for (int i = 0; i < job_step_list->List->Count; i++)
    //    current_job_step_list->Add(job_step_list->List[i]->GetCopy());
    //
    //waitForReady(connector);
    //printf("[!] Cloning job.\n");
    //Job^ job = gcnew Job(connector);
    //job->SetStepList(current_job_step_list->List);
    //waitForReady(job);
    //
    //job->Start(100);
    //printf("[+] Done.\n");
    //printf("[!] Starting job.\n");

    //bool is_stopped = false;
    bool was_pressed_space = false;
    //unsigned int tactSpeed = speed_tact->Percent;      // For saving speed percent when stopping (should return to this speed when continuing).

    // TODO: Generate spirals from isolines. Execute group of isolines in one job. Several jobs to complete the entire heightfield.

    while (jobController.isJobRunning())
    {
        // Code necessary if manual move is needed when the current job is stopped.
        //if (GetKeyState(VK_SPACE) & 0x8000)
        //{
        //    if (!was_pressed_space)
        //    {
        //        was_pressed_space = true;
        //        /* Stopping with setting pin. (Doesnt seem to work)*/
        //        //if (!is_stopped) mySwitch->SetPinPermanently(Output::Con2Pin8MotorCurrent, false);
        //        //else mySwitch->SetPinPermanently(Output::Con2Pin8MotorCurrent, true);
        //        /* Speed control with SpeedTact. (Slows down, also affects Manual Move, doesnt stop the job)*/
        //        //if (!is_stopped) speedTact->Tact(25);
        //        //else speedTact->Tact(100);
        //        /* Pausing and continuing job. (Continue doesnt seem to work.*/
        //        //if (!is_stopped) job->Pause();
        //        //else job->Continue(255);
        //
        //        /* Abort/Pause job, do movement, calculate residual job and execute it.*/
        //        int currentIndex = job->GetPosition();
        //        //job->Pause();                               // Slow stopping, while still following path.
        //        job->Abort();
        //        //job->Dispose();
        //        printf(" [!] Job interrupted at step %d.\n", currentIndex);
        //        waitForReady(connector);
        //        manual_move->Step(MoveAxis::X, true, 0.0f);    // Somehow resets the connector from "Abort" to "Ready".
        //        manual_move->Stop();                           //
        //        Sleep(2000);
        //        job = gcnew Job(connector);
        //        
        //        if (currentIndex > 0)
        //        {
        //            currentIndex--; // Slight Non-Linearity in the step indices: GetPosition() == 0: Initializing, moving to the first coordinate. GetPosition() == 2: Going the first distance. GetPosition() == 1 doesnt exists.
        //            StepList^ newJobStepList = gcnew StepList(connector->SMCSettings);
        //            for (int i = 0; i < current_job_step_list->List->Count - currentIndex; i++)
        //                newJobStepList->Add(current_job_step_list->List[i + currentIndex]->GetCopy());
        //            current_job_step_list = newJobStepList;
        //        }
        //        job->SetStepList(current_job_step_list->List);
        //        printf(" [!] Continuing job...\n");
        //        waitForReady(connector);
        //        waitForReady(job);
        //        job->Start(100);
        //        
        //        was_pressed_space = false;
        //    }
        //}
            
        // Stopping via speed.
        if (GetKeyState(VK_ESCAPE) & 0x8000)
        {
            jobController.abortJob();
            break;
        }

        // Controlling speed
        //if (!is_stopped)
        //{
        if (GetKeyState(VK_OEM_PERIOD) & 0x8000)
        {
            jobController.setJobSpeed(50);
            Sleep(30);
        }  
        if (GetKeyState(VK_OEM_COMMA) & 0x8000)
        {
            jobController.setJobSpeed(100);
        }
        //}
    }
    //waitForReady(connector);
    printf("[+] Job completed.\n");
    printf("\n[+] OK!!\n");

    return true;
}

//bool initConnector(Connector^ connector, System::String^ config_loc)
//{
//    if (!(connector->Load(config_loc)))
//    {
//        printf("[-] Config loading failed.\n");
//        return false;
//    }
//    printf("[!] Configuration loaded. Trying to connect .");
//
//    SMCStatus result = connector->Enable();
//
//    int timeout = 5000;
//    while (connector->Status != SMCStatus::Ready && timeout > 0)
//    {
//        Sleep(50);
//        timeout -= 50;
//        printf(".");
//    }
//    if (connector->Status != SMCStatus::Ready)
//    {
//        printf("\n[-] Timeout with error %d.\n", connector->Status);
//        return false;
//    }
//    printf("\n[+] Connection successfully established with %s.\n", connector->GetControllerName());
//    return true;
//}
//void waitForReady(Job^ job)
//{
//    while (job->Status != SMCStatus::Ready)
//    {
//        Sleep(1);
//    }
//}
//void waitForReady(Reference^ reference)
//{
//    while (reference->Status != SMCStatus::Ready)
//    {
//        Sleep(1);
//    }
//}
//void waitForReady(Connector^ connector)
//{
//    while (connector->Status != SMCStatus::Ready)
//    {
//        Sleep(1);
//    }
//}