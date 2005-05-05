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
 * Start Game
*/
int StartYahtzeeGame (CmdParams* cmdparams)
{
	GameData *gd;
	Channel *c;
	
	if (!cmdparams->channel && !cmdparams->ac) {
		return NS_SUCCESS;
	}
	if (cmdparams->ac) {
		if (!YahtzeeServ.multichan) {
			irc_prefmsg (ys_bot, cmdparams->source, "Games outside %s are not currently allowed", YahtzeeServ.yahtzeeroom);
			return NS_SUCCESS;
		}
		if (ValidateChannel(cmdparams->av[0]) == NS_FAILURE) {
			irc_prefmsg (ys_bot, cmdparams->source, "Invalid Channel Name Specified (%s)", cmdparams->av[0]);
			return NS_SUCCESS;
		}
		c = FindChannel(cmdparams->av[0]);
		if (!c) {
			irc_prefmsg (ys_bot, cmdparams->source, "%s doesn't currently exist, unable to start.", cmdparams->av[0]);
			return NS_SUCCESS;
		}
		if (IsChannelMember(c, cmdparams->source) == NS_FALSE) {
			irc_prefmsg (ys_bot, cmdparams->source, "You are not currently in %s. Unable to start.", c->name);
			return NS_SUCCESS;
		}
		if ( YahtzeeServ.chanoponly && !IsChanOp(c->name, cmdparams->source->name)) {
			irc_prefmsg (ys_bot, cmdparams->source, "Currently only Channel Ops may start games outside the main game channel.");
			return NS_SUCCESS;
		}
		if ((YahtzeeServ.exclusions && IsExcluded(c)) || ModIsChannelExcluded(c)) {
			irc_prefmsg (ys_bot, cmdparams->source, "%s is currently excluded from Yahtzee games.", c->name);
			return NS_SUCCESS;
		}
	} else {
		c = cmdparams->channel;
	}
	gd = (GameData *)GetChannelModValue(c);
	if (!gd) {
		CreateChannelGameData(c);
		gd = (GameData *)GetChannelModValue(c);
	} else if (gd->gamestatus != YS_GAME_STOPPED) {
		irc_prefmsg (ys_bot, cmdparams->source, "A game is already running in %s.", c->name);
		return NS_SUCCESS;	
	}
	gd->timer = 25;
	gd->gamestatus = YS_GAME_STARTING;
	irc_chanprivmsg (ys_bot, c->name, "\0037Yahtzee has been started by %s. The game will start shortly, type '\2\003!Join\2\0037' to play.", cmdparams->source->name);
	if (YahtzeeServ.verbose) {
		irc_chanalert (ys_bot, "Game starting in %s", c->name);
	}
	return NS_SUCCESS;
}

