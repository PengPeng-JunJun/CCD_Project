// CamMgr.cpp : 实现文件
//

#include "stdafx.h"
#include "CamMgr.h"
#include "afxdialogex.h"


// CCamMgr 对话框

IMPLEMENT_DYNAMIC(CCamMgr, CAppBase)

CCamMgr::CCamMgr(CWnd* pParent /*=NULL*/)
	: CAppBase(CCamMgr::IDD, pParent)
	, m_nCameras(0)
	, m_nNetCount(0)
	, m_pNetInterface(nullptr)
{

}

CCamMgr::~CCamMgr()
{
	Release();
}

void CCamMgr::DoDataExchange(CDataExchange* pDX)
{
	CAppBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCamMgr, CAppBase)
END_MESSAGE_MAP()


// CCamMgr 消息处理程序

int CCamMgr::FindCamera(CWnd * pOwner)
{
	m_pOwner = pOwner;

	PvResult Result;
	PvSystem CamSys;

	Release();

	CamSys.SetDetectionTimeout(1000);		// 设置连接超时时间

	Result = CamSys.Find();					// 查找相机

	if(!Result.IsOK())
	{
#ifdef _DEBUG

		CString strErrInfo = _T("PvSystem::Find Error: ");

#ifdef UNICODE
		strErrInfo += Result.GetCodeString().GetUnicode();
#else
		strErrInfo += Result.GetCodeString().GetAscii();
#endif

		TRACE(strErrInfo + _T("\n"));
#endif

		return -1;
	}

	m_nCameras	= 0;									// 相机数量
	m_nNetCount	= CamSys.GetInterfaceCount();			// 网口数量

	m_pNetInterface = new NET_INTERFACE[m_nNetCount];	// 网口指针

	for (int nNet = 0; nNet < m_nNetCount; nNet++)		// 检索每一个网口
	{
		NET_INTERFACE *pNet = &m_pNetInterface[nNet];

		pNet->nCamSum = 0;
		pNet->pDevice = nullptr;

		PvInterface *pInterface	= CamSys.GetInterface(nNet);
		pNet->nIndex				= nNet;

#ifdef UNICODE
		pNet->strMac				= pInterface->GetMACAddress().GetUnicode();
		pNet->strIP				= pInterface->GetIPAddress().GetUnicode();
		pNet->strSubnetMask		= pInterface->GetSubnetMask().GetUnicode();
#else
		pNet->strMac				= pInterface->GetMACAddress().GetAscii();
		pNet->strIP				= pInterface->GetIPAddress().GetAscii();
		pNet->strSubnetMask		= pInterface->GetSubnetMask().GetAscii();
#endif

		//////////////////////////////////////////////////////////////////////////
		BYTE NetIP[4];
		CString strIpTemp = pNet->strIP;

		for (int i = 0; i < 3; i++)
		{
			const int nFind = strIpTemp.Find('.');
			NetIP[i] = _ttoi(strIpTemp.Left(nFind));

			strIpTemp.Delete(0, nFind + 1);
		}

		NetIP[3] = _ttoi(strIpTemp);
		//////////////////////////////////////////////////////////////////////////

		const int nDevCount	= pInterface->GetDeviceCount();	// 一个网口连接的相机数量
		pNet->bWithCam = nDevCount;					// 标记是否有连接相机

		pNet->nCamSum = nDevCount;					// 相机数量		

		if (nDevCount)
		{
			pNet->pDevice = new CStCamera[nDevCount];

			for (int nDev = 0; nDev < nDevCount; nDev++)
			{
				PvDeviceInfo *pDevInfo = pInterface->GetDeviceInfo(nDev);

				CStCamera *pDev = &pNet->pDevice[nDev];

				pNet->pDevice[nDev].CreateBlendWnd(m_pOwner);

				ST_CAM(pDev)->m_NetCam.nNet	= nNet;
				ST_CAM(pDev)->m_NetCam.nCam	= nDev;				
				ST_CAM(pDev)->m_pMgr			= this;
				ST_CAM(pDev)->m_nCamSum		= nDevCount;
				ST_CAM(pDev)->m_AccType		= pDevInfo->GetAccessStatus();
				ST_CAM(pDev)->m_bUsed		= (PvAccessOpen != ST_CAM(pDev)->m_AccType);

#ifdef UNICODE
				ST_CAM(pDev)->m_strMac		= pDevInfo->GetMACAddress().GetUnicode();
				ST_CAM(pDev)->m_strIp		= pDevInfo->GetIPAddress().GetUnicode();
				ST_CAM(pDev)->m_strSerial	= pDevInfo->GetSerialNumber().GetUnicode();
				ST_CAM(pDev)->m_strManuFacture = pDevInfo->GetManufacturerInfo().GetUnicode();
#else
				ST_CAM(pDev)->m_strMac		= pDevInfo->GetMACAddress().GetAscii();
				ST_CAM(pDev)->m_strIp		= pDevInfo->GetIPAddress().GetAscii();
				ST_CAM(pDev)->m_strSerial	= pDevInfo->GetSerialNumber().GetAscii();
				ST_CAM(pDev)->m_strManuFacture = pDevInfo->GetManufacturerInfo().GetAscii();
#endif

				ST_CAM(pDev)->m_strMac.MakeUpper();
				ST_CAM(pDev)->m_NetCam.strMac = ST_CAM(pDev)->m_strMac;

				//////////////////////////////////////////////////////////////////////////
				BYTE CamIP[4];
				CString strCamIpTemp = ST_CAM(pDev)->m_strIp;

				for (int i = 0; i < 3; i++)
				{
					const int nFind = strCamIpTemp.Find('.');
					CamIP[i] = _ttoi(strCamIpTemp.Left(nFind));
					strCamIpTemp.Delete(0, nFind + 1);
				}

				CamIP[3] = _ttoi(strCamIpTemp);

				CString strMask = pNet->strSubnetMask;
				int nMask = -1;
				int nMaskCount = 0;

				do 
				{
					nMask = strMask.Find(_T("255"));

					if (nMask >= 0)
					{
						nMaskCount++;
						strMask.Delete(nMask, 4);
					}

				} while (nMask >= 0);

				for (int i = 0; i < nMaskCount; i++)
				{
					if (NetIP[i] != CamIP[i])
					{
						ST_CAM(pDev)->m_bUsed = FALSE;
					}
				}
				//////////////////////////////////////////////////////////////////////////
				if (!ST_CAM(pDev)->m_bUsed)
				{
					m_nCameras++;
					m_vNetCam.push_back(ST_CAM(pDev)->m_NetCam);
				}

				ST_CAM(pDev)->m_strMac.MakeUpper();
				ST_CAM(pDev)->m_strIp.Format(_T("%d.%d.%d.%d"), NetIP[0], NetIP[1], NetIP[2], NetIP[3] + nDev + 1);
			}
		}
	}

	for (int nNet = 0; nNet < m_nNetCount; nNet++)
	{
		for (int nCam = 0; nCam < m_pNetInterface[nNet].nCamSum; nCam++)
		{
			CStCamera *pCam = &m_pNetInterface[nNet].pDevice[nCam];

			for (int nX = 0; nX < m_nNetCount; nX++)
			{
				for (int nY = 0; nY < m_pNetInterface[nX].nCamSum; nY++)
				{
					CStCamera *pDst = &m_pNetInterface[nX].pDevice[nY];

					ST_CAM(pDst)->m_vNetIndex.push_back(ST_CAM(pCam)->m_NetCam.nNet + 1);
					ST_CAM(pDst)->m_vCamIndex.push_back(ST_CAM(pCam)->m_NetCam.nCam + 1);
					ST_CAM(pDst)->m_vMacAdd.push_back(ST_CAM(pCam)->m_strMac);
					ST_CAM(pDst)->m_vIpAdd.push_back(ST_CAM(pCam)->m_strIp);
				}
			}
		}
	}

	return m_nCameras;
}


