#pragma once

// 注意, 可以动态的改变m_nSize来改变数组大小, 但是不能比New出来的空间大

// 禁用下面这个警告是因为VC编译器没有实现异常检测,
// 虽然可以这样声明, 但是这个函数还是会抛出声明类型外的异常.
// 这是编译器的问题, 编译器不一定要遵守c++标准
// 用#pragma warning (disable:4290)来屏蔽掉这个警告.
#pragma warning (disable:4290)

#include <vector>
#include <exception>

enum
{
	RADIX_DEC = 10,
	RADIX_HEX = 16
};


#define GET_PTR_MIN_SIZE	const int nMinSize = min(m_nSize, m_nRealSize)


template<typename T>
class CNewPtr
{
public:
	CNewPtr(void)
		: m_pNew(nullptr)
		, m_nSize(0)
		, m_nRealSize(0)
	{
	}

	CNewPtr(int nNew)
	{
		ASSERT(nNew >= 0);

		if (nNew < 0)
		{
			nNew = 0;
		}

		m_nSize = nNew;
		m_nRealSize = nNew;
		m_pNew = new T[nNew];
	}

	CNewPtr(int nNew, T nVal)
	{
		ASSERT(nNew >= 0);

		if (nNew < 0)
		{
			nNew = 0;
		}

		m_nSize = nNew;
		m_nRealSize = nNew;
		m_pNew = new T[nNew];

		memset(m_pNew, nVal, m_nSize * sizeof(T));
	}

	CNewPtr(const CNewPtr<T> * p)
	{
		m_nSize = p->m_nSize;
		m_nRealSize = p->m_nRealSize;

		m_pNew = new T[m_nSize];

		memcpy(m_pNew, p->m_pNew, m_nSize * sizeof(T));
	}

	CNewPtr(const CNewPtr<T> & p)
	{
		m_nSize = p.m_nSize;
		m_nRealSize = p.m_nRealSize;

		m_pNew = new T[m_nSize];

		memcpy(m_pNew, p.m_pNew, m_nSize * sizeof(T));
	}

	~CNewPtr(void)
	{
		Delete();
	}

public:
	T *m_pNew;	
	int m_nSize;
private:
	int m_nRealSize; // m_nSize <= m_nRealSize;
public:

	void Delete(void)
	{
		if (nullptr != m_pNew)
		{
			m_nSize = 0;
			m_nRealSize = 0;

			delete []m_pNew;
			m_pNew = nullptr;
		}
	}

	void ReNew(int nSize)
	{
		ASSERT(nSize >= 0);

		if (nSize < 0)
		{
			nSize = 0;
		}

		Delete();

		m_nSize = nSize;
		m_nRealSize = nSize;
		m_pNew = new T[m_nSize];
	}

	void ReNew(int nSize, T tVal)
	{
		ASSERT(nSize >= 0);

		if (nSize < 0)
		{
			nSize = 0;
		}

		Delete();

		m_nSize = nSize;
		m_nRealSize = nSize;
		m_pNew = new T[nSize];

		memset(m_pNew, tVal, m_nSize * sizeof(T));
	}

	void ReNew2(int nSize, ...)
	{
		ASSERT(nSize >= 0);

		if (nSize < 0)
		{
			nSize = 0;
		}

		Delete();

		m_nSize = nSize;
		m_nRealSize = nSize;
		m_pNew = new T[m_nSize];

		va_list ap = nullptr;
		va_start(ap, nSize);

		for (int i = 0; i < nSize; i++)
		{
			const T t = va_arg(ap, T);

			m_pNew[i] = t;
		}

		va_end(ap);
	}


	int Find(T tWant) const
	{
		GET_PTR_MIN_SIZE;

		const int *pos = std::find(m_pNew, m_pNew + nMinSize, tWant);

		if (m_pNew + nMinSize == pos)
		{
			return -1;
		}

		return pos - m_pNew;
	}

