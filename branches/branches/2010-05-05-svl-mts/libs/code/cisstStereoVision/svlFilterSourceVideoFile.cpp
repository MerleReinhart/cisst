/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$
  
  Author(s):  Balazs Vagvolgyi
  Created on: 2006 

  (C) Copyright 2006-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#include <cisstStereoVision/svlFilterSourceVideoFile.h>
#include <cisstMultiTask/mtsInterfaceProvided.h>
#include <cisstOSAbstraction/osaSleep.h>


#ifdef _MSC_VER
    // Quick fix for Visual Studio Intellisense:
    // The Intellisense parser can't handle the CMN_UNUSED macro
    // correctly if defined in cmnPortability.h, thus
    // we should redefine it here for it.
    // Removing this part of the code will not effect compilation
    // in any way, on any platforms.
    #undef CMN_UNUSED
    #define CMN_UNUSED(argument) argument
#endif


/**********************************************/
/*** svlFilterSourceVideoFile::Config class ***/
/**********************************************/

svlFilterSourceVideoFile::Config::Config() :
    SourceConfig(),
    Channels(0)
{
}

svlFilterSourceVideoFile::Config::Config(const svlFilterSourceVideoFile::Config& objref) :
    SourceConfig(objref)
{
    SetChannels(objref.Channels);
    FilePath  = objref.FilePath;
    Length    = objref.Length;
    Position  = objref.Position;
    Range     = objref.Range;
}

void svlFilterSourceVideoFile::Config::SetChannels(const int channels)
{
    if (channels < 0) return;
    
    FilePath.SetSize(channels);
    Length.SetSize(channels);
    Position.SetSize(channels);
    Range.SetSize(channels);
    
    Channels = channels;
    Length.SetAll(-1);
    Position.SetAll(-1); 
    Range.SetAll(vctInt2(-1, -1));
}

std::ostream & operator << (std::ostream & stream, const svlFilterSourceVideoFile::Config& objref)
{
    stream << objref.Framerate << "Hz, ";
    if (objref.Loop) stream << "loop=ON";
    else stream << "loop=OFF";

    for (int i = 0; i < objref.Channels; i ++) {
        stream << ", ("
        << objref.FilePath[i] << ", "
        << objref.Length[i]   << ", "
        << objref.Position[i] << ", "
        << objref.Range[i][0] << ", "
        << objref.Range[i][1] << ")";
    }
    return stream;
}


/***************************************/
/*** svlFilterSourceVideoFile class ****/
/***************************************/

CMN_IMPLEMENT_SERVICES(svlFilterSourceVideoFile)
CMN_IMPLEMENT_SERVICES_TEMPLATED(svlFilterSourceVideoFile_Config)
CMN_IMPLEMENT_SERVICES_TEMPLATED(svlFilterSourceVideoFile_vctInt2)

svlFilterSourceVideoFile::svlFilterSourceVideoFile() :
    svlFilterSourceBase(false),  // manual timestamp management
    OutputImage(0),
    FirstTimestamp(-1.0),
    NativeFramerate(-1.0)
{
    CreateInterfaces();

    AddOutput("output", true);
    SetAutomaticOutputType(false);
}

svlFilterSourceVideoFile::svlFilterSourceVideoFile(unsigned int channelcount) :
    svlFilterSourceBase(false),  // manual timestamp management
    OutputImage(0),
    FirstTimestamp(-1.0),
    NativeFramerate(-1.0)
{
    CreateInterfaces();

    AddOutput("output", true);
    SetAutomaticOutputType(false);
    SetChannelCount(channelcount);
}

svlFilterSourceVideoFile::~svlFilterSourceVideoFile()
{
    Release();

    if (OutputImage) delete OutputImage;
}

