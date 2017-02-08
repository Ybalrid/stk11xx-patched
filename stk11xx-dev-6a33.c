/** 
 * @file stk11xx-dev-6a33.c
 * @author Nicolas VIVIEN
 * @date 2006-10-23
 * @version v2.2.x
 *
 * @brief Driver for Syntek USB video camera
 *
 * @note Copyright (C) Nicolas VIVIEN
 *
 * @par Licences
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * @par SubVersion
 *   $Date: 2011-06-13 21:18:58 +0200 (lun. 13 juin 2011) $
 *   $Revision: 103 $
 *   $Author: nicklas79 $
 *   $HeadURL: https://svn.code.sf.net/p/syntekdriver/code/trunk/driver/stk11xx-dev-6a33.c $
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/kref.h>

#include <linux/usb.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>

#include "stk11xx.h"
#include "stk11xx-dev.h"


//=============================================================================
//
// STK-6A33 API
//
//=============================================================================


/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief This function permits to initialize the device.
 *
 * This function must be called at first. It's the start of the
 * initialization process. After this process, the device is
 * completly initalized and it's ready.
 *
 * This function is written from the USB log.
 */
int dev_stk6a33_initialize_device(struct usb_stk11xx *dev)
{
	int i;
	int value;

	STK_INFO("Initialize USB2.0 Syntek Camera\n");

	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0002, 0x0078);
	usb_stk11xx_write_registry(dev, 0x0003, 0x0080);

	usb_stk11xx_write_registry(dev, 0x0002, 0x007f);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0002, 0x007d);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);

	for (i=0; i<16; i++) {
		usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
		usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
		usb_stk11xx_read_registry(dev, 0x0000, &value);
	
		STK_DEBUG("Loop 1 : Read 0x0000 = %02X\n", value);
	}

	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0020);

	usb_stk11xx_write_registry(dev, 0x0002, 0x007f);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0002, 0x007d);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);

	for (i=0; i<16; i++) {
		usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
		usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
		usb_stk11xx_read_registry(dev, 0x0000, &value);

		STK_DEBUG("Loop 2 : Read 0x0000 = %02X\n", value);
	}

	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0020);

	usb_stk11xx_write_registry(dev, 0x0002, 0x007f);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0027);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0026);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0002, 0x007d);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);

	for (i=0; i<16; i++) {
		usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
		usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
		usb_stk11xx_read_registry(dev, 0x0000, &value);
	
		STK_DEBUG("Loop 3 : Read 0x0000 = %02X\n", value);
	}

	usb_stk11xx_write_registry(dev, 0x0000, 0x0025);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0020);
	usb_stk11xx_write_registry(dev, 0x0002, 0x007f);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0020);

	usb_stk11xx_write_registry(dev, 0x0117, 0x0000);
	usb_stk11xx_read_registry(dev, 0x0103, &value);
	usb_stk11xx_write_registry(dev, 0x0103, 0x0001);
	usb_stk11xx_read_registry(dev, 0x0103, &value);
	usb_stk11xx_write_registry(dev, 0x0103, 0x0000);

	usb_stk11xx_write_registry(dev, 0x0000, 0x00e0);
	usb_stk11xx_write_registry(dev, 0x0002, 0x00f8);
	usb_stk11xx_write_registry(dev, 0x0002, 0x0078);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0020);

	dev_stk6a33_configure_device(dev, 0);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 1);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 2);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 3);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 4);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 5);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 6);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 7);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 8);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 9);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 10);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 11);
	dev_stk11xx_check_device(dev, 65);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0008);

	dev_stk6a33_configure_device(dev, 12);
	usb_stk11xx_read_registry(dev, 0x02ff, &value);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0208, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0020);
	dev_stk11xx_check_device(dev, 500);
	usb_stk11xx_read_registry(dev, 0x0209, &value);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);

	dev_stk6a33_configure_device(dev, 13);
	usb_stk11xx_read_registry(dev, 0x02ff, &value);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0208, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0020);
	dev_stk11xx_check_device(dev, 500);
	usb_stk11xx_read_registry(dev, 0x0209, &value);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);
	usb_stk11xx_read_registry(dev, 0x02ff, &value);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0208, 0x00f1);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0020);
	dev_stk11xx_check_device(dev, 500);
	usb_stk11xx_read_registry(dev, 0x0209, &value);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);

	dev_stk6a33_configure_device(dev, 14);
	dev_stk11xx_check_device(dev, 500);
	usb_stk11xx_write_registry(dev, 0x0204, 0x00f1);
	usb_stk11xx_write_registry(dev, 0x0205, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0005);
	dev_stk11xx_check_device(dev, 500);
	usb_stk11xx_read_registry(dev, 0x02ff, &value);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0208, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0020);
	dev_stk11xx_check_device(dev, 500);
	usb_stk11xx_read_registry(dev, 0x0209, &value);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);
	usb_stk11xx_read_registry(dev, 0x02ff, &value);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0208, 0x00f1);
	usb_stk11xx_write_registry(dev, 0x0200, 0x0020);
	dev_stk11xx_check_device(dev, 500);
	usb_stk11xx_read_registry(dev, 0x0209, &value);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);

	dev_stk6a33_configure_device(dev, 15);

	dev_stk6a33_camera_asleep(dev);

	usb_stk11xx_set_feature(dev, 0);

	// Device is initialized and is ready !!!
	STK_INFO("Syntek USB2.0 Camera is ready\n");

	return 0;
}


