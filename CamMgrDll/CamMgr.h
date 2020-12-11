#pragma once
#include "Resource.h"

#include "..\SenTechCamDll\StCamera.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\SenTechCamDll.lib")
#else
#pragma comment(lib, "..\\Release\\SenTechCamDll.lib")
#endif


typedef struct _tagNetInterface
{
	int nIndex;						// 网口序号
	BOOL bWithCam;					// 是否有连接相机
	CString strMac;					// Mac地址
	CString strIP;					// IP地址
	CString strSubnetMask;			// 子网掩码
	int nCamSum;					// 相机总数
	CStCamera *pDevice;				// 相机
}NET_INTERFACE;


// CCamMgr 对话框

class AFX_EXT_CLASS CCamMgr : public CAppBase
{
	DECLARE_DYNAMIC(CCamMgr)

public:
	CCamMgr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCamMgr();

// 对话框数据
	enum { IDD = IDD_CAMMGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nCameras;
	int m_nNetCount;
	NET_INTERFACE *m_pNetInterface;
	vector<ST_INDEX> m_vNetCam;
	vector<ST_INDEX> m_vUsedCam;

	int FindCamera(CWnd * pOwner);

	BOOL CCamMgr::Attach(CStCamera ** ppDevice, const CString & strMac);
	BOOL Attach(CStCamera ** ppDevice, int nNet, int nCam);
	BOOL Attach(CStCamera ** ppDevice, ST_INDEX Index);
	ST_INDEX Attach(CStCamera ** ppDevice);

	BOOL Disconnect(int nNet, int nCam);
	BOOL Disconnect(ST_INDEX Index);

	void DisconnectUnusedCam(void);
	void Release(void);
protected:
	static UINT _DisconnecUnusedThread(LPVOID pParam);
	UINT _DisconnecUnusedKernal(void);
private:
	vector<ST_INDEX> m_vUnusedCam;
};
