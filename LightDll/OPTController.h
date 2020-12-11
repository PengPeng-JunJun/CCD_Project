/*************************************************
Copyright (C), 2005 OPT MACHINE VISION TECH CO.,LTD. All rights reserved.
Version: 1.0.6          
Date: 2014.12.03         
Description: the set of functions that exported for the controller to control light source, such as turn on/off the light source, 
             adjust the intensity.
History: 
Shengping Wang 2014/10/17 1.0.0 build this moudle
Shengping Wang 2014/11/14 1.0.1 fix bug:#001  
Shengping Wang 2014/11/19 1.0.5 fix bug:#002 #003
Shengping Wang 2014/12/03 1.0.6 fix bug:#004
For more details, please refer to the README.txt.
*************************************************/

#ifndef __OPTCONTROLER_H__
#define __OPTCONTROLER_H__

#define DLL_EXPORT __declspec(dllimport)

typedef	  long   OPTController_Handle;

typedef struct OPTController_IntensityItem
{
	int channelIndex;
	int intensity;
}IntensityItem;

typedef struct OPTController_TriggerWidthItem
{
	int channelIndex;
	int triggerWidth;
}TriggerWidthItem;

typedef struct OPTController_HBTriggerWidthItem
{
	int channelIndex;
	int HBTriggerWidth;
}HBTriggerWidthItem;

