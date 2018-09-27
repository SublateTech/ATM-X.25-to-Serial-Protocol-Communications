
/*****************************************************************************/
/*									     */
/*	Moxa C101 board used Hitachi HD64570-10 USART.			     */
/*      Crystal clock is 9.8304MHz.                                                */
/*									     */
/*	History:							     */
/*	Date	    Author		Comment 			     */
/*	02-04-1995  JE			Wrote it. Version 1.00		     */
/*	04-17-2000  Victor Yu.		Modified it for Linux Driver.	     */
/*									     */
/*****************************************************************************/
/*		   (C) Copyright 1995, MOXA Technologies Inc.		     */
/*		   (C) Copyright 2000, MOXA Technologies Inc.		     */
/*****************************************************************************/

#ifndef _HD64570_H
#define _HD64570_H

/****************************************/
/*	Hitachi Register define 	*/
/****************************************/
/*	Low power control Reg.		*/
#define D_HR_LPR	0x00		/* Lower Power Reg. */
/*	Interrupt control Reg.		*/
#define D_HR_IVR	0x1A		/* Interrupt Vector Reg. */
#define D_HR_IMVR	0x1C		/* Interrupt Modified Vector Reg. */
#define D_HR_ITCR	0x18		/* Interrupt Control Reg. */
#define D_HR_ISR0	0x10		/* Interrupt Status Reg. 0 */
#define D_HR_ISR1	0x11		/* Interrupt Status Reg. 1 */
#define D_HR_ISR2	0x12		/* Interrupt Status Reg. 2 */
#define D_HR_IER0	0x14		/* Interrupt Enable Reg. 0 */
#define D_HR_IER1	0x15		/* Interrupt Enable Reg. 1 */
#define D_HR_IER2	0x16		/* Interrupt Enable Reg. 2 */

/*	Port bit for MSCI		*/
#define __HR_P0 	0x20		/* Port 0 MASK */
#define __HR_P1 	0x40		/* Port 1 MASK */

/*	MSCI Register (1)		*/
#define __HR_MD0	0x0E		/* Mode Reg. 0 */
#define __HR_MD1	0x0F		/* Mode Reg. 1 */
#define __HR_MD2	0x10		/* Mode Reg. 2 */
#define __HR_CTL	0x11		/* Control Reg. */
#define __HR_RXS	0x16		/* RX clock Source Reg. */
#define __HR_TXS	0x17		/* TX clock Source Reg. */
#define __HR_TMC	0x15		/* TiMe Constant Reg. */
#define __HR_CMD	0x0C		/* CoMmanD Reg. */
#define __HR_ST0	0x02		/* Status Reg. 0 */
#define __HR_ST1	0x03		/* Status Reg. 1 */
#define __HR_ST2	0x04		/* Status Reg. 2 */
#define __HR_ST3	0x05		/* Status Reg. 3 */
#define __HR_FST	0x06		/* Frame Status Reg. */
#define __HR_IE0	0x08		/* Interrupt Enable Reg. 0 */
#define __HR_IE1	0x09		/* Interrupt Enable Reg. 1 */
#define __HR_IE2	0x0A		/* Interrupt Enable Reg. 2 */
#define __HR_FIE	0x0B		/* Frame Interrupt Enable Reg. */
#define __HR_SA0	0x12		/* Sync/Address Reg. 0 */
#define __HR_SA1	0x13		/* Sync/Address Reg. 1 */
#define __HR_IDL	0x14		/* IDLe pattern Reg. */
#define D_HR_P0_MD0	(__HR_MD0 | __HR_P0)
#define D_HR_P0_MD1	(__HR_MD1 | __HR_P0)
#define D_HR_P0_MD2	(__HR_MD2 | __HR_P0)
#define D_HR_P0_CTL	(__HR_CTL | __HR_P0)
#define D_HR_P0_RXS	(__HR_RXS | __HR_P0)
#define D_HR_P0_TXS	(__HR_TXS | __HR_P0)
#define D_HR_P0_TMC	(__HR_TMC | __HR_P0)
#define D_HR_P0_CMD	(__HR_CMD | __HR_P0)
#define D_HR_P0_ST0	(__HR_ST0 | __HR_P0)
#define D_HR_P0_ST1	(__HR_ST1 | __HR_P0)
#define D_HR_P0_ST2	(__HR_ST2 | __HR_P0)
#define D_HR_P0_ST3	(__HR_ST3 | __HR_P0)
#define D_HR_P0_FST	(__HR_FST | __HR_P0)
#define D_HR_P0_IE0	(__HR_IE0 | __HR_P0)
#define D_HR_P0_IE1	(__HR_IE1 | __HR_P0)
#define D_HR_P0_IE2	(__HR_IE2 | __HR_P0)
#define D_HR_P0_FIE	(__HR_FIE | __HR_P0)
#define D_HR_P0_SA0	(__HR_SA0 | __HR_P0)
#define D_HR_P0_SA1	(__HR_SA1 | __HR_P0)
#define D_HR_P0_IDL	(__HR_IDL | __HR_P0)
#define D_HR_P1_MD0	(__HR_MD0 | __HR_P1)
#define D_HR_P1_MD1	(__HR_MD1 | __HR_P1)
#define D_HR_P1_MD2	(__HR_MD2 | __HR_P1)
#define D_HR_P1_CTL	(__HR_CTL | __HR_P1)
#define D_HR_P1_RXS	(__HR_RXS | __HR_P1)
#define D_HR_P1_TXS	(__HR_TXS | __HR_P1)
#define D_HR_P1_TMC	(__HR_TMC | __HR_P1)
#define D_HR_P1_CMD	(__HR_CMD | __HR_P1)
#define D_HR_P1_ST0	(__HR_ST0 | __HR_P1)
#define D_HR_P1_ST1	(__HR_ST1 | __HR_P1)
#define D_HR_P1_ST2	(__HR_ST2 | __HR_P1)
#define D_HR_P1_ST3	(__HR_ST3 | __HR_P1)
#define D_HR_P1_FST	(__HR_FST | __HR_P1)
#define D_HR_P1_IE0	(__HR_IE0 | __HR_P1)
#define D_HR_P1_IE1	(__HR_IE1 | __HR_P1)
#define D_HR_P1_IE2	(__HR_IE2 | __HR_P1)
#define D_HR_P1_FIE	(__HR_FIE | __HR_P1)
#define D_HR_P1_SA0	(__HR_SA0 | __HR_P1)
#define D_HR_P1_SA1	(__HR_SA1 | __HR_P1)
#define D_HR_P1_IDL	(__HR_IDL | __HR_P1)

