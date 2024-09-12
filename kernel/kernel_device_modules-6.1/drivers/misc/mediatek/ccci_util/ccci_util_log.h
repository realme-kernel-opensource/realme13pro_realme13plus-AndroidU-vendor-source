/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2015 MediaTek Inc.
 */

#ifndef __CCCI_UTIL_LOG_H__
#define __CCCI_UTIL_LOG_H__

#if IS_ENABLED(CONFIG_MTK_AEE_IPANIC)
extern int mrdump_mini_add_extra_file(unsigned long vaddr, unsigned long paddr,
	unsigned long size, const char *name);
#endif

/* #define BRING_UP_LOG_MODE */
#ifndef BRING_UP_LOG_MODE
/* ------------------------------------------------------------------------- */
/* For normal stage log */
/* ------------------------------------------------------------------------- */
/* No MD id message part */
#define CCCI_UTIL_DBG_MSG(fmt, args...) \
do {\
	ccci_dump_write(CCCI_DUMP_INIT,\
		CCCI_DUMP_TIME_FLAG, "[0/util]" fmt, ##args);\
	ccci_dump_write(CCCI_DUMP_NORMAL,\
		CCCI_DUMP_TIME_FLAG, "[0/util]" fmt, ##args);\
} while (0)

#define CCCI_UTIL_INF_MSG(fmt, args...) \
do {\
	ccci_dump_write(CCCI_DUMP_INIT,\
		CCCI_DUMP_TIME_FLAG, "[0/util]" fmt, ##args);\
	ccci_dump_write(CCCI_DUMP_NORMAL,\
		CCCI_DUMP_TIME_FLAG, "[0/util]" fmt, ##args);\
} while (0)

#define CCCI_UTIL_ERR_MSG(fmt, args...) \
do {\
	ccci_dump_write(CCCI_DUMP_INIT,\
		CCCI_DUMP_TIME_FLAG, "[0/util]" fmt, ##args);\
	pr_notice("[ccci0/util]" fmt, ##args);\
} while (0)

#else

/* ------------------------------------------------------------------------- */
/* For bring up stage log */
/* ------------------------------------------------------------------------- */
/* No MD id message part */
#define CCCI_UTIL_DBG_MSG(fmt, args...) pr_notice("[ccci0/util]" fmt, ##args)
#define CCCI_UTIL_INF_MSG(fmt, args...) pr_notice("[ccci0/util]" fmt, ##args)
#define CCCI_UTIL_ERR_MSG(fmt, args...) pr_notice("[ccci0/util]" fmt, ##args)

#endif /* end of #ifndef BRING_UP_LOG_MODE */
#endif /*__CCCI_UTIL_LOG_H__ */
