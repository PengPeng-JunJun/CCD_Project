#pragma once

// 以下两个类是用于变量在指定的作用区域内复原

template<typename T>
class CValRst
{
public:
	CValRst(T * pVal, const T & RstVal, BOOL bEnable = TRUE)
		: m_pVal(pVal)
		, m_RstVal(RstVal)
		, m_bEnable(bEnable)
	{
	}

	~CValRst(void)
	{
		if (m_bEnable && (nullptr != m_pVal))
		{
			*m_pVal = m_RstVal;
		}
		
		m_bEnable = FALSE;
	}

public:
	void ResetValue(const T & RstVal)
	{
		m_RstVal = RstVal;
	}

	void Enable(void)
	{
		m_bEnable = TRUE;
	}

	void Disable(void)
	{
		m_bEnable = FALSE;
	}

protected:
	T *m_pVal;			// 变量指针
	T m_RstVal;			// 重设值
	BOOL m_bEnable;		// 使能状态
};


class CCallBackRst
{
	typedef void (*pRst)(void);

public:
	CCallBackRst(pRst pfn, BOOL bEnalbe = TRUE)
		: m_pFun(pfn)
		, m_bEnable(bEnalbe)
	{
	}

	~CCallBackRst(void)
	{
		if (m_bEnable && nullptr != m_pFun)
		{
			(*m_pFun)();
		}

		m_bEnable = FALSE;
	}

public:
	void Enable(void)
	{
		m_bEnable = TRUE;
	}

	void Disable(void)
	{
		m_bEnable = FALSE;
	}

protected:
	pRst m_pFun;		// 回调函数指针
	BOOL m_bEnable;		// 使能状态
};