/*	MSCI Register (2)		*/
#define __HR_TRBL	0x00		/* Tx/Rx Buffer Reg. L */
#define __HR_TRBH	0x01		/* Tx/Rx Buffer Reg. H */
#define __HR_RRC	0x1A		/* Rx Ready Control Reg. */
#define __HR_TRC0	0x18		/* Tx Ready Control Reg. 0 */
#define __HR_TRC1	0x19		/* Tx Ready Control Reg. 1 */
#define __HR_CST0	0x1C		/* Current status Reg. 0 */
#define __HR_CST1	0x1D		/* Current status Reg. 1 */
#define D_HR_P0_TRBL	(__HR_TRBL | __HR_P0)
#define D_HR_P0_TRBH	(__HR_TRBH | __HR_P0)
#define D_HR_P0_RRC	(__HR_RRC  | __HR_P0)
#define D_HR_P0_TRC0	(__HR_TRC0 | __HR_P0)
#define D_HR_P0_TRC1	(__HR_TRC1 | __HR_P0)
#define D_HR_P0_CST0	(__HR_CST0 | __HR_P0)
#define D_HR_P0_CST1	(__HR_CST1 | __HR_P0)
#define D_HR_P1_TRBL	(__HR_TRBL | __HR_P1)
#define D_HR_P1_TRBH	(__HR_TRBH | __HR_P1)
#define D_HR_P1_RRC	(__HR_RRC  | __HR_P1)
#define D_HR_P1_TRC0	(__HR_TRC0 | __HR_P1)
#define D_HR_P1_TRC1	(__HR_TRC1 | __HR_P1)
#define D_HR_P1_CST0	(__HR_CST0 | __HR_P1)
#define D_HR_P1_CST1	(__HR_CST1 | __HR_P1)

/*	DMA Register Common to CH0-3	*/
#define D_HR_PCR	0x08		/* Priority Control Reg. */
#define D_HR_DMER	0x09		/* DMA Master Enable Reg. */

/*	Channel bit for DMA		*/
#define __HR_DMACH0	0x80		/* Channel 0 MASK */
#define __HR_DMACH1	0xA0		/* Channel 1 MASK */
#define __HR_DMACH2	0xC0		/* Channel 2 MASK */
#define __HR_DMACH3	0xE0		/* Channel 3 MASK */

