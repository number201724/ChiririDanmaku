#include "pch.h"
#include "common.h"
#include "sys.h"
#include "protocol.h"
#include "client.h"
#include "hud.h"


client_t cl;

void client_t::ParseServerMessage( json_t *object )
{
	const char *cmd;
	if( json_unpack( object, "{s:s}", "cmd", &cmd ) )
		return;


	if( !strncmp( cmd, "DANMU_MSG", charsmax( "DANMU_MSG" ) ) )
	{
		CL_ParseDanmakuMsg( object );
		return;
	}

	if( !strncmp( cmd, "SEND_GIFT", charsmax( "SEND_GIFT" ) ) )
	{
		CL_ParseSendGift( object );
		return;
	}

	if( !strncmp( cmd, "GUARD_BUY", charsmax( "GUARD_BUY" ) ) )
	{
		CL_ParseGuardBuy( object );
		return;
	}

	if( !strncmp( cmd, "SUPER_CHAT_MESSAGE", charsmax( "SUPER_CHAT_MESSAGE" ) ) )
	{
		CL_ParseSuperChat( object );
		return;
	}

	OutputDebugStringA( cmd );
	OutputDebugStringA( "\n" );
}

extern INT g_nCount;
//-----------------------------------------------------------------------------
// Purpose:
// Input  : 
// Output : 
//-----------------------------------------------------------------------------
void
CL_ParseDanmakuMsg(
	json_t *object
)
{
	danmaku_data_t dmk;
	json_t *j_dmkinfo, *j_userinfo, *j_medal, *j_rank, *j_title;
	const char *text;
	int code;

	memset( &dmk, 0, sizeof( dmk ) );

	code = json_unpack( object, "{s:[o,s,o,o,o,o,,i]}",
						"info",
						&j_dmkinfo,
						&text,
						&j_userinfo,
						&j_medal,
						&j_rank,
						&j_title,
						&dmk.guard
	);

	if( code )
		return;


	if( j_dmkinfo )
	{
		const char *userhash;

		code = json_unpack( j_dmkinfo, "[i,i,i,i,I,i,i,s,i,i,i]",
							&dmk.dminfo.zero,
							&dmk.dminfo.mode,
							&dmk.dminfo.fontsize,
							&dmk.dminfo.color,
							&dmk.dminfo.ts,
							&dmk.dminfo.rnd,
							&dmk.dminfo.zero2,
							&userhash,
							&dmk.dminfo.zero3,
							&dmk.dminfo.type,
							&dmk.dminfo.bubble );

		if( code )
		{
			return;
		}

		if( userhash )
		{
			strncpy_s( dmk.dminfo.userhash, userhash, charsmax( dmk.dminfo.userhash ) );
			dmk.dminfo.userhash[charsmax( dmk.dminfo.userhash )] = 0;
		}
	}
	

	if( text )
	{
		MultiByteToWideChar( CP_UTF8, 0, text, -1, dmk.text, charsmax( dmk.text ) );
		dmk.text[charsmax( dmk.text ) - 1] = 0;
	}

	if( j_userinfo )
	{
		const char *nickname;
		const char *nickcolor;

		code = json_unpack( j_userinfo, "[I,s,i,i,i,i,i,s]",
							&dmk.userinfo.uid,
							&nickname,
							&dmk.userinfo.is_admin,
							&dmk.userinfo.is_vip,
							&dmk.userinfo.is_svip,
							&dmk.userinfo.rank,
							&dmk.userinfo.phone_validate,
							&nickcolor);

		if( code )
		{
			return;
		}

		if( nickname )
		{
			MultiByteToWideChar( CP_UTF8, 0, nickname, -1, dmk.userinfo.nickname, charsmax( dmk.userinfo.nickname ) );
			dmk.userinfo.nickname[charsmax( dmk.userinfo.nickname ) - 1] = 0;
		}

		if( nickcolor )
		{
			strncpy_s( dmk.userinfo.nickname_color, nickcolor, charsmax( dmk.userinfo.nickname_color ) );
			dmk.userinfo.nickname_color[charsmax( dmk.userinfo.nickname_color )] = 0;
		}
	}

	if( j_medal )
	{
		if( json_array_size( j_medal )  != 0 )
		{
			const char *medal_name;
			const char *anchor_name;
			const char *style;
			code = json_unpack( j_medal, "[i,s,s,i,i,s,i]",
								&dmk.medal.level,
								&medal_name,
								&anchor_name,
								&dmk.medal.anchor_id,
								&dmk.medal.color,
								&style,
								&dmk.medal.modal_icon_id
			);

			if( code )
				return;

			if( medal_name )
			{
				MultiByteToWideChar( CP_UTF8, 0, medal_name, -1, dmk.medal.medal_name, charsmax( dmk.medal.medal_name ) );
				dmk.medal.medal_name[charsmax( dmk.medal.medal_name ) - 1] = 0;
			}

			if( anchor_name )
			{
				MultiByteToWideChar( CP_UTF8, 0, anchor_name, -1, dmk.medal.anchor_name, charsmax( dmk.medal.anchor_name ) );
				dmk.medal.anchor_name[charsmax( dmk.medal.anchor_name ) - 1] = 0;
			}

			if( style )
			{
				strncpy_s( dmk.medal.style, style, charsmax( dmk.medal.style ) );
				dmk.medal.style[charsmax( dmk.medal.style )] = 0;
			}

			dmk.has_medal = true;
		}
		else
		{
			dmk.has_medal = false;
		}
	}

	if( j_rank )
	{
		json_t *rank;
		code = json_unpack( j_rank, "[i,i,i,o]",
							&dmk.rank.level,
							&dmk.rank.zero,
							&dmk.rank.color,
							&rank
		);


		if( code )
		{
			OutputDebugStringA( json_dumps( j_rank, 0 ) );
			return;
		}

		if( rank && json_typeof( rank ) == JSON_STRING )
		{
			strncpy_s( dmk.rank.rank, json_string_value( rank ), charsmax( dmk.rank.rank ) );
			dmk.rank.rank[charsmax( dmk.rank.rank )] = 0;
		}

		if( rank && json_typeof( rank ) == JSON_INTEGER )
		{
			_snprintf_s( dmk.rank.rank, charsmax( dmk.rank.rank ), "%d",  (int)json_integer_value( rank ) );
			dmk.rank.rank[charsmax( dmk.rank.rank )] = 0;
		}
	}

	if( j_title )
	{
		if( json_array_size( j_title ) != 0 )
		{
			const char *oldname, *newname;
			
			code = json_unpack( j_title, "[s,s]",
								&oldname,
								&newname 
			);

			if( code )
				return;

			if( oldname )
			{
				strncpy_s( dmk.title.oldname, oldname, charsmax( dmk.title.oldname ) );
				dmk.title.oldname[charsmax( dmk.title.oldname )] = 0;
			}

			if( newname )
			{
				strncpy_s( dmk.title.newname, newname, charsmax( dmk.title.newname ) );
				dmk.title.newname[charsmax( dmk.title.newname )] = 0;
			}

			dmk.has_title = true;
		}
		else
		{
			dmk.has_title = false;
		}
	}
	g_nCount++;
	gHUD.AddDanmaku( &dmk );
}


