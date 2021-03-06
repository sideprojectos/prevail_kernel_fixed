/* arch/arm/mach-msm/vreg.c
 *
 * Copyright (C) 2008 Google, Inc.
 * Copyright (c) 2009, Code Aurora Forum. All rights reserved.
 * Author: Brian Swetland <swetland@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/string.h>
#include <mach/vreg.h>

#include "proc_comm.h"

#if defined(CONFIG_MSM_VREG_SWITCH_INVERTED)
#define VREG_SWITCH_ENABLE 0
#define VREG_SWITCH_DISABLE 1
#else
#define VREG_SWITCH_ENABLE 1
#define VREG_SWITCH_DISABLE 0
#endif

struct vreg {
	const char *name;
	unsigned id;
	int status;
	unsigned refcnt;
};

#define VREG(_name, _id, _status, _refcnt) \
	{ .name = _name, .id = _id, .status = _status, .refcnt = _refcnt }

static struct vreg vregs[] = {
	VREG("msma",	0, 0, 0),
	VREG("msmp",	1, 0, 0),
	VREG("msme1",	2, 0, 0),
	VREG("msmc1",	3, 0, 0),
	VREG("msmc2",	4, 0, 0),
	VREG("gp3",	5, 0, 0),
	VREG("msme2",	6, 0, 0),
	VREG("gp4",	7, 0, 0),
	VREG("gp1",	8, 0, 0),
	VREG("tcxo",	9, 0, 0),
	VREG("pa",	10, 0, 0),
	VREG("rftx",	11, 0, 0),
	VREG("rfrx1",	12, 0, 0),
	VREG("rfrx2",	13, 0, 0),
	VREG("synt",	14, 0, 0),
	VREG("wlan",	15, 0, 0),
	VREG("usb",	16, 0, 0),
	VREG("boost",	17, 0, 0),
	VREG("mmc",	18, 0, 0),
	VREG("ruim",	19, 0, 0),
	VREG("msmc0",	20, 0, 0),
	VREG("gp2",	21, 0, 0),
	VREG("gp5",	22, 0, 0),
	VREG("gp6",	23, 0, 0),
	VREG("rf",	24, 0, 0),
	VREG("rf_vco",	25, 0, 0),
	VREG("mpll",	26, 0, 0),
	VREG("s2",	27, 0, 0),
	VREG("s3",	28, 0, 0),
	VREG("rfubm",	29, 0, 0),
	VREG("ncp",	30, 0, 0),
	VREG("gp7",	31, 0, 0),
	VREG("gp8",	32, 0, 0),
	VREG("gp9",	33, 0, 0),
	VREG("gp10",	34, 0, 0),
	VREG("gp11",	35, 0, 0),
	VREG("gp12",	36, 0, 0),
	VREG("gp13",	37, 0, 0),
	VREG("gp14",	38, 0, 0),
	VREG("gp15",	39, 0, 0),
	VREG("ldo1",	40, 0, 0),
	VREG("ldo2",	41, 0, 0),
	VREG("ldo3",	42, 0, 0),
	VREG("ldo4",	43, 0, 0),
	VREG("ldo5",	44, 0, 0),
	VREG("ldo6",	45, 0, 0),
	VREG("ldo7",	46, 0, 0),
	VREG("ldo8",	47, 0, 0),
	VREG("ldo9",	48, 0, 0),
	VREG("ldo10",	49, 0, 0),
	VREG("ldo11",	50, 0, 0),
	VREG("ldo12",	51, 0, 0),
	VREG("ldo13",	52, 0, 0),
	VREG("ldo14",	53, 0, 0),
	VREG("ldo15",	54, 0, 0),
	VREG("ldo16",	55, 0, 0),
	VREG("ldo17",	56, 0, 0),
	VREG("ldo18",	57, 0, 0),
	VREG("ldo19",	58, 0, 0),
	VREG("gp16",	59, 0, 0),
	VREG("gp17",	60, 0, 0),
	VREG("s4",	61, 0, 0),
	VREG("usb2",	62, 0, 0),
	VREG("wlan2",	63, 0, 0),
	VREG("xo_out",	64, 0, 0),
	VREG("lvsw0",	65, 0, 0),
	VREG("lvsw1",	66, 0, 0),
};

struct vreg *vreg_get(struct device *dev, const char *id)
{
	int n;
	for (n = 0; n < ARRAY_SIZE(vregs); n++) {
		if (!strcmp(vregs[n].name, id))
			return vregs + n;
	}
	return ERR_PTR(-ENOENT);
}
EXPORT_SYMBOL(vreg_get);

void vreg_put(struct vreg *vreg)
{
}

/* Function definition to added for only keypad backlight enabling */
int vreg_backlight_enable(struct vreg *vreg)
{
	unsigned id = vreg->id;
	int enable = VREG_SWITCH_ENABLE;

	vreg->status = msm_proc_comm(PCOM_VREG_SWITCH, &id, &enable);
	return vreg->status;
}
EXPORT_SYMBOL(vreg_backlight_enable);


