#pragma once


class CSizeBuf
{
public:
	CSizeBuf( void );
	~CSizeBuf( void );


	void Init( void *pData, size_t nSize );
	void *GetSpace( size_t nSize );
	void Clear( void );
	bool Write( const void *pData, size_t nSize );
	void *GetData( void );
	size_t GetSize( void );
	size_t GetMaxSize( void );

private:
	unsigned char *m_pData;
	size_t m_nCurSize;
	size_t m_nMaxSize;
};

class CMessage
{
public:
	CMessage( void );
	~CMessage( void );

	void Begin( CSizeBuf *pSizeBuf );
	bool End( void );
	void WriteChar( CHAR Value );
	void WriteByte( BYTE Value );
	void WriteWord( WORD Value );
	void WriteLong( ULONG Value );
	void WriteString( LPCSTR lpStr );
	void WriteBlob( CONST PVOID pBlob, SIZE_T nBlobSize );

private:
	CSizeBuf *m_pMsgDest;
	bool m_fHasError;
};

extern CMessage gMsg;

typedef size_t HDXFont;
typedef size_t HDXSprite;

#define charsmax(STRING) (ARRAYSIZE(STRING)-1)