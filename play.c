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
 * Stops Game
 */
void stopyahtzee() {
	if (currentyahtzeegamestatus == YS_GAME_STARTING) {
		DelTimer ("startyahtzee");
	}
	for (ypln = 0; ypln < 10; ypln++) {
		strlcpy(ysplayernick[ypln], " ", MAXNICK);
		ysplayerscore[ypln]= 0;
		for (yplnh = 0; yplnh < 15; yplnh++) {
			ysplayerhand[ypln][yplnh]= 0;
		}
		for (yplnh = 0; yplnh < 13; yplnh++) {
			ysplayerhandused[ypln][yplnh]= "x";
		}
	}
	currentyahtzeeplayercount = 0;
	currentyahtzeegamestatus = YS_GAME_STOPPED;
	currenthand= 0;
}

/* 
 * Rolls the First Roll after checking game is not over
 * adds nicks to high score lists if needed,
 * and lets them know if they made a list
 */
void yrolldie() {
	Client *u;
	char *ydiec[5];
	char *dienums;
	int ywn, cdthsl, cdthslf, olutt, dlutt, wlutt, mlutt, dcfegisrw;

	ywn= 0;
	dcfegisrw= 1;
	olutt= 0;
	dlutt= 0;
	wlutt= 0;
	mlutt= 0;
	if (currentplayer == 0) {
		srand((unsigned int)me.now);
		currenthand++;
		for (yplnh = 0; yplnh < 13; yplnh++) {
			if (!ircstrcasecmp(ysplayerhandused[currentplayer][yplnh], "x")) {
				dcfegisrw= 0;
			}
		}
		if (dcfegisrw > 0) {
			currenthand= 14;
		}
		if (currenthand > 13) {
			htsslrn= 0;
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0038Thats the end of the game folks.");
			for (ypln = 0; ypln < currentyahtzeeplayercount; ypln++) {
				if ((ypln != 0) && (ysplayerscore[ypln] > ysplayerscore[ywn])) {
					ywn= ypln;
				}
				irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s\00310 ended with\00311 %d\00310 points.", ysplayernick[ypln], ysplayerscore[ypln]);
			}
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s \0038is the winner with\00311 %d \0038points.", ysplayernick[ywn], ysplayerscore[ywn]);
			for (ypln = 0; ypln < currentyahtzeeplayercount; ypln++) {
				for (yplnh = 0; yplnh < 100; yplnh++) {
					if (ysplayerscore[ypln] > htsslr[yplnh]) {
						olutt= 1;
						if (yplnh == 0) {
							if (htsslrn == 0) {
								strlcpy(htsslrnickold, htsslrnick[0], MAXNICK);
								htsslrold= htsslr[0];
							}
							htsslrn= 1;
						}
						for (cdthsl = 99; cdthsl > yplnh; cdthsl--) {
							cdthslf= (cdthsl - 1);
							strlcpy(htsslrnick[cdthsl], htsslrnick[cdthslf], MAXNICK);
							htsslr[cdthsl]= htsslr[cdthslf];
						}
						strlcpy(htsslrnick[yplnh], ysplayernick[ypln], MAXNICK);
						htsslr[yplnh]= ysplayerscore[ypln];
						u = FindUser(ysplayernick[ypln]);
						if (u) {
							irc_prefmsg (ys_bot, u, "Congratulations, You are now on the high score list at position %d", (yplnh + 1));
						}
						yplnh= 101;
					}
				}
				for (yplnh = 0; yplnh < 10; yplnh++) {
					if (ysplayerscore[ypln] > dhtsslr[yplnh]) {
						dlutt= 1;
						for (cdthsl = 9; cdthsl > yplnh; cdthsl--) {
							cdthslf= (cdthsl - 1);
							strlcpy(dhtsslrnick[cdthsl], dhtsslrnick[cdthslf], MAXNICK);
							dhtsslr[cdthsl]= dhtsslr[cdthslf];
						}
						strlcpy(dhtsslrnick[yplnh], ysplayernick[ypln], MAXNICK);
						dhtsslr[yplnh]= ysplayerscore[ypln];
						u = FindUser(ysplayernick[ypln]);
						if (u) {
							irc_prefmsg (ys_bot, u, "Congratulations, You are now on the Daily high score list at position %d", (yplnh + 1));
						}
						yplnh= 11;
					}
				}
				for (yplnh = 0; yplnh < 10; yplnh++) {
					if (ysplayerscore[ypln] > whtsslr[yplnh]) {
						wlutt= 1;
						for (cdthsl = 9; cdthsl > yplnh; cdthsl--) {
							cdthslf= (cdthsl - 1);
							strlcpy(whtsslrnick[cdthsl], whtsslrnick[cdthslf], MAXNICK);
							whtsslr[cdthsl]= whtsslr[cdthslf];
						}
						strlcpy(whtsslrnick[yplnh], ysplayernick[ypln], MAXNICK);
						whtsslr[yplnh]= ysplayerscore[ypln];
						u = FindUser(ysplayernick[ypln]);
						if (u) {
							irc_prefmsg (ys_bot, u, "Congratulations, You are now on the Weekly high score list at position %d", (yplnh + 1));
						}
						yplnh= 11;
					}
				}
				for (yplnh = 0; yplnh < 10; yplnh++) {
					if (ysplayerscore[ypln] > mhtsslr[yplnh]) {
						mlutt= 1;
						for (cdthsl = 9; cdthsl > yplnh; cdthsl--) {
							cdthslf= (cdthsl - 1);
							strlcpy(mhtsslrnick[cdthsl], mhtsslrnick[cdthslf], MAXNICK);
							mhtsslr[cdthsl]= mhtsslr[cdthslf];
						}
						strlcpy(mhtsslrnick[yplnh], ysplayernick[ypln], MAXNICK);
						mhtsslr[yplnh]= ysplayerscore[ypln];
						u = FindUser(ysplayernick[ypln]);
						if (u) {
							irc_prefmsg (ys_bot, u, "Congratulations, You are now on the Monthly high score list at position %d", (yplnh + 1));
						}
						yplnh= 11;
					}
				}
			}
			if (htsslrn == 1) {
				irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s \0038now has the highest score, with\00311 %d \0038points, overtaking \0037%s\0038 who had\00311 %d \0038points.", htsslrnick[0], htsslr[0], htsslrnickold, htsslrold);
			} else {
				irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s \0038still has the highest score, with\00311 %d \0038points.", htsslrnick[0], htsslr[0]);
			}
			htsslrn= 0;
			stopyahtzee();
			if (olutt == 1) {
				saveyahtzeeoverall();
			}
			if (dlutt == 1) {
				saveyahtzeedaily();
			}
			if (wlutt == 1) {
				saveyahtzeeweekly();
			}
			if (mlutt == 1) {
				saveyahtzeemonthly();
			}
			return;
		}
	}
	for (ypln = 0; ypln < 5; ypln++) {
		ydie[ypln]= rand() % 6;
		if (ydie[ypln] == 0) {
			ydiec[ypln]= "\0034One\003";
		} else if (ydie[ypln] == 1) {
			ydiec[ypln]= "\0034Two\003";
		} else if (ydie[ypln] == 2) {
			ydiec[ypln]= "\0034Three\003";
		} else if (ydie[ypln] == 3) {
			ydiec[ypln]= "\0034Four\003";
		} else if (ydie[ypln] == 4) {
			ydiec[ypln]= "\0034Five\003";
		} else if (ydie[ypln] == 5) {
			ydiec[ypln]= "\0034Six\003";
		}
	}
	dienums= joinbuf(ydiec, 5, 0);
	currentroll= 1;
	irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s\0039 this is roll\00311 %d\0039, you rolled\00311 %s", ysplayernick[currentplayer], currentroll, dienums);
}

