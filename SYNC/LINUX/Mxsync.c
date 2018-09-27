/*
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 *	(c) Copyright 2000 Moxa Technologies Co., Ltd.
 *
 *	http://www.moxa.com.tw
 *	http://www.moxa.com
 *	email:support@moxa.com.tw
 *	ftp://ftp.moxa.com.tw
 *	ftp://ftp.moxa.com
 */


#define RT_LOCK
#define RT_UNLOCK



#include "hd64570.h"

static void c101_irq_tx(struct c101_dev *);
static void c101_irq_dmia(struct c101_dev *);
static void c101_irq_dmib(struct c101_dev *);
static void c101_rx_queue(void *);

static void c101_irq_tx(struct c101_dev *dev)
{
	ulong	    usartaddr;
	u8	    ch;
	u16	    cda, eda;

	usartaddr = dev->usartaddr;
	if ( (ch=readb(usartaddr+D_HR_P0_ST1)) & 0x80 )
	    writeb(0x80, usartaddr+D_HR_P0_ST1);
	if ( (ch & 0x40) == 0 )
	    return;
	writeb(0x40, usartaddr+D_HR_P0_ST1);
	cda = readb(usartaddr+D_HR_CH1_CDAL);
	cda |= (readb(usartaddr+D_HR_CH1_CDAH) << 8);
	eda = readb(usartaddr+D_HR_CH1_EDAL);
	eda |= (readb(usartaddr+D_HR_CH1_EDAH) << 8);
	if ( cda == eda ) {
	    dev->netdev->tbusy = 0;
	    dev->txflag = 0;
	    writeb(0x80, usartaddr+D_HR_P0_IE1);
	} else {
	    writeb(0x02, usartaddr+D_HR_CH1_DSR);
	    writeb(D_CMD_TxEnable, usartaddr+D_HR_P0_CMD);
	}
}

static void c101_irq_dmia(struct c101_dev *dev)
{
	ulong	    usartaddr;

	usartaddr = dev->usartaddr;
	writeb(D_CMD_RxReset, usartaddr+D_HR_P0_CMD);
	writeb(D_CMD_RxCRCInit, usartaddr+D_HR_P0_CMD);
	writeb(D_CMD_RxEnable, usartaddr+D_HR_P0_CMD);
	writeb(0xE2, usartaddr+D_HR_CH0_DSR);
}

static void c101_irq_dmib(struct c101_dev *dev)
{
	ulong	    usartaddr;
	u16	    cda;

	usartaddr = dev->usartaddr;
	cda = readb(usartaddr+D_HR_CH0_CDAL);
	cda |= (readb(usartaddr+D_HR_CH0_CDAH) << 8);
	dev->rxtail = cda;
	writeb(0xE2, usartaddr+D_HR_CH0_DSR);
	queue_task(&dev->rxqueue, &tq_immediate);
	mark_bh(IMMEDIATE_BH);
}

