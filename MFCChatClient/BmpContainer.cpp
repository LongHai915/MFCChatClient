/*
 *	文件名：BmpContainer.cpp
 *	作用：位图拾取器的容器面板
 */

// BmpContainer.cpp : 实现文件
//

#include "stdafx.h"
#include "BmpContainer.h"
#include "BmpPicker.h"
#include "math.h"
// CBmpContainer
#define IDC_INNERBUTTON 10000
#define IDC_ICONCONTAINER 10001
IMPLEMENT_DYNAMIC(CBmpContainer, CWnd)

CBmpContainer::CBmpContainer()
{
	m_nRow=0;
	m_nCol=0;
	m_bClosed=FALSE;
}

CBmpContainer::~CBmpContainer()
{
	for(int i=0;i<m_InnerButtonArray.GetSize();i++)
	{
		CInnerButton* pInnerButton;
		pInnerButton=m_InnerButtonArray.GetAt(i);
		pInnerButton->SendMessage(WM_CLOSE,0,0);
		//delete pInnerButton;
	}
}


BEGIN_MESSAGE_MAP(CBmpContainer, CWnd)
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CBmpContainer 消息处理程序



BOOL CBmpContainer::Create(POINT pt,CButton* pParentButton,CArray<CBitmap*,CBitmap*> *pBitmapArray) 
{
	if(pBitmapArray->GetSize()<=0)return FALSE;

	m_pParentButton=pParentButton;

	///根据每张图片的大小创建IconContainer
	//m_nCol=int(sqrt((double)pBitmapArray->GetSize()))+1;    //计算列数
	m_nCol=10;
	BITMAP bm;
	pBitmapArray->GetAt(0)->GetBitmap(&bm);			//以图片列表中的第0号图片的大小为基准
	m_nCellWidth=bm.bmWidth+4;						//内部单元的宽度
	m_nCellHeight=bm.bmHeight+4;					//内部单元的高度

	CRect rect;
	rect.left=pt.x,rect.top=pt.y;
	rect.right=pt.x+m_nCellWidth*m_nCol;			//容器的宽度

	if(pBitmapArray->GetSize()%m_nCol==0)			//计算行数
	{
		m_nRow=pBitmapArray->GetSize()/m_nCol;
	}
	else
	{
		m_nRow=pBitmapArray->GetSize()/m_nCol+1;
	}
	//rect.bottom=pt.y+m_nCellHeight*m_nRow+2+46;		///容器的高度=(行数+2)*单元宽度,要多两个扩展功能按钮的话，用这一行
	rect.bottom=pt.y+m_nCellHeight*m_nRow+2;
	rect.top=pt.y-m_nCellHeight*m_nRow-2;
	rect.bottom=pt.y;
	//pParentButton->GetParent()->ScreenToClient(&rect);

	///创建容器
	//CWnd::Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
	//rect,pParentButton->GetParent(),IDC_ICONCONTAINER, NULL);
	CWnd::CreateEx(WS_EX_LEFT| WS_EX_TOOLWINDOW,AfxRegisterWndClass(0),NULL,WS_VISIBLE|WS_POPUP,rect,NULL,NULL );
	///创建图片张数+2个按钮
	for(int i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol&& i*m_nCol+j<pBitmapArray->GetSize();j++)
		{
			///计算按钮的位置
			CRect innerrect;
			innerrect.left=j*m_nCellWidth;
			innerrect.top=i*m_nCellHeight;
			innerrect.right=innerrect.left+m_nCellWidth;
			innerrect.bottom=innerrect.top+m_nCellHeight;

			innerrect.DeflateRect(2,2);

			///新建按钮
			CInnerButton *pInnerButton;
			pInnerButton=new CInnerButton;
			pInnerButton->Create(NULL,WS_CHILD |WS_VISIBLE,
				innerrect,this,IDC_INNERBUTTON+i*m_nCol+j);

			///设置按钮的图标
			pInnerButton->SetBitmap(pBitmapArray->GetAt(i*m_nCol+j));
			pInnerButton->ShowWindow(SW_SHOW);

			///记录该按钮的指针
			m_InnerButtonArray.Add(pInnerButton);
		}
	}


	////创建两个扩展功能按钮
	/*
	this->GetClientRect(&rect);
	CInnerButton *pInnerButton;
	pInnerButton=new CInnerButton;

	rect.left=2;
	rect.right-=2;
	rect.top=m_nRow*m_nCellHeight+3;
	rect.bottom=rect.top+20;  //按钮的高度为22


	pInnerButton->Create(NULL,WS_CHILD |WS_VISIBLE,
		rect,this,IDC_INNERBUTTON-1);
	pInnerButton->SetFont(m_pParentButton->GetParent()->GetFont());
	pInnerButton->SetWindowText(_T("显示更多的图释"));

	pInnerButton=new CInnerButton;

	rect.top=rect.bottom+3;
	rect.bottom=rect.top+20;
	pInnerButton->Create(NULL,WS_CHILD |WS_VISIBLE,
		rect,this,IDC_INNERBUTTON-2);
	pInnerButton->SetWindowText(_T("我的自定义图释"));
	pInnerButton->SetFont(m_pParentButton->GetParent()->GetFont());
	*/
	return TRUE;
}

