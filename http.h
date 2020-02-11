#pragma once

class CWebRequest
{
public:
	CWebRequest( void );
	~CWebRequest( void );

	void SetUrl( const char *szUrl );
	void SetPost( bool Post );
	void SetTimeout( int nTimeout = 15 );
	void SetConnectTimeout( int nTimeout = 15 );
	
	virtual void OnCompleted( void );
	virtual void OnDone( void );
protected:
	virtual size_t OnWrite( void *buffer, size_t size, size_t nmemb );
private:
	static size_t OnWriteStaticProc( void *buffer, size_t size, size_t nmemb, void *pdata );

public:
	inline CURL *GetHandle( void ) {
		return m_pCurl;
	}
protected:
	CURL *m_pCurl;
};

class CWebJsonRequest : public CWebRequest
{
public:
	CWebJsonRequest( void );
	~CWebJsonRequest( void );

protected:
	virtual size_t OnWrite( void *buffer, size_t size, size_t nmemb );
	virtual void OnDone( void );
protected:
	CStringA m_strJsonData;
	json_t *m_pJsonRoot;
	json_error_t m_JsonErr;
};

class CWebFileDownloadRequest : public CWebRequest
{
public:
	CWebFileDownloadRequest( void );
	~CWebFileDownloadRequest( void );
};

class CHttp
{
public:
	CHttp( void );
	~CHttp( void );

	void Init( void );
	void Shutdown( void );
	void Frame( void );

	int GetPendingRequestCount( void );
	bool Run( CWebRequest *pRequest );
private:
	CURLM *m_pCurlM;
	INT m_nPendingRequests;
	CArray<CWebRequest *> m_vecRequests;
};




extern CHttp gHttp;