/** 
 * @param dev Device structure
 * @param step The step of configuration [0-11]
 * 
 * @returns 0 if all is OK
 *
 * @brief This function permits to configure the device.
 *
 * The configuration of device is composed of 11 steps.
 * This function is called by the initialization process.
 *
 * We don't know the meaning of these steps ! We only replay the USB log.
 *
 * The steps 0 to 9 are called during the initialization.
 */
int dev_stk6a33_configure_device(struct usb_stk11xx *dev, int step)
{
	int value;

	//     0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17

	static const int values_001B[] = {
		0x0e, 0x03, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07
	};
	static const int values_001C[] = {
		0x06, 0x02, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06
	};
	static const int values_0202[] = {
		0x1e, 0x0a, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x14, 0x1e, 0x1f, 0x1f, 0x1f, 0x1f
	};
	static const int values_0110[] = {
		0x07, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	static const int values_0112[] = {
		0x07, 0x00, 0x00, 0x00, 0x00, 0x09, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	static const int values_0114[] = {
		0x87, 0x80, 0x80, 0x80, 0x80, 0xbe, 0xbe, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
	};
	static const int values_0115[] = {
		0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x05, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02
	};
	static const int values_0116[] = {
		0xe7, 0xe0, 0xe0, 0xe0, 0xe0, 0xe9, 0xe9, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0
	};
	static const int values_0117[] = {
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
	};
	static const int values_0100[] = {
		0x20, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20
	};

	STK_DEBUG("dev_stk6a33_configure_device : %d\n", step);

	usb_stk11xx_write_registry(dev, 0x0000, 0x0024);
	usb_stk11xx_write_registry(dev, 0x0002, 0x0068);
	usb_stk11xx_write_registry(dev, 0x0003, 0x0080);
	usb_stk11xx_write_registry(dev, 0x0005, 0x0000);
	
	usb_stk11xx_write_registry(dev, 0x0007, 0x0003);
	usb_stk11xx_write_registry(dev, 0x000d, 0x0000);
	usb_stk11xx_write_registry(dev, 0x000f, 0x0002);
	usb_stk11xx_write_registry(dev, 0x0300, 0x0012);
	usb_stk11xx_write_registry(dev, 0x0350, 0x0041);
	
	usb_stk11xx_write_registry(dev, 0x0351, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0352, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0353, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0018, 0x0010);
	usb_stk11xx_write_registry(dev, 0x0019, 0x0000);
	
	usb_stk11xx_write_registry(dev, 0x001b, values_001B[step]);
	usb_stk11xx_write_registry(dev, 0x001c, values_001C[step]);
	usb_stk11xx_write_registry(dev, 0x0300, 0x0080);
	usb_stk11xx_write_registry(dev, 0x001a, 0x0004);
	usb_stk11xx_write_registry(dev, 0x0202, values_0202[step]);
	
	usb_stk11xx_write_registry(dev, 0x0110, values_0110[step]);
	usb_stk11xx_write_registry(dev, 0x0111, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0112, values_0112[step]);
	usb_stk11xx_write_registry(dev, 0x0113, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0114, values_0114[step]);
	
	usb_stk11xx_write_registry(dev, 0x0115, values_0115[step]);
	usb_stk11xx_write_registry(dev, 0x0116, values_0116[step]);
	usb_stk11xx_write_registry(dev, 0x0117, values_0117[step]);

	usb_stk11xx_read_registry(dev, 0x0100, &value);
	usb_stk11xx_write_registry(dev, 0x0100, values_0100[step]);

	usb_stk11xx_write_registry(dev, 0x0200, 0x0080); 
	usb_stk11xx_write_registry(dev, 0x0200, 0x0000); 
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000); 


	switch (step) {
		case 0:
			usb_stk11xx_write_registry(dev, 0x0203, 0x0040); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0041); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0001); 
			usb_stk11xx_write_registry(dev, 0x0204, 0x001c); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0002); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;

		case 1:
			usb_stk11xx_write_registry(dev, 0x0203, 0x0022); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0027); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x00a5); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;
	
		case 2:
			usb_stk11xx_write_registry(dev, 0x0203, 0x0060); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0012); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0080); 
			usb_stk11xx_write_registry(dev, 0x0204, 0x0013); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x00bf); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;

		case 3:
			usb_stk11xx_write_registry(dev, 0x0203, 0x0042); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0012); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0080); 
			usb_stk11xx_write_registry(dev, 0x0204, 0x0024); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x00a5); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;

		case 4:
			usb_stk11xx_write_registry(dev, 0x0203, 0x0042); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0012); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0080); 
			usb_stk11xx_write_registry(dev, 0x0204, 0x0013); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x00e0); 
			usb_stk11xx_write_registry(dev, 0x0204, 0x0024); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x00a5); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;

		case 5:
			usb_stk11xx_write_registry(dev, 0x0203, 0x0060); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0012); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0080); 
			usb_stk11xx_write_registry(dev, 0x0204, 0x0013); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x00ff); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;

		case 6:
			usb_stk11xx_write_registry(dev, 0x0203, 0x0060); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0012); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0080); 
			usb_stk11xx_write_registry(dev, 0x0204, 0x0013); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x00ff); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;

		case 7:
			usb_stk11xx_write_registry(dev, 0x0203, 0x0060); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0012); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0080); 
			usb_stk11xx_write_registry(dev, 0x0204, 0x0013); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x00b7); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;

		case 8:
			usb_stk11xx_write_registry(dev, 0x0203, 0x0060); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0012); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0080); 
			usb_stk11xx_write_registry(dev, 0x0204, 0x0013); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x00b7); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;

		case 9:
			usb_stk11xx_write_registry(dev, 0x0203, 0x0080); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0012); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0080); 
			usb_stk11xx_write_registry(dev, 0x0204, 0x000a); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x00ff); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;

		case 10:
			usb_stk11xx_write_registry(dev, 0x0203, 0x00dc); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0015); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0080); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;

		case 11:
			usb_stk11xx_write_registry(dev, 0x0203, 0x00ec); 

			usb_stk11xx_write_registry(dev, 0x0204, 0x0015); 
			usb_stk11xx_write_registry(dev, 0x0205, 0x0080); 

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005); 

			break;

		case 12:
			usb_stk11xx_write_registry(dev, 0x0203, 0x00ba); 
			break;

		case 13:
			usb_stk11xx_write_registry(dev, 0x0203, 0x00ba); 
			break;

		case 14:
			usb_stk11xx_write_registry(dev, 0x0203, 0x00ba);

			usb_stk11xx_write_registry(dev, 0x0204, 0x0001);
			usb_stk11xx_write_registry(dev, 0x0205, 0x0000);

			usb_stk11xx_write_registry(dev, 0x0200, 0x0005);
			break;

		case 15:
			usb_stk11xx_write_registry(dev, 0x0203, 0x00ba);

			dev_stk6a33_sensor_settings(dev);

			usb_stk11xx_write_registry(dev, 0x0200, 0x0080);
			usb_stk11xx_write_registry(dev, 0x0200, 0x0000);
			usb_stk11xx_write_registry(dev, 0x02ff, 0x0001);
			usb_stk11xx_write_registry(dev, 0x0203, 0x00a0);

			break;

		case 16:
			usb_stk11xx_write_registry(dev, 0x0203, 0x00ba); 

			dev_stk6a33_sensor_settings(dev);

			usb_stk11xx_write_registry(dev, 0x0200, 0x0080);
			usb_stk11xx_write_registry(dev, 0x0200, 0x0000);
			usb_stk11xx_write_registry(dev, 0x02ff, 0x0001);
			usb_stk11xx_write_registry(dev, 0x0203, 0x00a0);


			break;

		case 17:
			usb_stk11xx_write_registry(dev, 0x0203, 0x00ba); 

			dev_stk6a33_sensor_settings(dev);

			break;
	}
	
	return 0;
}


