#include "pch.h"
#include "common.h"
#include "sys.h"
#include "http.h"

CHttp gHttp;


CHttp::CHttp( void )
{
	m_pCurlM = NULL;
	m_nPendingRequests = 0;
}

CHttp::~CHttp( void )
{

}

void CHttp::Init( void )
{
	curl_global_init( CURL_GLOBAL_ALL );

	m_pCurlM = curl_multi_init( );
	if( m_pCurlM == NULL )
	{
		gSys.Error( TEXT( "curl_multi_init init failed." ) );
	}

	m_nPendingRequests = 0;
}

void CHttp::Shutdown( void )
{
	if( m_pCurlM )
	{
		curl_multi_cleanup( m_pCurlM );
		m_pCurlM = NULL;
	}

	m_nPendingRequests = 0;
	curl_global_cleanup( );
}


void CHttp::Frame( void )
{
	int  msgs_left;
	CURLMsg *pMsg;

	for( ;;)
	{
		if( CURLM_CALL_MULTI_PERFORM != curl_multi_perform( m_pCurlM, &m_nPendingRequests ) )
			break;
	}

	while( (pMsg = curl_multi_info_read( m_pCurlM, &msgs_left )) )
	{
		CWebRequest *pRequest = NULL;
		if( CURLMSG_DONE != pMsg->msg )
			continue;

		curl_multi_remove_handle( m_pCurlM, pMsg->easy_handle );

		curl_easy_getinfo( pMsg->easy_handle, CURLINFO_PRIVATE,&pRequest );

		if( pRequest )
		{
			pRequest->OnDone( );
		}
	}
}

bool CHttp::Run( CWebRequest *pRequest )
{
	if( !curl_multi_add_handle( m_pCurlM, pRequest->GetHandle( ) ) )
	{
		m_vecRequests.Add( pRequest );
		return true;
	}
	
	return false;
}

int CHttp::GetPendingRequestCount( void )
{
	return m_nPendingRequests;
}

CWebRequest::CWebRequest( void )
{
	m_pCurl = curl_easy_init( );
	curl_easy_setopt( m_pCurl, CURLOPT_FOLLOWLOCATION, 1 );
	curl_easy_setopt( m_pCurl, CURLOPT_PRIVATE, this );
	curl_easy_setopt( m_pCurl, CURLOPT_VERBOSE, 0 );
	curl_easy_setopt( m_pCurl, CURLOPT_HEADER, 0 );
	curl_easy_setopt( m_pCurl, CURLOPT_NOSIGNAL, 1 );
	curl_easy_setopt( m_pCurl, CURLOPT_WRITEDATA, this );
	curl_easy_setopt( m_pCurl, CURLOPT_WRITEFUNCTION, OnWriteStaticProc );
}

CWebRequest::~CWebRequest( void )
{
	curl_easy_cleanup( m_pCurl );
}

void CWebRequest::SetUrl( const char *szUrl )
{
	curl_easy_setopt( m_pCurl, CURLOPT_URL, szUrl );
}

void CWebRequest::SetPost( bool Post )
{
	curl_easy_setopt( m_pCurl, CURLOPT_POST, (int)Post );
}

void CWebRequest::SetTimeout( int nTimeout )
{
	curl_easy_setopt( m_pCurl, CURLOPT_TIMEOUT, nTimeout );
}

void CWebRequest::SetConnectTimeout( int nTimeout )
{
	curl_easy_setopt( m_pCurl, CURLOPT_CONNECTTIMEOUT, nTimeout );
}

size_t CWebRequest::OnWriteStaticProc( void *buffer, size_t size, size_t nmemb, void *pdata )
{
	CWebRequest *pRequest = (CWebRequest *)pdata;

	return pRequest->OnWrite( buffer, size, nmemb );
}

void CWebRequest::OnCompleted( void )
{

}

void CWebRequest::OnDone( void )
{
	OnCompleted( );
	delete this;
}

size_t CWebRequest::OnWrite( void *buffer, size_t size, size_t nmemb )
{
	return size * nmemb;
}

CWebJsonRequest::CWebJsonRequest( void )
{
	m_pJsonRoot = NULL;
}

CWebJsonRequest::~CWebJsonRequest( void )
{
	if( m_pJsonRoot != NULL )
	{
		json_decref( m_pJsonRoot );
		m_pJsonRoot = NULL;
	}
}

size_t CWebJsonRequest::OnWrite( void *buffer, size_t size, size_t nmemb )
{
	m_strJsonData.Append( (LPCSTR)buffer, (int)(size * nmemb) );
	return size * nmemb;
}

void CWebJsonRequest::OnDone( void )
{
	m_pJsonRoot = json_loads( m_strJsonData, 0, &m_JsonErr );

	OnCompleted( );
	delete this;
}

CWebFileDownloadRequest::CWebFileDownloadRequest( void )
{

}

CWebFileDownloadRequest::~CWebFileDownloadRequest( void )
{

}