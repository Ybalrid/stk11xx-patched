/** 
 * @file stk11xx-usb.c
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
 *   $Date: 2012-03-10 09:54:57 +0100 (sam. 10 mars 2012) $
 *   $Revision: 105 $
 *   $Author: nicklas79 $
 *   $HeadURL: https://svn.code.sf.net/p/syntekdriver/code/trunk/driver/stk11xx-usb.c $
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/kref.h>
#include <linux/mm.h>

#include <linux/usb.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>

#include "stk11xx.h"


/** 
 * @var default_fps
 *   Number of frame per second by default
 */
static int default_fps = -1;

/**
 * @var default_hflip
 *   Enable / Disable horizontal flip image
 */
static int default_hflip = -1;

/**
 * @var default_vflip
 *   Enable / Disable vertical flip image
 */
static int default_vflip = -1;

/**
 * @var default_brightness
 *   Set brightness
 */
static int default_brightness = -1;

/**
 * @var default_whiteness
 *   Set whiteness
 */
static int default_whiteness = -1;

/**
 * @var default_contrast
 *   Set contrast
 */
static int default_contrast = -1;

/**
 * @var default_colour
 *   Set colour
 */
static int default_colour = -1;

/**
 * @var default_norm
 *   Set norm
 */
static int default_norm = -1;

 
/**
 * @var stk11xx_table
 * Define all the hotplug supported devices by this driver
 */
static struct usb_device_id stk11xx_table[] = {
	{ USB_DEVICE(USB_SYNTEK1_VENDOR_ID, USB_STK_A311_PRODUCT_ID) },
	{ USB_DEVICE(USB_SYNTEK1_VENDOR_ID, USB_STK_A821_PRODUCT_ID) },
	{ USB_DEVICE(USB_SYNTEK1_VENDOR_ID, USB_STK_AA11_PRODUCT_ID) },
	{ USB_DEVICE(USB_SYNTEK1_VENDOR_ID, USB_STK_6A31_PRODUCT_ID) },
	{ USB_DEVICE(USB_SYNTEK1_VENDOR_ID, USB_STK_6A33_PRODUCT_ID) },
	{ USB_DEVICE(USB_SYNTEK1_VENDOR_ID, USB_STK_6A51_PRODUCT_ID) },
	{ USB_DEVICE(USB_SYNTEK1_VENDOR_ID, USB_STK_6A54_PRODUCT_ID) },
	{ USB_DEVICE(USB_SYNTEK1_VENDOR_ID, USB_STK_6D51_PRODUCT_ID) },

	{ USB_DEVICE(USB_SYNTEK2_VENDOR_ID, USB_STK_0408_PRODUCT_ID) },
	{ USB_DEVICE(USB_SYNTEK2_VENDOR_ID, USB_STK_0500_PRODUCT_ID) },
	{ USB_DEVICE(USB_SYNTEK2_VENDOR_ID, USB_STK_0501_PRODUCT_ID) },
	{ }
};


MODULE_DEVICE_TABLE(usb, stk11xx_table);		/**< Define the supported devices */



/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief Initilize an isochronous pipe.
 *
 * This function permits to initialize an URB transfert (or isochronous pipe).
 */
