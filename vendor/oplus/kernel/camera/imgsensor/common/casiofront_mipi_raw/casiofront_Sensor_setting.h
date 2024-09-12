/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
 */
/*****************************************************************************
 *
 * Filename:
 * ---------
 *	 casiofront_Sensor_setting.h
 *
 * Project:
 * --------
 * Description:
 * ------------
 *	 CMOS sensor header file
 *
 ****************************************************************************/
#ifndef _CASIOFRONTMIPI_SENSOR_SETTING_H
#define _CASIOFRONTMIPI_SENSOR_SETTING_H

#include "kd_camera_typedef.h"

static u16 casiofront_sensor_preinit_setting[] = {
    0x6028, 0x4000,
    0x0000, 0x101F,
    0x0000, 0x3109,
    0x6010, 0x0001,
};

static u16 casiofront_sensor_init_setting_array1[] = {
    0x6214, 0x7970,
    0x6218, 0x7150,
    0x0A02, 0x007E,
    0x6028, 0x2000,
    0x602A, 0x3F4C,
};

static u16 casiofront_sensor_init_setting_array2[] = {
    0x0000,
    0x0000,
    0x0649,
    0x0548,
    0xC1F8,
    0xC405,
    0x0549,
    0x081A,
    0x0349,
    0xA1F8,
    0xC805,
    0x00F0,
    0x65BC,
    0x0000,
    0x2000,
    0x4A84,
    0x2000,
    0x2ED0,
    0x2000,
    0x6C00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x40BA,
    0x7047,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0xC0BA,
    0x7047,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x2DE9,
    0xF047,
    0x1C46,
    0x9046,
    0x8946,
    0x0746,
    0xFE48,
    0x0022,
    0x0068,
    0x86B2,
    0x050C,
    0x3146,
    0x2846,
    0x00F0,
    0xC5FC,
    0x2346,
    0x4246,
    0x4946,
    0x3846,
    0x00F0,
    0xC4FC,
    0xF848,
    0x90F8,
    0x8B02,
    0x88B1,
    0x788A,
    0x04F1,
    0x0054,
    0x04EB,
    0x8001,
    0x09E0,
    0x2268,
    0xC2F3,
    0xC360,
    0x90FA,
    0xA0F0,
    0x22F0,
    0x7842,
    0x42EA,
    0x5000,
    0x01C4,
    0x8C42,
    0xF3D1,
    0x3146,
    0x2846,
    0xBDE8,
    0xF047,
    0x0122,
    0x00F0,
    0xA2BC,
    0x2DE9,
    0xFC5F,
    0x8346,
    0xE748,
    0x8A46,
    0x0022,
    0x4068,
    0x010C,
    0x80B2,
    0xCDE9,
    0x0001,
    0x0146,
    0x0198,
    0x00F0,
    0x93FC,
    0xABFB,
    0x0A10,
    0xE24B,
    0xE04D,
    0xE04A,
    0x93F8,
    0x9160,
    0x05F5,
    0xAA69,
    0x06FB,
    0x0BF6,
    0x0023,
    0x891B,
    0x4D46,
    0x60EB,
    0x0300,
    0x03C5,
    0x1D46,
    0xEBFB,
    0x0A65,
    0x02F5,
    0xAB67,
    0x3A46,
    0xD64C,
    0x60C2,
    0xA4F8,
    0x4835,
    0x04F5,
    0xA962,
    0x94F8,
    0xA0C4,
    0x94F8,
    0xA144,
    0x4FF4,
    0xF858,
    0xBCF1,
    0x010F,
    0x03D0,
    0xBCF1,
    0x020F,
    0x14D0,
    0x29E0,
    0x08EA,
    0x0423,
    0x43F0,
    0x1103,
    0x1380,
    0x2346,
    0x0022,
    0x00F0,
    0x6AFC,
    0xC9E9,
    0x0001,
    0x2346,
    0x0022,
    0x2846,
    0x3146,
    0x00F0,
    0x62FC,
    0xC7E9,
    0x0001,
    0x15E0,
    0x08EA,
    0x042C,
    0x4CF0,
    0x010C,
    0xA2F8,
    0x00C0,
    0xA1FB,
    0x042C,
    0x00FB,
    0x04C0,
    0x01FB,
    0x0301,
    0xC9E9,
    0x0012,
    0xA6FB,
    0x0401,
    0x05FB,
    0x0411,
    0x06FB,
    0x0311,
    0xC7E9,
    0x0010,
    0xB848,
    0xB949,
    0xB0F8,
    0x4805,
    0x0880,
    0xB848,
    0x0CC8,
    0x48F6,
    0x2200,
    0x00F0,
    0x43FC,
    0xB548,
    0x0830,
    0x0CC8,
    0x48F6,
    0x2A00,
    0x00F0,
    0x3CFC,
    0x5846,
    0x00F0,
    0x3EFC,
    0xAD49,
    0x0122,
    0xC1F8,
    0x68A5,
    0xDDE9,
    0x0010,
    0x02B0,
    0xBDE8,
    0xF05F,
    0x00F0,
    0x1FBC,
    0xA84A,
    0x92F8,
    0xD525,
    0x2AB1,
    0xA64A,
    0xA54B,
    0xD2F8,
    0x6825,
    0xC3F8,
    0x3024,
    0xA34A,
    0xD2F8,
    0x3024,
    0x00F0,
    0x29BC,
    0x10B5,
    0xA049,
    0xA34A,
    0xA44B,
    0xD1F8,
    0x3C14,
    0x947C,
    0x0CB1,
    0x908A,
    0x1BE0,
    0x9B4A,
    0x92F8,
    0xA220,
    0xC2F1,
    0x0C02,
    0xD140,
    0x4843,
    0x010A,
    0x9D48,
    0xD0F8,
    0x8400,
    0x0279,
    0x4A43,
    0x4179,
    0xC088,
    0xCA40,
    0x00EB,
    0x1210,
    0x4FF4,
    0x8021,
    0xB1FB,
    0xF0F0,
    0x0911,
    0x8842,
    0x04D2,
    0x4028,
    0x00D8,
    0x4020,
    0x5880,
    0x10BD,
    0x0846,
    0xFBE7,
    0x4168,
    0x4A7B,
    0x9149,
    0xA1F8,
    0x8223,
    0x4268,
    0x537B,
    0x002B,
    0x15D0,
    0x01F5,
    0x6171,
    0x927B,
    0x0A80,
    0x4068,
    0xC07B,
    0x4880,
    0x8B48,
    0xB0F8,
    0xC220,
    0x8A80,
    0xB0F8,
    0xC420,
    0xCA80,
    0x10F8,
    0xC72F,
    0xC078,
    0x5208,
    0x4008,
    0x42EA,
    0x8000,
    0x0881,
    0x7047,
    0x2DE9,
    0xFF4F,
    0x8348,
    0x83B0,
    0x1D46,
    0xC079,
    0xDDF8,
    0x44B0,
    0x1646,
    0x0F46,
    0x0028,
    0x6ED0,
    0xDFF8,
    0xF4A1,
    0x0AF1,
    0xBA0A,
    0xAAF1,
    0x1C00,
    0xB0F8,
    0x0090,
    0xB0F8,
    0x0480,
    0x00F0,
    0xCCFB,
    0x0399,
    0x109C,
    0x0843,
    0x04F1,
    0x8044,
    0x07D0,
    0xA780,
    0xE680,
    0xAAF1,
    0x1C00,
    0x0188,
    0x2181,
    0x8088,
    0x20E0,
    0x6848,
    0x9AF8,
    0x0C10,
    0xB0F8,
    0xD801,
    0x4843,
    0x0290,
    0x00F0,
    0xBAFB,
    0x0028,
    0x0298,
    0x01D0,
    0x361A,
    0x00E0,
    0x0744,
    0xA780,
    0xE680,
    0x6048,
    0xB0F8,
    0xDA61,
    0x90F8,
    0x8A02,
    0x4643,
    0x00F0,
    0xAFFB,
    0x10B1,
    0xA8EB,
    0x0608,
    0x00E0,
    0xB144,
    0xA4F8,
    0x0890,
    0x4046,
    0x6081,
    0x0398,
    0x28B1,
    0x5648,
    0x90F8,
    0x4F11,
    0x90F8,
    0x8902,
    0x03E0,
    0x9AF8,
    0x0D10,
    0x9AF8,
    0x0C00,
    0x0A01,
    0x5149,
    0x91F8,
    0x4E11,
    0x42EA,
    0x8121,
    0x41F0,
    0x0301,
    0xA181,
    0x0121,
    0xFF22,
    0x02EB,
    0x4000,
    0x41EA,
    0x0020,
    0xE081,
    0x01A9,
    0x6846,
    0x00F0,
    0x8BFB,
    0x9DF8,
    0x0000,
    0x9DF8,
    0x0410,
    0x40EA,
    0x0120,
    0x2082,
    0x5F46,
    0x3E46,
    0x00F0,
    0x7BFB,
    0x791E,
    0x0028,
    0xA889,
    0x04D0,
    0x4718,
    0x46F6,
    0xA410,
    0x03E0,
    0x3FE0,
    0x4618,
    0x46F6,
    0x2410,
    0xA880,
    0x6782,
    0xE682,
    0x0020,
    0xA082,
    0xA888,
    0x2080,
    0x00F0,
    0x70FB,
    0x0128,
    0x0CD1,
    0x00F0,
    0x71FB,
    0x48B1,
    0x00F0,
    0x73FB,
    0x30B1,
    0x40F2,
    0x1340,
    0xA081,
    0x40F2,
    0x0110,
    0xE081,
    0x2082,
    0x2B6A,
    0x0021,
    0x8320,
    0x109A,
    0x00F0,
    0x6AFB,
    0xE881,
    0x00F0,
    0x58FB,
    0x0126,
    0x0128,
    0x12D1,
    0x00F0,
    0x58FB,
    0x78B1,
    0x00F0,
    0x5AFB,
    0x60B1,
    0x2680,
    0x3048,
    0x0021,
    0x04E0,
    0x0288,
    0x5208,
    0x20F8,
    0x022B,
    0x491C,
    0xEA89,
    0xB1EB,
    0x420F,
    0xF6DB,
    0xE989,
    0xA889,
    0x8142,
    0x00D9,
    0xE881,
    0x2680,
    0x07B0,
    0xBDE8,
    0xF08F,
    0x2DE9,
    0xF843,
    0x1A48,
    0x0022,
    0x4069,
    0x85B2,
    0x4FEA,
    0x1048,
    0x2946,
    0x4046,
    0x00F0,
    0xFBFA,
    0x00F0,
    0x3FFB,
    0x204F,
    0x97F8,
    0x7300,
    0x30B1,
    0x1348,
    0x90F8,
    0x8B02,
    0x10B1,
    0x1D49,
    0x1B20,
    0x0880,
    0x1C48,
    0x0E4E,
    0x3436,
    0x90F8,
    0xC046,
    0xB089,
    0x98B9,
    0x0020,
    0xADF8,
    0x0000,
    0x0A48,
    0x0222,
    0x6946,
    0xB0F8,
    0x0006,
    0x2E30,
    0x00F0,
    0x27FB,
    0x10B1,
    0xBDF8,
    0x0000,
    0xB081,
    0xB089,
    0x10B9,
    0x4FF4,
    0x8060,
    0xB081,
    0x97F8,
    0x7500,
    0x1DE0,
    0x2000,
    0x4A40,
    0x2000,
    0x2ED0,
    0x2000,
    0x0E20,
    0x4000,
    0x8832,
    0x2000,
    0x3420,
    0x2000,
    0x21A0,
    0x2000,
    0x3F40,
    0x2000,
    0x3E70,
    0x4000,
    0xA000,
    0x2000,
    0x38C0,
    0x2000,
    0x2210,
    0x2000,
    0x8000,
    0x2000,
    0x2850,
    0x4000,
    0xF47E,
    0x2000,
    0x0FE0,
    0x28B1,
    0xB089,
    0x18B1,
    0x6043,
    0x00F5,
    0x0070,
    0x840A,
    0xFE48,
    0x4FF4,
    0x8072,
    0xB0F8,
    0x7C07,
    0x9042,
    0x01D9,
    0x0146,
    0x00E0,
    0x1146,
    0x8B01,
    0xA3F5,
    0x8043,
    0x9042,
    0x01D9,
    0x0146,
    0x00E0,
    0x1146,
    0x01FB,
    0x0431,
    0xFF23,
    0xB3EB,
    0x112F,
    0x0ED9,
    0x9042,
    0x01D9,
    0x0146,
    0x00E0,
    0x1146,
    0x8901,
    0xA1F5,
    0x8041,
    0x9042,
    0x00D8,
    0x1046,
    0x00FB,
    0x0410,
    0x000A,
    0x00E0,
    0xFF20,
    0xEB49,
    0x0880,
    0x2946,
    0x4046,
    0xBDE8,
    0xF843,
    0x0122,
    0x00F0,
    0x7ABA,
    0x70B5,
    0xE748,
    0x0022,
    0x8169,
    0x0C0C,
    0x8DB2,
    0x2946,
    0x2046,
    0x00F0,
    0x70FA,
    0x00F0,
    0xBEFA,
    0xE248,
    0x90F8,
    0x7410,
    0x11B1,
    0x0021,
    0x80F8,
    0x7010,
    0xE048,
    0x4FF4,
    0x8071,
    0x90F8,
    0x6F20,
    0x4FF4,
    0x3040,
    0x00F0,
    0x5EFA,
    0x2946,
    0x2046,
    0xBDE8,
    0x7040,
    0x0122,
    0x00F0,
    0x57BA,
    0x70B5,
    0x0446,
    0xD648,
    0xD74D,
    0x90F8,
    0x0804,
    0xC8B1,
    0x2846,
    0x90F8,
    0x0906,
    0xA8B1,
    0x2846,
    0xD5F8,
    0x8423,
    0xC0F8,
    0x1424,
    0x00F2,
    0x1441,
    0x2A46,
    0xD5F8,
    0x9003,
    0xC2F8,
    0x2004,
    0xD5F8,
    0xC043,
    0x1046,
    0xC5F8,
    0xE442,
    0xC0F8,
    0x3044,
    0x0846,
    0x00F0,
    0x8BFA,
    0xC749,
    0xB5F8,
    0xB022,
    0x088F,
    0x498F,
    0x201A,
    0x401E,
    0x1144,
    0x8142,
    0x00D9,
    0x0846,
    0xA5F8,
    0xB202,
    0x70BD,
    0x2DE9,
    0xF041,
    0x0646,
    0xBD48,
    0x0022,
    0x006A,
    0x85B2,
    0x040C,
    0x2946,
    0x2046,
    0x00F0,
    0x1CFA,
    0x3046,
    0x00F0,
    0x73FA,
    0xBB48,
    0xBB4F,
    0x0068,
    0x3B68,
    0x418B,
    0x090A,
    0x83F8,
    0x3610,
    0xC17E,
    0x83F8,
    0x3810,
    0xB449,
    0x91F8,
    0x4C21,
    0x002A,
    0xD1F8,
    0x3421,
    0x01D0,
    0x521C,
    0x5208,
    0xCE33,
    0x160A,
    0x1E71,
    0x9A71,
    0xB1F8,
    0x3C21,
    0xC2F3,
    0x5712,
    0x1A70,
    0x91F8,
    0x3D21,
    0xD200,
    0x9A70,
    0x91F8,
    0x4D21,
    0xCE3B,
    0x22B1,
    0xD1F8,
    0x3821,
    0x521C,
    0x5608,
    0x01E0,
    0xD1F8,
    0x3861,
    0x7A68,
    0x4FEA,
    0x162C,
    0x01F5,
    0x9071,
    0x82F8,
    0x16C0,
    0x1676,
    0xCE8B,
    0x00F5,
    0xBA70,
    0xC6F3,
    0x5716,
    0x9674,
    0xCE7F,
    0xF600,
    0x1675,
    0x0E8C,
    0xCF68,
    0xF608,
    0x7E43,
    0x360B,
    0x370A,
    0x03F8,
    0xD67F,
    0x7732,
    0x9E70,
    0x0688,
    0x360A,
    0x02F8,
    0x276C,
    0x4678,
    0x02F8,
    0x256C,
    0x8688,
    0x360A,
    0x02F8,
    0x1F6C,
    0x4679,
    0x02F8,
    0x1D6C,
    0x8F4E,
    0x96F8,
    0x1064,
    0xD671,
    0x8D4E,
    0x96F8,
    0x1164,
    0x5672,
    0x8B4E,
    0x96F8,
    0x0B64,
    0xD672,
    0x894E,
    0x96F8,
    0x0964,
    0x5673,
    0x90F8,
    0x3060,
    0xD673,
    0x90F8,
    0xDE00,
    0x02F8,
    0x1F0F,
    0x8448,
    0x00F2,
    0x7246,
    0x90F8,
    0x7204,
    0x9074,
    0x3078,
    0x1075,
    0xA522,
    0xDA70,
    0x0E20,
    0x1871,
    0x11F8,
    0x7E0C,
    0xC0F1,
    0x0C01,
    0x7C48,
    0xD0F8,
    0x3C04,
    0xC840,
    0x060A,
    0x9E71,
    0x1872,
    0x0120,
    0x03F8,
    0x2C0C,
    0x7748,
    0xD0F8,
    0x4C04,
    0xC840,
    0xAA21,
    0x03F8,
    0x571D,
    0x0226,
    0x5E70,
    0x9A70,
    0x3022,
    0xDA70,
    0x5A22,
    0x1A71,
    0x060A,
    0x5E71,
    0x9A71,
    0xD871,
    0x1972,
    0x0020,
    0x5872,
    0x2946,
    0x2046,
    0xBDE8,
    0xF041,
    0x0122,
    0x00F0,
    0x77B9,
    0x2DE9,
    0xF041,
    0x0746,
    0x6448,
    0x0C46,
    0x0022,
    0x406A,
    0x86B2,
    0x050C,
    0x3146,
    0x2846,
    0x00F0,
    0x6AF9,
    0x2146,
    0x3846,
    0x00F0,
    0xC5F9,
    0x6048,
    0x90F8,
    0x9702,
    0x10B9,
    0x00F0,
    0x97F9,
    0x20B1,
    0x04F1,
    0x8044,
    0xA08A,
    0x401C,
    0xA082,
    0x3146,
    0x2846,
    0xBDE8,
    0xF041,
    0x0122,
    0x00F0,
    0x53B9,
    0x2DE9,
    0xF041,
    0x0746,
    0x5248,
    0x0E46,
    0x0022,
    0x806A,
    0x85B2,
    0x040C,
    0x2946,
    0x2046,
    0x00F0,
    0x46F9,
    0x3146,
    0x3846,
    0x00F0,
    0xA6F9,
    0x4B4F,
    0x4DF2,
    0x0C26,
    0x3437,
    0x4FF4,
    0x8061,
    0x3A78,
    0x3046,
    0x00F0,
    0x38F9,
    0x7878,
    0xC8B3,
    0x0022,
    0x4FF4,
    0x0071,
    0x3046,
    0x00F0,
    0x30F9,
    0x4848,
    0x0088,
    0x484B,
    0xA3F8,
    0x4402,
    0x4648,
    0x001D,
    0x0088,
    0xA3F8,
    0x4602,
    0xB3F8,
    0x4402,
    0xB3F8,
    0x4612,
    0x4218,
    0x02D0,
    0x8002,
    0xB0FB,
    0xF2F2,
    0x91B2,
    0x404A,
    0xA3F8,
    0x4812,
    0x5088,
    0x1288,
    0x3D4B,
    0xA3F8,
    0xA605,
    0xA3F8,
    0xA825,
    0x8018,
    0x05D0,
    0x9202,
    0xB2FB,
    0xF0F0,
    0x1A46,
    0xA2F8,
    0xAA05,
    0x3648,
    0xB0F8,
    0xAA05,
    0x0A18,
    0x01FB,
    0x1020,
    0x40F3,
    0x9510,
    0x1028,
    0x06DC,
    0x0028,
    0x05DA,
    0x0020,
    0x03E0,
    0xFFE7,
    0x0122,
    0xC3E7,
    0x1020,
    0x2F49,
    0x0880,
    0x2946,
    0x2046,
    0xBDE8,
    0xF041,
    0x0122,
    0x00F0,
    0xEFB8,
    0x70B5,
    0x2148,
    0x0022,
    0xC16A,
    0x0C0C,
    0x8DB2,
    0x2946,
    0x2046,
    0x00F0,
    0xE5F8,
    0x2148,
    0x0268,
    0xB2F8,
    0x6202,
    0x8301,
    0x92F8,
    0x6002,
    0x10F0,
    0x020F,
    0x09D0,
    0x1848,
    0x3430,
    0x8188,
    0x9942,
    0x06D8,
    0x4088,
    0xA0F5,
    0x5141,
    0x2339,
    0x01D1,
    0x00F0,
    0x38F9,
    0x2946,
    0x2046,
    0xBDE8,
    0x7040,
    0x0122,
    0x00F0,
    0xC8B8,
    0x70B5,
    0x0646,
    0x0D48,
    0x0022,
    0x016B,
    0x0C0C,
    0x8DB2,
    0x2946,
    0x2046,
    0x00F0,
    0xBDF8,
    0x3046,
    0x00F0,
    0x28F9,
    0x0749,
    0x114A,
    0x3431,
    0xCB79,
    0xD068,
    0x9840,
    0xD060,
    0x1068,
    0x9840,
    0x1060,
    0x8868,
    0x19E0,
    0x2000,
    0x0FE0,
    0x4000,
    0xF474,
    0x2000,
    0x4A40,
    0x2000,
    0x2850,
    0x2000,
    0x0E20,
    0x2000,
    0x2ED0,
    0x2000,
    0x08D0,
    0x2000,
    0x36E0,
    0x4000,
    0x9404,
    0x2000,
    0x38C0,
    0x4000,
    0xD214,
    0x4000,
    0xA410,
    0x2000,
    0x3254,
    0xD063,
    0x2946,
    0x2046,
    0xBDE8,
    0x7040,
    0x0122,
    0x00F0,
    0x8CB8,
    0x10B5,
    0x0022,
    0xAFF6,
    0x9701,
    0x3348,
    0x00F0,
    0xF8F8,
    0x334C,
    0x0022,
    0xAFF6,
    0x3F01,
    0x2060,
    0x3148,
    0x00F0,
    0xF0F8,
    0x6060,
    0xAFF2,
    0x4970,
    0x2F49,
    0x0022,
    0xC861,
    0xAFF2,
    0x9F61,
    0x2E48,
    0x00F0,
    0xE5F8,
    0x0022,
    0xAFF2,
    0x2D51,
    0x2061,
    0x2B48,
    0x00F0,
    0xDEF8,
    0x0022,
    0xAFF2,
    0x2341,
    0x6061,
    0x2948,
    0x00F0,
    0xD7F8,
    0x0022,
    0xAFF2,
    0xE931,
    0xA061,
    0x2648,
    0x00F0,
    0xD0F8,
    0x0022,
    0xAFF2,
    0x6B71,
    0xE061,
    0x2448,
    0x00F0,
    0xC9F8,
    0x0022,
    0xAFF2,
    0x2371,
    0xA060,
    0x2148,
    0x00F0,
    0xC2F8,
    0x0022,
    0xAFF2,
    0xB731,
    0xE060,
    0x1F48,
    0x00F0,
    0xBBF8,
    0x0022,
    0xAFF2,
    0x6121,
    0x2062,
    0x1C48,
    0x00F0,
    0xB4F8,
    0x6062,
    0x0020,
    0x04F1,
    0x3401,
    0x0246,
    0x8881,
    0xAFF2,
    0x3121,
    0x1848,
    0x00F0,
    0xA9F8,
    0x0022,
    0xAFF2,
    0x7511,
    0xA062,
    0x1548,
    0x00F0,
    0xA2F8,
    0x0022,
    0xAFF2,
    0x3711,
    0xE062,
    0x1348,
    0x00F0,
    0x9BF8,
    0x1249,
    0x2063,
    0x40F6,
    0xF100,
    0x0968,
    0x4883,
    0x10BD,
    0x0000,
    0x0000,
    0xDE1F,
    0x2000,
    0x4A40,
    0x0000,
    0x5F3B,
    0x2000,
    0x0850,
    0x0000,
    0xD719,
    0x0000,
    0x27FF,
    0x0000,
    0x39E3,
    0x0001,
    0x32CF,
    0x0001,
    0x1E3B,
    0x0000,
    0xEC45,
    0x0000,
    0x67B9,
    0x0000,
    0xE62B,
    0x0001,
    0x2265,
    0x0000,
    0x8C83,
    0x0000,
    0x5449,
    0x2000,
    0x08D0,
    0x4AF2,
    0x2B1C,
    0xC0F2,
    0x000C,
    0x6047,
    0x4DF6,
    0x1F6C,
    0xC0F2,
    0x000C,
    0x6047,
    0x44F6,
    0x655C,
    0xC0F2,
    0x010C,
    0x6047,
    0x45F6,
    0x433C,
    0xC0F2,
    0x000C,
    0x6047,
    0x45F6,
    0xE36C,
    0xC0F2,
    0x000C,
    0x6047,
    0x46F2,
    0x7B1C,
    0xC0F2,
    0x000C,
    0x6047,
    0x44F6,
    0xD90C,
    0xC0F2,
    0x000C,
    0x6047,
    0x44F6,
    0x791C,
    0xC0F2,
    0x000C,
    0x6047,
    0x44F6,
    0x811C,
    0xC0F2,
    0x000C,
    0x6047,
    0x44F2,
    0xB50C,
    0xC0F2,
    0x000C,
    0x6047,
    0x44F6,
    0xE90C,
    0xC0F2,
    0x000C,
    0x6047,
    0x43F6,
    0x155C,
    0xC0F2,
    0x000C,
    0x6047,
    0x43F6,
    0x1D5C,
    0xC0F2,
    0x000C,
    0x6047,
    0x4DF2,
    0xC95C,
    0xC0F2,
    0x000C,
    0x6047,
    0x42F2,
    0xFF7C,
    0xC0F2,
    0x000C,
    0x6047,
    0x48F2,
    0x712C,
    0xC0F2,
    0x000C,
    0x6047,
    0x43F6,
    0xE31C,
    0xC0F2,
    0x000C,
    0x6047,
    0x43F2,
    0x374C,
    0xC0F2,
    0x010C,
    0x6047,
    0x46F2,
    0xB97C,
    0xC0F2,
    0x000C,
    0x6047,
    0x4EF2,
    0x2B6C,
    0xC0F2,
    0x000C,
    0x6047,
    0x42F2,
    0x652C,
    0xC0F2,
    0x010C,
    0x6047,
    0x48F6,
    0x834C,
    0xC0F2,
    0x000C,
    0x6047,
    0x45F2,
    0x494C,
    0xC0F2,
    0x000C,
    0x6047,
    0x4CF2,
    0x2D1C,
    0xC0F2,
    0x000C,
    0x6047,
};

