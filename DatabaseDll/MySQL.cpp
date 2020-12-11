#include "stdafx.h"
#include "MySQL.h"

#ifdef DATABASE_ENABLE

CMySQL::CMySQL(void)
	: m_bConnected(FALSE)
{
}


CMySQL::~CMySQL(void)
{
	if (m_bConnected)
	{
		Disconnect();
	}
}


BOOL CMySQL::Init(void)
{
	return nullptr != mysql_init(&m_Sql);
}


BOOL CMySQL::IsConnected(void)
{
	return m_bConnected;
}


BOOL CMySQL::Connet(const CString & strHost, unsigned int nPort,
					const CString & strDataBase, const CString & strUser, const CString & strPsd)
{
	if (m_bConnected)
	{
		return TRUE;
	}

	USES_CONVERSION;

	char bReconntect = 1;
	int nRes = mysql_options(&m_Sql, MYSQL_OPT_RECONNECT, &bReconntect);

	m_bConnected = nullptr != mysql_real_connect(&m_Sql, W2A(strHost), W2A(strUser), W2A(strPsd),
		W2A(strDataBase), nPort, nullptr, 0);

	m_strHost = strHost;

	if (m_bConnected)
	{
		m_strDbName = strDataBase;
	}
	else
	{
		TRACE(_T("\n%s"), _GetErrInfo());
	}

	return m_bConnected;
}


BOOL CMySQL::Disconnect(void)
{
	if (!m_bConnected)
	{
		return TRUE;
	}

	mysql_close(&m_Sql);

	m_bConnected = FALSE;

	return TRUE;
}

int CMySQL::SetCharSet(const CString & strName)
{
	USES_CONVERSION;

	const int nRes = mysql_set_character_set(&m_Sql, W2A(strName));
//	const int nRes = mysql_options(&m_Sql, MYSQL_SET_CHARSET_NAME, W2A(strName));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


int CMySQL::CreateDatabase(const CString & strName)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;

	strQuery.Format(_T("CREATE DATABASE `%s`"), strName);

	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


int CMySQL::SelectDatabase(const CString & strName)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("USE `%s`"), strName);

	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}
	else
	{
		m_strDbName = strName;
	}

	return nRes;
}


int CMySQL::IsDbExist(const CString & strName, BOOL & bExist)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT COUNT(*) FROM information_schema.schemata WHERE schema_name='%s'"), strName);

	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 2;
	}

	const my_ulonglong nRows = mysql_num_rows(pRes);

	for (my_ulonglong i = 0; i < nRows; i++)
	{
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			bExist = _ttoi(A2W(*SqlRow));
		}
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::DeleteDatabase(const CString & strName)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("DROP DATABASE `%s`"), strName);

	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


int CMySQL::GetDbName(CString * pstrName)
{
	ASSERT(nullptr != pstrName);

	if (nullptr == pstrName)
	{
		return 2;
	}

	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT DATABASE()"));
	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 3;
	}

	const my_ulonglong nRows = mysql_num_rows(pRes);

	for (my_ulonglong i = 0; i < nRows; i++)
	{
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			*pstrName = A2W(*SqlRow);
		}
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::CreateTable(const CString & strTable, const CString & strColsInfo)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("CREATE TABLE `%s` (%s) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8, COLLATE=utf8_general_ci"),
		strTable, strColsInfo);

	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


int CMySQL::SelectTable(const CString & strTable)
{
	m_strTbName = strTable;

	return TRUE;
}


int CMySQL::IsTableExist(const CString & strName, BOOL & bExist)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT COUNT(*) FROM information_schema.TABLES t WHERE t.TABLE_SCHEMA = '%s' AND t.TABLE_NAME = '%s'"), m_strDbName, strName);

	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 2;
	}

	const my_ulonglong nRows = mysql_num_rows(pRes);

	for (my_ulonglong i = 0; i < nRows; i++)
	{
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			bExist = _ttoi(A2W(*SqlRow));
		}
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::IsTableExist(const CString & strDb, const CString & strTable, BOOL & bExist)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT COUNT(*) FROM information_schema.TABLES t WHERE t.TABLE_SCHEMA = '%s' AND t.TABLE_NAME = '%s'"), strDb, strTable);

	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 2;
	}

	const my_ulonglong nRows = mysql_num_rows(pRes);

	for (my_ulonglong i = 0; i < nRows; i++)
	{
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			bExist = _ttoi(A2W(*SqlRow));
		}
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::DeleteTable(const CString & strTable)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("DROP TABLE `%s`"), strTable);

	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


