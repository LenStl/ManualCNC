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
        m_StepList = StepIO::generateCircle(center_x, center_y, radius, m_TravelHeight, m_BaseTravelSpeed, divisions, m_Connector->SMCSettings);
    };
    void JobController::generateLine(float start_x, float start_y, float end_x, float end_y) {
        m_StepList = StepIO::generateLine(start_x, start_y, end_x, end_y, m_TravelHeight, m_BaseTravelSpeed, m_Connector->SMCSettings);
    };
    void JobController::generateSquare(float min_x, float min_y, float max_x, float max_y) {
        m_StepList = StepIO::generateSquare(min_x, min_y, max_x, max_y, m_TravelHeight, m_BaseTravelSpeed, m_Connector->SMCSettings);
    };

    void setTravelHeight(float travelHeight);
    float getTravelHeight() { return m_TravelHeight; }

    void setBaseTravelSpeed(float baseTravelSpeed);
    float getBaseTravelSpeed() { return m_BaseTravelSpeed; }

    bool JobController::isJobRunning() {
        return (m_Job.operator->() == nullptr) ? false : m_Job->Status != SMCStatus::Ready;
    };



private:
    bool m_DidReferencePass;
    float m_TravelHeight;
    float m_BaseTravelSpeed;
    gcroot<Connector^>  m_Connector;
    gcroot<Reference^>  m_Reference;
    gcroot<ManualMove^> m_ManualMove;
    gcroot<SpeedTact^>  m_SpeedTact;
    gcroot<StepList^>   m_StepList;
    gcroot<Job^>        m_Job;
};