static u16 casiofront_sensor_init_setting_array3[] = {
    0x6028, 0x2000,
    0x602A, 0x16F0,
    0x6F12, 0x2929,
    0x602A, 0x16F2,
    0x6F12, 0x2929,
    0x602A, 0x16FA,
    0x6F12, 0x0029,
    0x602A, 0x16FC,
    0x6F12, 0x0029,
    0x602A, 0x1708,
    0x6F12, 0x0029,
    0x602A, 0x170A,
    0x6F12, 0x0029,
    0x602A, 0x1712,
    0x6F12, 0x2929,
    0x602A, 0x1714,
    0x6F12, 0x2929,
    0x602A, 0x1716,
    0x6F12, 0x2929,
    0x602A, 0x1722,
    0x6F12, 0x152A,
    0x602A, 0x1724,
    0x6F12, 0x152A,
    0x602A, 0x172C,
    0x6F12, 0x002A,
    0x602A, 0x172E,
    0x6F12, 0x002A,
    0x602A, 0x1736,
    0x6F12, 0x1500,
    0x602A, 0x1738,
    0x6F12, 0x1500,
    0x602A, 0x1740,
    0x6F12, 0x152A,
    0x602A, 0x1742,
    0x6F12, 0x152A,
    0x602A, 0x16BE,
    0x6F12, 0x1515,
    0x6F12, 0x1515,
    0x602A, 0x16C8,
    0x6F12, 0x0029,
    0x6F12, 0x0029,
    0x602A, 0x16D6,
    0x6F12, 0x0015,
    0x6F12, 0x0015,
    0x602A, 0x16E0,
    0x6F12, 0x2929,
    0x6F12, 0x2929,
    0x6F12, 0x2929,
    0x602A, 0x19B8,
    0x6F12, 0x0100,
    0x602A, 0x2224,
    0x6F12, 0x0100,
    0x602A, 0x0DF8,
    0x6F12, 0x1001,
    0x602A, 0x1EDA,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x6F12, 0x000E,
    0x602A, 0x16A0,
    0x6F12, 0x3D09,
    0x602A, 0x10A8,
    0x6F12, 0x000E,
    0x602A, 0x1198,
    0x6F12, 0x002B,
    0x602A, 0x1002,
    0x6F12, 0x0001,
    0x602A, 0x0F70,
    0x6F12, 0x0101,
    0x6F12, 0x002F,
    0x6F12, 0x007F,
    0x6F12, 0x0030,
    0x6F12, 0x0080,
    0x6F12, 0x000B,
    0x6F12, 0x0009,
    0x6F12, 0xF46E,
    0x602A, 0x0FAA,
    0x6F12, 0x000D,
    0x6F12, 0x0003,
    0x6F12, 0xF464,
    0x602A, 0x1698,
    0x6F12, 0x0D05,
    0x602A, 0x20A0,
    0x6F12, 0x0001,
    0x6F12, 0x0203,
    0x602A, 0x4A74,
    0x6F12, 0x0101,
    0x6F12, 0x0000,
    0x6F12, 0x1F80,
    0x6F12, 0x0000,
    0x6F12, 0x0000,
    0x6F12, 0x0000,
    0x602A, 0x0FF4,
    0x6F12, 0x0100,
    0x6F12, 0x1800,
    0x6028, 0x4000,
    0x0FEA, 0x1440,
    0x0B06, 0x0101,
    0xF44A, 0x0007,
    0xF456, 0x000A,
    0xF46A, 0xBFA0,
    0x0D80, 0x1388,
    0xB134, 0x0000,
    0xB136, 0x0000,
    0xB138, 0x0000,
};

