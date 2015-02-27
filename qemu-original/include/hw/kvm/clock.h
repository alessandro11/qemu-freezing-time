/*
 * QEMU KVM support, paravirtual clock device
 *
 * Copyright (C) 2011 Siemens AG
 *
 * Authors:
 *  Jan Kiszka        <jan.kiszka@siemens.com>
 *
 * This work is licensed under the terms of the GNU GPL version 2.
 * See the COPYING file in the top-level directory.
 *
 */
void kvmclock_stop(void);
void kvmclock_set(void);
void kvmclock_start(void);
int kvmclock_elapsed(void);
bool kvmclock(void);

#ifdef CONFIG_KVM
void kvmclock_create(void);

#else /* CONFIG_KVM */

static inline void kvmclock_create(void)
{
}

#endif /* !CONFIG_KVM */
