#ifndef __AETRACE_H__
#define __AETRACE_H__     1

#include <sys/syscall.h>

#define  AETRACE1(msg, p1) \
    fprintf(stderr, "\nAETRACE ThreadId: %ld " msg "\n", syscall(SYS_gettid), p1)

#define  AETRACE(msg) \
    fprintf(stderr, "\nAETRACE ThreadId: %ld " msg "\n", syscall(SYS_gettid))

#endif /* __AETRACE_H__ */
