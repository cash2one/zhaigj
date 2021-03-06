#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#pragma once

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class CControlUI;


/////////////////////////////////////////////////////////////////////////////////////
//

typedef enum EVENTTYPE_UI
{
    UIEVENT__FIRST = 1,
    UIEVENT__KEYBEGIN,
    UIEVENT_KEYDOWN,
    UIEVENT_KEYUP,
    UIEVENT_CHAR,
    UIEVENT_SYSKEY,
    UIEVENT__KEYEND,
    UIEVENT__MOUSEBEGIN,
    UIEVENT_MOUSEMOVE,
	UIEVENT_MOUSEWHEEL,
    UIEVENT_MOUSELEAVE,
    UIEVENT_MOUSEENTER,
    UIEVENT_MOUSEHOVER,
    UIEVENT_BUTTONDOWN,
    UIEVENT_BUTTONUP,
    UIEVENT_RBUTTONDOWN,
	UIEVENT_RBUTTONUP,
    UIEVENT_DBLCLICK,
    UIEVENT_CONTEXTMENU,
    UIEVENT_SCROLLWHEEL,
    UIEVENT__MOUSEEND,
    UIEVENT_KILLFOCUS,
    UIEVENT_SETFOCUS,
    UIEVENT_WINDOWSIZE,
    UIEVENT_SETCURSOR,
    UIEVENT_TIMER,
    UIEVENT_NOTIFY,
    UIEVENT_COMMAND,
	UIEVENT_SETCHECKED,
	UIEVENT_CHILDCLICK,
    UIEVENT__LAST,
};

/////////////////////////////////////////////////////////////////////////////////////
//

// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000

// Flags for the CDialogLayout stretching
#define UISTRETCH_NEWGROUP   0x00000001
#define UISTRETCH_NEWLINE    0x00000002
#define UISTRETCH_MOVE_X     0x00000004
#define UISTRETCH_MOVE_Y     0x00000008
#define UISTRETCH_SIZE_X     0x00000010
#define UISTRETCH_SIZE_Y     0x00000020
#define UISTRETCH_MOVE_HALF_X     0x00000040
#define UISTRETCH_MOVE_HALF_Y     0x00000080


// Flags used for controlling the paint
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040



/////////////////////////////////////////////////////////////////////////////////////
//

typedef struct tagTFontInfo
{
    HFONT hFont;
    TCHAR sFontName[50];
    int iSize;
    BOOL bBold;
    BOOL bUnderline;
    BOOL bItalic;
    TEXTMETRIC tm;
} TFontInfo;

typedef struct tagTImageInfo
{
    HBITMAP hBitmap;
    int nX;
    int nY;
    BOOL alphaChannel;
	TCHAR sResType[50];
    DWORD dwMask;
} TImageInfo;

// Structure for notifications from the system
// to the control implementation.
typedef struct tagTEventUI
{
    int Type;
    CControlUI* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    TCHAR chKey;
    WORD wKeyState;
    WPARAM wParam;
    LPARAM lParam;
} TEventUI;

// Structure for notifications to the outside world
typedef struct tagTNotifyUI 
{
    CString sType;
    CControlUI* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    WPARAM wParam;
    LPARAM lParam;
} TNotifyUI;

// Structure for relative position to the parent
typedef struct tagTRelativePosUI
{
	BOOL bRelative;
	SIZE szParent;
	int nMoveXPercent;
	int nMoveYPercent;
	int nZoomXPercent;
	int nZoomYPercent;
}TRelativePosUI;

// Listener interface
class INotifyUI
{
public:
    virtual void Notify(TNotifyUI& msg) = 0;
};

