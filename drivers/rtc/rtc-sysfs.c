/*
 * RTC subsystem, sysfs interface
 *
 * Copyright (C) 2005 Tower Technologies
 * Author: Alessandro Zummo <a.zummo@towertech.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/module.h>
#include <linux/rtc.h>

#include "rtc-core.h"


/* device attributes */

/*
 * NOTE:  RTC times displayed in sysfs use the RTC's timezone.  That's
 * ideally UTC.  However, PCs that also boot to MS-Windows normally use
 * the local time and change to match daylight savings time.  That affects
 * attributes including date, time, since_epoch, and wakealarm.
 */

static ssize_t
name_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s %s\n", dev_driver_string(dev->parent),
		       dev_name(dev->parent));
}
static DEVICE_ATTR_RO(name);

static ssize_t
date_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t retval;
	struct rtc_time tm;

	retval = rtc_read_time(to_rtc_device(dev), &tm);
	if (retval == 0) {
		retval = sprintf(buf, "%04d-%02d-%02d\n",
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	}

	return retval;
}
static DEVICE_ATTR_RO(date);

static ssize_t
time_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t retval;
	struct rtc_time tm;

	retval = rtc_read_time(to_rtc_device(dev), &tm);
	if (retval == 0) {
		retval = sprintf(buf, "%02d:%02d:%02d\n",
			tm.tm_hour, tm.tm_min, tm.tm_sec);
	}

	return retval;
}
static DEVICE_ATTR_RO(time);

static ssize_t
since_epoch_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t retval;
	struct rtc_time tm;

	retval = rtc_read_time(to_rtc_device(dev), &tm);
	if (retval == 0) {
		unsigned long time;
		rtc_tm_to_time(&tm, &time);
		retval = sprintf(buf, "%lu\n", time);
	}

	return retval;
}
static DEVICE_ATTR_RO(since_epoch);

static ssize_t
max_user_freq_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", to_rtc_device(dev)->max_user_freq);
}

static ssize_t
max_user_freq_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t n)
{
	struct rtc_device *rtc = to_rtc_device(dev);
	unsigned long val;
	int err;

	err = kstrtoul(buf, 0, &val);
	if (err)
		return err;

	if (val >= 4096 || val == 0)
		return -EINVAL;

	rtc->max_user_freq = (int)val;

	return n;
}
static DEVICE_ATTR_RW(max_user_freq);

/**
 * rtc_sysfs_show_hctosys - indicate if the given RTC set the system time
 *
 * Returns 1 if the system clock was set by this RTC at the last
 * boot or resume event.
 */
static ssize_t
hctosys_show(struct device *dev, struct device_attribute *attr, char *buf)
{
#ifdef CONFIG_RTC_HCTOSYS_DEVICE
	if (strcmp(dev_name(&to_rtc_device(dev)->dev),
		   CONFIG_RTC_HCTOSYS_DEVICE) == 0)
		return sprintf(buf, "1\n");
	else
#endif
		return sprintf(buf, "0\n");
}
static DEVICE_ATTR_RO(hctosys);

/* Tab A8 code for SR-AX6300-01-434 by wenyaqi at 20211108 start */
#ifdef CONFIG_RTC_PWRON_ALARM
extern int alarm_get_alarm(struct rtc_wkalrm *alarm);
static ssize_t
alarm_boot_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	ssize_t retval;
	struct rtc_wkalrm alm;

	retval = alarm_get_alarm(&alm);
	if (retval) {
		retval = sprintf(buf, "%d", alm.enabled);
		pr_info("sapa_%s:%d\n",__func__, alm.enabled);
		return retval;
	}

	return retval;
}
static DEVICE_ATTR_RO(alarm_boot);
#endif
/* Tab A8 code for SR-AX6300-01-433 by wenyaqi at 20211108 end */

static ssize_t
power_on_alarm_store(struct device *dev, struct device_attribute *attr,
	     const char *buf, size_t n)
{
	unsigned long val;
	int retval;
	struct rtc_wkalrm alm;
	struct rtc_device *rtc = to_rtc_device(dev);

	retval = kstrtoul(buf, 10, &val);
	if (retval)
		return retval;
	retval = val / 1000000000000;
	if (retval == 1) {
		alm.enabled = 1;
		val = val % 1000000000000;
		alm.time.tm_year = (val / 100000000) - 1900;
		val = val % 100000000;
		alm.time.tm_mon = (val / 1000000) - 1;
		val = val % 1000000;
		alm.time.tm_mday = val / 10000;
		val = val % 10000;
		alm.time.tm_hour = val / 100;
		alm.time.tm_min = val % 100;
		pr_info("power_on_alarm, expires: %d-%d-%d %d:%d\n",
			alm.time.tm_year + 1900, alm.time.tm_mon + 1, alm.time.tm_mday,
			alm.time.tm_hour, alm.time.tm_min);
		retval = rtc_set_alarm(rtc, &alm);
	} else if (val == 0) {
		pr_info("cancel the  power_on_alarm\n");
		rtc_alarm_irq_enable(rtc, 0);
	} else
		pr_warn("The power_on_alarm parameter is wrong\n");

	return (retval < 0) ? retval : n;
}
static DEVICE_ATTR_WO(power_on_alarm);

