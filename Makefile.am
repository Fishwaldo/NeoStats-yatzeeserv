AUTOMAKE_OPTIONS        = foreign
ACLOCAL_AMFLAGS         = -I autotools
AM_MAKEFLAGS            = -s

pkglib_LTLIBRARIES	= yahtzeeserv.la
yahtzeeserv_la_SOURCES	= channel.c htmlscores.c play.c yahtzeeserv_help.c \
			  highscores.c misc.c yahtzeeserv.c
noinst_HEADERS		= yahtzeeserv.h

yahtzeeserv_la_LDFLAGS	= -module -avoid-version

EXTRA_DIST		= autotools/ccdv.c autotools/shtool RELNOTES

dist_doc_DATA		= README.YahtzeeServ
dist_data_DATA		= html/ys.tpl

include $(top_srcdir)/autotools/rules.mk
