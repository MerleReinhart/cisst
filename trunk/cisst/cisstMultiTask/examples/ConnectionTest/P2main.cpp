/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Min Yang Jung
  Created on: 2009-01-27

  (C) Copyright 2010 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#include <cisstOSAbstraction/osaSleep.h>

#include "C2ServerTask.h"
#include "C3Task.h"

int main(int argc, char * argv[])
{
    // Set global component manager IP
    std::string globalComponentManagerIP;
    if (argc == 1) {
        globalComponentManagerIP = "localhost";
    } else if (argc == 2) {
        globalComponentManagerIP = argv[1];
    } else {
        std::cerr << "Usage: " << argv[0] << " (global component manager IP)" << std::endl;
        return 1;
    }
    std::cout << "Global component manager IP: " << globalComponentManagerIP << std::endl;

    // log configuration
    cmnLogger::SetMask(CMN_LOG_ALLOW_ALL);
    cmnLogger::AddChannel(std::cout, CMN_LOG_ALLOW_ERRORS_AND_WARNINGS);
    // add a log per thread
    //osaThreadedLogFile threadedLog("example9Server");
    //cmnLogger::AddChannel(threadedLog, CMN_LOG_ALLOW_ALL);

    // Get local component manager instance
    mtsManagerLocal * localManager;
    try {
        localManager = mtsManagerLocal::GetInstance(globalComponentManagerIP, "P2");
    } catch (...) {
        CMN_LOG_INIT_ERROR << "Failed to initialize local component manager" << std::endl;
        return 1;
    }
    /* If there are more than one network interfaces installed on this machine
       and an user wants to specify one of them to use, the following codes can
       be used:
        
       std::vector<std::string> ipAddresses = mtsManagerLocal::GetIPAddressList();
       std::string thisProcessIP = ipAddresses[i];  // i=[0, ipAddresses.size()-1]

       mtsManagerLocal * localManager = mtsManagerLocal::GetInstance(globalComponentManagerIP, "P2", thisProcessIP);
    */

    // create our server task
    const double PeriodClient = 10 * cmn_ms; // in milliseconds
    C2ServerTask * C2Server = new C2ServerTask("C2", PeriodClient);
    C3Task * C3 = new C3Task("C3", PeriodClient);
    localManager->AddComponent(C2Server);
    localManager->AddComponent(C3);

    if (!localManager->Connect("C3", "r1", "C2", "p2")) {
        CMN_LOG_INIT_ERROR << "Connect failed: C3:r1-C2:p2" << std::endl;
        return 1;
    }
    
    // create the tasks, i.e. find the commands
    localManager->CreateAll();
    // start the periodic Run
    localManager->StartAll();
    
    while (1) {
        osaSleep(10 * cmn_ms);
    }
    
    // cleanup
    localManager->KillAll();
    localManager->Cleanup();

    return 0;
}