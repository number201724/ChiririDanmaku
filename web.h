#pragma once

class CHudTitleLoadRequest : public CWebJsonRequest
{
public:
	CHudTitleLoadRequest( void );
	~CHudTitleLoadRequest( void );

	virtual void OnCompleted( void );
};