extern "C"
{
	/*******************************************************************
	Function: OPTController_InitSerialPort
	Description:  initialize an available serial port
	Input(s):  
	    comName  -the name of the serial port. e.g., "COM1"
	Output(s): 
	    controllerHandle -the handle of the controller
	Return: 
	    succeed  -OPT_SUCCEED
	    failed   -OPT_ERR_INITSERIAL_FAILED or OPT_ERR_SERIALPORT_UNOPENED (see the error code in  OPTErrorCode.h)
	See also: OPTController_ReleaseSerialPort
	*******************************************************************/
	long DLL_EXPORT OPTController_InitSerialPort(char *comName, OPTController_Handle *controllerHandle);
	
	/*******************************************************************
	Function: OPTController_ReleaseSerialPort
	Description: release an existing serial port
	Input(s): 
	    controllerHandle -the handle of the controller
	Return:   
	    succeed  -OPT_SUCCEED
		failed   -OPT_ERR_RELEASESERIALPORT_FAILED     
	See also: OPTController_InitSerialPort                                                        
	****************************************************************/
	long DLL_EXPORT OPTController_ReleaseSerialPort(OPTController_Handle controllerHandle);

	/*******************************************************************
	Function: OPTController_CreateEtheConnectionByIP
	Description: create an Ethernet connection by IP address
	Input(s):  
	    serverIPAddress    -the IP of the server. e.g.: IP address of the device which is employed as server. The server IP address can be 127.0.0.1
	Output(s): 
	    controllerHandle   -the handle of the controller
	Return: 
	    succeed   -OPT_SUCCEED
		failed    -OPT_ERR_CREATEETHECON_FAILED        
	See also: OPTController_CancelEtheConnect   
	Remarks:  
	    (1) connect to a server as a client. Before connecting, make sure that the controller is connected to the LAN
		(2) We recommend creating an Ethernet connection by SN (compared with by IP) because IP is likely to be changed dynamically in LAN 
	        under the DHCP protocol. We have provided a tool (SearchForControllers.exe) to check SN.
	*******************************************************************/
	long DLL_EXPORT OPTController_CreateEtheConnectionByIP(char *serverIPAddress, OPTController_Handle *controllerHandle);

	/*******************************************************************
	Function: OPTController_CreateEtheConnectionBySN
	Description: create an Ethernet connection by serial number
	Input(s): 
	    serialNumber     -the serial number of the controller 
	Output(s): 
	    controllerHandle -the handle of the controller
	Return: 
	    succeed   -OPT_ SUCCEED
	    failed    -OPT_ERR_CREATEETHECON_FAILED 
	See also: OPTController_DestroyEtheConnect  
	Remarks:  
	    (1) connect to a server as a client. Before connecting, make sure that the controller is connected to the LAN
	    (2) We recommend creating an Ethernet connection by SN (compared with by IP) because IP is likely to be changed dynamically in LAN 
	        under the DHCP protocol. We have provided a tool (SearchForControllers.exe) to check SN.
	*******************************************************************/
	long DLL_EXPORT OPTController_CreateEtheConnectionBySN(char *serialNumber, OPTController_Handle *controllerHandle);

	/*****************************************************************
	Function: OPTController_DestoryEtheConnection
	Description: destroy an existing Ethernet Connection
	Input(s):    
	    controllerHandle  -the handle of the controller
	Return: 
	    succeed   -OPT_SUCCEED
	    failed    -OPT_ERR_DESTORYETHECON_FAILED
	*****************************************************************/
	long DLL_EXPORT OPTController_DestoryEtheConnection(OPTController_Handle controllerHandle);

	/*******************************************************************
	Function: OPTController_TurnOnChannel
	Description:turn on the specified channel(s)
	Input(s):
	    controllerHandle -the handle of controller
	    channelIndex     -the index(es) of the channel(s) to be turned on, range:[0-16] (in decimal form, 0 for all channels)
	Return: 
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_TURNONCH_FAILED
	See also: OPTController_TurnOffChannel           
	*******************************************************************/
	long DLL_EXPORT OPTController_TurnOnChannel(OPTController_Handle controllerHandle,int channelIndex);

	/*******************************************************************
	Function: OPTController_TurnOnMultiChannel
	Description: turn on the specified multiple channels
	Input(s):
	    controllerHandle   -the handle of controller
	    channelIndexArray  -an array consists of the indexes of the channels to be turned on, range:[1-16] (in decimal form)
		length             -the length of the channel index array
	Return: 
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_TURNONCH_FAILED
	See also: OPTController_TurnOffMultiChannel              
	*******************************************************************/
	long DLL_EXPORT OPTController_TurnOnMultiChannel(OPTController_Handle controllerHandle,int* channelIndexArray, int length);

	/*******************************************************************
	Function: OPTController_TurnOffChannel
	Description: turn off the specified channel(s)
	Input(s):
	    controllerHandle -the handle of controller
	    channelIndex     -the index(es) of the channel(s) to be turned off, range:[0-16] (in decimal form, 0 for all channels);
	Return:
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_TURNOFFCH_FAILED
    See also: OPTController_TurnOnChannel  
	*******************************************************************/
	long DLL_EXPORT OPTController_TurnOffChannel(OPTController_Handle controllerHandle,int channelIndex);

	/*******************************************************************
	Function: OPTController_TurnOffMultiChannel
	Description: turn off the specified multiple channels
	Input(s):
	    controllerHandle  -the handle of controller;
	    channelIndexArray -an array consists of the indexes of the channels to be turned off, range:[1-16] (in decimal form);
		length            -the length of the channel index array
	Return: 
	    succeed    -OPT_SUCCEED;
	    failed     -OPT_ERR_TURNOFFCH_FAILED
    See also: OPTController_TurnOnMultiChannel 
	*******************************************************************/
	long DLL_EXPORT OPTController_TurnOffMultiChannel(OPTController_Handle controllerHandle,int* channelIndexArray, int length);

	/*******************************************************************
	Function: SetIntensity
	Description: set intensity for the specified channel(s)
	Input(s):
	    controllerHandle -the handle of controller
	    channelIndex     -the index(es) of the channel(s), range:[0-16] (in decimal form, 0 for all channels)
	    intensity        -the intensity value, range: [0-255] (in decimal form)
	Return:   
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_SET_INTENSITY_FAILED
    See also: OPTController_ReadIntensity
	*******************************************************************/
	long DLL_EXPORT OPTController_SetIntensity (OPTController_Handle controllerHandle,int channelIndex, int intensity);

	/*******************************************************************
	Function: SetMultiIntensity
	Description: set intensities for the specified multiple  channels
	Input(s):
	    controllerHandle -the handle of controller
	    intensityArray   -an array consists of the intensities (and the indexes of corresponding channels) to be set, range: [0-255] (in decimal form)
		length           -the length of the intensity array
	Return:   
	    succeed    -OPT_SUCCEED
		failed     -OPT_ERR_SET_INTENSITY_FAILED    
    See also: OPTController_ReadIntensity
	*******************************************************************/
	long DLL_EXPORT OPTController_SetMultiIntensity (OPTController_Handle controllerHandle,IntensityItem* intensityArray, int length);

	/*******************************************************************
	Function: ReadIntensity
	Description: read intensity of the specified channel
	Input(s):
	controllerHandle -the handle of controller
	channelIndex     -the index of the channel, range: [1-16] (in decimal form)
	Output(s): 
	    intensity    -the obtained intensity value;
	Return: 
	    succeed    -OPT_SUCCEED
		failed     -OPT_ERR_READ_INTENSITY_FAILED   
	See also: OPTController_SetIntensity & OPTController_SetMultiIntensity
	****************************************************************/
	long DLL_EXPORT OPTController_ReadIntensity (OPTController_Handle controllerHandle,int channelIndex, int *intensity);

	/*******************************************************************
	Function: SetTriggerWidth
	Description: set trigger pulse width for corresponding channel(s)
	Input(s):
	    controllerHandle  -the handle of controller
	    channelIndex      -the index(es) of the channel(s), range:[0-16] (in decimal form, 0 for all channels)
	    triggerWidth      -the value of the trigger pulse width to be set, range:[1-999]
	Return: 
	    succeed    -OPT_SUCCEED
		failed     -OPT_ERR_SET_TRIGGERWIDTH_FAILED        
	See also: OPTController_ReadTriggerWidth
	******************************************************************/
	long DLL_EXPORT OPTController_SetTriggerWidth(OPTController_Handle controllerHandle,int channelIndex, int triggerWidth);

	/*******************************************************************
	Function: SetMultiTriggerWidth
	Description: set trigger pulse width for the specified multiple  channels simultaneously 
	Input(s):
	    controllerHandle   -the handle of controller
	    triggerWidthArray  -an array consists of values of the trigger pulse width (and the indexes of corresponding channels) to be set, range:[1-999]
		length             -the length of the trigger width array
	Return: 
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_SET_TRIGGERWIDTH_FAILED
    See also: OPTController_ReadTriggerWidth
	******************************************************************/
	long DLL_EXPORT OPTController_SetMultiTriggerWidth(OPTController_Handle controllerHandle,TriggerWidthItem* triggerWidthArray, int length);

	/*******************************************************************
	Function: ReadTriggerWidth
	Description: read the trigger pulse width of the specified channel
	Input(s):
	    controllerHandle  -the handle of controller;
	    channelIndex      -the index of the channel, range:[1-16] (in decimal form)
	Output(s):    
	    triggerWidth      -the obtained trigger pulse width;
	Return: 
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_SET_TRIGGERWIDTH_FAILED	    
	See also: OPTController_SetTriggerWidth & OPTController_SetMultiTriggerWidth
	****************************************************************/
	long DLL_EXPORT OPTController_ReadTriggerWidth(OPTController_Handle controllerHandle,int channelIndex, int* triggerWidth);

	/*******************************************************************
	Function: SetHBTriggerWidth
	Description: set the high brightness trigger pulse width for corresponding channel(s);
	Input(s):
	    controllerHandle  -the handle of controller;
	    channelIndex      -the index(es) of the channel(s), range:[0-16] (in decimal form, 0 for all channels);
	    HBTriggerWidth    -the value of the high brightness trigger pulse width to be set, range:[1-500].
	Return: 
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_SET_HBTRIGGERWIDTH_FAILED
	See also: OPTController_ReadHBTriggerWidth
	****************************************************************/
	long DLL_EXPORT OPTController_SetHBTriggerWidth(OPTController_Handle controllerHandle,int channelIndex, int HBTriggerWidth);

	/*******************************************************************
	Function: SetMultiHBTriggerWidth
	Description: set high brightness trigger pulse width for the specified multiple channels
	Input(s):
	    controllerHandle     -the handle of controller;
	    HBtriggerWidthArray  -an array consists of values of the high brightness trigger pulse width (and the indexes of corresponding channels) 
		                      to be set, range:[1-500]
		length               -the length of the high brightness trigger width array
	Return: 
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_SET_HBTRIGGERWIDTH_FAILED
    See also: OPTController_ReadHBTriggerWidth
	****************************************************************/
	long DLL_EXPORT OPTController_SetMultiHBTriggerWidth(OPTController_Handle controllerHandle,HBTriggerWidthItem* HBtriggerWidthArray, int length);

	/*******************************************************************
	Function£ºReadHBTriggerWidth
	Description: read the high brightness trigger pulse width of the specified channel
	Input(s):
	    controllerHandle  -the handle of controller
	    channelIndex      -the index of the channel, range:[1-16] (in decimal form)
	Output(s):  	 
	    HBTriggerWidth    -the obtained high brightness trigger pulse width
	Return: 
	    succeed    -OPT_SUCCEED
 	    failed     -OPT_ERR_READ_HBTRIGGERWIDTH_FAILED
    See also: OPTController_SetHBTriggerWidth & OPTController_SetMultiHBTriggerWidth
	*****************************************************************/
	long DLL_EXPORT OPTController_ReadHBTriggerWidth(OPTController_Handle controllerHandle,int channelIndex,int* HBTriggerWidth);

	/*******************************************************************
	Function: OPTController_EnableResponse
	Description:  to set whether return value are needed or not
	Input:
		controllerHandle  -the handle of controller
		isResponse        -"true" means "need response" while "false" stands for not
	Return: 
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_UNKNOWN
	*****************************************************************/
	long DLL_EXPORT OPTController_EnableResponse(OPTController_Handle controllerHandle,bool isResponse);

	/*******************************************************************
	Function: OPTController_EnableCheckSum
	Description: to set whether checksum are needed or not
	Input:
	    controllerHandle  -the handle of controller
	    isCheckSum        -"true" means "need checksum" while "false" stands for not
	Return: 
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_UNKNOWN
	*****************************************************************/
	long DLL_EXPORT OPTController_EnableCheckSum(OPTController_Handle controllerHandle, bool isCheckSum);

	/*******************************************************************
	Function£ºOPTController_EnablePowerOffBackup
	Description:  to set whether backup are needed or not in the case of power off
	Input:
		controllerHandle   -the handle of controller
		isSave:            -"true" means "need back up" while "false" stands for not
	Return: 
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_UNKNOWN
	*****************************************************************/
	long DLL_EXPORT OPTController_EnablePowerOffBackup(OPTController_Handle controllerHandle,bool isSave);

	/*******************************************************************
	Function: OPTController_ReadSN
	Description: read the serial number (SN) of the controller
	Input:
		controllerHandle  -the handle of controller
	Output:
		SN 	              -the obtained serial number
	Return: 
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_READ_SN_FAILED
	*****************************************************************/
	long DLL_EXPORT OPTController_ReadSN(OPTController_Handle controllerHandle, char *SN);

	/*******************************************************************
	Function: OPTController_ReadIPConfig
	Description: read IP configuration of the controller
	Input:
		OPTController_Handle -the handle of controller
	Output:
		IP                   -the obtained IP address
		subnetMask           -the obtained subnet mask
		defaultGateway       -the obtained default gateway
	Return: 
	    succeed    -OPT_SUCCEED
	    failed     -OPT_ERR_READ_IPCONFIG_FAILED
	*****************************************************************/
	long DLL_EXPORT OPTController_ReadIPConfig(OPTController_Handle controllerHandle, char *IP, char *subnetMask, char *defaultGateway);
};

#endif // __OPTCONTROLER_H__





