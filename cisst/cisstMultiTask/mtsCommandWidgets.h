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

#ifndef _mtsCommandWidgets_h
#define _mtsCommandWidgets_h

#include <cisstMultiTask/mtsCommandWidget.h>

#include <QObject>

#include <cisstMultiTask/mtsCommandVoid.h>
#include <cisstMultiTask/mtsCommandVoidReturn.h>
#include <cisstMultiTask/mtsCommandWrite.h>
#include <cisstMultiTask/mtsCommandWriteReturn.h>
#include <cisstMultiTask/mtsCommandRead.h>
#include <cisstMultiTask/mtsCommandQualifiedRead.h>
//#include <cisstMultiTask/mtsEventVoid.h>
//#include <cisstMultiTask/mtsEventWrite.h>

#include <cisstMultiTask/mtsFunctionVoid.h>
#include <cisstMultiTask/mtsFunctionVoidReturn.h>
#include <cisstMultiTask/mtsFunctionWrite.h>
#include <cisstMultiTask/mtsFunctionWriteReturn.h>
#include <cisstMultiTask/mtsFunctionRead.h>
#include <cisstMultiTask/mtsFunctionQualifiedRead.h>

// Always include last
#include <cisstMultiTask/mtsExportQt.h>

class CISST_EXPORT CommandVoidWidget : public mtsCommandWidget {

    Q_OBJECT;

 private:
    mtsFunctionVoid function;

 public slots:
    virtual void Execute();

 public:
    explicit CommandVoidWidget(mtsFunctionVoid& command);
};


class CISST_EXPORT CommandVoidReturnWidget : public mtsCommandWidget {

    Q_OBJECT;

 private:
    mtsFunctionVoidReturn function;

 public slots:
    virtual void Execute();

 public:
    explicit CommandVoidReturnWidget(mtsFunctionVoidReturn& command);
};

class CISST_EXPORT CommandWriteWidget : public mtsCommandWidget {

    Q_OBJECT;

 private:
    mtsFunctionWrite function;

 public slots:
    virtual void Execute();

 public:
    explicit CommandWriteWidget(mtsFunctionWrite& command);
};

class CISST_EXPORT CommandWriteReturnWidget : public mtsCommandWidget {

    Q_OBJECT;

 private:
    mtsFunctionWriteReturn function;

 public slots:
    virtual void Execute();

 public:
    explicit CommandWriteReturnWidget(mtsFunctionWriteReturn& command);
};

class CISST_EXPORT CommandReadWidget : public mtsCommandWidget {

    Q_OBJECT;

 private:
    mtsFunctionRead function;

 public slots:
    virtual void Execute();

 public:
    explicit CommandReadWidget(mtsFunctionRead& command);
};

class CISST_EXPORT CommandQualifiedReadWidget : public mtsCommandWidget {

    Q_OBJECT;

 private:
    mtsFunctionQualifiedRead function;

 public slots:
    virtual void Execute();

 public:
    explicit CommandQualifiedReadWidget(mtsFunctionQualifiedRead& command);
};

class CISST_EXPORT CommandEventVoidWidget : public mtsCommandWidget {

    Q_OBJECT;

 public slots:
    virtual void Execute();

 public:
    CommandEventVoidWidget();
};

class CommandEventWriteWidget : public mtsCommandWidget {

    Q_OBJECT;

public slots:
    virtual void Execute();

public:
    CommandEventWriteWidget();
};

#endif //ifndef _mtsCommandWidgets_h