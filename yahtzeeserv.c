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

#include "neostats.h"    /* Required for bot support */
#include "yahtzeeserv.h"

/** Copyright info */
const char *ys_copyright[] = {
	"Copyright (c) 2004-2005 DeadNotBuried",
	"Portions Copyright (c) 1999-2005, NeoStats",
	NULL
};

const char *ys_about[] = {
	"\2Yahtzee Game Service\2",
	"",
	"Plays just like the dice game.",
	"",
	"See Rules for instructions.",
	NULL
};

/*
 * Commands and Settings
*/
static bot_cmd ys_commands[]=
{
	{"START",	StartYahtzeeGame,	0,	0,	ys_help_start,		ys_help_start_oneline},
	{"STOP",	StopYahtzeeGame,	0,	0,	ys_help_stop,		ys_help_stop_oneline},
	{"JOIN",	JoinYahtzeeGame,	0,	0,	ys_help_join,		ys_help_join_oneline},
	{"REMOVE",	RemoveYahtzeeGame,	0,	0,	ys_help_remove,		ys_help_remove_oneline},
	{"PLAYERS",	ShowPlayersYahtzeeGame,	0,	0,	ys_help_players,	ys_help_players_oneline},
	{"TURN",	ShowTurnYahtzeeGame,	0,	0,	ys_help_turn,		ys_help_turn_oneline},
	{"PASS",	PassYahtzeeGame,	1,	0,	ys_help_pass,		ys_help_pass_oneline},
	{"DICE",	ShowYahtzeeDice,	0,	0,	ys_help_dice,		ys_help_dice_oneline},
	{"SHEET",	ShowYahtzeeSheet,	0,	0,	ys_help_sheet,		ys_help_sheet_oneline},
	{"ROLL",	RollYahtzeeDice,	0,	0,	ys_help_roll,		ys_help_roll_oneline},
	{"KEEP",	KeepYahtzeeDice,	0,	0,	ys_help_keep,		ys_help_keep_oneline},
	{"SCORE",	ScoreYahtzeeDice,	0,	0,	ys_help_score,		ys_help_score_oneline},
	{"TOP10",	ShowTop10Lists,		0,	0,	ys_help_top10,		ys_help_top10_oneline},
	{"HIGH",	ShowHighList,		0,	0,	ys_help_high,		ys_help_high_oneline},
	{"RULES",	ShowRulePages,		0,	0,	ys_help_rules,		ys_help_rules_oneline},
	{NULL,		NULL,			0, 	0,	NULL,			NULL}
};

static bot_setting ys_settings[]=
{
	{"CHAN",	&yahtzeeroom,	SET_TYPE_CHANNEL,	0,	MAXCHANLEN,	NS_ULEVEL_ADMIN,	NULL,	ys_help_set_chan,	ys_cmd_set_chan,	(void *)"#Games_Yahtzee" },
	{NULL,		NULL,		0,			0,	0,		0,			NULL,	NULL,			NULL, 			NULL },
};

/*
 * Module Info definition 
*/
ModuleInfo module_info = {
	"YahtzeeServ",
	"Yahtzee Game Module For NeoStats",
	ys_copyright,
	ys_about,
	NEOSTATS_VERSION,
	"3.0",
	__DATE__,
	__TIME__,
	0,
	0,
};

/*
 * Module event list
*/
ModuleEvent module_events[] = {
	{EVENT_NICK, PlayerNickChange},
	{EVENT_NULL, NULL}
};

/** BotInfo */
static BotInfo ys_botinfo = 
{
	"YahtzeeServ",
	"YahtzeeServ1",
	"Yahtzee",
	BOT_COMMON_HOST,
	"Yahtzee Game Service",
	BOT_FLAG_SERVICEBOT|BOT_FLAG_PERSIST,
	ys_commands,
	ys_settings,
};

char *s_module_bot_name;
char yahtzeeroom[MAXCHANLEN];
char ysplayernick[10][MAXNICK];
int ysplayerscore[10];
char *ysscoretype[15];
int ysplayerhand[10][15];
char *ysplayerhandused[10][15];
int ypln;
int yplnh;
int currentyahtzeeplayercount;
int currentyahtzeegamestatus;
int currentplayer;
int currentroll;
int currenthand;
int ydie[5];
char *scoretext[100];
int scoretextargs;
char *tmpstr;
int crs;
int htsslrn;
int htsslrold;
char htsslrnickold[MAXNICK];
int htsslr[100];
char htsslrnick[100][MAXNICK];
int dhtsslr[10];
char dhtsslrnick[10][MAXNICK];
int whtsslr[10];
char whtsslrnick[10][MAXNICK];
int mhtsslr[10];
char mhtsslrnick[10][MAXNICK];
int dtsc[6];