static u16 casiofront_preview_setting[] = {
    0x6028, 0x4000,
    0x6214, 0x7970,
    0x6218, 0x7150,
    0x6028, 0x2000,
    0x602A, 0x0ED6,
    0x6F12, 0x0000,
    0x602A, 0x1CF0,
    0x6F12, 0x0200,
    0x602A, 0x0E58,
    0x6F12, 0x0023,
    0x602A, 0x1694,
    0x6F12, 0x170F,
    0x602A, 0x16AA,
    0x6F12, 0x009D,
    0x6F12, 0x000F,
    0x602A, 0x1098,
    0x6F12, 0x0012,
    0x602A, 0x2690,
    0x6F12, 0x0100,
    0x6F12, 0x0000,
    0x602A, 0x16A8,
    0x6F12, 0x38C0,
    0x602A, 0x108C,
    0x6F12, 0x0002,
    0x602A, 0x10CC,
    0x6F12, 0x0001,
    0x602A, 0x10D0,
    0x6F12, 0x000F,
    0x602A, 0x0F50,
    0x6F12, 0x0200,
    0x602A, 0x1758,
    0x6F12, 0x0000,
    0x6028, 0x4000,
    0x0344, 0x0010,
    0x0346, 0x0018,
    0x0348, 0x121F,
    0x034A, 0x0D97,
    0x034C, 0x0900,
    0x034E, 0x06C0,
    0x0350, 0x0004,
    0x0900, 0x0122,
    0x0380, 0x0002,
    0x0382, 0x0002,
    0x0384, 0x0002,
    0x0386, 0x0002,
    0x0404, 0x1000,
    0x0402, 0x1010,
    0x0400, 0x1010,
    0x0114, 0x0300,
    0x0110, 0x1002,
    0x0136, 0x1800,
    0x0300, 0x0007,
    0x0302, 0x0001,
    0x0304, 0x0006,
    0x0306, 0x00F5,
    0x0308, 0x0008,
    0x030A, 0x0001,
    0x030C, 0x0000,
    0x030E, 0x0004,
    0x0310, 0x0064,
    0x0312, 0x0000,
    0x6028, 0x2000,
    0x602A, 0x16A6,
    0x6F12, 0x006C,
    0x6028, 0x4000,
    0x0340, 0x0E50,
    0x0342, 0x13E0,
    0x0202, 0x0100,
    0x0200, 0x0100,
    0x021E, 0x0000,
    0x0D00, 0x0000,
    0x0D02, 0x0001,
    0x0804, 0x0200,
    0x0816, 0x1900,
};

