#pragma once

#include "Resource.h"

#include <opencv2\opencv.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>

extern "C" {  
#include "setupapi.h" 
#include "hidsdi.h" 
}

using namespace std;
using namespace cv;

#include <SetupAPI.h>
#pragma comment(lib, "Setupapi.lib")

#include "vfw.h"
#pragma comment(lib, "Vfw32.lib")

#define MY_USB_PID_VID	_T("VID_1111&PID_2222")

#include <initguid.h>
#pragma comment(lib,"setupapi.lib")


#include <windows.h>

typedef struct _HIDD_VIDPID
{
	ULONG	VendorID;
	ULONG	ProductID;
} HIDD_VIDPID;


DEFINE_GUID (UsbClassGuid, 0x6BDD1FC6, 0x810F, 0x11D0, 0xBE, 0xC7, 0x08, 0x00, 0x2B, 0xE2, 0x09, 0x2F);


// CUSBCamMgr 对话框

class AFX_EXT_CLASS CUSBCamMgr : public CDialogEx
{
	DECLARE_DYNAMIC(CUSBCamMgr)

public:
	CUSBCamMgr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUSBCamMgr();

// 对话框数据
	enum { IDD = IDD_USBCAMMGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	int FindCamera();//查找USB相機數量


public:

	void USBCamShowImg();
	HWND hWndC;

	void CamInfo();

	void GetCamInfo();

	vector<HIDD_VIDPID> m_vVIDPID;// 計算機上所有的USB相機的VID&PID向量

	vector<HIDD_VIDPID> m_vFreeCamVIDPID;//處於空閒狀態的相機的VID和PID編號

	vector<HIDD_VIDPID> m_vUsedCamVIDPID;//被使用的相機的VID和PID編號


	/*
功能：获取系统所有设备的VIDPID
入口参数：
	[out] pVidPid：存储返回的VIDPID，会自动过滤掉重复的VIDPID
	[in] iCapacity：存储单元的容量，建议为32
	[in] SetupClassGuid：设备安装类GUID，默认为NULL
	[in] InterfaceClassGuid：设备接口类GUID，默认为NULL
返回值：
	获取到的VID和PID数目
优点：
	直接通过设备实例ID提取VIDPID，从而无需获取设备路径来读写IO。
*/

	BOOL WDK_WhoAllVidPid(vector<HIDD_VIDPID> *pvVidPid, INT iCapacity = 32, 
		const GUID* SetupClassGuid = NULL/*設備安裝類GUID*/, 
		const GUID* InterfaceClassGuid = NULL/*設備接口類GUID*/);
};
