// CameraCalibration.cpp : 实现文件
//

#include "stdafx.h"
#include "CameraCalibration.h"
#include "afxdialogex.h"


// CCameraCalibration 对话框

IMPLEMENT_DYNAMIC(CCameraCalibration, CDialogEx)

CCameraCalibration::CCameraCalibration(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraCalibration::IDD, pParent)
{

}

CCameraCalibration::~CCameraCalibration()
{
}

void CCameraCalibration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraCalibration, CDialogEx)
END_MESSAGE_MAP()


// CCameraCalibration 消息处理程序


BOOL CCameraCalibration::_CameraCalibration()
{
	ifstream fin("calibdata.txt");//标定所用图像文件的路径
    ofstream fout("calibration_result.txt");//保存标定结果

    //读取每一幅图像，从中提取出内角点，然后对角点进行亚像素精确化
    cout<<"开始提取特征点..........";
    int image_count = 0;//图像的数量
	cv::Size image_size;//图像的尺寸
    cv::Size board_size = cv::Size(13,9);//标定板上每行、列的内角点数，一般地，行列数不要相同
    vector<Point2f> image_points_buf;//用于存储检测到的内角点图像坐标位置
    vector<vector<Point2f>> image_points_seq;//保存检测到的所有角点
    string filename;//图像名
    int count = 0;//保存角点数
    while(getline(fin,filename))//从文本文档中依次读取待标定图片名
    {
        image_count++;
        cout<<"image_count = "<<image_count<<endl;

        Mat imageInput = imread(filename);//依次读取当前目录下图片

        if(image_count == 1)//读入第一张图片时获取图像宽高信息
        {
            image_size.width = imageInput.cols;
            image_size.height = imageInput.rows;

            cout<<"image_size.width = "<<image_size.width<<endl;
            cout<<"image_size.height = "<<image_size.height<<endl;
        }

        //提取标定板的内角点，输入必须是8位的灰度或者彩色图像
        if(0 == findChessboardCorners(imageInput,board_size,image_points_buf))
        {
            cout<<"Cannot find chessboard corners!\n";//沒有找到角點
            exit(1);
        }
        else
        {
            Mat view_gray;
            cvtColor(imageInput,view_gray,CV_RGB2GRAY);//转灰度图
            //find4QuadCornerSubpix(view_gray,image_points_buf,Size(5,5));
            //亚像素精确化方法一
            //image_points_buf初始化的角点坐标向量，同时作为亚像素坐标位置的输出，浮点型数

            //cornerSubPix(view_gray, image_points_buf, Size(5,5), Size(-1,-1));
			count += image_points_buf.size();
           //亚像素精确化方法二
             //Size(5,5)是搜索窗口的大小,Size(-1,-1)表示没有死区
            //第四个参数定义求角点的迭代过程的终止条件，可以为迭代次数和角点精度两者的组合

            image_points_seq.push_back(image_points_buf);//保存亚像素角点

            drawChessboardCorners(view_gray,board_size,image_points_buf,false);
            //用于绘制被成功标定的角点，输入8位灰度或者彩色图像
            //第四个参数是标志位，用来指示定义的棋盘内角点是否被完整的探测到
            //false表示有未被探测到的内角点，这时候函数会以圆圈标记出检测到的内角点
            imshow("Camera Calibration",view_gray);//显示图片
            waitKey(500);//暂停0.5S

        }
        cout << "count = " << count<< endl;//显示角点累加后的值
    }

    int total = image_points_seq.size();//图片总数
    cout << "total = " << total << endl;
    int CornerNum = board_size.width*board_size.height;//每张图片上总的内角点数

    for(int ii = 0;ii<total;ii++)
    {
        cout << "第" << ii + 1 << "张图片的数据：" << endl;
        for(int jj = 0;jj<CornerNum;jj++)
        {
            if (0 == jj % 3)
                cout << endl;//每三个角点坐标之后换行
            else
                cout.width(10);//输出格式控制

            cout << "（" << image_points_seq[ii][jj].x << "，" << image_points_seq[ii][jj].y << ")";
        }
        cout << endl;
    }

    cout<<"角点提取完成！\n";

    cout<<"开始标定............";

    cv::Size square_size = cv::Size(10,10);//设置棋盘格子的实际边长，单位为mm
    vector<vector<Point3f>> object_points;//保存标定板上角点的三维坐标
    Mat cameraMatrix = Mat(3,3,CV_32FC1,Scalar::all(0));//相机内参数矩阵
    vector<int> point_counts;//每幅图像中角点的数量
    Mat distCoeffs = Mat(1,5,CV_32FC1,Scalar::all(0));//摄像机的5个畸变系数：k1,k2,k3,p1,p2
    vector<Mat> tvecsMat;//每幅图像的平移向量
    vector<Mat> rvecsMat;//每幅图像的旋转向量

    //初始化标定板上角点的三维坐标
    int i,j,t;
    for(t = 0;t<image_count;t++)
    {
        vector<Point3f> temPointSet;
        for(i = 0;i<board_size.height;i++)
        {
            for(j = 0;j<board_size.width;j++)
            {
                Point3f realPoint;

                //假设标定板放在世界坐标系中的z = 0平面上
                //需要依据棋盘上单个黑白矩阵的大小，计算出（初始化）每一个内角点的世界坐标
                realPoint.x = i*square_size.width;
                realPoint.y = j*square_size.height;
                realPoint.z = 0;
                temPointSet.push_back(realPoint);

            }
        }
        object_points.push_back(temPointSet);
    }

    //初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板
    for(i = 0;i<image_count;i++)
    {
        point_counts.push_back(board_size.width*board_size.height);
    }

    calibrateCamera(object_points,image_points_seq,image_size,
        cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0);
/*  object_points 世界坐标系中角点的三维坐标，image_points_seq 每个内角点对应的图像坐标点
    image_size 图像的像素尺寸大小，cameraMatrix 输出，内参数矩阵，distCoeffs 输出，畸变系数
    rvecsMat 输出，旋转向量，tvecsMat 输出，位移向量，0标定时采用的算法
    在使用该函数进行标定运算之前，需要对棋盘上每一个内角点的空间坐标系的位置坐标进行初始化，
    标定的结果是生成相机的内参数矩阵cameraMatrix、相机的5个畸变系数distCoeffs，另外每张图像
    都会生成属于自己的平移向量和旋转向量
*/
    cout<<"标定完成！"<<endl;

    cout<<"开始输出标定结果....."<<endl;
    double total_err = 0.0; //所有图像的平均误差的总和，初始化为0.0
    double err = 0.0; //每幅图像的平均误差
    vector<Point2f> image_points2; //保存重新计算得到的投影点

    cout<<"每幅图像的标定误差：\n";
    fout<<"每幅图像的标定误差：\n";

    for(i = 0;i<image_count;i++)
    {
        vector<Point3f> tempPointSet = object_points[i];//取出每幅图像角点的三维空间坐标
        projectPoints(tempPointSet,rvecsMat[i],tvecsMat[i],
            cameraMatrix,distCoeffs,image_points2);
        //通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点

        vector<Point2f> tempImagePoint = image_points_seq[i];//原来每幅图像中角点图像坐标
        Mat tempImagePointMat = Mat(1,tempImagePoint.size(),CV_32FC2);
        //用于将原图像坐标点存储成一行多列的Mat，由两个通道浮点型数据构成
        Mat image_points2Mat = Mat(1,image_points2.size(),CV_32FC2);
        //用于将重投影坐标点存储成一行多列的Mat，以便于计算重投影误差

        for(int j = 0;j <tempImagePoint.size(); j++)
        {
            image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x,image_points2[j].y);
            tempImagePointMat.at<Vec2f>(0,j) = Vec2f(tempImagePoint[j].x,tempImagePoint[j].y);
        }//赋值
        //Vec2f表示的是2通道float类型的Vector,mat.at<Vec2f>(y, x)是访问图像的一种方式

        err = norm(image_points2Mat,tempImagePointMat,NORM_L2);
        //计算每张图片重投影坐标和亚像素角点坐标之间的偏差
        total_err += err /= point_counts[i];//累加误差
        cout<<"第"<<i+1<<"幅图像的平均误差："<<err<<"像素"<<endl;
        fout<<"第"<<i+1<<"幅图像的平均误差："<<err<<"像素"<<endl;

    }

    cout<<"总体平均误差："<<total_err/image_count<<"像素"<<endl;
    fout<<"总体平均误差："<<total_err/image_count<<"像素"<<endl<<endl;
    cout<<"评价完成！"<<endl;

    cout<<"开始保存标定结果............"<<endl;
    Mat rotation_matrix = Mat(3,3,CV_32FC1,Scalar::all(0));//保存每幅图像的旋转矩阵
    fout<<"相机内参数矩阵："<<endl;
    fout<<cameraMatrix<<endl<<endl;
    fout<<"畸变系数：\n";
    fout<<distCoeffs<<endl<<endl<<endl;

    for(i = 0; i<image_count;i++)
    {
        fout<<"第"<<i+1<<"幅图像的旋转向量："<< endl;
        fout<<rvecsMat[i]<<endl;
        Rodrigues(rvecsMat[i],rotation_matrix);//将旋转向量转换为相应的旋转矩阵
        fout<<"第"<<i+1<<"幅图像的旋转矩阵："<< endl;
        fout<<rotation_matrix<< endl << endl;
        fout<<"第"<<i+1<<"幅图像的平移向量："<<  endl;
        fout<<tvecsMat[i]<< endl <<endl;

    }
    cout<<"完成保存"<< endl;
    fout<< endl;


    //显示标定结果
    Mat mapx = Mat(image_size,CV_32FC1);//输出的X坐标重映射参数
    Mat mapy = Mat(image_size,CV_32FC1);//输出的Y坐标重映射参数
    Mat R = Mat::eye(3,3,CV_32F);
    cout<<"保存矫正图像"<<endl;

    string imageFileName;
    std::stringstream StrStm;
    for(int i = 0;i < image_count;i++)
    {
        cout<<"Frame # "<<i+1<<"....."<<endl;
        initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix,
            image_size, CV_32FC1, mapx, mapy);//用来计算畸变映射

        StrStm.clear();//清除缓存
        imageFileName.clear();
        string filePath = "chess";
        StrStm<<i+1;
        StrStm>>imageFileName;
        filePath += imageFileName;
        filePath += ".bmp";
        //获取图片路径
        Mat imageSource = imread(filePath);//读取图像
        Mat newimage = imageSource.clone();//拷贝图像

       remap(imageSource,newimage,mapx,mapy,INTER_LINEAR);//把求得的映射应用到图像上
       //与initUndistortRectifyMap结合使用，为矫正方法之一

       //undistort(imageSource,newimage,cameraMatrix,distCoeffs);//矫正方法二
       //第五个参数newCameraMatrix=noArray()，默认跟cameraMatrix保持一致,故可省

        imageFileName += "_d.jpg";//矫正后图片命名
        imwrite(imageFileName,newimage);//保存矫正后的图片

    }
    cout<<"保存结束"<<endl;

    fin.close();
    fout.close();
    getchar();//等待输入以退出

    return 0;
}


	