int usb_stk11xx_isoc_init(struct usb_stk11xx *dev)
{
	int i, j;
	int ret = 0;
	struct urb *urb;
	struct usb_device *udev;

	if (dev == NULL)
		return -EFAULT;

	if (dev->isoc_init_ok)
		return 0;

	udev = dev->udev;

	STK_DEBUG("usb_stk11xx_isoc_init()\n");

	// Allocate URB structure
	for (i=0; i<MAX_ISO_BUFS; i++) {
		urb = usb_alloc_urb(ISO_FRAMES_PER_DESC, GFP_KERNEL);

		if (urb == NULL) {
			STK_ERROR("Failed to allocate URB %d\n", i);
			ret = -ENOMEM;
			break;
		}

		dev->isobuf[i].urb = urb;
	}

	if (ret) {
		while (i >= 0) {
			if (dev->isobuf[i].urb != NULL)
				usb_free_urb(dev->isobuf[i].urb);

			dev->isobuf[i].urb = NULL;
			i--;
		}

		return ret;
	}

	// Init URB structure
	for (i=0; i<MAX_ISO_BUFS; i++) {
		urb = dev->isobuf[i].urb;

		urb->interval = 1; 
		urb->dev = udev;
		urb->pipe = usb_rcvisocpipe(udev, dev->isoc_in_endpointAddr);
		urb->transfer_flags = URB_ISO_ASAP;
		urb->transfer_buffer = dev->isobuf[i].data;
		urb->transfer_buffer_length = ISO_BUFFER_SIZE;
		urb->complete = usb_stk11xx_isoc_handler;
		urb->context = dev;
		urb->start_frame = 0;
		urb->number_of_packets = ISO_FRAMES_PER_DESC;

		for (j=0; j<ISO_FRAMES_PER_DESC; j++) {
			urb->iso_frame_desc[j].offset = j * ISO_MAX_FRAME_SIZE;
			urb->iso_frame_desc[j].length = ISO_MAX_FRAME_SIZE; //dev->isoc_in_size;
		}
	}

	STK_DEBUG("dev->isoc_in_size = %X\n", dev->isoc_in_size);
	STK_DEBUG("dev->isoc_in_endpointAddr = %X\n", dev->isoc_in_endpointAddr);

	// Link
	for (i=0; i<MAX_ISO_BUFS; i++) {
		ret = usb_submit_urb(dev->isobuf[i].urb, GFP_KERNEL);

		if (ret)
			STK_ERROR("isoc_init() submit_urb %d failed with error %d\n", i, ret);
		else
			STK_DEBUG("URB 0x%p submitted.\n", dev->isobuf[i].urb);

		switch (ret) {
			case -ENOMEM:
				STK_ERROR("ENOMEM\n");
				break;
			case -ENODEV:
				STK_ERROR("ENODEV\n");
				break;
			case -ENXIO:
				STK_ERROR("ENXIO\n");
				break;
			case -EINVAL:
				STK_ERROR("EINVAL\n");
				break;
			case -EAGAIN:
				STK_ERROR("EAGAIN\n");
				break;
			case -EFBIG:
				STK_ERROR("EFBIG\n");
				break;
			case -EPIPE:
				STK_ERROR("EPIPE\n");
				break;
			case -EMSGSIZE:
				STK_ERROR("EMSGSIZE\n");
				break;
		}
	}

	// All is done
	dev->isoc_init_ok = 1;

	return 0;
}


/** 
 * @param urb URB structure
 *
 * @brief ISOC handler
 *
 * This function is called as an URB transfert is complete (Isochronous pipe).
 * So, the traitement is done in interrupt time, so it has be fast, not crash,
 * ans not stall. Neat.
 */
void usb_stk11xx_isoc_handler(struct urb *urb)
{
	int i;
	int ret;
	int skip;

	int awake = 0;
	int framestatus;
	int framelen;

	unsigned char *fill = NULL;
	unsigned char *iso_buf = NULL;

	struct usb_stk11xx *dev;
	struct stk11xx_frame_buf *framebuf;

	STK_STREAM("Isoc handler\n");

	dev = (struct usb_stk11xx *) urb->context;

	if (dev == NULL) {
		STK_ERROR("isoc_handler called with NULL device !\n");
		return;
	}

	if (urb->status == -ENOENT || urb->status == -ECONNRESET) {
		STK_DEBUG("URB unlinked synchronuously !\n");
		return;
	}

	if (urb->status != -EINPROGRESS && urb->status != 0) {
		const char *errmsg;

		errmsg = "Unknown";

		switch(urb->status) {
			case -ENOSR:
				errmsg = "Buffer error (overrun)";
				break;

			case -EPIPE:
				errmsg = "Stalled (device not responding)";
				break;

			case -EOVERFLOW:
				errmsg = "Babble (bad cable?)";
				break;

			case -EPROTO:
				errmsg = "Bit-stuff error (bad cable?)";
				break;

			case -EILSEQ:
				errmsg = "CRC/Timeout (could be anything)";
				break;

			case -ETIMEDOUT:
				errmsg = "NAK (device does not respond)";
				break;
		}

		STK_ERROR("isoc_handler() called with status %d [%s].\n", urb->status, errmsg);

		dev->visoc_errors++;

		wake_up_interruptible(&dev->wait_frame);

		urb->dev = dev->udev;
		ret = usb_submit_urb(urb, GFP_ATOMIC);

		if (ret != 0) {
			STK_ERROR("Error (%d) re-submitting urb in stk11xx_isoc_handler.\n", ret);
		}

		return;
	}

	framebuf = dev->fill_frame;

	if (framebuf == NULL) {
		STK_ERROR("isoc_handler without valid fill frame !\n");
		
		wake_up_interruptible(&dev->wait_frame);

		urb->dev = dev->udev;
		ret = usb_submit_urb(urb, GFP_ATOMIC);

		if (ret != 0) {
			STK_ERROR("Error (%d) re-submitting urb in stk11xx_isoc_handler.\n", ret);
		}

		return;
	}
	else {
		fill = framebuf->data + framebuf->filled;
	}

	// Reset ISOC error counter
	dev->visoc_errors = 0;

	// Compact data
	for (i=0; i<urb->number_of_packets; i++) {
		framestatus = urb->iso_frame_desc[i].status;
		framelen = urb->iso_frame_desc[i].actual_length;
		iso_buf = urb->transfer_buffer + urb->iso_frame_desc[i].offset;

		if (framestatus == 0) {
			skip = 4;

			if (framelen > 4) {
				// we found something informational from there
				// the isoc frames have to type of headers
				// type1: 00 xx 00 00 or 20 xx 00 00
				// type2: 80 xx 00 00 00 00 00 00 or a0 xx 00 00 00 00 00 00
				// xx is a sequencer which has never been seen over 0x3f
				//
				// imho data written down looks like bayer, i see similarities after
				// every 640 bytes
				if (*iso_buf & 0x80) {
					skip = 8;
				}

				// Determine if odd or even frame, and set a flag
				if (framelen == 8) {
					if (*iso_buf & 0x40)
						framebuf->odd = true;
					else
						framebuf->odd = false;
				}
				
				// Our buffer is full !!!
				if (framelen - skip + framebuf->filled > dev->frame_size) {
					STK_ERROR("Frame buffer overflow %d %d %d!\n",
							framelen, framelen-skip+framebuf->filled, dev->frame_size);
					framebuf->errors++;
				}
				// All is OK
				else {
					memcpy(fill, iso_buf + skip, framelen - skip);
					fill += framelen - skip;
				}

				// New size of our buffer
				framebuf->filled += framelen - skip;
			}

			STK_STREAM("URB : Length = %d - Skip = %d - Buffer size = %d\n",
				framelen, skip, framebuf->filled);

			// Data is always follow by a frame with a length '4'
			if (framelen == 4) {
				if (framebuf->filled > 0) {
					// Our buffer has enough data ?
					if (framebuf->filled < dev->frame_size)
						framebuf->errors++;

					// If there are errors, we skip a frame...
					if (framebuf->errors == 0) {
						if (stk11xx_next_frame(dev))
							dev->vframes_dumped++;
					}
					else
						dev->vframes_error++;

					awake = 1;
					framebuf = dev->fill_frame;
					framebuf->filled = 0;
					framebuf->errors = 0;
					fill = framebuf->data;
				}
			}
		}
		else {
			STK_ERROR("Iso frame %d of USB has error %d\n", i, framestatus);
		}
	}

	if (awake == 1)
		wake_up_interruptible(&dev->wait_frame);

	urb->dev = dev->udev;

	ret = usb_submit_urb(urb, GFP_ATOMIC);

	if (ret != 0) {
		STK_ERROR("Error (%d) re-submitting urb in stk11xx_isoc_handler.\n", ret);
	}
}


