#pragma once
#include "step_io.h"

#include <msclr/gcroot.h>
using namespace msclr;

using namespace System;
using namespace De::Boenigk::SMC5D::Settings;
using namespace De::Boenigk::SMC5D::Basics;
using namespace De::Boenigk::SMC5D::Move;

class JobController
{
public:
    JobController();
    ~JobController();
    bool initController(const char* configLoc, const char* logLoc);

    void doReference();

    bool loadGRF(const char* filePath);
    void generateCircle(float centerX, float centerY, float radius, unsigned int divisions);
    void generateLine(float startX, float startY, float endX, float endY);
    void generateSquare(float minX, float minY, float maxX, float maxY);

    bool beginJob();
    void abortJob();
    bool isJobRunning();
    void setJobSpeed(int jobSpeed);
    void increaseJobSpeed();
    void decreaseJobSpeed();
    float getJobSpeed();

    void setTravelHeight(float travelHeight);
    float getTravelHeight();
    void setBaseTravelSpeed(float baseTravelSpeed);
    float getBaseTravelSpeed();

private:
    bool m_DidReferencePass;
    float m_TravelHeight;
    float m_BaseTravelSpeed;
    gcroot<Connector^> m_Connector;
    gcroot<Reference^> m_Reference;
    gcroot<ManualMove^> m_ManualMove;
    gcroot<SpeedTact^> m_SpeedTact;
    gcroot<StepList^> m_StepList;
    gcroot<Job^> m_Job;
};
