/**
  ******************************************************************************
  * @file    Templates/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "./usart/bsp_debug_usart.h"
#include "./tim/bsp_basic_tim.h"
#include "./spi/bsp_spi.h"
#include "./adc/bsp_adc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define TX              0       // cc1101����ģʽ
#define RX              1       // cc1101����ģʽ
#define IDLE          	2       // cc1101����ģʽ
#define ACK_LENGTH      60   		// �������ݰ����� 
#define ACK_CNT					ACK_LENGTH/6	// floor(ACK_LENGTH/6)
#define SEND_LENGTH     60      // �������ݰ�����
#define SEND_GAP  			2 			// ���͵ȴ�1s
#define RECV_TIMEOUT		20    	// ���յȴ�2s
/* Exported macro ------------------------------------------------------------*/
uint8_t SendFlag = 0;         	// =1�����������ݣ�=0������
uint16_t SendTime = 1;         	// �������ݷ��ͼ��ʱ��
uint8_t RecvFlag;       				// =1���յȴ�ʱ�������=0������
uint16_t RecvWaitTime;  				// ���յȴ�ʱ��
uint8_t CollectCnt;      				// ����MMA7361L����
uint8_t	Chip_Addr	= 0;					// cc1101��ַ
uint8_t	RSSI = 0;								// RSSIֵ
uint16_t SendCnt = 80;       	// ���Ͱ���
uint16_t RecvCnt = 30;        	// Ӧ�����
uint8_t SendBuffer[SEND_LENGTH] = {'0','C','h','a','n','g','e',' ','i','s',' ','a',' ','l','a','w',',',' ','a','n','d',' ','n','o',' ','a','m','o','u','n','t',' ','o','f',' ','p','r','e','t','e','n','d','i','n','g',' ','w','i','l','l',' ','a','l','t','e','r'};// �������ݰ�
uint8_t AckBuffer[ACK_LENGTH] = {'a','c','k','n','o','w','l','e','d','g','e'};// Ӧ�����ݰ�
extern uint16_t ADC_ConvertedValue[MMA7361L_NOFCHANEL];// ADCת���ĵ�ѹֵͨ��MDA���ݵ�SRAM
float ADC_ConvertedValueLocal[MMA7361L_NOFCHANEL];// ���ڱ���ת�������ĵ�ѹֵ

uint8_t temp2 = 0;//for test
/* Exported functions ------------------------------------------------------- */
static void SystemClock_Config(void);
void Error_Handler(void);
static void Show_Message(void);
uint8_t	RF_Acknowledge(void);
static void Delay(__IO uint32_t nCount);
void MMA7361L_ReadHandler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
