=============================================================================
	MOXA C101 Sync Board PPP Device Driver Ver. 1.0 Installation Guide
			    for Linux Kernel 2.2.x
	       Copyright (C) 2000-2000, Moxa Technologies Co, Ltd.
=============================================================================
Contents

1. Introduction
2. System Requirements
3. Module Driver Installation
   3.1 Hardware Installation
   3.2 Driver Files --- Copy and Compile
   3.3 Module Driver Configuration
       3.3.1 Parameter Description
       3.3.2 Command Description
       3.3.3 Boot Time Configuration
       3.3.4 Example Installation
4. Module Driver Removal
5. Troubleshooting

-----------------------------------------------------------------------------

1. Introduction

   This driver was developed to work with Linux Kernel 2.2.x running on
   either an x86 or Alpha hardware platform. If you encounter any problems,
   please contact Moxa by e-mail at support@moxa.com.tw.

   For this driver version, the source code is published under GNU General
   Public License. Refer to the GNU General Public License announcement in
   each source code file for more details.

   In the following explanations, Linux commands are given on a separate
   line, with descriptive comments inside square brackets, either to the
   right of, or directly below the command. For example:

    # linux command		    [descriptive comments]

-----------------------------------------------------------------------------

2. System Requirements

   - Hardware platform: Intel x86 or Alpha machine
   - Linux kernel version: 2.2.x
   - gcc version 2.72 or later
   - glib version
   - Maximum of 4 boards can be installed in combination

-----------------------------------------------------------------------------

