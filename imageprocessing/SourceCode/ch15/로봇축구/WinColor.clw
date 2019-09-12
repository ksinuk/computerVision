; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWinColorView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WinColor.h"
LastPage=0

ClassCount=7
Class1=CWinColorApp
Class2=CWinColorDoc
Class3=CWinColorView
Class4=CMainFrame

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CChildFrame
Class6=CAboutDlg
Resource3=IDR_WINCOLTYPE
Class7=CBinaryCtrlDlg
Resource4=IDD_DIALOG1

[CLS:CWinColorApp]
Type=0
HeaderFile=WinColor.h
ImplementationFile=WinColor.cpp
Filter=N

[CLS:CWinColorDoc]
Type=0
HeaderFile=WinColorDoc.h
ImplementationFile=WinColorDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CWinColorDoc

[CLS:CWinColorView]
Type=0
HeaderFile=WinColorView.h
ImplementationFile=WinColorView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=IDM_BLOBCOLORING


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
BaseClass=CMDIChildWnd
VirtualFilter=mfWC
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=WinColor.cpp
ImplementationFile=WinColor.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_WINCOLTYPE]
Type=1
Class=CWinColorView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_MENUITEM32778
Command18=IDM_COLOR2GRAY
Command19=IDM_IMG_DYNBIN
Command20=IDM_CHANNELSPLIT_RGB
Command21=IDM_CHANNELSPLIT_HSI
Command22=IDM_RGB_EMBOSSING
Command23=IDM_RGB_SHAPENNING
Command24=ID_MENUITEM32779
Command25=IDM_BLOBCOLORING
Command26=ID_WINDOW_NEW
Command27=ID_WINDOW_CASCADE
Command28=ID_WINDOW_TILE_HORZ
Command29=ID_WINDOW_ARRANGE
Command30=ID_APP_ABOUT
CommandCount=30

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DIALOG1]
Type=1
Class=CBinaryCtrlDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SLIDER1,msctls_trackbar32,1342242840
Control4=IDC_EDIT1,edit,1350633602
Control5=IDC_IMG_HISTO_VIEW,static,1350696960

[CLS:CBinaryCtrlDlg]
Type=0
HeaderFile=BinaryCtrlDlg.h
ImplementationFile=BinaryCtrlDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CBinaryCtrlDlg
VirtualFilter=dWC

