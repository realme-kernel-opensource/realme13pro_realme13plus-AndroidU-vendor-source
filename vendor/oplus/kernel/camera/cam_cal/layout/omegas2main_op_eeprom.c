// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#define PFX "CAM_CAL_OMEGAS2MAIN"
#define pr_fmt(fmt) PFX "[%s] " fmt, __func__

#include <linux/kernel.h>
#include "cam_cal_list.h"
#include "eeprom_i2c_common_driver.h"
#include "eeprom_i2c_custom_driver.h"
#include "cam_cal_config.h"
#include "oplus_kd_imgsensor.h"

#define READ_4000K 0

static unsigned int do_single_lsc_omegas2main(struct EEPROM_DRV_FD_DATA *pdata,
		unsigned int start_addr, unsigned int block_size, unsigned int *pGetSensorCalData);
static unsigned int do_2a_gain_omegas2main(struct EEPROM_DRV_FD_DATA *pdata,
		unsigned int start_addr, unsigned int block_size, unsigned int *pGetSensorCalData);
static unsigned int do_lens_id_omegas2main(struct EEPROM_DRV_FD_DATA *pdata,
		unsigned int start_addr, unsigned int block_size, unsigned int *pGetSensorCalData);
static unsigned int do_pdaf_omegas2main(struct EEPROM_DRV_FD_DATA *pdata,
		unsigned int start_addr, unsigned int block_size, unsigned int *pGetSensorCalData);

static struct STRUCT_CALIBRATION_LAYOUT_STRUCT cal_layout_table = {
	0x00000006, 0x016B012B, CAM_CAL_SINGLE_EEPROM_DATA,
	{
		{0x00000001, 0x00000000, 0x00000000, do_module_version},
		{0x00000001, 0x00000000, 0x00000002, do_part_number},
		{0x00000001, 0x00001070, 0x0000074C, do_single_lsc_omegas2main},
		{0x00000001, 0x00000007, 0x0000000E, do_2a_gain_omegas2main}, //Start address, block size is useless
		{0x00000001, 0x000017D0, 0x00000A3A, do_pdaf_omegas2main},
		{0x00000000, 0x00000FAE, 0x00000550, do_stereo_data},
		{0x00000001, 0x00000000, 0x00008000, do_dump_all},
		{0x00000001, 0x00000008, 0x00000002, do_lens_id_omegas2main}
	}
};

struct STRUCT_CAM_CAL_CONFIG_STRUCT omegas2main_op_eeprom = {
	.name = "omegas2main_op_eeprom",
	.check_layout_function = layout_check,
	.read_function = Common_read_region,
	.layout = &cal_layout_table,
	.sensor_id = OMEGAS2MAIN_SENSOR_ID,
	.i2c_write_id = 0xA0,
	.max_size = 0x8000,
	.enable_preload = 1,
	.preload_size = 0x8000,
};

/*
unsigned int do_pdaf_omegas2main(struct EEPROM_DRV_FD_DATA *pdata,
		unsigned int start_addr, unsigned int block_size, unsigned int *pGetSensorCalData)
{
	struct STRUCT_CAM_CAL_DATA_STRUCT *pCamCalData =
				(struct STRUCT_CAM_CAL_DATA_STRUCT *)pGetSensorCalData;

	int read_data_size;
	int err =  CamCalReturnErr[pCamCalData->Command];

	pCamCalData->PDAF.Size_of_PDAF = block_size;
	debug_log("PDAF start_addr =%x table_size=%d\n", start_addr, block_size);

	read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
			start_addr, block_size, (unsigned char *)&pCamCalData->PDAF.Data[0]);
	if (read_data_size > 0)
		err = CAM_CAL_ERR_NO_ERR;

	debug_log("======================PDAF Data==================\n");
	debug_log("First five %x, %x, %x, %x, %x\n",
		pCamCalData->PDAF.Data[0],
		pCamCalData->PDAF.Data[1],
		pCamCalData->PDAF.Data[2],
		pCamCalData->PDAF.Data[3],
		pCamCalData->PDAF.Data[4]);
	debug_log("RETURN = 0x%x\n", err);
	debug_log("======================PDAF Data==================\n");

	return err;

}
*/

#define OMEGAS2MAIN_HVBIN_PDAF_PROC1_SIZE  (0x1BA0-0x17D0) //976      0x3D0
#define OMEGAS2MAIN_HVBIN_PDAF_PROC2_SIZE  (0x2208-0x1BB0) //1624     2600  0xA28
#define OMEGAS2MAIN_HVBIN_PDAF_PROC1_ADDR  (0x17D0)
#define OMEGAS2MAIN_HVBIN_PDAF_PROC2_ADDR  (0x1BB0)

