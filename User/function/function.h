/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FUNCTION_H
#define __FUNCTION_H

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
#define RECV_TIMEOUT		20    	// ���յȴ�2s
#define SEND_LENGTH     15			// �������ݰ�����
#define SEND_PACKAGE_NUM	100		// �������ݰ���
#define RECV_LENGTH   	10			// �������ݰ�����
/* Exported macro ------------------------------------------------------------*/
extern uint8_t RecvFlag;      	// =1���յȴ�ʱ�������=0������
extern uint16_t RecvWaitTime;  	// ���յȴ�ʱ��
extern uint16_t ADC_ConvertedValue[MMA7361L_NOFCHANEL];// ADCת���ĵ�ѹֵͨ��MDA���ݵ�SRAM

/* Exported functions ------------------------------------------------------- */
void MCU_Initial(void);
void RF_Initial(uint8_t addr, uint16_t sync, uint8_t mode);
void System_Initial(void);
void RF_SendPacket(uint8_t index);
uint8_t RF_RecvHandler(void);
void MMA7361L_ReadHandler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
