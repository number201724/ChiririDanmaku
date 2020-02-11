#pragma once


class client_t
{
public:
	void ParseServerMessage( json_t *object );
};

extern client_t cl;

void
CL_ParseDanmakuMsg( 
	json_t *object
);

void
CL_ParseSendGift(
	json_t *object
);


void
CL_ParseSuperChat(
	json_t *object
);

void
CL_ParseGuardBuy(
	json_t *object
);