/*
 * Timer Procedure
 * checks for starting games
 * if game if due to start, checks for joined users
 * if users joined starts game, if not end game.
*/
int yahtzeetimer(void) {
	lnode_t *ln, *ln2;
	GameData *gd;
	Channel *c;
	
	ln = list_first(gamelist);
	while (ln != NULL) {
		c = lnode_get(ln);
		gd = (GameData *)GetChannelModValue(c);
		if (!gd) {
			ln2 = list_next(gamelist, ln);
			list_delete(gamelist, ln);
			lnode_destroy(ln);
			ln = ln2;
		} else if (gd->gamestatus == YS_GAME_STARTING) {
			if (gd->timer > 4) {
				gd->timer -= 5;
				ln = list_next(gamelist, ln);
			} else {
				gd->timer = 0;
				if (gd->playercount < 1) {
					irc_chanprivmsg (ys_bot, c->name, "\0034No Players joined to current Game, Exiting");
					if (YahtzeeServ.verbose) {
						irc_chanalert (ys_bot, "No Players Joined Game In %s, exiting", c->name);
					}
					ln2 = list_next(gamelist, ln);
					lnode_destroy(ln);
					ln = ln2;
					RemoveChannelGameData(c, 0);
				} else {
					irc_chanprivmsg (ys_bot, c->name, "\00310Yahtzee is now starting, current players are \0037%s %s %s %s %s %s %s %s %s %s", gd->pd[0]->u ? gd->pd[0]->u->name : "", gd->pd[1]->u ? gd->pd[1]->u->name : "", gd->pd[2]->u ? gd->pd[2]->u->name : "", gd->pd[3]->u ? gd->pd[3]->u->name : "", gd->pd[4]->u ? gd->pd[4]->u->name : "", gd->pd[5]->u ? gd->pd[5]->u->name : "", gd->pd[6]->u ? gd->pd[6]->u->name : "", gd->pd[7]->u ? gd->pd[7]->u->name : "", gd->pd[8]->u ? gd->pd[8]->u->name : "", gd->pd[9]->u ? gd->pd[9]->u->name : "");
					if (YahtzeeServ.verbose) {
						irc_chanalert (ys_bot, "Game started in %s, with %d players", c->name, gd->playercount);
					}
					gd->gamestatus = YS_GAME_PLAYING;
					gd->currenthand = 1;
					RollDice(c);
					ln = list_next(gamelist, ln);
				}

			}
		} else {
			ln = list_next(gamelist, ln);
		}
	}
	return NS_SUCCESS;
}

/*
 * Join Player To Game
*/
int JoinYahtzeeGame (CmdParams* cmdparams) {
	GameData *gd;
	int i;
	
	gd = (GameData *)GetChannelModValue(cmdparams->channel);
	if (!gd) {
		return NS_SUCCESS;
	}
	if (gd->gamestatus == YS_GAME_STARTING || (gd->gamestatus == YS_GAME_PLAYING && gd->currenthand < 2)) {
		if (gd->playercount < YS_MAX_PLAYERS) {
			for (i = 0; i < gd->playercount; i++) {
				if (gd->pd[i]->u == cmdparams->source) {
					return NS_SUCCESS;
				}
			}
			gd->pd[gd->playercount]->u = cmdparams->source;
			gd->pd[gd->playercount]->score = 0;
			for (i=0;i<15;i++) {
				gd->pd[gd->playercount]->hand[i] = -1;
			}
			gd->playercount++;
			if (!GetUserModValue(cmdparams->source)) {
				SetUserModValue(cmdparams->source,(void *)1);
			} else {
				SetUserModValue(cmdparams->source,(void *)((int)GetUserModValue(cmdparams->source) + 1));
			}
			irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0038Welcome to Yahtzee \0037%s", cmdparams->source->name);
		} else {
			irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0034Sorry all places are filled \0037%s\0034, your welcome to try the next game though", cmdparams->source->name);
		}
	}
	return NS_SUCCESS;
}

/*
 * Remove Player
*/
int RemoveYahtzeeGame (CmdParams* cmdparams)
{
	Client *u;

	if (cmdparams->ac > 0 && (cmdparams->source->user->ulevel >= NS_ULEVEL_LOCOPER || IsChanOp(cmdparams->channel->name, cmdparams->source->name))) {
		u = FindUser(cmdparams->av[0]);
		if (!u) {
			return NS_SUCCESS;
		}
	} else {
		u = cmdparams->source;
	}
	removenickfromgame(cmdparams->channel, u);
	return NS_SUCCESS;
}

