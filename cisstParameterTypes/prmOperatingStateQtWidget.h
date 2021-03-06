/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  Author(s):  Anton Deguet
  Created on: 2019-01-15

  (C) Copyright 2019 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#ifndef _prmOperatingStateQtWidget_h
#define _prmOperatingStateQtWidget_h

#include <QWidget>
#include <QMetaType>

#include <cisstParameterTypes/prmOperatingState.h>

// Always include last
#include <cisstParameterTypes/prmExportQt.h>

// so we can have slots/signals with prmOperatingState
Q_DECLARE_METATYPE(prmOperatingState);

class QLabel;

class CISST_EXPORT prmOperatingStateQtWidget: public QWidget
{
    Q_OBJECT;

public:
    prmOperatingStateQtWidget(void);
    ~prmOperatingStateQtWidget(void) {};

    inline void setupUi(void) {};

    /*! Set value, this method will update the values display in the
      Qt Widget. */
    void SetValue(const prmOperatingState & newValue);

protected:
    QLabel * QLState;
    QLabel * QLTime; // time and valid using background color
    QLabel * QLIsHomed;
    QLabel * QLIsBusy;
};

#endif // _prmOperatingStateQtWidget_h