/*	DMA Register Provided to CH0-3	*/
#define __HR_DARL	0x00		/* Destination Address Reg. L */
#define __HR_DARH	0x01		/* Destination Address Reg. H */
#define __HR_DARB	0x02		/* Destination Address Reg. B */
#define __HR_SARL	0x04		/* Source Address Reg. L */
#define __HR_SARH	0x05		/* Source Address Reg. H */
#define __HR_SARB	0x06		/* Source Address Reg. B */
#define __HR_CDAL	0x08		/* Current Dest. Address Reg. L */
#define __HR_CDAH	0x09		/* Current Dest. Address Reg. H */
#define __HR_EDAL	0x0A		/* Error Dest. Address Reg. L */
#define __HR_EDAH	0x0B		/* Error Dest. Address Reg. H */
#define __HR_BFLL	0x0C		/* Receive Buffer Length Reg. L */
#define __HR_BFLH	0x0D		/* Receive Buffer Length Reg. H */
#define __HR_BCRL	0x0E		/* Byte Count Reg. L */
#define __HR_BCRH	0x0F		/* Byte Count Reg. H */
#define __HR_DSR	0x10		/* DMA status Reg. */
#define __HR_DMR	0x11		/* DMA mode Reg. */
#define __HR_FCT	0x13		/* end-of-Frame CounTer Reg. */
#define __HR_DIR	0x14		/* DMA Interrupt enable Reg. */
#define __HR_DCR	0x15		/* DMA Command Reg. */
#define D_HR_CH0_DARL	(__HR_DARL | __HR_DMACH0)
#define D_HR_CH0_DARH	(__HR_DARH | __HR_DMACH0)
#define D_HR_CH0_DARB	(__HR_DARB | __HR_DMACH0)
#define D_HR_CH0_SARL	(__HR_SARL | __HR_DMACH0)
#define D_HR_CH0_SARH	(__HR_SARH | __HR_DMACH0)
#define D_HR_CH0_SARB	(__HR_SARB | __HR_DMACH0)
#define D_HR_CH0_CDAL	(__HR_CDAL | __HR_DMACH0)
#define D_HR_CH0_CDAH	(__HR_CDAH | __HR_DMACH0)
#define D_HR_CH0_EDAL	(__HR_EDAL | __HR_DMACH0)
#define D_HR_CH0_EDAH	(__HR_EDAH | __HR_DMACH0)
#define D_HR_CH0_BFLL	(__HR_BFLL | __HR_DMACH0)
#define D_HR_CH0_BFLH	(__HR_BFLH | __HR_DMACH0)
#define D_HR_CH0_BCRL	(__HR_BCRL | __HR_DMACH0)
#define D_HR_CH0_BCRH	(__HR_BCRH | __HR_DMACH0)
#define D_HR_CH0_DSR	(__HR_DSR  | __HR_DMACH0)
#define D_HR_CH0_DMR	(__HR_DMR  | __HR_DMACH0)
#define D_HR_CH0_FCT	(__HR_FCT  | __HR_DMACH0)
#define D_HR_CH0_DIR	(__HR_DIR  | __HR_DMACH0)
#define D_HR_CH0_DCR	(__HR_DCR  | __HR_DMACH0)
#define D_HR_CH1_DARL	(__HR_DARL | __HR_DMACH1)
#define D_HR_CH1_DARH	(__HR_DARH | __HR_DMACH1)
#define D_HR_CH1_DARB	(__HR_DARB | __HR_DMACH1)
#define D_HR_CH1_SARL	(__HR_SARL | __HR_DMACH1)
#define D_HR_CH1_SARH	(__HR_SARH | __HR_DMACH1)
#define D_HR_CH1_SARB	(__HR_SARB | __HR_DMACH1)
#define D_HR_CH1_CDAL	(__HR_CDAL | __HR_DMACH1)
#define D_HR_CH1_CDAH	(__HR_CDAH | __HR_DMACH1)
#define D_HR_CH1_EDAL	(__HR_EDAL | __HR_DMACH1)
#define D_HR_CH1_EDAH	(__HR_EDAH | __HR_DMACH1)
#define D_HR_CH1_BFLL	(__HR_BFLL | __HR_DMACH1)
#define D_HR_CH1_BFLH	(__HR_BFLH | __HR_DMACH1)
#define D_HR_CH1_BCRL	(__HR_BCRL | __HR_DMACH1)
#define D_HR_CH1_BCRH	(__HR_BCRH | __HR_DMACH1)
#define D_HR_CH1_DSR	(__HR_DSR  | __HR_DMACH1)
#define D_HR_CH1_DMR	(__HR_DMR  | __HR_DMACH1)
#define D_HR_CH1_FCT	(__HR_FCT  | __HR_DMACH1)
#define D_HR_CH1_DIR	(__HR_DIR  | __HR_DMACH1)
#define D_HR_CH1_DCR	(__HR_DCR  | __HR_DMACH1)
#define D_HR_CH2_DARL	(__HR_DARL | __HR_DMACH2)
#define D_HR_CH2_DARH	(__HR_DARH | __HR_DMACH2)
#define D_HR_CH2_DARB	(__HR_DARB | __HR_DMACH2)
#define D_HR_CH2_SARL	(__HR_SARL | __HR_DMACH2)
#define D_HR_CH2_SARH	(__HR_SARH | __HR_DMACH2)
#define D_HR_CH2_SARB	(__HR_SARB | __HR_DMACH2)
#define D_HR_CH2_CDAL	(__HR_CDAL | __HR_DMACH2)
#define D_HR_CH2_CDAH	(__HR_CDAH | __HR_DMACH2)
#define D_HR_CH2_EDAL	(__HR_EDAL | __HR_DMACH2)
#define D_HR_CH2_EDAH	(__HR_EDAH | __HR_DMACH2)
#define D_HR_CH2_BFLL	(__HR_BFLL | __HR_DMACH2)
#define D_HR_CH2_BFLH	(__HR_BFLH | __HR_DMACH2)
#define D_HR_CH2_BCRL	(__HR_BCRL | __HR_DMACH2)
#define D_HR_CH2_BCRH	(__HR_BCRH | __HR_DMACH2)
#define D_HR_CH2_DSR	(__HR_DSR  | __HR_DMACH2)
#define D_HR_CH2_DMR	(__HR_DMR  | __HR_DMACH2)
#define D_HR_CH2_FCT	(__HR_FCT  | __HR_DMACH2)
#define D_HR_CH2_DIR	(__HR_DIR  | __HR_DMACH2)
#define D_HR_CH2_DCR	(__HR_DCR  | __HR_DMACH2)
#define D_HR_CH3_DARL	(__HR_DARL | __HR_DMACH3)
#define D_HR_CH3_DARH	(__HR_DARH | __HR_DMACH3)
#define D_HR_CH3_DARB	(__HR_DARB | __HR_DMACH3)
#define D_HR_CH3_SARL	(__HR_SARL | __HR_DMACH3)
#define D_HR_CH3_SARH	(__HR_SARH | __HR_DMACH3)
#define D_HR_CH3_SARB	(__HR_SARB | __HR_DMACH3)
#define D_HR_CH3_CDAL	(__HR_CDAL | __HR_DMACH3)
#define D_HR_CH3_CDAH	(__HR_CDAH | __HR_DMACH3)
#define D_HR_CH3_EDAL	(__HR_EDAL | __HR_DMACH3)
#define D_HR_CH3_EDAH	(__HR_EDAH | __HR_DMACH3)
#define D_HR_CH3_BFLL	(__HR_BFLL | __HR_DMACH3)
#define D_HR_CH3_BFLH	(__HR_BFLH | __HR_DMACH3)
#define D_HR_CH3_BCRL	(__HR_BCRL | __HR_DMACH3)
#define D_HR_CH3_BCRH	(__HR_BCRH | __HR_DMACH3)
#define D_HR_CH3_DSR	(__HR_DSR  | __HR_DMACH3)
#define D_HR_CH3_DMR	(__HR_DMR  | __HR_DMACH3)
#define D_HR_CH3_FCT	(__HR_FCT  | __HR_DMACH3)
#define D_HR_CH3_DIR	(__HR_DIR  | __HR_DMACH3)
#define D_HR_CH3_DCR	(__HR_DCR  | __HR_DMACH3)

