/*
 *	文件名：InnerButton.cpp
 *	作用：位图拾取器的容器面板上的自绘按钮
 *	注意：下面代码鼠标滑过的时候加载一张“面纱”的bmp图片，根据需要改动。
 */
// InnerButton.cpp : 实现文件
//

#include "stdafx.h"
#include "InnerButton.h"
#include "BmpPicker.h"

// CInnerButton
/*                  CInnerButton
用来显示一张图片
能够感应鼠标的经过
从CButton派生的一个自画按钮
*/
IMPLEMENT_DYNAMIC(CInnerButton, CButton)

CInnerButton::CInnerButton()
{
	m_bOver=FALSE;
	m_bTracking=FALSE;
	m_pBitmap=NULL;
}

CInnerButton::~CInnerButton()
{
}


BEGIN_MESSAGE_MAP(CInnerButton, CButton)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CInnerButton 消息处理程序



void CInnerButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//从lpDrawItemStruct获取控件的相关信息
	CRect rect =  lpDrawItemStruct->rcItem;
	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	int nSaveDC=pDC->SaveDC();
	UINT state = lpDrawItemStruct->itemState;
	TCHAR szText[MAX_PATH + 1];
	::GetWindowText(m_hWnd, szText, MAX_PATH);	
	/////
	///准备工作
	CPen *pOldPen,BorderPen,NullPen;
	HBRUSH pOldBrush;
	CBrush BGBrush;
	NullPen.CreatePen(PS_NULL,0,RGB(241,238,231));
	BGBrush.CreateSolidBrush(RGB(241,238,231));
	BorderPen.CreatePen(PS_SOLID,1,RGB(79,107,160));
	
	///如果鼠标经过的话,画一个边框
	if(m_bOver)
	{
		pOldPen=pDC->SelectObject(&NullPen);
		pDC->SelectObject(&BGBrush);
		pDC->Rectangle(&rect);
		pDC->SelectObject(pOldPen);

	}
	
	////贴上图片
	BITMAP bm;
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SetMapMode(pDC->GetMapMode());

	if(m_pBitmap!=NULL)
	{
		m_pBitmap->GetBitmap(&bm);
		MemDC.SelectObject(m_pBitmap);
		/*
		////这里是TransparentBlt()的用法,您也可以用这个函数来实现同样的
		///功能,注意要在本文件头部加入 "#pragma comment( lib, "Msimg32.lib" )"
		if(bm.bmHeight>rect.bottom || bm.bmWidth>rect.right)
			//pDC->StretchBlt(0,0,rect.right,rect.bottom,&MemDC,0,0,bm.bmWidth,
				//bm.bmHeight,SRCCOPY);
			TransparentBlt(pDC->GetSafeHdc(),0,0,rect.right,rect.bottom,
				MemDC.GetSafeHdc(),0,0,bm.bmWidth,bm.bmHeight,RGB(0xff,0xff,0xff));
		else
			//pDC->BitBlt(0,0,rect.right,rect.bottom,&MemDC,0,0,SRCCOPY);
			TransparentBlt(pDC->GetSafeHdc(),0,0,rect.right,rect.bottom,
				MemDC.GetSafeHdc(),0,0,bm.bmWidth,bm.bmHeight,RGB(0xff,0xff,0xff));*/
		//如果想实现图标选择器,用下面这行就可以了
		//pDC->DrawIcon(0,0,m_hIcon);

		if(bm.bmHeight>rect.bottom || bm.bmWidth>rect.right)
			pDC->StretchBlt(0,0,rect.right,rect.bottom,&MemDC,0,0,bm.bmWidth,
				bm.bmHeight,SRCCOPY);
		else
			pDC->BitBlt(0,0,rect.right,rect.bottom,&MemDC,0,0,SRCCOPY);
		if(m_bOver)
		{
			///如果鼠标经过的画,在图片上面在贴上一张薄薄的"面纱"
			CBitmap MarkBitmap;
			//MarkBitmap.LoadBitmap(IDB_BITMAP_INNERBUTTONMARK);   
			MarkBitmap.m_hObject=(HBITMAP)::LoadImage(NULL,_T("img\\InnerButtonMark.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
			MemDC.SelectObject(MarkBitmap);
			pDC->BitBlt(0,0,rect.right,rect.bottom,&MemDC,0,0,SRCAND);
			MarkBitmap.DeleteObject();
		}
	}
	MemDC.DeleteDC();

	//鼠标经过状态
	if(m_bOver)
	{
		
		////////////
		pOldPen=pDC->SelectObject(&BorderPen);
		pOldBrush=HBRUSH(pDC->SelectObject(HBRUSH(::GetStockObject(NULL_BRUSH))));

		pDC->Rectangle(&rect);

		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);

	}
	///输出文本
	if (szText!=NULL)
	{
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		CFont* hFont = GetFont();
		CFont* hOldFont = pDC->SelectObject(hFont);
		CSize sizeExtent = pDC->GetTextExtent(szText, lstrlen(szText));
		CPoint pt;
		pt.x=(rect.right-tm.tmAveCharWidth*lstrlen(szText))/2-2;
		pt.y=rect.bottom-tm.tmHeight-2;
		
		int nMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(60,77,129));
		if (state & ODS_DISABLED)
			pDC->DrawState(pt, sizeExtent, szText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		else
			pDC->DrawState(pt, sizeExtent, szText, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
		pDC->SelectObject(hOldFont);
		pDC->SetBkMode(nMode);
	}
}

void CInnerButton::PreSubclassWindow()
{
	this->ModifyStyle(0,BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}

LRESULT CInnerButton::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	m_bOver=FALSE;
	m_bTracking=FALSE;
	this->Invalidate();
	return 0;
}

LRESULT CInnerButton::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
	m_bOver=TRUE;
	this->Invalidate();
	return 0;
}

void CInnerButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		_TrackMouseEvent(&tme);
		m_bTracking=TRUE;
	}

	CButton::OnMouseMove(nFlags, point);
}

BOOL CInnerButton::OnEraseBkgnd(CDC* pDC)
{
	RECT rect;
	this->GetClientRect(&rect);
	CPen BorderPen;
	BorderPen.CreatePen(PS_SOLID,1,RGB(255,255,255));
	pDC->SelectObject(&BorderPen);
	pDC->SelectObject(HBRUSH(::GetStockObject(WHITE_BRUSH)));
	pDC->Rectangle(&rect);

	return TRUE;
}
void CInnerButton::SetBitmap(CBitmap *pBitmap)
{
	m_pBitmap=pBitmap;
}