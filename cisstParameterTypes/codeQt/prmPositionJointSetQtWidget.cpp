/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  Author(s):  Anton Deguet
  Created on: 2019-12-09

  (C) Copyright 2019-2020 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cisstParameterTypes/prmPositionJointSetQtWidget.h>
#include <cisstParameterTypes/prmConfigurationJoint.h>
#include <cisstParameterTypes/prmPositionJointSet.h>

#include <QHBoxLayout>
#include <QPushButton>

prmPositionJointSetQtWidget::prmPositionJointSetQtWidget(void):
    QWidget(),
    GetStateJoint(0),
    GetConfigurationJoint(0),
    SetPositionGoalJoint(0),
    mPrismaticFactor(1.0),
    mRevoluteFactor(1.0),
    mNeedsConversion(false)
{
}

void prmPositionJointSetQtWidget::setupUi(void)
{
    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins(2, 2, 2, 2);
    this->setLayout(layout);

    QPBReset = new QPushButton("Reset");
    layout->addWidget(QPBReset);
    QPBMove = new QPushButton("Move");
    QPBMove->setEnabled(false);
    layout->addWidget(QPBMove);

    QVWPosition = new vctQtWidgetDynamicVectorDoubleWrite(vctQtWidgetDynamicVectorWriteBase::SPINBOX_WIDGET);
    QVWPosition->SetPrecision(3);
    QVWPosition->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed)); 
    layout->addWidget(QVWPosition);

    // connect buttons
    connect(QPBReset, SIGNAL(clicked()),
            this, SLOT(SlotReset()));
    connect(QPBMove, SIGNAL(clicked()),
            this, SLOT(SlotSetPositionGoalJoint()));
}

void prmPositionJointSetQtWidget::SlotReset(void)
{
    QPBMove->setEnabled(false);
    if (!GetStateJoint) {
        return;
    }
    if (!GetConfigurationJoint) {
        return;
    }
    prmStateJoint state;
    mtsExecutionResult executionResult = (*GetStateJoint)(state);
    if (!executionResult) {
        return;
    }

    if (mNeedsConversion) {
        prmConfigurationJoint configuration;
        executionResult = (*GetConfigurationJoint)(configuration);
        if (!executionResult) {
            return;
        }
        if (configuration.Type().size() == state.Position().size()) {
            mFactors.SetSize(configuration.Type().size());
            prmJointTypeToFactor(configuration.Type(), mPrismaticFactor, mRevoluteFactor, mFactors);
            mTemp1.SetSize(state.Position().size());
            mTemp2.SetSize(state.Position().size());
            mTemp1.ElementwiseProductOf(mFactors, configuration.PositionMin());
            mTemp2.ElementwiseProductOf(mFactors, configuration.PositionMax());
            QVWPosition->SetRange(mTemp1, mTemp2);
            mTemp1.ElementwiseProductOf(mFactors, state.Position());
            QVWPosition->SetValue(mTemp1);
            QPBMove->setEnabled(true);
        }
    } else {
        QVWPosition->SetValue(state.Position());
        QPBMove->setEnabled(true);
    }
}

void prmPositionJointSetQtWidget::SlotSetPositionGoalJoint(void)
{
    if (!SetPositionGoalJoint) {
        return;
    }
    // get the values from the widget
    mTemp1.SetSize(QVWPosition->columnCount());
    QVWPosition->GetValue(mTemp1);
    // prepare goal with proper units
    prmPositionJointSet goal;
    goal.Goal().SetSize(mTemp1.size());
    if (mNeedsConversion) {
        goal.Goal().ElementwiseRatioOf(mTemp1, mFactors);
    } else {
        goal.Goal().Assign(mTemp1);
    }
    (*SetPositionGoalJoint)(goal);
}
