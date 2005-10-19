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
** $Id: highscores.c 26 2005-10-15 18:06:38Z DNB $
*/

#include "neostats.h"    /* Required for bot support */
#include "yahtzeeserv.h"
#ifdef HAVE_FCNTL_H
#include <fcntl.h> 
#endif /* HAVE_FCNTL_H */

/** HTML output handler type */
typedef void( *htmlhandler )( void );

/** HTML handler table struct */
typedef struct htmlfunc {
	char* directive;
	htmlhandler handler;
}htmlfunc;

#define TABLE_START( opf ) os_fprintf( ( opf ), "<table border = 0>" );
#define TABLE_END( opf ) os_fprintf( opf, "</table>" );

/** HTML handler prototypes */
static void html_title( void );
static void html_version( void );
static void put_copyright( void );
static void html_highscores( void );
static void html_dailyscores( void );
static void html_weeklyscores( void );
static void html_monthlyscores( void );

/** Output file pointer */
static FILE *opf;
/** Output file template */
static const char html_template[]="data/ys.tpl";

/** HTML handler lookup table */
static htmlfunc htmlfuncs[]=
{
	{"!HIGHSCORES!", html_highscores},
	{"!DAILYSCORES!", html_dailyscores},
	{"!WEEKLYSCORES!", html_weeklyscores},
	{"!MONTHLYSCORES!", html_monthlyscores},
	{"!VERSION!", html_version},
	{"!TITLE!", html_title},
	{NULL, NULL},
};

/** @brief html_title
 *
 *  HTML handler for title
 *
 *  @param none
 *
 *  @return none
 */

static void html_title( void )
{
	os_fprintf( opf, "YahtzeeServ High Scores for %s", me.netname );
}

/** @brief html_version
 *
 *  HTML handler for version
 *
 *  @param none
 *
 *  @return none
 */

static void html_version( void )
{
	os_fputs( me.version, opf );
}

/** @brief put_copyright
 *
 *  HTML handler for copyright footer
 *
 *  @param none
 *
 *  @return none
 */

static void put_copyright( void )
{
	os_fprintf( opf, "<br><br><center>Scores last changed at %s<br>", sftime( time( NULL ) ) );
	os_fprintf( opf, "<b>YahtzeeServ Information:</b>\n" );
	os_fprintf( opf, "<br> %s compiled on %s at %s\n", me.name, version_date, version_time );
	os_fprintf( opf, "<br> %s compiled on %s at %s\n", module_info.name,
		module_info.build_date, module_info.build_time );
	os_fprintf( opf, "<br><a href=\"http://www.neostats.net\">http://www.neostats.net</a>\n" );
	os_fprintf( opf, "</center>\n" );
}

/** @brief html_highscores
 *
 *  HTML handler for High Scores
 *
 *  @param none
 *
 *  @return none
 */

static void html_highscores( void )
{
	lnode_t *ln;
	HighScoreData *hs;
	int i=0;

	list_sort(highscores, sortlistbytypepos);
	TABLE_START( opf );
	os_fprintf( opf, "<tr><th><b>Position</b></th><th><b>Nickname</b></th><th><b>Score</b></th></tr>\n" );
	ln = list_first(highscores);
	while (ln) 
	{
		hs = lnode_get(ln);
		if (hs->type == 'O') 
			os_fprintf( opf, "<tr><td>%d</td><td>%s</td><td>%d</td>\n", ++i, hs->name, hs->score );
		ln = list_next(highscores, ln);
	}
	TABLE_END( opf );
}

/** @brief html_dailyscores
 *
 *  HTML handler for daily scores
 *
 *  @param none
 *
 *  @return none
 */

static void html_dailyscores( void )
{
	lnode_t *ln;
	HighScoreData *hs;
	int i=0;

	list_sort(highscores, sortlistbytypepos);
	TABLE_START( opf );
	os_fprintf( opf, "<tr><th><b>Position</b></th><th><b>Nickname</b></th><th><b>Score</b></th></tr>\n" );
	ln = list_first(highscores);
	while (ln) 
	{
		hs = lnode_get(ln);
		if (hs->type == 'D') 
			os_fprintf( opf, "<tr><td>%d</td><td>%s</td><td>%d</td>\n", ++i, hs->name, hs->score );
		ln = list_next(highscores, ln);
	}
	TABLE_END( opf );
}

