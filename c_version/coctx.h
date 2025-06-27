#ifndef __CO_CTX_H__
#define __CO_CTX_H__
#include <stdlib.h>

typedef void *(*coctx_pfn_t)(void *s, void *s2);
struct coctx_param_t
{
	const void *s1;
	const void *s2;
};

struct coctx_t
{
#if defined(_WIN32)
	void *fiber; // Windows Fiber句柄
	size_t ss_size;
	char *ss_sp;
#else
#if defined(__i386__)
	void *regs[8];
#else
	void *regs[14];
#endif
	size_t ss_size;
	char *ss_sp;
#endif
};

int coctx_init(coctx_t *ctx);
// 保存上一个协程的上下文
int coctx_make(coctx_t *ctx, coctx_pfn_t pfn, const void *s, const void *s1);
void coctx_swap(coctx_t *from, coctx_t *to);

#endif