	CNewPtr<T> Slice(int nStart, int nLength) const throw(std::out_of_range)
	{
		GET_PTR_MIN_SIZE;

		if (nStart < 0 || nStart + nLength < 0)
		{
			CString strInfo;
			strInfo.Format(_T("CNewPtr 类型数组 Slice 函数标号越界。\n范围是(%d~%d)，调用函数列表: (%d, %d))")
				-nMinSize, nMinSize - 1, nStart, nLength);

			throw(std::out_of_range(CT2A(strInfo)));

			return CNewPtr<T>();
		}

		if (nLength < 0)
		{
			nStart += nLength;
			nLength *= (-1);
		}

		nLength = min(nLength + nStart, nMinSize);

		CNewPtr<T> Ptr(nLength);

		memcpy(Ptr.m_pNew, m_pNew + nStart, nLength * sizeof(T));

		return Ptr;
	}


	int A2I(int nStart, int nLength, int nBase) const throw(std::out_of_range)
	{
		GET_PTR_MIN_SIZE;

		if (nStart < 0 || nStart + nLength < 0)
		{
			CString strInfo;
			strInfo.Format(_T("CNewPtr 类型数组 A2I 函数标号越界。\n范围是(%d~%d)，调用函数列表: (%d, %d, %d))")
				-nMinSize, nMinSize - 1, nStart, nLength, nBase);

			throw(std::out_of_range(CT2A(strInfo)));
			
			return 0;
		}

		if (nLength < 0)
		{
			nStart += nLength;
			nLength *= (-1);
		}

		nLength = min(nLength + nStart, nMinSize);

		int nRetVal = 0;

		if (RADIX_DEC == nBase)
		{
			for(int i = nStart; i < nLength; i++)
			{
				if (m_pNew[i] >= '0' && m_pNew[i] <= '9')
				{
					const int t = m_pNew[i] - '0';

					nRetVal *= 10;
					nRetVal += t;
				}
			}
		}
		else if (RADIX_HEX == nBase)
		{
			for(int i = nStart; i < nLength; i++)
			{
				int t = 0;

				if (m_pNew[i] >= '0' && m_pNew[i] <= '9')
				{
					t = m_pNew[i] - '0';

					nRetVal <<= 4;
					nRetVal += t;
				}
				else
				{
					t = (m_pNew[i] | 32);

					if (t >= 'a' && t <= 'f')
					{
						t = t - 'a' + 10;

						nRetVal <<= 4;
						nRetVal += t;
					}
				}
			}
		}

		return nRetVal;
	}

	int Hex2I(int nStart, int nLength) const throw(std::out_of_range)
	{
		GET_PTR_MIN_SIZE;

		if (nStart < 0 || nStart + nLength < 0)
		{
			CString strInfo;
			strInfo.Format(_T("CNewPtr 类型数组 Hex2I 函数标号越界。\n范围是(%d~%d)，调用函数列表: (%d, %d))")
				-nMinSize, nMinSize - 1, nStart, nLength);

			throw(std::out_of_range(CT2A(strInfo)));
		}

		if (nLength < 0)
		{
			nStart += nLength;
			nLength *= (-1);
		}

		nLength = min(nLength + nStart, nMinSize);

		int nRetVal = 0;

		for (int i = nStart; i < nLength; i++)
		{
			nRetVal <<= 8;		// 乘以256
			nRetVal += (BYTE)m_pNew[i];
		}

		return nRetVal;
	}

	T & at(int nPos) throw(std::out_of_range)
	{
		GET_PTR_MIN_SIZE;

		if (abs(nPos) > nMinSize)
		{
			CString strInfo;
			strInfo.Format(_T("CNewPtr 类型数组 at 函数标号越界，范围是(%d~%d)，调用索引值: %d。"),
				-nMinSize, nMinSize - 1, nPos);

			throw(std::out_of_range(CT2A(strInfo)));
		}

		if (nPos < 0)
		{
			nPos += m_nSize;
		}

		return m_pNew[nPos];
	}

	void Append(const T & t)
	{
		CNewPtr<T> NewTmp(this);

		ReNew(NewTmp.m_nSize + 1);

		memcpy(m_pNew, NewTmp.m_pNew, sizeof(T) * NewTmp.m_nSize);

		m_pNew[NewTmp.m_nSize] = t;
	}

	void CopyTo(std::vector<T> & v) const
	{
		v.resize(m_nSize);

		for (int i = 0; i < m_nSize; i++)
		{
			v[i] = m_pNew[i];
		}
	}

	void CloneFrom(const std::vector<T> & v)
	{
		ReNew(v.size());

		for (int i = 0; i < m_nSize; i++)
		{
			m_pNew[i] = v[i];
		}
	}

