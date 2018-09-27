/****************************************************************************/
/*									    */
/*  PROGRAM: Example.c							    */
/*									    */
/*  PURPOSE: Demonstrates how to use MOXA serial I/O cards under MS Windows */
/*	     Written by Microsoft C 6.0 				    */
/****************************************************************************/

#include <windows.h>
#include "ex1.h"
#include "../driver/msa.h"

void	test1(HWND hWnd);
void	test2();
void	GateWay();
void	GateWayLoop();

HANDLE hInst;

	HDC	hDC;
	NPTTYINFO  npTTYInfo ;
	DCB dcb;
    int err;
    char  szTemp[ TXQUEUE ] ;
	HFILE hfTempFile;
	char szBuf[144];

	/* Create a temporary file. */

	//GetTempFileName(0, "tst", 0, szBuf);

	

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    BOOL bMsgAvail;

    if (!hPrevInstance)
	if (!InitApplication(hInstance))
	    return (FALSE);
    if (!InitInstance(hInstance, nCmdShow))
		return (FALSE);
	
    while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage((LPMSG)&msg);
		DispatchMessage((LPMSG)&msg);
	    }
    return (msg.wParam);
    	
}

BOOL InitApplication(HANDLE hInstance)
{
    WNDCLASS wc;

    wc.style = NULL;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  "SDLC";
    wc.lpszClassName = "SDLCClass";
    return (RegisterClass(&wc));
}

BOOL InitInstance(HANDLE hInstance, int nCmdShow)
{
    HWND hWnd;
    int i;
    char tmp[100];

    hInst = hInstance;

    hWnd = CreateWindow(
	"SDLCClass",
	"Gateway from SDLC to Serial Port",
	WS_SYSMENU ,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	NULL,
	NULL,
	hInstance,
	NULL
    );
    if (!hWnd)
	return (FALSE);
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

/*    if ((i = MSAInit()) <= 0) { 				//**
	wsprintf(tmp, "MOXA INIT error %d", i);                 //**
	MessageBox(hWnd, tmp, NULL, MB_OK | MB_ICONHAND);	//**
	return(FALSE);						//**
    }								//**
    
       if ((COMDEV( npTTYInfo ) = OpenComm( "COM5", RXQUEUE, TXQUEUE )) < 0)
 	{ 
 	
	   MessageBox(hWnd,"Error:OpenComm","",MB_OK);
       return ( FALSE ) ;
     }
      
   err = BuildCommDCB("COM5:19200,n,8,1", &dcb);
   if (err < 0) {
    	MessageBox(hWnd, "Ok", "BuildCommDCB", MB_OK); 
     	return 0;
		}              
	err = SetCommState(&dcb);
	if (err < 0) {
    	MessageBox(hWnd, "Ok", "SetCommState", MB_OK); 
    	return 0;
	}
    
    strcpy(szBuf, "c:\\SDLC.TXT");
	hfTempFile = _lcreat(szBuf, 0);

	if (hfTempFile == HFILE_ERROR) {
    	                                               
		MessageBox(hWnd, "Ok", "OpenFile", MB_OK); 
		return ( FALSE);    	                                               
	}
	         
         
    SendMessage( hWnd, WM_COMMAND, (WPARAM) IDM_T1, 	NULL ) ;
*/
  
  
  
    test2(hWnd);
  
    return (TRUE);
}

