#pragma once

class CStDevice;

class CStCallBack
{
public:
	CStCallBack(CStDevice * pDevice);
	~CStCallBack(void);

	void Start(void);
	void Stop(void);

	void SetPriority(int nPriority);
	int GetPriority(void) const;

	BOOL IsDone(void);
	DWORD GetReturnValue(void);

	CStDevice *m_pDevice;

protected:
	static unsigned long WINAPI Link(void * pParam);
	virtual DWORD CallbackFunction(void);
	BOOL IsStopped(void) const;

private:
	HANDLE	m_Handle;
	DWORD	m_dwID;
	BOOL	m_bStop;
	DWORD	m_dwRetVal;
};