3. Module Driver Installation

   3.1 Hardware installation

       You will need to set each board's base memory
       address manually by configuring a jumper and a switch
       on the Moxa C101 Sync Board. The hardware installation
       procedure given in the User's Manual contains specific
       instructions. If you have not already configured the
       hardware, refer to the User Manual before proceeding further.

   3.2 Driver files --- Copy and Compile

       This section describes how to copy the appropriate driver
       files from a diskette to a directory on the hard drive,
       and then compile the driver's Makefile.

       NOTE ---> If you do not have the raw driver files on diskette,
		 you first need to download the files from one of
		 Moxa's ftp sites:

	ftp://ftp.moxa.com     or     ftp://ftp.moxa.com.tw

       These instructions assume that the files will be copied
       from the diskette to the "/moxa" directory, although you
       may choose a different directory name if you like.

       Insert the diskette containing the driver files into the
       disk drive, and then issue the following commands:

	# cd /moxa
	# mcopy -t a:/linux/readme.txt .
	# mcopy -t a:/linux/hd64570.h .
	# mcopy -t a:/linux/mxmain.c .
	# mcopy -t a:/linux/mxsync.c .
	# mcopy -t a:/linux/makefile .
	# mv makefile Makefile
	# make

   3.3 Module driver configuration

       The easiest way to install the driver is as a Module driver.

       You must first issue one command to copy two files to the
       proper system directories.

	# make install		      [Copies driver files
				       "mxsync.o" and "mxmain.o"
				       to the proper system
				       directories.]

       In 3.3.1 below, we describe the parameters which must be
       initialized, and in 3.3.2 we describe the commands which
       must be executed to setup the driver. In 3.3.3 we explain
       how to modify the file /etc/rc.d/rc.local to setup a boot time
       configuration. In this way, the driver will be automatically
       installed every time the system boots up.

       3.3.1 Parameter Description

	     In order to use Moxa C101 boards with your system,
	     certain parameters must be initialized while the
	     module driver is being loaded. The parameters "baseaddr",
	     "txclock", "speed", and "irq" are initialized with
	     the command "insmod mxmain". The correct format is
	     as follows:

	      # insmod mxmain baseaddr=0x?,0x?,0x?,0x? txclock=0,0,0,0 speed=?,?,?,? irq=?,?,?,?

	     Note that between one and four boards may be installed
	     at the same time. Simply remove the appropriate number
	     of parameter values in the above command if fewer than
	     four boards are installed. We also point out that:

	      txclock=1 means Tx clock out, or use internal clock,
	      txclock=0 means Tx clock in,  or use external clock,
	      speed	can equal up to 4000000 bps, and
	      irq	cannot be shared.

       3.3.2. Command Description

	       # insmod mxsync		     [activates the physical
					      level module driver for
					      the Moxa C101 board]

	       # insmod syncppp 	     [activates the PPP protocol
					      module driver]

	       # insmod mxmain baseaddr=0x?,0x?,0x?,0x? txclock=0,0,0,0 speed=?,?,?,? irq=?,?,?,?
					     [activates the Moxa PPP
					      module driver; remove the
					      appropriate number of
					      parameter values if fewer
					      than four drivers are
					      intalled]

	       # ifconfig mxppp0 192.168.3.10 dstaddr 192.168.3.11
					     [sets the interface
					      IP address for one
					      C101 board; add
					      additional lines if
					      two, three, or four
					      drivers are installed]

       3.3.3. Boot Time Configuration

	      We can use an rc file to automatically install the
	      driver when the system boots up. This involves editing
	      a file in the /etc/rc.d directory.

	      (Note that we are using S.u.S.E Linux, which has a special rc
	      file naming convention. If you are not running S.u.S.E Linux,
	      use /etc/boot.local instead of /etc/rc.d/rc.local, and use
	      /etc/serial instead of /etc/rc.d/rc.serial.)

	      To activate the vi editor, type:

	       # vi /etc/rc.d/rc.local

	      Now append the following lines at the end of the file:

	       insmod mxsync
	       insmod syncppp
	       insmod mxmain baseaddr=0x?,0x?,0x?,0x? txclock=0,0,0,0 speed=?,?,?,? irq=?,?,?,?
	       ifconfig mxppp0 192.168.3.10 dstaddr 192.168.3.11

	      Note ---> Line three above activates the Moxa PPP module driver;
			remove the appropriate number of parameter values if fewer
			than four drivers are intalled.
	      Note ---> Line four above sets the interface IP address for one
			C101 board; add additional lines if two, three, or four
			drivers are to be installed.

       3.3.4. Example

	      There are 2 boards installed on your system with the following
	      properties.

	      board 1: memory is bank c8000, irq=10, tx clock is out, speed=1540000 bps
	      board 2: memory is bank cc000, irq=11, tx clock is out, speed=1540000 bps

	      The installation procedure is as follows (or modify the file
	      /etc/rc.d/rc.local as described in 3.3.3 above).

	       # insmod mxsync
	       # insmod syncppp
	       # insmod mxmain baseaddr=0xc8000,0xcc000 txclock=1,1 irq=10,11 speed=1540000,1540000
	       # ifconfig mxppp0 192.168.3.10 dstaddr 192.168.3.11
	       # ifconfig mxppp1 192.168.4.10 dstaddr 192.168.4.11

   3.4 Verify Driver Installation

       Reboot your computer and issue the command

	# lsmod

       to see if mxmain.o syncppp.o and mxsync.o have been activated.

       You may refer to /var/log/messages to check the latest status log
       created when the driver was activated.

-----------------------------------------------------------------------------

4. Module Driver Removal

   If the interface program is installed, you must first execute
   one command for each board, e.g.,
    
    # ifconfig mxppp0 down          [for first board]
    # ifconfig mxppp1 down          [for second board, etc.]

   And then execute the following three commands to remove the driver.

    # rmmod mxmain
    # rmmod syncppp
    # rmmod mxsync

-----------------------------------------------------------------------------

5. Troubleshooting

   Boot time error messages and possible solutions are given below.

   If you encounter an error, please try all suggested solutions
   before contacting our technical support team to get more help.


   Error msg:	 mxmain: No Moxa board is set up.

   Solution :	 Install at least one Moxa board.


   Error msg:	 mxmain: Moxa C101 board 1 IRQ 10 already in use.

   Solution :	 The specified IRQ is in use by another device.
		 Please specify a different IRQ number.


   Error msg:	 mxmain: Moxa C101 board 1 not found.

   Solution :	 The memory bank settings may not match between
		 the software and hardware settings.


   Error msg:	 mxmain: mxppp0 unable to register device.

   Solution :	 The interface name conflicts with another
		 driver name on this system. Check to make
		 sure the driver interface names are unique.