/** @brief html_weeklycores
 *
 *  HTML handler for weekly scores
 *
 *  @param none
 *
 *  @return none
 */

static void html_weeklyscores( void )
{
	lnode_t *ln;
	HighScoreData *hs;
	int i=0;

	list_sort(highscores, sortlistbytypepos);
	TABLE_START( opf );
	os_fprintf( opf, "<tr><th><b>Position</b></th><th><b>Nickname</b></th><th><b>Score</b></th></tr>\n" );
	ln = list_first(highscores);
	while (ln) 
	{
		hs = lnode_get(ln);
		if (hs->type == 'W') 
			os_fprintf( opf, "<tr><td>%d</td><td>%s</td><td>%d</td>\n", ++i, hs->name, hs->score );
		ln = list_next(highscores, ln);
	}
	TABLE_END( opf );
}

/** @brief html_monthlyscores
 *
 *  HTML handler for monthly scores
 *
 *  @param none
 *
 *  @return none
 */

static void html_monthlyscores( void )
{
	lnode_t *ln;
	HighScoreData *hs;
	int i=0;

	list_sort(highscores, sortlistbytypepos);
	TABLE_START( opf );
	os_fprintf( opf, "<tr><th><b>Position</b></th><th><b>Nickname</b></th><th><b>Score</b></th></tr>\n" );
	ln = list_first(highscores);
	while (ln) 
	{
		hs = lnode_get(ln);
		if (hs->type == 'M') 
			os_fprintf( opf, "<tr><td>%d</td><td>%s</td><td>%d</td>\n", ++i, hs->name, hs->score );
		ln = list_next(highscores, ln);
	}
	TABLE_END( opf );
}

/** @brief ys_HTMLOutput
 *
 *  output HTML file
 *
 *  @param none
 *
 *  @return none
 */

void ys_HTMLOutput( void )
{
#define HTMLREADBUFSIZE 512
	static char buf[HTMLREADBUFSIZE];
	FILE *tpl;
	char *buftemp;
	char *bufptr;
	htmlfunc* htmlfuncptr;

	tpl = os_fopen( html_template, "rt" );
	if( !tpl ) {
		nlog( LOG_WARNING, "Failed to open YahtzeeServ HTML template %s.", html_template );
		irc_chanalert( ys_bot, "Failed to open YahtzeeServ HTML template %s.", html_template );
		return;
	}
	opf = os_fopen( YahtzeeServ.htmlpath, "wt" );
	if( !opf ) {
		nlog( LOG_WARNING, "Failed to open HTML output file %s. Check file permissions.", YahtzeeServ.htmlpath );
		irc_chanalert( ys_bot, "Failed to open HTML output file %s. Check file permissions.", YahtzeeServ.htmlpath );
		return;
	}
	while( os_fgets( buf, HTMLREADBUFSIZE, tpl ) ) {
		bufptr = buf;
		htmlfuncptr = htmlfuncs;
		while( htmlfuncptr->directive ) {
			buftemp = strstr( bufptr, htmlfuncptr->directive );
			if( buftemp ) {
				os_fwrite( bufptr, ( int )buftemp -( int )bufptr, 1, opf );
				htmlfuncptr->handler();
				bufptr = buftemp + strlen( htmlfuncptr->directive );
			}		
			htmlfuncptr++;
		}
		buftemp = strcasestr( bufptr, "</html>" );
		if( buftemp ) {
			os_fwrite( bufptr, ( int )buftemp -( int )bufptr, 1, opf );
			put_copyright();
			bufptr = buftemp;
			os_fputs( bufptr, opf );
		} else {
			os_fputs( bufptr, opf );
		}
	}
	os_fclose( tpl );
	os_fclose( opf );
    /* update the umode so others can read it and owner can overwrite it */
    chmod(YahtzeeServ.htmlpath, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
}

/** @brief ss_cmd_forcehtml
 *
 *  FORCEHTML command handler
 *  Reports current statistics to requesting user
 *
 *  @param cmdparams
 *
 *  @return NS_SUCCESS if succeeds, else NS_FAILURE
 */

int ys_cmd_forcehtml( const CmdParams *cmdparams )
{
	nlog( LOG_NOTICE, "%s!%s@%s forced an update of the HTML file.",
		cmdparams->source->name, cmdparams->source->user->username, cmdparams->source->user->hostname );
	ys_HTMLOutput();
	return NS_SUCCESS;
}
