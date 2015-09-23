/*
 * hack.h
 *
 *  Created on: Jun 15, 2015
 *      Author: alessandro
 */

#ifndef QEMU_INCLUDE_BLOCK_HACK_H_
#define QEMU_INCLUDE_BLOCK_HACK_H_

typedef struct HackList {
	const char *name;
	struct HackList *next;
}HackList;

#endif /* QEMU_INCLUDE_BLOCK_HACK_H_ */