/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief Wake-up the camera.
 *
 * This function permits to wake-up the device.
 */
int dev_stk6a33_camera_asleep(struct usb_stk11xx *dev)
{
	int value;

	usb_stk11xx_read_registry(dev, 0x0104, &value);
	usb_stk11xx_read_registry(dev, 0x0105, &value);
	usb_stk11xx_read_registry(dev, 0x0106, &value);

	usb_stk11xx_write_registry(dev, 0x0100, 0x0021);
	usb_stk11xx_write_registry(dev, 0x0116, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0117, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0018, 0x0000);

	usb_stk11xx_read_registry(dev, 0x0000, &value);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0049);

	return 0;
}


/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief This function initializes the device for the stream.
 *
 * It's the start. This function has to be called at first, before
 * enabling the video stream.
 */
int dev_stk6a33_init_camera(struct usb_stk11xx *dev)
{
	dev_stk6a33_camera_asleep(dev);

	usb_stk11xx_set_feature(dev, 0);

	usb_stk11xx_write_registry(dev, 0x0000, 0x00e0);
	usb_stk11xx_write_registry(dev, 0x0002, 0x00e8);
	usb_stk11xx_write_registry(dev, 0x0002, 0x0078);
	usb_stk11xx_write_registry(dev, 0x0000, 0x0020);

	dev_stk6a33_configure_device(dev, 16);

	dev_stk11xx_camera_off(dev);

	return 0;
}


