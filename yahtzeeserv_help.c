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

#include "neostats.h"    /* Required for bot support */
#include "yahtzeeserv.h"

const char *ys_help_set_exclusions[] = {
	"\2EXCLUSIONS <ON|OFF>\2 - Use Global Exclusion Lists",
	NULL
};

const char *ys_help_set_chan[] = {
	"\2CHAN <#Channel>\2 - Set Main Channel Yahtzee Games Play in",
	NULL
};

const char *ys_help_set_multichan[] = {
	"\2MULTICHAN <ON|OFF>\2 - Enable games outside Main Game Channel.",
	NULL
};

const char *ys_help_set_chanoponly[] = {
	"\2CHANOPONLY <ON|OFF>\2 - Restrict starting of games outside main channel to ChanOps only.",
	NULL
};

const char *ys_help_start[] = {
	"Start Game",
	"Syntax: \2START\2",
	"Syntax: \2START <#channel>\2",
	"",
	"!Start in the configured Game Channel starts a game.",
	"",
	"Start <#Channel> starts a game in the specified #Channel",
	NULL
};

const char *ys_help_stop[] = {
	"Stops Game",
	"Syntax: \2STOP\2",
	"",
	"Stops Currently Running Yahtzee Game.",
	"Only Available to a Current Player, if",
	"only one player playing. otherwise players",
	"should use REMOVE to leave the game.",
	"",
	"NOTE: ChanOP/IRCop access overrides command restrictions.",
	"",
	"This command is used in Channel ONLY.",
	NULL
};

const char *ys_help_join[] = {
	"Join a game currently Starting",
	"Syntax: \2JOIN\2",
	"",
	"Joins you to the Currently Starting Game.",
	"",
	"NOTE: Joins are allowed once game has",
	"started, only untill each player has",
	"scored their first points",
	"",
	"This command is used in Channel ONLY.",
	NULL
};

const char *ys_help_remove[] = {
	"Remove From Game",
	"Syntax: \2REMOVE\2",
	"Syntax: \2REMOVE <nick>\2 (ChanOP/IRCop Only)",
	"",
	"Removes you from the current game.",
	"",
	"With Nick specified, removes Nick from the curent Game.",
	"",
	"This command is used in Channel ONLY.",
	NULL
};

const char *ys_help_players[] = {
	"Display Player List In Channel",
	"Syntax: \2PLAYERS\2",
	"",
	"Displays List of Current Players in channel.",
	"",
	"This command is used in Channel ONLY.",
	NULL
};

const char *ys_help_turn[] = {
	"Display Current Player In Channel",
	"Syntax: \2TURN\2",
	"",
	"Shows Current Player waiting to roll dice, or score.",
	"",
	"This command is used in Channel ONLY.",
	NULL
};

const char *ys_help_pass[] = {
	"Passes game to the specified nick",
	"Syntax: \2PASS\2 <nick>",
	"",
	"Passes your game to nick. the nick MUST be",
	"connected to the network, and in the channel.",
	"",
	"This command is used in Channel ONLY.",
	NULL
};

const char *ys_help_dice[] = {
	"Shows your currently rolled dice",
	"Syntax: \2DICE\2",
	"",
	"Shows the dice you have currently",
	"rolled, in channel",
	"",
	"This command is used in Channel ONLY.",
	NULL
};

const char *ys_help_sheet[] = {
	"Shows your current score sheet",
	"Syntax: \2SHEET\2",
	"",
	"Shows your current Score Sheet in channel",
	"",
	"This command is used in Channel ONLY.",
	NULL
};

const char *ys_help_roll[] = {
	"Rolls the specified Dice, keeping the rest",
	"Syntax: \2ROLL\2 <#> <#> <#> <#> <#>",
	"",
	"Rolls the specified Dice",
	"",
	"NOTE: Dice are numbered 1 to 5, from left to right",
	"the position number should be used, not the number showing",
	"",
	"This command is used in Channel ONLY.",
	NULL
};

const char *ys_help_keep[] = {
	"Keeps the specified Dice, and rolls the rest",
	"Syntax: \2KEEP\2 <#> <#> <#> <#> <#>",
	"",
	"Keeps the specified Dice, and rolls the rest",
	"",
	"NOTE: Dice are numbered 1 to 5, from left to right",
	"the position number should be used, not the number showing",
	"",
	"This command is used in Channel ONLY.",
	NULL
};

const char *ys_help_score[] = {
	"Scores to the entry on the score sheet",
	"Syntax: \2SCORE\2 <entry>",
	"",
	"Scores to the specified entry on the score sheet",
	"",
	"Valid Entires are :",
	"1 , 2 , 3 , 4 , 5 , 6 , fh",
	"ss , ls , 3k , 4k , c , y",
	"",
	"This command is used in Channel ONLY.",
	NULL
};