BOOL CCamMgr::Attach(CStCamera ** ppDevice, const CString & strMac)
{
	const int nSize = m_vNetCam.size();

	for (int i = 0; i < nSize; i++)
	{
		if (m_vNetCam[i].strMac == strMac)
		{
			const int nNet = m_vNetCam[i].nNet;
			const int nCam = m_vNetCam[i].nCam;

			m_pNetInterface[nNet].pDevice[nCam].m_Device->m_bUsed = TRUE;
			m_pNetInterface[nNet].pDevice[nCam].m_Device->m_ppCamTach = (void **)ppDevice;

			*ppDevice = &m_pNetInterface[nNet].pDevice[nCam];

			((CStCamera *)(*ppDevice))->m_Device->m_pMgr = this;

			((CStCamera *)(*ppDevice))->m_Device->m_ppCamTach =
				m_pNetInterface[nNet].pDevice[nCam].m_Device->m_ppCamTach;

			return TRUE;
		}
	}

	return FALSE;
}


BOOL CCamMgr::Attach(CStCamera ** ppDevice, int nNet, int nCam)
{
	const int nSize = m_vNetCam.size();

	for (int i = 0; i < nSize; i++)
	{
		if ((m_vNetCam[i].nNet == nNet) &&
			(m_vNetCam[i].nCam == nCam))
		{
			m_pNetInterface[nNet].pDevice[nCam].m_Device->m_bUsed = TRUE;
			m_pNetInterface[nNet].pDevice[nCam].m_Device->m_ppCamTach = (void **)ppDevice;

			*ppDevice = &m_pNetInterface[nNet].pDevice[nCam];

			((CStCamera *)(*ppDevice))->m_Device->m_pMgr = this;

			((CStCamera *)(*ppDevice))->m_Device->m_ppCamTach =
				m_pNetInterface[nNet].pDevice[nCam].m_Device->m_ppCamTach;

			return TRUE;
		}
	}

	return FALSE;
}


