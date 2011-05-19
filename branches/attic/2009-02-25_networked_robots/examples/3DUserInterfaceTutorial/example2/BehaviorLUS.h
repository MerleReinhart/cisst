/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: BehaviorLUS.h 309 2009-05-05 01:26:24Z adeguet1 $

  Author(s):	Balazs Vagvolgyi, Simon DiMaio, Anton Deguet
  Created on:	2008-05-23

  (C) Copyright 2008 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/


#include <cisst3DUserInterface.h>
#include <list>


#define DEPTH           -200

// forward declarations
class BehaviorLUSProbeHead;
class BehaviorLUSProbeJoint;
class BehaviorLUSProbeShaft;
class BehaviorLUSBackground;
class BehaviorLUSOutline;
class BehaviorLUSText;
class BehaviorLUSMarker;
struct MarkerType;



class BehaviorLUS : public ui3BehaviorBase
{
public:
    BehaviorLUS(const std::string & name);
    ~BehaviorLUS();

    void Startup(void);
    void Cleanup(void);
    void ConfigureMenuBar(void);
    bool RunForeground(void);
    bool RunBackground(void);
    bool RunNoInput(void);
    void SetUpScene(void);
    void Configure(const std::string & configFile);
    bool SaveConfiguration(const std::string & configFile);
    inline ui3VisibleObject * GetVisibleObject(void) {
        return this->VisibleList;

    //method to queary joint space of the arm
    //method to set joint orientations?


    }
    void SetJoints(double A1, double A2, double insertion, double roll);
    void SetProbeColor(double r, double g, double b);
    void SetText(BehaviorLUSText *obj, const std::string & text);
    void CheckLimits(double p, double y, double i, double r);
    void GetMeasurement(void);
    void AddMarker();
    void RemoveLastMarker();

    void MasterClutchPedalCallback(const prmEventButton & payload);
    void CameraControlPedalCallback(const prmEventButton & payload);
    void DropMarkerCallback(void);
    void RemoveMarkerCallback(void);
    vctFrm3 GetCurrentCursorPositionWRTECM(void);
    vctFrm3 GetCurrentCursorPositionWRTECMRCM(void);
    

protected:
    unsigned long Ticker;
    void FirstButtonCallback(void);
    void EnableMapButtonCallback(void);
    void Master_clutch_callback(void);
    void PrimaryMasterButtonCallback(const prmEventButton & event);
    void SecondaryMasterButtonCallback(const prmEventButton & event);
    void UpdateVisibleMap();

    StateType PreviousState;
    bool PreviousMaM;
    bool RightMTMOpen, prevRightMTMOpen, LeftMTMOpen;
    bool ClutchPressed,CameraPressed, MarkerDropped, MarkerRemoved;
    bool Following;
    bool MapEnabled;
    //    bool isRightMTMOpen(double grip);

    vctDouble3 PreviousCursorPosition;
    vctDouble3 CursorPosition;
    vctDouble3 PreviousSlavePosition;
    vctDouble3 Offset;
    vctDouble3 CursorOffset;
    vctFrm3 Position, ProbePosition;
    

    void OnStreamSample(svlSample* sample, int streamindex);
    ui3ImagePlane* ImagePlane;

    ui3SlaveArm * Slave1;
    ui3SlaveArm * ECM1;
    // ui3MasterArm * RMaster;
    prmPositionCartesianGet Slave1Position;
    prmPositionCartesianGet ECM1Position;
 
    mtsFunctionRead GetJointPositionSlave;
    mtsFunctionRead GetCartesianPositionSlave;
    mtsFunctionRead GetJointPositionECM;
    prmPositionJointGet JointsSlave;
    prmPositionJointGet JointsECM;

    void UpdateMap(vtkMatrix4x4 * Camera2Frame,
                   double *q_ecm,  // q_ecm
                   double *P_psmtip_cam,
                   double *xaxis, double *yaxis, double *zaxis,
                   bool & setCenter); // ANTON TO FIX , double insertion);
#if 0
    mtsFunctionRead GetJointPositionECM;
    prmPositionJointGet JointsECM;
#endif
    bool MeasurementActive;

    bool setCenter;
    vctDouble3 MeasurePoint1;
    
    typedef  std::list<MarkerType*> MarkersType;
    MarkersType Markers;
    

    
    vtkMatrix4x4 * camera2map;
    double          zero_position[2];
    

    

private:

    ui3VisibleList * VisibleList; // all actors for this behavior
    ui3VisibleList * ProbeList; // all actors moving wrt the slave arm
    ui3VisibleList * ProbeListJoint1;
    ui3VisibleList * ProbeListJoint2;
    ui3VisibleList * ProbeListJoint3;
    ui3VisibleList * ProbeListShaft;
    ui3VisibleList * BackgroundList;
    ui3VisibleList * TextList;
    ui3VisibleList * MarkerList;
    ui3VisibleList * MapCursorList;
    ui3VisibleList * AxesList;

    int MarkerCount;
    BehaviorLUSProbeHead  *ProbeHead;
    BehaviorLUSProbeJoint *ProbeJoint1;
    BehaviorLUSProbeJoint *ProbeJoint2;
    BehaviorLUSProbeJoint *ProbeJoint3;
    BehaviorLUSProbeShaft *ProbeShaft;
    BehaviorLUSBackground *Backgrounds;
    BehaviorLUSOutline    *Outline;
    BehaviorLUSText       *WarningText, * MeasureText;
    BehaviorLUSMarker     *MapCursor;
    BehaviorLUSMarker     *M1,*M2,*M3, *M4;
    BehaviorLUSMarker * MyMarkers[20];
    
    ui3VisibleAxes * ProbeAxes;
    ui3VisibleAxes * AxesJoint1;
    ui3VisibleAxes * AxesJoint2;
    ui3VisibleAxes * AxesJoint3;
    ui3VisibleAxes * AxesShaft;

    vctFrm3 ECMtoECMRCM;
    vctFrm3 ECMRCMtoVTK;
    double ECMRCMtoVTKscale;
    vctDouble3 CenterRotatedTranslated;
    
    void                    SetTransform(vtkMatrix4x4 *mat, 
                                         double e11, double e12, double e13, double e14,
                                         double e21, double e22, double e23, double e24,
                                         double e31, double e32, double e33, double e34);
    void                    vectorSum(double A[4], double B[4], double Result[4]);
};
