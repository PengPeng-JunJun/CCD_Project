#include "stdafx.h"
#include "LightCtrl.h"


CLightCtrl::CLightCtrl(void)
{
}


CLightCtrl::~CLightCtrl(void)
{
}


void CLightCtrl::CreateBlendWnd(CWnd * pParent)
{
	m_Device.CreateBlendWnd(IDD_LITDEVICE, pParent);
}


INT_PTR CLightCtrl::CreateTopWnd(BOOL bModal, BOOL bShowNow/* = TRUE*/)
{
	return m_Device.CreateTopWnd(bModal, bShowNow);
}


void CLightCtrl::ShowParamWnd(int nShow)
{
	m_Device->ShowWindow(nShow);
}


void CLightCtrl::Serialize(CArchive& ar)
{
	m_Device->Serialize(ar);
}


int CLightCtrl::GetBrand(void)
{
	return m_Device->GetBrand();
}


void CLightCtrl::SetBrand(int nBrand)
{
	m_Device->SetBrand(nBrand);
}


int CLightCtrl::GetLightMode(void)
{
	return m_Device->GetLightMode();
}


void CLightCtrl::SetLightMode(int nMode)
{
	m_Device->SetLightMode(nMode);
}


int CLightCtrl::GetDelyTime(void)
{
	return m_Device->m_nDelay;
}


BOOL CLightCtrl::SetChannelStatus(int nChannel, BOOL bStatus)
{
	return m_Device->SetChannelStatus(nChannel, bStatus);
}


BOOL CLightCtrl::SetChannelIntensity(int nChannel, int nIntensity)
{
	return m_Device->SetChannelIntensity(nChannel, nIntensity);
}


BOOL CLightCtrl::GetChannelIntensity(int nChannel, int & nIntensity)
{
	return m_Device->GetChannelIntensity(nChannel, nIntensity);
}