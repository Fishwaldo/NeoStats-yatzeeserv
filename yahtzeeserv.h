/* YahtzeeServ - Yahtzee Game Service - NeoStats Addon Module
** Copyright (c) 2003-2005 Justin Hammond, Mark Hetherington, Jeff Lang
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
**  USA
**
** YahtzeeServ CVS Identification
** $Id$
*/

Bot *ys_bot;

/*
 * YahtzeeServ Module Help
*/
extern const char *ys_help_set_exclusions[];
extern const char *ys_help_set_chan[];
extern const char *ys_help_set_multichan[];
extern const char *ys_help_set_chanoponly[];
extern const char *ys_help_set_html[];
extern const char *ys_help_set_htmlpath[];
extern const char *ys_help_start[];
extern const char *ys_help_stop[];
extern const char *ys_help_join[];
extern const char *ys_help_remove[];
extern const char *ys_help_players[];
extern const char *ys_help_turn[];
extern const char *ys_help_pass[];
extern const char *ys_help_dice[];
extern const char *ys_help_roll[];
extern const char *ys_help_keep[];
extern const char *ys_help_score[];
extern const char *ys_help_sheet[];
extern const char *ys_help_top10[];
extern const char *ys_help_high[];
extern const char *ys_help_rules[];
extern const char *ys_help_rules_page_0[];
extern const char *ys_help_rules_page_1[];
extern const char *ys_help_rules_page_2[];
extern const char *ys_help_rules_page_3[];
extern const char *ys_help_rules_page_4[];
extern const char *ys_help_forcehtml[];

/*
 * Defines
*/
#define YS_GAME_STOPPED		0x00000001	/* Game Not Running */
#define YS_GAME_STARTING	0x00000002	/* Game Starting */
#define YS_GAME_PLAYING		0x00000003	/* Game Running */
#define YS_GAME_STOPPING	0x00000004	/* Game stopping */
#define YS_MAX_PLAYERS		0x0000000A	/* Max Players Per Game */
#define YS_HIGH_OVERALL		0x00000001	/* Overall High Score Type */
#define YS_HIGH_DAILY		0x00000002	/* Daily High Score Type */
#define YS_HIGH_WEEKLY		0x00000003	/* Weekly High Score Type */
#define YS_HIGH_MONTHLY		0x00000004	/* Monthly High Score Type */
#define YS_SCORE_DELETE		0x00000001	/* Delete Score From DB */
#define YS_SCORE_ADD		0x00000002	/* Add Score To DB */
#define YS_SCORE_NEW		0x00000003	/* New High Score Entry */

/*
 * Variables
*/
struct YahtzeeServ {
	char yahtzeeroom[MAXCHANLEN];
	int exclusions;
	int multichan;
	int chanoponly;
	int html; 
	char htmlpath[MAXPATH]; 
} YahtzeeServ;

typedef struct Players {
	Client *u;
	int score;
	int hand[15];
} Players;

typedef struct GameData {
	int playercount;
	int gamestatus;
	int timer;
	int currentplayer;
	int currentroll;
	int currenthand;
	int dice[5];
	Players *pd[YS_MAX_PLAYERS];
} GameData;

typedef struct HighScoreData {
	char typepos[6];
	int type;
	int position;
	char name[MAXNICK];
	int score;
	int changed;
} HighScoreData;

extern list_t *gamelist;
extern list_t *highscores;

extern char *ysscoretype[15];
extern int crs;
extern int dtsc[6];
extern char dicetext[5][15];

/*
 * Procedures
*/

/* yahzteeserv.c */
int ys_cmd_set_html( const CmdParams *cmdparams, SET_REASON reason );
int ys_cmd_set_htmlpath( const CmdParams *cmdparams, SET_REASON reason );

/* channel.c */
int ys_cmd_set_chan (const CmdParams *cmdparams, SET_REASON reason);
void CreateChannelGameData(Channel *c);
void RemoveChannelGameData(Channel *c, int fdc);

/* highscores.c */
int ShowHighList (const CmdParams *cmdparams);
int ShowTop10Lists (const CmdParams *cmdparams);
void loadyahtzeescores(void);
int loadhighscores(void *data, int size);
int sortlistbytypepos( const void *key1, const void *key2 );
int yahtzeeday(void *userptr);
int yahtzeeweek(void *userptr);
int yahtzeemonth(void *userptr);
void clearyahtzeescores(int stc);
void checkhighscorelists(Channel*c);

/* misc.c */
int ShowRulePages (const CmdParams *cmdparams);
int PassYahtzeeGame (const CmdParams *cmdparams);
int ShowPlayersYahtzeeGame (const CmdParams *cmdparams);
int ShowTurnYahtzeeGame (const CmdParams *cmdparams);
int ShowYahtzeeSheet (const CmdParams *cmdparams);
int ShowYahtzeeDice (const CmdParams *cmdparams);
int CheckPlayerPart (const CmdParams *cmdparams);
int CheckPlayerKick (const CmdParams *cmdparams);
int CheckPlayerQuit (const CmdParams *cmdparams);
int CheckPlayerKill (const CmdParams *cmdparams);

/* play.c */
int StartYahtzeeGame (const CmdParams *cmdparams);
int yahtzeetimer(void *userptr);
int JoinYahtzeeGame (const CmdParams *cmdparams);
int RemoveYahtzeeGame (const CmdParams *cmdparams);
void removenickfromgame(Channel *c, Client *u);
int StopYahtzeeGame (const CmdParams *cmdparams);
int ScoreYahtzeeDice (const CmdParams *cmdparams);
void RollDice(Channel *c);
int RollYahtzeeDice (const CmdParams *cmdparams);
int KeepYahtzeeDice (const CmdParams *cmdparams);
void reroll (const CmdParams *cmdparams, int rolltype);

/* htmlscores */
void ys_HTMLOutput( void );
int ys_cmd_forcehtml( const CmdParams *cmdparams );
