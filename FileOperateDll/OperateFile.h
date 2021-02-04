#pragma once

#include<iostream>
#include<fstream>
#include <afxmt.h>
#include <time.h>

using namespace std;
class AFX_EXT_CLASS COperateFile
{
public:
	COperateFile(void);
	~COperateFile(void);
public:
	BOOL _RecycleFileOrFolder(CString strPath, BOOL bDelete = TRUE);


/*================================================================
功能: 写入数据到txt文件
传入参数:	
	1. strPath: 文件夹路径
	2. strFileName: 文件名称
	3. strInfo: 写入的信息
	4. bTime: 是否写入时间
返回值: 无
================================================================*/
	BOOL _Write2TxtFile(CString strPath, CString strFileName, CString strInfo, BOOL bTime = TRUE);//寫入信息到txt文件

public:
	CSemaphore m_seWrite;//寫入鎖，使用多線程時，防止信息衝突	
};