long FAR PASCAL MainWndProc(HWND hWnd, unsigned message,
	WORD wParam, LONG lParam)
{
    FARPROC lpProcAbout;
    HMENU   hMenu;
    PAINTSTRUCT ps;

    switch (message) {
	
	//case WM_CREATE:
		
	//	GateWayLoop(hWnd);
	//	break;    
	
	case WM_CHAR:
		
		GateWay(wParam);
		break;
		
	
	case WM_COMMAND:
	    switch (wParam) {
	    
		//case IDM_T1:			Test1(hWnd);	break;
		
		default:		/* Lets Windows process it	 */
		    return (DefWindowProc(hWnd, message, wParam, lParam));
	    }
	    break;
	case WM_DESTROY:
	    PostQuitMessage(0);
	    MSAEnd();
	    CloseComm( COMDEV( npTTYInfo ) ) ;
	    _lclose(hfTempFile);
	    break;
	    
	default:
	    return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (NULL);
}


#define MAX_TEST_PORTS	50

int	total_port;
int	irq_count;
int	port_no[MAX_TEST_PORTS];

/***************************************************************************/
/*									   */
/*  FUNCTION: test1(HWND)						   */
/*									   */
/*  PURPOSE:  get card info						   */
/*									   */
/***************************************************************************/
void	test1(HWND hWnd)
{
     

    
	int	i, j = 1000, j1 = 1000;
	char	buf[1024], tmp[1024];
	long	total = 0;
	unsigned	long	t1, t2;

	ScrollWindow(hWnd, 0, -300, NULL, NULL);
	UpdateWindow(hWnd);
	hDC = GetDC(hWnd);
	
	strcpy(buf, "Esta es una prueba");
	//MSAWrite(buf, strlen(buf));
	
	
		t1 = GetCurrentTime();
		while ((i = MSARead(tmp, 1024)) == 0)
			if (((t2 = GetCurrentTime()) - t1) > 30000)
				break;
		
	t1 = MSAIsUnderRun(); 
	
	MessageBox(hWnd, tmp ,"ok", MB_OK); 
	ReleaseDC(hWnd, hDC);
		
	
}

void	test2(HWND hWnd)
{   	
	HDC	hDC;
    
/*	int	i, j = 1000, j1 = 1000;
	char	buf[1024], tmp[1024];
	long	total = 0;
	unsigned	long	t1, t2;

	ScrollWindow(hWnd, 0, -300, NULL, NULL);
	UpdateWindow(hWnd);
	hDC = GetDC(hWnd);
	buf[0] = j;
	MSAWrite(buf, j--);
	buf[0] = j;
	MSAWrite(buf, j--);
	while (j1 > 100) {
		t1 = GetCurrentTime();
		while ((i = MSARead(buf, 1024)) == 0)
			if (((t2 = GetCurrentTime()) - t1) > 2000)
				break;
		buf[1] = j1;
		if (i != j1 || buf[0] != buf[1])
			break;
		j1--;
		buf[0] = j;
		MSAWrite(buf, j--);
	}
	t1 = MSAIsUnderRun(); */
	
	int c, cc;
	PSTR pszBuf, pszKey;
	char szMsg[80], szVal[80];

	/* Allocate a buffer for the entries. */

	pszBuf = (PSTR) LocalAlloc(LMEM_FIXED, 1024);

	/* Retrieve all the entries in the [windows] section. */

	GetPrivateProfileString("GateWay", "Sleep", "10", szMsg, sizeof(szMsg),"C:\\windows\\c101.cnf");
	
	MessageBox(hWnd,szMsg,"",MB_OK);
	


	ReleaseDC(hWnd, hDC);
}
    
void GateWay(char wParam)
{

   memset( szTemp, 0, TXQUEUE) ;
   *szTemp = wParam;
   //strcpy(szTemp, "Hola");
   WriteComm(COMDEV( npTTYInfo ),  (LPSTR) szTemp, strlen(szTemp));
   //SendMessage( hWnd, WM_COMMAND, (WPARAM) WM_DESTROY, 	NULL ) ;
  // _lwrite(hfTempFile, (LPSTR) szTemp, strlen(szTemp) );
   //DestroyWindow(hWnd);
}

void GateWayLoop()
{

   int i;
   //SendMessage( hWnd, WM_COMMAND, (WPARAM) WM_DESTROY, 	NULL ) ;
   //DestroyWindow(hWnd);
   
   
   	//	if (MSAIFrame())       
     
     if ((i = MSARead(szTemp, TXQUEUE)) != 0)
        {
 		
 	//	memset( szTemp, 0, TXQUEUE) ;
 	//	i = MSARead(szTemp, TXQUEUE);
 		WriteComm(COMDEV( npTTYInfo ),  (LPSTR) szTemp, strlen(szTemp));
 		_lwrite(hfTempFile, (LPSTR) szTemp, strlen(szTemp) );
 		
 		//MSAFlush(0);

 		
 		}

       
}