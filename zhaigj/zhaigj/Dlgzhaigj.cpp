#include "StdAfx.h"
#include "Dlgzhaigj.h"
#include "resource.h"

CDlgzhaigj::CDlgzhaigj(void)
{
}


CDlgzhaigj::~CDlgzhaigj(void)
{
}


BOOL CDlgzhaigj::Initialization(){
	::SetWindowText(GetHWND(),_T("zgj_title"));
	::SetWindowPos(GetHWND(), NULL, 0, 0, 960, 600, 0);
	this->CenterWindow();
	//SetSkinImage(IDB_PNG_MAIN_BK);
	this->SetSkinColor(COLORREF(0xffffffff));
	this->SetIcon(IDI_SMALL);
	this->SetTitleIcon(IDI_ZHAIGJ);

	m_gaThread.Resume();	//��ʼͳ���߳�
	return TRUE;
}

void CDlgzhaigj::Notify(TNotifyUI& msg){

}

LRESULT CDlgzhaigj::OnNcHitTest(WPARAM wParam, LPARAM lParam, BOOL &bHandled){
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;	// �ͻ�������
	RECT rcCaption;	// ���϶�����
	bHandled = TRUE;// �ӹ�
	::GetClientRect(*this, &rcClient);
	rcCaption = rcClient;
	if(PtInRect(&rcCaption,pt)){
		return HTCAPTION;
	}
	return HTCLIENT;
}

LRESULT CDlgzhaigj::OnSystemCmd(WPARAM wParam, LPARAM lParam, BOOL &bHandled){
	switch (wParam){
	case SC_CLOSE:	// �������Ҽ��ر���Ϣ
		this->Close();
		m_gaThread.CancelThread();
		PostQuitMessage(0);
		break;
	}
	return CWindowWnd::HandleMessage(WM_SYSCOMMAND, wParam, lParam);
}