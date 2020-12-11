#ifndef __OPTERRORCODE_H__
#define __OPTERRORCODE_H__

#define   OPT_SUCCEED			                           0	 //operation succeed
#define   OPT_ERR_INVALIDHANDLE						 3001001     //invalid handle
#define   OPT_ERR_UNKNOWN                            3001002     //error unknown 
#define   OPT_ERR_INITSERIAL_FAILED                  3001003	 //failed to initialize a serial port
#define   OPT_ERR_RELEASESERIALPORT_FAILED           3001004	 //failed to release a serial port
#define   OPT_ERR_SERIALPORT_UNOPENED                3001005	 //attempt to access an unopened serial port
#define   OPT_ERR_CREATEETHECON_FAILED               3001006     //failed to create an Ethernet connection
#define   OPT_ERR_DESTORYETHECON_FAILED              3001007     //failed to destroy an Ethernet connection
#define   OPT_ERR_SN_NOTFOUND						 3001008     //SN is not found
#define   OPT_ERR_TURNONCH_FAILED                    3001009	 //failed to turn on the specified channel(s)
#define   OPT_ERR_TURNOFFCH_FAILED                   3001010	 //failed to turn off the specified channel(s)
#define   OPT_ERR_SET_INTENSITY_FAILED               3001011     //failed to set the intensity for the specified channel(s)
#define   OPT_ERR_READ_INTENSITY_FAILED              3001012     //failed to read the intensity for the specified channel(s)
#define   OPT_ERR_SET_TRIGGERWIDTH_FAILED	         3001013	 //failed to set trigger pulse width
#define   OPT_ERR_READ_TRIGGERWIDTH_FAILED           3001014	 //failed to read trigger pulse width	
#define   OPT_ERR_READ_HBTRIGGERWIDTH_FAILED         3001015	 //failed to read high brightness trigger pulse width
#define   OPT_ERR_SET_HBTRIGGERWIDTH_FAILED          3001016	 //failed to set high brightness trigger pulse width
#define   OPT_ERR_READ_SN_FAILED                     3001017	 //failed to read serial number
#define   OPT_ERR_READ_IPCONFIG_FAILED               3001018	 //failed to read IP address
#define   OPT_ERR_CHINDEX_OUTRANGE	                 3001019     //index(es) out of the range
#define   OPT_ERR_WRITE_FAILED		                 3001020     //failed to write data
#define   OPT_ERR_PARAM_OUTRANGE	                 3001021     //parameter(s) out of the range 

#endif