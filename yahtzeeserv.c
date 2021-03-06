/* YahtzeeServ - Yahtzee Game Service - NeoStats Addon Module
** Copyright (c) 2003-2006 Justin Hammond, Mark Hetherington, Jeff Lang
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
	"Copyright (c) 2003-2006 Justin Hammond, Mark Hetherington, DeadNotBuried",
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
	{"START",	StartYahtzeeGame,	0,	0,			ys_help_start},
	{"STOP",	StopYahtzeeGame,	0,	0,			ys_help_stop,		CMD_FLAG_CHANONLY},
	{"JOIN",	JoinYahtzeeGame,	0,	0,			ys_help_join,		CMD_FLAG_CHANONLY},
	{"REMOVE",	RemoveYahtzeeGame,	0,	0,			ys_help_remove,		CMD_FLAG_CHANONLY},
	{"PLAYERS",	ShowPlayersYahtzeeGame,	0,	0,			ys_help_players,	CMD_FLAG_CHANONLY},
	{"TURN",	ShowTurnYahtzeeGame,	0,	0,			ys_help_turn,		CMD_FLAG_CHANONLY},
	{"PASS",	PassYahtzeeGame,	1,	0,			ys_help_pass,		CMD_FLAG_CHANONLY},
	{"DICE",	ShowYahtzeeDice,	0,	0,			ys_help_dice,		CMD_FLAG_CHANONLY},
	{"SHEET",	ShowYahtzeeSheet,	0,	0,			ys_help_sheet,		CMD_FLAG_CHANONLY},
	{"ROLL",	RollYahtzeeDice,	0,	0,			ys_help_roll,		CMD_FLAG_CHANONLY},
	{"KEEP",	KeepYahtzeeDice,	0,	0,			ys_help_keep,		CMD_FLAG_CHANONLY},
	{"SCORE",	ScoreYahtzeeDice,	1,	0,			ys_help_score,		CMD_FLAG_CHANONLY},
	{"TOP10",	ShowTop10Lists,		0,	0,			ys_help_top10},
	{"HIGH",	ShowHighList,		0,	0,			ys_help_high},
	{"RULES",	ShowRulePages,		0,	0,			ys_help_rules},
	{"FORCEHTML",	ys_cmd_forcehtml,	0, 	NS_ULEVEL_ADMIN,	ys_help_forcehtml},
	NS_CMD_END()
};

static bot_setting ys_settings[]=
{
	{"EXCLUSIONS",	&YahtzeeServ.exclusions,	SET_TYPE_BOOLEAN,	0,	0,		NS_ULEVEL_ADMIN,	NULL,	ys_help_set_exclusions,	NULL,			( void * )0 },
	{"CHAN",	&YahtzeeServ.yahtzeeroom,	SET_TYPE_CHANNEL,	0,	MAXCHANLEN,	NS_ULEVEL_ADMIN,	NULL,	ys_help_set_chan,	ys_cmd_set_chan,	( void * )"#Games_Yahtzee" },
	{"MULTICHAN",	&YahtzeeServ.multichan,		SET_TYPE_BOOLEAN,	0,	0,		NS_ULEVEL_ADMIN,	NULL,	ys_help_set_multichan,	NULL,			( void * )0 },
	{"CHANOPONLY",	&YahtzeeServ.chanoponly,	SET_TYPE_BOOLEAN,	0,	0,		NS_ULEVEL_ADMIN,	NULL,	ys_help_set_chanoponly,	NULL,			( void * )0 },
	{"HTML",	&YahtzeeServ.html,		SET_TYPE_BOOLEAN,	0, 	0, 		NS_ULEVEL_ADMIN, 	NULL,	ys_help_set_html,	ys_cmd_set_html,	( void * )0},
	{"HTMLPATH",	&YahtzeeServ.htmlpath,		SET_TYPE_STRING,	0,	MAXPATH,	NS_ULEVEL_ADMIN, 	NULL,	ys_help_set_htmlpath,	ys_cmd_set_htmlpath,	( void * )""},
	NS_SETTING_END()
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
	MODULE_VERSION,
	__DATE__,
	__TIME__,
	MODULE_FLAG_LOCAL_EXCLUDES,
	0,
};

/*
 * Module event list
*/
ModuleEvent module_events[] = {
	{EVENT_QUIT,		CheckPlayerQuit},
	{EVENT_KILL,		CheckPlayerKill},
	{EVENT_LOCALKILL,	CheckPlayerKill},
	{EVENT_PART,		CheckPlayerPart},
	{EVENT_KICK,		CheckPlayerKick},
	NS_EVENT_END()
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

list_t *gamelist;
list_t *highscores;

char *ysscoretype[15];
int crs;
int dtsc[6];
char dicetext[5][15];
YahtzeeServCnf YahtzeeServ;
Bot *ys_bot;

/*
 * Online event processing
*/
int ModSynch (void)
{
	Channel *c;
	
	ys_bot = AddBot (&ys_botinfo);	
	if (!ys_bot)
		return NS_FAILURE;
	if( YahtzeeServ.html )
		ys_HTMLOutput();
	srand((unsigned int)me.now);
	irc_chanalert (ys_bot, "Game will start in %s", YahtzeeServ.yahtzeeroom);
	irc_join (ys_bot, YahtzeeServ.yahtzeeroom, "+o");
	c = FindChannel(YahtzeeServ.yahtzeeroom);
	gamelist = list_create( LISTCOUNT_T_MAX );
	CreateChannelGameData(c);
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
	ModuleConfig (ys_settings);
	AddTimer (TIMER_TYPE_DAILY, yahtzeeday, "yahtzeeday", 0, NULL);
	AddTimer (TIMER_TYPE_WEEKLY, yahtzeeweek, "yahtzeeweek", 0, NULL);
	AddTimer (TIMER_TYPE_MONTHLY, yahtzeemonth, "yahtzeemonth", 0, NULL);
	AddTimer (TIMER_TYPE_INTERVAL, yahtzeetimer, "yahtzeetimer", 5, NULL);
	loadyahtzeescores();
	return NS_SUCCESS;
}

/*
 * Exit module
*/
int ModFini( void )
{
	DelTimer ("yahtzeeday");
	DelTimer ("yahtzeeweek");
	DelTimer ("yahtzeemonth");
	DelTimer ("yahtzeetimer");
	return NS_SUCCESS;
}

/** @brief ys_cmd_set_html
 *
 *  Set callback for SET HTML
 *  Enable or disable html output
 *
 *  @params cmdparams pointer to commands param struct
 *  @params reason for SET
 *
 *  @return NS_SUCCESS if suceeds else NS_FAILURE
 */

int ys_cmd_set_html( const CmdParams *cmdparams, SET_REASON reason )
{
	if( reason == SET_CHANGE )
	{
		if( YahtzeeServ.html && YahtzeeServ.htmlpath[0] == 0 )
		{
			irc_prefmsg ( ys_bot, cmdparams->source, 
				"You need to SET HTMLPATH. HTML output disabled." );
			YahtzeeServ.html = 0;
			return NS_SUCCESS;
		}
		if( YahtzeeServ.html && YahtzeeServ.htmlpath[0] != 0 )
			ys_HTMLOutput();
	}
	return NS_SUCCESS;
}

/** @brief ys_cmd_set_htmlpath
 *
 *  Set callback for SET HTMLPATH
 *  Change html output path
 *
 *  @params cmdparams pointer to commands param struct
 *  @params reason for SET
 *
 *  @return NS_SUCCESS if suceeds else NS_FAILURE
 */

int ys_cmd_set_htmlpath( const CmdParams *cmdparams, SET_REASON reason )
{
	FILE *opf;

	if( reason == SET_CHANGE )
	{
		opf = os_fopen( YahtzeeServ.htmlpath, "wt" );
		if( !opf )
		{
			irc_prefmsg( ys_bot, cmdparams->source, 
				"Failed to open HTML output file %s. Check file permissions. HTML output disabled.", YahtzeeServ.htmlpath );
			return NS_SUCCESS;
		}
		os_fclose( opf );
		if( YahtzeeServ.html )
			ys_HTMLOutput();
	}
	return NS_SUCCESS;
}
