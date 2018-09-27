Attribute VB_Name = "syncapi"
'********************************************************
'    syncapi.bas
'     -- Moxa C502-ISA WindowsNT API module for Visual Basic(5.0 above)
'
'    Description:
'       When you want to develop one VB application with P188Api,
'       you should add this module to your project.
'
'    History:
'           Date    Author      Comment
'       08/17/1999  Victor Yu   wrote it.
'       06/23/2000  Victor Yu   Add to support BSC. Add following function:
'                               syio_SetIdleCode(), syio_SetSyncChar()
'                               syio_SetSyncLength(), syio_GetOpMode()
'                               syio_GetIdleCode(), syio_GetSyncChar()
'                               syio_GetSyncLength()
'       08/17/2000  Victor Yu.  Add C101/ISA board type.
'       08/28/2000  Victor Yu.  Add syio_ReadWithError() function.
'
'********************************************************

'Constant define
'flush function define
Global Const FLUSH_INPUT = 1
Global Const FLUSH_OUTPUT = 2
Global Const FLUSH_ALL = 3

'DTR, RTS control define
Global Const CONTROL_ON = 1
Global Const CONTROL_OFF = 0

'data encoding define
Global Const NRZ = 0
Global Const NRZI = 1
Global Const FM0 = 2
Global Const FM1 = 3

'CRC mode define
Global Const NONE_CRC = 0
Global Const CRC16_0 = 1
Global Const CRC16_1 = 2
Global Const CCITT_0 = 3
Global Const CCITT_1 = 4

'Operate mode define
Global Const HDLC_MODE = 0
Global Const BI_SYNC_8 = 1
Global Const BI_SYNC_16 = 2

'transmit clock directory define
Global Const TX_CLOCK_OUT = 0
Global Const TX_CLOCK_IN = 1

'modem status define
Global Const DCD_ON = 1
Global Const DSR_ON = 2
Global Const CTS_ON = 4

'interrupt function define
Global Const CNTFRAME_INT = 4
Global Const DCDCHANGE_INT = 8
Global Const CTSCHANGE_INT = &H10
Global Const DSRCHANGE_INT = &H20
Global Const TXEMPTY_INT = &H40

'Rx error flag
Global Const CRC_ERROR = 1

'error code define
Global Const SYIO_OK = 0	    ' funciton ok
Global Const SYIO_BADPORT = -1	    ' no such port or not opened
Global Const SYIO_OPENED = -2	    ' the port is opened
Global Const SYIO_BADPARM = -3	    ' parameter error
Global Const SYIO_WIN32FAIL = -4    ' call win32 function fail, please call
				    ' GetLastError() funciton to get the error
				    ' code
Global Const SYIO_ABORT = -5	    ' abort read or write
Global Const SYIO_TIMEOUT = -6	    ' read or write timeout
Global Const SYIO_BUFFERTOOSHORT = -8	' the buffer is too short

' To define the funciton call
Declare Function syio_Open Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_Close Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_Write Lib "syncapi.dll" (ByVal port As Long, ByRef buf As Byte, ByVal length As Long) As Long
Declare Function syio_Read Lib "syncapi.dll" (ByVal port As Long, ByRef buf As Byte, ByVal length As Long) As Long
Declare Function syio_Flush Lib "syncapi.dll" (ByVal port As Long, ByVal mode As Long) As Long
Declare Function syio_View Lib "syncapi.dll" (ByVal port As Long, ByRef buf As Byte, ByVal length As Long) As Long
Declare Function syio_SetBaud Lib "syncapi.dll" (ByVal port As Long, ByVal speed As Long) As Long
Declare Function syio_GetBaud Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_SetReadTimeouts Lib "syncapi.dll" (ByVal port As Long, ByVal timeouts As Long) As Long
Declare Function syio_GetReadTimeouts Lib "syncapi.dll" (ByVal port As Long, ByRef timeouts As Long) As Long
Declare Function syio_SetWriteTimeouts Lib "syncapi.dll" (ByVal port As Long, ByVal timeouts As Long) As Long
Declare Function syio_GetWriteTimeouts Lib "syncapi.dll" (ByVal port As Long, ByRef timeouts As Long) As Long
Declare Function syio_AbortRead Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_AbortWrite Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_DTR Lib "syncapi.dll" (ByVal port As Long, ByVal mode As Long) As Long
Declare Function syio_RTS Lib "syncapi.dll" (ByVal port As Long, ByVal mode As Long) As Long
Declare Function syio_SkipFrame Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_InFrame Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_OutFrame Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_InFreeFrame Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_OutFreeFrame Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_SetDataEncoding Lib "syncapi.dll" (ByVal port As Long, ByVal mode As Long) As Long
Declare Function syio_GetDataEncoding Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_SetCRCMode Lib "syncapi.dll" (ByVal port As Long, ByVal mode As Long) As Long
Declare Function syio_GetCRCMode Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_LineStatus Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_InQueue Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_OutQueue Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_InFree Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_OutFree Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_FrameIrq Lib "syncapi.dll" (ByVal port As Long, ByVal func  As Long, ByVal framecnt As Long) As Long
Declare Function syio_ModemIrq Lib "syncapi.dll" (ByVal port As Long, ByVal func As Long, ByVal mode As Long) As Long
Declare Function syio_TxEmptyIrq Lib "syncapi.dll" (ByVal port As Long, ByVal func As Long) As Long
Declare Function syio_SetTxClockDir Lib "syncapi.dll" (ByVal port As Long, ByVal directory As Long) As Long
Declare Function syio_GetTxClockDir Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_TxDisable Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_TxEnable Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_TxStatus Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_GetFirstFrameLen Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_GetBoardID Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_SetIdleCode Lib "syncapi.dll" (ByVal port As Long, ByVal idlecode As Byte) As Long
Declare Function syio_SetSyncChar Lib "syncapi.dll" (ByVal port As Long, ByVal syncchar As Integer) As Long
Declare Function syio_SetSyncLength Lib "syncapi.dll" (ByVal port As Long, ByVal length As Long) As Long
Declare Function syio_GetOpMode Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_GetIdleCode Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_GetSyncChar Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_GetSyncLength Lib "syncapi.dll" (ByVal port As Long) As Long
Declare Function syio_Read Lib "syncapi.dll" (ByVal port As Long, ByRef buf As Byte, ByVal length As Long, ByRef error As Byte) As Long
