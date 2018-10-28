//Description: 设计一个键盘程序，当按Ctrl键时，表明要画椭圆；当按Shift键时，表明要画矩形。
//			   然后单击向右箭头键，椭圆或矩形的长度加10；单击向下箭头时，椭圆或矩形的高度加10；
//			   单击Home键时，整个圆形或矩形向左移动；单击End键时，整个圆形或矩形向右移动；
//			   单击PageUp键时，整个圆形或矩形向上移动；单击PageDown键时，整个圆形或矩形向下移动。
//Author: Luo Wenyang
//Date: 2018/10/28
//Environment: 请确保在Visual Studio 2017 IDE中编译，如有其它问题请参看README-运行说明

#include <windows.h>

//完成初始化过程：定义并注册窗口类，创建并显示窗口
//输入程序示例句柄和窗口显示方式
//返回一个布尔值，为TRUE说明初始化成功，为FALSE说明初始化失败
BOOLEAN InitWindow(HINSTANCE hInstance, int nCmdShow);

//窗口消息处理函数，自定义对WM_CREATE与WM_PAINT的处理过程
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//窗口程序的入口函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;	//定义消息变量

	//初始化窗口，如果失败就弹出一个提示框并错误退出
	if (!InitWindow(hInstance, nCmdShow))
	{
		MessageBox(NULL, "创建窗口失败!", "创建窗口", NULL);
		return 1;
	}

	//消息循环
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOLEAN InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;														//定义窗口句柄
	TCHAR szWindowClass[] = "MOVEIT";								//定义窗口类名称
	TCHAR szTitle[] = "用键盘控制图形显示";							//定义窗口标题
	WNDCLASS WndClass;												//定义窗口类
	WndClass.cbClsExtra = 0;										//无窗口类扩展
	WndClass.cbWndExtra = 0;										//无窗口实例扩展
	WndClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));	//背景为白色
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//窗口采用箭头光标
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//采用缺省图标
	WndClass.hInstance = hInstance;									//当前应用程序句柄
	WndClass.lpfnWndProc = WndProc;									//窗口处理函数
	WndClass.lpszClassName = szWindowClass;							//窗口类名称
	WndClass.lpszMenuName = NULL;									//无窗口菜单
	WndClass.style = 0;												//窗口类型为缺省类型

	if (!RegisterClass(&WndClass))	//注册窗口
		return FALSE;
	hWnd = CreateWindow(
		szWindowClass,				//窗口类名
		szTitle,					//标题名
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,				//窗口左上角坐标
		0,
		CW_USEDEFAULT,				//采用缺省的宽度和高度
		0,
		NULL,						//无父窗口
		NULL,						//无主菜单
		hInstance,					//当前实例句柄
		NULL						//不需要传递参数
	);
	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, nCmdShow);		//显示窗口
	UpdateWindow(hWnd);				//更新并绘制用户区
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;						//定义设备环境句柄
	PAINTSTRUCT PtStr;				//定义包含绘图信息的结构体变量
	HPEN hPen;						//定义画笔结构体变量
	HBRUSH hBrush;					//定义画刷结构体变量
	static RECT rect;				//定义用于保存图形坐标的结构体变量
	static BOOLEAN bEllipse = TRUE;	//定义静态布尔型变量，为真表示画椭圆，为假表示不画椭圆，初始为真
	static BOOLEAN bRect = FALSE;	//定义静态布尔型变量，为真表示画矩形，为假表示不画矩形，初始为假
	RECT ClientRect;				//定义保存客户区大小的结构体变量
	int nStep = 10;					//定义图形移动的步长为10
	BOOLEAN bFresh;					//定义布尔型变量，为真刷新用户区，为假不刷新用户区

	//待显示文字，用于进行操作提示
	LPCSTR lpsz = {
		"Ctrl画椭圆，Shift画矩形，→长度加10，←长度减10，↓高度加10，↑高度减10，Home向左移动，End向右移动，Page Up向上移动，Page Down向下移动",
	};

	switch (message)
	{
	case WM_CREATE:	//处理窗口创建消息
		//计算客户区中心，将中心位置作为图形中心，以10X10大小计算图形起始坐标
		GetClientRect(hWnd, &ClientRect);
		rect.left = (ClientRect.left + ClientRect.right) / 2 - 20;
		rect.right = (ClientRect.left + ClientRect.right) / 2 + 20;
		rect.top = (ClientRect.top + ClientRect.bottom) / 2 - 20;
		rect.bottom = (ClientRect.top + ClientRect.bottom) / 2 + 20;
		break;
	case WM_KEYDOWN:		//处理键盘按键消息
		bFresh = TRUE;		//默认刷新用户区
		switch (wParam)
		{
		case VK_CONTROL:		//按下Ctrl键时，画椭圆，同时设置不画矩形
			bEllipse = TRUE;
			bRect = FALSE;
			break;
		case VK_SHIFT:			//按下Shift键时，画矩形，同时设置不画椭圆
			bEllipse = FALSE;
			bRect = TRUE;
			break;
		case VK_RIGHT:			//按向右箭头时，计算图形长度加10后的坐标
			rect.right += 10;
			break;
		case VK_LEFT:			//按向左箭头时，计算图形长度减10后的坐标，图形长度最小为10
			rect.right = (rect.right - rect.left <= 10) ? (rect.left + 10): (rect.right - 10);
			break;
		case VK_DOWN:			//按向下箭头时，计算图形高度加10后的坐标
			rect.bottom += 10;
			break;
		case VK_UP:				//按向上箭头时，计算图形高度减10后的坐标，图形高度最小为10
			rect.bottom = (rect.bottom - rect.top <= 10) ? (rect.top + 10) : (rect.bottom - 10);
			break;
		case VK_HOME:			//按Home键时，图形左移nStep长度，计算左移后各点坐标
			rect.left -= nStep;
			rect.right -= nStep;
			break;
		case VK_END:			//按End键时，图形右移nStep长度，计算右移后各点坐标
			rect.left += nStep;
			rect.right += nStep;
			break;
		case VK_PRIOR:			//按Page Up键时，图形上移nStep长度，计算上移后各点坐标
			rect.top -= nStep;
			rect.bottom -= nStep;
			break;
		case VK_NEXT:			//按Page Down键时，图形下移nStep长度，计算下移后各点坐标
			rect.top += nStep;
			rect.bottom += nStep;
			break;
		default:
			bFresh = FALSE;		//如果按键未定义，不刷新用户区
			break;
		}
		if (bFresh)
			InvalidateRect(hWnd, NULL, TRUE);	//刷新整个用户区，使按键立即生效
		break;
	case WM_PAINT:										//处理绘图消息
		hDC = BeginPaint(hWnd, &PtStr);					//获得设备环境指针
		TextOut(hDC, 0, 0, lpsz, lstrlen(lpsz));		//输出操作提示
		hPen = (HPEN)GetStockObject(BLACK_PEN);			//获取黑色画笔
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);	//获取虚画刷

		//绘制出给定位置和大小的图形，位置和大小由rect记录的坐标决定
		//若bEllipse为真，绘制椭圆
		if (bEllipse)
		{
			Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);
		}

		//若bRect为真，绘制椭圆
		if (bRect)
		{
			Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
		}

		DeleteObject(hBrush);	//删除画刷对象
		DeleteObject(hPen);		//删除画笔对象
		EndPaint(hWnd, &PtStr);	//删除设备环境句柄
		break;
	case WM_DESTROY:										//处理关闭窗口消息
		PostQuitMessage(0);									//发送WM_QUIT，退出程序
		break;
	default:												//默认消息处理函数
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