//-----------------------------------------------------------------------------
// Purpose:
// Input  : 
// Output : 
//-----------------------------------------------------------------------------
void
CL_ParseSendGift(
	json_t *object
)
{
	const char *giftName;
	int num;
	const char *uname;
	const char *face;
	int guard_level;
	json_int_t uid;
	json_int_t timestamp;
	int giftId;
	int giftType;

	int code;

	code = json_unpack( object, "{s:{s:s,s:i,s:s,s:s,s:i,s:I,s:I,s:i,s:i}}", "data",
						"giftName", &giftName,
						"num", &num,
						"uname", &uname,
						"face", &face,
						"guard_level", &guard_level,
						"uid", &uid,
						"timestamp", &timestamp,
						"giftId", &giftId,
						"giftType", &giftType
	);

	if( code )
		return;

//	OutputDebugStringA( json_dumps( object, 0 ) );
//	OutputDebugStringA( "\n" );
}


//-----------------------------------------------------------------------------
// Purpose:
// Input  : 
// Output : 
//-----------------------------------------------------------------------------
void
CL_ParseSuperChat(
	json_t *object
)
{
//	OutputDebugStringA( json_dumps( object, 0 ) );
//	OutputDebugStringA( "\n" );
}


//-----------------------------------------------------------------------------
// Purpose:
// Input  : 
// Output : 
//-----------------------------------------------------------------------------
void
CL_ParseGuardBuy(
	json_t *object
)
{
//	OutputDebugStringA( json_dumps( object, 0 ) );
//	OutputDebugStringA( "\n" );
}