/*
 * Online event processing
*/
int ModSynch (void)
{
	/* Introduce a bot onto the network */
	ys_bot = AddBot (&ys_botinfo);	
	if (!ys_bot) {
		return NS_FAILURE;
	}
	srand((unsigned int)me.now);
	irc_chanalert (ys_bot, "Game will start in %s", yahtzeeroom);
	irc_join (ys_bot, yahtzeeroom, "+o");
	return NS_SUCCESS;
};

/*
 * Init module
*/
int ModInit( void )
{
	ysscoretype[0]= "One's";
	ysscoretype[1]= "Two's";
	ysscoretype[2]= "Three's";
	ysscoretype[3]= "Four's";
	ysscoretype[4]= "Five's";
	ysscoretype[5]= "Six's";
	ysscoretype[6]= "Full House";
	ysscoretype[7]= "Short Straight";
	ysscoretype[8]= "Long Straight";
	ysscoretype[9]= "Three of a Kind";
	ysscoretype[10]= "Four of a Kind";
	ysscoretype[11]= "Chance";
	ysscoretype[12]= "Yahtzee";
	ysscoretype[13]= "Bonus";
	ysscoretype[14]= "Yahtzee Bonus";
	stopyahtzee();
	htsslrn= 0;
	htsslrold= 0;
	for (ypln = 0; ypln < 100; ypln++) {
		strlcpy(htsslrnick[ypln], " ", MAXNICK);
		htsslr[ypln]= 0;
	}
	ModuleConfig (ys_settings);
	AddTimer (TIMER_TYPE_DAILY, yahtzeeday, "yahtzeeday", 0);
	AddTimer (TIMER_TYPE_WEEKLY, yahtzeeweek, "yahtzeeweek", 0);
	AddTimer (TIMER_TYPE_MONTHLY, yahtzeemonth, "yahtzeemonth", 0);
	loadyahtzeescores();
	strlcpy(htsslrnickold, " ", MAXNICK);
	return NS_SUCCESS;
}

/*
 * Exit module
*/
int ModFini( void )
{
	if (currentyahtzeegamestatus == YS_GAME_STARTING) {
		DelTimer ("startyahtzee");
	}
	DelTimer ("yahtzeeday");
	DelTimer ("yahtzeeweek");
	DelTimer ("yahtzeemonth");
	return NS_SUCCESS;
}

/*
 * Set Overrides
*/
static int ys_cmd_set_chan(CmdParams *cmdparams, SET_REASON reason) 
{
	if (reason == SET_VALIDATE) {
		if (currentyahtzeegamestatus != YS_GAME_STOPPED) {
			irc_prefmsg (ys_bot, cmdparams->source, "Unable to change Game Channel while Game in Progress.");
			return NS_FAILURE;
		}
		irc_chanalert (ys_bot, "Game Channel Changing to %s , Parting %s (%s)", cmdparams->av[1], yahtzeeroom, cmdparams->source->name);
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039%s has changed Channels, Game will now be available in %s", cmdparams->source->name, cmdparams->av[1]);
		irc_part (ys_bot, yahtzeeroom, NULL);
		return NS_SUCCESS;
	}
	if (reason == SET_CHANGE) {
		irc_join (ys_bot, yahtzeeroom, "+o");
		irc_chanalert (ys_bot, "Game Now Available In %s", yahtzeeroom);
		return NS_SUCCESS;
	}
	return NS_SUCCESS;
}

