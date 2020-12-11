#pragma once

template<typename T>
class CNewPtr
{
public:
	CNewPtr(int nNew)
	{
		m_pNew = new T[nNew];
	}

	~CNewPtr(void)
	{
		Delete();
	}

public:
	T *m_pNew;

	void Delete(void)
	{
		delete []m_pNew;
		m_pNew = nullptr;
	}

	void operator =(BYTE * p)
	{
		Delete();

		m_pNew = p;
	}
};
