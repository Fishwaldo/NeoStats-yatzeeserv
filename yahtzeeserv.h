/* YahtzeeServ - Yahtzee Game Service - NeoStats Addon Module
** Copyright (c) 2003-2005 Justin Hammond, Mark Hetherington, DeadNotBuried
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
extern const char ys_help_start_oneline[];
extern const char ys_help_stop_oneline[];
extern const char ys_help_join_oneline[];
extern const char ys_help_remove_oneline[];
extern const char ys_help_players_oneline[];
extern const char ys_help_turn_oneline[];
extern const char ys_help_pass_oneline[];
extern const char ys_help_dice_oneline[];
extern const char ys_help_roll_oneline[];
extern const char ys_help_keep_oneline[];
extern const char ys_help_score_oneline[];
extern const char ys_help_sheet_oneline[];
extern const char ys_help_top10_oneline[];
extern const char ys_help_high_oneline[];
extern const char ys_help_rules_oneline[];
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

/* channel.c */
int ys_cmd_set_chan (CmdParams *cmdparams, SET_REASON reason);
void CreateChannelGameData(Channel *c);
void RemoveChannelGameData(Channel *c, int fdc);

/* highscores.c */
int ShowHighList (CmdParams* cmdparams);
int ShowTop10Lists (CmdParams* cmdparams);
void loadyahtzeescores(void);
int loadhighscores(void *data, int size);
int sortlistbytypepos( const void *key1, const void *key2 );
int yahtzeeday(void);
int yahtzeeweek(void);
int yahtzeemonth(void);
void clearyahtzeescores(int stc);
void checkhighscorelists(Channel*c);

/* misc.c */
int ShowRulePages (CmdParams* cmdparams);
int PassYahtzeeGame (CmdParams* cmdparams);
int ShowPlayersYahtzeeGame (CmdParams* cmdparams);
int ShowTurnYahtzeeGame (CmdParams* cmdparams);
int ShowYahtzeeSheet (CmdParams* cmdparams);
int ShowYahtzeeDice (CmdParams* cmdparams);
int CheckPlayerPart (CmdParams *cmdparams);
int CheckPlayerKick (CmdParams *cmdparams);
int CheckPlayerQuit (CmdParams *cmdparams);
int CheckPlayerKill (CmdParams *cmdparams);

/* play.c */
int StartYahtzeeGame (CmdParams* cmdparams);
int yahtzeetimer(void);
int JoinYahtzeeGame (CmdParams* cmdparams);
int RemoveYahtzeeGame (CmdParams* cmdparams);
void removenickfromgame(Channel *c, Client *u);
int StopYahtzeeGame (CmdParams* cmdparams);
int ScoreYahtzeeDice (CmdParams* cmdparams);
void RollDice(Channel *c);
int RollYahtzeeDice (CmdParams* cmdparams);
int KeepYahtzeeDice (CmdParams* cmdparams);
void reroll (CmdParams* cmdparams, int rolltype);
