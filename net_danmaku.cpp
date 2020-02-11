#include "pch.h"
#include "common.h"
#include "sys.h"
#include "net.h"
#include "client.h"

CDanmakuNet gDanmakuNet;

CDanmakuNet::CDanmakuNet( void ) :
	m_OutgoingMessage( m_OutgoingMessageBuf, sizeof( m_OutgoingMessageBuf ) ),
	m_IncomingMessage( m_IncomingMessageBuf, sizeof( m_IncomingMessageBuf ) )
{
	m_pSock = SOCKET_ERROR;
}

CDanmakuNet::~CDanmakuNet( void )
{

}

void CDanmakuNet::Init( uint32_t roomid, uint64_t uid )
{
	m_nRoomId = roomid;
	m_nUID = 480731;
	m_fIsActive = true;
}

void CDanmakuNet::Shutdown( void )
{
	Clear( );
	m_fIsActive = false;
}

void CDanmakuNet::Frame( void )
{
	struct addrinfo hints;
	struct addrinfo *results;
	struct timeval timeout;
	struct fd_set readfds, writefds, exceptfds;
	int code;

	if( !m_fIsActive )
		return;

	if( !m_IsLoadLiveCMT )
	{
		memset( &hints, 0, sizeof( struct addrinfo ) );
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		results = NULL;

		if( 0 == getaddrinfo( LIVECMT_DOMAIN, LIVECMT_PORT, &hints, &results ) )
		{
			memcpy( &m_LiveCMTAddr, results->ai_addr, results->ai_addrlen );
			m_LiveCMTAddrLen = results->ai_addrlen;
			m_LiveCMTFamily = results->ai_family;
			m_LiveCMTProtocol = results->ai_protocol;
			m_LiveCMTSockType = results->ai_socktype;
			freeaddrinfo( results );

			m_IsLoadLiveCMT = true;
		}
	}

	if( m_IsLoadLiveCMT && m_nStage == STAGE_INIT )
	{
		m_pSock = socket( m_LiveCMTFamily, m_LiveCMTSockType, m_LiveCMTProtocol );
		if( m_pSock == SOCKET_ERROR )
			return;

		u_long mode = 1;
		ioctlsocket( m_pSock, FIONBIO, &mode );

		code = connect( m_pSock, (const sockaddr *)&m_LiveCMTAddr, (int)m_LiveCMTAddrLen );

		if( code && WSAGetLastError( ) != WSAEWOULDBLOCK )
		{
			closesocket( m_pSock );
			m_pSock = SOCKET_ERROR;
			return;
		}

		m_OutgoingMessage.clear( );
		m_IncomingMessage.clear( );

		m_nStage = STAGE_CONNECTING;
	}


	if( m_nStage == STAGE_INIT )
		return;

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	FD_ZERO( &readfds );
	FD_ZERO( &writefds );
	FD_ZERO( &exceptfds );

	FD_SET( m_pSock, &readfds );
	FD_SET( m_pSock, &writefds );
	FD_SET( m_pSock, &exceptfds );

	code = select( (int)m_pSock + 1, &readfds, &writefds, &exceptfds, &timeout );

	if( code <= 0 )
		return;

	if( FD_ISSET( m_pSock, &exceptfds ) )
	{
		Clear( );
		return;
	}

	if( FD_ISSET( m_pSock, &writefds ) )
	{
		if( m_nStage != STAGE_CONNECTED )
		{
			m_nStage = STAGE_CONNECTED;
			JoinRoom( );
		}

		Transmit( );
	}

	if( FD_ISSET( m_pSock, &readfds ) )
	{
		ReadPacket( );
	}
}

void CDanmakuNet::Transmit( void )
{
	int len;
	char buffer[1024];
	
	if( m_OutgoingMessage.size( ) == 0 )
		return;

	len = (int)min( sizeof( buffer ), m_OutgoingMessage.size( ) );
	m_OutgoingMessage.front( buffer, len );

	len = send( m_pSock, buffer, len, 0 );

	if( len <= 0 )
	{
		Clear( );
		return;
	}

	m_OutgoingMessage.pop( len );
}

void CDanmakuNet::Clear( void )
{
	m_nStage = STAGE_INIT;
	if( m_pSock != SOCKET_ERROR )
	{
		closesocket( m_pSock );
		m_pSock = SOCKET_ERROR;
	}
}


void CDanmakuNet::SendPacket( uint32_t id, const char *str )
{
	unsigned char message_buf[1024];
	CSizeBuf message;

	message.Init( message_buf, sizeof( message_buf ) );

	gMsg.Begin( &message );

	if( str == NULL )
		gMsg.WriteLong( sizeof( packetheader_t ) );
	else
		gMsg.WriteLong( (unsigned int)(sizeof( packetheader_t ) + strlen( str )) );

	gMsg.WriteWord( 16 );
	gMsg.WriteWord( 1 );
	gMsg.WriteLong( id );
	gMsg.WriteLong( 1 );

	if( str )
		gMsg.WriteString( str );

	

	if( gMsg.End( ) || !m_OutgoingMessage.queue( message.GetData( ), message.GetSize( ) ) )
	{
		Clear( );
		return;
	}
}

void CDanmakuNet::ReadPacket( void )
{
	char buffer[4096];
	json_error_t jerr;
	int len;
	packetheader_t header;

	while( true )
	{
		len = recv( m_pSock, buffer, sizeof( buffer ), 0 );

		if( len == 0 )
		{
			Clear( );
			return;
		}

		if( len == -1 )
		{
			if( WSAGetLastError( ) == WSAEWOULDBLOCK )
				return;

			Clear( );
			return;
		}

		if( !m_IncomingMessage.queue( buffer, len ) )
		{
			Clear( );
			return;
		}

		while( m_IncomingMessage.size( ) > sizeof( packetheader_t ) )
		{
			m_IncomingMessage.front( &header, sizeof( header ) );
			header.length = htonl( header.length );
			header.id = htonl( header.id );

			if( m_IncomingMessage.size( ) < header.length )
				return;

			if( header.id == 4 )
			{
				m_IncomingMessage.pop( header.length );
				continue;
			}

			if( header.id != 5 )
			{
				m_IncomingMessage.pop( header.length );
				continue;
			}

			if( header.length < sizeof( m_TempBuf ) )
			{
				m_IncomingMessage.deque( m_TempBuf, header.length );
				uint32_t bufferlen = header.length - sizeof( header );
				json_t *object = json_loadb( (char *)m_TempBuf + sizeof( header ), bufferlen, 0, &jerr );

				if( object == NULL )
					continue;

				cl.ParseServerMessage( object );

				json_decref( object );
			}
			else
			{
				m_IncomingMessage.pop( header.length );
			}
		}
	}
	
}


void CDanmakuNet::JoinRoom( void )
{
	char buffer[1024];
	size_t buffer_len;

	json_t *json = json_pack( "{s:i,s:i}", "roomid", m_nRoomId, "uid", m_nUID );
	buffer_len = json_dumpb( json, buffer, sizeof( buffer ), 0 );
	buffer[buffer_len] = 0;

	SendPacket( 7, buffer );

	json_decref( json );
}

void CDanmakuNet::Heartbeat( void )
{
	SendPacket( 2, NULL );
}