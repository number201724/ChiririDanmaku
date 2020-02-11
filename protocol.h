#pragma once


typedef struct danmaku_info_s
{
	int zero;
	int mode;
	int fontsize;
	uint32_t color;
	json_int_t ts;
	uint32_t rnd;
	int zero2;
	char userhash[32];
	int zero3;
	int type;		// 0  = Normal   1 = Storm	2 = Television
	int bubble;
}danmaku_info_t;

typedef struct danmaku_userinfo_s
{
	json_int_t uid;
	wchar_t nickname[64];
	int is_admin;
	int is_vip;
	int is_svip;
	int rank;
	int phone_validate;
	char nickname_color[16];
}danmaku_userinfo_t;

typedef struct danmaku_medal_s
{
	int level;
	wchar_t medal_name[32];
	wchar_t anchor_name[64];
	uint32_t anchor_id;
	uint32_t color;
	char style[32];
	uint32_t modal_icon_id;
}danmaku_medal_t;

typedef struct danmaku_rank_s
{
	int level;
	int zero;
	uint32_t color;
	char rank[16];
}danmaku_rank_t;

typedef struct danmaku_title_s
{
	char oldname[32];
	char newname[32];
}danmaku_title_t;

typedef struct danmaku_data_s
{
	danmaku_info_t dminfo;
	wchar_t text[256];
	danmaku_userinfo_t userinfo;
	bool has_medal;
	danmaku_medal_t medal;
	danmaku_rank_t rank;
	bool has_title;
	danmaku_title_t title;
	int guard;
}danmaku_data_t;