/* 
 * Gets Current Roll and set variables
 */
void getdieroll() {
	char *ydiec[5];
	char *dienums;
	for (ypln = 0; ypln < 5; ypln++) {
		if (ydie[ypln] == 0) {
			ydiec[ypln]= "\0034One\003";
		} else if (ydie[ypln] == 1) {
			ydiec[ypln]= "\0034Two\003";
		} else if (ydie[ypln] == 2) {
			ydiec[ypln]= "\0034Three\003";
		} else if (ydie[ypln] == 3) {
			ydiec[ypln]= "\0034Four\003";
		} else if (ydie[ypln] == 4) {
			ydiec[ypln]= "\0034Five\003";
		} else if (ydie[ypln] == 5) {
			ydiec[ypln]= "\0034Six\003";
		}
	}
	tmpstr= joinbuf(ydiec, 5, 0);
}

/* 
 * Gets Current Roll and set variables
 */
void yscore(char **argvy, int argcy) {
	int tstbonus;
	int scoredthisturn;
	int scoredalready;
	scoredalready= 0;
	if ( argcy < 1 ) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0034Sorry\0037 %s\0034, thats not a valid score slot, please use one of the following", ysplayernick[currentplayer]);
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\00311One's (1) , Two's (2) , Three's (3) , Four's (4) , Five's (5) , Six's (6)");
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\00311Full House (FH) , Short Straight (SS) , Long Straight (LS)");
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\00311Three of a Kind (3K) , Four of a Kind (4K) , Chance (C) , Yahtzee (Y)");
		return;
	}
	crs= 0;
	for (ypln = 0; ypln < 6; ypln++) {
		dtsc[ypln]= 0;
	}
	for (ypln = 0; ypln < 5; ypln++) {
		dtsc[ydie[ypln]]++;
	}
	if (!ircstrcasecmp(argvy[0], "1")) {
		scoredthisturn= 0;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][0], "x")) {
			scoredalready= 1;
		} else {
			scoredie(1);
		}
	} else if (!ircstrcasecmp(argvy[0], "2")) {
		scoredthisturn= 1;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][1], "x")) {
			scoredalready= 1;
		} else {
			scoredie(2);
		}
	} else if (!ircstrcasecmp(argvy[0], "3")) {
		scoredthisturn= 2;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][2], "x")) {
			scoredalready= 1;
		} else {
			scoredie(3);
		}
	} else if (!ircstrcasecmp(argvy[0], "4")) {
		scoredthisturn= 3;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][3], "x")) {
			scoredalready= 1;
		} else {
			scoredie(4);
		}
	} else if (!ircstrcasecmp(argvy[0], "5")) {
		scoredthisturn= 4;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][4], "x")) {
			scoredalready= 1;
		} else {
			scoredie(5);
		}
	} else if (!ircstrcasecmp(argvy[0], "6")) {
		scoredthisturn= 5;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][5], "x")) {
			scoredalready= 1;
		} else {
			scoredie(6);
		}
	} else if (!ircstrcasecmp(argvy[0], "fh")) {
		scoredthisturn= 6;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][6], "x")) {
			scoredalready= 1;
		} else {
			scorefh();
		}
	} else if (!ircstrcasecmp(argvy[0], "ss")) {
		scoredthisturn= 7;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][7], "x")) {
			scoredalready= 1;
		} else {
			scoress();
		}
	} else if (!ircstrcasecmp(argvy[0], "ls")) {
		scoredthisturn= 8;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][8], "x")) {
			scoredalready= 1;
		} else {
			scorels();
		}
	} else if (!ircstrcasecmp(argvy[0], "3k")) {
		scoredthisturn= 9;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][9], "x")) {
			scoredalready= 1;
		} else {
			score3k();
		}
	} else if (!ircstrcasecmp(argvy[0], "4k")) {
		scoredthisturn= 10;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][10], "x")) {
			scoredalready= 1;
		} else {
			score4k();
		}
	} else if (!ircstrcasecmp(argvy[0], "c")) {
		scoredthisturn= 11;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][11], "x")) {
			scoredalready= 1;
		} else {
			scorec();
		}
	} else if (!ircstrcasecmp(argvy[0], "y")) {
		scoredthisturn= 12;
		if (ircstrcasecmp(ysplayerhandused[currentplayer][12], "x")) {
			scoredalready= 1;
		} else {
			scorey();
			if (crs == 50) {
				ysplayerhand[currentplayer][12] = 9999;
			}
		}
	} else {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0034Sorry\0037 %s\0034, thats not a valid score slot, please use one of the following", ysplayernick[currentplayer]);
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\00311One's (1) , Two's (2) , Three's (3) , Four's (4) , Five's (5) , Six's (6)");
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\00311Full House (FH) , Short Straight (SS) , Long Straight (LS)");
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\00311Three of a Kind (3K) , Four of a Kind (4K) , Chance (C) , Yahtzee (Y)");
		return;
	}
	if (scoredalready == 1) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0034Sorry\0037 %s\0034 you have already scored on \00311%s", ysplayernick[currentplayer], ysscoretype[scoredthisturn]);
		return;
	}
	ysplayerhandused[currentplayer][scoredthisturn]= "";
	if (scoredthisturn != 12) {
		ysplayerhand[currentplayer][scoredthisturn] = crs;
	}
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s\0039 scored\00311 %d\0039 on \00311%s", ysplayernick[currentplayer], crs, ysscoretype[scoredthisturn]);
	if (ysplayerhand[currentplayer][13] < 1) {
		tstbonus= 0;
		for (yplnh = 0; yplnh < 6; yplnh++) {
			tstbonus += ysplayerhand[currentplayer][yplnh];
		}
		if (tstbonus > 62) {
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s\0039 scored a \0038Bonus\00311 35 \0039points", ysplayernick[currentplayer]);
			crs += 35;
			ysplayerhand[currentplayer][13]= 35;
			ysplayerhandused[currentplayer][13]= "";
		}
	}
	if ((ysplayerhand[currentplayer][12] == 50) && (crs > 0)) {
		if ( (ydie[0] == ydie[1]) && (ydie[0] == ydie[2]) && (ydie[0] == ydie[3]) && (ydie[0] == ydie[4])) {
			irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s\0039 scored a \0038Bonus\00311 100 \0039points", ysplayernick[currentplayer]);
			crs += 100;
			ysplayerhand[currentplayer][14] += 100;
			ysplayerhandused[currentplayer][14]= "";
		}
	}
	if (ysplayerhand[currentplayer][12] == 9999) {
		ysplayerhand[currentplayer][12] = 50;
	}
	ysplayerscore[currentplayer] += crs;
	irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s\0039 now has a total of\00311 %d\0039 points", ysplayernick[currentplayer], ysplayerscore[currentplayer]);
	currentplayer++;
	if (currentplayer > (currentyahtzeeplayercount - 1)) {
		currentplayer= 0;
	}
	currentroll= 0;
	yrolldie();
	return;
}

