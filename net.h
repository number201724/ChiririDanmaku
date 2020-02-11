#pragma once

#define STAGE_INIT 0
#define STAGE_CONNECTING 1
#define STAGE_CONNECTED 2

#define MAX_PAYLOAD 3990

#pragma pack(push,1)

typedef struct packetheader_s
{
	uint32_t length;
	uint16_t a;
	uint16_t b;
	uint32_t id;
	uint32_t c;
}packetheader_t;
#pragma pack(pop)

#define LIVECMT_DOMAIN "livecmt-2.bilibili.com"
#define LIVECMT_PORT "788"

#include "fifo.h"

class CDanmakuNet
{
public:
	CDanmakuNet( void );
	~CDanmakuNet( void );

	void Init( uint32_t roomid, uint64_t uid );
	void Shutdown( void );
	void Frame( void );
	void Transmit( void );
	void Clear( void );
	void SendPacket( uint32_t id, const char *str );
	void ReadPacket( void );

	void JoinRoom( void );
	void Heartbeat( void );
private:
	bool m_fIsActive;
	uint32_t m_nRoomId;
	uint64_t m_nUID;
	bool m_IsLoadLiveCMT;
	int m_LiveCMTFamily;      // PF_xxx
	int m_LiveCMTSockType;    // SOCK_xxx
	int m_LiveCMTProtocol;    // 0 or IPPROTO_xxx for IPv4 and IPv6
	struct sockaddr_storage m_LiveCMTAddr;
	size_t m_LiveCMTAddrLen;
	int m_nStage;
	SOCKET m_pSock;
	CFifoBuf m_OutgoingMessage;
	unsigned char m_OutgoingMessageBuf[MAX_PAYLOAD];
	CFifoBuf m_IncomingMessage;
	unsigned char m_IncomingMessageBuf[MAX_PAYLOAD * 16];
	unsigned char m_TempBuf[16384];
};

class CNet
{
public:
	CNet( void );
	~CNet( void );
	void Init( void );
	void Shutdown( void );
};


extern CNet gNet;
extern CDanmakuNet gDanmakuNet;
//void
//NET_Init(
//	void
//);
//
//
//void
//NET_Shutdown(
//	void
//);
//
//void
//NET_DanmakuBeginMonitor(
//	uint32_t roomid
//);
//
//
//void
//NET_DanmakuEndMonitor(
//	void
//);
//
//void
//NET_Frame(
//	void
//);