int CMySQL::GetColsType(const CString & strTable, std::vector<CString> * pvstrType)
{
	ASSERT(nullptr != pvstrType);

	if (nullptr == pvstrType)
	{
		return 2;
	}

	pvstrType->clear();

	CString strDbName;
	int nRes = GetDbName(&strDbName);

	if (nRes)
	{
		return nRes;
	}

	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT `DATA_TYPE` FROM `information_schema`.`COLUMNS` WHERE `TABLE_SCHEMA` = '%s' AND `TABLE_NAME` = '%s'"),
		strDbName, strTable);

	nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 3;
	}

	const my_ulonglong nRows = mysql_num_rows(pRes);

	for (my_ulonglong i = 0; i < nRows; i++)
	{
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			const CString strTemp = A2W(*SqlRow);
			pvstrType->push_back(strTemp);
		}		
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::GetColType(const CString & strTable, const CString & strCol, CString * pstType)
{
	ASSERT(nullptr != pstType);

	if (nullptr == pstType)
	{
		return 2;
	}

	CString strDbName;
	int nRes = GetDbName(&strDbName);

	if (nRes)
	{
		return nRes;
	}

	USES_CONVERSION;
	
	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT `DATA_TYPE` FROM `information_schema`.`COLUMNS` WHERE `TABLE_SCHEMA` = '%s' AND `TABLE_NAME` = '%s' AND `COLUMN_NAME` = '%s'"),
		strDbName, strTable, strCol);

	nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 3;
	}

	const my_ulonglong nRows = mysql_num_rows(pRes);	

	for (my_ulonglong i = 0; i < nRows; i++)
	{
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			*pstType = A2W(*SqlRow);
		}
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::Insert(const CString & strInfo)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	const int nRes = mysql_real_query(&m_Sql, W2A(strInfo), (unsigned long)strlen(W2A(strInfo)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}	

	return nRes;
}


int CMySQL::Insert(const CString & strTable, CString strInfo)
{
	std::vector<CString> vstrTypes;

	int nRes = GetColsType(strTable, &vstrTypes);

	if (nRes)
	{
		return nRes;
	}

	CSyncCtrl s(&m_seSql);

	int nCol = 0;
	int nSegPos = -1;
	std::vector<CString> vstrValues;

LABEL_REFIND:
	nSegPos = strInfo.Find(',');

	if (nSegPos > 0)
	{
		CString strTemp = strInfo.Left(nSegPos);

		if (_T("tinytext") == vstrTypes[nCol] ||
			_T("text") == vstrTypes[nCol] ||
			_T("mediumtext") == vstrTypes[nCol] ||
			_T("longtext") == vstrTypes[nCol])
		{
			strTemp.Insert(0, '\'');
			strTemp.Append(_T("'"));
		}

		vstrValues.push_back(strTemp);

		strInfo.Delete(0, nSegPos + 1);

		nCol++;
		goto LABEL_REFIND;
	}

	if (_T("tinytext") == vstrTypes[nCol] ||
		_T("text") == vstrTypes[nCol] ||
		_T("mediumtext") == vstrTypes[nCol] ||
		_T("longtext") == vstrTypes[nCol])
	{
		strInfo.Insert(0, '\'');
		strInfo.Append(_T("'"));
	}

	vstrValues.push_back(strInfo);

	strInfo.Empty();

	const size_t nSize = vstrValues.size();

	for (size_t i = 0; i < nSize; i++)
	{
		strInfo.Append(vstrValues[i]);

		if (i < nSize - 1)
		{
			strInfo.Append(_T(","));
		}
	}

	USES_CONVERSION;

	CString strQuery;
	strQuery.Format(_T("INSERT INTO `%s` VALUES (%s)"), strTable, strInfo);
	nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


int CMySQL::Insert(const CString & strTable, const CString & strCols, CString strValue)
{
	std::vector<CString> vstrTypes;

	int nRes = GetColsType(strTable, &vstrTypes);

	if (nRes)
	{
		return nRes;
	}

	CSyncCtrl s(&m_seSql);

	int nCol = 0;
	int nSegPos = -1;	
	std::vector<CString> vstrValues;

LABEL_REFIND:
	nSegPos = strValue.Find(',');

	if (nSegPos > 0)
	{
		CString strTemp = strValue.Left(nSegPos);

		if (_T("tinytext") == vstrTypes[nCol] ||
			_T("text") == vstrTypes[nCol] ||
			_T("mediumtext") == vstrTypes[nCol] ||
			_T("longtext") == vstrTypes[nCol])
		{
			strTemp.Insert(0, '\'');
			strTemp.Append(_T("'"));
		}

		vstrValues.push_back(strTemp);

		strValue.Delete(0, nSegPos + 1);

		nCol++;
		goto LABEL_REFIND;
	}

	if (_T("tinytext") == vstrTypes[nCol] ||
		_T("text") == vstrTypes[nCol] ||
		_T("mediumtext") == vstrTypes[nCol] ||
		_T("longtext") == vstrTypes[nCol])
	{
		strValue.Insert(0, '\'');
		strValue.Append(_T("'"));
	}

	vstrValues.push_back(strValue);

	strValue.Empty();

	const size_t nSize = vstrValues.size();

	for (size_t i = 0; i < nSize; i++)
	{
		strValue.Append(vstrValues[i]);

		if (i < nSize - 1)
		{
			strValue.Append(_T(","));
		}
	}

	USES_CONVERSION;

	CString strQuery;
	strQuery.Format(_T("INSERT INTO `%s` (%s) VALUES (%s)"), strTable, strCols, strValue);
	nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());	
	}

	return nRes;
}


