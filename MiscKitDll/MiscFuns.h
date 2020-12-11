#pragma once

#include "NewPtr.h"
#include "ValRst.h"
#include "SyncCtrl.h"
#include "GdiplusText.h"

#include <math.h>			// 数学公式
#include <regex>			// 用于正则表达式
#include <algorithm>		// 算法头文件

#include <wininet.h>		// 网络协议
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "wininet.lib")

#ifndef IS_ODD
#define IS_ODD(v)			((v) & 0x01)
#endif // !IS_ODD

#ifndef IS_EVENT
#define IS_EVENT(v)			(!IS_ODD(v))
#endif // !IS_EVENT

// 180 / PI;
#ifndef _180_PI
#define _180_PI				57.295779513082320876798154814105
#endif // !_180_PI

// PI / 180;
#ifndef _PI_180
#define _PI_180				0.0174532925199432957692369076848
#endif // !_PI_180

#ifndef VEC2_MODULUS
#define VEC2_MODULUS(vec2)	sqrt(vec2.x * vec2.x + vec2.y * vec2.y)
#endif // !VEC2_MODULUS


#define MISC_FUN			AFX_API_EXPORT

enum 
{
	FIND_NN_DEC = 0,
	FIND_NN_FLOAT,
	FIND_NN_HEX_0X,
	FIND_NN_HEX
};

enum
{
	EXP_CONSTANT = 1,
	EXP_VARIABLE = 2,
	EXP_NEGTIVE	 = 4,
	EXP_POSITIVE = 8,
	EXP_OPERATOR = 16,
	EXP_ROUND_LEFT = 32,
	EXP_ROUND_RIGHT = 64,
	EXP_SQUARE_LEFT = 128,
	EXP_SQUARE_RIGHT = 256
};


typedef struct _tagFolderInfo
{
	CString strPath;

	BOOL bFolder;
	BOOL bSysFile;
	BOOL bHidden;
	BOOL bReadOnly;
	BOOL bTemporary;
}FOLDER_INFO;

typedef struct _tagExpCalcItem
{
	int nType;		// 类型
	int nOps;		// 操作数个数
	int nLevel;		// 优先级
	CString strVal; // 值
}EXP_CALC_ITEM;

const int g_nOps = 26;
const CString g_strOp[g_nOps] = {_T("="),  _T("||"), _T("$$"), _T("|"),  _T("^"), _T("$"),
								 _T("=="), _T("!="), _T("<"),  _T("<="), _T(">"), _T(">="),
								 _T("<<"), _T(">>"), _T("+"),  _T("-"),  _T("*"), _T("/"),
								 _T("%"),  _T("!"),  _T("~"),  _T("("),  _T(")"), _T("["), _T("]"),
								 _T(".")};

MISC_FUN DWORD GetLastErrInfo(CString * pErr);		// 取得错误信息

MISC_FUN BOOL UserDetect(const CString & strUser);	// 检测电脑使用者名称

MISC_FUN BOOL IsCtrlDown(void);						// Ctrl键是否被按下
MISC_FUN BOOL IsShfitDown(void);					// Shift键是否被按下

MISC_FUN BOOL IsAlpha(TCHAR t);						// 判断是不是字母
MISC_FUN BOOL IsDigit(TCHAR t);						// 判断是不是数字
MISC_FUN BOOL IsAlphaDigit(TCHAR t);				// 判断是不是字母或数字
MISC_FUN void TrimAndUpper(CString & strSrc);		// 去掉首尾空格并转换成大写

MISC_FUN std::vector<CString> GetSegInfo(const CString & strInfo, TCHAR nMark);			// 取得分割字符串
MISC_FUN std::vector<CString> GetSegInfoEx(const CString & strInfo, TCHAR nMarks, ...);	// 取得分割字符串

