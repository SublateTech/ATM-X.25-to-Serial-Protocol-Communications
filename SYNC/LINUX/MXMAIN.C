#define LINUX_21

/*
 *	Moxa Sync Board serial Linux PPP device driver main program.
 *	Hardware is C101 board. Max support 4 boards on one system.
 *	The memory bank and irq can not be shared.
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	1 of the License, or (at your option) any later version.
 *
 *	(c) Copyright 2000 Moxa Technologies Co., Ltd.
 *
 *	http://www.moxa.com.tw
 *	http://www.moxa.com
 *	email:support@moxa.com.tw
 *	ftp://ftp.moxa.com.tw
 *	ftp://ftp.moxa.com
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <net/arp.h>
#include <asm/io.h>
#include <asm/dma.h>
#include <asm/byteorder.h>

#include "/usr/src/linux/drivers/net/syncppp.h"
#include "hd64570.h"

struct moxa_device {
	struct c101_dev     c101dev;
	struct ppp_device   pppdev;
	char		    name[16];
};

static int  baseaddr[MAX_BOARD] = {0xc8000, 0,	   0,	  0};
static int  irq[MAX_BOARD]	= {10,	    0,	   0,	  0};
static int  speed[MAX_BOARD]	= {64000,   64000, 64000, 64000};
static int  txclock[MAX_BOARD]	= {0,	    0,	   0,	  0};

static struct moxa_device   *moxadev;

static void moxa_shutdown(struct moxa_device *);

#ifdef MODULE
static struct moxa_device *moxasync_init(void);

#ifdef LINUX_21
MODULE_PARM(baseaddr,"1-4i");
MODULE_PARM_DESC(baseaddr, "The dual-port memory base address of Moxa C101 board");
MODULE_PARM(irq,"1-4i");
MODULE_PARM_DESC(irq, "The interrupt line setting for Moxa C101 board");
MODULE_PARM(speed, "1-4i");
MODULE_PARM_DESC(speed, "The used speed for connection");
MODULE_PARM(txclock, "1-4i");
MODULE_PARM_DESC(txclock, "The transmit clock directory");

MODULE_AUTHOR("Victor Yu");
MODULE_DESCRIPTION("Modular driver for Moxa C101 board");
#endif

int init_module(void)
{
	printk(KERN_INFO "Moxa C101 Synchronous Driver v 1.0.\n");
	printk(KERN_INFO "(c) Copyright 2000, Moxa Technologies Co., Ltd.\n");
	if( (moxadev=moxasync_init()) == NULL )
	    return -ENODEV;
	return 0;
}

void cleanup_module(void)
{
	if ( moxadev ) {
	    moxa_shutdown(moxadev);
	    kfree(moxadev);
	}
}

#endif

/*
 *	Network driver support routines
 */

/*
 *	Frame receive. Simple for our card as we do sync ppp and there
 *	is no funny garbage involved
 */

static void mxmain_input(struct c101_dev *c, struct sk_buff *skb)
{
	skb->protocol=htons(ETH_P_WAN_PPP);
	skb->mac.raw=skb->data;
	skb->dev=c->netdev;
	/*
	 *	Send it to the PPP layer. We dont have time to process
	 *	it right now.
	 */
	netif_rx(skb);
}

/*
 *	We've been placed in the UP state
 */

static int mxmain_open(struct device *d)
{
	struct moxa_device  *mxdev=d->priv;
	int		    err=-1;

	/*
	 *	Link layer up.
	 */
	if ( (err=c101_open(&mxdev->c101dev)) )
	    return err;

	/*
	 *	Begin PPP
	 */
	if ( (err=sppp_open(d)) ) {
	    c101_close(&mxdev->c101dev);
	    return err;
	}

	mxdev->c101dev.rx_function=mxmain_input;

	/*
	 *	Go go go
	 */
	d->tbusy=0;
	MOD_INC_USE_COUNT;
	return 0;
}

static int mxmain_close(struct device *d)
{
	struct moxa_device  *mxdev=d->priv;

	/*
	 *	Discard new frames
	 */
	mxdev->c101dev.rx_function = c101_null_rx;

	/*
	 *	PPP off
	 */
	sppp_close(d);

	/*
	 *	Link layer down
	 */
	d->tbusy=1;
	c101_close(&mxdev->c101dev);
	MOD_DEC_USE_COUNT;
	return 0;
}

static int mxmain_ioctl(struct device *d, struct ifreq *ifr, int cmd)
{
	/* struct moxa_device	*mxdev=d->priv;
	   c101_ioctl(d,&mxdev->c101dev,ifr,cmd) */
	return sppp_do_ioctl(d, ifr,cmd);
}

static struct enet_statistics *mxmain_get_stats(struct device *d)
{
	struct moxa_device  *mxdev=d->priv;

	if( mxdev )
		return c101_get_stats(&mxdev->c101dev);
	else
		return NULL;
}

/*
 *	Passed PPP frames, fire them downwind.
 */
static int mxmain_queue_xmit(struct sk_buff *skb, struct device *d)
{
	struct moxa_device  *mxdev=d->priv;

	return c101_queue_xmit(&mxdev->c101dev, skb);
}

#ifdef LINUX_21
static int mxmain_neigh_setup(struct neighbour *n)
{
	if (n->nud_state == NUD_NONE) {
	    n->ops = &arp_broken_ops;
	    n->output = n->ops->output;
	}
	return 0;
}

static int mxmain_neigh_setup_dev(struct device *dev, struct neigh_parms *p)
{
	if (p->tbl->family == AF_INET) {
	    p->neigh_setup = mxmain_neigh_setup;
	    p->ucast_probes = 0;
	    p->mcast_probes = 0;
	}
	return 0;
}