/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief This function permits to set default sensor settings.
 *
 * We set some registers in using a I2C bus.
 * WARNING, the sensor settings can be different following the situation.
 */
int dev_stk6a33_sensor_settings(struct usb_stk11xx *dev)
{
	int i;
	int retok;
	int value;

	int asize;
	static const int values_204[] = {
		0xf0, 0xf1, 0x0d, 0xf1, 0x0d, 0xf1, 0xf0, 0xf1, 0x35, 0xf1, 
		0xf0, 0xf1, 0x06, 0xf1, 0xf0, 0xf1, 0xdd, 0xf1, 0xf0, 0xf1, 
		0x1f, 0xf1, 0x20, 0xf1, 0x21, 0xf1, 0x22, 0xf1, 0x23, 0xf1, 
		0x24, 0xf1, 0x28, 0xf1, 0x29, 0xf1, 0x5e, 0xf1, 0x5f, 0xf1, 
		0x60, 0xf1, 0xef, 0xf1, 0xf2, 0xf1, 0x02, 0xf1, 0x03, 0xf1, 
		0x04, 0xf1, 0x09, 0xf1, 0x0a, 0xf1, 0x0b, 0xf1, 0x0c, 0xf1, 
		0x0d, 0xf1, 0x0e, 0xf1, 0x0f, 0xf1, 0x10, 0xf1, 0x11, 0xf1, 
		0x15, 0xf1, 0x16, 0xf1, 0x17, 0xf1, 0x18, 0xf1, 0x19, 0xf1, 
		0x1a, 0xf1, 0x1b, 0xf1, 0x1c, 0xf1, 0x1d, 0xf1, 0x1e, 0xf1, 
		0xf0, 0xf1, 0x06, 0xf1, 0x06, 0xf1, 0xf0, 0xf1, 0x80, 0xf1, 
		0x81, 0xf1, 0x82, 0xf1, 0x83, 0xf1, 0x84, 0xf1, 0x85, 0xf1, 
		0x86, 0xf1, 0x87, 0xf1, 0x88, 0xf1, 0x89, 0xf1, 0x8a, 0xf1, 
		0x8b, 0xf1, 0x8c, 0xf1, 0x8d, 0xf1, 0x8e, 0xf1, 0x8f, 0xf1, 
		0x90, 0xf1, 0x91, 0xf1, 0x92, 0xf1, 0x93, 0xf1, 0x94, 0xf1, 
		0x95, 0xf1, 0xb6, 0xf1, 0xb7, 0xf1, 0xb8, 0xf1, 0xb9, 0xf1, 
		0xba, 0xf1, 0xbb, 0xf1, 0xbc, 0xf1, 0xbd, 0xf1, 0xbe, 0xf1, 
		0xbf, 0xf1, 0xc0, 0xf1, 0xc1, 0xf1, 0xc2, 0xf1, 0xc3, 0xf1, 
		0xc4, 0xf1, 0x06, 0xf1, 0xf0, 0xf1, 0x53, 0xf1, 0x54, 0xf1, 
		0x55, 0xf1, 0x56, 0xf1, 0x57, 0xf1, 0x58, 0xf1, 0xf0, 0xf1, 
		0xa7, 0xf1, 0xaa, 0xf1, 0x3a, 0xf1, 0xa1, 0xf1, 0xa4, 0xf1, 
		0x9b, 0xf1, 0x08, 0xf1, 0xf0, 0xf1, 0x2f, 0xf1, 0x9c, 0xf1, 
		0xd2, 0xf1, 0xcc, 0xf1, 0xcb, 0xf1, 0x2e, 0xf1, 0x67, 0xf1, 
		0xf0, 0xf1, 0x65, 0xf1, 0x66, 0xf1, 0x67, 0xf1, 0x65, 0xf1, 
		0xf0, 0xf1, 0x05, 0xf1, 0x07, 0xf1, 0xf0, 0xf1, 0x39, 0xf1, 
		0x3b, 0xf1, 0x3a, 0xf1, 0x3c, 0xf1, 0x57, 0xf1, 0x58, 0xf1, 
		0x59, 0xf1, 0x5a, 0xf1, 0x5c, 0xf1, 0x5d, 0xf1, 0x64, 0xf1, 
		0xf0, 0xf1, 0x5b, 0xf1, 0xf0, 0xf1, 0x36, 0xf1, 0x37, 0xf1, 
		0xf0, 0xf1, 0x08, 0xf1
	};
	static const int values_205[] = {
		0x00, 0x00, 0x00, 0x0b, 0x00, 0x08, 0x00, 0x00, 0x00, 0x22, 
		0x00, 0x01, 0x70, 0x0e, 0x00, 0x02, 0x18, 0xe0, 0x00, 0x02, 
		0x01, 0x80, 0xc8, 0x14, 0x80, 0x80, 0xa0, 0x78, 0xa0, 0x78, 
		0x5f, 0x20, 0xea, 0x02, 0x86, 0x7a, 0x59, 0x4c, 0x4d, 0x51, 
		0x00, 0x02, 0x00, 0x08, 0x00, 0x00, 0x00, 0xee, 0x39, 0x23, 
		0x07, 0x24, 0x00, 0xcd, 0x00, 0x93, 0x00, 0x04, 0x00, 0x5c, 
		0x00, 0xd9, 0x00, 0x53, 0x00, 0x08, 0x00, 0x91, 0x00, 0xcf, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x01, 0xf0, 0x0e, 0x70, 0x0e, 0x00, 0x01, 0x00, 0x0b, 
		0xee, 0x08, 0xf8, 0xf3, 0x00, 0xfb, 0xee, 0x09, 0xf7, 0xf3, 
		0xff, 0xfa, 0xf2, 0x08, 0xf9, 0xf5, 0xff, 0xf9, 0xe4, 0x10, 
		0xf1, 0xea, 0xf8, 0xf6, 0x00, 0x00, 0xe9, 0x0f, 0xf2, 0xed, 
		0xf6, 0xf6, 0x00, 0x00, 0xea, 0x0d, 0xf4, 0xf1, 0xf8, 0xf8, 
		0x00, 0x00, 0x07, 0x06, 0x1c, 0x0b, 0x08, 0x06, 0x17, 0x0a, 
		0x05, 0x06, 0x1a, 0x07, 0x06, 0x04, 0x0a, 0x08, 0x00, 0x19, 
		0x06, 0x05, 0x07, 0x0a, 0x00, 0x1f, 0x04, 0x04, 0x04, 0x08, 
		0x00, 0x1b, 0x74, 0x0e, 0x00, 0x01, 0x0b, 0x03, 0x47, 0x22, 
		0xac, 0x82, 0xda, 0xc7, 0xf5, 0xe9, 0xff, 0x00, 0x00, 0x01, 
		0x02, 0x80, 0x01, 0xe0, 0x43, 0x00, 0x05, 0x00, 0x04, 0x00, 
		0x43, 0x00, 0x01, 0x80, 0x00, 0x02, 0xd1, 0x00, 0xd1, 0x00, 
		0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x0c, 0x50, 0x10, 0x10, 
		0x00, 0x00, 0xa0, 0x00, 0x20, 0x03, 0x05, 0x01, 0x20, 0x00, 
		0x00, 0x00, 0x01, 0xb8, 0x00, 0xd8, 0x00, 0x02, 0x06, 0xc0, 
		0x04, 0x0e, 0x06, 0xc0, 0x05, 0x64, 0x02, 0x08, 0x02, 0x71, 
		0x02, 0x08, 0x02, 0x71, 0x12, 0x0d, 0x17, 0x12, 0x5e, 0x1c, 
		0x00, 0x02, 0x00, 0x03, 0x00, 0x02, 0x78, 0x10, 0x83, 0x04, 
		0x00, 0x00, 0x00, 0x21
	};
		

	asize = ARRAY_SIZE(values_204);

	for(i=0; i<asize; i++) {
		usb_stk11xx_read_registry(dev, 0x02ff, &value);
		usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);

		usb_stk11xx_write_registry(dev, 0x0203, 0x00ba);

		usb_stk11xx_write_registry(dev, 0x0204, values_204[i]);
		usb_stk11xx_write_registry(dev, 0x0205, values_205[i]);
		usb_stk11xx_write_registry(dev, 0x0200, 0x0001);

		retok = dev_stk11xx_check_device(dev, 500);

		if (retok != 1) {
			STK_ERROR("Load default sensor settings fail !\n");
			return -1;
		}

		usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);
	}

	retok = dev_stk11xx_check_device(dev, 500);

	return 0;
}


