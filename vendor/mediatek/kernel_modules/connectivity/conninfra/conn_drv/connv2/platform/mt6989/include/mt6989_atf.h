/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2023 MediaTek Inc.
 */

#ifndef MT6989_ATF_H
#define MT6989_ATF_H

#include "conninfra.h"

int consys_init_atf_data_mt6989_atf(void);
int consys_polling_chipid_mt6989_atf(void);
int connsys_d_die_cfg_mt6989_atf(void);
int connsys_spi_master_cfg_mt6989_atf(unsigned int curr_status, unsigned int next_status);
int connsys_subsys_pll_initial_mt6989_atf(void);
int connsys_low_power_setting_mt6989_atf(unsigned int curr_status, unsigned int next_status);
int consys_conninfra_wakeup_mt6989_atf(void);
int consys_conninfra_sleep_mt6989_atf(void);
int consys_spi_read_mt6989_atf(
	enum sys_spi_subsystem subsystem, unsigned int addr, unsigned int *data);
int consys_spi_write_mt6989_atf(
	enum sys_spi_subsystem subsystem, unsigned int addr, unsigned int data);
int consys_spi_update_bits_mt6989_atf(
	enum sys_spi_subsystem subsystem, unsigned int addr, unsigned int data, unsigned int mask);
int consys_spi_clock_switch_mt6989_atf(enum connsys_spi_speed_type type);
int consys_subsys_status_update_mt6989_atf(bool on, int radio);
int consys_reset_power_state_mt6989_atf(void);
int consys_power_state_dump_mt6989_atf(char *buf, unsigned int size);
void consys_set_mcu_control_mt6989_atf(int type, bool onoff);
void connsys_afe_sw_patch_mt6989_atf(void);

#endif /* MT6989_ATF_H */
