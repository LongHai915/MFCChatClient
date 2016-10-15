
// MFCChatClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCChatClient.h"
#include "MFCChatClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCChatClientDlg 对话框



CMFCChatClientDlg::CMFCChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCChatClientDlg::IDD, pParent)
	, m_ipAddress(0)
	, m_port(0)
	, m_isConnect(FALSE)
	, m_clientSocket(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS_IP, m_ipAddress);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Control(pDX, IDC_RICHEDIT_MSG1, m_RichLog);
	DDX_Control(pDX, IDC_BTN_FACE, m_BmpPicker);
	DDX_Control(pDX, IDC_RICHEDIT_MSG2, m_RichEdit);
}

BEGIN_MESSAGE_MAP(CMFCChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_SERVER, &CMFCChatClientDlg::OnBtnConnectServer)
	ON_BN_CLICKED(IDC_BTN_SEND, &CMFCChatClientDlg::OnBtnSend)
	ON_MESSAGE(WM_RECIEVE, &CMFCChatClientDlg::OnMsgRecv)
END_MESSAGE_MAP()


// CMFCChatClientDlg 消息处理程序
BOOL CMFCChatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	//首先把控件的属性设置为多行
	CEdit *ce = (CEdit*)GetDlgItem(IDC_EDIT_PORT);
	CRect rect;
	ce->GetClientRect(&rect); //hwndEdit为编辑框的对象
	//设置内容的,左边距与上边距
	::OffsetRect(&rect, 0, 4);
	::SendMessage(ce->m_hWnd, EM_SETRECT, 0, (LPARAM)&rect);
	//获取本机ip地址
	char sHostName[100];
	gethostname(sHostName, sizeof(sHostName));
	struct hostent FAR * lpHostEnt = gethostbyname(sHostName);
	if (lpHostEnt == NULL)
	{
		MessageBox(_T("error"));
	}

	LPSTR lpAddr = lpHostEnt->h_addr_list[0];
	if (lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr, lpAddr, 4);
		DWORD  dw1 = inet_addr(inet_ntoa(inAddr));
		m_ipAddress = htonl(dw1); 
		//UpdateData(FALSE);
	}
	m_port = 5566;
	UpdateData(FALSE);
	m_loginUser = "abc3";

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCChatClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCChatClientDlg::OnBtnConnectServer()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_isConnect)
	{
		// disconnect
		if (IDOK == MessageBox(_T("Are you sure to close it?"), _T("warnning"), MB_OKCANCEL | MB_ICONQUESTION))
		{
			this->StopServer();
			m_isConnect = false;
			SetDlgItemText(IDC_BTN_CONNECT_SERVER, _T("Start Server"));
			SetDlgItemText(IDC_STATIC_STATUS, _T("Disconnected."));
		}
		return;
	}
	if (m_port > 65535 || m_port < 1024)
	{
		CEdit* ce = (CEdit *)GetDlgItem(IDC_EDIT_PORT);
		ce->ShowBalloonTip(_T("Port"), _T("Port is only from 1024 to 65535"), TTI_WARNING);
		ce->SetWindowText(_T("65535"));
		return;
	}
	AfxBeginThread(/*(AFX_THREADPROC)*/ThreadProc, this, THREAD_PRIORITY_NORMAL);
}

/************************************************************************/
/* 初始化套接口                                                          */
/************************************************************************/
BOOL CMFCChatClientDlg::WinSockInit()
{
	WSADATA wsaData = { 0 };
	WORD wRequestVersion = MAKEWORD(2, 2);
	if (WSAStartup(wRequestVersion, &wsaData))
	{
		MessageBox(_T("wsa loads failed."), _T("Error"), MB_OK | MB_ICONERROR);
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		MessageBox(_T("wsa version error."), _T("Error"), MB_OK | MB_ICONERROR);
		WSACleanup();
		return false;
	}
	return true;
}