/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief This function permits to modify the settings of the camera.
 *
 * This functions permits to modify the settings :
 *   - brightness
 *   - contrast
 *   - white balance
 *   - ...
 */
int dev_stk6a33_camera_settings(struct usb_stk11xx *dev)
{
	int i;
	int value;

	int asize;
	static const int values_204[] = {
		0xf0, 0xf1, 0x2e, 0xf1, 0xf0, 0xf1, 0x5b, 0xf1, 0xf0, 0xf1, 0x36, 0xf1, 0x37, 0xf1, 0xf0, 0xf1, 0x08, 0xf1
	};
	static const int values_205[] = {
		0x00, 0x02, 0x0c, 0x3c, 0x00, 0x02, 0x00, 0x03, 0x00, 0x02, 0x78, 0x10, 0x83, 0x04, 0x00, 0x00, 0x00, 0x21
	};


	asize = ARRAY_SIZE(values_204);

	// Contrast register
	for (i=0; i<asize; i++) {
		usb_stk11xx_read_registry(dev, 0x02ff, &value);
		usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);

		usb_stk11xx_write_registry(dev, 0x0204, values_204[i]);
		usb_stk11xx_write_registry(dev, 0x0205, values_205[i]);

		usb_stk11xx_write_registry(dev, 0x0200, 0x0001);
		dev_stk11xx_check_device(dev, 500);
		usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);
	}

	return 0;
}


