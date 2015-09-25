#ifndef __AETRACE_H__
#define __AETRACE_H__     1

#ifndef AE_NODEBUG
#include <sys/syscall.h>

#define eprintf(fmt, ...) \
	do { fprintf(stderr, \
	"ThreadId(%ld):\n" \
	"%s(%d): func=%s\n\t" \
	fmt "\n", \
	syscall(SYS_gettid), __FILE__, __LINE__, __func__, ## __VA_ARGS__); } while (0)

#define  AETRACE1(msg, p1) \
    fprintf(stderr, "AETRACE %s(%d): ThreadId(%ld): " msg "\n", __FILE__, __LINE__, syscall(SYS_gettid), (p1))
#define  AETRACE2(msg, p1, p2) \
    fprintf(stderr, "AETRACE %s(%d): ThreadId(%ld): " msg "\n", __FILE__, __LINE__, syscall(SYS_gettid), (p1), (p2))
#define  AETRACE3(msg, p1, p2, p3) \
    fprintf(stderr, "AETRACE %s(%d): ThreadId(%ld): " msg "\n", __FILE__, __LINE__, syscall(SYS_gettid), (p1), (p2), (p3))
#define  AETRACE(msg) \
    fprintf(stderr, "AETRACE %s(%d): ThreadId(%ld): " msg "\n", __FILE__, __LINE__, syscall(SYS_gettid))

#else

#define eprintf(fmt, ...)  ((void)0)
#define AETRACE  ((void)0)
#define AETRACE1 ((void)0)
#define AETRACE2 ((void)0)
#define AETRACE3 ((void)0)

#endif /* AE_NODEBUG */

#endif /* __AETRACE_H__ */
