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

/*
 * Nick Change Check
*/
int PlayerNickChange (CmdParams* cmdparams)
{
	if (currentyahtzeeplayercount < 1) {
		return NS_SUCCESS;
	}
	for (ypln = 0; ypln < currentyahtzeeplayercount; ypln++) {
		if (!ircstrcasecmp (ysplayernick[ypln], cmdparams->param)) {
			strlcpy(ysplayernick[ypln], cmdparams->source->name, MAXNICK);
		}
	}
	return NS_SUCCESS;
}

/*
 * Display Rules
*/
int ShowRulePages (CmdParams* cmdparams)
{
	if (cmdparams->ac < 1) {
		irc_prefmsg (ys_bot, cmdparams->source, "Welcome To Yahtzee %s", cmdparams->source->name);
		irc_prefmsg (ys_bot, cmdparams->source, " ");
		irc_prefmsg (ys_bot, cmdparams->source, "Written By DeadNotBuried");
		irc_prefmsg (ys_bot, cmdparams->source, " ");
		irc_prefmsg (ys_bot, cmdparams->source, "There are currently 4 rule pages");
		irc_prefmsg (ys_bot, cmdparams->source, "To view each page use the rules command");
		irc_prefmsg (ys_bot, cmdparams->source, "placing the page number after the command");
		irc_prefmsg (ys_bot, cmdparams->source, "E.G. '!rules 2' to display page 2.");
	} else {
		if (!ircstrcasecmp (cmdparams->av[0], "1")) {
			irc_prefmsg (ys_bot, cmdparams->source, "Objective of the Game");
			irc_prefmsg (ys_bot, cmdparams->source, "---------------------");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "Yahtzee can be played alone or in a group.");
			irc_prefmsg (ys_bot, cmdparams->source, "The Game being the same for groups, but with players taking");
			irc_prefmsg (ys_bot, cmdparams->source, "turns to roll the dice, with the highest score winning.");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "The game consists of 13 turns.");
			irc_prefmsg (ys_bot, cmdparams->source, "In each turn, you roll the dice and then score the roll in one of 13 slots.");
			irc_prefmsg (ys_bot, cmdparams->source, "You must score once in each slot which means that some slots may have to be");
			irc_prefmsg (ys_bot, cmdparams->source, "sacrificed (score 0 in).");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "Each Slot has its own rules for scoring, which will be explained below");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "The object of the game is to maximize your total score.");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "After all Players have scored in all 13 Slots, the game ends.");
		} else if (!ircstrcasecmp (cmdparams->av[0], "2")) {
			irc_prefmsg (ys_bot, cmdparams->source, "Rolling the Dice");
			irc_prefmsg (ys_bot, cmdparams->source, "----------------");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "Yahtzee is played with 5 die, each die is referenced by");
			irc_prefmsg (ys_bot, cmdparams->source, "it's position and not the number it rolls. The Dice are numbered");
			irc_prefmsg (ys_bot, cmdparams->source, "from left to right in their postions as 1 through 5");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "The Dice are rolled automatically for you when it is your turn.");
			irc_prefmsg (ys_bot, cmdparams->source, "You then have 2 further chances to improve your score by");
			irc_prefmsg (ys_bot, cmdparams->source, "rolling or keeping the die.");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "After the third roll (or before if you wish) you must select");
			irc_prefmsg (ys_bot, cmdparams->source, "a slot to score against, your score will be automatically");
			irc_prefmsg (ys_bot, cmdparams->source, "calculated including any bonus points, and added to your Total.");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "Play will then continue with the next Player.");
		} else if (!ircstrcasecmp (cmdparams->av[0], "3")) {
			irc_prefmsg (ys_bot, cmdparams->source, "Scoring - Part 1");
			irc_prefmsg (ys_bot, cmdparams->source, "----------------");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "Once you have the die combination you wish, score the roll in");
			irc_prefmsg (ys_bot, cmdparams->source, "one of the 13 slots. Do this by typing Score and the slot code");
			irc_prefmsg (ys_bot, cmdparams->source, "you wish to score in. Each slot may only be used once per game.");
			irc_prefmsg (ys_bot, cmdparams->source, "To see the score slot codes, type score without a slot code.");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "Each Slot has its own scoring rules as listed:");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "Upper Slots:");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "One's through to Six's: Total of the die matching the score slot.");
			irc_prefmsg (ys_bot, cmdparams->source, "E.G. rolling a 4 3 4 6 4 and scoring in Four's would give 12 points.");
			irc_prefmsg (ys_bot, cmdparams->source, "The same roll scored in the One's Two's or Fives slot would score 0,");
			irc_prefmsg (ys_bot, cmdparams->source, "3 points for the Three's slot, or 6 points for the Six's slot.");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "A Bonus 35 points is received if scoring 63 points or more in One's To Six's");
			irc_prefmsg (ys_bot, cmdparams->source, "NOTE: Scoring 3 die in each slot will equal 63 points, enough for the bonus.");
		} else if (!ircstrcasecmp (cmdparams->av[0], "4")) {
			irc_prefmsg (ys_bot, cmdparams->source, "Scoring - Part 2");
			irc_prefmsg (ys_bot, cmdparams->source, "----------------");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "Lower Slots:");
			irc_prefmsg (ys_bot, cmdparams->source, "Score is 0 if requirements not met for the slot.");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "Full House : 25 points (Three Die the Same, and Two Die the same)");
			irc_prefmsg (ys_bot, cmdparams->source, "Short Straight : 30 points (Four Die in order, E.G. 3 4 5 6)");
			irc_prefmsg (ys_bot, cmdparams->source, "Long Straight : 40 points  (All Five Die in order, E.G. 1 2 3 4 5)");
			irc_prefmsg (ys_bot, cmdparams->source, "3 of a Kind : Total of all Die (Three Die must be the same)");
			irc_prefmsg (ys_bot, cmdparams->source, "4 of a Kind : Total of all Die (Four Die must be the same)");
			irc_prefmsg (ys_bot, cmdparams->source, "Chance : Total of all Die (No Requirements)");
			irc_prefmsg (ys_bot, cmdparams->source, "Yahtzee : 50 points (ALL Die must be the same)");
			irc_prefmsg (ys_bot, cmdparams->source, " ");
			irc_prefmsg (ys_bot, cmdparams->source, "Yahtzee Bonus : If after Scoring 50 points on Yahtzee, another");
			irc_prefmsg (ys_bot, cmdparams->source, "Yahtzee is rolled a Bonus 100 points is added to your total.");
			irc_prefmsg (ys_bot, cmdparams->source, "NOTE: You must receive points when scoring to receive the Bonus.");
		}
	}
	return NS_SUCCESS;
}

