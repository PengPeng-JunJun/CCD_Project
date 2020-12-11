
#pragma once

#include "..\BlendWndDll\BtLayerWnd.h"

template<typename T>
class CBlender
{
public:
	CBlender()
		: m_pTopWnd(nullptr)
		, m_pOwner(nullptr)
	{
	}

	CBlender(UINT nIDTemplate, CWnd * pParent)
	{
		CreateBlendWnd(nIDTemplate, pParent);
	}

	~CBlender()
	{
		if (nullptr != m_pTopWnd)
		{
			((T *)m_pTopWnd)->PreWndDestroy();

			delete m_pTopWnd;
			m_pTopWnd = nullptr;
		}
	}

public:
	CBtLayerWnd m_BlendWnd;
	T *m_pTopWnd;
private:
	CWnd * m_pOwner;
public:
	void CreateBlendWnd(CWnd * pOwner, BOOL bCreateWnd = TRUE)
	{
		if (bCreateWnd)
		{
			if (nullptr == m_BlendWnd.GetSafeHwnd())
			{
				HINSTANCE exe_hInstance = GetModuleHandle(nullptr);

				HINSTANCE dll_hInstance = GetModuleHandle(_T("BlendWndDll"));
				AfxSetResourceHandle(dll_hInstance); // ÇÐ»»×´Ì¬

				m_BlendWnd.Create(IDD_BTLAYERWND, pOwner);

				AfxSetResourceHandle(exe_hInstance); // »Ö¸´×´Ì¬
			}
		}

		if (nullptr == m_pTopWnd)
		{
			m_pTopWnd = new T(&m_BlendWnd);
		}

		m_pOwner = pOwner;
		m_BlendWnd.m_pTopWnd = m_pTopWnd;
	}

	void CreateBlendWnd(UINT nIDTemplate, CWnd * pOwner, BOOL bCreateWnd = TRUE)
	{
		if (bCreateWnd)
		{
			if (nullptr == m_BlendWnd.GetSafeHwnd())
			{
				HINSTANCE exe_hInstance = GetModuleHandle(nullptr);

				HINSTANCE dll_hInstance = GetModuleHandle(_T("BlendWndDll"));
				AfxSetResourceHandle(dll_hInstance); // ÇÐ»»×´Ì¬

				m_BlendWnd.Create(IDD_BTLAYERWND, pOwner);

				AfxSetResourceHandle(exe_hInstance); // »Ö¸´×´Ì¬
			}
		}

		if (nullptr == m_pTopWnd)
		{
			m_pTopWnd = new T(nIDTemplate, &m_BlendWnd);
		}

		m_pOwner = pOwner;
		m_BlendWnd.m_pTopWnd = m_pTopWnd;
	}

	INT_PTR CreateTopWnd(BOOL bModal, BOOL bShowNow = TRUE)
	{
		INT_PTR nRetVal = IDCANCEL;

		if (nullptr != m_pTopWnd)
		{
			if (m_pTopWnd->GetSafeHwnd())
			{
				return nRetVal;
			}

			nRetVal = m_pTopWnd->CreateTopWnd(bModal, m_pOwner);

			if (!bModal && bShowNow)
			{
				m_pTopWnd->ShowWindow(SW_SHOW);
			}
		}

		return nRetVal;
	}

	INT_PTR CreateTopWnd(const CString & strWnd, BOOL bModal, BOOL bShowNow = TRUE)
	{
		INT_PTR nRetVal = IDCANCEL;

		if (nullptr != m_pTopWnd)
		{
			if (m_pTopWnd->GetSafeHwnd())
			{
				return nRetVal;
			}

			m_pTopWnd->SetTitle(strWnd);
			nRetVal = m_pTopWnd->CreateTopWnd(bModal, m_pOwner);

			if (!bModal && bShowNow)
			{
				m_pTopWnd->ShowWindow(SW_SHOW);
			}
		}

		return nRetVal;
	}

	T * operator ->(void)
	{
		return m_pTopWnd;
	}
};
