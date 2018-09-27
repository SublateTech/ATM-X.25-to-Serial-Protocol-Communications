/*****************************************************************************/
/* Filename:	C101.H							     */
/*****************************************************************************/
/* System:	MOXA C101 Driver					     */
/* Module Name: C101 API drivers					     */
/* Description: Header file for MOXA C101				     */
/*****************************************************************************/
/* Author:	JE							     */
/* Version:	1.00							     */
/* Date:	02-04-1995						     */
/*****************************************************************************/
/*		   (C) Copyright 1995, MOXA Technologies Inc.		     */
/*****************************************************************************/

typedef unsigned char	uchar;
typedef unsigned int	uint;
typedef unsigned short	ushort;
typedef unsigned long	ulong;

int		sio_init(void);
int		sio_reset(void);
void		sio_release(void);
int		sio_flush(int mode);
ulong		sio_isoverrun(void);
ulong		sio_isunderrun(void);
ulong		sio_isbadframe(void);
int		sio_iframe(void);
int		sio_oframe(void);
int		sio_iframefree(void);
int		sio_oframefree(void);
int		sio_read(char far *buf, int len);
int		sio_write(char far *buf, int len);
int		sio_lstatus(void);
void		sio_lctrl(int mode);

/* lstatus return value define */
#define 	D_DSR	1
#define 	D_CTS	2
#define 	D_DCD	4

/* lctrl define  */
#define 	D_DTR	1
#define 	D_RTS	2

/*
 *	Error define for C101 API
 */
#define ERR_CNF_FILE	-1	/* 'config.dat' not found */
#define ERR_CNF_KEYWORD -2	/* error keyword */
#define ERR_CNF_ARG	-3	/* error (no) argment */
#define ERR_CNF_ADDR	-4	/* error base addr setting */
#define ERR_CNF_IRQ	-5	/* error irq setting */
#define ERR_CNF_SPEED	-6	/* error speed setting */
#define ERR_CNF_PKTSIZE -7	/* error packet size setting */
#define ERR_CNF_PKTCNT	-8	/* error packet count setting */
#define ERR_CNF_RXCS	-9	/* error receive clock source setting */
#define ERR_CNF_TXCS	-10	/* error transmit clock source setting */
#define ERR_CNF_LOOPM	-11	/* error look back mode setting */
#define ERR_INIT_MEM	-12	/* error when allocate memory */
#define ERR_INIT_CARD	-13	/* error card not found */
