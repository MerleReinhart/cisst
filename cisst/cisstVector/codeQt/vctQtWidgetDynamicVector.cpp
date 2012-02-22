/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Anton Deguet
  Created on: 2011-12-08

  (C) Copyright 2011 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#include <cisstVector/vctQtWidgetDynamicVector.h>

#include <QTableWidget>
#include <QHeaderView>
#include <QSpinBox>
#include <QDoubleSpinBox>

// -- for doubles
vctQtWidgetDynamicVectorDoubleRead::vctQtWidgetDynamicVectorDoubleRead(void):
    QTableWidget()
{
    this->setRowCount(1);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    int verticalHeight = this->horizontalHeader()->sizeHint().height() + this->verticalHeader()->sizeHint().height();
    this->setFixedHeight(verticalHeight);
}

bool vctQtWidgetDynamicVectorDoubleRead::SetValue(const vctDynamicVector<double> & vector)
{
    size_t size = vector.size();
    if (this->columnCount() != size) {
        this->setColumnCount(size);
    }
    QTableWidgetItem * tableItem;
    QString itemValue;
    for (unsigned int index = 0; index < size; ++index) {
        tableItem = this->item(0, index);
        if (tableItem == 0) {
            tableItem = new QTableWidgetItem();
            tableItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            this->setItem(0, index, tableItem);
        }
        itemValue.setNum(vector.Element(index));
        tableItem->setText(itemValue);
    }
    return true;
}

vctQtWidgetDynamicVectorDoubleWrite::vctQtWidgetDynamicVectorDoubleWrite(void):
    QTableWidget()
{
    this->setRowCount(1);
    this->setContentsMargins(0, 0, 0, 0);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    int verticalHeight = this->horizontalHeader()->sizeHint().height() + this->verticalHeader()->sizeHint().height();
    this->setFixedHeight(verticalHeight);
}

bool vctQtWidgetDynamicVectorDoubleWrite::SetValue(const vctDynamicVector<double> & vector)
{
    size_t size = vector.size();
    if (this->columnCount() != size) {
        this->setColumnCount(size);
    }
    QDoubleSpinBox * spinBox;
    for (unsigned int index = 0; index < size; ++index) {
        spinBox = dynamic_cast<QDoubleSpinBox*>(this->cellWidget(0, index));
        if(spinBox == 0) {
            spinBox = new QDoubleSpinBox();
            this->setCellWidget(0, index, spinBox);
            spinBox->setDecimals(5);
        }
        spinBox->setValue(vector.Element(index));
    }
    return true;
}

bool vctQtWidgetDynamicVectorDoubleWrite::GetValue(vctDynamicVector<double> & placeHolder) const
{
    int columns = this->columnCount();
    vctDynamicVector<double> newVector(columns);
    QDoubleSpinBox * spinBox;
    for(int column = 0; column < columns; ++column) {
        spinBox = dynamic_cast<QDoubleSpinBox*>(this->cellWidget(1, column));
        newVector.Element(column) = spinBox->value();
    }
    placeHolder.Assign(newVector);
    return true;
}


// -- for ints
vctQtWidgetDynamicVectorIntRead::vctQtWidgetDynamicVectorIntRead(void):
    QTableWidget()
{
    this->setRowCount(1);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    int verticalHeight = this->horizontalHeader()->sizeHint().height() + this->verticalHeader()->sizeHint().height();
    this->setFixedHeight(verticalHeight);
}

bool vctQtWidgetDynamicVectorIntRead::SetValue(const vctDynamicVector<int> & vector)
{
    size_t size = vector.size();
    if (this->columnCount() != size) {
        this->setColumnCount(size);
    }
    QTableWidgetItem * tableItem;
    QString itemValue;
    for (unsigned int index = 0; index < size; ++index) {
        tableItem = this->item(0, index);
        if (tableItem == 0) {
            tableItem = new QTableWidgetItem();
            tableItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            this->setItem(0, index, tableItem);
        }
        itemValue.setNum(vector.Element(index));
        tableItem->setText(itemValue);
    }
    return true;
}

