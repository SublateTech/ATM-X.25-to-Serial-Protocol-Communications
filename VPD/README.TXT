


	The Virtual Serial Ports Driver (VSPD)
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	DESCRIPTION
	
	Virtual Serial Ports Driver creates two new serial ports in your 
	system which are virtually connected to each other. In other words, 
        after installing, you will have Virtual Serial Port COM5 and Virtual 
	Serial Port COM6. All data sent by other application to COM5 will 
	arrive at COM6, and all data sent by other applications to COM6 will 
	arrive at COM5. It could be used as a debugging tool or as a way to 
	create data channel between two windows applications running together.

	KEY FEATURES

	- Windows 95/98/ME compatibility
	- Full software serial port emulation
	- All serial port services are supported (baud rates, flow control, 
	  parity check, etc.) 
	- FIFO buffer limited only by size of windows memory
	- Instant data transfer


	INSTALLATION

	1) Download in unzip vport.zip archive into temporary folder.
	2) Open "Control Panel", "Add new hardware", press "Next" two times, 
	   choose "No, select from list", "Ports (COM&LPT)", select "Have a 
	   disk..." 
	   and enter location of a temporary folder where you have unpacked 
	   downloaded archive file.
	3) Select "Virtual Serial Port" and press "Next"
	4) Repeat steps (2)-(3) to have second virtual serial port installed.


	FREE DEMO VERSION LIMITATIONS
	In demo version only one direction data transfer is allowed. Commonly 
	saying, you will be able to transfer data only from COM5 to COM6 
	but will not be able to transfer from COM6 to COM5.


	BUGS AND OTHER PROBLEMS
	
	Please, report any bugs you find to email: development2000@ukr.net


	Vitaliy Golubenko (C) 2001