BOOL CCamMgr::Attach(CStCamera ** ppDevice, ST_INDEX Index)
{
	const int nSize = m_vNetCam.size();

	for (int i = 0; i < nSize; i++)
	{
		if ((m_vNetCam[i].nNet == Index.nNet) &&
			(m_vNetCam[i].nCam == Index.nCam))
		{
			m_pNetInterface[Index.nNet].pDevice[Index.nCam].m_Device->m_bUsed = TRUE;
			m_pNetInterface[Index.nNet].pDevice[Index.nCam].m_Device->m_ppCamTach = (void **)ppDevice;

			*ppDevice = &m_pNetInterface[Index.nNet].pDevice[Index.nCam];

			((CStCamera *)(*ppDevice))->m_Device->m_pMgr = this;

			((CStCamera *)(*ppDevice))->m_Device->m_ppCamTach =
				m_pNetInterface[Index.nNet].pDevice[Index.nCam].m_Device->m_ppCamTach;

			return TRUE;
		}
	}

	return FALSE;
}


ST_INDEX CCamMgr::Attach(CStCamera ** ppDevice)
{
	ST_INDEX Index = {-1, -1};

	for (int nNet = 0; nNet < m_nNetCount; nNet++)
	{
		const int nSize = m_pNetInterface[nNet].nCamSum;

		for (int nCam = 0; nCam < nSize; nCam++)
		{
			if (!m_pNetInterface[nNet].pDevice[nCam].m_Device->m_bUsed)
			{
				m_pNetInterface[nNet].pDevice[nCam].m_Device->m_bUsed = TRUE;
				m_pNetInterface[nNet].pDevice[nCam].m_Device->m_ppCamTach = (void **)ppDevice;

				*ppDevice = &m_pNetInterface[nNet].pDevice[nCam];

				((CStCamera *)(*ppDevice))->m_Device->m_pMgr = this;

				((CStCamera *)(*ppDevice))->m_Device->m_ppCamTach =
					m_pNetInterface[nNet].pDevice[nCam].m_Device->m_ppCamTach;

				Index.nNet = nNet;
				Index.nCam = nCam;

				return Index;
			}
		}
	}

	return Index;
}


BOOL CCamMgr::Disconnect(int nNet, int nCam)
{
	CStCamera *pCamera = nullptr;
	Attach(&pCamera, nNet, nCam);

	m_pNetInterface[nNet].pDevice[nCam].m_Device->m_ppCamTach = nullptr;

	return ST_CAM(pCamera)->Disconnect();
}


BOOL CCamMgr::Disconnect(ST_INDEX Index)
{
	CStCamera *pCamera = nullptr;
	Attach(&pCamera, Index);

	m_pNetInterface[Index.nNet].pDevice[Index.nCam].m_Device->m_ppCamTach = nullptr;
	return ST_CAM(pCamera)->Disconnect();
}


void CCamMgr::DisconnectUnusedCam(void)
{
	m_vUnusedCam.clear();

	const size_t nUsed = m_vUsedCam.size();

	for (int i = 0; i < m_nCameras; i++)
	{
		CStCamera *pCamera = nullptr;
		Attach(&pCamera, m_vNetCam[i]);

		if (nullptr != pCamera)
		{
			BOOL bFind = FALSE;

			for (size_t j = 0; j < nUsed; j++)
			{
				if (m_vNetCam[i].nNet == m_vUsedCam[j].nNet &&
					m_vNetCam[i].nCam == m_vUsedCam[j].nCam)
				{
					bFind = TRUE;
					break;
				}
			}

			if (!bFind)
			{
				m_vUnusedCam.push_back(m_vNetCam[i]);
			}
		}
	}
	AfxBeginThread(_DisconnecUnusedThread, this);
}


UINT CCamMgr::_DisconnecUnusedThread(LPVOID pParam)
{
	CCamMgr *pMgr = (CCamMgr *)pParam;
	UINT nRetVul = pMgr->_DisconnecUnusedKernal();

	return nRetVul;
}


UINT CCamMgr::_DisconnecUnusedKernal(void)
{
	const size_t nSize = m_vUnusedCam.size();

	for (size_t i = 0; i < nSize; i++)
	{
		Disconnect(m_vUnusedCam[i]);
	}

	return 0;
}


void CCamMgr::Release(void)
{
	for (int nNet = 0; nNet < m_nNetCount; nNet++)
	{
		const int nSize = m_pNetInterface[nNet].nCamSum;

		for (int nCam = 0; nCam < nSize; nCam++)
		{
			if (nullptr != m_pNetInterface[nNet].pDevice[nCam].m_Device->m_ppCamTach)
			{
				*m_pNetInterface[nNet].pDevice[nCam].m_Device->m_ppCamTach = nullptr;
			}
		}

		delete []m_pNetInterface[nNet].pDevice;
		m_pNetInterface[nNet].pDevice = nullptr;
	}

	if (nullptr != m_pNetInterface)
	{
		delete []m_pNetInterface;
		m_pNetInterface = nullptr;
	}

	m_nNetCount = 0;
	m_vNetCam.clear();
}