/*
 * Loads High Score Lists
*/
void loadyahtzeescores() {

	char cfgid[255];

	for (ypln = 0; ypln < 10; ypln++) {
		snprintf(cfgid, 255, "HSON%d", ypln);
		if (DBAFetchStr ("Scores", cfgid, htsslrnick[ypln], MAXNICK) != NS_SUCCESS) {
			strlcpy (htsslrnick[ypln], " ", MAXNICK);
		}
		snprintf(cfgid, 255, "HSOS%d", ypln);
		if (DBAFetchInt ("Scores", cfgid, &htsslr[ypln]) != NS_SUCCESS) {
			htsslr[ypln] = 0;
		}
		snprintf(cfgid, 255, "HSDN%d", ypln);
		if (DBAFetchStr ("Scores", cfgid, dhtsslrnick[ypln], MAXNICK) != NS_SUCCESS) {
			strlcpy (dhtsslrnick[ypln], " ", MAXNICK);
		}
		snprintf(cfgid, 255, "HSDS%d", ypln);
		if (DBAFetchInt ("Scores", cfgid, &dhtsslr[ypln]) != NS_SUCCESS) {
			dhtsslr[ypln] = 0;
		}
		snprintf(cfgid, 255, "HSWN%d", ypln);
		if (DBAFetchStr ("Scores", cfgid, whtsslrnick[ypln], MAXNICK) != NS_SUCCESS) {
			strlcpy (whtsslrnick[ypln], " ", MAXNICK);
		}
		snprintf(cfgid, 255, "HSWS%d", ypln);
		if (DBAFetchInt ("Scores", cfgid, &whtsslr[ypln]) != NS_SUCCESS) {
			whtsslr[ypln] = 0;
		}
		snprintf(cfgid, 255, "HSMN%d", ypln);
		if (DBAFetchStr ("Scores", cfgid, mhtsslrnick[ypln], MAXNICK) != NS_SUCCESS) {
			strlcpy (mhtsslrnick[ypln], " ", MAXNICK);
		}
		snprintf(cfgid, 255, "HSMS%d", ypln);
		if (DBAFetchInt ("Scores", cfgid, &mhtsslr[ypln]) != NS_SUCCESS) {
			mhtsslr[ypln] = 0;
		}
	}
	for (ypln = 10; ypln < 100; ypln++) {
		snprintf(cfgid, 255, "HSON%d", ypln);
		if (DBAFetchStr ("Scores", cfgid, htsslrnick[ypln], MAXNICK) != NS_SUCCESS) {
			strlcpy (htsslrnick[ypln], " ", MAXNICK);
		}
		snprintf(cfgid, 255, "HSOS%d", ypln);
		if (DBAFetchInt ("Scores", cfgid, &htsslr[ypln]) != NS_SUCCESS) {
			htsslr[ypln] = 0;
		}
	}
}

/** 
 * These clear the appropriate score lists
 */
void clearyahtzeedaily() {
	for (ypln = 0; ypln < 10; ypln++) {
		strlcpy(dhtsslrnick[ypln], " ", MAXNICK);
		dhtsslr[ypln]= 0;
	}
	saveyahtzeedaily();
}
void clearyahtzeeweekly() {
	for (ypln = 0; ypln < 10; ypln++) {
		strlcpy(whtsslrnick[ypln], " ", MAXNICK);
		whtsslr[ypln]= 0;
	}
	saveyahtzeeweekly();
}
void clearyahtzeemonthly() {
	for (ypln = 0; ypln < 10; ypln++) {
		strlcpy(mhtsslrnick[ypln], " ", MAXNICK);
		mhtsslr[ypln]= 0;
	}
	saveyahtzeemonthly();
}

/** 
 * These save the appropriate score lists
 */
void saveyahtzeeoverall() {
	char cfgid[255];
	for (ypln = 0; ypln < 100; ypln++) {
		snprintf(cfgid, 255, "HSON%d", ypln);
		DBAStoreStr ("Scores", cfgid, htsslrnick[ypln], MAXNICK);
		snprintf(cfgid, 255, "HSOS%d", ypln);
		DBAStoreInt ("Scores", cfgid, &htsslr[ypln]);
	}
}

void saveyahtzeedaily() {
	char cfgid[255];
	for (ypln = 0; ypln < 10; ypln++) {
		snprintf(cfgid, 255, "HSDN%d", ypln);
		DBAStoreStr ("Scores", cfgid, dhtsslrnick[ypln], MAXNICK);
		snprintf(cfgid, 255, "HSDS%d", ypln);
		DBAStoreInt ("Scores", cfgid, &dhtsslr[ypln]);
	}
}
void saveyahtzeeweekly() {
	char cfgid[255];
	for (ypln = 0; ypln < 10; ypln++) {
		snprintf(cfgid, 255, "HSWN%d", ypln);
		DBAStoreStr ("Scores", cfgid, whtsslrnick[ypln], MAXNICK);
		snprintf(cfgid, 255, "HSWS%d", ypln);
		DBAStoreInt ("Scores", cfgid, &whtsslr[ypln]);
	}
}
void saveyahtzeemonthly() {
	char cfgid[255];
	for (ypln = 0; ypln < 10; ypln++) {
		snprintf(cfgid, 255, "HSMN%d", ypln);
		DBAStoreStr ("Scores", cfgid, mhtsslrnick[ypln], MAXNICK);
		snprintf(cfgid, 255, "HSMS%d", ypln);
		DBAStoreInt ("Scores", cfgid, &mhtsslr[ypln]);
	}
}