/*
 * Display Top10 lists
*/
int ShowTop10Lists (CmdParams* cmdparams)
{
	if (cmdparams->ac < 1) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039Top 10 Scores so far\0038 :");
		for (ypln = 0; ypln < 10; ypln++) {
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039 %d\0038  :\0037 %s \0039with\00311 %d\0039 points.", (ypln + 1), htsslrnick[ypln], htsslr[ypln]);
		}
	} else {
		if (!ircstrcasecmp (cmdparams->av[0], "d")) {
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039Top 10 Scores so far Today\0038 :");
			for (ypln = 0; ypln < 10; ypln++) {
				irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039 %d\0038  :\0037 %s \0039with\00311 %d\0039 points.", (ypln + 1), dhtsslrnick[ypln], dhtsslr[ypln]);
			}
		} else if (!ircstrcasecmp (cmdparams->av[0], "w")) {
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039Top 10 Scores so far This Week\0038 :");
			for (ypln = 0; ypln < 10; ypln++) {
				irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039 %d\0038  :\0037 %s \0039with\00311 %d\0039 points.", (ypln + 1), whtsslrnick[ypln], whtsslr[ypln]);
			}
		} else if (!ircstrcasecmp (cmdparams->av[0], "m")) {
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039Top 10 Scores so far This Month\0038 :");
			for (ypln = 0; ypln < 10; ypln++) {
				irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039 %d\0038  :\0037 %s \0039with\00311 %d\0039 points.", (ypln + 1), mhtsslrnick[ypln], mhtsslr[ypln]);
			}
		}
	}
	return NS_SUCCESS;
}

/*
 * Start Game
*/
int StartYahtzeeGame (CmdParams* cmdparams)
{
	if (currentyahtzeegamestatus == YS_GAME_STOPPED) {
		startcountdowntimer(cmdparams->source->name);
	} else if (currentyahtzeeplayercount < 10) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0034A game has already started \0037%s\0034, Type '\2\003Join\2\0034' To Join in.", cmdparams->source->name);
	} else {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0034A game has already started \0037%s\0034 and all spots are taken. Please try the next game.", cmdparams->source->name);
	}
	return NS_SUCCESS;
}