UINT CMFCChatClientDlg::ThreadProc(LPVOID pParam)
{
	VERIFY(pParam);
	CMFCChatClientDlg *pThis = (CMFCChatClientDlg*)pParam;
	CString sIP;
	pThis->WinSockInit();
	pThis->m_clientSocket = INVALID_SOCKET;
	pThis->m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	BOOL bReuseaddr = TRUE;
	//setsockopt(pThis->m_serverSocket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (const char*)&bReuseaddr, sizeof(BOOL));
	if (INVALID_SOCKET == pThis->m_clientSocket)
	{
		TRACE(_T("system info: create socket failed: %d\n"), WSAGetLastError());
		//pThis->m_RichLog.AddMessageUser(_T("system info: create socket failed..."), MSG_SYSTEM_COLOR);
		//((CEdit*)pThis->GetDlgItem(IDC_BTN_START_SERVER))->ShowBalloonTip(_T("Error"), _T("create socket failed."), TTI_ERROR_LARGE);
		goto _Error_End;
		/*closesocket(pThis->m_serverSocket);
		WSACleanup();
		return -1;*/
	}
	TRACE(_T("system info: create socket success...\n"));
	//pThis->m_RichLog.AddMessageUser(_T("system info: create socket success..."), MSG_SYSTEM_COLOR);
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(pThis->m_ipAddress);
	serverAddr.sin_port = htons(pThis->m_port);
	if (connect(pThis->m_clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		TRACE(_T("system info: connect failed, please check ip or port: %d\n"), WSAGetLastError());
		goto _Error_End;
	}

	TRACE(_T("system info: connect server success...\n"));
	WSAAsyncSelect(pThis->m_clientSocket, pThis->m_hWnd, WM_RECIEVE, FD_READ | FD_CLOSE);
	pThis->m_RichLog.AddMessageUser(_T("system info: connect server success..."), MSG_SYSTEM_COLOR);
	pThis->m_isConnect = true;
	pThis->SetDlgItemText(IDC_BTN_START_SERVER, _T("Disconnect Server"));
	pThis->SetDlgItemText(IDC_STATIC_STATUS, _T("Connected."));
	return 1;

_Error_End:
	if (pThis->m_clientSocket != INVALID_SOCKET)
		closesocket(pThis->m_clientSocket);
	WSACleanup();
	return 0;
}
/************************************************************************/
/* 关闭套接字                                                           */
/************************************************************************/
void CMFCChatClientDlg::StopServer()
{

	if (INVALID_SOCKET == m_clientSocket)
		closesocket(m_clientSocket);
	m_RichLog.AddMessageUser(_T("disconnect to server, please re-connect."), MSG_SYSTEM_COLOR);
	WSACleanup();
}
/************************************************************************/
/* 接收服务端发送过来的信息                                                      */
/************************************************************************/
afx_msg LRESULT CMFCChatClientDlg::OnMsgRecv(WPARAM wParam, LPARAM lParam)
{
	UserMsg uMsg;
	ZeroMemory(&uMsg, sizeof(UserMsg));
	int errMsg;
	CString msg;
	SOCKET tmpSock = (SOCKET)wParam;
	if (WSAGETSELECTEVENT(lParam) == FD_READ)
	{
		//pos = FindUserInfo((SOCKET)wParam);
		errMsg = recv(tmpSock, (char*)&uMsg, sizeof(UserMsg), 0);
		if (errMsg == SOCKET_ERROR)
		{
			//m_userInfoList.RemoveAt(pos);
			CString msg;
			msg.Format(_T("system info: recieve message failed: %d"), WSAGetLastError());
			m_RichLog.AddMessageUser(msg, MSG_SYSTEM_COLOR);
			return 1;
		}
		//sSender = ((UserInfo*)m_userInfoList.GetAt(pos))->UserName;
		switch (uMsg.Protocol)
		{
		case USER_LOAD:
			break;
		case USER_MSGTOALL:
			uMsg.Protocol = USER_MSGTOALL;
			msg.Format(_T("「%s」sends message to everyone:"), uMsg.UserName);
			m_RichLog.AddMessageUser(msg, MSG_SYSTEM_COLOR);
			msg.Format(_T("%s"), uMsg.Msg);
			m_RichLog.AddMessgaeText(uMsg.Msg, uMsg.m_FontName, uMsg.m_FontSize, uMsg.bIsBold, uMsg.bIsItalic, uMsg.bIsUnderline, uMsg.crColor, &m_BmpPicker);
			break;
		case USER_MSGTOPERSONAL:
			uMsg.Protocol = USER_MSGTOPERSONAL;
			msg.Format(_T("「%s」sends message to you:"), uMsg.UserName);
			m_RichLog.AddMessageUser(msg, MSG_SYSTEM_COLOR);
			msg.Format(_T("%s"), uMsg.Msg);
			m_RichLog.AddMessgaeText(uMsg.Msg, uMsg.m_FontName, uMsg.m_FontSize, uMsg.bIsBold, uMsg.bIsItalic, uMsg.bIsUnderline, uMsg.crColor, &m_BmpPicker);
			break;
		default:
			break;
		}
	}
	else if (WSAGETSELECTEVENT(lParam) == FD_CLOSE)
	{
		if (WSAGETSELECTERROR(lParam) == 0)
		{
			//从容关闭。
			m_RichLog.AddMessageUser(_T("server is closed."), MSG_SYSTEM_COLOR);
		}
		else if (WSAGETSELECTERROR(lParam) == WSAECONNREFUSED)
		{
			//硬关闭。
			m_RichLog.AddMessageUser(_T("server is closed."), MSG_SYSTEM_COLOR);
		}
	}
	return 0;
}

void CMFCChatClientDlg::OnBtnSend()
{
	// TODO:  在此添加控件通知处理程序代码
	/*if (theApp.bIsStopSpeak)
	{
		m_RichLog.AddMessageUser(_T("很抱歉，您已被禁止发言！"), MSG_SYSTEM_COLOR);
		return;
	}*/
	CString strEdit = m_RichEdit.EncodeRichMessage();
	if (strEdit.IsEmpty())
	{
		MessageBox(_T("Message can not be empty, please input."), _T("Warning"));
		return;
	}
	UpdateData(TRUE);

	CString strFontName, strFontSize;
	/*m_FontName.GetWindowText(strFontName);
	m_FontSize.GetWindowText(strFontSize);
	COLORREF color = m_ColorPicker.GetColor();
	if (color == -1)
	{
	color = m_ColorPicker.GetAutomaticColor();
	}

	CString strTemp;
	m_UserChat.GetLBText(m_UserChat.GetCurSel(), strTemp);*/
	UserMsg msgTemp;
	//ZeroMemory(&msgTemp, sizeof(msgTemp));
	//if (strTemp == _T("所有人"))
	//{
	//	msgTemp.Protocol = USER_MSGTOALL;
	//	_tcscpy_s(msgTemp.UserName, theApp.CurrentLoginUser.GetBuffer(0));
	//	//theApp.CurrentLoginUser.ReleaseBuffer();
	//}
	//else
	//{
	//	msgTemp.Protocol = USER_MSGTOPERSONAL;
	//	_tcscpy_s(msgTemp.UserName, strTemp.GetBuffer(0));
	//	strTemp.ReleaseBuffer();
	//}
	msgTemp.Protocol = USER_MSGTOALL;
	_tcscpy_s(msgTemp.Msg, strEdit.GetBuffer(0));
	strEdit.ReleaseBuffer();
	_tcscpy_s(msgTemp.m_FontName, strFontName);
	msgTemp.m_FontSize = _ttoi(strFontSize);
	_tcscpy_s(msgTemp.UserName, m_loginUser);
	//msgTemp.bIsBold = bIsBold;
	//msgTemp.bIsItalic = bIsItalic;
	//msgTemp.bIsUnderline = bIsUnderline;
	//msgTemp.crColor = color;
	int ErrorMsg = send(m_clientSocket, (char *)&msgTemp, sizeof(msgTemp), 0);
	if (ErrorMsg == SOCKET_ERROR)
	{
		CString msg;
		msg.Format(_T("system info: send message failed: %d"), WSAGetLastError());
		m_RichLog.AddMessageUser(msg, MSG_SYSTEM_COLOR);
		//MessageBox(_T("给客户端发送数据失败，客户端已退出！"), _T("错误提示"), MB_OK | MB_ICONERROR);
	}
	m_RichEdit.SetWindowText(_T(""));
	m_RichEdit.SetFocus();
}