/* Function definition to added for only keypad backlight disabling */
int vreg_backlight_disable(struct vreg *vreg)
{
	unsigned id = vreg->id;
	int disable = VREG_SWITCH_DISABLE;

	vreg->status = msm_proc_comm(PCOM_VREG_SWITCH, &id, &disable);
	return vreg->status;
}
EXPORT_SYMBOL(vreg_backlight_disable);


int vreg_enable(struct vreg *vreg)
{
	unsigned id = vreg->id;
	int enable = VREG_SWITCH_ENABLE;

	if (vreg->refcnt == 0)
		vreg->status = msm_proc_comm(PCOM_VREG_SWITCH, &id, &enable);

	if ((vreg->refcnt < UINT_MAX) && (!vreg->status))
		vreg->refcnt++;

	return vreg->status;
}
EXPORT_SYMBOL(vreg_enable);

int vreg_disable(struct vreg *vreg)
{
	unsigned id = vreg->id;
	int disable = VREG_SWITCH_DISABLE;

	if (!vreg->refcnt)
		return 0;

	if (vreg->refcnt == 1)
		vreg->status = msm_proc_comm(PCOM_VREG_SWITCH, &id, &disable);

	if (!vreg->status)
		vreg->refcnt--;

	return vreg->status;
}
EXPORT_SYMBOL(vreg_disable);

#if 0 //MAX8899 PMIC
int vreg_set_level(struct vreg *vreg, unsigned mv)
{
	unsigned id = vreg->id;

	vreg->status = msm_proc_comm(PCOM_VREG_SET_LEVEL, &id, &mv);
	return vreg->status;
}
#else
int vreg_set_level(struct vreg *vreg, out_voltage_type mv)
{
	unsigned id = vreg->id;

	if(mv >= FAIL_VOLT) {
		printk(KERN_ERR "[VREG] %s : invalid vreg_level\n", __func__);
		return 1; //Error
	}

	vreg->status = msm_proc_comm(PCOM_VREG_SET_LEVEL, &id, &mv);
	return vreg->status;
}
#endif

EXPORT_SYMBOL(vreg_set_level);

#if defined(CONFIG_DEBUG_FS)

static int vreg_debug_set(void *data, u64 val)
{
	struct vreg *vreg = data;
	switch (val) {
	case 0:
		vreg_disable(vreg);
		break;
	case 1:
		vreg_enable(vreg);
		break;
	default:
		vreg_set_level(vreg, val);
		break;
	}
	return 0;
}

static int vreg_debug_get(void *data, u64 *val)
{
	struct vreg *vreg = data;

	if (!vreg->status)
		*val = 0;
	else
		*val = 1;

	return 0;
}

static int vreg_debug_count_set(void *data, u64 val)
{
	struct vreg *vreg = data;
	if (val > UINT_MAX)
		val = UINT_MAX;
	vreg->refcnt = val;
	return 0;
}

static int vreg_debug_count_get(void *data, u64 *val)
{
	struct vreg *vreg = data;

	*val = vreg->refcnt;

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(vreg_fops, vreg_debug_get, vreg_debug_set, "%llu\n");
DEFINE_SIMPLE_ATTRIBUTE(vreg_count_fops, vreg_debug_count_get,
			vreg_debug_count_set, "%llu\n");

static int __init vreg_debug_init(void)
{
	struct dentry *dent;
	int n;
	char name[32];
	const char *refcnt_name = "_refcnt";

	dent = debugfs_create_dir("vreg", 0);
	if (IS_ERR(dent))
		return 0;

	for (n = 0; n < ARRAY_SIZE(vregs); n++) {
		(void) debugfs_create_file(vregs[n].name, 0644,
					   dent, vregs + n, &vreg_fops);

		strlcpy(name, vregs[n].name, sizeof(name));
		strlcat(name, refcnt_name, sizeof(name));
		(void) debugfs_create_file(name, 0644,
					   dent, vregs + n, &vreg_count_fops);
	}

	return 0;
}

device_initcall(vreg_debug_init);
#endif
