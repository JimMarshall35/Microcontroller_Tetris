/*
 * OS.h
 *
 *  Created on: 26 Jun 2022
 *      Author: James.Marshall
 */

#ifndef INC_OS_H_
#define INC_OS_H_

#include "BasicTypedefs.h"

typedef struct{
	void* sp;
}OSThread;

typedef void (*OSThreadHandler)();

void OSThread_Start(
		OSThread* me,
		OSThreadHandler threadHandler,
		void* stkSto,
		u32 stkSize);

void OSSched();
void OSPendSV_Handler();


#endif /* INC_OS_H_ */