	void CloneFrom(const CString & s)
	{
		USES_CONVERSION;

		const char *p = W2A(s);
		const int nLength = strlen(p);

		ReNew(nLength + 1);
		m_nSize--;
		m_nRealSize--;

		memcpy(m_pNew, p, nLength);

		m_pNew[m_nSize] = 0;
	}
	
	void Trace(void) const
	{
#ifdef _DEBUG

		TRACE(_T("\n------------------------------------"));

		for (int i = 0; i < m_nSize; i++)
		{
			TRACE(_T("\n0x%02X(%d)"), (T)m_pNew[i], (T)m_pNew[i]);
		}
#endif // _DEBUG
	}

	void Trace(int nPos) const
	{
#ifdef _DEBUG

		GET_PTR_MIN_SIZE;

		if (abs(nPos) > nMinSize)
		{
			CString strInfo;
			strInfo.Format(_T("CNewPtr 类型数组 Trace 函数标号越界，范围是(%d~%d)，调用索引值: %d。"),
				-nMinSize, nMinSize - 1, nPos);

			throw(std::out_of_range(CT2A(strInfo)));
		}

		if (nPos < 0)
		{
			nPos += m_nSize;
		}

		TRACE(_T("\n------------------------------------"));

		TRACE(_T("\n0x%02X(%d)"), (T)m_pNew[nPos], (T)m_pNew[nPos]);
#else
		return;

#endif // _DEBUG
	}

	void Trace(int nStart, int nLength) const throw(std::out_of_range)
	{
#ifdef _DEBUG

		GET_PTR_MIN_SIZE;

		if (nStart < 0 || nStart + nLength < 0)
		{
			CString strInfo;
			strInfo.Format(_T("CNewPtr 类型数组 Trace 函数标号越界。\n范围是(%d~%d)，调用函数列表: (%d, %d))")
				-nMinSize, nMinSize - 1, nStart, nLength);

			throw(std::out_of_range(CT2A(strInfo)));
		}

		TRACE(_T("\n------------------------------------"));

		if (nLength < 0)
		{
			nStart += nLength;
			nLength *= (-1);
		}

		nLength = min(nLength + nStart, nMinSize);

		for (int i = nStart; i < nLength; i++)
		{
			TRACE(_T("\n0x%02X(%d)"), (T)m_pNew[i], (T)m_pNew[i]);
		}
#else
		return;

#endif // _DEBUG
	}

	const CNewPtr & operator =(const CNewPtr & p)
	{
		Delete();

		m_nSize = p.m_nSize;
		m_nRealSize = p.m_nRealSize;
		m_pNew = new T[m_nSize];

		memcpy(m_pNew, p.m_pNew, m_nSize * sizeof(T));

		return *this;
	}

	T & operator [](int nPos) throw(std::out_of_range)
	{
		GET_PTR_MIN_SIZE;

		if (abs(nPos) > nMinSize)
		{
			CString strInfo;
			strInfo.Format(_T("CNewPtr 类型数组 [] 操作符标号越界，范围是(%d~%d)，调用索引值: %d。"),
				-nMinSize, nMinSize - 1, nPos);

			throw(std::out_of_range(CT2A(strInfo)));
		}

		if (nPos < 0)
		{
			nPos += m_nSize;
		}

		return m_pNew[nPos];
	}

	CNewPtr<T> & operator + (const T & t)
	{
		Append(t);

		return *this;
	}

	void operator += (const T & t)
	{
		Append(t);
	}

	CNewPtr<T> & operator + (const CNewPtr<T> & t)
	{
		CNewPtr<T> NewTmp(this);

		ReNew(m_nSize + t.m_nSize);

		memcpy(m_pNew, NewTmp.m_pNew, sizeof(T) * NewTmp.m_nSize);
		memcpy(m_pNew + NewTmp.m_nSize, t.m_pNew, sizeof(T) * t.m_nSize);

		return *this;
	}

	void operator += (const CNewPtr<T> & t)
	{
		CNewPtr<T> NewTmp(this);

		ReNew(m_nSize + t.m_nSize);

		memcpy(m_pNew, NewTmp.m_pNew, sizeof(T) * NewTmp.m_nSize);
		memcpy(m_pNew + NewTmp.m_nSize, t.m_pNew, sizeof(T) * t.m_nSize);
	}
};