//partial PD
#define OMEGAS2MAIN_PARTIAL_PD_PROC1_SIZE  (0x3590-0x33A0) //496       0X1F0
#define OMEGAS2MAIN_PARTIAL_PD_PROC2_SIZE  (0x398C-0x35A0) //1004      1500   0x5DC
#define OMEGAS2MAIN_PARTIAL_PD_PROC1_ADDR  (0x33A0)
#define OMEGAS2MAIN_PARTIAL_PD_PROC2_ADDR  (0x35A0)
unsigned int do_pdaf_omegas2main(struct EEPROM_DRV_FD_DATA *pdata,
		unsigned int start_addr, unsigned int block_size, unsigned int *pGetSensorCalData)
{
	struct STRUCT_CAM_CAL_DATA_STRUCT *pCamCalData =
				(struct STRUCT_CAM_CAL_DATA_STRUCT *)pGetSensorCalData;

	int read_data_size;
	int err =  CamCalReturnErr[pCamCalData->Command];
	unsigned char isValid = 0;
	int bios = 0;
	bool partial_pd_proc1_flag = false;
	bool partial_pd_proc2_flag = false;
	bool qpd_proc1_flag = false;
	bool qpd_proc2_flag = false;

	pCamCalData->PDAF.Size_of_PDAF = 0;
	debug_log("======================PDAF Data==================\n");
	debug_log("[%s] QPD proc1 start_addr =%x table_size=%d\n", __FUNCTION__, OMEGAS2MAIN_HVBIN_PDAF_PROC1_ADDR, OMEGAS2MAIN_HVBIN_PDAF_PROC1_SIZE);
	debug_log("[%s] QPD proc2 start_addr =%x table_size=%d\n", __FUNCTION__, OMEGAS2MAIN_HVBIN_PDAF_PROC2_ADDR, OMEGAS2MAIN_HVBIN_PDAF_PROC2_SIZE);
	debug_log("[%s] partial PD proc1 start_addr =%x table_size=%d\n", __FUNCTION__, OMEGAS2MAIN_PARTIAL_PD_PROC1_ADDR, OMEGAS2MAIN_PARTIAL_PD_PROC1_SIZE);
	debug_log("[%s] partial PD proc2 start_addr =%x table_size=%d\n", __FUNCTION__, OMEGAS2MAIN_PARTIAL_PD_PROC2_ADDR, OMEGAS2MAIN_PARTIAL_PD_PROC2_SIZE);

// partial PD
	isValid = 0;
	bios = 0;
// partial PD proc1
	read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
			OMEGAS2MAIN_PARTIAL_PD_PROC1_ADDR + OMEGAS2MAIN_PARTIAL_PD_PROC1_SIZE,
			1, (unsigned char *)&isValid);
	if(isValid != 1) {
		debug_log("[%s] partial PD proc1 unvalid\n", __FUNCTION__);
	} else {
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				OMEGAS2MAIN_PARTIAL_PD_PROC1_ADDR, OMEGAS2MAIN_PARTIAL_PD_PROC1_SIZE,
				(unsigned char *)&pCamCalData->PDAF.Data[bios]);
		if (read_data_size > 0) {
			debug_log("[%s] partial PD proc1 First five %x, %x, %x, %x, %x\n",
				__FUNCTION__,
				pCamCalData->PDAF.Data[bios],
				pCamCalData->PDAF.Data[bios + 1],
				pCamCalData->PDAF.Data[bios + 2],
				pCamCalData->PDAF.Data[bios + 3],
				pCamCalData->PDAF.Data[bios + 4]);
			bios += OMEGAS2MAIN_PARTIAL_PD_PROC1_SIZE;
			partial_pd_proc1_flag = true;
			debug_log("[%s] bios = %d  0x%x\n",__FUNCTION__, bios, bios);
		}
	}

	if(partial_pd_proc1_flag) {
	// partial PD proc2
		isValid = 0;
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				OMEGAS2MAIN_PARTIAL_PD_PROC2_ADDR + OMEGAS2MAIN_PARTIAL_PD_PROC2_SIZE,
				1, (unsigned char *)&isValid);
		if(isValid != 1) {
			debug_log("[%s]  proc2 unvalid\n", __FUNCTION__);
		} else {
			read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
					OMEGAS2MAIN_PARTIAL_PD_PROC2_ADDR, OMEGAS2MAIN_PARTIAL_PD_PROC2_SIZE,
					(unsigned char *)&pCamCalData->PDAF.Data[bios]);
			if (read_data_size > 0) {
				debug_log("[%s] partial PD proc2 First five %x, %x, %x, %x, %x\n",
					__FUNCTION__,
					pCamCalData->PDAF.Data[bios],
					pCamCalData->PDAF.Data[bios + 1],
					pCamCalData->PDAF.Data[bios + 2],
					pCamCalData->PDAF.Data[bios + 3],
					pCamCalData->PDAF.Data[bios + 4]);
				bios += OMEGAS2MAIN_PARTIAL_PD_PROC2_SIZE;
				partial_pd_proc2_flag = true;
				debug_log("[%s] bios = %d  0x%x\n",__FUNCTION__, bios, bios);
			}
		}
	}

	if(partial_pd_proc1_flag && partial_pd_proc2_flag) {
		pCamCalData->PDAF.Size_of_PDAF = bios;
	} else {
		bios = pCamCalData->PDAF.Size_of_PDAF;
		debug_log("[%s] partial PD eeprom error",__FUNCTION__);
	}
	err = CAM_CAL_ERR_NO_ERR;

