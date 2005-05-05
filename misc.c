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

#include "neostats.h"    /* Required for bot support */
#include "yahtzeeserv.h"

/*
 * Display Rules
*/
int ShowRulePages (CmdParams* cmdparams)
{
	if (cmdparams->ac < 1) {
		irc_prefmsg (ys_bot, cmdparams->source, "Welcome To Yahtzee %s", cmdparams->source->name);
		irc_prefmsg (ys_bot, cmdparams->source, " ");
		irc_prefmsg_list (ys_bot, cmdparams->source, ys_help_rules_page_0);
	} else {
		if (!ircstrcasecmp (cmdparams->av[0], "1")) {
			irc_prefmsg_list (ys_bot, cmdparams->source, ys_help_rules_page_1);
		} else if (!ircstrcasecmp (cmdparams->av[0], "2")) {
			irc_prefmsg_list (ys_bot, cmdparams->source, ys_help_rules_page_2);
		} else if (!ircstrcasecmp (cmdparams->av[0], "3")) {
			irc_prefmsg_list (ys_bot, cmdparams->source, ys_help_rules_page_3);
		} else if (!ircstrcasecmp (cmdparams->av[0], "4")) {
			irc_prefmsg_list (ys_bot, cmdparams->source, ys_help_rules_page_4);
		}
	}
	return NS_SUCCESS;
}

/*
 * Pass Game To Another User
*/
int PassYahtzeeGame (CmdParams* cmdparams)
{
	GameData *gd;
	Client *u;
	int i, gp;

	if (!GetUserModValue(cmdparams->source)) {
		return NS_SUCCESS;
	}
	gd = (GameData *)GetChannelModValue(cmdparams->channel);
	if (!gd) {
		return NS_SUCCESS;
	}
	if (!gd->playercount) {
		return NS_SUCCESS;
	}
	if (cmdparams->ac < 1) {
		return NS_SUCCESS;
	}
	gp = 0;
	for (i = 0 ; i < gd->playercount ; i++) {
		if (gd->pd[i]->u == cmdparams->source) {
			u = FindUser(cmdparams->av[0]);
			if (!u) {
				irc_chanprivmsg (ys_bot, cmdparams->channel->name, "Sorry %s , There is no %s currently Online", cmdparams->source->name, cmdparams->av[0]);
				return NS_SUCCESS;
			} else if ((YahtzeeServ.exclusions && IsExcluded(u)) || ModIsUserExcluded(u) || IsMe(u) || IsBot(u)) {
				irc_chanprivmsg (ys_bot, cmdparams->channel->name, "Sorry %s , %s Can't play your game for you at this time", cmdparams->source->name, u->name);
				return NS_SUCCESS;
			} else if (cmdparams->source == u) {
				irc_chanprivmsg (ys_bot, cmdparams->channel->name, "Just play the game %s and stop trying to be smart", cmdparams->source->name);
				return NS_SUCCESS;
			}
			if (GetUserModValue(cmdparams->source) > 0) {
				SetUserModValue(cmdparams->source,(void *)((int)GetUserModValue(cmdparams->source) - 1));
			}
			gd->pd[i]->u = u;
			if (!GetUserModValue(u)) {
				SetUserModValue(u,(void *)1);
			} else {
				SetUserModValue(u,(void *)((int)GetUserModValue(u) + 1));
			}
			if (!gp) {
				irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0037%s\0039 has passed their game to\0037 %s", cmdparams->source->name, u->name);
			}
			gp++;
		}
	}
	return NS_SUCCESS;
}

