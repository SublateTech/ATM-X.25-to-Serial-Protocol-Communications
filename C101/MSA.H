/*
 *	Moxa Sync API DDL Driver define
 */

#ifdef __cplusplus
extern "C" {
#endif

/* function declare */
int	_export WINAPI	MSAInit(void);
void	_export WINAPI	MSAReset(void);
int	_export WINAPI	MSARead(char far *buf, int len);
int	_export WINAPI	MSAWrite(char far *buf, int len);
int	_export WINAPI	MSAView(char far *buf, int len);
void	_export WINAPI	MSASkip(void);
void	_export WINAPI	MSAFlush(int mode);
long	_export WINAPI	MSAIsOverRun(void);
long	_export WINAPI	MSAIsUnderRun(void);
long	_export WINAPI	MSAIsBadFrame(void);
int	_export WINAPI	MSAIFrame(void);
int	_export WINAPI	MSAOFrame(void);
int	_export WINAPI	MSAIframeFree(void);
int	_export WINAPI	MSAOFrameFree(void);
void	_export WINAPI	MSALCtrl(int mode);
int	_export WINAPI	MSALStatus(void);
void	_export WINAPI	MSAEnd(void);

#ifdef __cplusplus
}
#endif

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
#define ERR_CNF_FILE	-1	/* Config error: 'c101.cnf' not found */
#define ERR_CNF_KEYWORD -2	/* Config error: keyword error */
#define ERR_CNF_ARG	-3	/* Config error: no argment */
#define ERR_CNF_ADDR	-4	/* Config error: base addr setting error */
#define ERR_CNF_IRQ	-5	/* Config error: irq setting error */
#define ERR_CNF_SPEED	-6	/* Config error: speed setting error */
#define ERR_CNF_PKTSIZE -7	/* Config error: packet size setting error */
#define ERR_CNF_PKTCNT	-8	/* Config error: packet count setting error */
#define ERR_CNF_RXCS	-9	/* Config error: receive clock source setting error */
#define ERR_CNF_TXCS	-10	/* Config error: transmit clock source setting error */
#define ERR_CNF_LOOPM	-11	/* Config error: look back mode setting error */
#define ERR_INIT_MEM	-12	/* INIT error: when allocate memory */
#define ERR_INIT_CARD	-13	/* INIT error: card not found */
#define ERR_INIT_NOEHM	-14	/* INIT error: Must in 386-Enhance mode */
