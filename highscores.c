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
 * Display High Score Pages
*/
int ShowHighList (CmdParams* cmdparams)
{
	lnode_t *ln;
	HighScoreData *hs;
	int i;

	i = 1;
	if (cmdparams->ac) {
		if (atoi(cmdparams->av[0]) && atoi(cmdparams->av[0]) < 6) {
			i = ( ( atoi(cmdparams->av[0]) * 20 ) - 19 );
		}
	}
	if (cmdparams->channel) {
		irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0039Top Scores so far %d-%d\0038 :", i, (i +19));
	} else {
		irc_prefmsg (ys_bot, cmdparams->source, "\0039Top Scores so far %d-%d\0038 :", i, (i +19));
	}
	ln = list_first(highscores);
	while (ln != NULL) {
		hs = lnode_get(ln);
		if (hs->typepos[0] == 'O' && hs->position >= i && hs->position < (i + 20)) {
			if (cmdparams->channel) {
				irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0039 %d\0038  :\0037 %s \0039with\00311 %d\0039 points.", hs->position, hs->name, hs->score);
			} else {
				irc_prefmsg (ys_bot, cmdparams->source, "\0039 %d\0038  :\0037 %s \0039with\00311 %d\0039 points.", hs->position, hs->name, hs->score);
			}
		} else if (hs->typepos[0] == 'O' && hs->position == (i + 20)) {
			ln = list_last(highscores);
			continue;
		}
		ln = list_next(highscores, ln);
	}
	return NS_SUCCESS;
}

/*
 * Display Top10 lists
*/
int ShowTop10Lists (CmdParams* cmdparams) {
	lnode_t *ln;
	HighScoreData *hs;
	int i, st;
	
	i = 0;
	st = 'O';
	if (cmdparams->ac > 0) {
		if (cmdparams->av[0][0] == 'D' || cmdparams->av[0][0] == 'd') {
			if (cmdparams->channel) {
				irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0039Top 10 Scores so far Today\0038 :");
			} else {
				irc_prefmsg (ys_bot, cmdparams->source, "\0039Top 10 Scores so far Today\0038 :");
			}
			st = 'D';
		} else if (cmdparams->av[0][0] == 'W' || cmdparams->av[0][0] == 'w') {
			if (cmdparams->channel) {
				irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0039Top 10 Scores so far This Week\0038 :");
			} else {
				irc_prefmsg (ys_bot, cmdparams->source, "\0039Top 10 Scores so far This Week\0038 :");
			}
			st = 'W';
		} else if (cmdparams->av[0][0] == 'M' || cmdparams->av[0][0] == 'm') {
			if (cmdparams->channel) {
				irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0039Top 10 Scores so far This Month\0038 :");
			} else {
				irc_prefmsg (ys_bot, cmdparams->source, "\0039Top 10 Scores so far This Month\0038 :");
			}
			st = 'M';
		}
	}
	if (st == 'O') {
		if (cmdparams->channel) {
			irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0039Top 10 Scores so far\0038 :");
		} else {
			irc_prefmsg (ys_bot, cmdparams->source, "\0039Top 10 Scores so far\0038 :");
		}
	}
	ln = list_first(highscores);
	while (ln != NULL && i < 10) {
		hs = lnode_get(ln);
		if (hs->type == st) {
			if (cmdparams->channel) {
				irc_chanprivmsg (ys_bot, cmdparams->channel->name, "\0039 %d\0038  :\0037 %s \0039with\00311 %d\0039 points.", hs->position, hs->name, hs->score);
			} else {
				irc_prefmsg (ys_bot, cmdparams->source, "\0039 %d\0038  :\0037 %s \0039with\00311 %d\0039 points.", hs->position, hs->name, hs->score);
			}
			i++;
		}
		ln = list_next(highscores, ln);
	}
	return NS_SUCCESS;
}

/*
 * Load High Scores
*/
void loadyahtzeescores(void) {
	highscores = list_create( -1 );
	DBAFetchRows("Scores", loadhighscores);
	list_sort(highscores, sortlistbytypepos);
}

/*
 * Add High Scores to list
*/
int loadhighscores(void *data, int size) {
	HighScoreData *hs;

	hs = ns_calloc(sizeof(HighScoreData));
	os_memcpy( hs, data, sizeof(HighScoreData));
	lnode_create_append( highscores, hs );
	return NS_FALSE;
}

/*
 * Sort High Score List
*/
int sortlistbytypepos( const void *key1, const void *key2 ) {
	const HighScoreData *hs1 = key1;
	const HighScoreData *hs2 = key2;
	if (hs1->type == hs2->type) {
		return (hs1->position - hs2->position);
	}
	return (hs1->type - hs2->type);
}

/*
 * Select High Scores to remove
*/
int yahtzeeday(void) {
	irc_chanalert (ys_bot, "\0038It's Midnight Here : \0037Clearing Daily Top 10 List");
	irc_chanprivmsg (ys_bot, YahtzeeServ.yahtzeeroom, "\0038It's Midnight Here : \0037Clearing Daily Top 10 List");
	clearyahtzeescores('D');
	return NS_SUCCESS;
}