/*
 * Removes User From Game In Channel
*/
void removenickfromgame(Channel *c, Client *u) {
	GameData *gd;
	int i, i2, i3, rp;

	if (!GetUserModValue(u)) {
		return;
	}
	gd = (GameData *)GetChannelModValue(c);
	if (!gd) {
		return;
	}
	if (gd->playercount < 1) {
		return;
	}
	if (gd->gamestatus != YS_GAME_STARTING && gd->gamestatus != YS_GAME_PLAYING) {
		return;
	}
	rp = 0;
	for (i = 0 ; i < gd->playercount ; i++) {
		if (gd->pd[i]->u == u) {
			if (GetUserModValue(u) > 0) {
				SetUserModValue(u,(void *)((int)GetUserModValue(u) - 1));
			}
			for (i2 = i ; i2 < gd->playercount ; i2++) {
				if (i2 < (gd->playercount - 1)) {
					gd->pd[i2]->u = gd->pd[(i2 + 1)]->u;
					gd->pd[i2]->score = gd->pd[(i2 + 1)]->score;
					for (i3 = 0 ; i3 < 15 ; i3++) {
						gd->pd[i2]->hand[i3] = gd->pd[(i2 + 1)]->hand[i3];
					}
				} else {
					gd->pd[i2]->u = NULL;
				}
			}
			gd->playercount--;
			if (gd->gamestatus == YS_GAME_PLAYING) {
				if (i < gd->currentplayer) {
					gd->currentplayer--;
				} else if (i == gd->currentplayer) {
					gd->currentroll = 0;
					if (i == gd->playercount) {
						gd->currenthand++;
						gd->currentplayer = 0;
					}
				}
			}
			if (!rp) {
				irc_chanprivmsg (ys_bot, c->name, "\0037%s \0038Removed from the current game of Yahtzee", u->name);
			}
			rp++;
			i--;
		}
	}
	if (gd->playercount < 1) {
		irc_chanprivmsg (ys_bot, c->name, "\0034Last player removed from Game, Stopping Current Game.");
		if (YahtzeeServ.verbose) {
			irc_chanalert (ys_bot, "Last player removed from game in %s, Stopping Game", c->name);
		}
		RemoveChannelGameData(c, 0);
		return;
	}
	if (gd->gamestatus == YS_GAME_PLAYING && !gd->currentroll) {
		RollDice(c);
		return;
	}
	return;
}

/*
 * Stop Game
*/
int StopYahtzeeGame (CmdParams* cmdparams)
{
	GameData *gd;

	if (!GetUserModValue(cmdparams->source) && cmdparams->source->user->ulevel < NS_ULEVEL_LOCOPER && !IsChanOp(cmdparams->channel->name, cmdparams->source->name)) {
		return NS_SUCCESS;
	}
	gd = (GameData *)GetChannelModValue(cmdparams->channel);
	if (!gd) {
		return NS_SUCCESS;
	}
	if (gd->gamestatus != YS_GAME_STARTING && gd->gamestatus != YS_GAME_PLAYING) {
		return NS_SUCCESS;
	}
	if (gd->playercount != 1 && cmdparams->source->user->ulevel < NS_ULEVEL_LOCOPER && !IsChanOp(cmdparams->channel->name, cmdparams->source->name)) {
		irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0034Unable to stop Yahtzee when more than one person playing, please use !remove if you wish to exit the game \0037%s.", cmdparams->source->name);
		return NS_SUCCESS;
	}
	irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0034Yahtzee has been stopped by \0037%s.", cmdparams->source->name);
	if (YahtzeeServ.verbose) {
		irc_chanalert (ys_bot, "Game stopped in %s by %s", cmdparams->channel->name, cmdparams->source->name);
	}
	RemoveChannelGameData(cmdparams->channel, 0);
	return NS_SUCCESS;
}

