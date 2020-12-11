#include "stdafx.h"
#include "HardwareInfo.h"

#define  IDE_ATAPI_IDENTIFY			0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY			0xEC  //  Returns ID sector for ATA.


CHardwareInfo::CHardwareInfo(void)
{
}


CHardwareInfo::~CHardwareInfo(void)
{
}


HANDLE CHardwareInfo::OpenDevice(CString strDevice)
{
	HANDLE hDeviceIOCTL = CreateFile(strDevice,
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr);

	return hDeviceIOCTL;
}


BOOL CHardwareInfo::GetDriveInfo(int nDisk, PDRIVEINFO pInfo)
{
	if (nullptr == pInfo)
	{
		return FALSE;
	}

	BOOL bResult = FALSE;

	CString strDriverName;
	strDriverName.Format(_T("\\\\.\\PhysicalDrive%d"), nDisk);

	HANDLE hDevice = OpenDevice(strDriverName);

	if (INVALID_HANDLE_VALUE == hDevice)
	{
		return bResult;
	}

	PSENDCMDINPARAMS pSCIP;					// 输入数据结构指针
	PSENDCMDOUTPARAMS pSCOP;				// 输出数据结构指针
	DWORD dwOutBytes;						// IOCTL输出数据长度

	// 申请输入/输出数据结构空间   
	pSCIP = (PSENDCMDINPARAMS)::GlobalAlloc(LMEM_ZEROINIT, sizeof(SENDCMDINPARAMS) - 1);
	pSCOP = (PSENDCMDOUTPARAMS)::GlobalAlloc(LMEM_ZEROINIT, sizeof(SENDCMDOUTPARAMS) + sizeof(DRIVEINFO) - 1); // 指定ATA/ATAPI命令的寄存器值

	pSCIP->irDriveRegs.bFeaturesReg		= 0;
	pSCIP->irDriveRegs.bSectorCountReg	= 0;
	pSCIP->irDriveRegs.bSectorNumberReg	= 0;
	pSCIP->irDriveRegs.bCylLowReg		= 0;
	pSCIP->irDriveRegs.bCylHighReg		= 0;
	pSCIP->irDriveRegs.bDriveHeadReg		= 0;
	pSCIP->irDriveRegs.bCommandReg		= IDE_ATA_IDENTIFY; // 指定输入/输出数据缓冲区大小 IDENTIFY DEVICE
	pSCIP->cBufferSize					= 0;
	pSCOP->cBufferSize					= sizeof(DRIVEINFO);

	bResult = ::DeviceIoControl(hDevice,					// 设备句柄
		SMART_RCV_DRIVE_DATA,								// 指定IOCTL
		pSCIP,
		sizeof(SENDCMDINPARAMS) - 1,							// 输入数据缓冲区
		pSCOP,
		sizeof(SENDCMDOUTPARAMS) + sizeof(DRIVEINFO) - 1,	// 输出数据缓冲区
		&dwOutBytes,										// 输出数据长度
		(LPOVERLAPPED)nullptr);								// 复制设备参数结构

	ZeroMemory(pInfo, sizeof(DRIVEINFO));
	::memcpy(pInfo, pSCOP->bBuffer, sizeof(DRIVEINFO));		// 释放输入/输出数据空间

	::GlobalFree(pSCOP);
	::GlobalFree(pSCIP);

	_AdjustString(pInfo->sSerialNumber, 20);
	_AdjustString(pInfo->sModelNumber, 40);
	_AdjustString(pInfo->sFirmwareRev, 8);
	
	::CloseHandle(hDevice);

	return bResult;
}


void CHardwareInfo::GetMacAdd(vector<CString> * pvMac)
{
	pvMac->clear();

	DWORD dwMacAdd = 0;
	IP_ADAPTER_INFO AdapterInfo[16];				// Allocate information
	// for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);			// Save memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(				// Call GetAdapterInfo
		AdapterInfo,								// [out] buffer to receive data
		&dwBufLen);									// [in] size of receive data buffer

	ASSERT(ERROR_SUCCESS == dwStatus);				// Verify return value is
	// valid, no buffer overflow

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;	// Contains pointer to
	// current adapter info
	do
	{
		if (!dwMacAdd)
		{
			dwMacAdd = pAdapterInfo->Address[5] + pAdapterInfo->Address[4] * 256 +
			pAdapterInfo->Address[3] * 256 * 256 + 
			pAdapterInfo->Address[2] * 256 * 256 * 256;
		}

		CString strMac;
		strMac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),
			pAdapterInfo->Address[0], pAdapterInfo->Address[1],
			pAdapterInfo->Address[2], pAdapterInfo->Address[3],
			pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

		pvMac->push_back(strMac);
		
		pAdapterInfo = pAdapterInfo->Next;			// Progress through linked list

	}while(pAdapterInfo);							// Terminate if last adapter
}


void CHardwareInfo::_AdjustString(char *pszData, int nLenght)
{
	char chTemp;
	int i = 0;

	// 若是右对齐的，调整为左对齐, 去掉左边的空格
	i = 0;
	while (i < nLenght && ' ' == pszData[i])
	{
		i++;
	}

	::memmove(pszData, &pszData[i], nLenght - i);

	for (i = nLenght - i; i < nLenght; i++)
	{
		pszData[i] = ' ';
	}

	// 去掉右边的空格
	i = nLenght - 1;

	while(i >= 0 && ' ' == pszData[i])
	{
		pszData[i] = '\0';
		i--;
	}

	for (int i = 0; i < nLenght && ' ' != pszData[i]; i += 2)
	{
		chTemp					= pszData[i];
		pszData[i]		= pszData[i + 1];
		pszData[i + 1]	= chTemp;
	}
}