int yahtzeeweek(void) {
	irc_chanalert (ys_bot, "\0038It's Sunday Here : \0037Clearing Weekly Top 10 List");
	irc_chanprivmsg (ys_bot, YahtzeeServ.yahtzeeroom, "\0038It's Sunday Here : \0037Clearing Weekly Top 10 List");
	clearyahtzeescores('W');
	return NS_SUCCESS;
}

int yahtzeemonth(void) {
	irc_chanalert (ys_bot, "\0038It's The First Here : \0037Clearing Monthly Top 10 List");
	irc_chanprivmsg (ys_bot, YahtzeeServ.yahtzeeroom, "\0038It's The First Here : \0037Clearing Monthly Top 10 List");
	clearyahtzeescores('M');
	return NS_SUCCESS;
}

/* 
 * Clear Selected High Scores
*/
void clearyahtzeescores(int stc) {
	lnode_t *ln, *ln2;
	HighScoreData *hs;

	ln = list_first(highscores);
	while (ln != NULL) {
		hs = lnode_get(ln);
		if (hs->typepos[0] == stc) {
			DBADelete( "Scores", hs->typepos);
			ln2 = list_next(highscores, ln);
			ns_free(hs);
			list_delete(highscores, ln);
			lnode_destroy(ln);
			ln = ln2;
		} else {
			ln = list_next(highscores, ln);
		}
	}
	list_sort(highscores, sortlistbytypepos);
}