const char *ys_help_top10[] = {
	"Shows the Top 10 Score Lists in channel",
	"Syntax: \2TOP10 <?>\2",
	"",
	"Displays the Top 10 Score Lists in channel",
	"",
	"With no parameters the Top 10 overall is listed.",
	"\"top10 d\" Lists the Top 10 Scores for the current day",
	"\"top10 w\" Lists the Top 10 Scores for the current week",
	"\"top10 m\" Lists the Top 10 Scores for the current month",
	NULL
};

const char *ys_help_high[] = {
	"Displays the Overall High Score Page Selected, In Channel",
	"Syntax: \2HIGH\2 <#>",
	"",
	"Displays the Overall High Score Page Specified, in Channel",
	"Valid Pages are numbered from 1 to 5",
	NULL
};

const char *ys_help_rules[] = {
	"Displays the rules pages",
	"Syntax: \2RULES\2 <#>",
	"",
	"Displays the selected Rules Page",
	"Valid page numbers are from 1 to 4",
	NULL
};
const char *ys_help_rules_page_0[] = {
	"There are currently 4 rule pages",
	"To view each page use the rules command",
	"placing the page number after the command",
	"E.G. '!rules 2' to display page 2.",
	NULL
};

const char *ys_help_rules_page_1[] = {
	"Objective of the Game",
	"---------------------",
	" ",
	"Yahtzee can be played alone or in a group.",
	"The Game being the same for groups, but with players taking",
	"turns to roll the dice, with the highest score winning.",
	" ",
	"The game consists of 13 turns.",
	"In each turn, you roll the dice and then score the roll in one of 13 slots.",
	"You must score once in each slot which means that some slots may have to be",
	"sacrificed (score 0 in).",
	" ",
	"Each Slot has its own rules for scoring, which will be explained below",
	" ",
	"The object of the game is to maximize your total score.",
	" ",
	"After all Players have scored in all 13 Slots, the game ends.",
	NULL
};

const char *ys_help_rules_page_2[] = {
	"Rolling the Dice",
	"----------------",
	" ",
	"Yahtzee is played with 5 dice, each dice is referenced by",
	"it's position and not the number it rolls. The Dice are numbered",
	"from left to right in their postions as 1 through 5",
	" ",
	"The Dice are rolled automatically for you when it is your turn.",
	"You then have 2 further chances to improve your score by",
	"rolling or keeping the dice.",
	" ",
	"After the third roll (or before if you wish) you must select",
	"a slot to score against, your score will be automatically",
	"calculated including any bonus points, and added to your Total.",
	" ",
	"Play will then continue with the next Player.",
	NULL
};

const char *ys_help_rules_page_3[] = {
	"Scoring - Part 1",
	"----------------",
	" ",
	"Once you have the dice combination you wish, score the roll in",
	"one of the 13 slots. Do this by typing !Score and the slot code",
	"you wish to score in. Each slot may only be used once per game.",
	"To see the score slot codes, type !score without a slot code.",
	" ",
	"Each Slot has its own scoring rules as listed:",
	" ",
	"Upper Slots:",
	" ",
	"One's through to Six's: Total of the dice matching the score slot.",
	"E.G. rolling a 4 3 4 6 4 and scoring in Four's would give 12 points.",
	"The same roll scored in the One's Two's or Fives slot would score 0,",
	"3 points for the Three's slot, or 6 points for the Six's slot.",
	" ",
	"A Bonus 35 points is received if scoring 63 points or more in One's To Six's",
	"NOTE: Scoring 3 dice in each slot will equal 63 points, enough for the bonus.",
	NULL
};

const char *ys_help_rules_page_4[] = {
	"Scoring - Part 2",
	"----------------",
	" ",
	"Lower Slots:",
	"Score is 0 if requirements not met for the slot.",
	" ",
	"Full House : 25 points (Three dice the Same, and Two dice the same)",
	"Short Straight : 30 points (Four dice in order, E.G. 3 4 5 6)",
	"Long Straight : 40 points  (All Five dice in order, E.G. 1 2 3 4 5)",
	"3 of a Kind : Total of all dice (Three dice must be the same)",
	"4 of a Kind : Total of all dice (Four dice must be the same)",
	"Chance : Total of all dice (No Requirements)",
	"Yahtzee : 50 points (ALL dice must be the same)",
	" ",
	"Yahtzee Bonus : If after Scoring 50 points on Yahtzee, another",
	"Yahtzee is rolled a Bonus 100 points is added to your total.",
	"NOTE: You must receive points when scoring to receive the Bonus.",
	NULL
};