MISC_FUN BOOL IsFileExist(const CString & strFile);	// 判断文件是否存在	
MISC_FUN BOOL CreateFileDir(CString strPath);		// 创建文件夹
MISC_FUN BOOL ClearFolder(CString strPath);			// 清空文件夹
MISC_FUN BOOL DeleteFolder(CString strPath);		// 删除文件夹

MISC_FUN void TraversalFile(const CString & strPath, std::vector<FOLDER_INFO> & vFiles, int nOrder = 0);		// 遍历文件夹中的文件

MISC_FUN BOOL FtpTraversalFile(const CString & strUrl, int nPort,
							   CString strPath, std::vector<FOLDER_INFO> & vFiles,
							   int nOrder = 0, CString * pErr = nullptr);	// 遍历服务器文件夹中的文件

MISC_FUN void FolderForword(std::vector<FOLDER_INFO> & vFiles);				// 遍历后文件夹排在前

MISC_FUN BOOL DownloadFile(const CString & strUrl, const CString & strSavePath, CString * pErr);	// 从服务器下载文件到本地
MISC_FUN BOOL DownloadFile(const CString & strUrl, CNewPtr<char> & Mem, CString * pErr);			// 从服务器下载文件到内存

MISC_FUN CString GetFileName(const FOLDER_INFO & Info);						// 取出文件名
MISC_FUN void TraceFiles(const std::vector<FOLDER_INFO> & vFiles);			// 输出遍历文件结果

MISC_FUN int Rand(int nMin, int nMax);				// 产生一个随机整数

MISC_FUN void DelayUs(long nTime);					// 延时单位为uS的函数
MISC_FUN void DelayMs(long nTime);					// 延时单位为mS的函数

MISC_FUN std::vector<int> GetIndex(CString strText, TCHAR nMark);			// 取得整数序号
MISC_FUN CString GetKilobitStr(int nData);									// 取得用千分位分隔的字符串

MISC_FUN int FindNonNumber(const CString & strData, int nMode);				// 找出非数字字符
MISC_FUN void DeleteSpace(CString & strData);		// 删除字符串中的空格
MISC_FUN void DeleteZeros(CString & strData);		// 去数字头或者掉小数点后多余的0

MISC_FUN int HexStr2int(CString strHex);			// 将十六进制字符串转换成整数
MISC_FUN int GetDataMode(const CString & strData);	// 返回值: -1. 错误的数据 0. 十进制数据 1. 带0x开始的十六进制 2. 不带0x开始的十六进制

MISC_FUN CString GetComputerName(CString * pErr);	// 取得计算机名称
MISC_FUN CString GetCurUserName(CString * pErr);	// 取得当前计算机用户名称

MISC_FUN CString GetCurTime(void);					// 取得当前系统时间
MISC_FUN CString GetAppExePath(void);				// 取得应用程序路径
MISC_FUN CString GetAppIniPath(void);				// 取得应用程序ini文件路径(应用程序路径中)
MISC_FUN CString GetSysFontPath(void);				// 取得系统字体路径
MISC_FUN int GetDiskFreeSpace(const CString & strPath);		// 获取磁盘可用空间单位:MB

MISC_FUN int GetOperatorPriority(const CString & op);		// 判断运算符优先级
MISC_FUN std::vector<EXP_CALC_ITEM> InfixToSuffix(CString strExp, int (*priority)(const CString & o),
												  BOOL (*var)(const CString & v, CString * pInfo), CString * pErr);	// 转换成逆波兰式
MISC_FUN double CalcRepolish(std::vector<EXP_CALC_ITEM> vItems,
							 double (*var)(const CString & v)) throw(std::out_of_range);			// 计算逆波兰试值

MISC_FUN BOOL CalcExpression(const CString & strExp,
							 int (*priority)(const CString & o),
							 BOOL (*varfind)(const CString & v, CString * pInfo),
							 double (*varget)(const CString & v), double & dVal, CString * pErr);	// 计算表达式的值