vctQtWidgetDynamicVectorIntWrite::vctQtWidgetDynamicVectorIntWrite(void):
    QTableWidget()
{
    this->setRowCount(1);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    int verticalHeight = this->horizontalHeader()->sizeHint().height() + this->verticalHeader()->sizeHint().height();
    this->setFixedHeight(verticalHeight);
}

bool vctQtWidgetDynamicVectorIntWrite::SetValue(const vctDynamicVector<int> & vector)
{
    size_t size = vector.size();
    if (this->columnCount() != size) {
        this->setColumnCount(size);
    }
    QSpinBox * spinBox;
    for (unsigned int index = 0; index < size; ++index) {
        spinBox = dynamic_cast<QSpinBox*>(this->cellWidget(0, index));
        if(spinBox == 0) {
            spinBox = new QSpinBox();
            this->setCellWidget(0, index, spinBox);
        }
        spinBox->setValue(vector.Element(index));
    }
    return true;
}

bool vctQtWidgetDynamicVectorIntWrite::GetValue(vctDynamicVector<int> & placeHolder) const
{
    int columns = this->columnCount();
    vctDynamicVector<double> newVector(columns);
    QSpinBox * spinBox;
    for(int column = 0; column < columns; ++column) {
        spinBox = dynamic_cast<QSpinBox*>(this->cellWidget(1, column));
        newVector.Element(column) = spinBox->value();
    }
    placeHolder.Assign(newVector);
    return true;
}


// -- for bools
vctQtWidgetDynamicVectorBoolRead::vctQtWidgetDynamicVectorBoolRead(void):
    QTableWidget()
{
    this->setRowCount(1);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    int verticalHeight = this->horizontalHeader()->sizeHint().height() + this->verticalHeader()->sizeHint().height();
    this->setFixedHeight(verticalHeight);
}

bool vctQtWidgetDynamicVectorBoolRead::SetValue(const vctDynamicVector<bool> & vector)
{
    size_t size = vector.size();
    if (this->columnCount() != size) {
        this->setColumnCount(size);
    }
    QTableWidgetItem * tableItem;
    QString itemValue;
    for (unsigned int index = 0; index < size; ++index) {
        tableItem = this->item(0, index);
        if (tableItem == 0) {
            tableItem = new QTableWidgetItem();
            tableItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            this->setItem(0, index, tableItem);
        }
        itemValue.setNum(vector.Element(index));
        tableItem->setText(itemValue);
    }
    return true;
}

vctQtWidgetDynamicVectorBoolWrite::vctQtWidgetDynamicVectorBoolWrite(void):
    QTableWidget()
{
    this->setRowCount(1);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    int verticalHeight = this->horizontalHeader()->sizeHint().height() + this->verticalHeader()->sizeHint().height();
    this->setFixedHeight(verticalHeight);
}

bool vctQtWidgetDynamicVectorBoolWrite::SetValue(const vctDynamicVector<bool> & vector)
{
    size_t size = vector.size();
    if (this->columnCount() != size) {
        this->setColumnCount(size);
    }
    QSpinBox * spinBox;
    for (unsigned int index = 0; index < size; ++index) {
        spinBox = dynamic_cast<QSpinBox*>(this->cellWidget(0, index));
        if(spinBox == 0) {
            spinBox = new QSpinBox();
            this->setCellWidget(0, index, spinBox);
            spinBox->setMinimum(0);
            spinBox->setMaximum(1);
        }
        spinBox->setValue(vector.Element(index));
    }
    return true;
}

bool vctQtWidgetDynamicVectorBoolWrite::GetValue(vctDynamicVector<bool> & placeHolder) const
{
    int columns = this->columnCount();
    vctDynamicVector<double> newVector(columns);
    QSpinBox * spinBox;
    for(int column = 0; column < columns; ++column) {
        spinBox = dynamic_cast<QSpinBox*>(this->cellWidget(1, column));
        newVector.Element(column) = (bool)spinBox->value();
    }
    placeHolder.Assign(newVector);
    return true;
}
