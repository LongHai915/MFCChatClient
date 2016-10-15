
// MFCChatClientDlg.h : 头文件
//

#pragma once
#include "RichEditCtrlEx.h"
#include "Util.h"
#include "BmpPicker.h"

#define WM_RECIEVE WM_USER+101

#define MSG_MY_COLOR RGB(0,128,64)
#define MSG_USER_COLOR RGB(0,110,254)
#define MSG_SYSTEM_COLOR RGB(0,0,255)

// CMFCChatClientDlg 对话框
class CMFCChatClientDlg : public CDialogEx
{
// 构造
public:
	CMFCChatClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCCHATCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	BOOL WinSockInit();
	void StopServer();

public:
	CString m_loginUser;
	UserInfo uInfo;
	SOCKET m_clientSocket;
	DWORD m_ipAddress;
	UINT m_port;
	BOOL m_isConnect;
	static UINT __cdecl ThreadProc(LPVOID pParam);
	
	afx_msg void OnBtnConnectServer();
	afx_msg void OnBtnSend();
	afx_msg LRESULT OnMsgRecv(WPARAM wParam, LPARAM lParam);
	CRichEditCtrlEx m_RichLog;
	CBmpPicker m_BmpPicker;
	CRichEditCtrlEx m_RichEdit;
};
