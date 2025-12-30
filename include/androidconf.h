/*	SCCS Id: @(#)androidconf.h	3.4	2011/03/31	*/
/* Copyright (c) Kenneth Lorber, Bethesda, Maryland, 1990, 1991, 1992, 1993. */
/* NetHack may be freely redistributed.  See license for details. */

#ifdef ANDROID

#ifndef ANDROIDCONF_H
#define ANDROIDCONF_H

#define error debuglog

#define NO_FILE_LINKS /* if no hard links */
#define LOCKDIR "." /* where to put locks */ 
#define SELF_RECOVER /* Allow the game itself to recover from an aborted game */ 

#ifdef getchar
#  undef getchar
#endif
#define getchar nhgetch
#undef tgetch
#define tgetch nhgetch

#undef SHELL

#define DUMP_LOG
#undef DUMP_FN
#define DUMP_HTML_CSS_FILE "unnethack_dump.css"
#define DUMP_HTML_CSS_EMBEDDED
#define DUMP_HTML_LOG
#undef DUMP_TEXT_LOG

#define UTF8_GLYPHS
//#undef STATUS_COLORS
#define ASCIIGRAPH
#undef SVR4
#undef NETWORK
#define LINUX
#undef MAIL
#undef DEF_MAILREADER

#define PARANOID

# endif /* ANDROIDCONF_H */
#endif /* ANDROID */