/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief This function permits to modify the settings of the camera.
 *
 * This functions permits to modify the settings :
 *   - brightness
 *   - contrast
 *   - white balance
 *   - ...
 *
 * 0x204 = 0xa1 : unkown (by default 0x00)
 * 0x204 = 0x10 : contrast (by default 0x7c)
 * 0x204 = 0x04 : Mode (unknown) (by default 0x00) (=> already looked 0x01 and 0x02)
 * 0x204 = 0x00 : brightness / white balance (by default 0x00)
 */
int dev_stk6a33_set_camera_quality(struct usb_stk11xx *dev)
{
	int value;

	// Contrast register
	usb_stk11xx_read_registry(dev, 0x02ff, &value);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);

	usb_stk11xx_write_registry(dev, 0x0204, 0x00b3);
	usb_stk11xx_write_registry(dev, 0x0205, (dev->vsettings.contrast >> 8));

	usb_stk11xx_write_registry(dev, 0x0200, 0x0001);
	dev_stk11xx_check_device(dev, 500);
	usb_stk11xx_write_registry(dev, 0x02ff, 0x0000);

	STK_DEBUG("Set colour : %d\n", dev->vsettings.colour);
	STK_DEBUG("Set contrast : %d\n", dev->vsettings.contrast);
	STK_DEBUG("Set whiteness : %d\n", dev->vsettings.whiteness);
	STK_DEBUG("Set brightness : %d\n", dev->vsettings.brightness);

	return 0;
}