// QPD
// QPD proc1
	read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
			OMEGAS2MAIN_HVBIN_PDAF_PROC1_ADDR + OMEGAS2MAIN_HVBIN_PDAF_PROC1_SIZE,
			1, (unsigned char *)&isValid);
	if(isValid != 1) {
		debug_log("[%s] QPD proc1 unvalid\n", __FUNCTION__);
	} else {
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				OMEGAS2MAIN_HVBIN_PDAF_PROC1_ADDR, OMEGAS2MAIN_HVBIN_PDAF_PROC1_SIZE,
				(unsigned char *)&pCamCalData->PDAF.Data[bios]);
		if (read_data_size > 0) {
			debug_log("[%s] QDP proc1 First five %x, %x, %x, %x, %x\n",
				__FUNCTION__,
				pCamCalData->PDAF.Data[bios],
				pCamCalData->PDAF.Data[bios + 1],
				pCamCalData->PDAF.Data[bios + 2],
				pCamCalData->PDAF.Data[bios + 3],
				pCamCalData->PDAF.Data[bios + 4]);
			bios += OMEGAS2MAIN_HVBIN_PDAF_PROC1_SIZE;
			qpd_proc1_flag = true;
			debug_log("[%s] bios = %d  0x%x\n",__FUNCTION__, bios, bios);
		}
	}

// QPD proc2
	if(qpd_proc1_flag) {
		isValid = 0;
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				OMEGAS2MAIN_HVBIN_PDAF_PROC2_ADDR + OMEGAS2MAIN_HVBIN_PDAF_PROC2_SIZE,
				1, (unsigned char *)&isValid);
		if(isValid != 1) {
			debug_log("[%s] QPD proc2 unvalid\n", __FUNCTION__);
		}
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				OMEGAS2MAIN_HVBIN_PDAF_PROC2_ADDR, OMEGAS2MAIN_HVBIN_PDAF_PROC2_SIZE,
				(unsigned char *)&pCamCalData->PDAF.Data[bios]);
		if (read_data_size > 0) {
			debug_log("[%s] QDP proc2 First five %x, %x, %x, %x, %x\n",
				__FUNCTION__,
				pCamCalData->PDAF.Data[bios],
				pCamCalData->PDAF.Data[bios + 1],
				pCamCalData->PDAF.Data[bios + 2],
				pCamCalData->PDAF.Data[bios + 3],
				pCamCalData->PDAF.Data[bios + 4]);
			bios += OMEGAS2MAIN_HVBIN_PDAF_PROC2_SIZE;
			qpd_proc2_flag = true;
			debug_log("[%s] bios = %d  0x%x\n",__FUNCTION__, bios, bios);
		}
	}

	if(qpd_proc1_flag && qpd_proc2_flag) {
		pCamCalData->PDAF.Size_of_PDAF = bios;
	} else {
		bios = pCamCalData->PDAF.Size_of_PDAF;
		debug_log("[%s] QPD eeprom error",__FUNCTION__);
	}
	if(pCamCalData->PDAF.Size_of_PDAF == 0) {
		err =  CamCalReturnErr[pCamCalData->Command];
	}

	debug_log("[%s] pCamCalData->PDAF.Size_of_PDAF= %d  0x%x\n", __FUNCTION__,
		pCamCalData->PDAF.Size_of_PDAF, pCamCalData->PDAF.Size_of_PDAF);
	debug_log("RETURN = 0x%x\n", err);
	debug_log("======================PDAF Data==================\n");

	return err;

}

static unsigned int do_single_lsc_omegas2main(struct EEPROM_DRV_FD_DATA *pdata,
		unsigned int start_addr, unsigned int block_size, unsigned int *pGetSensorCalData)
{
	struct STRUCT_CAM_CAL_DATA_STRUCT *pCamCalData =
				(struct STRUCT_CAM_CAL_DATA_STRUCT *)pGetSensorCalData;

	int read_data_size;
	unsigned int err = CamCalReturnErr[pCamCalData->Command];
	unsigned short table_size;

	if (pCamCalData->DataVer >= CAM_CAL_TYPE_NUM) {
		err = CAM_CAL_ERR_NO_DEVICE;
		error_log("Read Failed\n");
		show_cmd_error_log(pCamCalData->Command);
		return err;
	}
	if (block_size != CAM_CAL_SINGLE_LSC_SIZE)
		error_log("block_size(%d) is not match (%d)\n",
				block_size, CAM_CAL_SINGLE_LSC_SIZE);

	pCamCalData->SingleLsc.LscTable.MtkLcsData.MtkLscType = 2;//mtk type
	pCamCalData->SingleLsc.LscTable.MtkLcsData.PixId = 8;

	table_size = 1868;

	pr_debug("lsc table_size %d\n", table_size);
	pCamCalData->SingleLsc.LscTable.MtkLcsData.TableSize = table_size;
	if (table_size > 0) {
		pCamCalData->SingleLsc.TableRotation = 0;
		debug_log("u4Offset=%d u4Length=%d", start_addr, table_size);
		read_data_size = read_data(pdata,
			pCamCalData->sensorID, pCamCalData->deviceID,
			start_addr, table_size, (unsigned char *)
			&pCamCalData->SingleLsc.LscTable.MtkLcsData.SlimLscType);
		if (table_size == read_data_size)
			err = CAM_CAL_ERR_NO_ERR;
		else {
			error_log("Read Failed\n");
			err = CamCalReturnErr[pCamCalData->Command];
			show_cmd_error_log(pCamCalData->Command);
		}
	}
	#ifdef DEBUG_CALIBRATION_LOAD
	pr_debug("======================SingleLsc Data==================\n");
	pr_debug("[1st] = %x, %x, %x, %x\n",
		pCamCalData->SingleLsc.LscTable.Data[0],
		pCamCalData->SingleLsc.LscTable.Data[1],
		pCamCalData->SingleLsc.LscTable.Data[2],
		pCamCalData->SingleLsc.LscTable.Data[3]);
	pr_debug("[1st] = SensorLSC(1)?MTKLSC(2)?  %x\n",
		pCamCalData->SingleLsc.LscTable.MtkLcsData.MtkLscType);
	pr_debug("CapIspReg =0x%x, 0x%x, 0x%x, 0x%x, 0x%x",
		pCamCalData->SingleLsc.LscTable.MtkLcsData.CapIspReg[0],
		pCamCalData->SingleLsc.LscTable.MtkLcsData.CapIspReg[1],
		pCamCalData->SingleLsc.LscTable.MtkLcsData.CapIspReg[2],
		pCamCalData->SingleLsc.LscTable.MtkLcsData.CapIspReg[3],
		pCamCalData->SingleLsc.LscTable.MtkLcsData.CapIspReg[4]);
	pr_debug("RETURN = 0x%x\n", err);
	pr_debug("======================SingleLsc Data==================\n");
	#endif

	return err;
}

