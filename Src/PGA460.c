/*
 * PGA460.c
 *
 *  Created on: May 27, 2020
 *      Author: mati97
 */


#include "PGA460.h"

extern uint8_t pga_mem_map[127];

uint8_t calc_checksum(uint8_t* input_array, uint8_t size){

	uint16_t carry = 0;

	for (int i = 0; i < 3; i++){

		if ((*(input_array+i) + carry) < carry){

			carry = carry + *(input_array+i) + 1;

		}else{

			carry = carry + *(input_array+i);

		}
		if (carry > 0xFF){
		  carry = carry - 255;
		}
	}

	return (~carry & 0x00FF);

}

void write_reg(uint8_t address, uint8_t data){

	uint8_t arr[5]={0x55,0x0a};

	arr[2] = address;
	arr[3] = data;
	arr[4] = calc_checksum((arr+1), 3);

	USART_WriteData(arr, 5);
}


//in case of data dump, or ultrasonic measurement, address is not important
void request_data(uint8_t cmd, uint8_t address){

	uint8_t arr[4]={0x55};

	switch (cmd){
	case 0x05:
		arr[1] = 0x05;
		arr[2] = 0xfa;
		USART_WriteData(arr, 3);
		break;
	case 0x07:
		arr[1] = 0x07;
		arr[2] = 0xf8;
		USART_WriteData(arr, 3);
		break;
	case 0x09:
		arr[1] = 0x09;
		arr[2] = address;
		arr[3] = calc_checksum((arr+1), 2);
		USART_WriteData(arr, 4);
		break;
	}
}

uint8_t* get_data(uint8_t cmd){

	static uint8_t rcv[130];

	uint8_t size=0;

	switch (cmd){
	case 0x05:
		size = 4*1+2;
		break;
	case 0x07:
		size = 130;
		break;
	case 0x09:
		size = 3;
		break;
	}

	USART_ReadData((char*)rcv, size);
	return rcv;
}

int8_t init_pga_memory(void){
	  uint8_t i=0;
	  for(uint8_t add_offset=0; add_offset<91;add_offset++){

		  HAL_Delay(60);
		 write_reg(i,*(pga_mem_map+add_offset));

		  switch(i){
		  case 0x2b:
			  i = 0x40;
			  break;
		  case 0x4d:
			  i=0x5f;
			  break;
		  default:
			  i++;
			  break;
		  }
	  }
	  uint8_t* resp;

	  request_data(0x09, 0x4c);
	  HAL_Delay(1);
	  resp = get_data(0x09);
	  if((*resp != 0x40) | ((*(resp+1)&0x0f) != 0x00)) return -1;

	  request_data(0x09, 0x4d);
	  HAL_Delay(1);
	  resp = get_data(0x09);
	  if((*resp != 0x40) | ((*(resp+1)&0x7f) != 0x00)) return -1;

	  return 0;
}
void init_single(TIM_HandleTypeDef *htim){
	htim->Init.Prescaler = 43;
	  if (HAL_TIM_Base_Init(htim) != HAL_OK)
	  {
	    Error_Handler();
	  }
	HAL_TIM_Base_Start(htim);

	write_reg(0x40,0x00);
}

void init_burst(TIM_HandleTypeDef *htim){
	htim->Init.Prescaler = 21;
	  if (HAL_TIM_Base_Init(htim) != HAL_OK)
	  {
	    Error_Handler();
	  }
	HAL_TIM_Base_Start(htim);

	write_reg(0x40,0x80);
}

void burst(uint16_t time){



}

void single(uint16_t time){


}

__weak void burst_callback(void){}

__weak void single_callback(void){}
