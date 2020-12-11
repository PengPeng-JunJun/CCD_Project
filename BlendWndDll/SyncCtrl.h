#pragma once

class AFX_EXT_CLASS CSyncCtrl
{
public:
	CSyncCtrl(CSyncObject * pObj);
	CSyncCtrl(CSyncObject * pObj, const CString & strName);
	CSyncCtrl(CSyncObject * pObj, const CString & strName, const CString & strFunc);
	~CSyncCtrl(void);

	void Lock(void);
	void Unlock(void);
protected:
	BOOL m_bLocked;
	CString m_strName;
	CString m_strFunc;
	CSyncObject *m_pObj;
};