static u16 casiofront_capture_setting[] = {
    0x6028, 0x4000,
    0x6214, 0x7970,
    0x6218, 0x7150,
    0x6028, 0x2000,
    0x602A, 0x0ED6,
    0x6F12, 0x0000,
    0x602A, 0x1CF0,
    0x6F12, 0x0200,
    0x602A, 0x0E58,
    0x6F12, 0x0023,
    0x602A, 0x1694,
    0x6F12, 0x170F,
    0x602A, 0x16AA,
    0x6F12, 0x009D,
    0x6F12, 0x000F,
    0x602A, 0x1098,
    0x6F12, 0x0012,
    0x602A, 0x2690,
    0x6F12, 0x0100,
    0x6F12, 0x0000,
    0x602A, 0x16A8,
    0x6F12, 0x38C0,
    0x602A, 0x108C,
    0x6F12, 0x0002,
    0x602A, 0x10CC,
    0x6F12, 0x0001,
    0x602A, 0x10D0,
    0x6F12, 0x000F,
    0x602A, 0x0F50,
    0x6F12, 0x0200,
    0x602A, 0x1758,
    0x6F12, 0x0000,
    0x6028, 0x4000,
    0x0344, 0x0010,
    0x0346, 0x0018,
    0x0348, 0x121F,
    0x034A, 0x0D97,
    0x034C, 0x0900,
    0x034E, 0x06C0,
    0x0350, 0x0004,
    0x0900, 0x0122,
    0x0380, 0x0002,
    0x0382, 0x0002,
    0x0384, 0x0002,
    0x0386, 0x0002,
    0x0404, 0x1000,
    0x0402, 0x1010,
    0x0400, 0x1010,
    0x0114, 0x0300,
    0x0110, 0x1002,
    0x0136, 0x1800,
    0x0300, 0x0007,
    0x0302, 0x0001,
    0x0304, 0x0006,
    0x0306, 0x00F5,
    0x0308, 0x0008,
    0x030A, 0x0001,
    0x030C, 0x0000,
    0x030E, 0x0004,
    0x0310, 0x0064,
    0x0312, 0x0000,
    0x6028, 0x2000,
    0x602A, 0x16A6,
    0x6F12, 0x006C,
    0x6028, 0x4000,
    0x0340, 0x0E50,
    0x0342, 0x13E0,
    0x0202, 0x0100,
    0x0200, 0x0100,
    0x021E, 0x0000,
    0x0D00, 0x0000,
    0x0D02, 0x0001,
    0x0804, 0x0200,
    0x0816, 0x1900,
};

