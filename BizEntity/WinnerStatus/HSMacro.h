//this is HSmacro.h
//~
//这个存放一些在状态栏里面需要用到的宏定义
#pragma once


//宏定义
#define STATUE_BKCOLOR       RGB(50,50,51) //状态栏背景颜色
#define STATUE_HEIGHT        42            //状态栏高度

#define Status_Log_Name      _T("winnerstatus")

#define Transmited_Process	 24   //通讯状态大小
#define Red_Green_Panel   	 125  //红绿灯大小
#define SCROLL_FLOAT_WIDTH   500  //跑马灯浮动显示默认宽度
#define SCROLL_FLOAT_HEIGHT  16   //跑马灯浮动显示默认高度
#define SCROLL_BITMAP_WIDTH  20   //跑马灯滚动字体之间加入图片

#define INDEX_MAX_COUNT      4    //指数条中显示股票最多的数量
#define INDEX_MAX_COL        6    //指数条中显示股票翻滚的最大数量
#define INDEX_PANEL          330  //指数栏每个单元显示股票长度

#define LIST_MAX_COUNT       6    //指数条上一列最多显示6个
#define strStructLenth       32   //显示字符串最大的长度

#define Comm_OK             _T("连接成功")
#define Comm_Error          _T("连接失败")