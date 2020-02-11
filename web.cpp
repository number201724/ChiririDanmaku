#include "pch.h"
#include "common.h"
#include "sys.h"
#include "http.h"
#include "web.h"

CHudTitleLoadRequest::CHudTitleLoadRequest( void )
{

}

CHudTitleLoadRequest::~CHudTitleLoadRequest( void )
{

}

void CHudTitleLoadRequest::OnCompleted( void )
{
	json_t *pJsonData = NULL;
	int code;
	if( m_pJsonRoot == NULL )
	{
		return;
	}
	code = json_unpack( m_pJsonRoot, "{s:o}", "data", &pJsonData );

	if( code )
	{
		return;
	}

	if( json_typeof( pJsonData ) != JSON_ARRAY ) 
	{
		return;
	}

	size_t count = json_array_size( pJsonData );

	for( size_t i = 0; i < count; i++ )
	{
		const char *web_pic_url;
		const char *identification;
		const char *name;
		const char *title_id;


		json_t *jData = json_array_get( pJsonData, i );

		code = json_unpack( jData, "{s:s,s:s,s:s,s:s}",
					 "identification", &identification,
					 "name", &name,
					 "web_pic_url", &web_pic_url,
					 "title_id", &title_id 
		);

		if( code)
			continue;

		CStringA DumpS;

//		DumpS.Format( "identification:%s\n", identification );
//		OutputDebugStringA( DumpS );

		//4634167
	}
}