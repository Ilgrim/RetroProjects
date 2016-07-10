/*	cf.c

	Some tests for CF.

	Copyright (c) 2016 Miguel Garcia / FloppySoftware

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the
	Free Software Foundation; either version 3, or (at your option) any
	later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

	Author's contact:

		www.floppysoftware.es
		cpm-connections.blogspot.com
		floppysoftware@gmail.com

	To compile with MESCC:

		cc cf
		ccopt cf
		zsm cf
		hextocom cf

	Usage:

		cf

	Revisions:

		06 Jul 2016 : Work begins.
*/

/* Defines for MESCC libraries
   ---------------------------
   Leave only what you need.
*/
//#define CC_STDIO        // Support for stdin, stdout, stderr.
//#define CC_REDIR        // Support for command line redirection - needs CC_STDIO.

//#define CC_CONIO_BIOS   // Console I/O through BIOS instead of BDOS.

//#define CC_FCX          // Support for user number in filenames.
//#define CC_FCX_DIR      // Support for named user numbers - needs CC_FCX and DirToDrvUsr().

//#define CC_FILEIO_SMALL // Exclude fread(), fwrite() and fgets().

//#define CC_NO_MUL       // Exclude support for MULTIPLICATION.
//#define CC_NO_DIV       // Exclude support for DIVISION and MODULUS.
//#define CC_NO_SWITCH    // Exclude support for SWITCH.

#define CC_NO_ARGS      // Exclude support for ARGC and ARGV.

/* Standard MESCC library
   ----------------------
   You must include this library;
   it contains some defs. and the runtime.
*/
#include <mescc.h>

/* Standard MESCC libraries
   ------------------------
   Leave only what you need.
*/
//#include <alloc.h>
//#include <atexit.h>
//#include <bsearch.h>
//#include <clock.h>
//#include <conio.h>
//#include <cpm.h>
//#include <ctype.h>
//#include <fileio.h>
//#include <fprintf.h>
//#include <mem.h>
#include <printf.h>
//#include <qsort.h>
//#include <rand.h>
//#include <redir.h>
//#include <setjmp.h>
//#include <sprintf.h>
//#include <string.h>
//#include <xprintf.h>
//#include <z80.h>

/* Project libraries
   -----------------
*/
#include "cf.h"
#include "cf_read.h"
#include "cf_write.h"
#include "cf_bool.h"
#include "cf_int.h"
#include "cf_uint.h"
#include "cf_str.h"

/* Globals
   -------
*/
CF *cf;

/* Program entry
   -------------
*/
main()
{
	int k;

	printf("Creating CF\n\n");

	if(!(cf = cf_create(6)))
		error("Can't create CF");

	add_key("title", "That's cool!");
	add_key("author", "Jim Brown");

	// This should cause an error
	add_key("title", "This should cause an error: the key already exists");

	add_key("year", "1969");
	add_key("pages", "150");
	add_key("summary", "\"This book, blah, blah, blah...\"");
	add_key("lent", "true");

	// This should cause an error
	add_key("publisher", "This should cause an error: no more entries");

	printf("\n");
	pr_keys(cf);

	set_key("year", "1977");

	// This should cause an error
	set_key("month", "This should cause an error: the key does not exists");

	printf("\n");
	pr_keys(cf);

	printf("Writting test.cf\n\n");

	if(cf_write(cf, "test.cf"))
		error("Can't write test.cf");

	printf("Destroying CF\n\n");

	cf_destroy(cf);

	// -------------------------------------------

	printf("Creating CF\n\n");

	if(!(cf = cf_create(8)))
		error("Can't create CF");

	printf("Reading test.cf into CF\n\n");

	if(cf_read(cf, "test.cf"))
		error("Can't read test.cf");

	pr_keys(cf);

	printf("Reading test_2.cf into CF\n\n");

	if((k = cf_read(cf, "test_2.cf"))) { printf("!!%d!!\n", k);
		error("Can't read test_2.cf");}

	pr_keys(cf);

	printf("Title     >> %s\n", cf_get_key(cf, "title"));
	printf("Author    >> %s\n", cf_get_str(cf, "author", "unknown"));
	printf("Publisher >> %s\n", cf_get_str(cf, "publisher", "n/a"));
	printf("Year      >> %d\n", cf_get_uint(cf, "year", 9999));
	printf("Pages     >> %d\n", cf_get_int(cf, "pages", 9999));
	printf("Summary   >> %s\n", cf_get_str(cf, "summary", "n/a"));
	printf("Lent      >> %s\n", cf_get_bool(cf, "lent", 0) ? "Yes" : "No");
	printf("To        >> %s\n", cf_get_key(cf, "lent_to"));
	printf("Expires   >> %s\n", cf_get_key(cf, "lend_expires"));

	printf("\n");

	printf("Destroying CF\n\n");

	cf_destroy(cf);

	// -------------------------------------------

	printf("Done\n");
}

add_key(key, value)
char *key, *value;
{
	int result;

	result = cf_add_key(cf, key, value);

	printf("Add %s = %s%s\n", key, value, result ? " --> ERROR" : "");
}

set_key(key, value)
char *key, *value;
{
	int result;

	result = cf_set_key(cf, key, value);

	printf("Set %s = %s%s\n", key, value, result ? " --> ERROR" : "");
}

pr_keys(cf)
CF *cf;
{
	cf_pr_keys(cf);

	printf("\n");
}

error(msg)
char *msg;
{
	printf("ERROR: %s\n", msg);

	exit(-1);
}

