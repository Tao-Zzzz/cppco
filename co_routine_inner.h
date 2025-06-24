
#ifndef __CO_ROUTINE_INNER_H__

#include "co_routine.h"
#include "coctx.h"
struct stCoRoutineEnv_t;
struct stCoSpec_t
{
	void *value;
};

struct stStackMem_t
{
	// 当前是哪个协程在用栈
	stCoRoutine_t* occupy_co;
	int stack_size;
	// 栈顶, 高地址, buffer + stack_size
	char* stack_bp;
	// 栈底, 低地址, buffer
	char* stack_buffer;

};

// 共享栈中多栈可以使得我们在协程切换的时候减少拷贝次数
struct stShareStack_t
{
	unsigned int alloc_idx;		// 下一次调用中应该使用的那个共享栈的index
	int stack_size;				// 共享栈的大小，这里的大小指的是一个stStackMem_t*的大小
	int count;					// 共享栈的个数，共享栈可以为多个，所以以下为共享栈的数组
	stStackMem_t **stack_array; // 栈的内容，这里是个数组，元素是stStackMem_t*
};


// 存储协程的所有信息
struct stCoRoutine_t
{
	// 管理协程
	stCoRoutineEnv_t *env;
	// 协程启动时调用
	pfn_co_routine_t pfn;
	void *arg;
	// 协程上下文
	coctx_t ctx;

	
	char cStart;			//是否开始运行
	char cEnd;				//是否结束运行
	char cIsMain;			// 是否是主协程, 主协程不允许被挂起
	char cEnableSysHook;	// 是否启用系统hook, 主要是为了兼容老版本的代码	
	char cIsShareStack;		// 是否使用共享栈, 如果是共享栈, 则不需要释放栈空间

	// 额外的环境指针
	void *pvEnv;

	// char sRunStack[ 1024 * 128 ];
	// 指向协程使用栈
	stStackMem_t* stack_mem;


	//save satck buffer while confilct on same stack_buffer;
	char* stack_sp;   			// 当前协程的栈指针, 用于保存当前协程的栈状态
	unsigned int save_size;		// 当前协程的栈大小, 用于保存当前协程的栈状态
	char* save_buffer;			// 当前协程的栈缓冲区, 用于保存当前协程的栈状态

	// 特定数据
	stCoSpec_t aSpec[1024];

};



//1.env
void 				co_init_curr_thread_env();
stCoRoutineEnv_t *	co_get_curr_thread_env();

//2.coroutine
void    co_free( stCoRoutine_t * co );
void    co_yield_env(  stCoRoutineEnv_t *env );

//3.func



//-----------------------------------------------------------------------------------------------

struct stTimeout_t;
struct stTimeoutItem_t ;

stTimeout_t *AllocTimeout( int iSize );
void 	FreeTimeout( stTimeout_t *apTimeout );
int  	AddTimeout( stTimeout_t *apTimeout,stTimeoutItem_t *apItem ,uint64_t allNow );

struct stCoEpoll_t;
stCoEpoll_t * AllocEpoll();
void 		FreeEpoll( stCoEpoll_t *ctx );

stCoRoutine_t *		GetCurrThreadCo();
void 				SetEpoll( stCoRoutineEnv_t *env,stCoEpoll_t *ev );

typedef void (*pfnCoRoutineFunc_t)();

#endif

#define __CO_ROUTINE_INNER_H__
