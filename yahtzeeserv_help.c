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

const char *ys_help_set_chan[] = {
	"\2CHAN <#Channel>\2 - Set Channel Yahtzee Games Play in",
	NULL
};

/*
 * Help Text
*/
const char ys_help_start_oneline[] = "Start a Game in Channel";
const char ys_help_stop_oneline[] = "Stops Game";
const char ys_help_join_oneline[] = "Join a game currently Starting";
const char ys_help_remove_oneline[] = "Remove From Game";
const char ys_help_players_oneline[] = "Display Player List In Channel";
const char ys_help_turn_oneline[] = "Display Current Player In Channel";
const char ys_help_pass_oneline[] = "Passes game to the specified nick";
const char ys_help_dice_oneline[] = "Shows your currently rolled dice";
const char ys_help_sheet_oneline[] = "Shows yuour current score sheet";
const char ys_help_roll_oneline[] = "Rolls the specified Dice, keeping the rest";
const char ys_help_keep_oneline[] = "Keeps the specified Dice, and rolld the rest";
const char ys_help_score_oneline[] = "Scores to the entry on the score sheet";
const char ys_help_top10_oneline[] = "Shows the Top 10 Score Lists in channel";
const char ys_help_high_oneline[] = "Displays the Overall High Score Page Selected, In Channel";
const char ys_help_rules_oneline[] = "Displays the rules pages";

const char *ys_help_start[] = {
	"Syntax: \2START\2",
	"",
	"Starts Yahtzee Game in configured Channel.",
	NULL
};

const char *ys_help_stop[] = {
	"Syntax: \2STOP\2",
	"",
	"Stops Currently Running Yahtzee Game.",
	"Only Available to a Current Player, if",
	"only one player playing. otherwise players",
	"should use REMOVE to leave the game.",
	NULL
};

const char *ys_help_join[] = {
	"Syntax: \2JOIN\2",
	"",
	"Joins you to the Currently Starting Game.",
	"",
	"NOTE: Joins are allowed once game has",
	"started, only untill each player has",
	"scored their first points",
	NULL
};

const char *ys_help_remove[] = {
	"Syntax: \2REMOVE <nick>\2",
	"",
	"With No Nick specified, removes you from the current game.",
	"",
	"With Nick specified, removes Nick from the curent Game, as",
	"long as Nick is no longer connected to the network.",
	NULL
};

const char *ys_help_players[] = {
	"Syntax: \2PLAYERS\2",
	"",
	"Displays List of Current Players in channel.",
	NULL
};

const char *ys_help_turn[] = {
	"Syntax: \2TURN\2",
	"",
	"Shows Current Player waiting to roll dice, or score.",
	NULL
};

const char *ys_help_pass[] = {
	"Syntax: \2PASS\2 <nick>",
	"",
	"Passes your game to nick",
	"nick MUST be connected to the network.",
	NULL
};

const char *ys_help_dice[] = {
	"Syntax: \2DICE\2",
	"",
	"Shows the dice you have currently",
	"rolled, in channel",
	NULL
};

const char *ys_help_sheet[] = {
	"Syntax: \2SHEET\2 <nick>",
	"",
	"Shows your current Score Sheet in channel",
	NULL
};

const char *ys_help_roll[] = {
	"Syntax: \2ROLL\2 <#> <#> <#> <#> <#>",
	"",
	"Rolls the specified Dice",
	"",
	"NOTE: Dice are numbered 1 to 5, from left to right",
	"the position number should be used, not the number showing",
	NULL
};

const char *ys_help_keep[] = {
	"Syntax: \2KEEP\2 <#> <#> <#> <#> <#>",
	"",
	"Keeps the specified Dice, and rolls the rest",
	"",
	"NOTE: Dice are numbered 1 to 5, from left to right",
	"the position number should be used, not the number showing",
	NULL
};

const char *ys_help_score[] = {
	"Syntax: \2SCORE\2 <entry>",
	"",
	"Scores to the specified entry on the score sheet",
	"",
	"Valid Entires are :",
	"1 , 2 , 3 , 4 , 5 , 6 , fh",
	"ss , ls , 3k , 4k , c , y",
	NULL
};

const char *ys_help_top10[] = {
	"Syntax: \2TOP10\2 <?>",
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
	"Syntax: \2HIGH\2 <#>",
	"",
	"Displays the Overall High Score Page Specified, in Channel",
	"Valid Pages are numbered from 1 to 5",
	NULL
};

const char *ys_help_rules[] = {
	"Syntax: \2RULES\2 <#>",
	"",
	"Displays the selected Rules Page",
	"Valid page numbers are from 1 to 4",
	NULL
};
