/****************************************************************************/
/*																		    */
/*  PROGRAMA : BBVA.c							    						*/
/*									    									*/
/*  PROPOSITO: Puente de SDLC a Puerto Serial  								*/
/*	     	   Escrito por Alvaro Medina de PERURIMPEX S.A. 				*/
/****************************************************************************/

#include <windows.h>
#include "bbva.h"
#include "msa.h"

void GateWay(char wParam);
void Sleep(int Time);                 
                     	
	
	int y=0;

	HANDLE hInst;

	HDC	hDC;
	NPTTYINFO  npTTYInfo ;
	DCB dcb;
    int err;
    char  szTemp[ TXQUEUE ] ;
	HFILE hfTempFile;
	char szBuf[TXQUEUE];
    TIMERPROC lpfnMyTimerProc;
    int nTimer, pTimer, lTrans; 
    int lFirst;
	

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
    wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( ICONO_BBVA ) ); //LoadIcon(hInstance, ID_BBVA);
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
	WS_SYSMENU | WS_MINIMIZEBOX,
	200,
	100,
	290,
	50,
	NULL,
	NULL,
	hInstance,
	NULL
    );
    if (!hWnd)
	return (FALSE);
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    
        

    if ((i = MSAInit()) <= 0) { 				//**
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
	         
    pTimer = 200;
    lpfnMyTimerProc = (TIMERPROC) MakeProcInstance(MyTimerProc, hInstance);
	SetTimer(hWnd, ID_MYTIMER, pTimer, lpfnMyTimerProc);
    
    nTimer = 0;
    lTrans = 0;
             
    SendMessage( hWnd, WM_SYSCOMMAND, (WPARAM) SC_MINIMIZE, 	NULL ) ;
    return (TRUE);
}

long FAR PASCAL MainWndProc(HWND hWnd, unsigned message,
	WORD wParam, LONG lParam)
{
    FARPROC lpProcAbout;
    HMENU   hMenu;
    PAINTSTRUCT ps;

    switch (message) {
	
	case WM_CHAR:
		 GateWay(wParam);
		 break;
		
	case WM_COMMAND:
	    switch (wParam) {
	    
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
	   // GateWay(wParam, hWnd);
	    return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (NULL);
}




void CALLBACK MyTimerProc(HWND hWnd, UINT msg, UINT idTimer, DWORD dwTime)

{
	unsigned	long	t1;
   
   	int i,n=0, nFlg = 0; 
   	HDC	hDC;
   	
   	if (nTimer > 5000 && lTrans) 
    	{
    	nTimer = 0;
    	lTrans = 0;
    	WriteComm(COMDEV( npTTYInfo ),  (LPSTR) "FIN", 3);
    	TextOut(hDC, 10, y, (LPSTR) "fin", 3); 
    	MessageBox(hWnd, "FIN" ,"ok", MB_OK);                                           
    	
    	}
    else 
    	{
    	nTimer += pTimer;
    	sprintf(szBuf, "%d" , nTimer); 
    	TextOut(hDC, 10, y, (LPSTR) szBuf, strlen(szBuf));
    	}
    		
   
      
   	  hDC = GetDC(hWnd);
      memset( szTemp, 0, TXQUEUE) ;
      if  ((i = MSARead(szTemp, TXQUEUE)) > 0)
         {
    		if (strlen(szTemp)> 8  ) //(strcmp(Control, szTemp))
    		{
    		
    		
    			if (!nTimer )	{
    			
    					lTrans = 1;
    					nTimer = pTimer;
    					WriteComm(COMDEV( npTTYInfo ),  (LPSTR) "INICIO", 6);
    			    			
    					sprintf(szBuf, "%d" , nTimer); 
    					TextOut(hDC, 10, y, (LPSTR) szBuf, strlen(szBuf));
    				}
    		
    		        		
    				//TextOut(hDC, 10, y, (LPSTR) "Procesando....", 14);
    				for(n=0;i>n;n++)
    					{
    					if (!szTemp[n]) szTemp[n]=' ';
    		    		if (szTemp[n]== 12) { szTemp[n++]=0; nFlg = 1; break; }
    		    		}

		//    		if (nFlg) 
    	//				{
    		
    					memset( szBuf, 0, TXQUEUE) ;
    					//strcpy(szBuf, szTemp);
    					//strcpy(szBuf, "INICIO");      
    					strcat(szBuf, szTemp +  37); 
    					//strcat(szBuf,"FIN");
    					if (lTrans)
    		    			MessageBox(hWnd, szTemp +  37  ,"ok", MB_OK);                                           
    		 			else
    		 				MessageBox(hWnd, szTemp +  9   ,"ok", MB_OK);                                           
    		
    					//sprintf(szBuf, "%s --> %d ==>%d,%d,%d ---->i=%d\n", szTemp, strlen(szTemp),szTemp[0], szTemp[1], szTemp[2],i);
    					//TextOut(hDC, 10, y, (LPSTR) szBuf, strlen(szBuf));
    					WriteComm(COMDEV( npTTYInfo ),  (LPSTR) szBuf, strlen(szBuf));
 						_lwrite(hfTempFile, (LPSTR) szBuf, strlen(szBuf) );
    					//TextOut(hDC, 10, y, (LPSTR) "                            ", 28);
    					//y+=15;
    		//		}
            
    		} 
    		
    		else lTrans = 0;
    		    			
    	}    
        
    t1 = MSAIsUnderRun();  
    ReleaseDC(hWnd, hDC);
    

}

void GateWay(char wParam)
{

   memset( szTemp, 0, TXQUEUE) ;
   *szTemp = wParam;
   //strcpy(szTemp, "Hola");
   WriteComm(COMDEV( npTTYInfo ),  (LPSTR) szTemp, strlen(szTemp));
}
                    
                    
void Sleep(int Time)                 
{
DWORD n;

n= GetTickCount(); 

while ((GetTickCount() - n) > Time) ;
return;

}