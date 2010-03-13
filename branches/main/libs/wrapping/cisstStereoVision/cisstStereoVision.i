/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: cisstStereoVision.i,v 1.4 2009/01/07 05:04:36 pkaz Exp $

  Author(s):	Anton Deguet
  Created on:   2009-01-26

  (C) Copyright 2006-2009 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/


%module cisstStereoVisionPython


%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_pair.i"
%include "std_streambuf.i"
%include "std_iostream.i"

%include "swigrun.i"

%import "cisstConfig.h"

%import "cisstCommon/cisstCommon.i"
%import "cisstVector/cisstVector.i"

%init %{
    import_array() // numpy initialization
%}

%header %{
    // Put header files here
    #include "cisstStereoVision/cisstStereoVision.i.h"
%}

// Generate parameter documentation for IRE
%feature("autodoc", "1");

#define CISST_EXPORT
#define CISST_DEPRECATED


%include "cisstStereoVision/svlInitializer.h"

%include "cisstStereoVision/svlStreamManager.h"


//%include "cisstStereoVision/svlFilterSourceVideoCapture.h"
%include "cisstStereoVision/svlFilterImageRectifier.h"
%include "cisstStereoVision/svlFilterSourceVideoFile.h"
%include "cisstStereoVision/svlFilterVideoFileWriter.h"

%include "cisstStereoVision/svlFilterImageWindow.h"