static void c101_rx_queue(void *d)
{
	struct sk_buff	*skb;
	struct c101_dev *dev;
	u16		head, tail, tmphead, len, dl, bcp;
	ulong		dcp, usartaddr, dataaddr, pagectrl, baseaddr, ul;
	u8		*data, page;
	u32		bp;
	int		i, j;

	dcp = 0;
	dev = (struct c101_dev *)d;
	baseaddr = dev->baseaddr;
	pagectrl = dev->pagectrl;
	usartaddr = dev->usartaddr;
	dataaddr = dev->dataaddr;
	head = dev->rxhead;
	tail = dev->rxtail;
	while ( head != tail ) {
	    /* to get the first frame length */
	    len = 0;
	    tmphead = head;
	    while ( tmphead != tail ) {
		dcp = baseaddr + tmphead;
		len += readw(dcp+DCP_DL_OFFSET);
		if ( readb(dcp+DCP_ST_OFFSET) & END_FRAME )
		    break;
		tmphead = readw(dcp+DCP_CP_OFFSET);
	    }

	    /* to allocate the skb */
	    /* and receive the data */
	    skb = dev_alloc_skb(len);
	    if ( skb == NULL ) {
		i = 0;
		while ( i < len ) {
		    dcp = baseaddr + head;
		    bp = readl(dcp+DCP_BP_OFFSET);
		    dl = readw(dcp+DCP_DL_OFFSET);
		    page = bp / WINDOWS_SIZE;
		    writeb(page, pagectrl);
		    ul = bp & WINDOWS_SIZE_MASK;
		    bp = dataaddr + ul;
		    for ( j=0; j<dl && i<len; i++, j++, ul++, bp++ ) {
			if ( ul >= WINDOWS_SIZE ) {
			    page++;
			    writeb(page, pagectrl);
			    ul = 0;
			    bp = dataaddr;
			}
			readb(bp);
		    }
		    writew(BLOCK_BUFFER_SIZE, dcp+DCP_DL_OFFSET);
		    writeb(0, dcp+DCP_ST_OFFSET);
		    if ( i >= len )
			break;
		    head = readw(dcp+DCP_CP_OFFSET);
		}
		dev->stats.rx_dropped++;
	    } else {
		i = 0;
		data = skb->data;
		while ( i < len ) {
		    dcp = baseaddr + head;
		    bp = readl(dcp+DCP_BP_OFFSET);
		    dl = readw(dcp+DCP_DL_OFFSET);
		    page = bp / WINDOWS_SIZE;
		    writeb(page, pagectrl);
		    ul = bp & WINDOWS_SIZE_MASK;
		    bp = dataaddr + ul;
		    for ( j=0; j<dl && i<len; i++, j++, ul++, bp++ ) {
			if ( ul >= WINDOWS_SIZE ) {
			    page++;
			    writeb(page, pagectrl);
			    ul = 0;
			    bp = dataaddr;
			}
			*data++ = readb(bp);
		    }
		    writew(BLOCK_BUFFER_SIZE, dcp+DCP_DL_OFFSET);
		    writeb(0, dcp+DCP_ST_OFFSET);
		    if ( i >= len )
			break;
		    head = readw(dcp+DCP_CP_OFFSET);
		}
		skb->len = len;
		dev->rx_function(dev, skb);
		dev->stats.rx_packets++;
		dev->stats.rx_bytes+=len;
	    }
	    head = readw(dcp+DCP_CP_OFFSET);
	}

	/* to move the eda for HD64570 */
	dev->rxhead = head;
	dcp = baseaddr + head;
	bcp = readw(dcp+DCP_BCP_OFFSET);
	writeb(bcp & 0xFF, usartaddr+D_HR_CH0_EDAL);
	writeb(bcp >> 8, usartaddr+D_HR_CH0_EDAH);
}

void c101_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	struct c101_dev *dev=dev_id;
	ulong		usartaddr, intackaddr;

	/* may be need to check reentry or not */

	/* check is my irq or not */
	if ( irq != dev->irq )
	    return;

	/* to get the interrupt vector from HD64570 */
	usartaddr = dev->usartaddr;
	intackaddr = dev->intackaddr;
	do {
	    switch ( readb(intackaddr) ) {
	    case 0x0A : c101_irq_tx(dev);   break;
	    case 0x14 : c101_irq_dmia(dev); break;
	    case 0x16 : c101_irq_dmib(dev); break;
	    }
	} while ( readb(usartaddr+D_HR_ISR0) != 0 || readb(usartaddr+D_HR_ISR1) != 0 );
}

EXPORT_SYMBOL(c101_interrupt);