void CBmpContainer::OnKillFocus(CWnd* pNewWnd)
{
	POINT pt;
	::GetCursorPos(&pt);	
	if(::IsChild(this->m_hWnd,::WindowFromPoint(pt)))
	{
		///此时焦点在小图片上面,所以不关闭本窗口
		return;
	}
	///用户没有选择任何图片而单击了其他地方
	Close();
}

void CBmpContainer::OnPaint()
{
	CPaintDC dc(this); 

	///准备工作
	RECT rect;
	this->GetClientRect(&rect);
	CPen BorderPen,*pOldPen,LinePen,VPen;
	LinePen.CreatePen(PS_SOLID,1,RGB(222,222,222));
	BorderPen.CreatePen(PS_SOLID,1,RGB(79,107,160));
	VPen.CreatePen(PS_SOLID,1,RGB(165,182,222));

	///画底色
	pOldPen=dc.SelectObject(&BorderPen);

	dc.Rectangle(&rect);

	dc.SelectObject(pOldPen);
	///画横线
	/**/
	dc.SelectObject(&LinePen);
	for(int i=1;i<m_nRow;i++)
	{
		dc.MoveTo(2,i*m_nCellHeight);
		dc.LineTo(rect.right-2,i*m_nCellHeight);
	}
	///画两条扩展功能横线
	/*
	dc.MoveTo(2,m_nRow*m_nCellHeight);
	dc.LineTo(rect.right-2,m_nRow*m_nCellHeight);
	dc.MoveTo(2,m_nRow*m_nCellHeight+24);
	dc.LineTo(rect.right-2,m_nRow*m_nCellHeight+24);
	*/
	//画纵线
	dc.SelectObject(&VPen);
	for(int j=1;j<m_nCol;j++)
	{
		dc.MoveTo(j*m_nCellWidth,2);
		dc.LineTo(j*m_nCellWidth,m_nRow*m_nCellHeight-2);
	}
}

////把用户选择的图片序号发送给父窗口CIconPicker,并关闭自己
void CBmpContainer::Close(int index)
{
	this->ShowWindow(SW_HIDE);

	RECT rect;
	this->GetWindowRect(&rect);
	m_pParentButton->GetParent()->ScreenToClient(&rect);
	m_pParentButton->GetParent()->InvalidateRect(&rect);

	::SendMessage(m_pParentButton->GetSafeHwnd(),WM_USER+101,index,0);
	//this->SendMessage(WM_CLOSE,0,0);

}

BOOL CBmpContainer::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if(LOWORD(wParam)-IDC_INNERBUTTON==-1)
	{
		////在这里响应"显示更多的图释"
	}
	if(LOWORD(wParam)-IDC_INNERBUTTON==-2)
	{
		////在这里响应"我的自定义图释"
		CFileDialog SelectFileDlg(TRUE,_T("bmp"),_T("noname"),OFN_FILEMUSTEXIST,
			_T("Bitmap File(*.bmp)|*.bmp"),m_pParentButton);
		if(SelectFileDlg.DoModal()==IDOK)
		{
			((CBmpPicker*)m_pParentButton)->AddBitmap(SelectFileDlg.GetPathName());
		}
	}
	///关闭本窗口
	Close(LOWORD(wParam)-IDC_INNERBUTTON);
	return TRUE;
}
