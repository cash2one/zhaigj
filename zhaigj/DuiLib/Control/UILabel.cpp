#include "StdAfx.h"
#include "UILabel.h"

#include <atlconv.h>
namespace DuiLib
{

	Color _MakeRGB(int a, Color cl)
	{
		return Color(a, cl.GetR(), cl.GetG(), cl.GetB());
	}

	Color _MakeRGB(int r, int g, int b)
	{
		return Color(255, r, g, b);
	}

	CLabelUI::CLabelUI() : m_uTextStyle(DT_VCENTER), m_dwTextColor(0), 
		m_dwDisabledTextColor(0),
		m_iFont(-1),
		m_bShowHtml(FALSE),

		m_EnableEffect(FALSE),
		m_gdiplusToken(0),
		m_TextRenderingHintAntiAlias(TextRenderingHintSystemDefault),
		m_TransShadow(60),
		m_TransText(168),
		m_TransShadow1(60),
		m_TransText1(168),
		m_hAlign(DT_LEFT),
		m_vAlign(DT_CENTER),
		m_dwTextColor1(-1),
		m_dwTextShadowColorA(0xff000000),
		m_dwTextShadowColorB(-1),
		m_GradientAngle(0),
		m_EnabledStroke(FALSE),
		m_TransStroke(255),
		m_dwStrokeColor(0),
		m_EnabledShadow(FALSE),
		m_GradientLength(0),
		m_bCanCilck(FALSE)
	{
		m_ShadowOffset.X		= 0.0f;
		m_ShadowOffset.Y		= 0.0f;
		m_ShadowOffset.Width	= 0.0f;
		m_ShadowOffset.Height	= 0.0f;
		GdiplusStartup( &m_gdiplusToken,&m_gdiplusStartupInput, NULL);

		::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
	}

	//************************************
	// Method:    ~CLabelUI
	// FullName:  CLabelUI::~CLabelUI
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Note:	  
	//************************************
	CLabelUI::~CLabelUI()
	{
		try
		{
			GdiplusShutdown( m_gdiplusToken );
		}
		catch (...)
		{
			throw "CLabelUI::~CLabelUI";
		}
	}

	LPCTSTR CLabelUI::GetClass() const
	{
		return _T("LabelUI");
	}