int c101_open(struct c101_dev *dev)
{
	u16	cda, da;
	ulong	usartaddr, baseaddr;
	u8	ch;
	int	i;

	usartaddr = dev->usartaddr;
	baseaddr = dev->baseaddr;

	/* to check Moxa C101 board is exist or not */
	if ( readb(usartaddr+D_HR_IVR) != 0 || readb(usartaddr+D_HR_IMVR) != 0 ) {
	    printk(KERN_WARNING "Cannot find Moxa C101 board [%lx] !\n", baseaddr);
	    return -ENODEV;
	}

	/* disable the tx and rx */
	writeb(D_CMD_TxDisable, usartaddr+D_HR_P0_CMD);
	writeb(D_CMD_RxDisable, usartaddr+D_HR_P0_CMD);

	/* flush tx and rx buffer */
	cda = readb(usartaddr+D_HR_CH1_CDAL);
	cda |= (readb(usartaddr+D_HR_CH1_CDAH) << 8);
	writeb(cda & 0xFF, usartaddr+D_HR_CH1_EDAL);
	writeb(cda >> 8, usartaddr+D_HR_CH1_EDAH);

	cda = readb(usartaddr+D_HR_CH0_CDAL);
	cda |= (readb(usartaddr+D_HR_CH0_CDAH) << 8);
	dev->rxhead = cda;
	dev->rxtail = cda;
	/* to clear rx buffer DCP */
	for ( i=0, da=cda; i<RX_TOTAL_DCP; i++, da+=DCP_SIZE ) {
	    writeb(0, baseaddr+da+DCP_ST_OFFSET);
	    writew(BLOCK_BUFFER_SIZE, baseaddr+da+DCP_DL_OFFSET);
	}
	cda = readw(baseaddr+cda+DCP_BCP_OFFSET);
	writeb(cda & 0xFF, usartaddr+D_HR_CH0_EDAL);
	writeb(cda >> 8, usartaddr+D_HR_CH0_EDAH);

	/* enable the tx and rx */
	writeb(0xE1, usartaddr+D_HR_CH1_DSR);
	writeb(0xE2, usartaddr+D_HR_CH0_DSR);
	ch = readb(usartaddr+D_HR_IER0) & 0xF0;
	ch |= 0x08;
	writeb(ch, usartaddr+D_HR_IER0);
	ch = readb(usartaddr+D_HR_IER1) & 0xF0;
	ch |= 0x03;
	writeb(ch, usartaddr+D_HR_IER1);
	writeb(0x80, usartaddr+D_HR_P0_IE0);
	writeb(0x80, usartaddr+D_HR_P0_IE1);
	writeb(D_CMD_TxEnable, usartaddr+D_HR_P0_CMD);
	writeb(D_CMD_RxEnable, usartaddr+D_HR_P0_CMD);

	return 0;
}


EXPORT_SYMBOL(c101_open);

int c101_close(struct c101_dev *dev)
{
	ulong	usartaddr;
	u8	ch;

	usartaddr = dev->usartaddr;
	writeb(D_CMD_TxDisable, usartaddr+D_HR_P0_CMD);
	writeb(D_CMD_RxDisable, usartaddr+D_HR_P0_CMD);
	writeb(0, usartaddr+D_HR_P0_IE0);
	writeb(0, usartaddr+D_HR_P0_IE1);
	ch = readb(usartaddr+D_HR_IER0) & 0xF0;
	writeb(ch, usartaddr+D_HR_IER0);
	ch = readb(usartaddr+D_HR_IER1) & 0xF0;
	writeb(ch, usartaddr+D_HR_IER1);
	writeb(0xE1, usartaddr+D_HR_CH1_DSR);
	writeb(0xE1, usartaddr+D_HR_CH0_DSR);
	dev->txflag = 0;
	return 0;
}

EXPORT_SYMBOL(c101_close);

/*
 * to initialize the C101 board
 *
 *	crstal = 9.8304MHz
 *	Baud rate = (Crstal/TMC) / 2^BR
 *
 *	TMC = Crstal / (Baud rate * 2 ^ BR)
 *
 *	the compute logical is following:
 *
 *	br = 0;
 *	brvalue = 1;
 *	while ( 1 ) {
 *	    tmc = crstal / (baud-rate * brvalue);
 *	    if ( tmc <= 255 )
 *		break;
 *	    else {
 *		br++;
 *		brvalue *= 2;
 *	    }
 *	}
 */