static u16 casiofront_normal_video_setting[] = {
	0x6028, 0x4000,
	0x6214, 0x7970,
	0x6218, 0x7150,
	0x6028, 0x2000,
	0x602A, 0x0ED6,
	0x6F12, 0x0000,
	0x602A, 0x1CF0,
	0x6F12, 0x0200,
	0x602A, 0x0E58,
	0x6F12, 0x0023,
	0x602A, 0x1694,
	0x6F12, 0x170F,
	0x602A, 0x16AA,
	0x6F12, 0x009D,
	0x6F12, 0x000F,
	0x602A, 0x1098,
	0x6F12, 0x0012,
	0x602A, 0x2690,
	0x6F12, 0x0100,
	0x6F12, 0x0000,
	0x602A, 0x16A8,
	0x6F12, 0x38C0,
	0x602A, 0x108C,
	0x6F12, 0x0002,
	0x602A, 0x10CC,
	0x6F12, 0x0001,
	0x602A, 0x10D0,
	0x6F12, 0x000F,
	0x602A, 0x0F50,
	0x6F12, 0x0200,
	0x602A, 0x1758,
	0x6F12, 0x0000,
	0x6028, 0x4000,
	0x0344, 0x0010,
	0x0346, 0x01C8,
	0x0348, 0x121F,
	0x034A, 0x0BE7,
	0x034C, 0x0900,
	0x034E, 0x0510,
	0x0350, 0x0004,
	0x0900, 0x0122,
	0x0380, 0x0002,
	0x0382, 0x0002,
	0x0384, 0x0002,
	0x0386, 0x0002,
	0x0404, 0x1000,
	0x0402, 0x1010,
	0x0400, 0x1010,
	0x0114, 0x0300,
	0x0110, 0x1002,
	0x0136, 0x1800,
	0x0300, 0x0007,
	0x0302, 0x0001,
	0x0304, 0x0006,
	0x0306, 0x00F5,
	0x0308, 0x0008,
	0x030A, 0x0001,
	0x030C, 0x0000,
	0x030E, 0x0004,
	0x0310, 0x0070,
	0x0312, 0x0001,
	0x6028, 0x2000,
	0x602A, 0x16A6,
	0x6F12, 0x006C,
	0x6028, 0x4000,
	0x0340, 0x05A0,
	0x0342, 0x32A0,
	0x0202, 0x0100,
	0x0200, 0x0100,
	0x021E, 0x0000,
	0x0D00, 0x0000,
	0x0D02, 0x0001,
	0x0112, 0x0A0A,
	0x0804, 0x0200,
	0x0816, 0x1900,
};

