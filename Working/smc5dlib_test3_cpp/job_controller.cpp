#include "pch.h"
#include "job_controller.h"

/* Halts program until given argument module is ready again. */
void waitForReady(Job^ job);
void waitForReady(Reference^ reference);
void waitForReady(Connector^ connector);

JobController::JobController(const char* config_loc, const char* log_loc)
{
    mConnector = gcnew Connector(gcnew String(log_loc));
    initController(mConnector, gcnew String(config_loc));
}
JobController::~JobController()
{
    delete mSpeedTact;

    mJob->Abort();
    delete mJob;

    delete mStepList;

    mReference->Abort();
    delete mReference;

    mManualMove->Stop();
    delete mManualMove;

    delete mConnector;
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

    mDidReferencePass = false;
    mReference = gcnew Reference(mConnector);
    mManualMove = gcnew ManualMove(mConnector);
    mSpeedTact = gcnew SpeedTact(mConnector);
    mSpeedTact->Tact(100);

    return true;
}
void JobController::doReference()
{
    printf("[!] Doing reference run.\n");
    mReference->Start();
    waitForReady(mReference);
    printf("[+] Done.\n");
    mDidReferencePass = true;
}
bool JobController::beginJob()
{
    if (!mDidReferencePass) return false;
    StepList^ currentStepList = gcnew StepList(mConnector->SMCSettings);
    for (int i = 0; i < mStepList->List->Count; i++)
        currentStepList->Add(mStepList->List[i]->GetCopy());

    waitForReady(mConnector);
    printf("[!] Cloning job.\n");
    mJob = gcnew Job(mConnector);
    mJob->SetStepList(currentStepList->List);
    waitForReady(mJob);

    mJob->Start(100);
    printf("[+] Done.\n");
    printf("[!] Starting job.\n");

    return true;
}
void JobController::abortJob()
{
    mJob->Abort();

    delete mJob;
}
void JobController::setJobSpeed(int jobSpeed)
{
    if (jobSpeed < 0 || jobSpeed > 100) return;
    if (mSpeedTact->Percent == jobSpeed) return;

    while (mSpeedTact->Percent != jobSpeed)
    {
        mSpeedTact->Percent < jobSpeed ? mSpeedTact->TactPlus() : mSpeedTact->TactMinus();
        Sleep(30);
    }
}

bool JobController::loadGRF(const char* file_path)
{
    mStepList = StepIO::loadGRF(gcnew String(file_path), mTravelHeight, mBaseTravelSpeed, mConnector->SMCSettings);
    return mStepList.operator->() != nullptr;
}

void JobController::setTravelHeight(float travelHeight)
{
    mTravelHeight = travelHeight;
    // Update the steps accordingly.
    if (mStepList.operator->() == nullptr) return;
    for (int i = 0; i < mStepList->List->Count; i++)
        mStepList->List[i]->Z = travelHeight;
}
void JobController::setBaseTravelSpeed(float baseTravelSpeed)
{
    mBaseTravelSpeed = baseTravelSpeed;
    // Update the steps accordingly.
    if (mStepList.operator->() == nullptr) return;
    mStepList->List[0]->Speed = StepIO::INIT_SPEED;
    for (int i = 1; i < mStepList->List->Count; i++)
        mStepList->List[i]->Speed = baseTravelSpeed;
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