/* YahtzeeServ - Yahtzee Game Service - NeoStats Addon Module
** Copyright (c) 2003-2005 DeadNotBuried
** Portions Copyright (c) 1999-2005, NeoStats
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
extern const char *ys_help_set_chan[];
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

/*
 * Defines
*/
#define YS_GAME_STOPPED		0x00000001	/* Game Not Running */
#define YS_GAME_STARTING	0x00000002	/* Game Starting */
#define YS_GAME_PLAYING		0x00000003	/* Game Running */
#define YS_GAME_STOPPING	0x00000004	/* Game Running */

/*
 * Variables
*/
extern char yahtzeeroom[MAXCHANLEN];
extern char ysplayernick[10][MAXNICK];
extern int ysplayerscore[10];
extern char *ysscoretype[15];
extern int ysplayerhand[10][15];
extern char *ysplayerhandused[10][15];
extern int ypln;
extern int yplnh;
extern int currentyahtzeeplayercount;
extern int currentyahtzeegamestatus;
extern int currentplayer;
extern int currentroll;
extern int currenthand;
extern int ydie[5];
extern char *scoretext[100];
extern int scoretextargs;
extern char *tmpstr;
extern int crs;
extern int htsslrn;
extern int htsslrold;
extern char htsslrnickold[MAXNICK];
extern int htsslr[100];
extern char htsslrnick[100][MAXNICK];
extern int dhtsslr[10];
extern char dhtsslrnick[10][MAXNICK];
extern int whtsslr[10];
extern char whtsslrnick[10][MAXNICK];
extern int mhtsslr[10];
extern char mhtsslrnick[10][MAXNICK];
extern int dtsc[6];

/*
 * Procedures
*/
int PlayerNickChange (CmdParams* cmdparams);
int StartYahtzeeGame (CmdParams* cmdparams);
int StopYahtzeeGame (CmdParams* cmdparams);
int JoinYahtzeeGame (CmdParams* cmdparams);
int RemoveYahtzeeGame (CmdParams* cmdparams);
int ShowPlayersYahtzeeGame (CmdParams* cmdparams);
int ShowTurnYahtzeeGame (CmdParams* cmdparams);
int PassYahtzeeGame (CmdParams* cmdparams);
int ShowYahtzeeDice (CmdParams* cmdparams);
int ShowYahtzeeSheet (CmdParams* cmdparams);
int RollYahtzeeDice (CmdParams* cmdparams);
int KeepYahtzeeDice (CmdParams* cmdparams);
int ScoreYahtzeeDice (CmdParams* cmdparams);
int ShowTop10Lists (CmdParams* cmdparams);
int ShowHighList (CmdParams* cmdparams);
int ShowRulePages (CmdParams* cmdparams);
static int ys_cmd_set_chan (CmdParams *cmdparams, SET_REASON reason);
void stopyahtzee(void);
void yrolldie(void);
void getdieroll(void);
void yscore(char **argvy, int argcy);
void reroll(char **argvy, int argcy, char *rolltype);
void startcountdowntimer(char *nic);
int startyahtzee(void);
void scoredie(int sconum);
void scorefh(void);
void scoress(void);
void scorels(void);
void score3k(void);
void score4k(void);
void scorec(void);
void scorey(void);
void loadyahtzeescores(void);
int yahtzeeday(void);
int yahtzeeweek(void);
int yahtzeemonth(void);
void clearyahtzeedaily(void);
void clearyahtzeeweekly(void);
void clearyahtzeemonthly(void);
void saveyahtzeedaily(void);
void saveyahtzeeweekly(void);
void saveyahtzeemonthly(void);
void saveyahtzeeoverall(void);
