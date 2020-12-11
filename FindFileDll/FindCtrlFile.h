#pragma once

#include <vector>

extern CString _declspec(dllimport) g_strExePath;//exe文件路徑

using namespace std;

class AFX_EXT_CLASS CFindCtrlFile
{
public:
	CFindCtrlFile(void);
	~CFindCtrlFile(void);
public:
	vector <CString> m_vstrFileName;
public:
	BOOL SearchControllerFile();
	BOOL SearchReportFile();
	BOOL SearchTemplateFile();
	BOOL SearchImageFile();
	BOOL SearchFile(CString strPath, BOOL & bIfCreate);
	CString GetExePath();//获取exe路径
	CString LoadSignalData(CString strFileName);
	BOOL CheckFile(CString strFilePath);//查找是否存在文件

	vector<pair<CString,CString>> FindFilesInDirecotry(CString fileName, CString directory, int & filecounter);
	vector<pair<CString,CString>> FindFilesInDirecotryRecursion( CString fileName, CString directory, int recursionCount, int & filecounter, vector <CString> & vstrFileName);
	BOOL RecycleFileOrFolder(CString strPath, BOOL bDelete/*=FALSE*/);
	vector<CString> _FindFile(CString strFolderName, CString strFileType);//根據文件夾名稱和文件類型查找文件，返回文件路徑
};