/*
 * Stop Game
*/
int StopYahtzeeGame (CmdParams* cmdparams)
{
	if (currentyahtzeegamestatus != YS_GAME_STOPPED) {
		for (ypln = 0; ypln < currentyahtzeeplayercount; ypln++) {
			if (!ircstrcasecmp(ysplayernick[ypln], cmdparams->source->name)) {
				if ((currentyahtzeeplayercount == 1) || (currenthand == 1)) {
					stopyahtzee();
					irc_chanprivmsg (ys_bot, yahtzeeroom, "\0034Yahtzee has been stopped by \0037%s.", cmdparams->source->name);
				} else {
					irc_chanprivmsg (ys_bot, yahtzeeroom, "\0034Unable to stop Yahtzee when more than one person playing, please use !remove if you wish to exit the game \0037%s.", cmdparams->source->name);
				}
				ypln += currentyahtzeeplayercount;
			}
		}
	}
	return NS_SUCCESS;
}

/*
 * Pass Game To Nick
*/
int PassYahtzeeGame (CmdParams* cmdparams)
{
	Client *u;
	int dpe=0;

	if (currentyahtzeeplayercount < 1) {
		return NS_SUCCESS;
	}
	for (ypln = 0; ypln < currentyahtzeeplayercount; ypln++) {
		if (!ircstrcasecmp(ysplayernick[ypln], cmdparams->source->name)) {
			dpe += 1;
		}
	}
	if (dpe < 1) {
		return NS_SUCCESS;
	}
	u = FindUser(cmdparams->av[0]);
	if (!u) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "Sorry %s , The Nick %s is not currently Online", cmdparams->source->name, cmdparams->av[0]);
		return NS_SUCCESS;
	} else if ( IsExcluded(u) || IsMe(u) || IsBot(u) || IsAway(u) ) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "Sorry %s , %s Can't play your game for you at this time", cmdparams->source->name, u->name);
		return NS_SUCCESS;
	} else if (!ircstrcasecmp(cmdparams->source->name, u->name)) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "Just play your hand %s and stop trying to be smart", cmdparams->source->name);
		return NS_SUCCESS;
	}
	for (ypln = 0; ypln < currentyahtzeeplayercount; ypln++) {
		if (!ircstrcasecmp(ysplayernick[ypln], cmdparams->source->name)) {
			strlcpy(ysplayernick[ypln], u->name, MAXNICK);
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s\0039 has passed their game to\0037 %s", cmdparams->source->name, u->name);
		}
	}
	return NS_SUCCESS;
}

/*
 * Join Player To Game
*/
int JoinYahtzeeGame (CmdParams* cmdparams) {
	if (currentyahtzeegamestatus == YS_GAME_STARTING || (currentyahtzeegamestatus == YS_GAME_PLAYING && currenthand < 2)) {
		if (currentyahtzeeplayercount < 10) {
			for (ypln = 0; ypln < 10; ypln++) {
				if (!ircstrcasecmp(ysplayernick[ypln], cmdparams->source->name)) {
					return NS_SUCCESS;
				}
			}
			strlcpy(ysplayernick[currentyahtzeeplayercount], cmdparams->source->name, MAXNICK);
			currentyahtzeeplayercount++;
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0038Welcome to Yahtzee \0037%s", cmdparams->source->name);
		} else {
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0034Sorry all places are filled \0037%s\0034, your welcome to try the next game though", cmdparams->source->name);
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
	char nic[MAXNICK];
	int tfrpacp;
	int tfrpacpn;
	int yplnht;
	int cpfp;
	tfrpacp= 0;
	tfrpacpn= 0;

	if (currentyahtzeeplayercount < 1) {
		return NS_SUCCESS;
	}
	if (currentyahtzeegamestatus != YS_GAME_STARTING && currentyahtzeegamestatus != YS_GAME_PLAYING) {
		return NS_SUCCESS;
	}
	if (cmdparams->ac < 1) {
		strlcpy(nic, cmdparams->source->name, MAXNICK);
	} else {
		u = FindUser(cmdparams->av[0]);
		if (u) {
			return NS_SUCCESS;
		} else {
			strlcpy(nic, cmdparams->av[0], MAXNICK);
		}
	}
	for (ypln = 0; ypln < currentyahtzeeplayercount; ypln++) {
		if (!ircstrcasecmp(ysplayernick[ypln], nic)) {
			if (ypln == currentplayer) {
				tfrpacp= 1;
				if (currentplayer == 0) {
					currenthand--;
				}
			} else if (ypln < currentplayer) {
				tfrpacp= 2;
				tfrpacpn= ypln;
			}
			for (yplnh = ypln; yplnh < currentyahtzeeplayercount; yplnh++) {
				cpfp= (yplnh + 1);
				if (cpfp == currentyahtzeeplayercount) {
					strlcpy(ysplayernick[yplnh], " ", MAXNICK);
					ysplayerscore[yplnh]= 0;
					for (yplnht = 0; yplnht < 15; yplnht++) {
						ysplayerhand[yplnh][yplnht]= 0;
						if (yplnht < 13) {
							ysplayerhandused[yplnh][yplnht]= "x";
						}
					}
				} else {
					strlcpy(ysplayernick[yplnh], ysplayernick[cpfp], MAXNICK);
					ysplayerscore[yplnh]= ysplayerscore[cpfp];
					for (yplnht = 0; yplnht < 15; yplnht++) {
						ysplayerhand[yplnh][yplnht]= ysplayerhand[cpfp][yplnht];
						if (yplnht < 13) {
							if (!ircstrcasecmp(ysplayerhandused[cpfp][yplnht], "x")) {
								ysplayerhandused[yplnh][yplnht]= "x";
							} else {
								ysplayerhandused[yplnh][yplnht]= "";
							}
						}
					}
				}
			}
			currentyahtzeeplayercount--;
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s \0038Removed from the current game of Yahtzee", nic);
		}
	}
	if (currentyahtzeeplayercount < 1) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0034Last player removed from Game, Stopping Current Game.");
		stopyahtzee();
	} else if (tfrpacp == 1) {
		if (currentplayer > (currentyahtzeeplayercount - 1)) {
			currentplayer= 0;
		}
		currentroll= 0;
		if (currentyahtzeegamestatus != YS_GAME_STARTING) {
			yrolldie();
		}
	} else if (tfrpacp == 2) {
		if (currentplayer > tfrpacpn) {
			currentplayer--;
		}
	}
	return NS_SUCCESS;
}

/*
 * Display Current Players
*/
int ShowPlayersYahtzeeGame (CmdParams* cmdparams)
{
	if (currentyahtzeegamestatus == YS_GAME_PLAYING) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039Current Players are\0038 :\0037 %s %s %s %s %s %s %s %s %s %s", ysplayernick[0], ysplayernick[1], ysplayernick[2], ysplayernick[3], ysplayernick[4], ysplayernick[5], ysplayernick[6], ysplayernick[7], ysplayernick[8], ysplayernick[9]);
	}
	return NS_SUCCESS;
}

