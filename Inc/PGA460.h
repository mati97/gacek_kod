/*
 * PGA460.h
 *
 *  Created on: May 27, 2020
 *      Author: mati97
 */

#ifndef PGA460_H_
#define PGA460_H_

#include "main.h"
#include "usart.h"


void write_reg(uint8_t address, uint8_t data);
void burst_listen(uint8_t number);
void request_data(uint8_t cmd, uint8_t address);
uint8_t* get_data(uint8_t cmd);
int8_t init_pga_memory(void);
void init_single(TIM_HandleTypeDef *htim);
void init_burst(TIM_HandleTypeDef *htim);
void burst(uint16_t time);
void single(uint16_t time);
void burst_callback(void);
void single_callback(uint8_t* rcv);

#endif /* PGA460_H_ */