MISC_FUN CArchive & operator << (CArchive & ar, const std::vector<std::vector<CString>> & vv);	// 存储表格字符串
MISC_FUN CArchive & operator >> (CArchive & ar, std::vector<std::vector<CString>> & vv);		// 加载表格字符串

template<typename T>
MISC_FUN int FindElement(std::vector<T> & vSrc, const T & val)						// 在向量中查找指定元素是否在
{
	const vector<T>::iterator it = std::find(vSrc.begin(), vSrc.end(), val);

	if (it == vSrc.end())
	{
		return -1;
	}

	return std::distance(vSrc.begin(), it);
}

template<typename T>
MISC_FUN int EraseElement(std::vector<T> & vSrc, const std::vector<int> & vIndex)	// 删除向量中多个指定元素
{
	const int nSize = vSrc.size();

	std::vector<int> vTmp = vIndex;

	if (vTmp.size() < 1 || nSize < 1)
	{
		return nSize;
	}

	sort(vTmp.begin(), vTmp.end());

	// unique函数只是把重复的元素放到的向量的末尾
	vTmp.erase(unique(vTmp.begin(), vTmp.end()), vTmp.end());

	const int nErase = vTmp.size();

	for (int i = nErase - 1; i >= 0; i--)
	{
		if (vTmp[i] < nSize)
		{
			vSrc.erase(vSrc.begin() + vTmp[i]);
		}
	}

	return (int)vSrc.size();
}

template<typename T1, typename T2>
MISC_FUN double Get2PtsAngle(T1 pt1, T2 pt2, BOOL bInverseY)			// 计算两个点之与水平线间的角度, 使用笛卡尔坐标系, Y向上为正
{
	const double dOfst_X = pt2.x - pt1.x;
	const double dOfst_Y = bInverseY? (-pt2.y + pt1.y): (pt2.y - pt1.y);

	const double r = max(sqrt(dOfst_X * dOfst_X + dOfst_Y * dOfst_Y), 0.0001);

	double dAngle = acos(fabs(dOfst_X) / r) * _180_PI;

	if (dOfst_X >= 0)
	{
		if (dOfst_Y < 0)
		{
			dAngle = 360.0 - dAngle;
		}
	}
	else if (dOfst_Y >= 0)
	{
		dAngle = 180.0 - dAngle;
	}
	else
	{
		dAngle = 180.0 + dAngle;
	}
	
	return dAngle;
}


template <typename T1, typename T2>
MISC_FUN T1 PtRotate(const T1 & ptSrc, const T2 & ptCenter, double dAngle, BOOL bInverseY)
{
	const double dRad = dAngle * _PI_180;
	const double dSin = sin(dRad);
	const double dCos = cos(dRad);

	const Point2d ptOfst(ptSrc.x - ptCenter.x, (bInverseY? (-ptSrc.y + ptCenter.y): (ptSrc.y - ptCenter.y)));
	
	Point2d ptDelta(ptOfst.x * dCos - ptOfst.y * dSin, ptOfst.x * dSin + ptOfst.y * dCos);

	if (bInverseY)
	{
		ptDelta.y = -ptDelta.y;
	}

	if (typeid(ptSrc.x) == typeid(double))
	{
		return T1(ptCenter.x + ptDelta.x, ptCenter.y + ptDelta.y);
	}
	else if (typeid(ptSrc.x) == typeid(float))
	{
		return T1((float)(ptCenter.x + ptDelta.x), (float)(ptCenter.y + ptDelta.y));
	}

	return T1(cvRound(ptCenter.x + ptDelta.x), cvRound(ptCenter.y + ptDelta.y));
}


template <typename T1, typename T2>
MISC_FUN double _PointDistance(const T1 & pt1, const T2 & pt2)
{
	const double dx = pt2.x - pt1.x;
	const double dy = pt2.y - pt1.y;
	
	return sqrt(dx * dx + dy * dy);
}
