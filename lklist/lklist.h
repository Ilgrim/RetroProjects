/**
 * @file   lklist.h
 * @brief  Management library for doubly-linked lists in MESCC.
 * @author Miguel I. Garcia Lopez / FloppySoftware
 *
 * Doubly-linked lists management library, for MESCC (Mike's Enhanced
 * Small C Compiler for Z80 & CP/M).
 *
 * Public macros:
 *  - LKLIST       Linked list
 *  - LKLIST_ITEM  Linked list item
 *
 * Revisions:
 *  - 18 Feb 2018 : Released first public version.
 *
 * Copyright (c) 2018 Miguel I. Garcia Lopez / FloppySoftware.
 *
 * Licensed under the GNU General Public License v3.
 *
 * http://www.floppysoftware.es
 * http://cpm-connections.blogspot.com
 * floppysoftware@gmail.com
 */
#ifndef LKLIST_H

#define LKLIST_H

/* Dependencies
   ------------
*/
/////#include <alloc.h>
/////#include <mem.h>

/* Public macros
   -------------
*/
#define LKLIST      unsigned int  // Linked list
#define LKLIST_ITEM unsigned int  // Linked list item

/* Private macros
   --------------
*/
//#define _LK_DEBUG

#define _LK_FIRST  0  // First item or NULL if empty
#define _LK_LAST   1  // Last item or NULL if empty
#define _LK_CURSOR 2  // Cursor or NULL if undefined
#define _LK_ITEMS  3  // # of items

#define _LK_HDRSZ  8  // Size in bytes of header structure

#define _LK_PREV   0  // Pointer to previous item or NULL on first item
#define _LK_NEXT   1  // Pointer to next item or NULL on last item
#define _LK_SIZE   2  // Size of data
#define _LK_DATA   3  // Data offset in WORDS

#define _LK_ITMSZ  6  // Size in bytes of item structure

/**
 * @fn     LKLIST *LkAlloc(void)
 * @brief  Allocate a linked list.
 * @return linked list pointer, or NULL on failure.
 */
LkAlloc()
{
	LKLIST *lk;

	// Alloc memory for header
	if((lk = malloc(_LK_HDRSZ))) {

		// Setup as empty
		lk[_LK_FIRST] = lk[_LK_LAST] = lk[_LK_CURSOR] = NULL;
		lk[_LK_ITEMS] = 0;
	}

#ifdef _LK_DEBUG
	printf("LkAlloc = %04xH\n", lk);
#endif

	// Success or failure
	return lk;
}

/**
 * @fn     void LkFree(LKLIST *lk)
 * @brief  Free a linked list.
 * @param  lk = linked list pointer
 */
LkFree(lk)
LKLIST *lk;
{
	LKLIST_ITEM *item, *next;

	// Free items
	for(item = lk[_LK_FIRST]; item != NULL; item = next) {

		next = item[_LK_NEXT];

		free(item);

#ifdef _LK_DEBUG
	printf("LkFree = %04xH > %04xH\n", lk, item);
#endif

	}

	// Free header
	free(lk);
}

/**
 * @fn     int LkGetSize(LKLIST *lk)
 * @brief  Get # of items in a linked list.
 * @param  lk = linked list pointer
 * @return # of items
 */
LkGetSize(lk)
LKLIST *lk;
{
	return lk[_LK_ITEMS];
}

/**
 * @fn     LKLIST_ITEM *LkGetFirst(LKLIST *lk)
 * @brief  Get first item in a linked list and set cursor.
 * @param  lk = linked list pointer
 * @return item pointer, or NULL if linked list is empty
 *
 * In addition to get the first item in a linked list,
 * this funcion sets the linked list cursor to that item.
 */
LkGetFirst(lk)
LKLIST *lk;
{
	return (lk[_LK_CURSOR] = lk[_LK_FIRST]);
}

/**
 * @fn     LKLIST_ITEM *LkGetLast(LKLIST *lk)
 * @brief  Get last item in a linked list and set cursor.
 * @param  lk = linked list pointer
 * @return item pointer, or NULL if linked list is empty
 *
 * In addition to get the last item in a linked list,
 * this funcion sets the linked list cursor to that item.
 */
LkGetLast(lk)
LKLIST *lk;
{
	return (lk[_LK_CURSOR] = lk[_LK_LAST]);
}

/**
 * @fn     LKLIST_ITEM *LkGetPrev(LKLIST *lk)
 * @brief  Get previous item in a linked list according to cursor.
 * @param  lk = linked list pointer
 * @return item pointer, or NULL if there are no previuous items
 *
 * In addition to get the previous item in a linked list according to cursor,
 * this funcion sets the linked list cursor to that item.
 */
LkGetPrev(lk)
LKLIST *lk;
{
	LKLIST_ITEM *item;

	if(lk[_LK_CURSOR]) {
		item = lk[_LK_CURSOR];

		lk[_LK_CURSOR] = item[_LK_PREV];
	}

	return lk[_LK_CURSOR];
}