// MessageFilter interface
class IMessageFilterUI
{
public:
    virtual LRESULT MessageHandler(LPMSG lpMsg, BOOL& bHandled) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CPaintManagerUI
{
public:
    CPaintManagerUI();
    ~CPaintManagerUI();

public:
    void Init(HWND hWnd,BOOL bFackWnd = FALSE);
    void NeedUpdate();
    void Invalidate(RECT& rcItem);

    HDC GetPaintDC() const;
    HWND GetPaintWindow() const;
    HWND GetTooltipWindow() const;

    POINT GetMousePos() const;
    SIZE GetClientSize() const;
    SIZE GetInitSize();
    void SetInitSize(int cx, int cy);
    RECT& GetSizeBox();
    void SetSizeBox(RECT& rcSizeBox);
    RECT& GetCaptionRect();
    void SetCaptionRect(RECT& rcCaption);
    SIZE GetRoundCorner() const;
    void SetRoundCorner(int cx, int cy);
    SIZE GetMinInfo() const;
    void SetMinInfo(int cx, int cy);
    SIZE GetMaxInfo() const;
    void SetMaxInfo(int cx, int cy);
    void SetTransparent(int nOpacity);
    void SetBackgroundTransparent(BOOL bTrans);
    BOOL IsShowUpdateRect() const;
    void SetShowUpdateRect(BOOL show);
	void ClearEventClick(CControlUI * pContro = NULL);

    static HINSTANCE GetInstance();
    static CString GetInstancePath();
    static CString GetCurrentPath();
    static HINSTANCE GetResourceDll();
    static const CString& GetResourcePath();
    static const CString& GetResourceZip();
    static void SetInstance(HINSTANCE hInst);
    static void SetCurrentPath(LPCTSTR pStrPath);
    static void SetResourceDll(HINSTANCE hInst);
    static void SetResourcePath(LPCTSTR pStrPath);
    static void SetResourceZip(LPCTSTR pStrZip);
	static void SetDefaultResType(LPCTSTR pstrType);


    static void GetHSL(short* H, short* S, short* L);
    static void SetHSL(BOOL bUseHSL, short H, short S, short L); // H:0~360, S:0~200, L:0~200 
	TImageInfo* SetImage(UINT nID,HMODULE hModule,LPCTSTR strImageName);
	TImageInfo* SetImage(UINT nID,HMODULE hModule,LPCTSTR sType,LPCTSTR strImageName);
	TImageInfo* SetImage(HBITMAP hBitMap,UINT nX,UINT nY,BOOL bAph,LPCTSTR strImageName);
    static void ReloadSkin();

    BOOL UseParentResource(CPaintManagerUI* pm);
    CPaintManagerUI* GetParentResource() const;

    DWORD GetDefaultDisabledColor() const;
    void SetDefaultDisabledColor(DWORD dwColor);
    DWORD GetDefaultFontColor() const;
    void SetDefaultFontColor(DWORD dwColor);
    DWORD GetDefaultLinkFontColor() const;
    void SetDefaultLinkFontColor(DWORD dwColor);
    DWORD GetDefaultLinkHoverFontColor() const;
    void SetDefaultLinkHoverFontColor(DWORD dwColor);
    DWORD GetDefaultSelectedBkColor() const;
    void SetDefaultSelectedBkColor(DWORD dwColor);
    TFontInfo* GetDefaultFontInfo();
    void SetDefaultFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic);
    DWORD GetCustomFontCount() const;
    HFONT AddFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic);
    HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic);
    HFONT GetFont(int index);
    HFONT GetFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic);
    BOOL FindFont(HFONT hFont);
    BOOL FindFont(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic);
    int GetFontIndex(HFONT hFont);
    int GetFontIndex(LPCTSTR pStrFontName, int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic);
    BOOL RemoveFont(HFONT hFont);
    BOOL RemoveFontAt(int index);
    void RemoveAllFonts();
    TFontInfo* GetFontInfo(int index);
    TFontInfo* GetFontInfo(HFONT hFont);

    const TImageInfo* GetImage(LPCTSTR bitmap);
    const TImageInfo* GetImageEx(UINT nID, LPCTSTR type = NULL, DWORD mask = 0);
	const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
	const TImageInfo* AddImage(LPCTSTR bitmap,UINT nID,LPCTSTR type = NULL, DWORD mask = 0);
    const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, BOOL bAlpha);
    BOOL RemoveImage(LPCTSTR bitmap);
    void RemoveAllImages();
    void ReloadAllImages();

    void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
    LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
    BOOL RemoveDefaultAttributeList(LPCTSTR pStrControlName);
    const CStringPtrMap& GetDefaultAttribultes() const;
    void RemoveAllDefaultAttributeList();

    BOOL AttachDialog(CControlUI* pControl);
    BOOL InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
    void ReapObjects(CControlUI* pControl);

    BOOL AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
    void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    void RemoveAllOptionGroups();

    CControlUI* GetFocus() const;
    void SetFocus(CControlUI* pControl);
    void SetFocusNeeded(CControlUI* pControl);

    BOOL SetNextTabControl(BOOL bForward = TRUE);

    BOOL SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
    BOOL KillTimer(CControlUI* pControl, UINT nTimerID);
    void KillTimer(CControlUI* pControl);
    void RemoveAllTimers();

    void SetCapture();
    void ReleaseCapture();
    BOOL IsCaptured();

    BOOL AddNotifier(INotifyUI* pControl);
    BOOL RemoveNotifier(INotifyUI* pControl);   
    void SendNotify(TNotifyUI& Msg, BOOL bAsync = FALSE);
    void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, BOOL bAsync = FALSE);

    BOOL AddPreMessageFilter(IMessageFilterUI* pFilter);
    BOOL RemovePreMessageFilter(IMessageFilterUI* pFilter);

    BOOL AddMessageFilter(IMessageFilterUI* pFilter);
    BOOL RemoveMessageFilter(IMessageFilterUI* pFilter);

    int GetPostPaintCount() const;
    BOOL AddPostPaint(CControlUI* pControl);
    BOOL RemovePostPaint(CControlUI* pControl);
    BOOL SetPostPaintIndex(CControlUI* pControl, int iIndex);

    void AddDelayedCleanup(CControlUI* pControl);

	
	static LPCTSTR GetDefaultResType();
    CControlUI* GetRoot() const;
    CControlUI* FindControl(POINT pt) const;
    CControlUI* FindControl(LPCTSTR pstrName) const;
	CControlUI* FindControl(UINT nID) const;
    CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
    CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
    CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
    CStdPtrArray* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);
    CStdPtrArray* GetSubControlsByClass();

    static void MessageLoop();
    static BOOL TranslateMessage(const LPMSG pMsg);

    BOOL MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
    BOOL PreMessageHandler(LPMSG lpMsg, LRESULT& lRes);