/* 
 * ReRolls Selected Dice
 */
void reroll(char **argvy, int argcy, char *rolltype) {
	char *ydiec[5];
	char *dienums;
	int keepdie[5];
	char *tstdni;
	char *buf;
	int kdt1;
	int kdt2;
	if (!ircstrcasecmp(rolltype, "roll")) {
		kdt1= 0;
		kdt2= 1;
	} else {
		kdt1= 1;
		kdt2= 0;
	}
	for (ypln = 0; ypln < 5; ypln++) {
		keepdie[ypln]= kdt2;
	}
	buf= joinbuf(argvy, argcy, 0);
	tstdni= strcasestr(buf, "1");
	if (!tstdni==NULL) {
		keepdie[0]= kdt1;
	}
	tstdni= strcasestr(buf, "2");
	if (!tstdni==NULL) {
		keepdie[1]= kdt1;
	}
	tstdni= strcasestr(buf, "3");
	if (!tstdni==NULL) {
		keepdie[2]= kdt1;
	}
	tstdni= strcasestr(buf, "4");
	if (!tstdni==NULL) {
		keepdie[3]= kdt1;
	}
	tstdni= strcasestr(buf, "5");
	if (!tstdni==NULL) {
		keepdie[4]= kdt1;
	}
	for (ypln = 0; ypln < 5; ypln++) {
		if (keepdie[ypln] < 1) {
			ydie[ypln]= rand() % 6;
			if (ydie[ypln] == 0) {
				ydiec[ypln]= "\0034One\003";
			} else if (ydie[ypln] == 1) {
				ydiec[ypln]= "\0034Two\003";
			} else if (ydie[ypln] == 2) {
				ydiec[ypln]= "\0034Three\003";
			} else if (ydie[ypln] == 3) {
				ydiec[ypln]= "\0034Four\003";
			} else if (ydie[ypln] == 4) {
				ydiec[ypln]= "\0034Five\003";
			} else if (ydie[ypln] == 5) {
				ydiec[ypln]= "\0034Six\003";
			}
		} else {
			if (ydie[ypln] == 0) {
				ydiec[ypln]= "\00311One\003";
			} else if (ydie[ypln] == 1) {
				ydiec[ypln]= "\00311Two\003";
			} else if (ydie[ypln] == 2) {
				ydiec[ypln]= "\00311Three\003";
			} else if (ydie[ypln] == 3) {
				ydiec[ypln]= "\00311Four\003";
			} else if (ydie[ypln] == 4) {
				ydiec[ypln]= "\00311Five\003";
			} else if (ydie[ypln] == 5) {
				ydiec[ypln]= "\00311Six\003";
			}
		}
	}
	dienums= joinbuf(ydiec, 5, 0);
	currentroll++;
	irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037%s\0039 this is roll\00311 %d\0039, you rolled\00311 %s", ysplayernick[currentplayer], currentroll, dienums);
	if (currentroll == 3) {
		irc_chanprivmsg (ys_bot, yahtzeeroom, "\0039That was your last roll this turn\0037 %s\0039, select which score to take", ysplayernick[currentplayer]);
	}
}