/*
 * Display Current Players Turn
*/
int ShowTurnYahtzeeGame (CmdParams* cmdparams)
{
	if (currentyahtzeegamestatus == YS_GAME_PLAYING) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039The current Player is \0038 :\0037 %s", ysplayernick[currentplayer]);
	}
	return NS_SUCCESS;
}

/*
 * Show Players Score Sheet
*/
int ShowYahtzeeSheet (CmdParams* cmdparams)
{
	if (currentyahtzeegamestatus == YS_GAME_PLAYING) {
		for (ypln = 0; ypln < 10; ypln++) {
			if (!ircstrcasecmp(ysplayernick[ypln], cmdparams->source->name)) {
				irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s\0038 your current scores are :\0039 %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0034\2%s\2\0039 , %s\00311 %d\0039 , Total\00311 %d",
					cmdparams->source->name, ysscoretype[0], ysplayerhand[ypln][0], ysplayerhandused[ypln][0], ysscoretype[1], ysplayerhand[ypln][1], ysplayerhandused[ypln][1], ysscoretype[2], ysplayerhand[ypln][2], ysplayerhandused[ypln][2], ysscoretype[3], ysplayerhand[ypln][3]
					, ysplayerhandused[ypln][3], ysscoretype[4], ysplayerhand[ypln][4], ysplayerhandused[ypln][4], ysscoretype[5], ysplayerhand[ypln][5], ysplayerhandused[ypln][5], ysscoretype[13], ysplayerhand[ypln][13], ysscoretype[6], ysplayerhand[ypln][6], ysplayerhandused[ypln][6]
					, ysscoretype[7], ysplayerhand[ypln][7], ysplayerhandused[ypln][7], ysscoretype[8], ysplayerhand[ypln][8], ysplayerhandused[ypln][8], ysscoretype[9], ysplayerhand[ypln][9], ysplayerhandused[ypln][9], ysscoretype[10], ysplayerhand[ypln][10], ysplayerhandused[ypln][10]
					, ysscoretype[11], ysplayerhand[ypln][11], ysplayerhandused[ypln][11], ysscoretype[12], ysplayerhand[ypln][12], ysplayerhandused[ypln][12], ysscoretype[14], ysplayerhand[ypln][14], ysplayerscore[ypln]);
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
	if (currentyahtzeegamestatus == YS_GAME_PLAYING && !ircstrcasecmp(cmdparams->source->name, ysplayernick[currentplayer])) {
		getdieroll();
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s\0039 your last roll was roll \00311 %d\0039, you have \00311 %s\0039 showing", ysplayernick[currentplayer], currentroll, tmpstr);
	}
	return NS_SUCCESS;
}

/*
 * Score to Sheet Entry
*/
int ScoreYahtzeeDice (CmdParams* cmdparams)
{
	if (currentyahtzeegamestatus != YS_GAME_PLAYING || ircstrcasecmp(cmdparams->source->name,ysplayernick[currentplayer])) {
		return NS_SUCCESS;
	}
	yscore(cmdparams->av,cmdparams->ac);
	return NS_SUCCESS;
}

/*
 * Roll Dice
*/
int RollYahtzeeDice (CmdParams* cmdparams)
{
	if (currentyahtzeegamestatus != YS_GAME_PLAYING || ircstrcasecmp(cmdparams->source->name,ysplayernick[currentplayer]) || currentroll > 2) {
		return NS_SUCCESS;
	}
	reroll(cmdparams->av,cmdparams->ac,"roll");
	return NS_SUCCESS;
}

/*
 * Keep Dice
*/
int KeepYahtzeeDice (CmdParams* cmdparams)
{
	if (currentyahtzeegamestatus != YS_GAME_PLAYING || ircstrcasecmp(cmdparams->source->name,ysplayernick[currentplayer]) || currentroll > 2) {
		return NS_SUCCESS;
	}
	reroll(cmdparams->av,cmdparams->ac,"keep");
	return NS_SUCCESS;
}

/*
 * Display High Score Pages
*/
int ShowHighList (CmdParams* cmdparams)
{
	yplnh= 0;
	if (cmdparams->ac < 1) {
		yplnh= 1;
	} else {
		if (!ircstrcasecmp (cmdparams->av[0], "5")) {
			yplnh= 81;
		} else if (!ircstrcasecmp (cmdparams->av[0], "4")) {
			yplnh= 61;
		} else if (!ircstrcasecmp (cmdparams->av[0], "3")) {
			yplnh= 41;
		} else if (!ircstrcasecmp (cmdparams->av[0], "2")) {
			yplnh= 21;
		} else {
			yplnh= 1;
		}
	}
	if (yplnh > 0) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039Top Scores so far %d-%d\0038 :", yplnh, (yplnh + 19));
		for (ypln = (yplnh - 1); ypln < (yplnh + 19); ypln++) {
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039 %d\0038  :\0037 %s \0039with\00311 %d\0039 points.", (ypln + 1), htsslrnick[ypln], htsslr[ypln]);
		}
	}
	return NS_SUCCESS;
}

/*
 * Timer Procedures
*/


/*
 * Start Game
*/

int startyahtzee(void) {
	if (currentyahtzeegamestatus == YS_GAME_STARTING) {
		if (currentyahtzeeplayercount < 1) {
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0034No Players joined to current Game, Exiting");
			currentyahtzeegamestatus = YS_GAME_STOPPED;
			stopyahtzee();
			return NS_SUCCESS;
		}
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\00310Yahtzee is now starting, current players are \0037%s %s %s %s %s %s %s %s %s %s", ysplayernick[0], ysplayernick[1], ysplayernick[2], ysplayernick[3], ysplayernick[4], ysplayernick[5], ysplayernick[6], ysplayernick[7], ysplayernick[8], ysplayernick[9]);
		currentplayer= 0;
		currentyahtzeegamestatus = YS_GAME_PLAYING;
		yrolldie();
	}
	return NS_SUCCESS;
}

/*
 * Reset Score Lists
*/
int yahtzeeday(void) {
	irc_chanalert (ys_bot, "\0038It's Midnight Here : \0037Clearing Daily Top 10 List");
	irc_chanprivmsg (ys_bot, yahtzeeroom, "\0038It's Midnight Here : \0037Clearing Daily Top 10 List");
	clearyahtzeedaily();
	return NS_SUCCESS;
}

int yahtzeeweek(void) {
	irc_chanalert (ys_bot, "\0038It's Sunday Here : \0037Clearing Weekly Top 10 List");
	irc_chanprivmsg (ys_bot, yahtzeeroom, "\0038It's Sunday Here : \0037Clearing Weekly Top 10 List");
	clearyahtzeeweekly();
	return NS_SUCCESS;
}

int yahtzeemonth(void) {
	irc_chanalert (ys_bot, "\0038It's The First Here : \0037Clearing Monthly Top 10 List");
	irc_chanprivmsg (ys_bot, yahtzeeroom, "\0038It's The First Here : \0037Clearing Monthly Top 10 List");
	clearyahtzeemonthly();
	return NS_SUCCESS;
}

