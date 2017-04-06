/*
 * FSM.h
 *
 *  Created on: 2015Äê10ÔÂ8ÈÕ
 *      Author: Administrator
 */

#ifndef FSM_H_
#define FSM_H_

#define		FSM_STATE_INIT		0x00
#define		FSM_STATE_IDLE		0x01
#define		FSM_STATE_SET		0x02
#define		FSM_STATE_RUN		0x03
#define		FSM_STATE_FINISH	0x04
#define		FSM_STATE_SYS		0x05
#define		FSM_STATE_DEBUG		0x06
#define		FSM_STATE_ERR		0x07
#define		FSM_STATE_FAIL		0x08

void fsm_set(uint32_t);
uint32_t fsm_get(void);

#endif /* FSM_H_ */
