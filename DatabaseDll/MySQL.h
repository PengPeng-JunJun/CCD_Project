#pragma once

// #define DATABASE_ENABLE

#ifdef DATABASE_ENABLE

#include <vector>
#include <Sensapi.h>
#pragma comment(lib, "Sensapi.lib")

#include <D:\\mysql-5.7.25-win32\\include\\mysql.h>
#pragma comment(lib, "D:\\mysql-5.7.25-win32\\lib\\libmysql.lib")

#include "..\\MiscKitDll\\SyncCtrl.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\MiscKitDll.lib")
#else
#pragma comment(lib, "..\\Release\\MiscKitDll.lib")
#endif

class AFX_EXT_CLASS CMySQL
{
public:
	CMySQL(void);
	~CMySQL(void);

public:
	MYSQL m_Sql;
	CString m_strErrInfo;
public:
	BOOL Init(void);
	BOOL IsConnected(void);

	BOOL Connet(const CString & strHost, unsigned int nPort,
		const CString & strDataBase, const CString & strUser, const CString & strPsd);	// 连接数据库

	BOOL Disconnect(void);										// 断开连接

	int SetCharSet(const CString & strName);					// 设置字符集

	int CreateDatabase(const CString & strName);				// 创建数据库
	int SelectDatabase(const CString & strName);				// 选择数据库
	int IsDbExist(const CString & strName, BOOL & bExist);		// 判断数据库是否存在
	int DeleteDatabase(const CString & strName);				// 删除数据库	
	int GetDbName(CString * pstrName);							// 取得数据库名称

	int CreateTable(const CString & strTable, const CString & strColsInfo);		// 创建表
	int SelectTable(const CString & strTable);					// 选择表
	int IsTableExist(const CString & strName, BOOL & bExist);	// 判断当前数据库中表是否存在
	int IsTableExist(const CString & strDb, const CString & strTable, BOOL & bExist);	// 判断数据库中表是否存在
	int DeleteTable(const CString & strTable);					// 删除表

	int GetColsType(const CString & strTable, std::vector<CString> * pvstrType);
	int GetColType(const CString & strTable, const CString & strCol, CString * pstType);

	int Insert(const CString & strInfo);
	int Insert(const CString & strTable, CString strInfo);
	int Insert(const CString & strTable, const CString & strCols, CString strValue);

	int Delete(const CString & strTable, const CString & strConition);
	int ClearTable(const CString & strTable);

	int Update(const CString & strTable, CString strInfo);
	int SetItemText(const CString & strTable, const CString & strCol, const CString & strCondition, const CString & strNew);
	int GetItemText(const CString & strTable, const CString & strCol, const CString & strCondition, std::vector<CString> * pvText);

	int GetRowText(const CString & strTable, const CString & strCondition, std::vector<std::vector<CString>> * pvvText);

	int GetRows(const CString & strTable, my_ulonglong & nRows);
	int GetCols(const CString & strTable, my_ulonglong & nCols);
	int GetColSum(const CString & strTable, const CString & strCol, const CString & strCondition, double & dSum);

	int GetDistRows(const CString & strTable, const CString & strCondition, my_ulonglong & nRows);

	int GetCurTime(SYSTEMTIME & t);
	int GetDayofWeek(int & nDayofWeek);

	int Excute(const CString & strInfo);
	int Excute(const CString & strInfo, std::vector<CString> * pvResult);
	int Ping(void);

	BOOL IsNetAlive(void);
protected:
	BOOL m_bConnected;

	CString m_strHost;
	CString m_strDbName;
	CString m_strTbName;

	CSemaphore m_seSql;

	CString _GetErrInfo(void);
};

#endif // DATABASE_ENABLE