int CMySQL::Delete(const CString & strTable, const CString & strConition)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("DELETE FROM `%s` WHERE %s"), strTable, strConition);
	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


int CMySQL::ClearTable(const CString & strTable)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("DELETE FROM `%s`"), strTable);
	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


int CMySQL::Update(const CString & strTable, CString strInfo)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("UPDATE `%s` %s"), strTable, strInfo);
	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


int CMySQL::SetItemText(const CString & strTable, const CString & strCol, const CString & strCondition, const CString & strNew)
{
	CString strType;

	int nRes = GetColType(strTable, strCol, &strType);

	if (nRes)
	{
		return nRes;
	}

	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;

	if (_T("tinytext") == strType ||
		_T("text") == strType ||
		_T("mediumtext") == strType ||
		_T("longtext") == strType)
	{
		strQuery.Format(_T("UPDATE `%s` SET `%s` = '%s' WHERE %s"), strTable, strCol, strNew, strCondition);
	}
	else
	{
		strQuery.Format(_T("UPDATE `%s` SET `%s` = %s WHERE %s"), strTable, strCol, strNew, strCondition);
	}

	nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


int CMySQL::GetItemText(const CString & strTable, const CString & strCol, const CString & strCondition, std::vector<CString> * pvText)
{
	ASSERT(nullptr != pvText);

	if (nullptr == pvText)
	{
		return 2;
	}

	pvText->clear();

	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT `%s` FROM `%s` WHERE %s"), strCol, strTable, strCondition);	
	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 3;
	}

	const my_ulonglong nRows = mysql_num_rows(pRes);

	for (my_ulonglong i = 0; i < nRows; i++)
	{
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			CString strTemp = A2W(*SqlRow);
			pvText->push_back(strTemp);
		}		
	}

	mysql_free_result(pRes);

	return 0;
}


int CMySQL::GetRowText(const CString & strTable, const CString & strCondition, std::vector<std::vector<CString>> * pvvText)
{
	ASSERT(nullptr != pvvText);

	if (nullptr == pvvText)
	{
		return 2;
	}

	pvvText->clear();

	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT * FROM `%s` WHERE %s"), strTable, strCondition);
	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 3;
	}

	const my_ulonglong nRows = mysql_num_rows(pRes);

	std::vector<CString> vstrCols;

	for (my_ulonglong i = 0; i < nRows; i++)
	{
		vstrCols.clear();
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			for (my_ulonglong j = 0; j < mysql_num_fields(pRes); j++)
			{
				CString strTemp = A2W(SqlRow[j]);
				vstrCols.push_back(strTemp);
			}

			pvvText->push_back(vstrCols);
		}		
	}

	mysql_free_result(pRes);

	return 0;
}


