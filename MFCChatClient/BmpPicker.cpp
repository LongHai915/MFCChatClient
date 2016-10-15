/*
 *	文件名：BmpPicker.cpp
 *	作用：位图拾取器
 */
// BmpPicker.cpp : 实现文件
//

#include "stdafx.h"
#include "BmpPicker.h"


// CBmpPicker

IMPLEMENT_DYNAMIC(CBmpPicker, CButton)

CBmpPicker::CBmpPicker()
{
	m_iCurIndex=0;
	m_pBmpContainer=NULL;
	m_bState=FALSE;
}

CBmpPicker::~CBmpPicker()
{
	CBitmap* pBitmap;
	for(int i=0;i<m_BitmapArray.GetSize();i++)
	{
		pBitmap=m_BitmapArray.GetAt(i);
		pBitmap->DeleteObject();
	}
}


BEGIN_MESSAGE_MAP(CBmpPicker, CButton)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_USER+101,OnSelectICON)
	ON_WM_LBUTTONDBLCLK()
//	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CBmpPicker 消息处理程序



void CBmpPicker::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bState) return ;
	m_bState=TRUE;
	this->SetState(TRUE);

	RECT rect;
	this->GetWindowRect(&rect);

	POINT pt;
	//pt.x=rect.left;pt.y=rect.bottom;
	pt.x=rect.left;pt.y=rect.top;

	m_pBmpContainer=new CBmpContainer;
	if(m_pBmpContainer->Create(pt,this,&m_BitmapArray))
	{
		m_pBmpContainer->ShowWindow(SW_SHOW);
		m_pBmpContainer->UpdateWindow();
		m_pBmpContainer->SetFocus();
	}	
}
///收到来自CIconContainer的消息,返回用户选中的图片标号
///-3表示用户没有选中图片
LRESULT CBmpPicker::OnSelectICON(WPARAM index,LPARAM)
{

	if((int)index>=0 && (int)index<m_BitmapArray.GetSize())
	{
		///如果用户选择了图片,则发送WM_COMMAND给父窗口,表示本按钮被单击
		//	this->SetBitmap(m_BitmapArray.GetAt(index));
		//this->SetMouseOnBitmap(m_BitmapArray.GetAt(index));
		m_iCurIndex=index;

		::SendMessage(GetParent()->m_hWnd,WM_COMMAND,GetDlgCtrlID(),0);
	}
	///否则的话,不发送WM_COMMAND消息
	this->SetState(FALSE);
	this->Invalidate();
	m_bState=FALSE;

	return 0;

}
///以下四个函数设置本按钮的前景图片
void CBmpPicker::AddBitmap(UINT BITMAP_ID)
{
	CBitmap *pBitmap;
	pBitmap=new CBitmap;
	pBitmap->LoadBitmap(BITMAP_ID);
	m_BitmapArray.Add(pBitmap);
}

void CBmpPicker::AddBitmap(HBITMAP hBitmap)
{
	if(hBitmap==NULL)
		return;
	CBitmap *pBitmap;
	pBitmap=new CBitmap;
	pBitmap->Attach(hBitmap);
	m_BitmapArray.Add(pBitmap);
}

void CBmpPicker::AddBitmap(CBitmap* pBitmap)
{
	ASSERT(pBitmap);
	m_BitmapArray.Add(pBitmap);
}

void CBmpPicker::AddBitmap(CString strFileName)
{
	if(!strFileName.IsEmpty())
	{
		HBITMAP hBitmap;
		hBitmap=HBITMAP(::LoadImage(AfxGetInstanceHandle(),strFileName,
			IMAGE_BITMAP,0,0,LR_LOADFROMFILE));
		this->AddBitmap(hBitmap);
	}
}

////根据标号返回图片列表中的图片
CBitmap* CBmpPicker::GetBitmapAt(int index)
{
	if(index<0 && index>=m_BitmapArray.GetSize())
		return NULL;
	return m_BitmapArray.GetAt(index);
}

///返回图片列表中图片的个数
int CBmpPicker::GetBitmapCount()
{
	return m_BitmapArray.GetSize();
}

///返回当前图片的标号
int CBmpPicker::GetCurrentBitmapIndex()
{
	return m_iCurIndex;
}

void CBmpPicker::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//屏蔽双击事件
	//CButton::OnLButtonDblClk(nFlags, point);
}



void CBmpPicker::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	switch (nChar)
	{	
	case 32://OnLButtonDown(nFlags,CPoint(0,0));		
		return;
	default:return;
	}
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CBmpPicker::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==13) 
	{ 		
		//SendMessage(WM_LBUTTONDOWN);
		return TRUE;
	} 
	return CButton::PreTranslateMessage(pMsg);
}
