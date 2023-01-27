#include "pch.h"
#include "job_controller.h"

/* Halts program until given argument module is ready again. */
void waitForReady(Job^ job);
void waitForReady(Reference^ reference);
void waitForReady(Connector^ connector);

JobController::~JobController()
{
    delete m_SpeedTact;
    delete m_StepList;

    if (m_Job.operator->() != nullptr) m_Job->Abort();
    delete m_Job;

    if (m_Reference.operator->() != nullptr) m_Reference->Abort();
    delete m_Reference;

    if (m_ManualMove.operator->() != nullptr) m_ManualMove->Stop();
    delete m_ManualMove;

    delete m_Connector;
}
bool JobController::initController(const char* configPath, const char* logPath)
{
    m_Connector = gcnew Connector(gcnew String(logPath));

    if (!m_Connector->Load(gcnew String(configPath)))
    {
        printf("[-] Config loading failed.\n");
        return false;
    }
    printf("[!] Configuration loaded. Trying to connect .");
    m_Connector->Enable();

    int timeout = 5000;
    while (m_Connector->Status != SMCStatus::Ready && timeout > 0)
    {
        Sleep(50);
        timeout -= 50;
        printf(".");
    }
    if (m_Connector->Status != SMCStatus::Ready)
    {
        printf("\n[-] Timeout with error %d.\n", m_Connector->Status);
        return false;
    }
    printf("\n[+] Connection successfully established with %s.\n", m_Connector->GetControllerName());

    m_DidReferencePass = false;
    m_Reference = gcnew Reference(m_Connector);
    m_ManualMove = gcnew ManualMove(m_Connector);
    m_SpeedTact = gcnew SpeedTact(m_Connector);
    m_SpeedTact->Tact(100);

    return true;
}

void JobController::doReference()
{
    if (m_Reference.operator->() == nullptr) return;
    printf("[!] Doing reference run.\n");
    m_Reference->Start();
    waitForReady(m_Reference);
    printf("[+] Done.\n");
    m_DidReferencePass = true;
}

bool JobController::loadGRF(const char* filePath)
{
    return (m_StepList = StepIO::loadGRF(gcnew String(filePath), m_TravelHeight, m_BaseTravelSpeed, m_Connector->SMCSettings)).operator->() != nullptr;
}
void JobController::generateCircle(float centerX, float centerY, float radius, unsigned int divisions)
{
    m_StepList = StepIO::generateCircle(centerX, centerY, radius, m_TravelHeight, m_BaseTravelSpeed, divisions, m_Connector->SMCSettings);
}
void JobController::generateLine(float startX, float startY, float endX, float endY)
{
    m_StepList = StepIO::generateLine(startX, startY, endX, endY, m_TravelHeight, m_BaseTravelSpeed, m_Connector->SMCSettings);
}
void JobController::generateSquare(float minX, float minY, float maxX, float maxY)
{
    m_StepList = StepIO::generateSquare(minX, minY, maxX, maxY, m_TravelHeight, m_BaseTravelSpeed, m_Connector->SMCSettings);
}
void JobController::printCurrentStepList()
{
    printf("%s\n", m_StepList->ToString());
}

bool JobController::beginJob()
{
    if (!m_DidReferencePass || (m_StepList.operator->() == nullptr)) return false;
    StepList^ currentStepList = gcnew StepList(m_Connector->SMCSettings);
    for (int i = 0; i < m_StepList->List->Count; i++)
        currentStepList->Add(m_StepList->List[i]->GetCopy());

    waitForReady(m_Connector);
    printf("[!] Cloning job.\n");
    m_Job = gcnew Job(m_Connector);
    m_Job->SetStepList(currentStepList->List);
    waitForReady(m_Job);

    m_Job->Start(100);
    printf("[+] Done.\n");
    printf("[!] Starting job.\n");

    return true;
}
void JobController::abortJob()
{
    if (m_Job.operator-> () == nullptr) return;
    m_Job->Abort();
    delete m_Job;
}
bool JobController::isJobRunning() {
    return (m_Job.operator->() == nullptr) ? false : m_Job->Status != SMCStatus::Ready;
}
void JobController::increaseJobSpeed()
{
    m_SpeedTact->TactPlus();
}
void JobController::decreaseJobSpeed()
{
    m_SpeedTact->TactMinus();
}
void JobController::setJobSpeed(int jobSpeed)
{
    if (jobSpeed < 0 || jobSpeed > 100 || m_SpeedTact->Percent == jobSpeed) return;

    while (m_SpeedTact->Percent != jobSpeed)
    {
        m_SpeedTact->Percent < jobSpeed ? m_SpeedTact->TactPlus() : m_SpeedTact->TactMinus();
    }
}
int JobController::getJobSpeed()
{
    return (m_Job.operator->() == nullptr) ? -1 : m_SpeedTact->Percent;
}

void JobController::setTravelHeight(float travelHeight)
{
    m_TravelHeight = travelHeight;
    // Update the steps accordingly.
    if (m_StepList.operator->() == nullptr) return;
    for (int i = 0; i < m_StepList->List->Count; i++)
        m_StepList->List[i]->Z = travelHeight;
}
float JobController::getTravelHeight() 
{ 
    return m_TravelHeight; 
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
float JobController::getBaseTravelSpeed() 
{ 
    return m_BaseTravelSpeed; 
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