static unsigned int do_2a_gain_omegas2main(struct EEPROM_DRV_FD_DATA *pdata,
		unsigned int start_addr, unsigned int block_size, unsigned int *pGetSensorCalData)
{
	struct STRUCT_CAM_CAL_DATA_STRUCT *pCamCalData =
				(struct STRUCT_CAM_CAL_DATA_STRUCT *)pGetSensorCalData;
	int read_data_size;
	unsigned int err = CamCalReturnErr[pCamCalData->Command];

	long long CalGain, FacGain, CalValue;
	unsigned char AWBAFConfig = 0xf;

	unsigned short AFInf, AFMacro, AF_50cm;
	int tempMax = 0;
	int CalR = 1, CalGr = 1, CalGb = 1, CalG = 1, CalB = 1;
	int FacR = 1, FacGr = 1, FacGb = 1, FacG = 1, FacB = 1;
	int rgCalValue = 1, bgCalValue = 1;
	unsigned int awb_offset;

	(void) start_addr;
	(void) block_size;

	pr_debug("In %s: sensor_id=%x\n", __func__, pCamCalData->sensorID);
	memset((void *)&pCamCalData->Single2A, 0, sizeof(struct STRUCT_CAM_CAL_SINGLE_2A_STRUCT));
	/* Check rule */
	if (pCamCalData->DataVer >= CAM_CAL_TYPE_NUM) {
		err = CAM_CAL_ERR_NO_DEVICE;
		error_log("Read Failed\n");
		show_cmd_error_log(pCamCalData->Command);
		return err;
	}
	/* Check AWB & AF enable bit */
	pCamCalData->Single2A.S2aVer = 0x01;
	pCamCalData->Single2A.S2aBitEn = (0x03 & AWBAFConfig);
	pCamCalData->Single2A.S2aAfBitflagEn = (0x0C & AWBAFConfig);
	debug_log("S2aBitEn=0x%02x", pCamCalData->Single2A.S2aBitEn);
	/* AWB Calibration Data*/
	if (0x1 & AWBAFConfig) {
		pCamCalData->Single2A.S2aAwb.rGainSetNum = 0x02;
		awb_offset = 0x60;
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				awb_offset, 8, (unsigned char *)&CalValue);
		if (read_data_size > 0)	{
			debug_log( "Read CalValue OK\n");
			rgCalValue  = CalValue & 0xFFFF;
			bgCalValue = (CalValue >> 16) & 0xFFFF;
			debug_log("Light source calibration 5100K value R/G:%d, B/G:%d",rgCalValue, bgCalValue);
			err = CAM_CAL_ERR_NO_ERR;
		} else {
			pCamCalData->Single2A.S2aBitEn = CAM_CAL_NONE_BITEN;
			error_log("Read CalGain Failed\n");
			show_cmd_error_log(pCamCalData->Command);
		}
		/* AWB Unit Gain (5000K) */
		debug_log("5000K AWB\n");
		awb_offset = 0x20;
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				awb_offset, 8, (unsigned char *)&CalGain);
		if (read_data_size > 0)	{
			debug_log("Read CalGain OK %x\n", read_data_size);
			CalR  = CalGain & 0xFFFF;
			CalGr = (CalGain >> 16) & 0xFFFF;
			CalGb = (CalGain >> 32) & 0xFFFF;
			CalG  = ((CalGr + CalGb) + 1) >> 1;
			CalB  = (CalGain >> 48) & 0xFFFF;
			CalR  = CalR * rgCalValue / 1000;
			CalB  = CalB * bgCalValue / 1000;
			if (CalR > CalG)
				/* R > G */
				if (CalR > CalB)
					tempMax = CalR;
				else
					tempMax = CalB;
			else
				/* G > R */
				if (CalG > CalB)
					tempMax = CalG;
				else
					tempMax = CalB;
			debug_log("UnitR:%d, UnitG:%d, UnitB:%d, New Unit Max=%d",
					CalR, CalG, CalB, tempMax);
			err = CAM_CAL_ERR_NO_ERR;
		} else {
			pCamCalData->Single2A.S2aBitEn = CAM_CAL_NONE_BITEN;
			error_log("Read CalGain Failed\n");
			show_cmd_error_log(pCamCalData->Command);
		}
		if (CalGain != 0x0000000000000000 &&
			CalGain != 0xFFFFFFFFFFFFFFFF &&
			CalR    != 0x00000000 &&
			CalG    != 0x00000000 &&
			CalB    != 0x00000000) {
			pCamCalData->Single2A.S2aAwb.rGainSetNum = 1;
			pCamCalData->Single2A.S2aAwb.rUnitGainu4R =
					(unsigned int)((tempMax * 512 + (CalR >> 1)) / CalR);
			pCamCalData->Single2A.S2aAwb.rUnitGainu4G =
					(unsigned int)((tempMax * 512 + (CalG >> 1)) / CalG);
			pCamCalData->Single2A.S2aAwb.rUnitGainu4B =
					(unsigned int)((tempMax * 512 + (CalB >> 1)) / CalB);
		} else {
			pr_debug("There are something wrong on EEPROM, plz contact module vendor!!\n");
			pr_debug("Unit R=%d G=%d B=%d!!\n", CalR, CalG, CalB);
		}
		/* AWB Golden Gain (5000K) */
		awb_offset = 0x28;
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				awb_offset, 8, (unsigned char *)&FacGain);
		if (read_data_size > 0)	{
			debug_log("Read FacGain OK\n");
			FacR  = FacGain & 0xFFFF;
			FacGr = (FacGain >> 16) & 0xFFFF;
			FacGb = (FacGain >> 32) & 0xFFFF;
			FacG  = ((FacGr + FacGb) + 1) >> 1;
			FacB  = (FacGain >> 48) & 0xFFFF;
			if (FacR > FacG)
				if (FacR > FacB)
					tempMax = FacR;
				else
					tempMax = FacB;
			else
				if (FacG > FacB)
					tempMax = FacG;
				else
					tempMax = FacB;
			debug_log("GoldenR:%d, GoldenG:%d, GoldenB:%d, New Golden Max=%d",
					FacR, FacG, FacB, tempMax);
			err = CAM_CAL_ERR_NO_ERR;
		} else {
			pCamCalData->Single2A.S2aBitEn = CAM_CAL_NONE_BITEN;
			error_log("Read FacGain Failed\n");
			show_cmd_error_log(pCamCalData->Command);
		}
		if (FacGain != 0x0000000000000000 &&
			FacGain != 0xFFFFFFFFFFFFFFFF &&
			FacR    != 0x00000000 &&
			FacG    != 0x00000000 &&
			FacB    != 0x00000000)	{
			pCamCalData->Single2A.S2aAwb.rGoldGainu4R =
					(unsigned int)((tempMax * 512 + (FacR >> 1)) / FacR);
			pCamCalData->Single2A.S2aAwb.rGoldGainu4G =
					(unsigned int)((tempMax * 512 + (FacG >> 1)) / FacG);
			pCamCalData->Single2A.S2aAwb.rGoldGainu4B =
					(unsigned int)((tempMax * 512 + (FacB >> 1)) / FacB);
		} else {
			pr_debug("There are something wrong on EEPROM, plz contact module vendor!!");
			pr_debug("Golden R=%d G=%d B=%d\n", FacR, FacG, FacB);
		}
		/* Set AWB to 3A Layer */
		pCamCalData->Single2A.S2aAwb.rValueR   = CalR;
		pCamCalData->Single2A.S2aAwb.rValueGr  = CalGr;
		pCamCalData->Single2A.S2aAwb.rValueGb  = CalGb;
		pCamCalData->Single2A.S2aAwb.rValueB   = CalB;
		pCamCalData->Single2A.S2aAwb.rGoldenR  = FacR;
		pCamCalData->Single2A.S2aAwb.rGoldenGr = FacGr;
		pCamCalData->Single2A.S2aAwb.rGoldenGb = FacGb;
		pCamCalData->Single2A.S2aAwb.rGoldenB  = FacB;
		#ifdef DEBUG_CALIBRATION_LOAD
		pr_debug("======================AWB CAM_CAL==================\n");
		pr_debug("AWB Calibration @5100K\n");
		pr_debug("[CalGain] = 0x%x\n", CalGain);
		pr_debug("[FacGain] = 0x%x\n", FacGain);
		pr_debug("[rCalGain.u4R] = %d\n", pCamCalData->Single2A.S2aAwb.rUnitGainu4R);
		pr_debug("[rCalGain.u4G] = %d\n", pCamCalData->Single2A.S2aAwb.rUnitGainu4G);
		pr_debug("[rCalGain.u4B] = %d\n", pCamCalData->Single2A.S2aAwb.rUnitGainu4B);
		pr_debug("[rFacGain.u4R] = %d\n", pCamCalData->Single2A.S2aAwb.rGoldGainu4R);
		pr_debug("[rFacGain.u4G] = %d\n", pCamCalData->Single2A.S2aAwb.rGoldGainu4G);
		pr_debug("[rFacGain.u4B] = %d\n", pCamCalData->Single2A.S2aAwb.rGoldGainu4B);
		#endif
		/* AWB Unit Gain (4000K) */
		#if READ_4000K //zemin.lai@CamTuning delet ,module not support 4000k otp 20220421
		CalR  = 0;
		CalGr = 0;
		CalGb = 0;
		CalG  = 0;
		CalB  = 0;
		tempMax = 0;
		debug_log("4000K AWB\n");
		awb_offset = 0x32;
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				awb_offset, 8, (unsigned char *)&CalGain);
		if (read_data_size > 0)	{
			debug_log("Read CalGain OK %x\n", read_data_size);
			CalR  = CalGain & 0xFFFF;
			CalGr = (CalGain >> 16) & 0xFFFF;
			CalGb = (CalGain >> 32) & 0xFFFF;
			CalG  = ((CalGr + CalGb) + 1) >> 1;
			CalB  = (CalGain >> 48) & 0xFFFF;
			if (CalR > CalG)
				/* R > G */
				if (CalR > CalB)
					tempMax = CalR;
				else
					tempMax = CalB;
			else
				/* G > R */
				if (CalG > CalB)
					tempMax = CalG;
				else
					tempMax = CalB;
			debug_log(
					"UnitR:%d, UnitG:%d, UnitB:%d, New Unit Max=%d",
					CalR, CalG, CalB, tempMax);
			err = CAM_CAL_ERR_NO_ERR;
		} else {
			pCamCalData->Single2A.S2aBitEn = CAM_CAL_NONE_BITEN;
			error_log("Read CalGain Failed\n");
			show_cmd_error_log(pCamCalData->Command);
		}
		if (CalGain != 0x0000000000000000 &&
			CalGain != 0xFFFFFFFFFFFFFFFF &&
			CalR    != 0x00000000 &&
			CalG    != 0x00000000 &&
			CalB    != 0x00000000) {
			pCamCalData->Single2A.S2aAwb.rUnitGainu4R_mid =
				(unsigned int)((tempMax * 512 + (CalR >> 1)) / CalR);
			pCamCalData->Single2A.S2aAwb.rUnitGainu4G_mid =
				(unsigned int)((tempMax * 512 + (CalG >> 1)) / CalG);
			pCamCalData->Single2A.S2aAwb.rUnitGainu4B_mid =
				(unsigned int)((tempMax * 512 + (CalB >> 1)) / CalB);
		} else {
			pr_debug("There are something wrong on EEPROM, plz contact module vendor!!\n");
			pr_debug("Unit R=%d G=%d B=%d!!\n", CalR, CalG, CalB);
		}
		/* AWB Golden Gain (4000K) */
		FacR  = 0;
		FacGr = 0;
		FacGb = 0;
		FacG  = 0;
		FacB  = 0;
		tempMax = 0;
		awb_offset = 0x3A;
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				awb_offset, 8, (unsigned char *)&FacGain);
		if (read_data_size > 0)	{
			debug_log("Read FacGain OK\n");
			FacR  = FacGain & 0xFFFF;
			FacGr = (FacGain >> 16) & 0xFFFF;
			FacGb = (FacGain >> 32) & 0xFFFF;
			FacG  = ((FacGr + FacGb) + 1) >> 1;
			FacB  = (FacGain >> 48) & 0xFFFF;
			if (FacR > FacG)
				if (FacR > FacB)
					tempMax = FacR;
				else
					tempMax = FacB;
			else
				if (FacG > FacB)
					tempMax = FacG;
				else
					tempMax = FacB;
			debug_log("GoldenR:%d, GoldenG:%d, GoldenB:%d, New Golden Max=%d",
					FacR, FacG, FacB, tempMax);
			err = CAM_CAL_ERR_NO_ERR;
		} else {
			pCamCalData->Single2A.S2aBitEn = CAM_CAL_NONE_BITEN;
			error_log("Read FacGain Failed\n");
			show_cmd_error_log(pCamCalData->Command);
		}
		if (FacGain != 0x0000000000000000 &&
			FacGain != 0xFFFFFFFFFFFFFFFF &&
			FacR    != 0x00000000 &&
			FacG    != 0x00000000 &&
			FacB    != 0x00000000)	{
			pCamCalData->Single2A.S2aAwb.rGoldGainu4R_mid =
				(unsigned int)((tempMax * 512 + (FacR >> 1)) / FacR);
			pCamCalData->Single2A.S2aAwb.rGoldGainu4G_mid =
				(unsigned int)((tempMax * 512 + (FacG >> 1)) / FacG);
			pCamCalData->Single2A.S2aAwb.rGoldGainu4B_mid =
				(unsigned int)((tempMax * 512 + (FacB >> 1)) / FacB);
		} else {
			pr_debug("There are something wrong on EEPROM, plz contact module vendor!!");
			pr_debug("Golden R=%d G=%d B=%d\n", FacR, FacG, FacB);
		}
		#ifdef DEBUG_CALIBRATION_LOAD
		pr_debug("AWB Calibration @4000K\n");
		pr_debug("[CalGain] = 0x%x\n", CalGain);
		pr_debug("[FacGain] = 0x%x\n", FacGain);
		pr_debug("[rCalGain.u4R] = %d\n", pCamCalData->Single2A.S2aAwb.rUnitGainu4R_mid);
		pr_debug("[rCalGain.u4G] = %d\n", pCamCalData->Single2A.S2aAwb.rUnitGainu4G_mid);
		pr_debug("[rCalGain.u4B] = %d\n", pCamCalData->Single2A.S2aAwb.rUnitGainu4B_mid);
		pr_debug("[rFacGain.u4R] = %d\n", pCamCalData->Single2A.S2aAwb.rGoldGainu4R_mid);
		pr_debug("[rFacGain.u4G] = %d\n", pCamCalData->Single2A.S2aAwb.rGoldGainu4G_mid);
		pr_debug("[rFacGain.u4B] = %d\n", pCamCalData->Single2A.S2aAwb.rGoldGainu4B_mid);
		#endif
		#endif
		/* AWB Unit Gain (3100K) */
		CalR  = 0;
		CalGr = 0;
		CalGb = 0;
		CalG  = 0;
		CalB  = 0;
		tempMax = 0;
		debug_log("2850K AWB\n");
		rgCalValue = 0;
		bgCalValue = 0;
		awb_offset = 0x6C;
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				awb_offset, 8, (unsigned char *)&CalValue);
		if (read_data_size > 0)	{
			debug_log( "Read CalValue OK\n");
			rgCalValue  = CalValue & 0xFFFF;
			bgCalValue = (CalValue >> 16) & 0xFFFF;
			debug_log("Light source calibration value 3100 R/G:%d, B/G:%d",rgCalValue, bgCalValue);
			err = CAM_CAL_ERR_NO_ERR;
		} else {
			pCamCalData->Single2A.S2aBitEn = CAM_CAL_NONE_BITEN;
			error_log("Read CalGain Failed\n");
			show_cmd_error_log(pCamCalData->Command);
		}
		awb_offset = 0x44;
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				awb_offset, 8, (unsigned char *)&CalGain);
		if (read_data_size > 0)	{
			debug_log("Read CalGain OK %x\n", read_data_size);
			CalR  = CalGain & 0xFFFF;
			CalGr = (CalGain >> 16) & 0xFFFF;
			CalGb = (CalGain >> 32) & 0xFFFF;
			CalG  = ((CalGr + CalGb) + 1) >> 1;
			CalB  = (CalGain >> 48) & 0xFFFF;
			debug_log("CalR:%d, CalB:%d",CalR, CalB);
			CalR  = CalR * rgCalValue / 1000;
			CalB  = CalB * bgCalValue / 1000;
			if (CalR > CalG)
				/* R > G */
				if (CalR > CalB)
					tempMax = CalR;
				else
					tempMax = CalB;
			else
				/* G > R */
				if (CalG > CalB)
					tempMax = CalG;
				else
					tempMax = CalB;
			debug_log("UnitR:%d, UnitG:%d, UnitB:%d, New Unit Max=%d",
					CalR, CalG, CalB, tempMax);
			err = CAM_CAL_ERR_NO_ERR;
		} else {
			pCamCalData->Single2A.S2aBitEn = CAM_CAL_NONE_BITEN;
			error_log("Read CalGain Failed\n");
			show_cmd_error_log(pCamCalData->Command);
		}
		if (CalGain != 0x0000000000000000 &&
			CalGain != 0xFFFFFFFFFFFFFFFF &&
			CalR    != 0x00000000 &&
			CalG    != 0x00000000 &&
			CalB    != 0x00000000) {
			pCamCalData->Single2A.S2aAwb.rGainSetNum = 2;
			pCamCalData->Single2A.S2aAwb.rUnitGainu4R_low =
				(unsigned int)((tempMax * 512 + (CalR >> 1)) / CalR);
			pCamCalData->Single2A.S2aAwb.rUnitGainu4G_low =
				(unsigned int)((tempMax * 512 + (CalG >> 1)) / CalG);
			pCamCalData->Single2A.S2aAwb.rUnitGainu4B_low =
				(unsigned int)((tempMax * 512 + (CalB >> 1)) / CalB);
		} else {
			pr_debug("There are something wrong on EEPROM, plz contact module vendor!!\n");
			pr_debug("Unit R=%d G=%d B=%d!!\n", CalR, CalG, CalB);
		}
		/* AWB Golden Gain (3100K) */
		FacR  = 0;
		FacGr = 0;
		FacGb = 0;
		FacG  = 0;
		FacB  = 0;
		tempMax = 0;
		awb_offset = 0x4C;
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				awb_offset, 8, (unsigned char *)&FacGain);
		if (read_data_size > 0)	{
			debug_log("Read FacGain OK\n");
			FacR  = FacGain & 0xFFFF;
			FacGr = (FacGain >> 16) & 0xFFFF;
			FacGb = (FacGain >> 32) & 0xFFFF;
			FacG  = ((FacGr + FacGb) + 1) >> 1;
			FacB  = (FacGain >> 48) & 0xFFFF;
			if (FacR > FacG)
				if (FacR > FacB)
					tempMax = FacR;
				else
					tempMax = FacB;
			else
				if (FacG > FacB)
					tempMax = FacG;
				else
					tempMax = FacB;
			debug_log("GoldenR:%d, GoldenG:%d, GoldenB:%d, New Golden Max=%d",
					FacR, FacG, FacB, tempMax);
			err = CAM_CAL_ERR_NO_ERR;
		} else {
			pCamCalData->Single2A.S2aBitEn = CAM_CAL_NONE_BITEN;
			error_log("Read FacGain Failed\n");
			show_cmd_error_log(pCamCalData->Command);
		}
		if (FacGain != 0x0000000000000000 &&
			FacGain != 0xFFFFFFFFFFFFFFFF &&
			FacR    != 0x00000000 &&
			FacG    != 0x00000000 &&
			FacB    != 0x00000000)	{
			pCamCalData->Single2A.S2aAwb.rGoldGainu4R_low =
				(unsigned int)((tempMax * 512 + (FacR >> 1)) / FacR);
			pCamCalData->Single2A.S2aAwb.rGoldGainu4G_low =
				(unsigned int)((tempMax * 512 + (FacG >> 1)) / FacG);
			pCamCalData->Single2A.S2aAwb.rGoldGainu4B_low =
				(unsigned int)((tempMax * 512 + (FacB >> 1)) / FacB);
		} else {
			pr_debug("There are something wrong on EEPROM, plz contact module vendor!!");
			pr_debug("Golden R=%d G=%d B=%d\n", FacR, FacG, FacB);
		}
		#ifdef DEBUG_CALIBRATION_LOAD
		pr_debug("AWB Calibration @3100K\n");
		pr_debug("[CalGain] = 0x%x\n", CalGain);
		pr_debug("[FacGain] = 0x%x\n", FacGain);
		pr_debug("[rCalGain.u4R] = %d\n", pCamCalData->Single2A.S2aAwb.rUnitGainu4R_low);
		pr_debug("[rCalGain.u4G] = %d\n", pCamCalData->Single2A.S2aAwb.rUnitGainu4G_low);
		pr_debug("[rCalGain.u4B] = %d\n", pCamCalData->Single2A.S2aAwb.rUnitGainu4B_low);
		pr_debug("[rFacGain.u4R] = %d\n", pCamCalData->Single2A.S2aAwb.rGoldGainu4R_low);
		pr_debug("[rFacGain.u4G] = %d\n", pCamCalData->Single2A.S2aAwb.rGoldGainu4G_low);
		pr_debug("[rFacGain.u4B] = %d\n", pCamCalData->Single2A.S2aAwb.rGoldGainu4B_low);
		pr_debug("======================AWB CAM_CAL==================\n");
		#endif
	}
	/* AF Calibration Data*/
	if (0x2 & AWBAFConfig) {
		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				0x96, 2, (unsigned char *)&AF_50cm);
		if (read_data_size > 0)
			err = CAM_CAL_ERR_NO_ERR;
		else {
			pCamCalData->Single2A.S2aBitEn = CAM_CAL_NONE_BITEN;
			error_log("Read Failed\n");
			show_cmd_error_log(pCamCalData->Command);
		}

		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				0x94, 2, (unsigned char *)&AFInf);
		if (read_data_size > 0)
			err = CAM_CAL_ERR_NO_ERR;
		else {
			pCamCalData->Single2A.S2aBitEn = CAM_CAL_NONE_BITEN;
			error_log("Read Failed\n");
			show_cmd_error_log(pCamCalData->Command);
		}

		read_data_size = read_data(pdata, pCamCalData->sensorID, pCamCalData->deviceID,
				0x92, 2, (unsigned char *)&AFMacro);
		if (read_data_size > 0)
			err = CAM_CAL_ERR_NO_ERR;
		else {
			pCamCalData->Single2A.S2aBitEn = CAM_CAL_NONE_BITEN;
			error_log("Read Failed\n");
			show_cmd_error_log(pCamCalData->Command);
		}

		AFInf = AFInf >> 2;
		AFMacro = AFMacro >> 2;
		AF_50cm = AF_50cm >> 2;

		pCamCalData->Single2A.S2aAf[0] = AFInf;
		pCamCalData->Single2A.S2aAf[1] = AFMacro;
		pCamCalData->Single2A.S2aAf[2] = AF_50cm;

		////Only AF Gathering <////
		#ifdef DEBUG_CALIBRATION_LOAD
		pr_debug("======================AF CAM_CAL==================\n");
		pr_debug("[AFInf] = %d\n", AFInf);
		pr_debug("[AFMacro] = %d\n", AFMacro);
		pr_debug("[AF_50cm] = %d\n", AF_50cm);
		pr_debug("======================AF CAM_CAL==================\n");
		#endif
	}
	return err;
}

static unsigned int do_lens_id_omegas2main(struct EEPROM_DRV_FD_DATA *pdata,
		unsigned int start_addr, unsigned int block_size, unsigned int *pGetSensorCalData)
{
	return do_lens_id_base(pdata, start_addr, block_size, pGetSensorCalData);
}
