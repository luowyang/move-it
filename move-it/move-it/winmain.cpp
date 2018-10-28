//Description: ���һ�����̳��򣬵���Ctrl��ʱ������Ҫ����Բ������Shift��ʱ������Ҫ�����Ρ�
//			   Ȼ�󵥻����Ҽ�ͷ������Բ����εĳ��ȼ�10���������¼�ͷʱ����Բ����εĸ߶ȼ�10��
//			   ����Home��ʱ������Բ�λ���������ƶ�������End��ʱ������Բ�λ���������ƶ���
//			   ����PageUp��ʱ������Բ�λ���������ƶ�������PageDown��ʱ������Բ�λ���������ƶ���
//Author: Luo Wenyang
//Date: 2018/10/28
//Environment: ��ȷ����Visual Studio 2017 IDE�б��룬��������������ο�README-����˵��

#include <windows.h>

//��ɳ�ʼ�����̣����岢ע�ᴰ���࣬��������ʾ����
//�������ʾ������ʹ�����ʾ��ʽ
//����һ������ֵ��ΪTRUE˵����ʼ���ɹ���ΪFALSE˵����ʼ��ʧ��
BOOLEAN InitWindow(HINSTANCE hInstance, int nCmdShow);

//������Ϣ���������Զ����WM_CREATE��WM_PAINT�Ĵ������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//���ڳ������ں���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;	//������Ϣ����

	//��ʼ�����ڣ����ʧ�ܾ͵���һ����ʾ�򲢴����˳�
	if (!InitWindow(hInstance, nCmdShow))
	{
		MessageBox(NULL, "��������ʧ��!", "��������", NULL);
		return 1;
	}

	//��Ϣѭ��
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOLEAN InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;														//���崰�ھ��
	TCHAR szWindowClass[] = "MOVEIT";								//���崰��������
	TCHAR szTitle[] = "�ü��̿���ͼ����ʾ";							//���崰�ڱ���
	WNDCLASS WndClass;												//���崰����
	WndClass.cbClsExtra = 0;										//�޴�������չ
	WndClass.cbWndExtra = 0;										//�޴���ʵ����չ
	WndClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));	//����Ϊ��ɫ
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//���ڲ��ü�ͷ���
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//����ȱʡͼ��
	WndClass.hInstance = hInstance;									//��ǰӦ�ó�����
	WndClass.lpfnWndProc = WndProc;									//���ڴ�����
	WndClass.lpszClassName = szWindowClass;							//����������
	WndClass.lpszMenuName = NULL;									//�޴��ڲ˵�
	WndClass.style = 0;												//��������Ϊȱʡ����

	if (!RegisterClass(&WndClass))	//ע�ᴰ��
		return FALSE;
	hWnd = CreateWindow(
		szWindowClass,				//��������
		szTitle,					//������
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,				//�������Ͻ�����
		0,
		CW_USEDEFAULT,				//����ȱʡ�Ŀ�Ⱥ͸߶�
		0,
		NULL,						//�޸�����
		NULL,						//�����˵�
		hInstance,					//��ǰʵ�����
		NULL						//����Ҫ���ݲ���
	);
	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, nCmdShow);		//��ʾ����
	UpdateWindow(hWnd);				//���²������û���
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;						//�����豸�������
	PAINTSTRUCT PtStr;				//���������ͼ��Ϣ�Ľṹ�����
	HPEN hPen;						//���廭�ʽṹ�����
	HBRUSH hBrush;					//���廭ˢ�ṹ�����
	static RECT rect;				//�������ڱ���ͼ������Ľṹ�����
	static BOOLEAN bEllipse = TRUE;	//���徲̬�����ͱ�����Ϊ���ʾ����Բ��Ϊ�ٱ�ʾ������Բ����ʼΪ��
	static BOOLEAN bRect = FALSE;	//���徲̬�����ͱ�����Ϊ���ʾ�����Σ�Ϊ�ٱ�ʾ�������Σ���ʼΪ��
	RECT ClientRect;				//���屣��ͻ�����С�Ľṹ�����
	int nStep = 10;					//����ͼ���ƶ��Ĳ���Ϊ10
	BOOLEAN bFresh;					//���岼���ͱ�����Ϊ��ˢ���û�����Ϊ�ٲ�ˢ���û���

	//����ʾ���֣����ڽ��в�����ʾ
	LPCSTR lpsz = {
		"Ctrl����Բ��Shift�����Σ������ȼ�10�������ȼ�10�����߶ȼ�10�����߶ȼ�10��Home�����ƶ���End�����ƶ���Page Up�����ƶ���Page Down�����ƶ�",
	};

	switch (message)
	{
	case WM_CREATE:	//�����ڴ�����Ϣ
		//����ͻ������ģ�������λ����Ϊͼ�����ģ���10X10��С����ͼ����ʼ����
		GetClientRect(hWnd, &ClientRect);
		rect.left = (ClientRect.left + ClientRect.right) / 2 - 20;
		rect.right = (ClientRect.left + ClientRect.right) / 2 + 20;
		rect.top = (ClientRect.top + ClientRect.bottom) / 2 - 20;
		rect.bottom = (ClientRect.top + ClientRect.bottom) / 2 + 20;
		break;
	case WM_KEYDOWN:		//������̰�����Ϣ
		bFresh = TRUE;		//Ĭ��ˢ���û���
		switch (wParam)
		{
		case VK_CONTROL:		//����Ctrl��ʱ������Բ��ͬʱ���ò�������
			bEllipse = TRUE;
			bRect = FALSE;
			break;
		case VK_SHIFT:			//����Shift��ʱ�������Σ�ͬʱ���ò�����Բ
			bEllipse = FALSE;
			bRect = TRUE;
			break;
		case VK_RIGHT:			//�����Ҽ�ͷʱ������ͼ�γ��ȼ�10�������
			rect.right += 10;
			break;
		case VK_LEFT:			//�������ͷʱ������ͼ�γ��ȼ�10������꣬ͼ�γ�����СΪ10
			rect.right = (rect.right - rect.left <= 10) ? (rect.left + 10): (rect.right - 10);
			break;
		case VK_DOWN:			//�����¼�ͷʱ������ͼ�θ߶ȼ�10�������
			rect.bottom += 10;
			break;
		case VK_UP:				//�����ϼ�ͷʱ������ͼ�θ߶ȼ�10������꣬ͼ�θ߶���СΪ10
			rect.bottom = (rect.bottom - rect.top <= 10) ? (rect.top + 10) : (rect.bottom - 10);
			break;
		case VK_HOME:			//��Home��ʱ��ͼ������nStep���ȣ��������ƺ��������
			rect.left -= nStep;
			rect.right -= nStep;
			break;
		case VK_END:			//��End��ʱ��ͼ������nStep���ȣ��������ƺ��������
			rect.left += nStep;
			rect.right += nStep;
			break;
		case VK_PRIOR:			//��Page Up��ʱ��ͼ������nStep���ȣ��������ƺ��������
			rect.top -= nStep;
			rect.bottom -= nStep;
			break;
		case VK_NEXT:			//��Page Down��ʱ��ͼ������nStep���ȣ��������ƺ��������
			rect.top += nStep;
			rect.bottom += nStep;
			break;
		default:
			bFresh = FALSE;		//�������δ���壬��ˢ���û���
			break;
		}
		if (bFresh)
			InvalidateRect(hWnd, NULL, TRUE);	//ˢ�������û�����ʹ����������Ч
		break;
	case WM_PAINT:										//�����ͼ��Ϣ
		hDC = BeginPaint(hWnd, &PtStr);					//����豸����ָ��
		TextOut(hDC, 0, 0, lpsz, lstrlen(lpsz));		//���������ʾ
		hPen = (HPEN)GetStockObject(BLACK_PEN);			//��ȡ��ɫ����
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);	//��ȡ�黭ˢ

		//���Ƴ�����λ�úʹ�С��ͼ�Σ�λ�úʹ�С��rect��¼���������
		//��bEllipseΪ�棬������Բ
		if (bEllipse)
		{
			Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);
		}

		//��bRectΪ�棬������Բ
		if (bRect)
		{
			Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
		}

		DeleteObject(hBrush);	//ɾ����ˢ����
		DeleteObject(hPen);		//ɾ�����ʶ���
		EndPaint(hWnd, &PtStr);	//ɾ���豸�������
		break;
	case WM_DESTROY:										//����رմ�����Ϣ
		PostQuitMessage(0);									//����WM_QUIT���˳�����
		break;
	default:												//Ĭ����Ϣ������
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