void svlFilterSourceVideoFile::CreateInterfaces()
{
    // Add NON-QUEUED provided interface for configuration management
    mtsInterfaceProvided* provided = AddInterfaceProvided("Settings", MTS_COMMANDS_SHOULD_NOT_BE_QUEUED);
    if (provided) {
        provided->AddCommandRead (&svlFilterSourceVideoFile::confGet,          this, "Get", Settings);
        provided->AddCommandWrite(&svlFilterSourceVideoFile::confSet,          this, "Set");
        provided->AddCommandWrite(&svlFilterSourceVideoFile::confSetChannels,  this, "SetChannels");
        provided->AddCommandWrite(&svlFilterSourceVideoFile::confSetPathL,     this, "SetFilename");
        provided->AddCommandWrite(&svlFilterSourceVideoFile::confSetPathL,     this, "SetLeftFilename");
        provided->AddCommandWrite(&svlFilterSourceVideoFile::confSetPathR,     this, "SetRightFilename");
        provided->AddCommandWrite(&svlFilterSourceVideoFile::confSetPosL,      this, "SetPosition");
        provided->AddCommandWrite(&svlFilterSourceVideoFile::confSetPosL,      this, "SetLeftPosition");
        provided->AddCommandWrite(&svlFilterSourceVideoFile::confSetPosR,      this, "SetRightPosition");
        provided->AddCommandWrite(&svlFilterSourceVideoFile::confSetRangeL,    this, "SetRange");
        provided->AddCommandWrite(&svlFilterSourceVideoFile::confSetRangeL,    this, "SetLeftRange");
        provided->AddCommandWrite(&svlFilterSourceVideoFile::confSetRangeR,    this, "SetRightRange");
        provided->AddCommandWrite(&svlFilterSourceBase::confSetFramerate, dynamic_cast<svlFilterSourceBase*>(this), "SetFramerate");
        provided->AddCommandWrite(&svlFilterSourceBase::confSetLoop,      dynamic_cast<svlFilterSourceBase*>(this), "SetLoop");
    }
}

void svlFilterSourceVideoFile::confGet(svlFilterSourceVideoFile::Config& objref) const
{
    objref.SetChannels(Settings.Channels);

    objref = Settings;
    objref.Framerate = GetTargetFrequency();
    objref.Loop = GetLoop();
    for (int i = 0; i < objref.Channels; i ++) {
        objref.Length[i] = GetLength(i);
        objref.Position[i] = GetPosition(i);
    }
}

void svlFilterSourceVideoFile::confSet(const svlFilterSourceVideoFile::Config& objref)
{
    if (objref.Channels < 0) return;

    SetChannelCount(static_cast<unsigned int>(objref.Channels));
    for (int i = 0; i < objref.Channels; i ++) {
        SetFilePath(objref.FilePath[i], i);
        SetPosition(objref.Position[i], i);
        SetRange(objref.Range[i], i);
    }
    SetTargetFrequency(objref.Framerate);
    SetLoop(objref.Loop);
}

void svlFilterSourceVideoFile::confSetChannels(const int& channels)
{
    SetChannelCount(static_cast<unsigned int>(channels));
}

void svlFilterSourceVideoFile::confSetPathL(const std::string& filepath)
{
    SetFilePath(filepath, SVL_LEFT);
}

void svlFilterSourceVideoFile::confSetPathR(const std::string& filepath)
{
    SetFilePath(filepath, SVL_RIGHT);
}

void svlFilterSourceVideoFile::confSetPosL(const int& position)
{
    SetPosition(position, SVL_LEFT);
}

void svlFilterSourceVideoFile::confSetPosR(const int& position)
{
    SetPosition(position, SVL_RIGHT);
}

void svlFilterSourceVideoFile::confSetRangeL(const vctInt2& range)
{
    SetRange(range, SVL_LEFT);
}

void svlFilterSourceVideoFile::confSetRangeR(const vctInt2& range)
{
    SetRange(range, SVL_RIGHT);
}