private:
    static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromUpdate(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlsFromID(CControlUI* pThis, LPVOID pData);

private:
    HWND m_hWndPaint;
    HDC m_hDcPaint;
    HDC m_hDcOffscreen;
    HDC m_hDcBackground;
    HBITMAP m_hbmpOffscreen;
    HBITMAP m_hbmpBackground;
    HWND m_hwndTooltip;
    TOOLINFO m_ToolTip;
    BOOL m_bShowUpdateRect;
	BOOL m_bFakeWnd;
    //
    CControlUI* m_pRoot;
    CControlUI* m_pFocus;
    CControlUI* m_pEventHover;
    CControlUI* m_pEventClick;
    CControlUI* m_pEventKey;
    //
    POINT m_ptLastMousePos;
    SIZE m_szMinWindow;
    SIZE m_szMaxWindow;
    SIZE m_szInitWindowSize;
    RECT m_rcSizeBox;
    SIZE m_szRoundCorner;
    RECT m_rcCaption;
    UINT m_uTimerID;
    BOOL m_bFirstLayout;
    BOOL m_bUpdateNeeded;
    BOOL m_bFocusNeeded;
    BOOL m_bOffscreenPaint;
    BOOL m_bAlphaBackground;
    BOOL m_bMouseTracking;
    BOOL m_bMouseCapture;
    //
    CStdPtrArray m_aNotifiers;
    CStdPtrArray m_aTimers;
    CStdPtrArray m_aPreMessageFilters;
    CStdPtrArray m_aMessageFilters;
    CStdPtrArray m_aPostPaintControls;
    CStdPtrArray m_aDelayedCleanup;
    CStdPtrArray m_aAsyncNotify;
    CStdPtrArray m_aFoundControls;
    CStringPtrMap m_mNameHash;
    CStringPtrMap m_mOptionGroup;
    //
    CPaintManagerUI* m_pParentResourcePM;
    DWORD m_dwDefaultDisabledColor;
    DWORD m_dwDefaultFontColor;
    DWORD m_dwDefaultLinkFontColor;
    DWORD m_dwDefaultLinkHoverFontColor;
    DWORD m_dwDefaultSelectedBkColor;
    TFontInfo m_DefaultFontInfo;
    CStdPtrArray m_aCustomFonts;

    CStringPtrMap m_mImageHash;
    CStringPtrMap m_DefaultAttrHash;
    //
    static HINSTANCE m_hInstance;
    static HINSTANCE m_hResourceInstance;
    static CString m_pStrResourcePath;
    static CString m_pStrResourceZip;
    static short m_H;
    static short m_S;
    static short m_L;
    static CStdPtrArray m_aPreMessages;
	static CString   m_sDefaultResType;
};

} // namespace DuiLib

#endif // __UIMANAGER_H__
