/*
 *	文件名称：RichEditCtrlEx.h
 *	当前版本：1.0 Unicode版本
 *	作用：CRichEditCtrl的扩展类，带转义字符的表情图片，右键菜单消息，自动检测超链接，自动换行，RTF操作
 *	作者：无幻
 *	原作者：阙荣文 (querw)
 *	完成日期：2010年10月25日
 *	网址：http://blog.csdn.net/akof1314
 *	
 *	使用：
 *	在CxxxApp::InitInstance()中添加： AfxInitRichEdit2();
 *	在对话框上拖动一个Rich Eidt 2.0控件，设置其Multiline为True，want return 为True，Vertical Scroll为True；
 *	为其添加变量，变量类型为CRichEditCtrlEx，之后再调用这个对象的相关方法。
 *	当作为聊天记录框的时候，把聊天编辑框做为它的下一个TAB键顺序控件
 */
#pragma once


#include <afxodlgs.h>
#include <afxpriv.h>
#include "oleimpl2.h"
#include "Richole.h"
#include "BmpPicker.h"
#define CHATMSG	1
#define CHATLOG	2
class CRichEditCtrlEx : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CRichEditCtrlEx)

public:
	CRichEditCtrlEx();
	virtual ~CRichEditCtrlEx();

	static DWORD CALLBACK CStringFormatToRTF(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK RTFFormatToCString(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	void SetRTF(CString strRTF);
	CString GetRTF();
	void InsertBitmap(HBITMAP hBitmap,int index);
	void InsertBitmap(CString strFileName);
	void SetType(int type);	//设置聊天编辑框，或者聊天记录框（★★★★★）
	CString EncodeRichMessage();   //编码成转义字符
	void DecodeRichMessage(CString strText,CBmpPicker* m_BmpPicker);  //解码成表情图片
	void AddMessageUser(CString strName,COLORREF crColor);
	void AddMessgaeText(CString strText,CString crFontName,UINT crFontSize,BOOL bIsBold,BOOL bIsItalic,BOOL bIsUnderline,COLORREF crColor,CBmpPicker* m_BmpPicker);
protected:
	DECLARE_MESSAGE_MAP()
	LPSTORAGE m_lpStorage;  // provides storage for m_lpObject
	LPLOCKBYTES m_lpLockBytes;  // part of implementation of m_lpStorage
	LPOLEOBJECT m_lpObject; // in case you want direct access to the OLE object
	LPVIEWOBJECT2 m_lpViewObject;// IViewObject for IOleObject above
	int m_type;

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCopy() ;
	afx_msg void OnCut() { Cut(); }
	afx_msg void OnPaste();
	afx_msg void OnSelectall() { SetSel(0, -1); }
	afx_msg void OnUndo() { Undo(); }
	afx_msg void OnClear() { SetWindowText(_T("")); }
	afx_msg void OnSelectfont();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnEnLink(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	virtual void PreSubclassWindow();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


struct stFace
{
	long nPos;			//要插入的位置
	int nAtFace;		//插入表情的代号
	CString strFace;		//要插入的图的转义字符
	int nLength;		//转义字符表情的长度
};