int svlFilterSourceVideoFile::SetChannelCount(unsigned int channelcount)
{
    if (OutputImage) return SVL_FAIL;

    if (channelcount == 1) {
        GetOutput()->SetType(svlTypeImageRGB);
        OutputImage = new svlSampleImageRGB;
    }
    else if (channelcount == 2) {
        GetOutput()->SetType(svlTypeImageRGBStereo);
        OutputImage = new svlSampleImageRGBStereo;
    }
    else return SVL_FAIL;

    Codec.SetSize(channelcount);
    Codec.SetAll(0);

    Settings.SetChannels(channelcount);

    return SVL_OK;
}

int svlFilterSourceVideoFile::Initialize(svlSample* &syncOutput)
{
    if (OutputImage == 0) return SVL_FAIL;
    syncOutput = OutputImage;

    Release();

    unsigned int width, height;
    double framerate;
    int ret = SVL_OK;

    for (unsigned int i = 0; i < OutputImage->GetVideoChannels(); i ++) {

        // Get video codec for file extension
        Codec[i] = svlVideoIO::GetCodec(Settings.FilePath[i]);
        // Open video file
        if (!Codec[i] || Codec[i]->Open(Settings.FilePath[i], width, height, framerate) != SVL_OK) {
            ret = SVL_FAIL;
            break;
        }

        if (i == 0) {
            // The first video channel defines the video
            // framerate of all channels in the stream
            NativeFramerate = framerate;
        }

        Settings.Length[i] = Codec[i]->GetEndPos() + 1;
        Settings.Position[i] = Codec[i]->GetPos();

        // Create image sample of matching dimensions
        OutputImage->SetSize(i, width, height);
    }

    // Initialize timestamp for case of timestamp errors
    OutputImage->SetTimestamp(0.0);

    if (ret != SVL_OK) Release();
    return ret;
}

int svlFilterSourceVideoFile::OnStart(unsigned int CMN_UNUSED(procCount))
{
    if (GetTargetFrequency() < 0.1) SetTargetFrequency(NativeFramerate);
    RestartTargetTimer();

    return SVL_OK;
}

int svlFilterSourceVideoFile::Process(svlProcInfo* procInfo, svlSample* &syncOutput)
{
    syncOutput = OutputImage;

    // Try to keep target frequency
    _OnSingleThread(procInfo) WaitForTargetTimer();

    unsigned int idx, videochannels = OutputImage->GetVideoChannels();
    double timestamp, timespan;
    int pos, ret = SVL_OK;

    _ParallelLoop(procInfo, idx, videochannels)
    {
        if (Codec[idx]) {

            pos = Codec[idx]->GetPos();
            Settings.Position[idx] = pos;

            if (Settings.Range[idx][0] >= 0 &&
                Settings.Range[idx][0] <= Settings.Range[idx][1]) {
                // Check if position is outside of the playback segment
                if (pos < Settings.Range[idx][0] ||
                    pos > Settings.Range[idx][1]) {
                    Codec[idx]->SetPos(Settings.Range[idx][0]);
                    ResetTimer = true;
                }
            }

            ret = Codec[idx]->Read(0, *OutputImage, idx, true);
            if (ret == SVL_VID_END_REACHED) {
                if (!GetLoop()) ret = SVL_STOP_REQUEST;
                else {
                    // Loop around
                    ret = Codec[idx]->Read(0, *OutputImage, idx, true);
                }
            }
            if (ret != SVL_OK) break;

            if (idx == 0) {
                if (!IsTargetTimerRunning()) {

                    // Synchronizing all channels to channel #0
                    timestamp = Codec[idx]->GetTimestamp();
                    if (timestamp > 0.0) {

                        // Try to keep orignal frame intervals
                        if (ResetTimer || Codec[idx]->GetPos() == 1) {
                            FirstTimestamp = timestamp;
                            Timer.Reset();
                            Timer.Start();
                            ResetTimer = false;
                        }
                        else {
                            timespan = (timestamp - FirstTimestamp) - Timer.GetElapsedTime();
                            if (timespan > 0.0) osaSleep(timespan);
                        }

                        // Set timestamp to the one stored in the video file
                        OutputImage->SetTimestamp(timestamp);

                        continue;
                    }
                }

                // Ask Stream Manager for current timestamp
                OutputImage->SetTimestamp(-1.0);
            }
        }
    }

    return ret;
}

