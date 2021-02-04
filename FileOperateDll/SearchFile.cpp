#include "stdafx.h"
#include "SearchFile.h"

CString g_strExePath = _T("");

CSearchFile::CSearchFile(void)
{
}


CSearchFile::~CSearchFile(void)
{
}


BOOL CSearchFile::SearchControllerFile()
{
	const CString strFolderPath = g_strExePath + _T("\\Controller");

	if(!PathIsDirectory(strFolderPath))   
		return CreateDirectory(strFolderPath,NULL);

	return TRUE;
}


BOOL CSearchFile::SearchReportFile()
{
	CString strFolderPath = g_strExePath + _T("\\Report");

	//判断路径是否存在   
	if(!PathIsDirectory(strFolderPath))   
	{
		if(!CreateDirectory(strFolderPath,NULL))   
		{   
			return FALSE;
		}
	}

	strFolderPath.Empty();
	strFolderPath.Append(g_strExePath);
	strFolderPath.Append(_T("\\Data"));

	if(!PathIsDirectory(strFolderPath))   
	{
		if(!CreateDirectory(strFolderPath,NULL))   
		{   
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CSearchFile::SearchTemplateFile()
{
	const CString strFolderPath = g_strExePath + _T("\\Template");

	//判断路径是否存在   
	if(!PathIsDirectory(strFolderPath))   
		return CreateDirectory(strFolderPath,NULL);

	return TRUE;

}

BOOL CSearchFile::SearchImageFile()
{
	CString strFolderPath;
	strFolderPath = g_strExePath + _T("\\NG_Img");

	//判断路径是否存在   
	if(!PathIsDirectory(strFolderPath))   
	{
		if(!CreateDirectory(strFolderPath,NULL))   
		{   
			return FALSE;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		strFolderPath.Format(_T("\\測試組_%d"), i + 1);
		strFolderPath = g_strExePath + _T("\\NG_Img") + strFolderPath;
		//判断路径是否存在   
		if(!PathIsDirectory(strFolderPath))   
		{
			CString strMsg;
			if(!CreateDirectory(strFolderPath,NULL))   
			{   
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CSearchFile::SearchLogFile()
{
	const CString strFolderPath = g_strExePath + _T("\\Log");

	//判断路径是否存在   

	if(!PathIsDirectory(strFolderPath))   
		return CreateDirectory(strFolderPath,NULL);

	return TRUE;
}


BOOL CSearchFile::SearchFile(CString strPath, BOOL & bIfCreate)
{
	CString strFolderPath;
	strFolderPath = g_strExePath + strPath;

	//判断路径是否存在   
	if(!PathIsDirectory(strFolderPath))   
	{
		CString strMsg;
		if(!CreateDirectory(strFolderPath,NULL))   
		{   
			return FALSE;
		}
		else
		{
			bIfCreate = TRUE;
			return TRUE;
		}
	}
	return TRUE;
}

CString CSearchFile:: GetExePath(void)  
{  
	CString strExePath;  
	GetModuleFileName(NULL, strExePath.GetBuffer(MAX_PATH), MAX_PATH);  
	strExePath.ReleaseBuffer();  
	strExePath = strExePath.Left(strExePath.ReverseFind(_T('\\')));  
	g_strExePath = strExePath;
	return strExePath;  
}

CString CSearchFile::LoadSignalData(CString strFileName)
{
	CString strFilePath;
	strFilePath = g_strExePath + _T("\\Controller\\") + strFileName + _T(".ini");

	CString strCtrlName;
	CString strCtrlCheckModeName;
	CString strCoilStart;
	CString strCoilEnd;
	CString strRegistersStart;
	CString strRegistersEnd;

	if (GetFileAttributes(strFilePath) != -1) //如果文件存在
	{
		GetPrivateProfileString(strFileName,_T("strCtrlName"),_T(""),strCtrlName.GetBuffer(MAX_PATH + 1),MAX_PATH,strFilePath);
		GetPrivateProfileString(strFileName,_T("strCtrlCheckModeName"),_T(""),strCtrlCheckModeName.GetBuffer(MAX_PATH + 1),MAX_PATH,strFilePath);
		GetPrivateProfileString(strFileName,_T("strCoilStart"),_T(""),strCoilStart.GetBuffer(MAX_PATH + 1),MAX_PATH,strFilePath);
		GetPrivateProfileString(strFileName,_T("strCoilEnd"),_T(""),strCoilEnd.GetBuffer(MAX_PATH + 1),MAX_PATH,strFilePath);
		GetPrivateProfileString(strFileName,_T("strRegistersStart"),_T(""),strRegistersStart.GetBuffer(MAX_PATH + 1),MAX_PATH,strFilePath);
		GetPrivateProfileString(strFileName,_T("strRegistersEnd"),_T(""),strRegistersEnd.GetBuffer(MAX_PATH + 1),MAX_PATH,strFilePath);
	}
	return strCtrlCheckModeName;
	
}

BOOL CSearchFile::CheckFile(CString strFilePath)//查找是否存在文件
{
	if (GetFileAttributes(strFilePath) != -1) //如果文件存在
	{
		return TRUE;
	}
	return FALSE;
}

/************************************************************************
说明：
    在文件夹中查找文件（仅在指定文件夹查找，不递归）
参数：
    fileName:要查找的文件名。支持通配符*
    directory:要查找的文件所在的目录路径
返回值：
    pair<string,string>类型的数组，map<文件名，文件路径>
************************************************************************/

vector<pair<CString,CString>> CSearchFile::FindFilesInDirecotry(CString fileName,CString directory, int & filecounter)
{
	vector<pair<CString,CString>> files;

	if(directory.Right(1) != "\\") //保证目录是以\结尾的
	{  
		directory += "\\";
	}
	directory += fileName;
	CFileFind finder;
	BOOL success = finder.FindFile(directory);
	while(success)
	{
		success = finder.FindNextFile();
		CString name = finder.GetFileName();
		CString path = finder.GetFilePath();
		pair<CString,CString> fileMap(name,path);
		files.push_back(fileMap);
		filecounter++;
	}
	finder.Close();
	filecounter -= 2;
	return files;
}


/************************************************************************
说明：
    在文件夹中递归查找文件
参数：
    fileName:要查找的文件名。支持通配符*
    directory:要查找的文件所在的目录路径
    recursionCount:递归查找的深度，默认为-1（不限制深度）,0表示只搜索给定的directory目录
返回值：
    pair<string,string>类型的数组，map<文件名，文件路径>
************************************************************************/

vector<pair<CString,CString>> CSearchFile::FindFilesInDirecotryRecursion( CString fileName, CString directory, int recursionCount, int & filecounter, vector <CString> & vstrFileName)
{
	vector<pair<CString,CString>> files;
	
	int curRecursionCount = recursionCount;
	int nPart = 0;
	//先对参数传进来的文件夹查找
	vector<pair<CString,CString>> localFiles =  FindFilesInDirecotry(fileName, directory, nPart);
	files.insert(files.end(),localFiles.begin(),localFiles.end());

	//然后对此文件夹下面的子文件夹递归查找
	CFileFind finder;
	if(directory.Right(1) != "\\") //保证目录是以\结尾的
	{  
		directory += "\\";
	}
	directory += "*.*";

	BOOL res = finder.FindFile(directory);
	while (res)
	{
		res = finder.FindNextFile();
		CString path = finder.GetFilePath();
		CString name = finder.GetFileName();
		curRecursionCount = recursionCount;  //查找同级目录的时候将当前深度设置为给定值
		if(finder.IsDirectory() && !finder.IsDots())
		{
			if(curRecursionCount >= 1 || curRecursionCount == -1 )
			{
				if(curRecursionCount >= 1)
				{
					curRecursionCount--;
				}
				localFiles = FindFilesInDirecotryRecursion(fileName, path, curRecursionCount, nPart, vstrFileName); //如果目标是个文件夹，则利用嵌套来遍历
				files.insert(files.end(),localFiles.begin(),localFiles.end());
				filecounter++;
			}
		}
	}
	finder.Close();

	vstrFileName.clear();
	m_vstrFileName.push_back(directory);
	vstrFileName = m_vstrFileName;
	return files;
}


vector<CString> CSearchFile::_FindFile(CString strFolderName, CString strFileType)
{
	vector<CString> vstrPath;
	CString m_cstrFileList=_T("");
	CFileFind tempFind;
	BOOL bFound; //判断是否成功找到文件
	const CString strPath = strFolderName + _T("\\*.") + strFileType + _T("*");
	bFound = tempFind.FindFile(strPath);  //修改" "内内容给限定查找文件类型
	CString strTmp;   //如果找到的是文件夹 存放文件夹路径
	while(bFound)      //遍历所有文件
	{
		bFound=tempFind.FindNextFile(); //第一次执行FindNextFile是选择到第一个文件，以后执行为选择
		//到下一个文件
		//if(!tempFind.IsDots()) continue; //如果找到的是返回上层的目录 则结束本次查找
		strTmp = tempFind.GetFileName(); //保存文件名，包括后缀名
			// 在此处添加对找到文件的处理
		vstrPath.push_back(strFolderName +  + _T("\\") + strTmp);
	}
	tempFind.Close();
	return vstrPath;
}