/*
 * Start Game
*/

void startcountdowntimer(char *nic) {
	currentyahtzeegamestatus = YS_GAME_STARTING;
	irc_chanprivmsg (ys_bot, yahtzeeroom, "\0037A new game of Yahtzee has been started by %s. Game will start in 30 seconds, type '\2\003!Join\2\0037' to play.", nic);
	AddTimer (TIMER_TYPE_COUNTDOWN, startyahtzee, "startyahtzee", 30);

}

/* 
 * Check Scores
 */
void scoredie(int sconum) {
	for (ypln = 0; ypln < 5; ypln++) {
		if (ydie[ypln] == (sconum - 1)) {
			crs += sconum;
		}
	}
}

void scorefh() {
	int fhs;
	fhs= 0;
	for (ypln = 0; ypln < 6; ypln++) {
		if ((dtsc[ypln] == 2) || (dtsc[ypln] == 3)) {
			fhs++;
		} else if (dtsc[ypln] == 1) {
			fhs--;
		}
	}
	if (fhs == 2) {
		crs= 25;
	}
}

void scoress() {
	int snd;
	snd= 0;
	for (ypln = 0; ypln < 6; ypln++) {
		if (dtsc[ypln] == 0) {
			if (snd < 4) {
				snd= 0;
			}
		} else {
			snd++;
		}
	}
	if (snd > 3) {
		crs = 30;
	}
}

void scorels() {
	if ((dtsc[1] == 1) && (dtsc[2] == 1) && (dtsc[3] == 1) && (dtsc[4] == 1)) {
		crs = 40;
	}
}

void score3k() {
	for (ypln = 0; ypln < 6; ypln++) {
		if (dtsc[ypln] > 2) {
			for (yplnh = 0; yplnh < 5; yplnh++) {
				crs += (ydie[yplnh] + 1);
			}
		}
	}
}

void score4k() {
	for (ypln = 0; ypln < 6; ypln++) {
		if (dtsc[ypln] > 3) {
			for (yplnh = 0; yplnh < 5; yplnh++) {
				crs += (ydie[yplnh] + 1);
			}
		}
	}
}

void scorec() {
	for (ypln = 0; ypln < 5; ypln++) {
		crs += (ydie[ypln] + 1);
	}
}

void scorey() {
	if ( (ydie[0] == ydie[1]) && (ydie[0] == ydie[2]) && (ydie[0] == ydie[3]) && (ydie[0] == ydie[4])) {
		crs += 50;
	}
}
