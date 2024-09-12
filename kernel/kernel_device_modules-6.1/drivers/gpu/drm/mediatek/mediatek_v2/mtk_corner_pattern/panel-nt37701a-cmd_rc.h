/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2021 MediaTek Inc.
 */

#ifndef __NT37701A_CMD_RC_H__
#define __NT37701A_CMD_RC_H__

#define ROUND_CORNER_H_TOP 120

static unsigned char panel_nt37701a_cmd_top_pattern[] = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x5c, 0x02, 0x68, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x57, 0x02, 0x6d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x53, 0x02, 0x71, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x51, 0x02, 0x73, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x4f, 0x02, 0x75, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x4d, 0x02, 0x77, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x4b, 0x02, 0x79, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x49, 0x02, 0x7b, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x48, 0x02, 0x7c, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x47, 0x02, 0x7d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x45, 0x02, 0x7f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x44, 0x02, 0x80, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x43, 0x02, 0x81, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x42, 0x02, 0x82, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x41, 0x02, 0x83, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x40, 0x02, 0x84, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3f, 0x02, 0x85, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3f, 0x02, 0x85, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3e, 0x02, 0x86, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3d, 0x02, 0x87, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3c, 0x02, 0x88, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3c, 0x02, 0x88, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3b, 0x02, 0x89, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3b, 0x02, 0x89, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3a, 0x02, 0x8a, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3a, 0x02, 0x8a, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x39, 0x02, 0x8b, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x39, 0x02, 0x8b, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x38, 0x02, 0x8c, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x38, 0x02, 0x8c, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x37, 0x02, 0x8d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x37, 0x02, 0x8d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x37, 0x02, 0x8d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x37, 0x02, 0x8d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x36, 0x02, 0x8e, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x36, 0x02, 0x8e, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x36, 0x02, 0x8e, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x36, 0x02, 0x8e, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x36, 0x02, 0x8e, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x35, 0x02, 0x8f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x36, 0x02, 0x8e, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x36, 0x02, 0x8e, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x36, 0x02, 0x8e, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x36, 0x02, 0x8e, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x36, 0x02, 0x8e, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x37, 0x02, 0x8d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x37, 0x02, 0x8d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x37, 0x02, 0x8d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x37, 0x02, 0x8d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x38, 0x02, 0x8c, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x38, 0x02, 0x8c, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x38, 0x02, 0x8c, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x39, 0x02, 0x8b, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x39, 0x02, 0x8b, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3a, 0x02, 0x8a, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3b, 0x02, 0x89, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3b, 0x02, 0x89, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3c, 0x02, 0x88, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3c, 0x02, 0x88, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3d, 0x02, 0x87, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3e, 0x02, 0x86, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3f, 0x02, 0x85, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x3f, 0x02, 0x85, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x40, 0x02, 0x84, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x41, 0x02, 0x83, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x42, 0x02, 0x82, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x43, 0x02, 0x81, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x44, 0x02, 0x80, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x45, 0x02, 0x7f, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x47, 0x02, 0x7d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x48, 0x02, 0x7c, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x49, 0x02, 0x7b, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x4b, 0x02, 0x79, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x4d, 0x02, 0x77, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x4e, 0x02, 0x76, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x50, 0x02, 0x74, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x53, 0x02, 0x71, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x57, 0x02, 0x6d, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x5c, 0x02, 0x68, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};
#endif