static ssize_t
wakealarm_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t retval;
	unsigned long alarm;
	struct rtc_wkalrm alm;

	/* Don't show disabled alarms.  For uniformity, RTC alarms are
	 * conceptually one-shot, even though some common RTCs (on PCs)
	 * don't actually work that way.
	 *
	 * NOTE: RTC implementations where the alarm doesn't match an
	 * exact YYYY-MM-DD HH:MM[:SS] date *must* disable their RTC
	 * alarms after they trigger, to ensure one-shot semantics.
	 */
	retval = rtc_read_alarm(to_rtc_device(dev), &alm);
	if (retval == 0 && alm.enabled) {
		rtc_tm_to_time(&alm.time, &alarm);
		retval = sprintf(buf, "%lu\n", alarm);
	}

	return retval;
}

static ssize_t
wakealarm_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t n)
{
	ssize_t retval;
	unsigned long now, alarm;
	unsigned long push = 0;
	struct rtc_wkalrm alm;
	struct rtc_device *rtc = to_rtc_device(dev);
	const char *buf_ptr;
	int adjust = 0;

	/* Only request alarms that trigger in the future.  Disable them
	 * by writing another time, e.g. 0 meaning Jan 1 1970 UTC.
	 */
	retval = rtc_read_time(rtc, &alm.time);
	if (retval < 0)
		return retval;
	rtc_tm_to_time(&alm.time, &now);

	buf_ptr = buf;
	if (*buf_ptr == '+') {
		buf_ptr++;
		if (*buf_ptr == '=') {
			buf_ptr++;
			push = 1;
		} else
			adjust = 1;
	}
	retval = kstrtoul(buf_ptr, 0, &alarm);
	if (retval)
		return retval;
	if (adjust) {
		alarm += now;
	}
	if (alarm > now || push) {
		/* Avoid accidentally clobbering active alarms; we can't
		 * entirely prevent that here, without even the minimal
		 * locking from the /dev/rtcN api.
		 */
		retval = rtc_read_alarm(rtc, &alm);
		if (retval < 0)
			return retval;
		if (alm.enabled) {
			if (push) {
				rtc_tm_to_time(&alm.time, &push);
				alarm += push;
			} else
				return -EBUSY;
		} else if (push)
			return -EINVAL;
		alm.enabled = 1;
	} else {
		alm.enabled = 0;

		/* Provide a valid future alarm time.  Linux isn't EFI,
		 * this time won't be ignored when disabling the alarm.
		 */
		alarm = now + 300;
	}
	rtc_time_to_tm(alarm, &alm.time);

	retval = rtc_set_alarm(rtc, &alm);
	return (retval < 0) ? retval : n;
}
static DEVICE_ATTR_RW(wakealarm);

static ssize_t
offset_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t retval;
	long offset;

	retval = rtc_read_offset(to_rtc_device(dev), &offset);
	if (retval == 0)
		retval = sprintf(buf, "%ld\n", offset);

	return retval;
}

static ssize_t
offset_store(struct device *dev, struct device_attribute *attr,
	     const char *buf, size_t n)
{
	ssize_t retval;
	long offset;

	retval = kstrtol(buf, 10, &offset);
	if (retval == 0)
		retval = rtc_set_offset(to_rtc_device(dev), offset);

	return (retval < 0) ? retval : n;
}
static DEVICE_ATTR_RW(offset);

static ssize_t
range_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "[%lld,%llu]\n", to_rtc_device(dev)->range_min,
		       to_rtc_device(dev)->range_max);
}
static DEVICE_ATTR_RO(range);

static struct attribute *rtc_attrs[] = {
	&dev_attr_name.attr,
	&dev_attr_date.attr,
	&dev_attr_time.attr,
	&dev_attr_since_epoch.attr,
	&dev_attr_max_user_freq.attr,
	&dev_attr_hctosys.attr,
	&dev_attr_power_on_alarm.attr,
	&dev_attr_wakealarm.attr,
	&dev_attr_offset.attr,
	&dev_attr_range.attr,
/* Tab A8 code for SR-AX6300-01-434 by wenyaqi at 20211108 start */
#ifdef CONFIG_RTC_PWRON_ALARM
	&dev_attr_alarm_boot.attr,
#endif
/* Tab A8 code for SR-AX6300-01-434 by wenyaqi at 20211108 end */
	NULL,
};

/* The reason to trigger an alarm with no process watching it (via sysfs)
 * is its side effect:  waking from a system state like suspend-to-RAM or
 * suspend-to-disk.  So: no attribute unless that side effect is possible.
 * (Userspace may disable that mechanism later.)
 */
static bool rtc_does_wakealarm(struct rtc_device *rtc)
{
	if (!device_can_wakeup(rtc->dev.parent))
		return false;

	return rtc->ops->set_alarm != NULL;
}

static umode_t rtc_attr_is_visible(struct kobject *kobj,
				   struct attribute *attr, int n)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct rtc_device *rtc = to_rtc_device(dev);
	umode_t mode = attr->mode;

	if (attr == &dev_attr_wakealarm.attr) {
		if (!rtc_does_wakealarm(rtc))
			mode = 0;
	} else if (attr == &dev_attr_offset.attr) {
		if (!rtc->ops->set_offset)
			mode = 0;
	} else if (attr == &dev_attr_range.attr) {
		if (!(rtc->range_max - rtc->range_min))
			mode = 0;
	}

	return mode;
}

static struct attribute_group rtc_attr_group = {
	.is_visible	= rtc_attr_is_visible,
	.attrs		= rtc_attrs,
};

static const struct attribute_group *rtc_attr_groups[] = {
	&rtc_attr_group,
	NULL
};

const struct attribute_group **rtc_get_dev_attribute_groups(void)
{
	return rtc_attr_groups;
}