static u16 casiofront_hs_video_setting[] = {
	0x6028, 0x4000,
	0x6214, 0x7970,
	0x6218, 0x7150,
	0x6028, 0x2000,
	0x602A, 0x0ED6,
	0x6F12, 0x0000,
	0x602A, 0x1CF0,
	0x6F12, 0x0200,
	0x602A, 0x0E58,
	0x6F12, 0x0023,
	0x602A, 0x1694,
	0x6F12, 0x170F,
	0x602A, 0x16AA,
	0x6F12, 0x009D,
	0x6F12, 0x000F,
	0x602A, 0x1098,
	0x6F12, 0x0012,
	0x602A, 0x2690,
	0x6F12, 0x0100,
	0x6F12, 0x0000,
	0x602A, 0x16A8,
	0x6F12, 0x38C0,
	0x602A, 0x108C,
	0x6F12, 0x0002,
	0x602A, 0x10CC,
	0x6F12, 0x0001,
	0x602A, 0x10D0,
	0x6F12, 0x000F,
	0x602A, 0x0F50,
	0x6F12, 0x0200,
	0x602A, 0x1758,
	0x6F12, 0x0000,
	0x6028, 0x4000,
	0x0344, 0x0010,
	0x0346, 0x01C8,
	0x0348, 0x121F,
	0x034A, 0x0BE7,
	0x034C, 0x0900,
	0x034E, 0x0510,
	0x0350, 0x0004,
	0x0900, 0x0122,
	0x0380, 0x0002,
	0x0382, 0x0002,
	0x0384, 0x0002,
	0x0386, 0x0002,
	0x0404, 0x1000,
	0x0402, 0x1010,
	0x0400, 0x1010,
	0x0114, 0x0300,
	0x0110, 0x1002,
	0x0136, 0x1800,
	0x0300, 0x0007,
	0x0302, 0x0001,
	0x0304, 0x0006,
	0x0306, 0x00F5,
	0x0308, 0x0008,
	0x030A, 0x0001,
	0x030C, 0x0000,
	0x030E, 0x0004,
	0x0310, 0x005c,
	0x0312, 0x0001,
	0x6028, 0x2000,
	0x602A, 0x16A6,
	0x6F12, 0x006C,
	0x6028, 0x4000,
	0x0340, 0x05A0,
	0x0342, 0x32A0,
	0x0202, 0x0100,
	0x0200, 0x0100,
	0x021E, 0x0000,
	0x0D00, 0x0000,
	0x0D02, 0x0001,
	0x0810, 0x001E,
	0x0804, 0x0200,
};