#else

static int return_0(struct device *d)
{
	return 0;
}

#endif

/*
 *	Description block for a Comtrol Hostess SV11 card
 */
static struct moxa_device *moxasync_init()
{
	struct c101_dev     *c101dev=NULL;
	struct moxa_device  *mxdev;
	int		    i;

	/*
	 * to check any board is set
	 */
	for ( i=0; i<MAX_BOARD; i++ ) {
	    if ( baseaddr[i] != 0 )
		break;
	}
	if ( i == MAX_BOARD ) {
	    printk(KERN_WARNING "mxmain: Not any Moxa board is set.\n");
	    return NULL;
	}

	/*
	 * allocate the moxa device
	 */
	mxdev = (struct moxa_device *)kmalloc(sizeof(struct moxa_device) * MAX_BOARD, GFP_KERNEL);
	if ( mxdev == NULL )
	    return NULL;

	/*
	 * initialize the variable
	 */
	memset(mxdev, 0, sizeof(struct moxa_device) * MAX_BOARD);
	for ( i=0; i<MAX_BOARD; i++ ) {
	    if ( baseaddr[i] == 0 )
		continue;
	    c101dev = &mxdev[i].c101dev;
#ifdef __alpha__
	    if ( baseaddr[i] >= 0x100000 )
		c101dev->baseaddr = (ulong)ioremap((ulong)baseaddr[i], 0x4000);
	    else
#endif
		c101dev->baseaddr = (ulong)baseaddr[i];
	    c101dev->dataaddr = c101dev->baseaddr + WINDOWS_SIZE;
	    c101dev->pagectrl = c101dev->baseaddr + PAGE_CONTROL_ADDR;
	    c101dev->usartaddr = c101dev->baseaddr + USART_ADDR;
	    c101dev->intackaddr = c101dev->baseaddr + INT_ACK_ADDR;
	    c101dev->speed = speed[i];
	    c101dev->irq = (u8)irq[i];
	    c101dev->txclock = (u8)txclock[i];
	    c101dev->netdev = &mxdev[i].pppdev.dev;
	    /*
	    c101dev->active = 0;
	    c101dev->netdev->priv = &mxdev[i];
	    c101dev->netdev->name = mxdev[i].name;
	    */
	    if ( request_irq(irq[i], &c101_interrupt, SA_INTERRUPT, "Moxa", c101dev) < 0 ) {
		printk(KERN_WARNING "mxmain: Moxa C101 board [%d] IRQ [%d] already in use.\n", i+1, irq[i]);
		goto failirq;
	    }
	}

	/*
	 * initialize the module and every board
	 */
	for ( i=0; i<MAX_BOARD; i++ ) {
	    if ( baseaddr[i] == 0 )
		continue;
	    c101dev = &mxdev[i].c101dev;
	    if ( c101_init(c101dev) != 0 ) {
		printk(KERN_ERR "mxmain: Moxa C101 board [%d] not found !\n", i+1);
		goto failregister;
	    }
	    sprintf(mxdev[i].name, "mxppp%d", i);
	    if ( dev_get(mxdev[i].name) == NULL ) {
		struct device	*dev=mxdev[i].c101dev.netdev;

		/*
		 * initialize the PPP components
		 */
		sppp_attach(&mxdev[i].pppdev);
		dev->name = mxdev[i].name;
		dev->priv = &mxdev[i];
		dev->irq = irq[i];
		dev->rmem_start = mxdev[i].c101dev.baseaddr;
		dev->rmem_end = dev->rmem_start + (WINDOWS_SIZE * 2) - 1;
		dev->mem_start = dev->rmem_start;
		dev->mem_end = dev->rmem_end;
		dev->init = NULL;
		dev->open = mxmain_open;
		dev->stop = mxmain_close;
		dev->hard_start_xmit = mxmain_queue_xmit;
		dev->get_stats = mxmain_get_stats;
		dev->set_multicast_list = NULL;
		dev->do_ioctl = mxmain_ioctl;
#ifdef LINUX_21
		dev->neigh_setup = mxmain_neigh_setup_dev;
		dev_init_buffers(dev);
#else
		dev->init = return_0;
#endif
		dev->set_mac_address = NULL;

		if ( register_netdev(dev) == -1 ) {
		    printk(KERN_ERR "mxmain: %s unable to register device.\n",
			    mxdev[i].name);
		    goto failregister;
		}
		mxdev[i].c101dev.active = 1;
	    }
	}

	return mxdev;

failregister:
	for ( i=0; i<MAX_BOARD; i++ ) {
	    if ( baseaddr[i] == 0 || mxdev[i].c101dev.active == 0 )
		continue;
	    sppp_detach(&mxdev[i].pppdev.dev);
	    unregister_netdev(&mxdev[i].pppdev.dev);
	}

failirq:
	for ( i=0; i<MAX_BOARD; i++ ) {
	    if ( baseaddr[i] == 0 || mxdev[i].c101dev.irq == 0 )
		continue;
	    free_irq(mxdev[i].c101dev.irq, &mxdev[i].c101dev);
	}

	kfree(mxdev);

	return NULL;
}

static void moxa_shutdown(struct moxa_device *mxdev)
{
	int	i;

	for ( i=0; i<MAX_BOARD; i++ ) {
	    if ( baseaddr[i] == 0 )
		continue;
	    c101_shutdown(&mxdev[i].c101dev);
	    sppp_detach(&mxdev[i].pppdev.dev);
	    unregister_netdev(&mxdev[i].pppdev.dev);
	    free_irq(mxdev[i].c101dev.irq, &mxdev[i].c101dev);
	}
}