/** 
 * @param dev Device structure
 *
 * @brief Clean-up all the ISOC buffers
 *
 * This function permits to clean-up all the ISOC buffers.
 */
void usb_stk11xx_isoc_cleanup(struct usb_stk11xx *dev)
{
	int i;

	STK_DEBUG("Isoc cleanup\n");

	if (dev == NULL)
		return;

	if (dev->isoc_init_ok == 0)
		return;

	// Unlinking ISOC buffers
	for (i=0; i<MAX_ISO_BUFS; i++) {
		struct urb *urb;

		urb = dev->isobuf[i].urb;

		if (urb != 0) {
			if (dev->isoc_init_ok)
				usb_kill_urb(urb);
			
			usb_free_urb(urb);
			dev->isobuf[i].urb = NULL;
		}
	}

	// All is done
	dev->isoc_init_ok = 0;
}



/** 
 * @param dev Device structure
 * @param index Choice of the interface
 * 
 * @returns 0 if all is OK
 *
 * @brief Send the message SET_FEATURE and choose the interface
 *
 * This function permits to send the message SET_FEATURE on the USB bus.
 */
int usb_stk11xx_set_feature(struct usb_stk11xx *dev, int index)
{
	int result;
	struct usb_device *udev = dev->udev;

	result = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
			USB_REQ_SET_FEATURE,
			USB_TYPE_STANDARD | USB_DIR_OUT | USB_RECIP_DEVICE,
			USB_DEVICE_REMOTE_WAKEUP,
			index,
			NULL,
			0,
			500);
	
	if (result < 0)
		STK_ERROR("SET FEATURE fail !\n");
	else 
		STK_DEBUG("SET FEATURE\n");

	return result;
}


/** 
 * @param dev Device structure
 * 
 * @returns 0 if all is OK
 *
 * @brief Send the message SET_CONFIGURATION
 *
 * This function permits to send the message SET_CONFIGURATION on the USB bus.
 */
int usb_stk11xx_set_configuration(struct usb_stk11xx *dev)
{
	int result;
	struct usb_device *udev = dev->udev;

	result = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
			USB_REQ_SET_CONFIGURATION,
			USB_TYPE_STANDARD | USB_DIR_OUT | USB_RECIP_DEVICE,
			0,
			udev->config[0].desc.bConfigurationValue,
			NULL,
			0,
			500);

	if (result < 0)
		STK_ERROR("SET CONFIGURATION fail !\n");
	else 
		STK_DEBUG("SET CONFIGURATION %d\n", udev->config[0].desc.bConfigurationValue);

	return result;
}


