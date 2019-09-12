; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWinTestView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WinTest.h"
LastPage=0

ClassCount=7
Class1=CWinTestApp
Class2=CWinTestDoc
Class3=CWinTestView
Class4=CMainFrame

ResourceCount=5
Resource1=IDD_DIALOG1
Resource2=IDD_ABOUTBOX
Class5=CChildFrame
Class6=CAboutDlg
Resource3=IDR_WINTESTYPE
Class7=CBinCntrlDlg
Resource4=IDR_MAINFRAME
Resource5=IDD_RESULTSHOWBAR

[CLS:CWinTestApp]
Type=0
HeaderFile=WinTest.h
ImplementationFile=WinTest.cpp
Filter=N

[CLS:CWinTestDoc]
Type=0
HeaderFile=WinTestDoc.h
ImplementationFile=WinTestDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CWinTestDoc

[CLS:CWinTestView]
Type=0
HeaderFile=WinTestView.h
ImplementationFile=WinTestView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=IDM_OUT2IN


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=WinTest.cpp
ImplementationFile=WinTest.cpp
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
Command9=IDM_CONST_SUB
CommandCount=9

[MNU:IDR_WINTESTYPE]
Type=1
Class=CWinTestView
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
Command15=IDM_OUT2IN
Command16=ID_VIEW_TOOLBAR
Command17=ID_VIEW_STATUS_BAR
Command18=ID_WINDOW_NEW
Command19=ID_WINDOW_CASCADE
Command20=ID_WINDOW_TILE_HORZ
Command21=ID_WINDOW_ARRANGE
Command22=ID_MENUITEM32797
Command23=IDM_REVERSE_IMG
Command24=ID_MENUITEM32798
Command25=IDM_CONST_ADD
Command26=IDM_CONST_SUB
Command27=IDM_CONST_MUL
Command28=IDM_CONST_DIV
Command29=IDM_FRM_ADD
Command30=IDM_FRM_SUB
Command31=IDM_FRM_MUL
Command32=IDM_LUT_MUL
Command33=ID_MENUITEM32799
Command34=IDM_IMGHISTO
Command35=IDM_BINARIZATION
Command36=IDM_BIN_DYNAMIC
Command37=IDM_HISTO_EQUAL
Command38=IDM_HISTO_STRETCH
Command39=IDM_HISTO_UPSTRETCH
Command40=IDM_HISTO_SPEC_FILEIN
Command41=IDM_HISTO_SPEC_CAL
Command42=ID_MENUITEM32800
Command43=IDM_BIN_LABELING
Command44=IDM_BIN_LABELING_B
Command45=IDM_BORDER_FOLLOW
Command46=ID_APP_ABOUT
CommandCount=46

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
Class=CBinCntrlDlg
ControlCount=3
Control1=IDC_SLIDER1,msctls_trackbar32,1342242840
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_EDIT1,edit,1350633602

[CLS:CBinCntrlDlg]
Type=0
HeaderFile=BinCntrlDlg.h
ImplementationFile=BinCntrlDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CBinCntrlDlg

[DLG:IDD_RESULTSHOWBAR]
Type=1
Class=?
ControlCount=1
Control1=IDC_RESULTSHOW,edit,1352732868