/*
 * Display Current Players
*/
int ShowPlayersYahtzeeGame (CmdParams* cmdparams)
{
	GameData *gd;

	gd = (GameData *)GetChannelModValue(cmdparams->channel);
	if (!gd) {
		return NS_SUCCESS;
	}
	if (gd->gamestatus == YS_GAME_PLAYING) {
		irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\00310The Current Yahtzee Players in\0037 %s \00310are\0038 :\0037 %s %s %s %s %s %s %s %s %s %s", cmdparams->channel->name, gd->pd[0]->u ? gd->pd[0]->u->name : "", gd->pd[1]->u ? gd->pd[1]->u->name : "", gd->pd[2]->u ? gd->pd[2]->u->name : "", gd->pd[3]->u ? gd->pd[3]->u->name : "", gd->pd[4]->u ? gd->pd[4]->u->name : "", gd->pd[5]->u ? gd->pd[5]->u->name : "", gd->pd[6]->u ? gd->pd[6]->u->name : "", gd->pd[7]->u ? gd->pd[7]->u->name : "", gd->pd[8]->u ? gd->pd[8]->u->name : "", gd->pd[9]->u ? gd->pd[9]->u->name : "");
	}
	return NS_SUCCESS;
}

/*
 * Display Current Players Turn
*/
int ShowTurnYahtzeeGame (CmdParams* cmdparams)
{
	GameData *gd;

	gd = (GameData *)GetChannelModValue(cmdparams->channel);
	if (!gd) {
		return NS_SUCCESS;
	}
	if (gd->gamestatus == YS_GAME_PLAYING) {
		irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0039The current Player is \0038 :\0037 %s", gd->pd[gd->currentplayer]->u->name);
	}
	return NS_SUCCESS;
}

/*
 * Show Players Score Sheet
*/
int ShowYahtzeeSheet (CmdParams* cmdparams)
{
	GameData *gd;
	int i;

	gd = (GameData *)GetChannelModValue(cmdparams->channel);
	if (!gd) {
		return NS_SUCCESS;
	}
	if (!GetUserModValue(cmdparams->source)) {
		return NS_SUCCESS;
	}
	if (gd->gamestatus == YS_GAME_PLAYING) {
		for (i = 0; i < gd->playercount; i++) {
			if (gd->pd[i]->u == cmdparams->source) {
				irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0037%s\0038 your current scores are :\0039 %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d%s\0039 , %s\00311 %d\0039 , %s\00311 %d\0039 , Total\00311 %d",
					cmdparams->source->name, ysscoretype[0], gd->pd[i]->hand[0] < 0 ? 0 : gd->pd[i]->hand[0], gd->pd[i]->hand[0] < 0 ? "\0034\2x\2" : "", ysscoretype[1], gd->pd[i]->hand[1] < 0 ? 0 : gd->pd[i]->hand[1], gd->pd[i]->hand[1] < 0 ? "\0034\2x\2" : "", ysscoretype[2], gd->pd[i]->hand[2] < 0 ? 0 : gd->pd[i]->hand[2], gd->pd[i]->hand[2] < 0 ? "\0034\2x\2" : "", 
					ysscoretype[3], gd->pd[i]->hand[3] < 0 ? 0 : gd->pd[i]->hand[3], gd->pd[i]->hand[3] < 0 ? "\0034\2x\2" : "", ysscoretype[4], gd->pd[i]->hand[4] < 0 ? 0 : gd->pd[i]->hand[4], gd->pd[i]->hand[4] < 0 ? "\0034\2x\2" : "", ysscoretype[5], gd->pd[i]->hand[5] < 0 ? 0 : gd->pd[i]->hand[5], gd->pd[i]->hand[5] < 0 ? "\0034\2x\2" : "", 
					ysscoretype[6], gd->pd[i]->hand[6] < 0 ? 0 : gd->pd[i]->hand[6], gd->pd[i]->hand[6] < 0 ? "\0034\2x\2" : "", ysscoretype[7], gd->pd[i]->hand[7] < 0 ? 0 : gd->pd[i]->hand[7], gd->pd[i]->hand[7] < 0 ? "\0034\2x\2" : "", ysscoretype[8], gd->pd[i]->hand[8] < 0 ? 0 : gd->pd[i]->hand[8], gd->pd[i]->hand[8] < 0 ? "\0034\2x\2" : "", 
					ysscoretype[9], gd->pd[i]->hand[9] < 0 ? 0 : gd->pd[i]->hand[9], gd->pd[i]->hand[9] < 0 ? "\0034\2x\2" : "", ysscoretype[10], gd->pd[i]->hand[10] < 0 ? 0 : gd->pd[i]->hand[10], gd->pd[i]->hand[10] < 0 ? "\0034\2x\2" : "", ysscoretype[11], gd->pd[i]->hand[11] < 0 ? 0 : gd->pd[i]->hand[11], gd->pd[i]->hand[11] < 0 ? "\0034\2x\2" : "", 
					ysscoretype[12], gd->pd[i]->hand[12] < 0 ? 0 : gd->pd[i]->hand[12], gd->pd[i]->hand[12] < 0 ? "\0034\2x\2" : "", ysscoretype[13], gd->pd[i]->hand[13] < 0 ? 0 : gd->pd[i]->hand[13], ysscoretype[14], gd->pd[i]->hand[14] < 0 ? 0 : gd->pd[i]->hand[14], gd->pd[i]->score);
			}
		}
	}
	return NS_SUCCESS;
}