/*	Channel bit for Timer		*/
#define D_HR_TMCH0	0x60		/* Channel 0 MASK */
#define D_HR_TMCH1	0x68		/* Channel 1 MASK */
#define D_HR_TMCH2	0x70		/* Channel 2 MASK */
#define D_HR_TMCH3	0x78		/* Channel 3 MASK */

/*	Timer Register			*/
#define D_HR_TCNTL	0x00		/* Timer up CouNTer Reg. L */
#define D_HR_TCNTH	0x01		/* Timer up CouNTer Reg. H */
#define D_HR_TCONRL	0x02		/* Timer CONstant Reg. L */
#define D_HR_TCONRH	0x03		/* Timer CONstant Reg. H */
#define D_HR_TCSR	0x04		/* Timer Control/Status Reg. */
#define D_HR_TEPR	0x05		/* Timer Expend Prescale Reg. */

/*	Wait Controller Register	*/
#define D_HR_PABR0	0x02		/* Physical Address Boundary Reg. */
#define D_HR_PABR1	0x03		/* Physical Address Boundary Reg. */
#define D_HR_WCRL	0x04		/* Wait Control Reg. L */
#define D_HR_WCRM	0x05		/* Wait ContRoL Reg. M */
#define D_HR_WCRH	0x06		/* Wait ContRoL Reg. H */

