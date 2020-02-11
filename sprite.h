#pragma once


class CDXSprite
{
public:
	CDXSprite( CString szFileName, int nWidth, int nHeight, Color transparentColorKey );
	~CDXSprite( );

	void				PreFrame( );
	void				PostFrame( );

	void				OnReset( );

	void				DrawSprite( float x, float y, float scale );
	void				DrawGeometricTexture( float x, float y, float radius, int geometric );

private:
	ID3DXSprite			*m_pSprite;
	IDirect3DTexture9	*m_pTexture;

	CString				m_TextureFile;
	int					m_nSpriteHeight;
	int					m_nSpriteWidth;
	Color				m_TransparentColorKey;
};