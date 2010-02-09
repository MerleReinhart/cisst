/*

  Author(s): Simon Leonard
  Created on: Dec 02 2009

  (C) Copyright 2009 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#if (CISST_OS == CISST_LINUX_XENOMAI)

#include <cisstDevices/can/devRTSocketCAN.h>
#include <cisstCommon/cmnLogger.h>

devRTSocketCAN::devRTSocketCAN( const std::string& devicename, 
				devCAN::Rate rate ) : 
  devCAN( rate ){

  // Check if the device name is empty
  if( devicename.empty() ){
    CMN_LOG_RUN_WARNING << CMN_LOG_DETAILS
			<< ": No device name." 
			<< std::endl;
  }
  this->devicename = devicename;

}

devRTSocketCAN::~devRTSocketCAN(){}


devCAN::Errno devRTSocketCAN::Open(){

  struct ifreq ifr;

  // create a socket
  canfd = rt_dev_socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if( canfd < 0 ){
    CMN_LOG_RUN_ERROR << CMN_LOG_DETAILS
		      << ": Couldn't create a CAN socket."
		      << std::endl;
    return devCAN::EFAILURE;
  }

  // Get CAN interface index by name
  strncpy(ifr.ifr_name, devicename.data(), IFNAMSIZ);
  if( rt_dev_ioctl(canfd, SIOCGIFINDEX, &ifr) ){
    CMN_LOG_RUN_ERROR << CMN_LOG_DETAILS
		      << ": Couldn't get the CAN interface index by name."
		      << std::endl;
    return devCAN::EFAILURE;
  }

  // Set CAN filters
  // These are WAM specific filters and don't belong here
  filters[0].can_mask = 0x0000041F;  // mask broadcast to a group
  filters[0].can_id   = 0x00000403;  // allow group 3

  filters[1].can_mask = 0x0000041F;  // mask broadcast to a group
  filters[1].can_id   = 0x00000406;  // allow group 6

  filters[2].can_mask = 0x0000041F;  // mask broadcast to a group
  filters[2].can_id   = 0x00000000;  // allow direct messages to the host

  // Add the filter to the socket
  if( rt_dev_setsockopt(canfd, 
			SOL_CAN_RAW, 
			CAN_RAW_FILTER, 
			filters, 
			3*sizeof(struct can_filter)) ){
    CMN_LOG_RUN_ERROR << CMN_LOG_DETAILS
		      << ": Couldn't set the socket filters." 
		      << std::endl;
    return EFAILURE;
  }

  // Bind the socket to the local address
  memset(&addr, 0, sizeof(addr));     // clear the address
  addr.can_ifindex = ifr.ifr_ifindex; // ifr_ifindex was set from SIOCGIFINDEX
  addr.can_family = AF_CAN;           // Address Family CAN

  if( rt_dev_bind(canfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_can))){
    CMN_LOG_RUN_ERROR << CMN_LOG_DETAILS
		      << ": Couldn't bind the socket." 
		      << std::endl;
    return EFAILURE;
  }

  // set the baud rate
  can_baudrate_t* can_baudrate = (can_baudrate_t*)&ifr.ifr_ifru;
  *can_baudrate = rate;
  if( rt_dev_ioctl( canfd, SIOCSCANBAUDRATE, &ifr ) ){
    CMN_LOG_RUN_ERROR << CMN_LOG_DETAILS 
		      << ": Couldn't set the rate."
		      << std::endl;
    return EFAILURE;
  }

  /*
  can_ctrlmode_t* can_ctrlmode = (can_ctrlmode_t *)&ifr.ifr_ifru;
  *can_ctrlmode = CAN_CTRLMODE_LISTENONLY; // is this correct?
  if( rt_dev_ioctl(canfd, SIOCSCANCTRLMODE, &ifr) ){
    perror("devRTSocketCAN::open: Couldn't set the control mode: ");
    return FAILURE;
  }
  */

  // Set the mode 
  CAN_MODE* mode = (CAN_MODE*)&ifr.ifr_ifru;
  *mode = CAN_MODE_START;
  if( rt_dev_ioctl(canfd, SIOCSCANMODE, &ifr) ){
    CMN_LOG_RUN_ERROR << CMN_LOG_DETAILS 
		      << ": Couldn't set the operation mode."
		      << std::endl;
    return EFAILURE;
  }

  /*
  nanosecs_rel_t timeout = 50000000;
  if (rt_dev_ioctl(canfd, RTCAN_RTIOC_SND_TIMEOUT, &timeout) ){
    perror("devRTSocketCAN::open: Couldn't set the send timeout: ");
    return FAILURE;
  }

  if( rt_dev_ioctl(canfd, RTCAN_RTIOC_RCV_TIMEOUT, &timeout) ){
    perror("devRTSocketCAN::open: Couldn't set the recv timeout: ");
    return FAILURE;
  }
  */

  return ESUCCESS;
}

devCAN::Errno devRTSocketCAN::Close(){
  // close the socket
  if( rt_dev_close( canfd ) ){
    CMN_LOG_RUN_ERROR << CMN_LOG_DETAILS
		      << "Couldn't close the socket."
		      << std::endl;
    return EFAILURE;
  }
  return ESUCCESS;
}

// Send a can frame
// Note that block is useless for Socket CAN
devCAN::Errno devRTSocketCAN::Send( const devCANFrame& canframe, 
				    devCAN::Flags  ){

  // copy the data in to a RTSocket CAN frame
  // can_frame_t is defined in xenomai/include/rtdm/rtcan.h
  can_frame_t frame;
  frame.can_id = (can_id_t)canframe.GetID();
  frame.can_dlc = (uint8_t)canframe.GetLength();  

  const uint8_t* data = (const uint8_t*)canframe.GetData();
  for(size_t i=0; i<8; i++)
    { frame.data[i] = data[i]; }

  // send the frame
  int error = rt_dev_sendto( canfd, 
			     (void*)&frame, 
			     sizeof(can_frame_t), 
			     0,
			     (struct sockaddr*)&addr, 
			     sizeof(addr) );

  if( error < 0 ){
    CMN_LOG_RUN_ERROR << CMN_LOG_DETAILS 
		      << ": Failed to send CAN frame." 
		      << std::endl;
    return EFAILURE;
  }
  
  return ESUCCESS;
}

// Receive a CAN frame
devCAN::Errno devRTSocketCAN::Recv( devCANFrame& canframe, 
				    devCAN::Flags ){

  struct can_frame frame;            // the RT Socket CAN frame
  memset(&frame, 0, sizeof(frame));  // clear the frame

  struct sockaddr_can addr;          // the source address
  socklen_t addrlen = sizeof(addr);  // the size of the source address

  int error =  rt_dev_recvfrom( canfd, 
				(void*)&frame, 
				sizeof(can_frame_t), 
				0,
				(struct sockaddr*)&addr, 
				&addrlen );

  if( error < 0 ){
    CMN_LOG_RUN_ERROR << CMN_LOG_DETAILS
		      << ": Failed to receive the frame." 
		      << std::endl;
    return EFAILURE;
  }

  // create a devCANFrame
  canframe = devCANFrame( frame.can_id, frame.data, frame.can_dlc );

  return ESUCCESS;
}

#endif
