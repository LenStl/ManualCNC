#include "pch.h"
#include "job_controller.h"
#include <windows.h>
#include <string>

int main()
{
    printf("+------------------------------------------------+\n"
           "|  An attempt at using smc5d.core.dll from C++.  |\n"
           "+------------------------------------------------+\n"
           "|             PERIOD  - Increase speed           |\n"
           "|              COMMA  - Decrease speed           |\n"
           "|               ESC   - Abort Job                |\n"
           "+------------------------------------------------+\n\n");

    std::string directory = "C:\\Users\\CNC\\proj\\ManualCNC\\Working\\smc5dlib_test3_cpp\\";
    std::string fileName = "iso.grf5";

    JobController jobController = JobController();
    if (!jobController.initController(".\\config_Haase1290Expert.xml", ".\\")) return 1;

    jobController.setBaseTravelSpeed(50.0f);
    jobController.setTravelHeight(100.0f);

    jobController.doReference();

    if (!jobController.loadGRF(directory.append(fileName).c_str())) return 1;

    if (!jobController.beginJob()) return 1;
    while (jobController.isJobRunning())
    {
        if (GetKeyState(VK_ESCAPE) & 0x8000) break;
        if (GetKeyState(VK_NUMPAD1) & 0x8000)
        {
            jobController.setJobSpeed(10);
        }
        if (GetKeyState(VK_NUMPAD5) & 0x8000)
        {
            jobController.setJobSpeed(50);
        }
        if (GetKeyState(VK_NUMPAD0) & 0x8000)
        {
            jobController.setJobSpeed(100);
        }
        if (GetKeyState(VK_OEM_PERIOD) & 0x8000)
        {
            jobController.increaseJobSpeed();
            Sleep(10);
        }  
        if (GetKeyState(VK_OEM_COMMA) & 0x8000)
        {
            jobController.decreaseJobSpeed();
            Sleep(10);
        }
    }
    
    jobController.abortJob();
    printf("[+] Job completed.\n\n"
           "[+] OK!!\n");
    return 0;
}