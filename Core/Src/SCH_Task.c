/*
 * SCH_Task.c
 *
 *  Created on: Nov 24, 2023
 *      Author: phongtran
 */
#include "SCH_Task.h"

typedef struct{
	void (*pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t Runme;
	uint32_t TaskID;
} sTask;

#define TIME_CYCLE 10
static sTask SCH_task_array[SCH_MAX_TASKS];

static uint32_t newTaskId = 0;
static uint32_t count_SCH_Update = 0;
static uint32_t Get_Task_ID(void);

uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
	uint8_t newTaskIndex = 0;
	uint8_t sumDelay = 0; // sum of new task delay and tasks delay in the array
	uint8_t newDelay = 0; // new delay for the task

	for(;newTaskIndex < SCH_MAX_TASKS;newTaskIndex++){
		sumDelay  = sumDelay + SCH_task_array[newTaskIndex].Delay;
		if(sumDelay > DELAY){
			newDelay = DELAY - (sumDelay - SCH_task_array[newTaskIndex].Delay);
			// old task
			SCH_task_array[newTaskIndex].Delay = sumDelay - DELAY;

			// shift all the remaining task to right 1 position
			for(uint8_t i = SCH_MAX_TASKS - 1; i > newTaskIndex;i--){
				SCH_task_array[i].pTask = SCH_task_array[i - 1].pTask;
				SCH_task_array[i].Period = SCH_task_array[i - 1].Period;
				SCH_task_array[i].Delay = SCH_task_array[i - 1].Delay;
				SCH_task_array[i].Runme = SCH_task_array[i - 1].Runme;
				SCH_task_array[i].TaskID = SCH_task_array[i - 1].TaskID;
			}
			// Add new task at newIndex position
			SCH_task_array[newTaskIndex].pTask = pFunction;
			SCH_task_array[newTaskIndex].Delay = newDelay;
			SCH_task_array[newTaskIndex].Period = PERIOD;
			SCH_task_array[newTaskIndex].Runme = 0;
			if(SCH_task_array[newTaskIndex].Delay == 0){
				SCH_task_array[newTaskIndex].Runme = 1;
			}
			SCH_task_array[newTaskIndex].TaskID = Get_Task_ID();
			return SCH_task_array[newTaskIndex].TaskID;
		}
		// if get to the last then break
		if(SCH_task_array[newTaskIndex].pTask == 0x0000) break;
	}
	// Add new task at newIndex position
	if(newTaskIndex >= SCH_MAX_TASKS) {
		return -1;
	}
	SCH_task_array[newTaskIndex].pTask = pFunction;
	SCH_task_array[newTaskIndex].Delay = DELAY - sumDelay;
	SCH_task_array[newTaskIndex].Period = PERIOD;
	SCH_task_array[newTaskIndex].Runme = 0;
	if(SCH_task_array[newTaskIndex].Delay == 0){
		SCH_task_array[newTaskIndex].Runme = 1;
	}
	SCH_task_array[newTaskIndex].TaskID = Get_Task_ID();
	return SCH_task_array[newTaskIndex].TaskID;
}
void SCH_Update(){
	count_SCH_Update++;
	if(SCH_task_array[0].pTask != 0x0000 && SCH_task_array[0].Runme == 0 ){
		if(SCH_task_array[0].Delay > 0){
			SCH_task_array[0].Delay = SCH_task_array[0].Delay - 1;
		}
		if(SCH_task_array[0].Delay == 0) SCH_task_array[0].Runme = 1;
	}
}
uint8_t SCH_Delete_Task(uint32_t taskID){
	uint8_t Return_code = 0;
	uint8_t taskIndex = 0;
	uint8_t j;
	if(taskID != NO_TASK_ID){
		for(taskIndex = 0; taskIndex< SCH_MAX_TASKS;taskIndex++){
			if(taskID == SCH_task_array[taskIndex].TaskID){
				Return_code = 1;
				if(taskIndex != 0 && taskIndex < SCH_MAX_TASKS - 1){
					if(SCH_task_array[taskIndex + 1].pTask != 0x0000){
						SCH_task_array[taskIndex+1].Delay += SCH_task_array[taskIndex].Delay;
					}
				}
				// shift all the remain to left 1 position
				for(j = taskIndex; j< SCH_MAX_TASKS - 1;j++){
					SCH_task_array[j].pTask = SCH_task_array[j+1].pTask;
					SCH_task_array[j].Period = SCH_task_array[j+1].Period;
					SCH_task_array[j].Delay = SCH_task_array[j+1].Delay;
					SCH_task_array[j].Runme = SCH_task_array[j+1].Runme;
					SCH_task_array[j].TaskID = SCH_task_array[j+1].TaskID;
				}
				// delete the task
				SCH_task_array[j].pTask = 0x0000;
				SCH_task_array[j].Period = 0;
				SCH_task_array[j].Delay = 0;
				SCH_task_array[j].Runme = 0;
				SCH_task_array[j].TaskID = 0;
				break;

			}
		}
	}
	return Return_code;
}
void SCH_Dispatch_Tasks(void){
	if(SCH_task_array[0].Runme > 0){
		debugPrintout(SCH_task_array[0].Period,SCH_task_array[0].TaskID);
		(*SCH_task_array[0].pTask)();
		SCH_task_array[0].Runme = 0;
		sTask temTask = SCH_task_array[0];
		SCH_Delete_Task(temTask.TaskID);
		if(temTask.Period != 0){
			SCH_Add_Task(temTask.pTask, temTask.Period, temTask.Period);
		}
	}
}
static uint32_t Get_Task_ID(){
	newTaskId++;
//	if(newTaskId == NO_TASK_ID) newTaskId++;
	return newTaskId;
}

void SCH_Init(){
	unsigned char i ;
	for(i = 0; i< SCH_MAX_TASKS;i++){
		SCH_Delete_Task(i);
	}
}

