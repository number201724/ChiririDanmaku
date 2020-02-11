#include "pch.h"
#include "common.h"
#include "render.h"
#include "sprite.h"

extern IDirect3DDevice9Ex *g_pd3dDevice;

CDXSprite::CDXSprite( CString szFileName, int width, int height, Color transparentColorKey )
{
	m_pSprite = nullptr;
	m_pTexture = nullptr;
	m_TextureFile = szFileName;
	m_nSpriteWidth = width;
	m_nSpriteHeight = height;
	m_TransparentColorKey = transparentColorKey;
}

CDXSprite::~CDXSprite( )
{
	if( m_pSprite )
	{
		m_pSprite->Release( );
		m_pSprite = nullptr;
	}

	if( m_pTexture )
	{
		m_pTexture->Release( );
		m_pTexture = nullptr;
	}
}

void CDXSprite::PreFrame( )
{
	if( !m_pSprite )
	{
		D3DXCreateSprite( g_pd3dDevice, &m_pSprite );
	}

	if( m_pSprite )
	{
		m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
	}

	if( !m_pTexture )
	{
		D3DXCreateTextureFromFileEx( g_pd3dDevice,
									 m_TextureFile,
									 m_nSpriteWidth,
									 m_nSpriteHeight,
									 D3DX_DEFAULT,
									 NULL,
									 D3DFMT_A8R8G8B8,
									 D3DPOOL_DEFAULT,
									 D3DX_DEFAULT,
									 D3DX_DEFAULT,
									 NULL,
									 NULL,
									 NULL,
									 &m_pTexture
		);
	}
}

void CDXSprite::PostFrame( )
{
	if( m_pSprite )
	{
		m_pSprite->End( );
	}
}

void CDXSprite::OnReset( )
{
	if( m_pSprite )
	{
		m_pSprite->OnLostDevice( );
		m_pSprite->OnResetDevice( );
	}
}

void CDXSprite::DrawSprite( float x, float y, float scale )
{
	if( !m_pTexture )
		return;

	D3DXVECTOR2 spriteCentre = D3DXVECTOR2( (m_nSpriteWidth * scale) / 2, (m_nSpriteHeight * scale) / 2 );
	D3DXVECTOR2 trans = D3DXVECTOR2( x - spriteCentre.x, y - spriteCentre.y );
	D3DXMATRIX mat;
	D3DXVECTOR2 scaling( scale, scale );
	D3DXMatrixTransformation2D( &mat, NULL, 0.0, &scaling, &spriteCentre, NULL, &trans );

	m_pSprite->SetTransform( &mat );

	m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, Color::White( ).Code( ) );
}

void CDXSprite::DrawGeometricTexture( float x, float y, float radius, int geometric )
{
	std::vector<CUSTOMVERTEX_TEX> vCircle( geometric + 2 );
	if( !m_pTexture )
		return;
	vCircle[0].xyzrhw.x = x;
	vCircle[0].xyzrhw.y = y;
	vCircle[0].xyzrhw.z = 0;
	vCircle[0].xyzrhw.w = 1;
	vCircle[0].color = m_TransparentColorKey.Code( );
	vCircle[0].tu = (FLOAT)0.5f;
	vCircle[0].tv = (FLOAT)0.5f;

	for( int i = 1; i < geometric + 2; i++ )
	{
		float v = D3DX_PI * ((i - 1) / (geometric / 2.0f));

		vCircle[i].xyzrhw.x = (float)(x - radius * cos( v ));
		vCircle[i].xyzrhw.y = (float)(y - radius * sin( v ));
		vCircle[i].xyzrhw.z = 0;
		vCircle[i].xyzrhw.w = 1;
		vCircle[i].color = m_TransparentColorKey.Code( );
		vCircle[i].tu = (FLOAT)(1.0 - (cos( v ) + 1.0) * 0.5);
		vCircle[i].tv = (FLOAT)(1.0 - ((sin( v ) + 1.0) * 0.5));
	}

	IDirect3DVertexBuffer9 *pVertexBuffer = NULL;

	g_pd3dDevice->CreateVertexBuffer( geometric + 2 * sizeof( CUSTOMVERTEX_TEX ), 0,
									  D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT,
									  &pVertexBuffer, NULL );

	CUSTOMVERTEX_TEX *pVertices;
	pVertexBuffer->Lock( 0, geometric + 2 * sizeof( CUSTOMVERTEX_TEX ), (void **)&pVertices, 0 );
	memcpy( pVertices, &vCircle[0], (geometric + 2) * sizeof( CUSTOMVERTEX_TEX ) );
	pVertexBuffer->Unlock( );


	g_pd3dDevice->SetTexture( 0, m_pTexture );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	g_pd3dDevice->SetStreamSource( 0, pVertexBuffer, 0, sizeof( CUSTOMVERTEX_TEX ) );
	g_pd3dDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
	g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, geometric );
	g_pd3dDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	if( pVertexBuffer != NULL ) pVertexBuffer->Release( );
}