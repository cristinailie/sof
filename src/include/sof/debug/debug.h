/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(c) 2016 Intel Corporation. All rights reserved.
 *
 * Author: Liam Girdwood <liam.r.girdwood@linux.intel.com>
 *         Keyon Jie <yang.jie@linux.intel.com>
 */

#ifndef __SOF_DEBUG_DEBUG_H__
#define __SOF_DEBUG_DEBUG_H__

#include <sof/common.h>
#include <sof/debug/panic.h>
#include <sof/lib/cache.h>
#include <sof/lib/cpu.h>
#include <sof/sof.h>
#include <sof/string.h>
#include <ipc/info.h>
#include <ipc/trace.h>

#include <stddef.h>
#include <stdint.h>

#ifndef RELATIVE_FILE
#error "This file requires RELATIVE_FILE to be defined. " \
	"Add it to CMake's target with sof_append_relative_path_definitions."
#endif

#if CONFIG_DEBUG

#include <sof/lib/mailbox.h>

#define DEBUG_SET_FW_READY_FLAGS					\
(									\
	SOF_IPC_INFO_BUILD |						\
	(IS_ENABLED(CONFIG_DEBUG_LOCKS) ? SOF_IPC_INFO_LOCKS : 0) |	\
	(IS_ENABLED(CONFIG_DEBUG_LOCKS_VERBOSE) ? SOF_IPC_INFO_LOCKSV : 0) | \
	(IS_ENABLED(CONFIG_GDB_DEBUG) ? SOF_IPC_INFO_GDB : 0)		\
)

/* dump file and line to start of mailbox or shared memory */
#define dbg() \
	do { \
		volatile uint32_t *__m = (uint32_t *)mailbox_get_debug_base(); \
		*(__m++) = (RELATIVE_FILE[0] << 24) + \
			   (RELATIVE_FILE[1] << 16) + \
			   (RELATIVE_FILE[2] << 8) + (RELATIVE_FILE[3]); \
		*(__m++) = (__func__[0] << 24) + (__func__[1] << 16) + \
			(__func__[2] << 8) + (__func__[3]); \
		*__m = __LINE__; \
	} while (0)

/* dump file and line to offset in mailbox or shared memory */
#define dbg_at(__x) \
	do { \
		volatile uint32_t *__m = \
			(uint32_t *)mailbox_get_debug_base() + __x; \
		*(__m++) = (RELATIVE_FILE[0] << 24) + \
			   (RELATIVE_FILE[1] << 16) + \
			   (RELATIVE_FILE[2] << 8) + (RELATIVE_FILE[3]); \
		*(__m++) = (__func__[0] << 24) + (__func__[1] << 16) + \
			(__func__[2] << 8) + (__func__[3]); \
		*__m = __LINE__; \
	} while (0)

/* dump value to start of mailbox or shared memory */
#define dbg_val(__v) \
	do { \
		volatile uint32_t *__m = \
			(volatile uint32_t *)mailbox_get_debug_base(); \
		*__m = __v; \
	} while (0)

/* dump value to offset in mailbox or shared memory */
#define dbg_val_at(__v, __x) \
	do { \
		volatile uint32_t *__m = \
			(volatile uint32_t *)mailbox_get_debug_base() + __x; \
		*__m = __v; \
	} while (0)

/* dump data area at addr and size count to start of mailbox or shared memory */
#define dump(addr, count) \
	do { \
		volatile uint32_t *__m = (uint32_t *)mailbox_get_debug_base(); \
		volatile uint32_t *__a = (uint32_t *)addr; \
		volatile int __c = count; \
		while (__c--) \
			*(__m++) = *(__a++); \
	} while (0)

/* dump data area at addr and size count at mailbox offset or shared memory */
#define dump_at(addr, count, offset) \
	do { \
		volatile uint32_t *__m = \
			(uint32_t *)mailbox_get_debug_base() + offset; \
		volatile uint32_t *__a = (uint32_t *)addr; \
		volatile int __c = count; \
		while (__c--) \
			*(__m++) = *(__a++); \
	} while (0)

/* dump object to start of mailbox */
#define dump_object(__o) \
	do {						\
		dbg();					\
		dump(&__o, sizeof(__o) >> 2);		\
	} while (0)

/* dump object from pointer at start of mailbox */
#define dump_object_ptr(__o) \
	do {						\
		dbg();					\
		dump(__o, sizeof(*(__o)) >> 2);		\
	} while (0)

#define dump_object_ptr_at(__o, __at) \
	do {						\
		dbg();					\
		dump_at(__o, sizeof(*(__o)) >> 2, __at);\
	} while (0)

#else

#define DEBUG_SET_FW_READY_FLAGS					\
(									\
	(IS_ENABLED(CONFIG_DEBUG_LOCKS) ? SOF_IPC_INFO_LOCKS : 0) |	\
	(IS_ENABLED(CONFIG_DEBUG_LOCKS_VERBOSE) ? SOF_IPC_INFO_LOCKSV : 0) | \
	(IS_ENABLED(CONFIG_GDB_DEBUG) ? SOF_IPC_INFO_GDB : 0)		\
)

#define dbg() do {} while (0)
#define dbg_at(__x) do {} while (0)
#define dbg_val(__v) do {} while (0)
#define dbg_val_at(__v, __x) do {} while (0)
#define dump(addr, count) do {} while (0)
#define dump_object(__o) do {} while (0)
#define dump_object_ptr(__o) do {} while (0)
#endif

#endif /* __SOF_DEBUG_DEBUG_H__ */