static u16 casiofront_slim_video_setting[] = {
	0x6028, 0x4000,
	0x6214, 0x7970,
	0x6218, 0x7150,
	0x6028, 0x2000,
	0x602A, 0x0ED6,
	0x6F12, 0x0000,
	0x602A, 0x1CF0,
	0x6F12, 0x0200,
	0x602A, 0x0E58,
	0x6F12, 0x0023,
	0x602A, 0x1694,
	0x6F12, 0x170F,
	0x602A, 0x16AA,
	0x6F12, 0x009D,
	0x6F12, 0x000F,
	0x602A, 0x1098,
	0x6F12, 0x0012,
	0x602A, 0x2690,
	0x6F12, 0x0100,
	0x6F12, 0x0000,
	0x602A, 0x16A8,
	0x6F12, 0x38C0,
	0x602A, 0x108C,
	0x6F12, 0x0002,
	0x602A, 0x10CC,
	0x6F12, 0x0001,
	0x602A, 0x10D0,
	0x6F12, 0x000F,
	0x602A, 0x0F50,
	0x6F12, 0x0200,
	0x602A, 0x1758,
	0x6F12, 0x0000,
	0x6028, 0x4000,
	0x0344, 0x0010,
	0x0346, 0x01C8,
	0x0348, 0x121F,
	0x034A, 0x0BE7,
	0x034C, 0x0900,
	0x034E, 0x0510,
	0x0350, 0x0004,
	0x0900, 0x0122,
	0x0380, 0x0002,
	0x0382, 0x0002,
	0x0384, 0x0002,
	0x0386, 0x0002,
	0x0404, 0x1000,
	0x0402, 0x1010,
	0x0400, 0x1010,
	0x0114, 0x0300,
	0x0110, 0x1002,
	0x0136, 0x1800,
	0x0300, 0x0007,
	0x0302, 0x0001,
	0x0304, 0x0006,
	0x0306, 0x00F5,
	0x0308, 0x0008,
	0x030A, 0x0001,
	0x030C, 0x0000,
	0x030E, 0x0004,
	0x0310, 0x005c,
	0x0312, 0x0001,
	0x6028, 0x2000,
	0x602A, 0x16A6,
	0x6F12, 0x006C,
	0x6028, 0x4000,
	0x0340, 0x05A0,
	0x0342, 0x32A0,
	0x0202, 0x0100,
	0x0200, 0x0100,
	0x021E, 0x0000,
	0x0D00, 0x0000,
	0x0D02, 0x0001,
	0x0810, 0x001E,
	0x0804, 0x0200,
};

