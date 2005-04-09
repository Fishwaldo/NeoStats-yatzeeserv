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
 * Set Main Yahtzee Game Channel
*/
int ys_cmd_set_chan(CmdParams* cmdparams, SET_REASON reason) {
	Channel *c, *c2;
	GameData *gd, *gd2;

	if (reason == SET_VALIDATE) {
		c = FindChannel(YahtzeeServ.yahtzeeroom);
		if (!c) { 
			irc_prefmsg (ys_bot, cmdparams->source, "Critical Error : Unable to find main game channel online.");
			return NS_FAILURE;
		}
		gd = (GameData *)GetChannelModValue(c);
		if (gd) {
			if (gd->gamestatus != YS_GAME_STOPPED) {
				irc_prefmsg (ys_bot, cmdparams->source, "Unable to change Game Channel while Game in Progress.");
				return NS_FAILURE;
			}
		}
		c2 = FindChannel(cmdparams->av[1]);
		if (!c2) {
			irc_prefmsg (ys_bot, cmdparams->source, "Target Channel MUST be online.");
			return NS_FAILURE;
		}
		if (c == c2) {
			irc_prefmsg (ys_bot, cmdparams->source, "That's just stupid %s, I'm already using %s as the main game channel.", cmdparams->source->name, cmdparams->av[1]);
			return NS_FAILURE;
		}
		gd2 = (GameData *)GetChannelModValue(c2);
		if (gd2) {
			irc_prefmsg (ys_bot, cmdparams->source, "Unable to change main Game channel to a channel currently hosting a Game, Try again after the game has finished.");
			return NS_FAILURE;
		}
		irc_chanalert (ys_bot, "Game Channel Changing to %s , Parting %s (%s)", cmdparams->av[1], YahtzeeServ.yahtzeeroom, cmdparams->source->name);
		irc_chanprivmsg (ys_bot, YahtzeeServ.yahtzeeroom, "\0039%s has changed Channels, Game will now be available in %s", cmdparams->source->name, cmdparams->av[1]);
		if (gd) {
			RemoveChannelGameData(c, 1);
		}
		return NS_SUCCESS;
	}
	if (reason == SET_CHANGE) {
		irc_join (ys_bot, YahtzeeServ.yahtzeeroom, "+o");
		irc_chanalert (ys_bot, "Game Now Available In %s", YahtzeeServ.yahtzeeroom);
		c = FindChannel(YahtzeeServ.yahtzeeroom);
		if (c) {
			CreateChannelGameData(c);
		}
		return NS_SUCCESS;
	}
	return NS_SUCCESS;
}

/* 
 * Create Channel Game Data
*/
void CreateChannelGameData(Channel *c) {
	GameData *gd;
	int i;
	int i2;
	
	gd = ns_calloc(sizeof(GameData));
	gd->playercount = 0;
	gd->gamestatus = YS_GAME_STOPPED;
	gd->timer = me.now;
	gd->currentplayer = 0;
	gd->currentroll = 0;
	gd->currenthand = 0;
	for (i = 0 ; i < 5 ; i++) {
		gd->dice[i] = 0;
	}
	for (i = 0 ; i < YS_MAX_PLAYERS ; i++) {
		gd->pd[i] = ns_calloc(sizeof(Players));
		gd->pd[i]->u = NULL;
		gd->pd[i]->score = 0;
		for (i2 = 0 ; i2 < 15 ; i2++) {
			gd->pd[i]->hand[i2] = 0;
		}
	}
	SetChannelModValue(c, gd);
	lnode_create_append(gamelist, c);
	if (ircstrcasecmp(c->name, YahtzeeServ.yahtzeeroom)) {
		irc_join (ys_bot, c->name, "+o");
	}
}

/* 
 * Remove Channel Game Data
*/
void RemoveChannelGameData(Channel *c, int fdc) {
	GameData *gd;
	lnode_t	*ln;
	Channel *ct;
	int i;
	
	gd = (GameData *)GetChannelModValue(c);
	for ( i = 0 ; i < YS_MAX_PLAYERS ; i++) {
		if (gd->pd[i]->u) {
			if (GetUserModValue(gd->pd[i]->u) > 0) {
				SetUserModValue(gd->pd[i]->u,(GetUserModValue(gd->pd[i]->u) - 1));
			}
		}
		gd->pd[i]->u = NULL;
		ns_free(gd->pd[i]);
		gd->pd[i] = NULL;
	}
	ns_free(gd);
	ClearChannelModValue(c);
	ln = list_first(gamelist);
	while (ln != NULL) {
		ct = lnode_get(ln);
		if (c == ct) {
			list_delete(gamelist, ln);
			lnode_destroy(ln);
			break;
		}
		ln = list_next(gamelist, ln);
	}
	if (!ircstrcasecmp(c->name, YahtzeeServ.yahtzeeroom) && !fdc) {
		CreateChannelGameData(c);
	} else {
		irc_part (ys_bot, c->name, NULL);
	}
}
