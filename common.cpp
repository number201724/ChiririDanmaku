#include "pch.h"
#include "common.h"
#include "sys.h"

CMessage gMsg;

CSizeBuf::CSizeBuf( void )
{
	m_pData = NULL;
	m_nMaxSize = 0;
	m_nCurSize = 0;
}

CSizeBuf::~CSizeBuf( void )
{

}


void CSizeBuf::Init( void *pData, size_t nSize )
{
	m_pData = (unsigned char *)pData;
	m_nMaxSize = nSize;
	m_nCurSize = 0;
}

void CSizeBuf::Clear( void )
{
	m_nCurSize = 0;
}

void *CSizeBuf::GetSpace( size_t nSize )
{
	void *pData;

	if( m_nCurSize + nSize > m_nMaxSize)
	{
		return NULL;
	}

	pData = m_pData + m_nCurSize;
	m_nCurSize += nSize;

	return pData;
}

bool CSizeBuf::Write( const void *pData, size_t nSize )
{
	void *pBuf = GetSpace( nSize );
	if( pBuf == NULL )
		return false;

	memcpy( pBuf, pData, nSize );

	return true;
}

void *CSizeBuf::GetData( void )
{
	return m_pData;
}

size_t CSizeBuf::GetSize( void )
{
	return m_nCurSize;
}

size_t CSizeBuf::GetMaxSize( void )
{
	return m_nMaxSize;
}

CMessage::CMessage( void )
{
	m_fHasError = false;
	m_pMsgDest = NULL;
}

CMessage::~CMessage( void )
{

}

void CMessage::Begin( CSizeBuf *pSizeBuf )
{
	m_fHasError = false;
	m_pMsgDest = pSizeBuf;
}

bool CMessage::End( void )
{
	m_pMsgDest = NULL;
	return m_fHasError;
}

void CMessage::WriteChar( CHAR Value )
{
	if( m_pMsgDest == NULL )
	{
		gSys.Error( TEXT( "CMessage::m_pMsgDest == NULL" ) );
	}
	
	if( !m_pMsgDest->Write( &Value, sizeof( Value ) ) )
	{
		m_fHasError = true;
	}
}

void CMessage::WriteByte( BYTE Value )
{
	if( m_pMsgDest == NULL )
	{
		gSys.Error( TEXT( "CMessage::m_pMsgDest == NULL" ) );
	}

	if( !m_pMsgDest->Write( &Value, sizeof( Value ) ) )
	{
		m_fHasError = true;
	}
}

void CMessage::WriteWord( WORD Value )
{
	if( m_pMsgDest == NULL )
	{
		gSys.Error( TEXT( "CMessage::m_pMsgDest == NULL" ) );
	}

	Value = htons( Value );

	if( !m_pMsgDest->Write( &Value, sizeof( Value ) ) )
	{
		m_fHasError = true;
	}
}

void CMessage::WriteLong( ULONG Value )
{
	if( m_pMsgDest == NULL )
	{
		gSys.Error( TEXT( "CMessage::m_pMsgDest == NULL" ) );
	}

	Value = htonl( Value );

	if( !m_pMsgDest->Write( &Value, sizeof( Value ) ) )
	{
		m_fHasError = true;
	}
}

void CMessage::WriteString( LPCSTR lpStr )
{
	if( m_pMsgDest == NULL )
	{
		gSys.Error( TEXT( "CMessage::m_pMsgDest == NULL" ) );
	}

	
	if( !m_pMsgDest->Write( lpStr, strlen( lpStr ) ) )
	{
		m_fHasError = true;
	}
}

void CMessage::WriteBlob( CONST PVOID pBlob, SIZE_T nBlobSize )
{
	if( m_pMsgDest == NULL )
	{
		gSys.Error( TEXT( "CMessage::m_pMsgDest == NULL" ) );
	}


	if( !m_pMsgDest->Write( pBlob, nBlobSize ) )
	{
		m_fHasError = true;
	}
}