/*
 * Display The Last Rolled Dice
*/
int ShowYahtzeeDice (CmdParams* cmdparams)
{
	GameData *gd;
	int i;

	gd = (GameData *)GetChannelModValue(cmdparams->channel);
	if (!gd) {
		return NS_SUCCESS;
	}
	if (!GetUserModValue(cmdparams->source)) {
		return NS_SUCCESS;
	}
	if (gd->gamestatus == YS_GAME_PLAYING && gd->pd[gd->currentplayer]->u == cmdparams->source) {
		for (i = 0; i < 5; i++) {
			if (gd->dice[i] == 0) {
				strlcpy(dicetext[i], "One", 15);
			} else if (gd->dice[i] == 1) {
				strlcpy(dicetext[i], "Two", 15);
			} else if (gd->dice[i] == 2) {
				strlcpy(dicetext[i], "Three", 15);
			} else if (gd->dice[i] == 3) {
				strlcpy(dicetext[i], "Four", 15);
			} else if (gd->dice[i] == 4) {
				strlcpy(dicetext[i], "Five", 15);
			} else if (gd->dice[i] == 5) {
				strlcpy(dicetext[i], "Six", 15);
			}
		}
		irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0037%s\0039 your last roll was roll \00311 %d\0039, you have \0034 %s %s %s %s %s\0039 showing", gd->pd[gd->currentplayer]->u->name, gd->currentroll, dicetext[0], dicetext[1], dicetext[2], dicetext[3], dicetext[4]);
	}
	return NS_SUCCESS;
}

/*
 * User Events
 *
 * check if user is a Yahtzee Player
 * and if so, remove from games as appropriate
*/
int CheckPlayerPart (CmdParams *cmdparams) {
	if (GetUserModValue(cmdparams->source) > 0) {
		removenickfromgame(cmdparams->channel, cmdparams->source);
	}
}

int CheckPlayerKick (CmdParams *cmdparams) {
	if (GetUserModValue(cmdparams->target) > 0) {
		removenickfromgame(cmdparams->channel, cmdparams->target);
	}
}

int CheckPlayerQuit (CmdParams *cmdparams) {
	GameData *gd;
	Channel *c;
	lnode_t	*ln;

	if (GetUserModValue(cmdparams->source) > 0) {
		ln = list_first(gamelist);
		while (ln != NULL && GetUserModValue(cmdparams->source) > 0) {
			c = lnode_get(ln);
			gd = (GameData *)GetChannelModValue(c);
			if (gd) {
				removenickfromgame(c, cmdparams->source);
			}
			ln = list_next(gamelist, ln);
		}
	}
}

int CheckPlayerKill (CmdParams *cmdparams) {
	GameData *gd;
	Channel *c;
	lnode_t	*ln;

	if (GetUserModValue(cmdparams->target) > 0) {
		ln = list_first(gamelist);
		while (ln != NULL && GetUserModValue(cmdparams->target) > 0) {
			c = lnode_get(ln);
			gd = (GameData *)GetChannelModValue(c);
			if (gd) {
				removenickfromgame(c, cmdparams->target);
			}
			ln = list_next(gamelist, ln);
		}
	}
}