/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief This function permits to modify the settings of the camera.
 *
 * This functions permits to modify the frame rate per second.
 *
 * 0x204 = 0x2e : Fps MSB (by default 0x01)
 * 0x204 = 0x2d : Fps LSB (by default 0x00)
 *
 * 0x2e | 0x2d | Nbr fps
 * -----+------+--------
 * 0x00 | 0x00 |  30
 * 0x01 | 0x00 |  20
 * 0x02 | 0x00 |  15
 * 0x03 | 0x00 |  12
 * 0x04 | 0x00 |  10
 */
int dev_stk6a33_set_camera_fps(struct usb_stk11xx *dev)
{
	return 0;
}


/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief This function sets the device to start the stream.
 *
 * After the initialization of the device and the initialization of the video stream,
 * this function permits to enable the stream.
 */
int dev_stk6a33_start_stream(struct usb_stk11xx *dev)
{
	int value;
	int value_116, value_117;

	usb_stk11xx_read_registry(dev, 0x0114, &value); // read 0x80
	usb_stk11xx_read_registry(dev, 0x0115, &value); // read 0x02

	usb_stk11xx_read_registry(dev, 0x0116, &value_116);
	usb_stk11xx_read_registry(dev, 0x0117, &value_117);

	usb_stk11xx_write_registry(dev, 0x0116, 0x0000);
	usb_stk11xx_write_registry(dev, 0x0117, 0x0000);

	usb_stk11xx_read_registry(dev, 0x0100, &value); // read 0x21
	usb_stk11xx_write_registry(dev, 0x0100, 0x00a0);

	usb_stk11xx_write_registry(dev, 0x0116, value_116);
	usb_stk11xx_write_registry(dev, 0x0117, value_117);

	return 0;
}


/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief Reconfigure the camera before the stream.
 *
 * Before enabling the video stream, you have to reconfigure the device.
 */
int dev_stk6a33_reconf_camera(struct usb_stk11xx *dev)
{
	dev_stk6a33_configure_device(dev, 16);

	dev_stk11xx_camera_settings(dev);

	return 0;
}


/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief This function sets the device to stop the stream.
 *
 * You use the function start_stream to enable the video stream. So you
 * have to use the function stop_strem to disable the video stream.
 */
int dev_stk6a33_stop_stream(struct usb_stk11xx *dev)
{
	int value;

	usb_stk11xx_read_registry(dev, 0x0100, &value);
	usb_stk11xx_write_registry(dev, 0x0100, 0x0021);

	return 0;
}