/*
 * Score to Sheet
*/
int ScoreYahtzeeDice (CmdParams* cmdparams)
{
	GameData *gd;
	int i, crs, sa, stt;

	if (!GetUserModValue(cmdparams->source)) {
		return NS_SUCCESS;
	}
	gd = (GameData *)GetChannelModValue(cmdparams->channel);
	if (!gd) {
		return NS_SUCCESS;
	}
	if (gd->playercount < 1) {
		return NS_SUCCESS;
	}
	if (gd->gamestatus != YS_GAME_PLAYING) {
		return NS_SUCCESS;
	}
	if (gd->pd[gd->currentplayer]->u != cmdparams->source) {
		return NS_SUCCESS;
	}
	for (i = 0; i < 6; i++) {
		dtsc[i]= 0;
	}
	for (i = 0; i < 5; i++) {
		dtsc[gd->dice[i]]++;
	}
	crs = sa = 0;
	if (!ircstrcasecmp(cmdparams->av[0], "1") || !ircstrcasecmp(cmdparams->av[0], "one")) {
		stt= 0;
		if (gd->pd[gd->currentplayer]->hand[0] >= 0) {
			sa= 1;
		} else {
			crs = dtsc[0];
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "2") || !ircstrcasecmp(cmdparams->av[0], "two")) {
		stt= 1;
		if (gd->pd[gd->currentplayer]->hand[1] >= 0) {
			sa= 1;
		} else {
			crs = (dtsc[1] * 2);
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "3") || !ircstrcasecmp(cmdparams->av[0], "three")) {
		stt= 2;
		if (gd->pd[gd->currentplayer]->hand[2] >= 0) {
			sa= 1;
		} else {
			crs = (dtsc[2] * 3);
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "4") || !ircstrcasecmp(cmdparams->av[0], "four")) {
		stt= 3;
		if (gd->pd[gd->currentplayer]->hand[3] >= 0) {
			sa= 1;
		} else {
			crs = (dtsc[3] * 4);
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "5") || !ircstrcasecmp(cmdparams->av[0], "five")) {
		stt= 4;
		if (gd->pd[gd->currentplayer]->hand[4] >= 0) {
			sa= 1;
		} else {
			crs = (dtsc[4] * 5);
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "6") || !ircstrcasecmp(cmdparams->av[0], "six")) {
		stt= 5;
		if (gd->pd[gd->currentplayer]->hand[5] >= 0) {
			sa= 1;
		} else {
			crs = (dtsc[5] * 6);
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "fh") || !ircstrcasecmp(cmdparams->av[0], "full")) {
		stt= 6;
		if (gd->pd[gd->currentplayer]->hand[6] >= 0) {
			sa= 1;
		} else {
			for (i = 0; i < 6; i++) {
				if ((dtsc[i] == 2) || (dtsc[i] == 3)) {
					crs++;
				} else if (dtsc[i] == 1) {
					crs--;
				}
			}
			if (crs == 2) {
				crs = 25;
			} else {
				crs = 0;
			}
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "ss") || !ircstrcasecmp(cmdparams->av[0], "short") || !ircstrcasecmp(cmdparams->av[0], "small")) {
		stt= 7;
		if (gd->pd[gd->currentplayer]->hand[7] >= 0) {
			sa= 1;
		} else {
			for (i = 0; i < 6; i++) {
				if (dtsc[i] == 0) {
					if (crs < 4) {
						crs= 0;
					}
				} else {
					crs++;
				}
			}
			if (crs > 3) {
				crs = 30;
			} else {
				crs = 0;
			}
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "ls") || !ircstrcasecmp(cmdparams->av[0], "long") || !ircstrcasecmp(cmdparams->av[0], "large")) {
		stt= 8;
		if (gd->pd[gd->currentplayer]->hand[8] >= 0) {
			sa= 1;
		} else {
			if ((dtsc[1] == 1) && (dtsc[2] == 1) && (dtsc[3] == 1) && (dtsc[4] == 1)) {
				crs = 40;
			}
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "3k")) {
		stt= 9;
		if (gd->pd[gd->currentplayer]->hand[9] >= 0) {
			sa= 1;
		} else {
			for (i = 0; i < 6; i++) {
				if (dtsc[i] > 2) {
					crs = 1;
				}
			}
			if (crs) {
				crs = 0;
				for (i = 0; i < 6; i++) {
					crs += (dtsc[i] * (i + 1));
				}
			}
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "4k")) {
		stt= 10;
		if (gd->pd[gd->currentplayer]->hand[10] >= 0) {
			sa= 1;
		} else {
			for (i = 0; i < 6; i++) {
				if (dtsc[i] > 3) {
					crs = 1;
				}
			}
			if (crs) {
				crs = 0;
				for (i = 0; i < 6; i++) {
					crs += (dtsc[i] * (i + 1));
				}
			}
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "c") || !ircstrcasecmp(cmdparams->av[0], "chance")) {
		stt= 11;
		if (gd->pd[gd->currentplayer]->hand[11] >= 0) {
			sa= 1;
		} else {
			for (i = 0; i < 6; i++) {
				crs += (dtsc[i] * (i + 1));
			}
		}
	} else if (!ircstrcasecmp(cmdparams->av[0], "y") || !ircstrcasecmp(cmdparams->av[0], "yahtzee")) {
		stt= 12;
		if (gd->pd[gd->currentplayer]->hand[12] >= 0) {
			sa= 1;
		} else {
			for (i = 0; i < 6; i++) {
				if (dtsc[i] == 5) {
					crs = 50;
				}
			}
		}
	} else {
		irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0034Sorry\0037 %s\0034, thats not a valid score slot, please use one of the following", cmdparams->source->name);
		irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\00311One's (1) , Two's (2) , Three's (3) , Four's (4) , Five's (5) , Six's (6)");
		irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\00311Full House (FH) , Short Straight (SS) , Long Straight (LS)");
		irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\00311Three of a Kind (3K) , Four of a Kind (4K) , Chance (C) , Yahtzee (Y)");
		return NS_SUCCESS;
	}
	if (sa == 1) {
		irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0034Sorry\0037 %s\0034 you have already scored on \00311%s", cmdparams->source->name, ysscoretype[stt]);
		return NS_SUCCESS;
	}
	gd->pd[gd->currentplayer]->hand[stt] = crs;
	irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0037%s\0039 scored\00311 %d\0039 on \00311%s", cmdparams->source->name, crs, ysscoretype[stt]);
	gd->pd[gd->currentplayer]->score += crs;
	if (stt != 12 && gd->pd[gd->currentplayer]->hand[12] == 50 && crs) {
		for (i = 0; i < 6; i++) {
			if (dtsc[i] == 5) {
				irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0037%s\0039 scored\00311 100 \0038Yahtzee Bonus \0039points", cmdparams->source->name);
				if (gd->pd[gd->currentplayer]->hand[14] < 0) {
					gd->pd[gd->currentplayer]->hand[14] = 100;
				} else {
					gd->pd[gd->currentplayer]->hand[14] += 100;
				}
				gd->pd[gd->currentplayer]->score += 100;
			}
		}
	}
	if (gd->pd[gd->currentplayer]->hand[13] < 0) {
		crs = 0;
		for (i = 0; i < 6; i++) {
			crs += gd->pd[gd->currentplayer]->hand[i];
		}
		if (crs > 62) {
			irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0037%s\0039 scored a \0038Bonus\00311 35 \0039points", cmdparams->source->name);
			crs = 35;
			gd->pd[gd->currentplayer]->hand[13] = crs;
			gd->pd[gd->currentplayer]->score += crs;
		}
	}
	irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0037%s\0039 now has a total of\00311 %d\0039 points", cmdparams->source->name, gd->pd[gd->currentplayer]->score);
	gd->currentplayer++;
	if (gd->currentplayer >= gd->playercount) {
		gd->currentplayer = 0;
		gd->currenthand++;
	}
	gd->currentroll= 0;
	RollDice(cmdparams->channel);
	return NS_SUCCESS;
}

