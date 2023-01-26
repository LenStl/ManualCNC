#include "pch.h"
#include "job_controller.h"
#include <windows.h>
#include <string>

int main()
{
    printf("+------------------------------------------------+\n"
           "|  An attempt at using smc5d.core.dll from C++.  |\n"
           "+------------------------------------------------+\n"
           "|              SPACE  - Halt/Continue            |\n"
           "|             PERIOD  - Increase speed           |\n"
           "|              COMMA  - Decrease speed           |\n"
           "|               ESC   - Abort Job                |\n"
           "+------------------------------------------------+\n\n");

    std::string directory = "C:\\Users\\CNC\\proj\\ManualCNC\\Working\\smc5dlib_test3_cpp\\";
    std::string fileName = "circle.grf5";

    JobController jobController = JobController();
    if (!jobController.initController(".\\config_Haase1290Expert.xml", ".\\")) return 1;

    jobController.setBaseTravelSpeed(30.0f);
    jobController.setTravelHeight(100.0f);

    jobController.doReference();

    jobController.loadGRF(directory.append(fileName).c_str());

    jobController.beginJob();
    while (jobController.isJobRunning())
    {
        if (GetKeyState(VK_ESCAPE) & 0x8000) break;
        if (GetKeyState(VK_OEM_PERIOD) & 0x8000)
        {
            jobController.setJobSpeed(jobController.getJobSpeed() + 1);
            Sleep(30);
        }  
        if (GetKeyState(VK_OEM_COMMA) & 0x8000)
        {
            jobController.setJobSpeed(jobController.getJobSpeed() - 1);
            Sleep(30);
        }
    }

    jobController.abortJob();
    printf("[+] Job completed.\n\n"
           "[+] OK!!\n");
    return 0;
}