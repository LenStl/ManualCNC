#include "pch.h"
#include "job_controller.h"

/* Halts program until given argument module is ready again. */
void waitForReady(Job^ job);
void waitForReady(Reference^ reference);
void waitForReady(Connector^ connector);

JobController::JobController(const char* config_loc, const char* log_loc)
{
    m_Connector = gcnew Connector(gcnew String(log_loc));
    initController(m_Connector, gcnew String(config_loc));
}
JobController::~JobController()
{
    delete m_SpeedTact;

    m_Job->Abort();
    delete m_Job;
    delete m_StepList;

    m_Reference->Abort();
    delete m_Reference;

    m_ManualMove->Stop();
    delete m_ManualMove;

    delete m_Connector;
}

bool JobController::initController(Connector^ connector, System::String^ config_loc)
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

    m_DidReferencePass = false;
    m_Reference = gcnew Reference(m_Connector);
    m_ManualMove = gcnew ManualMove(m_Connector);
    m_SpeedTact = gcnew SpeedTact(m_Connector);
    m_SpeedTact->Tact(100);

    return true;
}
void JobController::doReference()
{
    printf("[!] Doing reference run.\n");
    m_Reference->Start();
    waitForReady(m_Reference);
    printf("[+] Done.\n");
    m_DidReferencePass = true;
}

bool JobController::loadGRF(const char* file_path)
{
    m_StepList = StepIO::loadGRF(gcnew String(file_path), m_TravelHeight, m_BaseTravelSpeed, m_Connector->SMCSettings);
    return m_StepList.operator->() != nullptr;
}

void JobController::setTravelHeight(float travelHeight)
{
    m_TravelHeight = travelHeight;
    // Update the steps accordingly.
    if (m_StepList.operator->() == nullptr) return;
    for (int i = 0; i < m_StepList->List->Count; i++)
        m_StepList->List[i]->Z = travelHeight;
}
void JobController::setBaseTravelSpeed(float baseTravelSpeed)
{
    m_BaseTravelSpeed = baseTravelSpeed;
    // Update the steps accordingly.
    if (m_StepList.operator->() == nullptr) return;
    m_StepList->List[0]->Speed = StepIO::INIT_SPEED;
    for (int i = 1; i < m_StepList->List->Count; i++)
        m_StepList->List[i]->Speed = baseTravelSpeed;
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