/*
 *	文件名：BmpPicker.h
 *	作用：位图拾取器
 */
#pragma once


#include "afxtempl.h"
#include "BmpContainer.h"
class CBmpPicker : public CButton
{
	DECLARE_DYNAMIC(CBmpPicker)

public:
	CBmpPicker();
	virtual ~CBmpPicker();
	int GetCurrentBitmapIndex();
	void AddBitmap(CString strFileName);
	int GetBitmapCount();
	CBitmap* GetBitmapAt(int index);
	void AddBitmap(UINT BITMAP_ID);
	void AddBitmap(HBITMAP hBitmap);
	void AddBitmap(CBitmap* pBitmap);

protected:
	DECLARE_MESSAGE_MAP()
	int m_iCurIndex;
	CArray<CBitmap*,CBitmap*> m_BitmapArray;
	BOOL m_bState;
	CBmpContainer *m_pBmpContainer;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnSelectICON(WPARAM index,LPARAM);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