/** 
 * @param dev 
 * @param index 
 * @param value 
 * 
 * @returns 0 if all is OK
 *
 * @brief Write a 16-bits value to a 16-bits register
 *
 * This function permits to write a 16-bits value to a 16-bits register on the USB bus.
 */
int usb_stk11xx_write_registry(struct usb_stk11xx *dev, __u16 index, __u16 value)
{
	int result;
	struct usb_device *udev = dev->udev;

	result = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
			0x01,
			USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			value,
			index,
			NULL,
			0,
			500);

	if (result < 0)
		STK_ERROR("Write registry fails %02X = %02X", index, value);

	return result;
}


/** 
 * @param dev 
 * @param index 
 * @param value
 * 
 * @returns 0 if all is OK
 *
 * @brief Read a 16-bits value from a 16-bits register
 *
 * This function permits to read a 16-bits value from a 16-bits register on the USB bus.
 */
int usb_stk11xx_read_registry(struct usb_stk11xx *dev, __u16 index, int *value)
{
	int result;

	struct usb_device *udev = dev->udev;

	*value = 0;

	result = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
			0x00,
			USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			0x00,
			index,
			(__u8 *) value,
			sizeof(__u8),
			500);

	if (result < 0)
		STK_ERROR("Read registry fails %02X", index);

	return result;
}


/** 
 * @param dev 
 * 
 * @returns 0 if all is OK
 *
 * @brief Set the default value about the video settings.
 *
 * This function permits to set the video settings for each video camera model.
 * 
 */
