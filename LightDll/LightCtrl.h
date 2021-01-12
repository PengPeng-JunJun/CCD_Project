#pragma once
#include "LitDevice.h"

class AFX_EXT_CLASS CLightCtrl
{
public:
	CLightCtrl(void);
	~CLightCtrl(void);

public:
	CBlender<CLitDevice> m_Device;

	void CreateBlendWnd(CWnd * pParent);
	INT_PTR CreateTopWnd(BOOL bModal, BOOL bShowNow = TRUE);
	void ShowParamWnd(int nShow);

	void Serialize(CArchive& ar);

	int GetBrand(void);
	void SetBrand(int nBrand);

	int GetLightMode(void);
	void SetLightMode(int nMode);

	int GetCNMode(void);

	int GetDelyTime(void);

	BOOL SetChannelStatus(int nChannel, BOOL bStatus);
	BOOL SetChannelIntensity(int nChannel, int nIntensity);
	BOOL GetChannelIntensity(int nChannel, int & nIntensity);

	BOOL Connect();
	BOOL DisConnect();
};

