/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */
/* $Id: sineTask.cpp 75 2009-02-24 16:47:20Z adeguet1 $ */

#include <cisstCommon/cmnConstants.h>
#include "sineTask.h"

// required to implement the class services, see cisstCommon
CMN_IMPLEMENT_SERVICES(sineTask);

sineTask::sineTask(const std::string & taskName, double period):
    // base constructor, same task name and period.  Set the length of
    // state table to 5000
    mtsTaskPeriodic(taskName, period, false, 5000)
{
    // add SineData to the StateTable defined in mtsTask
    SineData.AddToStateTable(StateTable, "SineData");
    // add one interface, this will create an mtsTaskInterface
    AddProvidedInterface("MainInterface"); 
    // add command to access state table values to the interface
    SineData.AddReadCommandToTask(this, "MainInterface", "GetData");
    // following should be done automatically
    AddCommandRead(&mtsStateTable::GetIndexReader, &StateTable,
                   "MainInterface", "GetStateIndex");
    // add command to modify the sine amplitude 
    SineAmplitude.AddWriteCommandToTask(this, "MainInterface", "SetAmplitude");
    // add command to test commandVoid
    AddCommandVoid(&sineTask::CommandVoidTest, this, "MainInterface", "CommandVoid");
}

void sineTask::Startup(void) {
    SineAmplitude = 1.0; // set the initial amplitude
}

void sineTask::Run(void) {
    // the state table provides an index
    const mtsStateIndex now = StateTable.GetIndexWriter();
    // process the commands received, i.e. possible SetSineAmplitude
    ProcessQueuedCommands();
    // compute the new values based on the current time and amplitude
    SineData = SineAmplitude.Data
        * sin(2 * cmnPI * static_cast<double>(now.Ticks()) * Period / 10.0);
}

void sineTask::CommandVoidTest(void)
{
    static int n = 0;

    std::cout << "CommandVoid test called:  " << ++n << std::endl;
}

/*
  Author(s):  Ankur Kapoor, Peter Kazanzides, Anton Deguet, Min Yang Jung
  Created on: 2004-04-30

  (C) Copyright 2004-2009 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/
