/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Praneeth Sadda, Anton Deguet
  Created on: 2011-11-11

  (C) Copyright 2011 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#include "TestComponent.h"

#include <cisstMultiTask/mtsComponentWidget.h>

#include <QApplication>
#include <QMainWindow>

#include <cisstMultiTask/mtsTaskManager.h>

int main(int argc, char** argv)
{
    mtsComponentManager * manager = mtsComponentManager::GetInstance();
    QApplication app(argc, argv);
    TestComponent* tc = new TestComponent();
    tc->Configure("");
    manager->AddComponent(tc);
    manager->CreateAll();
    manager->WaitForStateAll(mtsComponentState::READY, 5.0 * cmn_s);
    manager->StartAll();
    manager->WaitForStateAll(mtsComponentState::ACTIVE, 5.0 * cmn_s);
    mtsComponentWidget* cw = new mtsComponentWidget(tc);
    QMainWindow win;
    win.setCentralWidget(cw);
    win.show();
    manager->StartAll();
    manager->WaitForStateAll(mtsComponentState::ACTIVE, 5.0 * cmn_s);
    app.exec();
    return 0;
}