/*	Connamd */
#define D_CMD_Nop	0x00
#define D_CMD_TxReset	0x01
#define D_CMD_TxEnable	0x02
#define D_CMD_TxDisable 0x03
#define D_CMD_TxCRCInit 0x04
#define D_CMD_TxCRCExcl 0x05
#define D_CMD_EOM	0x06
#define D_CMD_AbortTx	0x07
#define D_CMD_MIPOn	0x08
#define D_CMD_ClrTxBuf	0x09
#define D_CMD_RxReset	0x11
#define D_CMD_RxEnable	0x12
#define D_CMD_RxDisable 0x13
#define D_CMD_RxCRCInit 0x14
#define D_CMD_MsgReject 0x15
#define D_CMD_SearchMIP 0x16
#define D_CMD_RxCRCExcl 0x17
#define D_CMD_RxCRCForc 0x18
#define D_CMD_ChReset	0x21
#define D_CMD_SearchST	0x22

#define END_FRAME	0x80
#define TX_END_FRAME	0x81	    /* tx end of a frame */

typedef struct DCP_str {
	u16	cp;
	u32	bp;
	u16	dl;
	u8	st;
	u8	reserved[3];
	u16	bcp;	/* up link description address */
	u16	reserved2;
} DCP, *pDCP;

#define DCP_CP_OFFSET	0
#define DCP_BP_OFFSET	(DCP_CP_OFFSET + sizeof(u16))
#define DCP_DL_OFFSET	(DCP_BP_OFFSET + sizeof(u32))
#define DCP_ST_OFFSET	(DCP_DL_OFFSET + sizeof(u16))
#define DCP_BCP_OFFSET	(DCP_ST_OFFSET + (sizeof(u8) * 4))
#define DCP_SIZE	(DCP_BCP_OFFSET+ (sizeof(u16) * 2))	

#define MAX_BOARD	4

#define TX_CLOCK_IN	0
#define TX_CLOCK_OUT	1

/*
 *	Moxa C101 sync board hardware specification :
 *
 *      The board utilizes memory map. There are 256K on-board memory and 16K
 *      memory space with PC interface, an 8-bit data path accessible to the
 *      memory and a 32-pages, 8K-memory window in addition. Otherwise the host
 *      PC can see 2 pages(16K). The first 8K is usually to map the first
 *      page. Yet the second page must utilize Memory-page-register(address) to
 *      switch to what you want to read.
 *
 *      The dual-port memory address relat to HD64570 is from 0 to 0x3FFFF.
 *
 *      The base address for PC is selected from on-board DIP switch (SW1).
 *
 *	The windows 16K is described by the following base address:
 *
 *	0000h - 1cffh : first bank (always the first page for memory)
 *	2000h - 3fffh : second bank (switch page with the page register)
 *	1d00h - 1dffh : memory page register, write only. Same as write to each
 *			byte of the range memory, otherwise you may write down
 *			the value 00h - 1fh 32 pages in total.
 *			Example:
 *			    Write value 2 on address 1d00h. The second bank
 *			    memory address is 4000h - 5fffh for HD64570.
 *	1e00h - 1effh : HD64570 interrupt acknowledge, read only. Same as read
 *			each byte of the range memory.
 *			Example:
 *			    Read value 0x30 from address 1e00h. The value 0x30
 *			    interrupt vector is released by HD64570.
 *	1e00h - 1effh : RS232/V35 DTR signal, write only (DTR control register)
 *			Same as write to each byte of the range memory, the
 *			bit 0 used only, 0 for OFF, 1 for ON.
 *	1f00h - 1fffh : HD64570 register set.
 *			Example 1:
 *			    Read or write address 1f1ah. It means to read or
 *			    write HD64570 register IVR.
 *			Example 2:
 *			    Read or write address 1f1ch. It means to read or
 *			    write HD64570 register IMVR.
 *
 *	How to reset the HD64570 with software :
 *          Reset the HD64570 after reading the memory-page-register and latching
 *          this-read until next-write to the memory-page-register.
 *
 *      Power on the DRAM is disabled until first-write to DTR control register.
 *
 *      The Tx clock directory is selected from JP2.
 *
 *      The interface RS232 or V.35 is selected from JP3.
 *
 *      The IRQ is selected from JP1.
 */

