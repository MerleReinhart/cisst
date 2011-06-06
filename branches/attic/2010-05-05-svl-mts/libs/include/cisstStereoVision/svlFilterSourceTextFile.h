/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: $

  Author(s):  Balazs P. Vagvolgyi
  Created on: 2010-07-22

  (C) Copyright 2010 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#ifndef _svlFilterSourceTextFile_h
#define _svlFilterSourceTextFile_h

#include <cisstStereoVision/svlFilterSourceBase.h>
#include <cisstStereoVision/svlBufferSample.h>

// Always include last!
#include <cisstStereoVision/svlExport.h>


class CISST_EXPORT svlFilterSourceTextFile : public svlFilterSourceBase
{
    CMN_DECLARE_SERVICES(CMN_DYNAMIC_CREATION, CMN_LOG_LOD_RUN_ERROR);

public:
    svlFilterSourceTextFile();
    virtual ~svlFilterSourceTextFile();

    void SetErrorValue(const float errorvalue);
    float GetErrorValue() const;
    int SetColumns(const unsigned int columns);
    unsigned int GetColumns() const;
    int AddFile(const std::string& filepath, const int timestamp_column = -1, const double timestamp_unit = cmn_s);
    void RemoveFiles();

protected:
    virtual int Initialize(svlSample* &syncOutput);
    virtual int OnStart(unsigned int procCount);
    virtual int Process(svlProcInfo* procInfo, svlSample* &syncOutput);
    virtual int Release();
    virtual void OnResetTimer();

private:
    svlSampleMatrixFloat OutputMatrix;
    svlSampleMatrixFloat WorkMatrix;
    vctDynamicVector<std::string> FilePaths;
    vctDynamicVector<std::ifstream*> Files;
    vctDynamicVector<int> TimeColumns;
    vctDynamicVector<double> TimeUnits;
    vctDynamicVector<bool> HoldLine;
    vctDynamicVector<char> LineBuffer;
    unsigned int Columns;
    bool ResetPosition;

    vctDynamicVector<double> FirstTimestamps;
    vctDynamicVector<double> Timestamps;
    osaStopwatch Timer;
    float ErrorValue;
    bool ResetTimer;
};

CMN_DECLARE_SERVICES_INSTANTIATION_EXPORT(svlFilterSourceTextFile)

#endif  // _svlFilterSourceTextFile_h