static int usb_stk11xx_default_settings(struct usb_stk11xx *dev)
{
	switch (dev->webcam_model) {
		case SYNTEK_STK_0408:
			dev->vsettings.fps = (default_fps == -1) ? 25 : default_fps;
			dev->vsettings.vflip = (default_vflip == -1) ? 0 : default_vflip;
			dev->vsettings.hflip = (default_hflip == -1) ? 0 : default_hflip;

			dev->vsettings.brightness = (default_brightness == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_brightness;
			dev->vsettings.whiteness = (default_whiteness == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_whiteness;
			dev->vsettings.contrast = (default_contrast == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_contrast;
			dev->vsettings.colour = (default_colour == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_colour;
			dev->vsettings.norm = (default_norm == -1) ? 0 : default_norm;
			dev->vsettings.input = 0;
			break;

		case SYNTEK_STK_M811:
		case SYNTEK_STK_A311:
			dev->vsettings.fps = (default_fps == -1) ? 25 : default_fps;
			dev->vsettings.vflip = (default_vflip == -1) ? 1 : default_vflip;
			dev->vsettings.hflip = (default_hflip == -1) ? 1 : default_hflip;

			dev->vsettings.brightness = (default_brightness == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_brightness;
			dev->vsettings.whiteness = (default_whiteness == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_whiteness;
			dev->vsettings.contrast = (default_contrast == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_contrast;
			dev->vsettings.colour = (default_colour == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_colour;
			break;

		case SYNTEK_STK_A821:
		case SYNTEK_STK_AA11:
			dev->vsettings.fps = (default_fps == -1) ? 25 : default_fps;
			dev->vsettings.vflip = (default_vflip == -1) ? 0 : default_vflip;
			dev->vsettings.hflip = (default_hflip == -1) ? 0 : default_hflip;

			dev->vsettings.brightness = (default_brightness == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_brightness;
			dev->vsettings.whiteness = (default_whiteness == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_whiteness;
			dev->vsettings.contrast = (default_contrast == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_contrast;
			dev->vsettings.colour = (default_colour == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_colour;
			break;

		case SYNTEK_STK_6A31:
		case SYNTEK_STK_6A33:
		case SYNTEK_STK_0500:
			dev->vsettings.fps = (default_fps == -1) ? 25 : default_fps;
			dev->vsettings.vflip = (default_vflip == -1) ? 0 : default_vflip;
			dev->vsettings.hflip = (default_hflip == -1) ? 0 : default_hflip;

			dev->vsettings.brightness = (default_brightness == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_brightness;
			dev->vsettings.whiteness = (default_whiteness == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_whiteness;
			dev->vsettings.contrast = (default_contrast == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_contrast;
			dev->vsettings.colour = (default_colour == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_colour;
			break;

		case SYNTEK_STK_6A51:
		case SYNTEK_STK_6D51:
		case SYNTEK_STK_6A54:
			dev->vsettings.fps = (default_fps == -1) ? 25 : default_fps;
			dev->vsettings.vflip = (default_vflip == -1) ? 0 : default_vflip;
			dev->vsettings.hflip = (default_hflip == -1) ? 0 : default_hflip;

			dev->vsettings.brightness = (default_brightness == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_brightness;
			dev->vsettings.whiteness = (default_whiteness == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_whiteness;
			dev->vsettings.contrast = (default_contrast == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_contrast;
			dev->vsettings.colour = (default_colour == -1) ? STK11XX_PERCENT(50, 0xFFFF) : default_colour;
			break;

		default:
			return -1;
	}

	dev->vsettings.default_brightness = dev->vsettings.brightness;
	dev->vsettings.default_whiteness = dev->vsettings.whiteness;
	dev->vsettings.default_contrast = dev->vsettings.contrast;
	dev->vsettings.default_colour = dev->vsettings.colour;
	dev->vsettings.default_hflip = dev->vsettings.hflip;
	dev->vsettings.default_vflip = dev->vsettings.vflip;

	return 0;
}


/** 
 * @param interface 
 * @param id 
 * 
 * @returns 0 if all is OK
 *
 * @brief Load the driver
 *
 * This function detects the device and allocate the buffers for the device
 * and the video interface.
 */
static int usb_stk11xx_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	int i;
	int err;
	size_t buffer_size;

	int vendor_id;
	int product_id;
	int bNumInterfaces;
	int webcam_model;
	int webcam_type;

	struct usb_stk11xx *dev = NULL;
	struct usb_device *udev = interface_to_usbdev(interface);
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;


	// Get USB VendorID and ProductID
	vendor_id = le16_to_cpu(udev->descriptor.idVendor);
	product_id = le16_to_cpu(udev->descriptor.idProduct);

	// Check if we can handle this device
	STK_DEBUG("Probe function called with VendorID=%04X, ProductID=%04X and InterfaceNumber=%d\n",
			vendor_id, product_id, interface->cur_altsetting->desc.bInterfaceNumber);

	// The interface are probed one by one.
	// We are interested in the video interface (always the interface '0')
	// The interfaces '1' or '2' (if presents) are the audio control.
	if (interface->cur_altsetting->desc.bInterfaceNumber > 0)
		return -ENODEV;

	// Detect device
	if (vendor_id == USB_SYNTEK1_VENDOR_ID) {
		switch (product_id) {
			case USB_STK_A311_PRODUCT_ID:
				STK_INFO("Syntek USB2.0 - STK-1135 based webcam found.\n");
				STK_INFO("Syntek AVStream USB2.0 1.3M WebCam - Product ID 0xA311.\n");
				webcam_model = SYNTEK_STK_A311;
				webcam_type = STK11XX_SXGA;
				break;

			case USB_STK_A821_PRODUCT_ID:
				STK_INFO("Syntek USB2.0 - STK-1135 based webcam found.\n");
				STK_INFO("Syntek AVStream USB2.0 VGA WebCam - Product ID 0xA821.\n");
				webcam_model = SYNTEK_STK_A821;
				webcam_type = STK11XX_VGA;
				break;

			case USB_STK_AA11_PRODUCT_ID:
				STK_INFO("Syntek AVStream USB2.0 VGA WebCam - Product ID 0xAA11.\n");
				STK_INFO("Using code for Product ID AxA821\n");
				webcam_model = SYNTEK_STK_AA11;
				webcam_type = STK11XX_VGA;
				break;

			case USB_STK_6A31_PRODUCT_ID:
				STK_INFO("Syntek USB2.0 - STK-1135 based webcam found.\n");
				STK_INFO("Syntek AVStream USB2.0 1.3M WebCam - Product ID 0x6A31.\n");
				webcam_model = SYNTEK_STK_6A31;
				webcam_type = STK11XX_VGA;
				break;

			case USB_STK_6A33_PRODUCT_ID:
				STK_INFO("Syntek USB2.0 - STK-1135 based webcam found.\n");
				STK_INFO("Syntek AVStream USB2.0 1.3M WebCam - Product ID 0x6A33.\n");
				webcam_model = SYNTEK_STK_6A33;
				webcam_type = STK11XX_VGA;
				break;

			case USB_STK_6A51_PRODUCT_ID:
				STK_INFO("Syntek USB2.0 - STK-1135 based webcam found.\n");
				STK_INFO("Syntek AVStream USB2.0 1.3M WebCam - Product ID 0x6A51.\n");
				webcam_model = SYNTEK_STK_6A51;
				webcam_type = STK11XX_VGA;
				break;

			case USB_STK_6A54_PRODUCT_ID:
				STK_INFO("Syntek USB2.0 - STK-1135 based webcam found.\n");
				STK_INFO("Syntek AVStream USB2.0 1.3M WebCam - Product ID 0x6A54.\n");
				webcam_model = SYNTEK_STK_6A54;
				webcam_type = STK11XX_VGA;
				break;

			case USB_STK_6D51_PRODUCT_ID:
				STK_INFO("Syntek USB2.0 - STK-1135 based webcam found.\n");
				STK_INFO("Syntek AVStream USB2.0 1.3M WebCam - Product ID 0x6D51.\n");
				webcam_model = SYNTEK_STK_6D51;
				webcam_type = STK11XX_VGA;
				break;

			default:
				STK_ERROR("usb_stk11xx_probe failed ! Camera product 0x%04X is not supported.\n",
						le16_to_cpu(udev->descriptor.idProduct));
				return -ENODEV;
		}
	}
	else if (vendor_id == USB_SYNTEK2_VENDOR_ID) {
		switch (product_id) {
			case USB_STK_0408_PRODUCT_ID:
				STK_INFO("Syntek USB2.0 - STK-1160 based device found.\n");
				STK_INFO("Syntek AVStream USB2.0 Video Capture - Product ID 0x0408.\n");
				webcam_model = SYNTEK_STK_0408;
				webcam_type = STK11XX_PAL;
				break;

			case USB_STK_0500_PRODUCT_ID:
				STK_INFO("Syntek USB2.0 - STK-1135 based webcam found.\n");
				STK_INFO("Syntek AVStream USB2.0 1.3M WebCam - Product ID 0x0500.\n");
				webcam_model = SYNTEK_STK_0500;
				webcam_type = STK11XX_VGA;
				break;

			case USB_STK_0501_PRODUCT_ID:
				STK_INFO("Syntek USB2.0 - STK-1135 based webcam found.\n");
				STK_INFO("Syntek AVStream USB2.0 1.3M WebCam - Product ID 0x0501.\n");
				webcam_model = SYNTEK_STK_M811;
				webcam_type = STK11XX_SXGA;
				break;

			default:
				STK_ERROR("usb_stk11xx_probe failed ! Camera product 0x%04X is not supported.\n",
						le16_to_cpu(udev->descriptor.idProduct));
				return -ENODEV;
		}
	}
	else
		return -ENODEV;

	// Allocate structure, initialize pointers, mutexes, etc. and link it to the usb_device
	dev = kzalloc(sizeof(struct usb_stk11xx), GFP_KERNEL);

	if (dev == NULL) {
		STK_ERROR("Out of memory !\n");
		return -ENOMEM;
	}

	// Init mutexes, spinlock, etc.

#ifndef init_MUTEX
	sema_init(&dev->mutex,1);
#else
	init_MUTEX(&dev->mutex);
#endif
	mutex_init(&dev->modlock);
	spin_lock_init(&dev->spinlock);
	init_waitqueue_head(&dev->wait_frame);

	// Save pointers
	dev->webcam_model = webcam_model;
	dev->webcam_type = webcam_type;
	dev->udev = udev;
	dev->interface = interface;

	// Read the product release 
	dev->release = le16_to_cpu(udev->descriptor.bcdDevice);
	STK_INFO("Release: %04x\n", dev->release);

	// How many interfaces (1 or 3) ?
	bNumInterfaces = udev->config->desc.bNumInterfaces;
	STK_INFO("Number of interfaces : %d\n", bNumInterfaces);


	// Constructor
	// 2 : enough for webcam
	// 3 : for easycap... it's usefull ?
	dev->nbuffers = 2;
//	dev->nbuffers = 3;
	dev->len_per_image = PAGE_ALIGN(STK11XX_FRAME_SIZE);


	// Switch on the camera (to detect size of buffers)
	dev_stk11xx_camera_on(dev);


	// Set up the endpoint information 
	// use only the first int-in and isoc-in endpoints
	// for the current alternate setting
	iface_desc = interface->cur_altsetting;

	for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
		endpoint = &iface_desc->endpoint[i].desc;

		if (!dev->int_in_endpointAddr
				&& ((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN)
				&& ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)) {
			// we found an interrupt in endpoint
			buffer_size = le16_to_cpu(endpoint->wMaxPacketSize);

			dev->int_in_size = buffer_size;
			dev->int_in_endpointAddr = (endpoint->bEndpointAddress & 0xf);
		}

		if (!dev->isoc_in_endpointAddr
				&& ((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN)
				&& ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_ISOC)) {
			// we found an isoc in endpoint
			buffer_size = le16_to_cpu(endpoint->wMaxPacketSize);

			dev->isoc_in_size = buffer_size;
			dev->isoc_in_endpointAddr = (endpoint->bEndpointAddress & 0xf);
		}
	}

	if (!(dev->int_in_endpointAddr && dev->isoc_in_endpointAddr)) {
		STK_ERROR("Could not find both int-in and isoc-in endpoints");

		kfree(dev);

		return -ENODEV;
	}


	// Switch off camera
	dev_stk11xx_camera_off(dev);

	// Initialize the video device
	dev->vdev = video_device_alloc();

	if (!dev->vdev) {
		kfree(dev);
		return -ENOMEM;
	}

	// Initialize the camera
	dev_stk11xx_initialize_device(dev);
	
	// Register the video device
	err = v4l_stk11xx_register_video_device(dev);

	if (err) {
		kfree(dev);
		return err;
	}

	// Create the entries in the sys filesystem
	stk11xx_create_sysfs_files(dev->vdev);

	// Save our data pointer in this interface device
	usb_set_intfdata(interface, dev);

	// Default settings video device
	usb_stk11xx_default_settings(dev);
	
	// Enable power management feature
//	usb_autopm_enable(dev->interface);

	return 0;
}


/** 
 * @param interface 
 *
 * @brief This function is called when the device is disconnected
 *   or when the kernel module is unloaded.
 */
static void usb_stk11xx_disconnect(struct usb_interface *interface)
{
	struct usb_stk11xx *dev = usb_get_intfdata(interface);

	STK_INFO("Syntek USB2.0 Camera disconnected\n");

	// We got unplugged; this is signalled by an EPIPE error code
	if (dev->vopen) {
		STK_INFO("Disconnected while webcam is in use !\n");
		dev->error_status = EPIPE;
	}

	// Alert waiting processes
	wake_up_interruptible(&dev->wait_frame);

	// Wait until device is closed
	while (dev->vopen)
		schedule();

	// Remove the entries in the sys filesystem
	stk11xx_remove_sysfs_files(dev->vdev);

	// Unregister the video device
	v4l_stk11xx_unregister_video_device(dev);
	
	usb_set_intfdata(interface, NULL);

	kfree(dev);
}

/*
int usb_stk11xx_suspend(struct usb_interface *interface, pm_message_t message)
{
	struct usb_stk11xx *dev = usb_get_intfdata(interface);

	STK_INFO("Syntek USB2.0 Camera Suspend\n");

	// Stop the video stream
	dev_stk11xx_stop_stream(dev);
			
	// ISOC and URB cleanup
	usb_stk11xx_isoc_cleanup(dev);

	// Free memory
//	stk11xx_free_buffers(dev);

	// Switch off the camera
	dev_stk11xx_camera_off(dev);

	return 0;
}


int usb_stk11xx_resume(struct usb_interface *interface)
{
	struct usb_stk11xx *dev = usb_get_intfdata(interface);

	STK_INFO("Syntek USB2.0 Camera Resume\n");

	// Initialize the camera
	dev_stk11xx_initialize_device(dev);

	// Select the new video mode
	v4l_stk11xx_select_video_mode(dev, dev->view.x, dev->view.y);

	// Clear the buffers
	stk11xx_clear_buffers(dev);

	// Initialize the device
	dev_stk11xx_init_camera(dev);
	dev_stk11xx_camera_on(dev);
	dev_stk11xx_reconf_camera(dev);

	// ISOC and URB init
	usb_stk11xx_isoc_init(dev);

	// Start the video stream
	dev_stk11xx_start_stream(dev);

	// Video settings
	dev_stk11xx_camera_settings(dev);

	return 0;
}
*/


/**
 * @var usb_stk11xx_driver
 *
 * This variable contains some callback
 */
static struct usb_driver usb_stk11xx_driver = {
	.name = "usb_stk11xx_driver",
	.probe = usb_stk11xx_probe,
	.disconnect = usb_stk11xx_disconnect,
	.id_table = stk11xx_table,
//	.suspend = usb_stk11xx_suspend,
//	.resume = usb_stk11xx_resume,
};


/**
 * @var fps
 *   Module parameter to set frame per second
 */
static int fps;

/**
 * @var hflip
 *  Module parameter to enable/disable the horizontal flip process
 */
static int hflip = -1;

/**
 * @var vflip
 *   Module parameter to enable/disable the vertical flip process
 */
static int vflip = -1;

/**
 * @var brightness
 *   Module parameter to set the brightness
 */
static int brightness = -1;

/**
 * @var whiteness
 *   Module parameter to set the whiteness
 */
static int whiteness = -1;

/**
 * @var contrast
 *   Module parameter to set the contrast
 */
static int contrast = -1;

/**
 * @var colour
 *   Module parameter to set the colour
 */
static int colour = -1;

/**
 * @var norm
 *   Module parameter to set the norm
 */
static int norm = -1;


module_param(fps, int, 0444);			/**< @brief Module frame per second parameter */
module_param(hflip, int, 0444);			/**< @brief Module horizontal flip process */
module_param(vflip, int, 0444);			/**< @brief Module vertical flip process */

module_param(brightness, int, 0444);	/**< @brief Module brightness */
module_param(whiteness, int, 0444);		/**< @brief Module whiteness */
module_param(contrast, int, 0444);		/**< @brief Module contrast */
module_param(colour, int, 0444);		/**< @brief Module colour */
module_param(norm, int, 0444);			/**< @brief Module norm */


/** 
 * @returns 0 if all is OK
 *
 * @brief Initialize the driver.
 *
 * This function is called at first.
 * This function permits to define the default values from the command line.
 */
static int __init usb_stk11xx_init(void)
{
	int result;

	STK_INFO("%s driver %s startup\n", DRIVER_DESC, DRIVER_VERSION);
	STK_INFO("Copyright(c) 2006-%d %s\n", DRIVER_YEAR, DRIVER_AUTHOR);
	STK_INFO("%s\n", DRIVER_URL);

	// Frame per second parameter
	if (fps) {
		if (fps < 9 || fps > 30) {
			STK_ERROR("Framerate out of bounds [10-30] !\n");
			return -EINVAL;
		}

		default_fps = fps;
	}

	// Horizontal flip value
	if ((hflip == 0) || (hflip == 1)) {
		STK_DEBUG("Set horizontal flip = %d\n", hflip);

		default_hflip = hflip;
	}

	// Vertical flip value
	if ((vflip == 0) || (vflip == 1)) {
		STK_DEBUG("Set vertical flip = %d\n", vflip);

		default_vflip = vflip;
	}

	// Brightness value
	if (brightness > -1) {
		STK_DEBUG("Set brightness = 0x%X\n", brightness);

		default_brightness = 0xffff & brightness;
	}

	// Whiteness value
	if (whiteness > -1) {
		STK_DEBUG("Set whiteness = 0x%X\n", whiteness);

		default_whiteness = 0xffff & whiteness;
	}

	// Contrast value
	if (contrast > -1) {
		STK_DEBUG("Set contrast = 0x%X\n", contrast);

		default_contrast = 0xffff & contrast;
	}

	// Colour value
	if (colour > -1) {
		STK_DEBUG("Set colour = 0x%X\n", colour);

		default_colour = 0xffff & colour;
	}

	// Norm value
	if (norm > -1) {
		default_norm = (norm > 0) ? 1 : 0;

		STK_INFO("Set norm = %s\n", (default_norm > 0) ? "NTSC" : "PAL");
	}
 

	// Register the driver with the USB subsystem
	result = usb_register(&usb_stk11xx_driver);

	if (result)
		STK_ERROR("usb_register failed ! Error number %d\n", result);

	return result;
}


/** 
 * @brief Close the driver
 *
 * This function is called at last when you unload the driver.
 */
static void __exit usb_stk11xx_exit(void)
{
	STK_INFO("usb_stk11xx_exit: Syntek USB2.0 webcam driver shutdown\n");

	// Deregister this driver with the USB subsystem
	usb_deregister(&usb_stk11xx_driver);
}


module_init(usb_stk11xx_init);						/**< @brief Module initialize */
module_exit(usb_stk11xx_exit);						/**< @brief Module exit */


MODULE_PARM_DESC(fps, "Frames per second [5-30]");	/**< @brief Description of 'fps' parameter */
MODULE_PARM_DESC(hflip, "Horizontal image flip");	/**< @brief Description of 'hflip' parameter */
MODULE_PARM_DESC(vflip, "Vertical image flip");		/**< @brief Description of 'vflip' parameter */
MODULE_PARM_DESC(brightness, "Brightness setting");	/**< @brief Description of 'brightness' parameter */
MODULE_PARM_DESC(whiteness, "Whiteness setting");	/**< @brief Description of 'whiteness' parameter */
MODULE_PARM_DESC(colour, "Colour setting");			/**< @brief Description of 'colour' parameter */
MODULE_PARM_DESC(contrast, "Contrast setting");		/**< @brief Description of 'contrast' parameter */
MODULE_PARM_DESC(norm, "Norm setting (0=NTSC, 1=PAL)"); /**< @brief Description of 'default_norm' parameter */


MODULE_LICENSE("GPL");								/**< @brief Driver is under licence GPL */
MODULE_AUTHOR(DRIVER_AUTHOR);						/**< @brief Driver is written by Nicolas VIVIEN */
MODULE_DESCRIPTION(DRIVER_DESC);					/**< @brief Define the description of the driver */
MODULE_SUPPORTED_DEVICE(DRIVER_SUPPORT);			/**< @brief List of supported device */
MODULE_VERSION(DRIVER_VERSION);						/**< @brief Define the version of the driver */
MODULE_INFO(url, DRIVER_URL);						/**< @brief Driver homepage URL */