#define DRAM_SIZE	0x40000 /* 256K bytes */
#define DRAM_SIZE_MASK	(DRAM_SIZE - 1)
#define WINDOWS_SIZE	0x2000	/* 8K bytes */
#define WINDOWS_SIZE_MASK   (WINDOWS_SIZE - 1)
#define TOTAL_PAGE	(DRAM_SIZE / WINDOWS_SIZE)

#define RESET_CONTROL_ADDR  0x1D00
#define PAGE_CONTROL_ADDR   0x1D00
#define USART_ADDR	    0x1F00
#define DTR_CONTROL_ADDR    0x1E00  /* write only */
#define INT_ACK_ADDR	    0x1E00  /* read only */

#define BLOCK_BUFFER_SIZE   512 /* each block buffer size 512 bytes */
#define BLOCK_BUFFER_SIZE_MASK	(BLOCK_BUFFER_SIZE - 1)
#define TOTAL_DCP_NO	    (RESET_CONTROL_ADDR / DCP_SIZE)
#define RX_TOTAL_DCP	    (TOTAL_DCP_NO / 4) 
#define TX_TOTAL_DCP	    (TOTAL_DCP_NO - RX_TOTAL_DCP)
#define TX_BUFFER_SIZE	    (TX_TOTAL_DCP * BLOCK_BUFFER_SIZE)
#define RX_BUFFER_SIZE	    (RX_TOTAL_DCP * BLOCK_BUFFER_SIZE)
#define TX_START_DCP_ADDR   0
#define TX_END_DCP_ADDR     (TX_START_DCP_ADDR + (TX_TOTAL_DCP * DCP_SIZE))
#define RX_START_DCP_ADDR   TX_END_DCP_ADDR
#define RX_END_DCP_ADDR     (RX_START_DCP_ADDR + (RX_TOTAL_DCP * DCP_SIZE))
#define TX_START_BLOCK_ADDR WINDOWS_SIZE
#define TX_END_BLOCK_ADDR   (TX_START_BLOCK_ADDR + TX_BUFFER_SIZE)
#define RX_START_BLOCK_ADDR TX_END_BLOCK_ADDR
#define RX_END_BLOCK_ADDR   (RX_START_BLOCK_ADDR + RX_BUFFER_SIZE)

struct c101_dev {
	ulong	baseaddr;	/* dual-port dram base address */
	ulong	dataaddr;	/* dual-port dram data base address */
	ulong	pagectrl;	/* dual-port dram page change control address */
	ulong	usartaddr;	/* hd64570 base address */
	ulong	intackaddr;	/* interrupt acknowlege address */
	u16	rxhead, rxtail; /* rx receive pointer */
	int	speed;		/* connect speed */
	int	active; 	/* interrupt active flag */

	void	(*rx_function)(struct c101_dev *, struct sk_buff *);

	struct tq_struct	rxqueue;
	struct device		*netdev;
	struct net_device_stats stats;
	struct sk_buff		*txskb;
	struct sk_buff		*rxskb;

	u16	mtu;
	u8	txclock;	/* tx clock directory */
	u8	irq;		/* interrupt number */
	u8	txflag; 	/* transmit first flag */
};

struct net_device_stats *c101_get_stats(struct c101_dev *);
int c101_queue_xmit(struct c101_dev *, struct sk_buff *);
void c101_null_rx(struct c101_dev *, struct sk_buff *);
int c101_shutdown(struct c101_dev *);
int c101_init(struct c101_dev *);
int c101_close(struct c101_dev *);
int c101_open(struct c101_dev *);
void c101_interrupt(int, void *, struct pt_regs *);

#endif	/* end define _HD64570_H */
