#include "hack/hack.h"
#include "qemu/option_int.h"
#include "sysemu/cpus.h"
#include "sysemu/kvm.h"
#include "math.h"

HackList *hacklist = NULL;
bool thereisbarrier = false;
int barrier_in = 0;

pthread_barrier_t our_barrier;
pthread_mutex_t our_mutex;
bool thereismutex = false;

KVMClockState *kvm_clock;
bool meu_pause = false;

extern int all_vcpus_paused(void);
extern int kvm_vm_ioctl(KVMState *s, int type, ...);

void initialize_hack(QemuOptsList *qemu_drive_opts, QemuOptsList *qemu_device_opts) {
       QemuOpts *opts;
       HackList *tmp;
       const char *status;

       // Configura para dataplane
       QTAILQ_FOREACH(opts, &qemu_device_opts->head, next) {
    	   configure_hack(opts);

       }

       // Configura para modo normal
    QTAILQ_FOREACH(opts, &qemu_drive_opts->head, next) {
        status = qemu_opt_get(opts,"hack");
        if( status && strcmp(status, "on") == 0 ) {
            if( hacklist == NULL) {
                hacklist = calloc(1,sizeof(HackList));
                hacklist->name = qemu_opt_get(opts,"file");
            }
            else {
                for (tmp=hacklist; tmp->next != NULL; tmp=tmp->next);
                tmp->next = calloc(1,sizeof(HackList));
                tmp->next->name = qemu_opt_get(opts,"file");
            }
        }
    }
}

void qemu_barrier_init(int size)
{
    int x;
    x = atomic_read(&barrier_in);
    while (x != 0)
        x = atomic_read(&barrier_in);

       pthread_barrier_init(&our_barrier,NULL,size);
       atomic_set(&thereisbarrier,true);
}

void qemu_barrier_wait(void)
{
    bool x;
    x = atomic_read(&thereisbarrier);

   if ( x )
   {
       atomic_inc(&barrier_in);
       pthread_barrier_wait(&our_barrier);
       atomic_dec(&barrier_in);
   }
}

void qemu_barrier_wait_inc(void)
{
    pthread_barrier_wait(&our_barrier);
}

void qemu_barrier_destroy(void)
{

    int x;

    extern int smp_cpus;

    // tive que fazer uma espera ocupada para esperar as vcpus chegarem na barreira
    // tinha um race condition que antes de eu "excluir" a barreira na io thread uma (ou mais)
    // vcpus poderiam entrar em contexto e sair (caindo na barreira) antes de eu remover
    while (1){
        x = atomic_read(&barrier_in);
        if (qemu_in_vcpu_thread()){

            if (x == smp_cpus-1)
                break;
        }
        else {
            if (x == smp_cpus)
                break;
        }


    }

    atomic_set(&thereisbarrier,false);
    return;
}

void meu_qemu_mutex_init(void) {
       pthread_mutex_init(&our_mutex,NULL);
}

void meu_qemu_mutex_lock(void) {
       pthread_mutex_lock(&our_mutex);
       thereismutex = true;
}
void meu_qemu_mutex_unlock(void){
       pthread_mutex_unlock(&our_mutex);
       thereismutex = false;
}


void meu_qemu_mutex_destroy(void){
       thereismutex = false;
       pthread_mutex_destroy(&our_mutex);
}

void pause_all_vcpus_hacked(struct kvm_clock_data *data)
{
    CPUState *cpu;

    extern QemuCond qemu_pause_cond;
    extern QemuMutex qemu_global_mutex;


    qemu_clock_enable(QEMU_CLOCK_VIRTUAL, false);
    CPU_FOREACH(cpu) {
        cpu->stop = true;
        qemu_cpu_kick(cpu);
    }

    kvmclock_set_meu(data);

    if (qemu_in_vcpu_thread()) {
        cpu_stop_current();
        if (!kvm_enabled()) {
            CPU_FOREACH(cpu) {
                cpu->stop = false;
                cpu->stopped = true;
            }
            return;
        }
    }

    while (!all_vcpus_paused()) {
        qemu_cond_wait(&qemu_pause_cond, &qemu_global_mutex);
        CPU_FOREACH(cpu) {
            qemu_cpu_kick(cpu);
        }
    }
}

bool kvmclock(KVMClockState *kvm_clock)
{
       return true;
}

inline void kvmclock_start(KVMClockState *kvm_clock)
{
       struct kvm_clock_data data;
       int ret;
       if (! kvm_clock->clock_armed) {
               meu_qemu_mutex_unlock();
               return;
       }
       kvm_clock->clock_armed = false;
       kvm_clock->clock_valid = false;
       data.clock = kvm_clock->clock ; //+ 10000000;
       data.flags = 0;

       ret = kvm_vm_ioctl(kvm_state, KVM_SET_CLOCK, &data);

       if (ret < 0) {
               fprintf(stderr, "KVM_SET_CLOCK failed: %s\n", strerror(ret));
               abort();
       }

}

void kvmclock_set_meu(struct kvm_clock_data *data)
{
    int ret;

    ret = kvm_vm_ioctl(kvm_state, KVM_GET_CLOCK, data);

    if (ret < 0) {
            fprintf(stderr, "KVM_GET_CLOCK failed: %s\n", strerror(ret));
            abort();
    }
}


void kvmclock_set(KVMClockState *kvm_clock)
{
       int ret;
       struct kvm_clock_data data;

       kvm_clock->need_pause = true;
       if ( kvm_clock->clock_armed ) return;
       kvm_clock->clock_armed = true;

       ret = kvm_vm_ioctl(kvm_state, KVM_GET_CLOCK, &data);
       kvm_clock->clock = data.clock;
       if (ret < 0) {
               fprintf(stderr, "KVM_GET_CLOCK failed: %s\n", strerror(ret));
               abort();
       }
}


double rand_normal(double mean, double stddev) {//Box muller method
    static double n2 = 0.0;
    static int n2_cached = 0;
    if (!n2_cached)
    {
        double x, y, r;
        do
        {
            x = 2.0*rand()/RAND_MAX - 1;
            y = 2.0*rand()/RAND_MAX - 1;

            r = x*x + y*y;
        }
        while (r == 0.0 || r > 1.0);
        {
            double d = sqrt(-2.0*log(r)/r);
            double n1 = x*d;
            n2 = y*d;
            double result = n1*stddev + mean;
            n2_cached = 1;
            return result;
        }
    }
    else
    {
        n2_cached = 0;
        return n2*stddev + mean; //ainda precisa descontar nosso overhead de ~74000
    }
}