	LPVOID CLabelUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("Label")) == 0 ) return static_cast<CLabelUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CLabelUI::SetTextStyle(UINT uStyle)
	{
		m_uTextStyle = uStyle;
		Invalidate();
	}

	UINT CLabelUI::GetControlFlags() const
	{
		return  (IsEnabled()&&m_bCanCilck? UIFLAG_SETCURSOR : 0);
	}


	UINT CLabelUI::GetTextStyle() const
	{
		return m_uTextStyle;
	}

	void CLabelUI::SetTextColor(DWORD dwTextColor)
	{
		m_dwTextColor = dwTextColor;
		Invalidate();
	}

	DWORD CLabelUI::GetTextColor() const
	{
		return m_dwTextColor;
	}

	void CLabelUI::SetDisabledTextColor(DWORD dwTextColor)
	{
		m_dwDisabledTextColor = dwTextColor;
		Invalidate();
	}

	DWORD CLabelUI::GetDisabledTextColor() const
	{
		return m_dwDisabledTextColor;
	}

	void CLabelUI::SetFont(int index)
	{
		m_iFont = index;
		Invalidate();
	}

	int CLabelUI::GetFont() const
	{
		return m_iFont;
	}

	RECT CLabelUI::GetTextPadding() const
	{
		return m_rcTextPadding;
	}

	void CLabelUI::SetTextPadding(RECT rc)
	{
		m_rcTextPadding = rc;
		Invalidate();
	}

	BOOL CLabelUI::IsShowHtml()
	{
		return m_bShowHtml;
	}

	void CLabelUI::SetShowHtml(BOOL bShowHtml)
	{
		if( m_bShowHtml == bShowHtml ) return;

		m_bShowHtml = bShowHtml;
		Invalidate();
	}

	SIZE CLabelUI::EstimateSize(SIZE szAvailable)
	{
		if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 4);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CLabelUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			m_bFocused = TRUE;
			return;
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			m_bFocused = FALSE;
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			// return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			// return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			 POINT pt = { GET_X_LPARAM(event.lParam), GET_Y_LPARAM(event.lParam) };

		}
		if( event.Type == UIEVENT_BUTTONUP)
		{
			if (m_bCanCilck)
			{
				if(m_pManager)m_pManager->SendNotify(this,_T("label_click"),(WPARAM)event.pSender,0);
			}
			return;
		}
		if( event.Type == UIEVENT_SETCURSOR ) {
			if (m_bCanCilck)
			{
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			}
			else
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			return;
		}
		CControlUI::DoEvent(event);
	}

	void CLabelUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("align")) == 0 ) {
			if( _tcsstr(pstrValue, _T("left")) != NULL ) {
				m_uTextStyle &= ~(DT_CENTER | DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
				m_uTextStyle |= DT_LEFT;
			}
			if( _tcsstr(pstrValue, _T("center")) != NULL ) {
				m_uTextStyle &= ~(DT_LEFT | DT_RIGHT );
				m_uTextStyle |= DT_CENTER;
			}
			if( _tcsstr(pstrValue, _T("right")) != NULL ) {
				m_uTextStyle &= ~(DT_LEFT | DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				m_uTextStyle |= DT_RIGHT;
			}
			if( _tcsstr(pstrValue, _T("top")) != NULL ) {
				m_uTextStyle &= ~(DT_BOTTOM | DT_VCENTER | DT_VCENTER);
				m_uTextStyle |= (DT_TOP | DT_SINGLELINE);
			}
			if( _tcsstr(pstrValue, _T("vcenter")) != NULL ) {
				m_uTextStyle &= ~(DT_TOP | DT_BOTTOM );			
				m_uTextStyle |= (DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			if( _tcsstr(pstrValue, _T("bottom")) != NULL ) {
				m_uTextStyle &= ~(DT_TOP | DT_VCENTER | DT_VCENTER);
				m_uTextStyle |= (DT_BOTTOM | DT_SINGLELINE);
			}
		}
		else if( _tcscmp(pstrName, _T("endellipsis")) == 0 ) {
			if( _tcscmp(pstrValue, _T("TRUE")) == 0 ) m_uTextStyle |= DT_END_ELLIPSIS;
			else m_uTextStyle &= ~DT_END_ELLIPSIS;
		}    
		else if( _tcscmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("disabledtextcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisabledTextColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("textpadding")) == 0 ) {
			RECT rcTextPadding = { 0 };
			LPTSTR pstr = NULL;
			rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			SetTextPadding(rcTextPadding);
		}
		else if( _tcscmp(pstrName, _T("showhtml")) == 0 ) SetShowHtml(_tcscmp(pstrValue, _T("TRUE")) == 0);

		else if( _tcscmp(pstrName, _T("enabledeffect")) == 0 ) SetEnabledEffect(_tcscmp(pstrValue, _T("TRUE")) == 0);
		else if( _tcscmp(pstrName, _T("rhaa")) == 0 ) SetTextRenderingHintAntiAlias(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("transshadow")) == 0 ) SetTransShadow(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("transtext")) == 0 ) SetTransText(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("transshadow1")) == 0 ) SetTransShadow1(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("transtext1")) == 0 ) SetTransText1(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("gradientangle")) == 0 ) SetGradientAngle(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("enabledstroke")) == 0 ) SetEnabledStroke(_tcscmp(pstrValue, _T("TRUE")) == 0);
		else if( _tcscmp(pstrName, _T("enabledshadow")) == 0 ) SetEnabledShadow(_tcscmp(pstrValue, _T("TRUE")) == 0);
		else if( _tcscmp(pstrName, _T("transstroke")) == 0 ) SetTransStroke(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("gradientlength")) == 0 ) SetGradientLength(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("shadowoffset")) == 0 ){
			LPTSTR pstr = NULL;
			int offsetx = _tcstol(pstrValue, &pstr, 10);	ASSERT(pstr);    
			int offsety = _tcstol(pstr + 1, &pstr, 10);		ASSERT(pstr);
			SetShadowOffset(offsetx,offsety);
		}
		else if( _tcscmp(pstrName, _T("textcolor1")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextColor1(clrColor);
		}
		else if( _tcscmp(pstrName, _T("textshadowcolora")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextShadowColorA(clrColor);
		}
		else if( _tcscmp(pstrName, _T("textshadowcolorb")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextShadowColorB(clrColor);
		}
		else if( _tcscmp(pstrName, _T("strokecolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetStrokeColor(clrColor);
		}
		else CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CLabelUI::PaintText(HDC hDC)
	{
		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;

		if(!GetEnabledEffect())
		{
			if( m_sText.IsEmpty() ) return;
			int nLinks = 0;
			if( IsEnabled() ) {
				if( m_bShowHtml )
					CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
					NULL, NULL, nLinks, m_uTextStyle);
				else
					CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
					m_iFont, DT_SINGLELINE | m_uTextStyle);
			}
			else {
				if( m_bShowHtml )
					CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
					NULL, NULL, nLinks, m_uTextStyle);
				else
					CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
					m_iFont, DT_SINGLELINE | m_uTextStyle);
			}
		}
		else
		{
			Font	nFont(hDC,m_pManager->GetFont(GetFont()));

			Graphics nGraphics(hDC);
			nGraphics.SetTextRenderingHint(m_TextRenderingHintAntiAlias);

			StringFormat format;
			format.SetAlignment((StringAlignment)m_hAlign);
			format.SetLineAlignment((StringAlignment)m_vAlign);

			RectF nRc((float)rc.left,(float)rc.top,(float)rc.right-rc.left,(float)rc.bottom-rc.top);
			RectF nShadowRc = nRc;
			nShadowRc.X += m_ShadowOffset.X;
			nShadowRc.Y += m_ShadowOffset.Y;

			int nGradientLength	= GetGradientLength();

			if(nGradientLength == 0)
				nGradientLength = (rc.bottom-rc.top);

			LinearGradientBrush nLineGrBrushA(Point(GetGradientAngle(), 0),Point(0,nGradientLength),_MakeRGB(GetTransShadow(),GetTextShadowColorA()),_MakeRGB(GetTransShadow1(),GetTextShadowColorB() == -1?GetTextShadowColorA():GetTextShadowColorB()));
			LinearGradientBrush nLineGrBrushB(Point(GetGradientAngle(), 0),Point(0,nGradientLength),_MakeRGB(GetTransText(),GetTextColor()),_MakeRGB(GetTransText1(),GetTextColor1() == -1?GetTextColor():GetTextColor1()));

			if(GetEnabledStroke() && GetStrokeColor() > 0)
			{
				LinearGradientBrush nLineGrBrushStroke(Point(GetGradientAngle(),0),Point(0,rc.bottom-rc.top+2),_MakeRGB(GetTransStroke(),GetStrokeColor()),_MakeRGB(GetTransStroke(),GetStrokeColor()));
				
#ifdef _UNICODE
				nRc.Offset(-1,0);
				nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nRc,&format,&nLineGrBrushStroke);
				nRc.Offset(2,0);
				nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nRc,&format,&nLineGrBrushStroke);
				nRc.Offset(-1,-1);
				nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nRc,&format,&nLineGrBrushStroke);
				nRc.Offset(0,2);
				nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nRc,&format,&nLineGrBrushStroke);
				nRc.Offset(0,-1);