/*
 * Check for new High Scores
*/
void checkhighscorelists(Channel *c) {
	lnode_t *ln, *ln2;
	HighScoreData *hs, *hs2;
	GameData *gd;
	int i, np, nhs;

	gd = (GameData *)GetChannelModValue(c);
	if (!gd) {
		return;
	}
	nhs = 0;
	for (i = 0 ; i < gd->playercount ; i++) {
		np = 1;
		list_sort(highscores, sortlistbytypepos);
		ln = list_last(highscores);
		while (ln != NULL) {
			hs = lnode_get(ln);
			if (hs->typepos[0] == 'D') {
				if (np == 1) {
					np = (hs->position + 1);
				}
				if (hs->score < gd->pd[i]->score) {
					DBADelete( "Scores", hs->typepos);
					np = hs->position;
					hs->position++;
					if (hs->position < 11) {
						ircsnprintf(hs->typepos, 6, "%s%d", "D", hs->position);
						DBAStore( "Scores", hs->typepos, (void*)hs, sizeof(HighScoreData));
					} else {
						ln2 = list_prev(highscores, ln);
						ns_free(hs);
						list_delete(highscores, ln);
						lnode_destroy(ln);
						ln = ln2;
						continue;						
					}
				}
			}
			ln = list_prev(highscores, ln);
		}
		if (np < 11) {
			hs = ns_calloc(sizeof(HighScoreData));
			ircsnprintf(hs->typepos, 6, "%s%d", "D", np);
			hs->type = 'D';
			hs->position = np;
			strlcpy(hs->name, gd->pd[i]->u->name, MAXNICK);
			hs->score = gd->pd[i]->score;
			hs->changed = 0;
			DBAStore( "Scores", hs->typepos, (void*)hs, sizeof(HighScoreData));
			hs->changed = YS_SCORE_NEW;
			lnode_create_append( highscores, hs );
		}
		np = 1;
		list_sort(highscores, sortlistbytypepos);
		ln = list_last(highscores);
		while (ln != NULL) {
			hs = lnode_get(ln);
			if (hs->typepos[0] == 'W') {
				if (np == 1) {
					np = (hs->position + 1);
				}
				if (hs->score < gd->pd[i]->score) {
					DBADelete( "Scores", hs->typepos);
					np = hs->position;
					hs->position++;
					if (hs->position < 11) {
						ircsnprintf(hs->typepos, 6, "%s%d", "W", hs->position);
						DBAStore( "Scores", hs->typepos, (void*)hs, sizeof(HighScoreData));
					} else {
						ln2 = list_prev(highscores, ln);
						ns_free(hs);
						list_delete(highscores, ln);
						lnode_destroy(ln);
						ln = ln2;
						continue;						
					}
				}
			}
			ln = list_prev(highscores, ln);
		}
		if (np < 11) {
			hs = ns_calloc(sizeof(HighScoreData));
			ircsnprintf(hs->typepos, 6, "%s%d", "W", np);
			hs->type = 'W';
			hs->position = np;
			strlcpy(hs->name, gd->pd[i]->u->name, MAXNICK);
			hs->score = gd->pd[i]->score;
			hs->changed = 0;
			DBAStore( "Scores", hs->typepos, (void*)hs, sizeof(HighScoreData));
			hs->changed = YS_SCORE_NEW;
			lnode_create_append( highscores, hs );
		}
		np = 1;
		list_sort(highscores, sortlistbytypepos);
		ln = list_last(highscores);
		while (ln != NULL) {
			hs = lnode_get(ln);
			if (hs->typepos[0] == 'M') {
				if (np == 1) {
					np = (hs->position + 1);
				}
				if (hs->score < gd->pd[i]->score) {
					DBADelete( "Scores", hs->typepos);
					np = hs->position;
					hs->position++;
					if (hs->position < 11) {
						ircsnprintf(hs->typepos, 6, "%s%d", "M", hs->position);
						DBAStore( "Scores", hs->typepos, (void*)hs, sizeof(HighScoreData));
					} else {
						ln2 = list_prev(highscores, ln);
						ns_free(hs);
						list_delete(highscores, ln);
						lnode_destroy(ln);
						ln = ln2;
						continue;						
					}
				}
			}
			ln = list_prev(highscores, ln);
		}
		if (np < 11) {
			hs = ns_calloc(sizeof(HighScoreData));
			ircsnprintf(hs->typepos, 6, "%s%d", "M", np);
			hs->type = 'M';
			hs->position = np;
			strlcpy(hs->name, gd->pd[i]->u->name, MAXNICK);
			hs->score = gd->pd[i]->score;
			hs->changed = 0;
			DBAStore( "Scores", hs->typepos, (void*)hs, sizeof(HighScoreData));
			hs->changed = YS_SCORE_NEW;
			lnode_create_append( highscores, hs );
		}
		np = 1;
		list_sort(highscores, sortlistbytypepos);
		ln = list_last(highscores);
		while (ln != NULL) {
			hs = lnode_get(ln);
			if (hs->typepos[0] == 'O') {
				if (np == 1) {
					np = (hs->position + 1);
				}
				if (hs->score < gd->pd[i]->score) {
					DBADelete( "Scores", hs->typepos);
					np = hs->position;
					hs->position++;
					if (hs->position < 101) {
						ircsnprintf(hs->typepos, 6, "%s%d", "O", hs->position);
						DBAStore( "Scores", hs->typepos, (void*)hs, sizeof(HighScoreData));
					} else {
						ln2 = list_prev(highscores, ln);
						ns_free(hs);
						list_delete(highscores, ln);
						lnode_destroy(ln);
						ln = ln2;
						continue;						
					}
				}
			}
			ln = list_prev(highscores, ln);
		}
		if (np < 101) {
			hs = ns_calloc(sizeof(HighScoreData));
			ircsnprintf(hs->typepos, 6, "%s%d", "O", np);
			hs->type = 'O';
			hs->position = np;
			strlcpy(hs->name, gd->pd[i]->u->name, MAXNICK);
			hs->score = gd->pd[i]->score;
			hs->changed = 0;
			DBAStore( "Scores", hs->typepos, (void*)hs, sizeof(HighScoreData));
			hs->changed = YS_SCORE_NEW;
			lnode_create_append( highscores, hs );
		}
	}
	list_sort(highscores, sortlistbytypepos);
	ln = list_first(highscores);
	while (ln != NULL) {
		hs = lnode_get(ln);
		if (hs->changed == YS_SCORE_NEW) {
			for (i = 0 ; i < gd->playercount ; i++) {
				if (!ircstrcasecmp(gd->pd[i]->u->name, hs->name)) {
					if (hs->type == 'D') {
						irc_prefmsg (ys_bot, gd->pd[i]->u, "Congratulations, You are now on the Daily high score list at position %d", hs->position);
					} else if (hs->type == 'W') {
						irc_prefmsg (ys_bot, gd->pd[i]->u, "Congratulations, You are now on the Weekly high score list at position %d", hs->position);
					} else if (hs->type == 'M') {
						irc_prefmsg (ys_bot, gd->pd[i]->u, "Congratulations, You are now on the Monthly high score list at position %d", hs->position);
					} else if (hs->type == 'O') {
						irc_prefmsg (ys_bot, gd->pd[i]->u, "Congratulations, You are now on the Overall high score list at position %d", hs->position);
						if (hs->position == 1) {
							ln2 = list_next(highscores, ln);
							hs2 = lnode_get(ln2);
							irc_chanprivmsg (ys_bot, c->name, "\0037%s \0038now has the highest score, with\00311 %d \0038points, overtaking \0037%s\0038 who had\00311 %d \0038points.", hs->name, hs->score, hs2->name, hs2->score);
							nhs = 1;
						}
					}
				}
			}
			hs->changed = 0;
		}
		ln = list_next(highscores, ln);
	}
	if (!nhs) {
		ln = list_first(highscores);
		while (ln != NULL) {
			hs = lnode_get(ln);
			if (hs->type == 'O' && hs->position == 1) {
				irc_chanprivmsg (ys_bot, c->name, "\0037%s \0038still has the highest score, with\00311 %d \0038points.", hs->name, hs->score);
				break;
			}
			ln = list_next(highscores, ln);
		}
	}
	return;
}
