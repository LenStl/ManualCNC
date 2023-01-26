#pragma once

#include <cstdio>
#include <windows.h>
#include <iostream>
#include <fstream>

#include <msclr/gcroot.h>
using namespace msclr;

#include "step_io.h"

using namespace System;
using namespace De::Boenigk::SMC5D::Settings;
using namespace De::Boenigk::SMC5D::Basics;
using namespace De::Boenigk::SMC5D::Move;

class JobController
{
public:
    JobController(const char* config_loc, const char* log_loc);
    ~JobController();

    bool initController(Connector^ connector, System::String^ config_loc);

    void doReference();

    bool loadGRF(const char* file_path);
    void JobController::generateCircle(float center_x, float center_y, float radius, unsigned int divisions) {
        mStepList = StepIO::generateCircle(center_x, center_y, radius, mTravelHeight, mBaseTravelSpeed, divisions, mConnector->SMCSettings);
    };
    void JobController::generateLine(float start_x, float start_y, float end_x, float end_y) {
        mStepList = StepIO::generateLine(start_x, start_y, end_x, end_y, mTravelHeight, mBaseTravelSpeed, mConnector->SMCSettings);
    };
    void JobController::generateSquare(float min_x, float min_y, float max_x, float max_y) {
        mStepList = StepIO::generateSquare(min_x, min_y, max_x, max_y, mTravelHeight, mBaseTravelSpeed, mConnector->SMCSettings);
    };

    void setTravelHeight(float travelHeight);
    float getTravelHeight() { return mTravelHeight; }

    void setBaseTravelSpeed(float baseTravelSpeed);
    float getBaseTravelSpeed() { return mBaseTravelSpeed; }

    bool JobController::isJobRunning() {
        return (mJob.operator->() == nullptr) ? false : mJob->Status != SMCStatus::Ready;
    };

    bool beginJob();
    void abortJob();
    void setJobSpeed(int jobSpeed);
    float JobController::getJobSpeed() { return (mJob.operator->() == nullptr) ? -1.0f : mSpeedTact->Percent; }



private:
    bool mDidReferencePass;
    float mTravelHeight;
    float mBaseTravelSpeed;
    gcroot<Connector^>  mConnector;
    gcroot<Reference^>  mReference;
    gcroot<ManualMove^> mManualMove;
    gcroot<SpeedTact^>  mSpeedTact;
    gcroot<StepList^>   mStepList;
    gcroot<Job^>        mJob;
};
