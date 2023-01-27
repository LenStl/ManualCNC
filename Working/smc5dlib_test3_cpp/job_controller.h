#pragma once
#include "step_io.h"

#include <msclr/gcroot.h>
#include <stdio.h>
#include <windows.h>
using namespace msclr;

using namespace System;
using namespace De::Boenigk::SMC5D::Settings;
using namespace De::Boenigk::SMC5D::Basics;
using namespace De::Boenigk::SMC5D::Move;

class JobController
{
public:
    ~JobController();

    /**
     * Connects the the SMC5D Controller, initializes settings and objects.
     *
     * @param configPath - File with configuration data for the given machine.
     * @param logPath - File to store log data.
     * @return Success of connecting to the SMC5D Controller and/or loading the config file.
     */
    bool initController(const char* configPath, const char* logPath);

    /**
      Performs a reference run on the machine. Needed for beginning a job.
     */
    void doReference();

    /**
     * Reads a .grf4 or .grf5 file and extracts the steps into a StepList.
     *
     * @param filePath - Path to the file.
     * @return Success of loading the file from filePath.
     */
    bool loadGRF(const char* filePath);
    /**
     * Generates a list of steps along a circle.
     *
     * @param centerX - X coordinate of the center of the circle.
     * @param centerY - Y coordinate of the center of the circle.
     * @param radius - Radius of the circle. 
     * @param divisions - Number of linear segments.
     */
    void generateCircle(float centerX, float centerY, float radius, unsigned int divisions);
    /**
     * Generates a list of steps along a line.
     *
     * @param startX - Starting x coordinate of the line.
     * @param startY - Starting y coordinate of the line.
     * @param endX - End x coordinate of the line.
     * @param endY - End y coordinate of the line.
     */
    void generateLine(float startX, float startY, float endX, float endY);
    /**
    * Generates a list of steps along a 2D-AABB.
    *
    * @param minX - Minimum and starting x coordinate of the square.
    * @param minY - Minimum and starting y coordinate of the square.
    * @param maxX - Maximum x coordinate of the square.
    * @param maxY - Maximum y coordinate of the square.
    */
    void generateSquare(float minX, float minY, float maxX, float maxY);

    /**
    * Starts the generated or loaded job.
    *
    * @return Could start job. Can fail if a reference run has not been done yet and/or if no job has been generated or loaded.
    */
    bool beginJob();
    /**
     Aborts the job.
    */
    void abortJob();
    /**
    * Checks if the job is currently running, i.e. is not in the SMCStatus::Ready state.
    * @return Is the job running.
    */
    bool isJobRunning();
    /**
    * Sets the speed of the current job in percent of the base travel speed. Quickly interpolates towards it.
    * @param jobSpeed - The new job speed.
    */
    void setJobSpeed(int jobSpeed);
    /**
    * Gets the speed of the current job in percent of the base travel speed.
    * @result The current job speed in percent.
    */
    int getJobSpeed();
    /**
     Increases the job speed by 1%.
    */
    void increaseJobSpeed();
    /**
     Decreases the job speed by 1%.
    */
    void decreaseJobSpeed();
    /**
     Prints out the current list of steps.
    */
    void printCurrentStepList();

    /**
    * Changes the travel height of the whole step list. Doesn't affect a running job.
    * @param travelHeight - The height at which the machine head will travel.
    */
    void setTravelHeight(float travelHeight);
    /*
    * Returns the current travel height.
    * @return The travel height of the machine head.
    */
    float getTravelHeight();
    /**
    * Changes the travel speed in mm/s of the whole step list. Doesn't affect a running job.
    * @param baseTravelSpeed - The speed at which the machine head will travel at 100% job speed.
    */
    void setBaseTravelSpeed(float baseTravelSpeed);
    /*
    * Returns the current travel speed.
    * @return The travel speed of the machine head in mm/s.
    */
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
