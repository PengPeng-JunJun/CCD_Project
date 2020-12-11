#pragma once

#include "StGigECam.h"

#define ST_CAM(p)	(p->m_Device)

class AFX_EXT_CLASS CStCamera
{
public:
	CStCamera(void);
	~CStCamera(void);

public:
	CBlender<CStGigECam> m_Device;

	void CreateBlendWnd(CWnd * pParent);
	INT_PTR CreateTopWnd(BOOL bModal, BOOL bShowNow = TRUE);
};