#else
				USES_CONVERSION;
				wstring mTextValue = A2W(m_TextValue.GetString());

				nRc.Offset(-1,0);
				nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nRc,&format,&nLineGrBrushStroke);
				nRc.Offset(2,0);
				nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nRc,&format,&nLineGrBrushStroke);
				nRc.Offset(-1,-1);
				nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nRc,&format,&nLineGrBrushStroke);
				nRc.Offset(0,2);
				nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nRc,&format,&nLineGrBrushStroke);
				nRc.Offset(0,-1);
#endif
				
			}
#ifdef _UNICODE
			if(GetEnabledShadow() && (GetTextShadowColorA() > 0 || GetTextShadowColorB() > 0))
				nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nShadowRc,&format,&nLineGrBrushA);

			nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nRc,&format,&nLineGrBrushB);
#else
			USES_CONVERSION;
			wstring mTextValue = A2W(m_TextValue.GetString());

			if(GetEnabledShadow() && (GetTextShadowColorA() > 0 || GetTextShadowColorB() > 0))
				nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nShadowRc,&format,&nLineGrBrushA);

			nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nRc,&format,&nLineGrBrushB);
#endif
			
		}
	}

	//************************************
	// Method:    SetTransShadow
	// FullName:  CLabelUI::SetTransShadow
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int _TransShadow
	// Note:	  
	//************************************
	void CLabelUI::SetTransShadow( int _TransShadow )
	{
		try
		{
			m_TransShadow = _TransShadow;
		}
		catch (...)
		{
			throw "CLabelUI::SetTransShadow";
		}
	}

	//************************************
	// Method:    GetTransShadow
	// FullName:  CLabelUI::GetTransShadow
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Note:	  
	//************************************
	int CLabelUI::GetTransShadow()
	{
		try
		{
			return m_TransShadow;
		}
		catch (...)
		{
			throw "CLabelUI::GetTransShadow";
		}
	}

	//************************************
	// Method:    SetTextRenderingHintAntiAlias
	// FullName:  CLabelUI::SetTextRenderingHintAntiAlias
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int _TextRenderingHintAntiAlias
	// Note:	  
	//************************************
	void CLabelUI::SetTextRenderingHintAntiAlias( int _TextRenderingHintAntiAlias )
	{
		try
		{
			if(_TextRenderingHintAntiAlias < 0 || _TextRenderingHintAntiAlias > 5)
				_TextRenderingHintAntiAlias = 0;
			m_TextRenderingHintAntiAlias = (TextRenderingHint)_TextRenderingHintAntiAlias;
		}
		catch (...)
		{
			throw "CLabelUI::SetTextRenderingHintAntiAlias";
		}
	}

	//************************************
	// Method:    GetTextRenderingHintAntiAlias
	// FullName:  CLabelUI::GetTextRenderingHintAntiAlias
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Note:	  
	//************************************
	int CLabelUI::GetTextRenderingHintAntiAlias()
	{
		try
		{
			return m_TextRenderingHintAntiAlias;
		}
		catch (...)
		{
			throw "CLabelUI::GetTextRenderingHintAntiAlias";
		}
	}

	//************************************
	// Method:    SetShadowOffset
	// FullName:  CLabelUI::SetShadowOffset
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int _offset
	// Parameter: int _angle
	// Note:	  
	//************************************
	void CLabelUI::SetShadowOffset( int _offset,int _angle )
	{
		try
		{
			if(_angle > 180 || _angle < -180)
				return;

			RECT rc = m_rcItem;

			if(_angle >= 0 && _angle <= 180)
				rc.top -= _offset;
			else if(_angle > -180 && _angle < 0)
				rc.top += _offset;

			if(_angle > -90 && _angle <= 90)
				rc.left -= _offset;
			else if( _angle > 90 || _angle < -90)
				rc.left += _offset;

			m_ShadowOffset.X = (float)rc.top;
			m_ShadowOffset.Y = (float)rc.left;
		}
		catch (...)
		{
			throw "CLabelUI::SetShadowOffset";
		}
	}

	//************************************
	// Method:    GetShadowOffset
	// FullName:  CLabelUI::GetShadowOffset
	// Access:    public 
	// Returns:   RectF
	// Qualifier:
	// Note:	  
	//************************************
	RectF CLabelUI::GetShadowOffset()
	{
		try
		{
			return m_ShadowOffset;
		}
		catch (...)
		{
			throw "CLabelUI::GetShadowOffset";
		}
	}

	//************************************
	// Method:    SetText
	// FullName:  CLabelUI::SetText
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: LPCTSTR pstrText
	// Note:	  
	//************************************
	void CLabelUI::SetText( LPCTSTR pstrText )
	{
		try
		{
			if(!GetEnabledEffect())
				return CControlUI::SetText(pstrText);

			m_TextValue = pstrText;
		}
		catch (...)
		{
			throw "CLabelUI::SetText";
		}
	}

	//************************************
	// Method:    GetText
	// FullName:  CLabelUI::GetText
	// Access:    public 
	// Returns:   UiLib::CDuiString
	// Qualifier: const
	// Note:	  
	//************************************
	CDuiString CLabelUI::GetText() const
	{
		try
		{
			if(!m_EnableEffect)
				return CControlUI::GetText();
			return m_TextValue;
		}
		catch (...)
		{
			throw "CLabelUI::GetText";
		}
	}

	//************************************
	// Method:    SetEnabledEffect
	// FullName:  CLabelUI::SetEnabledEffect
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: BOOL _EnabledEffect
	// Note:	  
	//************************************
	void CLabelUI::SetEnabledEffect( BOOL _EnabledEffect )
	{
		try
		{
			m_EnableEffect = _EnabledEffect;
		}
		catch (...)
		{
			throw "CLabelUI::SetEnabledEffect";
		}
	}

	//************************************
	// Method:    GetEnabledEffect
	// FullName:  CLabelUI::GetEnabledEffect
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Note:	  
	//************************************
	BOOL CLabelUI::GetEnabledEffect()
	{
		try
		{
			return m_EnableEffect;
		}
		catch (...)
		{
			throw "CLabelUI::GetEnabledEffect";
		}
	}

	//************************************
	// Method:    SetTextColor1
	// FullName:  CLabelUI::SetTextColor1
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: DWORD _TextColor1
	// Note:	  
	//************************************
	void CLabelUI::SetTextColor1( DWORD _TextColor1 )
	{
		try
		{
			m_dwTextColor1	= _TextColor1;
		}
		catch (...)
		{
			throw "CLabelUI::SetTextColor1";
		}
	}

	//************************************
	// Method:    GetTextColor1
	// FullName:  CLabelUI::GetTextColor1
	// Access:    public 
	// Returns:   DWORD
	// Qualifier:
	// Note:	  
	//************************************
	DWORD CLabelUI::GetTextColor1()
	{
		try
		{
			return m_dwTextColor1;
		}
		catch (...)
		{
			throw "CLabelUI::GetTextColor1";
		}
	}

	//************************************
	// Method:    SetTextShadowColorA
	// FullName:  CLabelUI::SetTextShadowColorA
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: DWORD _TextShadowColorA
	// Note:	  
	//************************************
	void CLabelUI::SetTextShadowColorA( DWORD _TextShadowColorA )
	{
		try
		{
			m_dwTextShadowColorA	= _TextShadowColorA;
		}
		catch (...)
		{
			throw "CLabelUI::SetTextShadowColorA";
		}
	}

	//************************************
	// Method:    GetTextShadowColorA
	// FullName:  CLabelUI::GetTextShadowColorA
	// Access:    public 
	// Returns:   DWORD
	// Qualifier:
	// Note:	  
	//************************************
	DWORD CLabelUI::GetTextShadowColorA()
	{
		try
		{
			return m_dwTextShadowColorA;
		}
		catch (...)
		{
			throw "CLabelUI::GetTextShadowColorA";
		}
	}

	//************************************
	// Method:    SetTextShadowColorB
	// FullName:  CLabelUI::SetTextShadowColorB
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: DWORD _TextShadowColorB
	// Note:	  
	//************************************
	void CLabelUI::SetTextShadowColorB( DWORD _TextShadowColorB )
	{
		try
		{
			m_dwTextShadowColorB	= _TextShadowColorB;
		}
		catch (...)
		{
			throw "CLabelUI::SetTextShadowColorB";
		}
	}

	//************************************
	// Method:    GetTextShadowColorB
	// FullName:  CLabelUI::GetTextShadowColorB
	// Access:    public 
	// Returns:   DWORD
	// Qualifier:
	// Note:	  
	//************************************
	DWORD CLabelUI::GetTextShadowColorB()
	{
		try
		{
			return m_dwTextShadowColorB;
		}
		catch (...)
		{
			throw "CLabelUI::GetTextShadowColorB";
		}
	}

	//************************************
	// Method:    SetTransText
	// FullName:  CLabelUI::SetTransText
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int _TransText
	// Note:	  
	//************************************
	void CLabelUI::SetTransText( int _TransText )
	{
		try
		{
			m_TransText = _TransText;
		}
		catch (...)
		{
			throw "CLabelUI::SetTransText";
		}
	}

	//************************************
	// Method:    GetTransText
	// FullName:  CLabelUI::GetTransText
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Note:	  
	//************************************
	int CLabelUI::GetTransText()
	{
		try
		{
			return m_TransText;
		}
		catch (...)
		{
			throw "CLabelUI::GetTransText";
		}
	}

	//************************************
	// Method:    SetTransShadow1
	// FullName:  CLabelUI::SetTransShadow1
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int _TransShadow
	// Note:	  
	//************************************
	void CLabelUI::SetTransShadow1( int _TransShadow )
	{
		try
		{
			m_TransShadow1	= _TransShadow;
		}
		catch (...)
		{
			throw "CLabelUI::SetTransShadow1";
		}
	}

	//************************************
	// Method:    GetTransShadow1
	// FullName:  CLabelUI::GetTransShadow1
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Note:	  
	//************************************
	int CLabelUI::GetTransShadow1()
	{
		try
		{
			return m_TransShadow1;
		}
		catch (...)
		{
			throw "CLabelUI::GetTransShadow1";
		}
	}

	//************************************
	// Method:    SetTransText1
	// FullName:  CLabelUI::SetTransText1
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int _TransText
	// Note:	  
	//************************************
	void CLabelUI::SetTransText1( int _TransText )
	{
		try
		{
			m_TransText1	= _TransText;
		}
		catch (...)
		{
			throw "CLabelUI::SetTransText1";
		}
	}

	//************************************
	// Method:    GetTransText1
	// FullName:  CLabelUI::GetTransText1
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Note:	  
	//************************************
	int CLabelUI::GetTransText1()
	{
		try
		{
			return m_TransText1;
		}
		catch (...)
		{
			throw "CLabelUI::GetTransText1";
		}
	}

	//************************************
	// Method:    SetGradientAngle
	// FullName:  CLabelUI::SetGradientAngle
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: float _SetGradientAngle
	// Note:	  
	//************************************
	void CLabelUI::SetGradientAngle( int _SetGradientAngle )
	{
		try
		{
			m_GradientAngle	= _SetGradientAngle;
		}
		catch (...)
		{
			throw "CLabelUI::SetGradientAngle";
		}
	}

	//************************************
	// Method:    GetGradientAngle
	// FullName:  CLabelUI::GetGradientAngle
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Note:	  
	//************************************
	int CLabelUI::GetGradientAngle()
	{
		try
		{
			return m_GradientAngle;
		}
		catch (...)
		{
			throw "CLabelUI::GetGradientAngle";
		}
	}

	//************************************
	// Method:    SetEnabledStroke
	// FullName:  CLabelUI::SetEnabledStroke
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: BOOL _EnabledStroke
	// Note:	  
	//************************************
	void CLabelUI::SetEnabledStroke( BOOL _EnabledStroke )
	{
		try
		{
			m_EnabledStroke = _EnabledStroke;
		}
		catch (...)
		{
			throw "CLabelUI::SetEnabledStroke";
		}
	}

	//************************************
	// Method:    GetEnabledStroke
	// FullName:  CLabelUI::GetEnabledStroke
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Note:	  
	//************************************
	BOOL CLabelUI::GetEnabledStroke()
	{
		try
		{
			return m_EnabledStroke;
		}
		catch (...)
		{
			throw "CLabelUI::GetEnabledStroke";
		}
	}

	//************************************
	// Method:    SetTransStroke
	// FullName:  CLabelUI::SetTransStroke
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int _TransStroke
	// Note:	  
	//************************************
	void CLabelUI::SetTransStroke( int _TransStroke )
	{
		try
		{
			m_TransStroke = _TransStroke;
		}
		catch (...)
		{
			throw "CLabelUI::SetTransStroke";
		}
	}

	//************************************
	// Method:    GetTransStroke
	// FullName:  CLabelUI::GetTransStroke
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Note:	  
	//************************************
	int CLabelUI::GetTransStroke()
	{
		try
		{
			return m_TransStroke;
		}
		catch (...)
		{
			throw "CLabelUI::GetTransStroke";
		}
	}

	//************************************
	// Method:    SetStrokeColor
	// FullName:  CLabelUI::SetStrokeColor
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: DWORD _StrokeColor
	// Note:	  
	//************************************
	void CLabelUI::SetStrokeColor( DWORD _StrokeColor )
	{
		try
		{
			m_dwStrokeColor = _StrokeColor;
		}
		catch (...)
		{
			throw "CLabelUI::SetStrokeColor";
		}
	}

	//************************************
	// Method:    GetStrokeColor
	// FullName:  CLabelUI::GetStrokeColor
	// Access:    public 
	// Returns:   DWORD
	// Qualifier:
	// Note:	  
	//************************************
	DWORD CLabelUI::GetStrokeColor()
	{
		try
		{
			return m_dwStrokeColor;
		}
		catch (...)
		{
			throw "CLabelUI::GetStrokeColor";
		}
	}

	//************************************
	// Method:    SetEnabledShadow
	// FullName:  CLabelUI::SetEnabledShadow
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: BOOL _EnabledShadowe
	// Note:	  
	//************************************
	void CLabelUI::SetEnabledShadow( BOOL _EnabledShadowe )
	{
		try
		{
			m_EnabledShadow = _EnabledShadowe;
		}
		catch (...)
		{
			throw "CLabelUI::SetEnabledShadow";
		}
	}

	//************************************
	// Method:    GetEnabledShadow
	// FullName:  CLabelUI::GetEnabledShadow
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Note:	  
	//************************************
	BOOL CLabelUI::GetEnabledShadow()
	{
		try
		{
			return m_EnabledShadow;
		}
		catch (...)
		{
			throw "CLabelUI::GetEnabledShadow";
		}
	}

	//************************************
	// Method:    SetGradientLength
	// FullName:  CLabelUI::SetGradientLength
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int _GradientLength
	// Note:	  
	//************************************
	void CLabelUI::SetGradientLength( int _GradientLength )
	{
		try
		{
			m_GradientLength	= _GradientLength;
		}
		catch (...)
		{
			throw "CLabelUI::SetGradientLength";
		}
	}

	//************************************
	// Method:    GetGradientLength
	// FullName:  CLabelUI::GetGradientLength
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Note:	  
	//************************************
	int CLabelUI::GetGradientLength()
	{
		try
		{
			return m_GradientLength;
		}
		catch (...)
		{
			throw "CLabelUI::GetGradientLength";
		}
	}
	void CLabelUI::SetLabelClick(BOOL bClick)
	{
		m_bCanCilck = bClick;
	}
}