int c101_init(struct c101_dev *dev)
{
	u8	ch;
	ulong	usartaddr, dcp, baseaddr, prevdcp;
	u16	head;
	u32	bp;
	ulong	tmc, crystal, br, brvalue, speed;
	int	i;

	/* to reset the board */
	writeb(0, dev->intackaddr);	/* enable DRAM */
	udelay(20000);
	ch = readb(dev->pagectrl);
	udelay(20000);
	writeb(ch, dev->pagectrl);

	/* reset the HD64570 */
	usartaddr = dev->usartaddr;
	baseaddr = dev->baseaddr;
	writeb(0, usartaddr+D_HR_IVR);
	writeb(0, usartaddr+D_HR_IMVR);

	/* to check Moxa C101 board is exist or not */
	if ( readb(usartaddr+D_HR_IVR) != 0 || readb(usartaddr+D_HR_IMVR) != 0 ) {
	    printk(KERN_WARNING "Cannot find Moxa C101 board [%lx] !\n", baseaddr);
	    return -ENODEV;
	}

	writeb(0xB0, usartaddr+D_HR_ITCR);  /* interrupt priority DMAC > MSCI
					       single acknowledge cycle
					       interrupt modified vector */
	writeb(0, usartaddr+D_HR_IER0);     /* disable all interrupt */
	writeb(0, usartaddr+D_HR_IER1);
	writeb(0, usartaddr+D_HR_IER2);     /* disable all timer interrupt */
	writeb(0x40, usartaddr+D_HR_PABR0);
	writeb(0x80, usartaddr+D_HR_PABR1);
	writeb(0, usartaddr+D_HR_WCRL);     /* set DRAM wait state zero */
	writeb(0, usartaddr+D_HR_WCRM);
	writeb(0, usartaddr+D_HR_WCRH);
	writeb(0x80, usartaddr+D_HR_DMER);  /* DMA master enable */
	writeb(0x12, usartaddr+D_HR_PCR);   /* DMA prioity control set */

	writeb(0x87, usartaddr+D_HR_P0_MD0);/* bit-sync HDLC mode
					       CRC code calculation enable
					       CCITT, init value = all 1s */
	writeb(0, usartaddr+D_HR_P0_MD1);   /* address field no-check
					       Tx/Rx data bit 8, no parity */
	writeb(0, usartaddr+D_HR_P0_MD2);   /* NRZ, APDLLx8 */
	writeb(0x31, usartaddr+D_HR_P0_CTL);/* enters idle state
					       after FCS and
					       flag transmission
					       transmits an idle
					       pattern RTS off */

	/* to compute the TMC value */
	speed = dev->speed;
	br = 0;
	brvalue = 1;
	crystal = 9830400;
	while ( 1 ) {
	    tmc = crystal / (speed * brvalue);
	    if ( tmc <= 255 )
		break;
	    br++;
	    brvalue <<= 1;
	}
	writeb((u8)tmc, usartaddr+D_HR_P0_TMC);
	ch = readb(usartaddr+D_HR_P0_RXS) & 0xF0;
	ch |= (u8)br;
	writeb(ch, usartaddr+D_HR_P0_RXS);
	ch = readb(usartaddr+D_HR_P0_TXS) & 0xF0;
	ch |= (u8)br;
	if ( dev->txclock == TX_CLOCK_OUT )
	    ch |= 0x40;
	writeb(ch, usartaddr+D_HR_P0_TXS);

	writeb(0, usartaddr+D_HR_P0_IE0);	/* disable all interrupt */
	writeb(0, usartaddr+D_HR_P0_IE1);
	writeb(0, usartaddr+D_HR_P0_IE2);
	writeb(0, usartaddr+D_HR_P0_FIE);	/* EOMF interrupt disable */
	writeb(0xFF, usartaddr+D_HR_P0_IDL);	/* idle pattern = 0xFF */
	writeb(0x04, usartaddr+D_HR_P0_RRC);	/* 04 chars to enable RxRDY */
	writeb(0x10, usartaddr+D_HR_P0_TRC0);	/* 16 chars to enable TxRDY */
	writeb(0x1F, usartaddr+D_HR_P0_TRC1);	/* 31 chars to disable TxRDY */

	/* initialize the DMA */
	writeb(0, usartaddr+D_HR_CH1_SARB);
	writeb(0, usartaddr+D_HR_CH0_SARB);
	writeb(0x14, usartaddr+D_HR_CH1_DMR);
	writeb(0x14, usartaddr+D_HR_CH0_DMR);
	writeb(0, usartaddr+D_HR_CH1_DIR);
	writeb(0xE0, usartaddr+D_HR_CH0_DIR);
	writeb(BLOCK_BUFFER_SIZE & 0xFF, usartaddr+D_HR_CH0_BFLL);
	writeb(BLOCK_BUFFER_SIZE >> 8, usartaddr+D_HR_CH0_BFLH);

	/* initial the tx DCP */
	head = TX_START_DCP_ADDR;
	dcp = baseaddr + head;
	bp = TX_START_BLOCK_ADDR;
	for ( i=0; i<TX_TOTAL_DCP; i++, bp+=BLOCK_BUFFER_SIZE ) {
	    prevdcp = dcp;
	    dcp += DCP_SIZE;
	    writew((u16)(dcp-baseaddr), prevdcp+DCP_CP_OFFSET);
	    writel(bp, prevdcp+DCP_BP_OFFSET);
	    writew(BLOCK_BUFFER_SIZE, prevdcp+DCP_DL_OFFSET);
	    writeb(0, prevdcp+DCP_ST_OFFSET);
	    writew((u16)(prevdcp-baseaddr), dcp+DCP_BCP_OFFSET);
	}
	writew(head, prevdcp+DCP_CP_OFFSET);
	writew((u16)(prevdcp-baseaddr), head+baseaddr+DCP_BCP_OFFSET);

	writeb(head & 0xFF, usartaddr+D_HR_CH1_CDAL);
	writeb(head >> 8, usartaddr+D_HR_CH1_CDAH);
	writeb(head & 0xFF, usartaddr+D_HR_CH1_EDAL);
	writeb(head >> 8, usartaddr+D_HR_CH1_EDAH);

	/* initialize the rx DCP */
	head = RX_START_DCP_ADDR;
	dcp = baseaddr + head;
	bp = RX_START_BLOCK_ADDR;
	for ( i=0; i<RX_TOTAL_DCP; i++, bp+=BLOCK_BUFFER_SIZE ) {
	    prevdcp = dcp;
	    dcp += DCP_SIZE;
	    writew((u16)(dcp-baseaddr), prevdcp+DCP_CP_OFFSET);
	    writel(bp, prevdcp+DCP_BP_OFFSET);
	    writew(BLOCK_BUFFER_SIZE, prevdcp+DCP_DL_OFFSET);
	    writeb(0, prevdcp+DCP_ST_OFFSET);
	    writew((u16)(prevdcp-baseaddr), dcp+DCP_BCP_OFFSET);
	}
	writew(head, prevdcp+DCP_CP_OFFSET);
	writew((u16)(prevdcp-baseaddr), head+baseaddr+DCP_BCP_OFFSET);
	writeb(head & 0xFF, usartaddr+D_HR_CH0_CDAL);
	writeb(head >> 8, usartaddr+D_HR_CH0_CDAH);
	head = (u16)(prevdcp-baseaddr);
	writeb(head & 0xFF, usartaddr+D_HR_CH0_EDAL);
	writeb(head >> 8, usartaddr+D_HR_CH0_EDAH);

	/* initialize the dma register */
	writeb(0xE1, usartaddr+D_HR_CH1_DSR);
	writeb(0xE1, usartaddr+D_HR_CH0_DSR);

	/* may be let the DTR ON */

	dev->txflag = 0;

	dev->rxqueue.routine = c101_rx_queue;
	dev->rxqueue.data = dev;

	return 0;
}


