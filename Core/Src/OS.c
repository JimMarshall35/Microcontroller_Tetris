/*
 * OS.c
 *
 *  Created on: 26 Jun 2022
 *      Author: James.Marshall
 */


#include "OS.h"
#include "cmsis_gcc.h"


OSThread* volatile _currentThread;
OSThread* volatile _nextThread;
void OSThread_Start(
		OSThread* me,
		OSThreadHandler threadHandler,
		void* stkSto,
		u32 stkSize){
	u32* sp = (u32*)((((u32)stkSto + stkSize)/8U)*8U);
	// these registers are saved automatically
	*(--sp) = (1 << 24);         // PSR
	*(--sp) = (u32)threadHandler; // PC
	*((u32*)--sp) = 0x0000000EU;        // LR
	*((u32*)--sp) = 0x0000000CU;        // R12
	*((u32*)--sp) = 0x00000003U;        // R3
	*((u32*)--sp) = 0x00000002U;        // R2
	*((u32*)--sp) = 0x00000001U;        // R1
	*((u32*)--sp) = 0x00000000U;        // R0
	// registers r4 - r11 must be  saved manually by the os kernel
	*((u32*)--sp) = 0x0000000BU; // r11
	*((u32*)--sp) = 0x0000000AU; // r10
	*((u32*)--sp) = 0x00000009U; // r9
	*((u32*)--sp) = 0x00000008U; // r8
	*((u32*)--sp) = 0x00000007U; // r7
	*((u32*)--sp) = 0x00000006U; // r6
	*((u32*)--sp) = 0x00000005U; // r5
	*((u32*)--sp) = 0x00000004U; // r4

	u32* stackLimit = (u32*)(((((u32)stkSto -1)/8) + 1)*8);
	me->sp = sp;
	for(sp = sp-1; sp>=stackLimit; --sp){
		*sp = 0xdeadc0deU;
	}

}

__attribute__((optimize("O0")))
static void trigger_pendsv(void) {
  volatile uint32_t *icsr = (void *)0xE000ED04;
  // Pend a PendSV exception using by writing 1 to PENDSVSET at bit 28
  *icsr = 0x1 << 28;
  // flush pipeline to ensure exception takes effect before we
  // return from this routine
  __asm("isb");
}

inline void OSPendSV_Handler(){
	void* sp = (void*)0xdeadbeef;
	__disable_irq();
	if(_currentThread != (OSThread*)0){
		// push registers r4-r11
		_currentThread->sp = sp;
	}
	sp = _nextThread->sp;
	_currentThread = _nextThread;
	// pop registers r4-r11
	__enable_irq();

}

void OSSched(){
	// set _nextThread... TODO
	if(_nextThread != _currentThread){
		trigger_pendsv();
	}
}