static u16 casiofront_custom1_setting[] = {
	0x6028, 0x4000,
	0x6214, 0x7970,
	0x6218, 0x7150,
	0x6028, 0x2000,
	0x602A, 0x0ED6,
	0x6F12, 0x0000,
	0x602A, 0x1CF0,
	0x6F12, 0x0200,
	0x602A, 0x0E58,
	0x6F12, 0x0023,
	0x602A, 0x1694,
	0x6F12, 0x170F,
	0x602A, 0x16AA,
	0x6F12, 0x009D,
	0x6F12, 0x000F,
	0x602A, 0x1098,
	0x6F12, 0x0012,
	0x602A, 0x2690,
	0x6F12, 0x0100,
	0x6F12, 0x0000,
	0x602A, 0x16A8,
	0x6F12, 0x38C0,
	0x602A, 0x108C,
	0x6F12, 0x0002,
	0x602A, 0x10CC,
	0x6F12, 0x0001,
	0x602A, 0x10D0,
	0x6F12, 0x000F,
	0x602A, 0x0F50,
	0x6F12, 0x0200,
	0x602A, 0x1758,
	0x6F12, 0x0000,
	0x6028, 0x4000,
	0x0344, 0x0000,
	0x0346, 0x0008,
	0x0348, 0x122F,
	0x034A, 0x0DA7,
	0x034C, 0x0910,
	0x034E, 0x06D0,
	0x0350, 0x0004,
	0x0900, 0x0122,
	0x0380, 0x0002,
	0x0382, 0x0002,
	0x0384, 0x0002,
	0x0386, 0x0002,
	0x0404, 0x1000,
	0x0402, 0x1010,
	0x0400, 0x1010,
	0x0114, 0x0300,
	0x0110, 0x1002,
	0x0136, 0x1800,
	0x0300, 0x0007,
	0x0302, 0x0001,
	0x0304, 0x0006,
	0x0306, 0x00F5,
	0x0308, 0x0008,
	0x030A, 0x0001,
	0x030C, 0x0000,
	0x030E, 0x0004,
	0x0310, 0x007A,
	0x0312, 0x0001,
	0x0340, 0x0911,
	0x0342, 0x2734,
	0x0202, 0x0100,
	0x0200, 0x0100,
	0x021E, 0x0000,
	0x0D00, 0x0000,
	0x0D02, 0x0001,
    0x0810, 0x001E,
    0x0804, 0x0200,
};

static u16 casiofront_custom2_setting[] = {
	0x6028, 0x4000,
	0x6214, 0x7970,
	0x6218, 0x7150,
	0x6028, 0x2000,
	0x602A, 0x0ED6,
	0x6F12, 0x0000,
	0x602A, 0x1CF0,
	0x6F12, 0x0200,
	0x602A, 0x0E58,
	0x6F12, 0x0023,
	0x602A, 0x1694,
	0x6F12, 0x170F,
	0x602A, 0x16AA,
	0x6F12, 0x009D,
	0x6F12, 0x000F,
	0x602A, 0x1098,
	0x6F12, 0x0012,
	0x602A, 0x2690,
	0x6F12, 0x0100,
	0x6F12, 0x0000,
	0x602A, 0x16A8,
	0x6F12, 0x38C0,
	0x602A, 0x108C,
	0x6F12, 0x0002,
	0x602A, 0x10CC,
	0x6F12, 0x0001,
	0x602A, 0x10D0,
	0x6F12, 0x000F,
	0x602A, 0x0F50,
	0x6F12, 0x0200,
	0x602A, 0x1758,
	0x6F12, 0x0000,
	0x6028, 0x4000,
	0x0344, 0x0000,
	0x0346, 0x0008,
	0x0348, 0x122F,
	0x034A, 0x0DA7,
	0x034C, 0x0910,
	0x034E, 0x06D0,
	0x0350, 0x0004,
	0x0900, 0x0122,
	0x0380, 0x0002,
	0x0382, 0x0002,
	0x0384, 0x0002,
	0x0386, 0x0002,
	0x0404, 0x1000,
	0x0402, 0x1010,
	0x0400, 0x1010,
	0x0114, 0x0300,
	0x0110, 0x1002,
	0x0136, 0x1800,
	0x0300, 0x0007,
	0x0302, 0x0001,
	0x0304, 0x0006,
	0x0306, 0x00F5,
	0x0308, 0x0008,
	0x030A, 0x0001,
	0x030C, 0x0000,
	0x030E, 0x0004,
	0x0310, 0x0070,
	0x0312, 0x0001,
	0x0340, 0x0A30,
	0x0342, 0x1BF0,
	0x0202, 0x0100,
	0x0200, 0x0100,
	0x021E, 0x0000,
	0x0D00, 0x0000,
	0x0D02, 0x0001,
    0x0810, 0x001E,
    0x0804, 0x0200,
};

static u16 casiofront_custom3_setting[] = {
	0x6028, 0x4000,
	0x6214, 0x7970,
	0x6218, 0x7150,
	0x6028, 0x2000,
	0x602A, 0x0ED6,
	0x6F12, 0x0100,
	0x602A, 0x1CF0,
	0x6F12, 0x0100,
	0x602A, 0x0E58,
	0x6F12, 0x0040,
	0x602A, 0x1694,
	0x6F12, 0x1B0F,
	0x602A, 0x16AA,
	0x6F12, 0x009D,
	0x6F12, 0x0007,
	0x602A, 0x1098,
	0x6F12, 0x000A,
	0x602A, 0x2690,
	0x6F12, 0x0000,
	0x6F12, 0x0055,
	0x602A, 0x16A8,
	0x6F12, 0x38CD,
	0x602A, 0x108C,
	0x6F12, 0x0003,
	0x602A, 0x10CC,
	0x6F12, 0x0008,
	0x602A, 0x10D0,
	0x6F12, 0x000F,
	0x602A, 0x0F50,
	0x6F12, 0x0200,
	0x602A, 0x1758,
	0x6F12, 0x0020,
	0x6028, 0x4000,
	0x0344, 0x0000,
	0x0346, 0x0008,
	0x0348, 0x122F,
	0x034A, 0x0DA7,
	0x034C, 0x1220,
	0x034E, 0x0DA0,
	0x0350, 0x0008,
	0x0900, 0x0011,
	0x0380, 0x0001,
	0x0382, 0x0001,
	0x0384, 0x0001,
	0x0386, 0x0001,
	0x0404, 0x1000,
	0x0402, 0x1010,
	0x0400, 0x1010,
	0x0114, 0x0300,
	0x0110, 0x1002,
	0x0136, 0x1800,
	0x0300, 0x0007,
	0x0302, 0x0001,
	0x0304, 0x0006,
	0x0306, 0x00F5,
	0x0308, 0x0008,
	0x030A, 0x0001,
	0x030C, 0x0000,
	0x030E, 0x0004,
	0x0310, 0x007A,
	0x0312, 0x0000,
	0x0340, 0x0E54,
	0x0342, 0x13E0,
	0x0202, 0x0100,
	0x0200, 0x0100,
	0x021E, 0x0000,
	0x0D00, 0x0000,
	0x0D02, 0x0001,
};

#endif
