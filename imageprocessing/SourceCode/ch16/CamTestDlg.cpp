// CamTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CamTest.h"
#include "CamTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BITMAPINFO BmInfo;
LPBYTE pImageBuffer;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamTestDlg dialog

CCamTestDlg::CCamTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCamTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	pImageBuffer=NULL;
}

void CCamTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCamTestDlg, CDialog)
	//{{AFX_MSG_MAP(CCamTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamTestDlg message handlers

BOOL CCamTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	// Capture window 생성
	m_hWndCap = capCreateCaptureWindow("Capture Window", WS_CHILD | WS_VISIBLE, 0, 0, 640, 480, this->m_hWnd, NULL);

	// 매 frame이 캡쳐될때마다 호출될 callback function 지정
	if (capSetCallbackOnFrame(m_hWndCap, capCallbackOnFrame) == FALSE) return FALSE;

	// Camera Driver와 Capture Window 연결
	if (capDriverConnect(m_hWndCap, 0) == FALSE) return FALSE;

	//////////////////////////////////////////////////////////////////////////////
	// 현재 Video Format을 조사해서 24bits color가 아니면 24bits color로 바꿈 
	capGetVideoFormat(m_hWndCap, &BmInfo, sizeof(BITMAPINFO));

	if (BmInfo.bmiHeader.biBitCount != 24)
	{
		BmInfo.bmiHeader.biBitCount		= 24;
		BmInfo.bmiHeader.biCompression	= 0;
		BmInfo.bmiHeader.biSizeImage		= BmInfo.bmiHeader.biWidth * BmInfo.bmiHeader.biHeight * 3;
		capSetVideoFormat(m_hWndCap, &BmInfo, sizeof(BITMAPINFO));
	} 
	////

	capPreviewRate(m_hWndCap, 66);  
	int a=capOverlay(m_hWndCap, true);
	capPreview(m_hWndCap, true);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCamTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCamTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCamTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


LRESULT CALLBACK capCallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	BYTE	pixel;
	int		i,j,index,counter=0;

	if(pImageBuffer==NULL) 
		pImageBuffer= (LPBYTE)new BYTE[BmInfo.bmiHeader.biHeight*BmInfo.bmiHeader.biWidth];

	for (i = 0; i <BmInfo.bmiHeader.biWidth*BmInfo.bmiHeader.biHeight; i++)
	{
			pixel = (*(lpVHdr->lpData + (i*3)    ) +
				     *(lpVHdr->lpData + (i*3) + 1) +
					 *(lpVHdr->lpData + (i*3) + 2) ) / 3;

			if(pixel > 200) *(pImageBuffer+i) = 255;
			else *(pImageBuffer+i) = 0;
	}

	int xCenter=0, yCenter=0;
	for(i=0; i<BmInfo.bmiHeader.biHeight; i++)
	{
		index = i*BmInfo.bmiHeader.biWidth;
		for(j=0; j<BmInfo.bmiHeader.biWidth; j++)
		{
			if(	*(pImageBuffer+index+j) == 255)
			{
				xCenter += i;
				yCenter += j;
				counter++;
			}
	}}
	xCenter =(int)((float)xCenter/(float)counter);
	yCenter =(int)((float)yCenter/(float)counter);

	for(i=xCenter-15; i<=xCenter+15; i++)
	{
		if(i<0 || i>=BmInfo.bmiHeader.biHeight) continue;
		index = i*BmInfo.bmiHeader.biWidth;
		*(lpVHdr->lpData + 3*(index+yCenter)) =0;
		*(lpVHdr->lpData + 3*(index+yCenter)+1)   =0;
		*(lpVHdr->lpData + 3*(index+yCenter)+2)   =255;
	}

	index = xCenter*BmInfo.bmiHeader.biWidth;
	for(j=yCenter-15; j<=yCenter+15; j++)
	{
		if(j<0 || j>=BmInfo.bmiHeader.biWidth) continue;
		*(lpVHdr->lpData + 3*(index+j)) =0;
		*(lpVHdr->lpData + 3*(index+j)+1)  =0;
		*(lpVHdr->lpData + 3*(index+j)+2)  =255;
	}

	// 차이가 나는 화소의 수를 caption bar에 표시
	CString		strTitle;
	strTitle.Format("Binary Tracker (%d,%d)", xCenter, yCenter);
	AfxGetMainWnd()->SetWindowText(strTitle);

	return (LRESULT)TRUE;
}


void CCamTestDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	capDriverDisconnect(m_hWndCap);
	if (pImageBuffer != NULL) delete []pImageBuffer;

}