/*
 * Check if Game over, if not First Dice Roll
*/
void RollDice(Channel *c) {
	GameData *gd;
	int i, ywn;

	ywn = 0;
	gd = (GameData *)GetChannelModValue(c);
	if (!gd) {
		return;
	}
	if (gd->currentplayer == 0) {
		srand((unsigned int)me.now);
		if (gd->currenthand > 13) {
			irc_chanprivmsg (ys_bot, c->name, "\0038Thats the end of the game folks.");
			for (i = 0; i < gd->playercount; i++) {
				if ((i != 0) && (gd->pd[i]->score > gd->pd[ywn]->score)) {
					ywn= i;
				}
				irc_chanprivmsg (ys_bot, c->name, "\0037%s\00310 ended with\00311 %d\00310 points.", gd->pd[i]->u->name, gd->pd[i]->score);
			}
			irc_chanprivmsg (ys_bot, c->name, "\0037%s \0038is the winner with\00311 %d \0038points.", gd->pd[ywn]->u->name, gd->pd[ywn]->score);
			checkhighscorelists(c);
			if (YahtzeeServ.verbose) {
				irc_chanalert (ys_bot, "Game in %s, completed sucessfully", c->name);
			}
			RemoveChannelGameData(c, 0);
			return;
		}
	}
	for (i = 0; i < 5; i++) {
		gd->dice[i]= rand() % 6;
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
	gd->currentroll = 1;
	irc_chanprivmsg (ys_bot, c->name, "\0037%s\0039 this is roll\00311 %d\0039, you rolled\0034 %s %s %s %s %s", gd->pd[gd->currentplayer]->u->name, gd->currentroll, dicetext[0], dicetext[1], dicetext[2], dicetext[3], dicetext[4]);
	return;
}

/*
 * Roll Dice
*/
int RollYahtzeeDice (CmdParams* cmdparams)
{
	reroll(cmdparams, 1);
	return NS_SUCCESS;
}

/*
 * Keep Dice
*/
int KeepYahtzeeDice (CmdParams* cmdparams)
{
	reroll(cmdparams, 0);
	return NS_SUCCESS;
}

/* 
 * ReRolls Selected Dice
*/
void reroll(CmdParams* cmdparams, int rolltype) {
	GameData *gd;
	char *ydiec[5];
	char *dienums;
	int keepdie[5];
	char *tstdni;
	char *buf;
	int kdt1;
	int kdt2;

	int i, dc;
	
	if (!GetUserModValue(cmdparams->source)) {
		return;
	}
	gd = (GameData *)GetChannelModValue(cmdparams->channel);
	if (!gd) {
		return;
	}
	if (gd->pd[gd->currentplayer]->u != cmdparams->source) {
		return;
	}
	if (gd->gamestatus != YS_GAME_PLAYING) {
		return;
	}
	if (gd->currentroll < 1 || gd->currentroll > 2) {
		return;
	}
	buf= joinbuf(cmdparams->av, cmdparams->ac, 0);
	strip_mirc_codes(buf);
	for (i = 0; i < 5; i++) {
		if ((!strchr(buf, (i + 49)) == NULL && rolltype) || (strchr(buf, (i + 49)) == NULL && !rolltype)) {
			gd->dice[i]= rand() % 6;
			dc = 4;
		} else {
			dc = 11;
		}
		if (gd->dice[i] == 0) {
			ircsnprintf(dicetext[i], 15, "\003%dOne", dc);
		} else if (gd->dice[i] == 1) {
			ircsnprintf(dicetext[i], 15, "\003%dTwo", dc);
		} else if (gd->dice[i] == 2) {
			ircsnprintf(dicetext[i], 15, "\003%dThree", dc);
		} else if (gd->dice[i] == 3) {
			ircsnprintf(dicetext[i], 15, "\003%dFour", dc);
		} else if (gd->dice[i] == 4) {
			ircsnprintf(dicetext[i], 15, "\003%dFive", dc);
		} else if (gd->dice[i] == 5) {
			ircsnprintf(dicetext[i], 15, "\003%dSix", dc);
		}
	}
	gd->currentroll++;
	irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0037%s\0039 this is roll\00311 %d\0039, you rolled %s %s %s %s %s", cmdparams->source->name, gd->currentroll, dicetext[0], dicetext[1], dicetext[2], dicetext[3], dicetext[4]);
	if (gd->currentroll == 3) {
		irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0039That was your last roll this turn\0037 %s\0039, select which score to take", cmdparams->source->name);
	}
	ns_free(buf);
	return;
}