int svlFilterSourceVideoFile::Release()
{
    for (unsigned int i = 0; i < Codec.size(); i ++) {
        svlVideoIO::ReleaseCodec(Codec[i]);
        Codec[i] = 0;
    }

    if (Timer.IsRunning()) Timer.Stop();

    SetTargetFrequency(-1.0);

    return SVL_OK;
}

void svlFilterSourceVideoFile::OnResetTimer()
{
    ResetTimer = true;
}

int svlFilterSourceVideoFile::DialogFilePath(unsigned int videoch)
{
    if (OutputImage == 0) return SVL_FAIL;
    if (IsInitialized() == true)
        return SVL_ALREADY_INITIALIZED;

    if (videoch >= OutputImage->GetVideoChannels()) return SVL_WRONG_CHANNEL;

    std::ostringstream out;
    out << "Open video file [channel #" << videoch << "]";
    std::string title(out.str());

    return svlVideoIO::DialogFilePath(false, title, Settings.FilePath[videoch]);
}

int svlFilterSourceVideoFile::SetFilePath(const std::string &filepath, unsigned int videoch)
{
    if (OutputImage == 0) return SVL_FAIL;
    if (IsInitialized() == true)
        return SVL_ALREADY_INITIALIZED;

    if (videoch >= OutputImage->GetVideoChannels()) return SVL_WRONG_CHANNEL;

    Settings.FilePath[videoch] = filepath;

    return SVL_OK;
}

int svlFilterSourceVideoFile::GetFilePath(std::string &filepath, unsigned int videoch) const
{
    if (Settings.FilePath.size() <= videoch) return SVL_FAIL;
    filepath = Settings.FilePath[videoch];
    return SVL_OK;
}

int svlFilterSourceVideoFile::SetPosition(const int position, unsigned int videoch)
{
    if (Codec.size() <= videoch || !Codec[videoch]) return SVL_FAIL;
    Codec[videoch]->SetPos(position);
    Settings.Position[videoch] = position;
    ResetTimer = true;
    return SVL_OK;
}

int svlFilterSourceVideoFile::GetPosition(unsigned int videoch) const
{
    if (Codec.size() <= videoch || !Codec[videoch]) return SVL_FAIL;
    return Codec[videoch]->GetPos();
}

int svlFilterSourceVideoFile::SetRange(const vctInt2 range, unsigned int videoch)
{
    if (Codec.size() <= videoch) return SVL_FAIL;
    Settings.Range[videoch] = range;
    return SVL_OK;
}

int svlFilterSourceVideoFile::GetRange(vctInt2& range, unsigned int videoch) const
{
    if (Codec.size() <= videoch) return SVL_FAIL;
    range = Settings.Range[videoch];
    return SVL_OK;
}

int svlFilterSourceVideoFile::GetLength(unsigned int videoch) const
{
    if (Codec.size() <= videoch || !Codec[videoch]) return SVL_FAIL;
    return (Codec[videoch]->GetEndPos() + 1);
}

int svlFilterSourceVideoFile::GetPositionAtTime(const double time, unsigned int videoch) const
{
    if (Codec.size() <= videoch || !Codec[videoch]) return -1;
    return (Codec[videoch]->GetPosAtTime(time));
}

double svlFilterSourceVideoFile::GetTimeAtPosition(const int position, unsigned int videoch) const
{
    if (Codec.size() <= videoch || !Codec[videoch]) return -1.0;
    return (Codec[videoch]->GetTimeAtPos(position));
}

