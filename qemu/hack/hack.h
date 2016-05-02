#include "hw/sysbus.h"
#include <linux/kvm.h>

typedef struct KVMClockState {
    /*< private >*/
    SysBusDevice busdev;
    /*< public >*/

    uint64_t clock;
    bool clock_valid;
    bool clock_armed;
    bool need_pause;

} KVMClockState;

typedef struct HackList {
    const char *name;
    struct HackList *next;
}HackList;

void initialize_hack(QemuOptsList *qemu_drive_opts, QemuOptsList *qemu_device_opts);
void qemu_barrier_init(int size);
void qemu_barrier_wait(void);
void qemu_barrier_wait_inc(void);
void qemu_barrier_destroy(void);
void meu_qemu_mutex_init(void);
void meu_qemu_mutex_lock(void);
void meu_qemu_mutex_unlock(void);
void meu_qemu_mutex_destroy(void);
void pause_all_vcpus_hacked(struct kvm_clock_data *data);
void kvmclock_set(KVMClockState *kvm_clock);
void kvmclock_set_meu(struct kvm_clock_data *data);
inline void kvmclock_start(KVMClockState *kvm_clock);
bool kvmclock(KVMClockState *kvm_clock);