EXPORT_SYMBOL(c101_init);

int c101_shutdown(struct c101_dev *dev)
{
	/* disable c101 */
	readb(dev->pagectrl);
	return 0;
}

EXPORT_SYMBOL(c101_shutdown);

/*
 *	Higher level shovelling - receive chains
 */

void c101_null_rx(struct c101_dev *c, struct sk_buff *skb)
{
	dev_kfree_skb(skb);
}

EXPORT_SYMBOL(c101_null_rx);

/*
 *	Queue a packet for transmission. Because we have rather
 *	hard to hit interrupt latencies for the Z85230 per packet
 *	even in DMA mode we do the flip to DMA buffer if needed here
 *	not in the IRQ.
 */

int c101_queue_xmit(struct c101_dev *c, struct sk_buff *skb)
{
	unsigned long	flags;
	unsigned long	baseaddr, dcp=0, pagectrl, dataaddr, ul, usartaddr;
	u16		cda, eda;
	u32		bp;
	int		i, len, j;
	u8		*data, page;

	/*
	 * to check tx output buffer is enough or not
	 */
	baseaddr = c->baseaddr;
	usartaddr = c->usartaddr;
	cda = readb(usartaddr+D_HR_CH1_CDAL);
	cda |= (readb(usartaddr+D_HR_CH1_CDAH) << 8);
	eda = readb(usartaddr+D_HR_CH1_EDAL);
	eda |= (readb(usartaddr+D_HR_CH1_EDAH) << 8);
	eda = readw(baseaddr+eda+DCP_CP_OFFSET);
	i = 0;
	len = skb->len;
	while ( eda != cda ) {
	    i += BLOCK_BUFFER_SIZE;
	    if ( i >= len )
		break;
	    eda = readw(baseaddr+eda+DCP_CP_OFFSET);
	}
	if ( i < len ) {
	    skb->dev->tbusy = 1;
	    return 1;
	}

	/*
	 * start to put the data on dual-port DRAM
	 */
	eda = readb(usartaddr+D_HR_CH1_EDAL);
	eda |= (readb(usartaddr+D_HR_CH1_EDAH) << 8);
	data = skb->data;
	pagectrl = c->pagectrl;
	dataaddr = c->dataaddr;
	i = 0;
	while ( i < len ) {
	    dcp = baseaddr + eda;
	    bp = readl(dcp+DCP_BP_OFFSET);
	    page = (u8)(bp / WINDOWS_SIZE);
	    writeb(page, pagectrl);
	    ul = bp & WINDOWS_SIZE_MASK;
	    bp = dataaddr + ul;
	    for ( j=0; j<BLOCK_BUFFER_SIZE && i<len; j++, i++, bp++, ul++ ) {
		if ( ul >= WINDOWS_SIZE ) {
		    page++;
		    writeb(page, pagectrl);
		    ul = 0;
		    bp = dataaddr;
		}
		writeb(*data++, bp);
	    }
	    writew((u16)j, dcp+DCP_DL_OFFSET);
	    if ( i >= len ) {
		writeb(TX_END_FRAME, dcp+DCP_ST_OFFSET);
		break;
	    }
	    writeb(0, dcp+DCP_ST_OFFSET);
	    eda = readw(dcp+DCP_CP_OFFSET);
	}
	eda = readw(dcp+DCP_CP_OFFSET);

	/*
	 * to check first transmit or not
	 */
	save_flags(flags);
	cli();
	writeb(eda&0xFF, usartaddr+D_HR_CH1_EDAL);
	writeb(eda>>8, usartaddr+D_HR_CH1_EDAH);
	if ( c->txflag == 0 ) {
	    writeb(0x02, usartaddr+D_HR_CH1_DSR);
	    writeb(D_CMD_TxEnable, usartaddr+D_HR_P0_CMD);
	    writeb(0xC0, usartaddr+D_HR_P0_IE1);
	    c->txflag = 1;
	}
	restore_flags(flags);

	c->stats.tx_packets++;
	c->stats.tx_bytes+=len;
	dev_kfree_skb(skb);
	mark_bh(NET_BH);

	return 0;
}

EXPORT_SYMBOL(c101_queue_xmit);

struct net_device_stats *c101_get_stats(struct c101_dev *c)
{
	return &c->stats;
}

EXPORT_SYMBOL(c101_get_stats);

#ifdef MODULE

/*
 *	Module support
 */

int init_module(void)
{
	printk(KERN_INFO "Generic Moxa C101 board  interface driver v1.0\n");
	return 0;
}

void cleanup_module(void)
{
}

#endif
