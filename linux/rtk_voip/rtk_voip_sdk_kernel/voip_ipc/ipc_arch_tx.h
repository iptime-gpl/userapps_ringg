#ifndef __IPC_ARCH_TX_H__
#define __IPC_ARCH_TX_H__

//#include "../voip_dsp/rtp/RtpPacket.h"
//#include "../voip_dsp/rtp/RtcpPacket.h"

#include "voip_mgr_define.h"

#include "voip_ipc.h"

extern int ipcSentControlPacket(unsigned short cmd, unsigned int chid, void* pMgr, unsigned short mgrLen,
									mgr_flags_t flags );
extern int ipcSentControlPacketNoChannel(unsigned short cmd, void* pMgr, unsigned short mgrLen,
									mgr_flags_t flags );
extern int ipcSentResponsePacket(unsigned short cmd, unsigned short seq_no, void* pdata, unsigned short data_len);
extern int ipcCheckRespPacket(int cmd, void* pCont, unsigned short* pDsp_id);

extern int ipcSentMirrorPacket(unsigned short cmd, unsigned int host_cch, void* mirror_data, unsigned short mirror_len);

extern int ipcSendDddH2dPacket( uint16 category, ddd_content_t *ddd_cont, uint16 ddd_len );
extern int ipcSendDddD2hPacket( uint16 category, ddd_content_t *ddd_cont, uint16 ddd_len );

// caller is DSP 
struct stRtpPacket;
struct stRtcpPacket;

extern void ipc_RtpTx(/*RtpPacket*/struct stRtpPacket* pst);
extern void ipc_RtcpTx(/*RtcpPacket*/struct stRtcpPacket* pst);
extern void ipc_T38Tx( unsigned int chid, unsigned int sid, void* packet, unsigned int pktLen);

extern int ipc_pkt_tx_final( uint16 category, uint8 protocol, 
						uint8* pdata /* not const */, uint16 data_len, 
						const TstTxPktCtrl* txCtrl, uint16 *psent_seq,
						uint8* pconthdr );
#endif