/**
 * @fn     LKLIST_ITEM *LkGetNext(LKLIST *lk)
 * @brief  Get next item in a linked list according to cursor.
 * @param  lk = linked list pointer
 * @return item pointer, or NULL if there are no more items
 *
 * In addition to get the next item in a linked list according to cursor,
 * this funcion sets the linked list cursor to that item.
 */
LkGetNext(lk)
LKLIST *lk;
{
	LKLIST_ITEM *item;

	if(lk[_LK_CURSOR]) {
		item = lk[_LK_CURSOR];

		lk[_LK_CURSOR] = item[_LK_NEXT];
	}

	return lk[_LK_CURSOR];
}

/**
 * @fn     void *LkGetItemData(LKLIST_ITEM *item)
 * @brief  Get pointer to item data.
 * @param  item = pointer to item
 * @return data pointer
 */
LkGetItemData(item)
LKLIST_ITEM *item;
{
	return item + _LK_DATA;
}

/**
 * @fn     void *LkGetItemSize(LKLIST_ITEM *item)
 * @brief  Get item data size in bytes.
 * @param  item = pointer to item
 * @return data size in bytes
 */
LkGetItemSize(item)
LKLIST_ITEM *item;
{
	return item[_LK_SIZE];
}

/**
 * @fn     LKLIST_ITEM *LkAdd(LKLIST *lk, void *data, int size)
 * @brief  Add an item to a linked list.
 * @param  lk = linked list pointer
 * @param  data = pointer to data
 * @param  size = data size in bytes
 * @return item pointer, or NULL if there is not enough memory
 */
LkAdd(lk, data, size)
LKLIST *lk;
BYTE *data;
int size;
{
	LKLIST_ITEM *item, *prev;

	if((item = malloc(_LK_ITMSZ + size))) {

		// Setup new item
		item[_LK_PREV] = lk[_LK_LAST];
		item[_LK_NEXT] = NULL;
		item[_LK_SIZE] = size;

		memcpy(item + _LK_DATA, data, size);

		// Modify previous item
		if((prev = lk[_LK_LAST])) {
			prev[_LK_NEXT] = item;
		}

		// Modify header
		if(!lk[_LK_FIRST]) {
			lk[_LK_FIRST] = item;
		}

		lk[_LK_LAST] = item;

		++lk[_LK_ITEMS];

#ifdef _LK_DEBUG
	printf("LkAdd = %04xH < %04xH [%d bytes]\n", lk, item, size);
#endif

	}

	// Success or failure
	return item;
}

/* Remove an item
   --------------
*/
/**
 * @fn     void LkRemove(LKLIST *lk, LKLIST_ITEM *item)
 * @brief  Remove an item in a linked list.
 * @param  lk = linked list pointer
 * @param  item = pointer to item
 *
 * If the cursor points to the removed item, this function
 * updates it to the next item.
 */
LkRemove(lk, item)
LKLIST *lk;
LKLIST_ITEM *item;
{
	LKLIST_ITEM *prev, *next;

	// Modify previous item if any
	if((prev = item[_LK_PREV])) {
		prev[_LK_NEXT] = item[_LK_NEXT];
	}
	else {
		// It's the first one
		lk[_LK_FIRST] = item[_LK_NEXT];
	}

	// Modify next item if any
	if((next = item[_LK_NEXT])) {
		next[_LK_PREV] = item[_LK_PREV];
	}
	else {
		// It's the last one
		lk[_LK_LAST] = item[_LK_PREV];
	}

	// Check and update cursor if needed
	if(lk[_LK_CURSOR] == item) {
		lk[_LK_CURSOR] = item[_LK_NEXT];
	}

	--lk[_LK_SIZE];

	free(item);

#ifdef _LK_DEBUG
	printf("LkDel = %04xH > %04xH\n", lk, item);
#endif

}

#ifdef _LK_DEBUG

/* Print linked list data
   ----------------------
*/
LkPrint(lk)
LKLIST *lk;
{
	LKLIST_ITEM *item, *next;
	int n;

	printf("LkPrint : first item = %04xH\n", lk[_LK_FIRST]);
	printf("LkPrint : last item  = %04xH\n", lk[_LK_LAST]);
	printf("LkPrint : # of items = %d\n\n", lk[_LK_ITEMS]);

	n = 0;

	for(item = lk[_LK_FIRST]; item != NULL; item = next) {

		printf("LkPrint : #%5u = %04xH\n", n, item);
		printf("LkPrint : #%5u = prev %04xH\n", n, item[_LK_PREV]);
		printf("LkPrint : #%5u = next %04xH\n", n, item[_LK_NEXT]);
		printf("LkPrint : #%5u = size %d\n\n", n, item[_LK_SIZE]);

		next = item[_LK_NEXT];

		++n;
	}
}

#endif

/* Cleaning
   --------
*/
#undef _LK_FIRST
#undef _LK_LAST
#undef _LK_CURSOR
#undef _LK_ITEMS

#undef _LK_HDRSZ

#undef _LK_PREV
#undef _LK_NEXT
#undef _LK_SIZE
#undef _LK_DATA

#undef _LK_ITMSZ

#endif