int CMySQL::GetRows(const CString & strTable, my_ulonglong & nRows)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT COUNT(*) FROM `%s`"), strTable);
	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		nRows = 0;

		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 2;
	}

	const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

	if (nullptr != *SqlRow)
	{
		nRows = _ttoi(A2W(*SqlRow));
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::GetCols(const CString & strTable, my_ulonglong & nCols)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT COUNT(*) FROM `information_schema`.`COLUMNS` WHERE `TABLE_SCHEMA`='%s' and `TABLE_NAME`='%s'"),
		m_strDbName, strTable);

	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		nCols = 0;

		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 2;
	}

	const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

	if (nullptr != *SqlRow)
	{
		nCols = _ttoi(A2W(*SqlRow));
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::GetColSum(const CString & strTable, const CString & strCol, const CString & strCondition, double & dSum)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT SUM(`%s`) FROM `%s` WHERE %s"), strCol, strTable, strCondition);
	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		dSum = 0;

		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 2;
	}

	const my_ulonglong nRows = mysql_num_rows(pRes);

	if (nRows)
	{
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			dSum = _ttof(A2W(*SqlRow));
		}
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::GetDistRows(const CString & strTable, const CString & strCondition, my_ulonglong & nRows)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT COUNT(*) FROM `%s` WHERE %S"), strTable, strCondition);
	const int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		nRows = 0;

		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 2;
	}

	const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

	if (nullptr != *SqlRow)
	{
		nRows = _ttoi(A2W(*SqlRow));
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::GetCurTime(SYSTEMTIME & t)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT NOW()"));
	int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 2;
	}

	my_ulonglong nRows = mysql_num_rows(pRes);

	if (nRows)
	{
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			CString strTime = A2W(*SqlRow);

			const CString strYear = strTime.Left(4);
			strTime.Delete(0, 5);

			const CString strMonth = strTime.Left(2);
			strTime.Delete(0, 3);

			const CString strDay = strTime.Left(2);
			strTime.Delete(0, 3);

			const CString strHour = strTime.Left(2);
			strTime.Delete(0, 3);

			const CString strMinite = strTime.Left(2);
			strTime.Delete(0, 3);

			const CString strSecond = strTime.Left(2);

			t.wYear		= _ttoi(strYear);
			t.wMonth	= _ttoi(strMonth);
			t.wDay		= _ttoi(strDay);
			t.wHour		= _ttoi(strHour);
			t.wMinute	= _ttoi(strMinite);
			t.wSecond	= _ttoi(strSecond);
		}
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::GetDayofWeek(int & nDayofWeek)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	CString strQuery;
	strQuery.Format(_T("SELECT DAYOFWEEK(NOW())"));
	int nRes = mysql_real_query(&m_Sql, W2A(strQuery), (unsigned long)strlen(W2A(strQuery)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 2;
	}

	my_ulonglong nRows = mysql_num_rows(pRes);

	if (nRows)
	{
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			const CString strDayofWeek = A2W(*SqlRow);

			nDayofWeek = _ttoi(strDayofWeek);
		}
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::Excute(const CString & strInfo)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	const int nRes = mysql_real_query(&m_Sql, W2A(strInfo), (unsigned long)strlen(W2A(strInfo)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


int CMySQL::Excute(const CString & strInfo, std::vector<CString> * pvResult)
{
	ASSERT(nullptr != pvResult);

	if (nullptr == pvResult)
	{
		return 3;
	}

	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	const int nRes = mysql_real_query(&m_Sql, W2A(strInfo), (unsigned long)strlen(W2A(strInfo)));

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());

		return nRes;
	}

	MYSQL_RES *pRes = mysql_store_result(&m_Sql);

	if (nullptr == pRes)
	{
		TRACE(_T("\n%s"), _GetErrInfo());
		return 2;
	}

	my_ulonglong nRows = mysql_num_rows(pRes);

	if (nRows)
	{
		const MYSQL_ROW SqlRow = mysql_fetch_row(pRes);

		if (nullptr != *SqlRow)
		{
			pvResult->push_back(A2W(*SqlRow));
		}
	}

	mysql_free_result(pRes);

	return nRes;
}


int CMySQL::Ping(void)
{
	USES_CONVERSION;

	CSyncCtrl s(&m_seSql);

	const int nRes = mysql_ping(&m_Sql);

	if (nRes)
	{
		TRACE(_T("\n%d,%s"), nRes, _GetErrInfo());
	}

	return nRes;
}


BOOL CMySQL::IsNetAlive(void)
{
	DWORD dwNetworkFlag = 0;
	const BOOL bNetAlive = IsNetworkAlive(&dwNetworkFlag);

	if (((dwNetworkFlag & NETWORK_ALIVE_LAN) == NETWORK_ALIVE_LAN) ||
		((dwNetworkFlag & NETWORK_ALIVE_WAN) == NETWORK_ALIVE_WAN) ||
		((dwNetworkFlag & NETWORK_ALIVE_AOL) == NETWORK_ALIVE_AOL))
	{
		return TRUE;
	}

	return FALSE;
}


CString CMySQL::_GetErrInfo(void)
{
	USES_CONVERSION;

	m_strErrInfo = A2W(mysql_error(&m_Sql));

// 	m_strErrInfo.Replace(m_strHost, _T("WoodyHost"));
// 	m_strErrInfo.Replace(_T("bp17627f6i33fnbhi"), _T("WoodyHost"));

	return m_strErrInfo;
}

#endif // DATABASE_ENABLE