/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

/*****************************************************************************
 *
 * Filename:
 * ---------
 *     OMEGAS2TELEmipi_Sensor.h
 *
 * Project:
 * --------
 *     ALPS
 *
 * Description:
 * ------------
 *     CMOS sensor header file
 *
 ****************************************************************************/
#ifndef _OMEGAS2TELEMIPI_SENSOR_H
#define _OMEGAS2TELEMIPI_SENSOR_H

#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/atomic.h>
#include <linux/types.h>

#include "kd_camera_typedef.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define_v4l2.h"
#include "kd_imgsensor_errcode.h"
#include "oplus_kd_eeprom.h"
#include "oplus_kd_imgsensor.h"
#include "omegas2tele_ana_gain_table.h"
#include "omegas2tele_Sensor_setting.h"
#include "oplus-adaptor-subdrv-ctrl.h"
#include "adaptor-subdrv-ctrl.h"
#include "adaptor-i2c.h"
#include "adaptor.h"

#endif