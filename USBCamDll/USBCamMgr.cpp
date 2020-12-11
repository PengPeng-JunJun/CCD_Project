// USBCamMgr.cpp : 实现文件
//

#include "stdafx.h"
#include "USBCamMgr.h"
#include "afxdialogex.h"


#define DeviceInstanceIdSize 256	// 设备实例ID最大长度

// 获取系统的VID和PID集合


// CUSBCamMgr 对话框

IMPLEMENT_DYNAMIC(CUSBCamMgr, CDialogEx)

CUSBCamMgr::CUSBCamMgr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUSBCamMgr::IDD, pParent)
{

}

CUSBCamMgr::~CUSBCamMgr()
{
}

void CUSBCamMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUSBCamMgr, CDialogEx)
END_MESSAGE_MAP()


// CUSBCamMgr 消息处理程序

void CUSBCamMgr::CamInfo()
{
}

int CUSBCamMgr::FindCamera()//查找USB相機數量
{
 	int nUSBCam = 0;
	Mat USBImage;

	m_vFreeCamVIDPID.clear();

	if (WDK_WhoAllVidPid(&m_vVIDPID, 32, &UsbClassGuid, NULL))
	{
		nUSBCam = m_vVIDPID.size();
		for (size_t i = 0; i < m_vVIDPID.size(); i++)
		{
			BOOL bImageValid = FALSE;
			VideoCapture capture;	
			capture.open(i);
			bool stop = false;
			int nFindCamCounter = 0;
			if (nUSBCam == 1)
			{
				while (!stop)        
				{           
					USBImage.release();
					capture >> USBImage;	
					if (!USBImage.empty())
						stop = true;
					if (waitKey(1) >= 0)
						stop = true;																		   
				}
			}
			else
			{
				while (!stop)        
				{           
					USBImage.release();
					capture >> USBImage;	
					nFindCamCounter++;
					if (nFindCamCounter >= 10)
						stop = true;														   
				}
				if (USBImage.empty())
					bImageValid = FALSE;
			}
			if (!USBImage.empty())
			{
				vector<Mat> mChannels;
				split(USBImage, mChannels);
				for (int j = 0; j < USBImage.channels(); j++)
				{
					for (int h = 0; h < USBImage.rows; h++)
					{
						BYTE *pDst = mChannels[j].ptr<BYTE>(h);
						for (int w = 0; w < USBImage.cols; w++)
						{
							if (pDst != NULL)
							{
								if (pDst[w] != 205)//如果是有效像素
								{
									bImageValid = TRUE;
									break;
								}
							}
						}
						if (bImageValid)
						{
							break;
						}
					}
					if (bImageValid)
					{
						break;
					}
				}
			}
			
			capture.release();
			if (bImageValid)
			{
				m_vFreeCamVIDPID.push_back(m_vVIDPID[i]);
			}
		}
		nUSBCam = m_vFreeCamVIDPID.size();
	}
	return nUSBCam;
}

void CUSBCamMgr::GetCamInfo()
{


}


void CUSBCamMgr::USBCamShowImg()
{


}

BOOL CUSBCamMgr:: WDK_WhoAllVidPid(vector<HIDD_VIDPID> *pvVidPid, INT iCapacity, const GUID* SetupClassGuid, const GUID* InterfaceClassGuid )
{
	// 检测入口参数
	pvVidPid->clear();
	if (iCapacity <= 0)
	{
		return FALSE;	
	}
	// 根据设备安装类GUID创建空的设备信息集合
	HDEVINFO DeviceInfoSet = SetupDiCreateDeviceInfoList(SetupClassGuid, NULL );
	if (DeviceInfoSet == INVALID_HANDLE_VALUE) 
	{
		return FALSE;
	}
	// 根据设备安装类GUID获取设备信息集合
	HDEVINFO hDevInfo;
	if(InterfaceClassGuid == NULL)
	{
		hDevInfo = SetupDiGetClassDevsEx( NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_DEVICEINTERFACE | DIGCF_PRESENT, DeviceInfoSet, NULL, NULL );
	}
	else
	{
		hDevInfo = SetupDiGetClassDevsEx( InterfaceClassGuid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT, DeviceInfoSet, NULL, NULL );
	}
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	// 存储设备实例ID	
	TCHAR DeviceInstanceId[DeviceInstanceIdSize];	

	// 存储设备信息数据
	SP_DEVINFO_DATA DeviceInfoData;					
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	// 获取设备信息数据
	DWORD DeviceIndex = 0;
	while (SetupDiEnumDeviceInfo(hDevInfo, DeviceIndex++, &DeviceInfoData))
	{
		// 获取设备实例ID
		if (SetupDiGetDeviceInstanceId(hDevInfo, &DeviceInfoData, DeviceInstanceId, DeviceInstanceIdSize, NULL))
		{
			// 从设备实例ID中提取VID和PID
			TCHAR* pVidIndex = _tcsstr(DeviceInstanceId, TEXT("VID_"));
			if (pVidIndex == NULL) 
			{
				continue;
			}

			TCHAR* pPidIndex = _tcsstr(pVidIndex + 4, TEXT("PID_"));
			if (pPidIndex == NULL) 
			{
				continue;
			}

			HIDD_VIDPID VIDPIDTem;
			VIDPIDTem.VendorID = _tcstoul(pVidIndex + 4, NULL, 16);
			VIDPIDTem.ProductID = _tcstoul(pPidIndex + 4, NULL, 16);

			BOOL bHasSameID = FALSE;

			if (pvVidPid->size() <= 0)//向量里沒喲元素
			{
				pvVidPid->push_back(VIDPIDTem);
			}
			else
			{
				for (size_t i = 0; i < pvVidPid->size(); i++)//遍历向量，查找是否存在相同值
				{
					if ((VIDPIDTem.VendorID == (*pvVidPid)[i].VendorID) && (VIDPIDTem.ProductID == (*pvVidPid)[i].ProductID))
					{
						bHasSameID = TRUE;
						break;
					}
				}
				if (!bHasSameID)
				{
					pvVidPid->push_back(VIDPIDTem);
				}
			}
		}
	}
	return TRUE;
}