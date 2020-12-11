#include "stdafx.h"
#include "StCamera.h"


CStCamera::CStCamera(void)
{
}


CStCamera::~CStCamera(void)
{
}


void CStCamera::CreateBlendWnd(CWnd * pParent)
{
	m_Device.CreateBlendWnd(IDD_STGIGECAM, pParent, FALSE);
}


INT_PTR CStCamera::CreateTopWnd(BOOL bModal, BOOL bShowNow/* = TRUE*/)
{
	return m_Device.CreateTopWnd(bModal, bShowNow);
}
