#include "stdafx.h"
#include "OperateFile.h"


COperateFile::COperateFile(void)
{
}


COperateFile::~COperateFile(void)
{
}


// 删除文件（第二个参数bDelete表示是否删除至回收站,默认删除到回收站）  
BOOL COperateFile::_RecycleFileOrFolder(CString strPath, BOOL bDelete/*=FALSE*/)  
{  
	strPath += '\0';  
	SHFILEOPSTRUCT  shDelFile;  
	memset(&shDelFile,0,sizeof(SHFILEOPSTRUCT));  
	shDelFile.fFlags |= FOF_SILENT;             // 不显示进度  
	shDelFile.fFlags |= FOF_NOERRORUI;          // 不报告错误信息  
	shDelFile.fFlags |= FOF_NOCONFIRMATION;     // 直接删除，不进行确认  

	// 设置SHFILEOPSTRUCT的参数为删除做准备  
	shDelFile.wFunc = FO_DELETE;        // 执行的操作  
	shDelFile.pFrom = strPath;          // 操作的对象，也就是目录（注意：以“\0\0”结尾）  
	shDelFile.pTo = NULL;               // 必须设置为NULL  
	if (bDelete) //根据传递的bDelete参数确定是否删除到回收站  
	{      
		shDelFile.fFlags &= ~FOF_ALLOWUNDO;    //直接删除，不进入回收站  
	}   
	else   
	{             
		shDelFile.fFlags |= FOF_ALLOWUNDO;    //删除到回收站  
	}  

	BOOL bres = SHFileOperation(&shDelFile);    //删除  
	return !bres;  
}


BOOL COperateFile::_Write2TxtFile(CString strPath, CString strFileName, CString strInfo, BOOL bTime)
{
	m_seWrite.Lock();

	CString strTemp = strPath + _T("\\") + strFileName + _T(".txt");
	
	ofstream WriteFile(strTemp,ios::app);//app表示每次操作前均定位到文件末尾

	CString strInfoWrite;

	USES_CONVERSION;

	if (bTime)
	{
		TCHAR szFileName[MAX_PATH] = {0};

		SYSTEMTIME st;
		GetLocalTime(&st);

		CString strTime;
		strTime.Format(_T("%04d年%02d月%02d日  %02d时%02d分%02d秒     "), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		
		strInfoWrite.Append(strTime);
	}
	strInfoWrite.Append(strInfo);

	WriteFile<<W2A(strInfoWrite)<<endl;

	WriteFile.close();

	m_seWrite.Unlock();	
	
	return TRUE;
}