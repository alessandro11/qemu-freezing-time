#ifndef __AETRACE_H__
#define __AETRACE_H__     1

#include <sys/syscall.h>

#define  AETRACE1(msg, p1) \
    fprintf(stderr, "\nAETRACE %s(%d): ThreadId: %ld \n" msg "\n", __FILE__, __LINE__, syscall(SYS_gettid), (p1))
#define  AETRACE2(msg, p1, p2) \
    fprintf(stderr, "\nAETRACE %s(%d): ThreadId: %ld \n" msg "\n", __FILE__, __LINE__, syscall(SYS_gettid), (p1), (p2))
#define  AETRACE3(msg, p1, p2, p3) \
    fprintf(stderr, "\nAETRACE %s(%d): ThreadId: %ld \n" msg "\n", __FILE__, __LINE__, syscall(SYS_gettid), (p1), (p2), (p3))
#define  AETRACE(msg) \
    fprintf(stderr, "\nAETRACE %s(%d): ThreadId: %ld \n" msg "\n", __FILE__, __LINE__, syscall(SYS_gettid))

#endif /* __AETRACE_H__ */
