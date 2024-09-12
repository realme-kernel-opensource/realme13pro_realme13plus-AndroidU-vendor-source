// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2023 OPLUS. All rights reserved.
 */

/*****************************************************************************
 *
 * Filename:
 * ---------
 *     nvwamainmipiraw_Sensor.h
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
#ifndef _NVWAMAINMIPI_SENSOR_H
#define _NVWAMAINMIPI_SENSOR_H

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
#include "nvwamain_ana_gain_table.h"
#include "nvwamain_Sensor_setting.h"

#include "adaptor-subdrv-ctrl.h"
#include "adaptor-i2c.h"
#include "adaptor.h"
#include "oplus-adaptor-subdrv-ctrl.h"
#endif
