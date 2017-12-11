/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L0xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*===========================================================================
* ���� :MCU_Initial() => ��ʼ��CPU����Ӳ��																	*
* ˵�� ��������Ӳ���ĳ�ʼ���������Ѿ�������C��,��bsp.c�ļ�									*
============================================================================*/
void MCU_Initial(void)
{ 
    Debug_USART_Config();
    GPIO_Config();
    TIM_Config();
    SPI_Config();
		MMA7361L_Config();
}

/*===========================================================================
* ���� :RF_Initial() => ��ʼ��RFоƬ																				*
* ���� :mode, =0,����ģʽ, else,����ģʽ																		*
* ˵�� :CC1101�Ĳ���,�Ѿ�������C��,��CC1101.c�ļ�,�ṩSPI��CSN����					*
				���ɵ������ڲ����к����û������ٹ���CC1101�ļĴ�����������					*
============================================================================*/
void RF_Initial(uint8_t addr, uint16_t sync, uint8_t mode)
{
	CC1101Init(addr, sync);                       			// ��ʼ��CC1101�Ĵ���
	if(mode == RX)				{CC1101SetTRMode(RX_MODE);}		// ����ģʽ
	else if(mode == TX)		{CC1101SetTRMode(TX_MODE);}   // ����ģʽ
	else
	{
		CC1101SetIdle();																	// ����ģʽ����ת��sleep״̬
		CC1101WORInit();																	// ��ʼ����Ų������
		CC1101SetWORMode();
	}
}

/*===========================================================================
* ����: System_Initial() => ��ʼ��ϵͳ��������                              *
============================================================================*/
void System_Initial(void)
{
    MCU_Initial();      // ��ʼ��CPU����Ӳ��
    RF_Initial(0x5, 0xD391, IDLE);     // ��ʼ������оƬ������ģʽ       
}

/*===========================================================================
* ���� : RF_SendPacket() => ���߷������ݺ���                            *
* ���� :Sendbufferָ������͵����ݣ�length���������ֽ���                     *
* ��� :0,����ʧ��;1,���ͳɹ�                                       *
============================================================================*/
uint8_t RF_SendPacket(void)
{

	uint8_t i=0;	
	
//	for (i=0; i<SEND_LENGTH; i++) // clear array
//		{SendBuffer[i] = 0;}	
//	printf("please write down what you want to say\r\n");
//	scanf("%[^\n]",SendBuffer);
//	printf("%s\n", SendBuffer);
		
	for(i=0; i<SendCnt; i++)
	{
		SendBuffer[0] = i;
		CC1101SendPacket(SendBuffer, SEND_LENGTH, ADDRESS_CHECK);    // ��������

		CC1101SetTRMode(RX_MODE);       // �������ģʽ���ȴ�Ӧ��
		
		Delay(0xFFFF);									// ����õ�ƽ��27ms����һ������
//		Delay(0xFFFFF);									// ����õ�ƽ��130ms����һ������
	}

	Usart_SendString(&UartHandle, (uint8_t *)"Transmit OK\r\n");
	RF_Initial(0x5, 0xD391, RX);
	RecvWaitTime = RECV_TIMEOUT;
	while(RF_Acknowledge() == 0 && RecvFlag == 0);
	RecvWaitTime = 0;
	CC1101SetIdle();																	// ����ģʽ����ת��sleep״̬
	CC1101WORInit();																	// ��ʼ����Ų������
	CC1101SetWORMode();
	//i=CC1101ReadStatus(CC1101_TXBYTES);//for test, TX status
	return(1);
}

/*===========================================================================
* ���� :Get_Address() => �������ݰ���ַ��ͬ��                             * 
============================================================================*/
void Get_Address(void)
{   
//		int chip_address;
//		unsigned int	sync_word;
	
		CC1101SetIdle();			//�˳�WORģʽ
		Delay(0x3FFFF);
//		temp2 = CC1101ReadStatus(CC1101_MARCSTATE);//for test, right is IDLE status
//		printf("state is %x\n",temp2);
	
//		printf("set receive chip address in package\r\n");
//    scanf("%d",&chip_address);
//		printf("%x\n", chip_address);
//		getchar();																// �ų��س�
//	
//		printf("set receive chip sync word in package\r\n");
//    scanf("%x",&sync_word);
//		printf("%x\n", sync_word);
//		getchar();																// �ų��س�
		RF_Initial(0x5, 0xD391, RX);     // ��ʼ������оƬ
}

/*===========================================================================
* ���� :RF_Acknowledge() => �������ݽ���Ӧ��                               * 
============================================================================*/
uint8_t	RF_Acknowledge(void)
{
	uint8_t i=0, k, length=0, rec_buffer[ACK_LENGTH]={0};
	int16_t rssi_dBm;

//	CC1101SetTRMode(RX_MODE);           // ����RFоƬ����ģʽ����������
//	temp2 = CC1101ReadStatus(CC1101_MARCSTATE);//for test, TX status
//	printf("state is %x\n",temp2);
	
	if(CC1101_IRQ_READ() == 0)         // �������ģ���Ƿ���������ж� 
	{
		while (CC1101_IRQ_READ() == 0);
		for (i=0; i<ACK_LENGTH; i++)   { rec_buffer[i] = 0; } // clear array

		// ��ȡ���յ������ݳ��Ⱥ���������
		length = CC1101RecPacket(rec_buffer, &Chip_Addr, &RSSI);
		// ��ӡ����
		if(length == 0)
		{
			printf("receive error or Address Filtering fail\n");
			return 0;
		}
		else
		{
			rssi_dBm = CC1101CalcRSSI_dBm(RSSI);
			printf("RSSI = %ddBm, length = %d, address = %d\n",rssi_dBm,length,Chip_Addr);
			for(k=0; k<ACK_LENGTH; k++)
			{	
				printf("%x ",rec_buffer[k]);
			}
			printf("\n");
			for(k=0; k<ACK_CNT; k++)
			{
				ADC_ConvertedValueLocal[0] =(float)((((uint16_t)rec_buffer[k*6+0]) + (0x0F00 & (((uint16_t)rec_buffer[k*6+1])<<8)))*3.3/4096); 
				ADC_ConvertedValueLocal[1] =(float)((((uint16_t)rec_buffer[k*6+2]) + (0x0F00 & (((uint16_t)rec_buffer[k*6+3])<<8)))*3.3/4096); 
				ADC_ConvertedValueLocal[2] =(float)((((uint16_t)rec_buffer[k*6+4]) + (0x0F00 & (((uint16_t)rec_buffer[k*6+5])<<8)))*3.3/4096);  
				printf("The current ADC1 value = %f V \r\n", ADC_ConvertedValueLocal[0]); 
				printf("The current ADC2 value = %f V \r\n", ADC_ConvertedValueLocal[1]);
				printf("The current ADC3 value = %f V \r\n", ADC_ConvertedValueLocal[2]);
			}
			return 1;
		}
	}
	else	{return 0;}
}

/*===========================================================================
* ���� :RF_RecvHandler() => �������ݽ��մ���                               * 
============================================================================*/
uint8_t RF_RecvHandler(void)
{
	uint8_t i=0, length=0, recv_buffer[SEND_LENGTH]={0};
	int16_t rssi_dBm;
	
	//CC1101ReadStatus(CC1101_RXBYTES);//for test, TX status

	//CC1101SetTRMode(RX_MODE);           // ����RFоƬ����ģʽ����������
	
//	if(CC1101_GDO2_READ() == 0)
//	{
//		while (CC1101_GDO2_READ() == 0){};
//		LED5_Red_TOG();
//	}
	
	if(CC1101_IRQ_READ() == 0)         // �������ģ���Ƿ���������ж� 
		{
			printf("interrupt occur\n");
			while (CC1101_IRQ_READ() == 0);
			for (i=0; i<SEND_LENGTH; i++)   { recv_buffer[i] = 0; } // clear array
            
			// rssi_p = CC1101ReadRSSI();
			// printf("RSSI_p = %ddBm ",rssi_p);
			// ��ȡ���յ������ݳ��Ⱥ���������
			length = CC1101RecPacket(recv_buffer, &Chip_Addr, &RSSI);
			// ��ӡ����
			if(length == 0)
				{
					printf("receive error or Address Filtering fail\n");
					return 0;
				}
			else
				{
					rssi_dBm = CC1101CalcRSSI_dBm(RSSI);
					printf("RSSI = %ddBm, length = %d, address = %d, number = %d\n%s\n",rssi_dBm,length,Chip_Addr,recv_buffer[0],recv_buffer);
					return 1;
				}

//			temp2 = CC1101ReadStatus(CC1101_WORTIME1);//for test, TX status
//			printf("WORTIME1 is %x\n",temp2);
//			temp2 = CC1101ReadStatus(CC1101_WORTIME0);//for test, TX status
//			printf("WORTIME0 is %x\n",temp2);
		}
	else	{return 0;}
}

/*===========================================================================
* ���� :RF_Reply() => �������ݽ��ն˻ظ�                              * 
============================================================================*/
void RF_Reply(void)
{
			uint8_t i;
			for(i=0; i<RecvCnt; i++)
			{
				Delay(0xFFFFF);
				AckBuffer[3] = ((0x0F & AckBuffer[3]) + (0xF0 & ((uint8_t)(CollectCnt-1)<<4))); // ���ص�ǰ��ѹֵ���ݰ����������ݵı��
				CC1101SendPacket(AckBuffer, ACK_LENGTH, ADDRESS_CHECK);    // ��������
			}
			CC1101SetIdle();																	// ����ģʽ����ת��sleep״̬
			CC1101WORInit();																	// ��ʼ����Ų������
			CC1101SetWORMode();
}

/*===========================================================================
* ���� :MMA7361L_display() => ��ӡMMA7361L������                              * 
============================================================================*/
//void MMA7361L_display(void)
//{
//	Delay(0xffffee);
//	if(temp2 < 10)
//	{
//		MMA7361L_GS_1G5();
//		MMA7361L_SL_OFF();
//		Delay(0xFF);
//		ADC_ConvertedValueLocal[0] =(float)((uint16_t)ADC_ConvertedValue[0]*3.3/4096); 
//		ADC_ConvertedValueLocal[1] =(float)((uint16_t)ADC_ConvertedValue[1]*3.3/4096);
//		ADC_ConvertedValueLocal[2] =(float)((uint16_t)ADC_ConvertedValue[2]*3.3/4096);  
//    
//		printf("The current AD1 value = 0x%08X \r\n", ADC_ConvertedValue[0]); 
//		printf("The current AD2 value = 0x%08X \r\n", ADC_ConvertedValue[1]);
//		printf("The current AD3 value = 0x%08X \r\n", ADC_ConvertedValue[2]);   
//    
//		printf("The current ADC1 value = %f V \r\n", ADC_ConvertedValueLocal[0]); 
//		printf("The current ADC2 value = %f V \r\n", ADC_ConvertedValueLocal[1]);
//		printf("The current ADC2 value = %f V \r\n", ADC_ConvertedValueLocal[2]);
//		temp2++;
//	}
//	else if(temp2 < 20)
//	{
//		MMA7361L_GS_6G();
//		MMA7361L_SL_OFF();
//		Delay(0xFF);
//		ADC_ConvertedValueLocal[0] =(float)((uint16_t)ADC_ConvertedValue[0]*3.3/4096); 
//		ADC_ConvertedValueLocal[1] =(float)((uint16_t)ADC_ConvertedValue[1]*3.3/4096);
//		ADC_ConvertedValueLocal[2] =(float)((uint16_t)ADC_ConvertedValue[2]*3.3/4096);  
//    
//		printf("The current AD1 value = 0x%08X \r\n", ADC_ConvertedValue[0]); 
//		printf("The current AD2 value = 0x%08X \r\n", ADC_ConvertedValue[1]);
//		printf("The current AD3 value = 0x%08X \r\n", ADC_ConvertedValue[2]);   
//    
//		printf("The current ADC1 value = %f V \r\n", ADC_ConvertedValueLocal[0]); 
//		printf("The current ADC2 value = %f V \r\n", ADC_ConvertedValueLocal[1]);
//		printf("The current ADC2 value = %f V \r\n", ADC_ConvertedValueLocal[2]);
//		temp2++;
//	}
//	else if(temp2 < 30)
//	{
//		MMA7361L_SL_ON();
//		Delay(0xFF);
//		ADC_ConvertedValueLocal[0] =(float)((uint16_t)ADC_ConvertedValue[0]*3.3/4096); 
//		ADC_ConvertedValueLocal[1] =(float)((uint16_t)ADC_ConvertedValue[1]*3.3/4096);
//		ADC_ConvertedValueLocal[2] =(float)((uint16_t)ADC_ConvertedValue[2]*3.3/4096);  
//    
//		printf("The current AD1 value = 0x%08X \r\n", ADC_ConvertedValue[0]); 
//		printf("The current AD2 value = 0x%08X \r\n", ADC_ConvertedValue[1]);
//		printf("The current AD3 value = 0x%08X \r\n", ADC_ConvertedValue[2]);   
//    
//		printf("The current ADC1 value = %f V \r\n", ADC_ConvertedValueLocal[0]); 
//		printf("The current ADC2 value = %f V \r\n", ADC_ConvertedValueLocal[1]);
//		printf("The current ADC2 value = %f V \r\n", ADC_ConvertedValueLocal[2]);
//		temp2++;
//	}
//	else
//	{
//		temp2 = 0;
//	}
//}

/*===========================================================================
* ���� : main() => ���������������                                         *
* ˵�� : ����һ�����ݣ����ȴ�Ӧ������															          *
				 ����һ�����ݣ�������Ӧ������																				*
============================================================================*/
int main(void)
{

  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock to have a frequency of 2 MHz (Up to 32MHZ possible) */
  SystemClock_Config();
	System_Initial();
	Show_Message();
	
	while(1)
		{
			if(RF_RecvHandler() == 1)   // �������ݽ��մ���
			{
				printf("receive succeed!!\r\n");
				RF_Reply();
			}
//					MMA7361L_display();
		}
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(RecvWaitTime != 0 && RecvWaitTime != 1)	// ���ݽ��ռ�ʱ
		{	RecvWaitTime--;}
	else if(RecvWaitTime == 1)
		{	RecvFlag=1;}
	else
		{	RecvFlag=0;}
        
	if(SendTime != 0)                           // 1msʱ�䵽����λSendFlag��־����������ѯ��������    
		{ 
			if(--SendTime == 0)    
				{   SendTime=SEND_GAP; 
						SendFlag=1;
						LED_Red_TOG();
						MMA7361L_ReadHandler();
				}
		}
}

void MMA7361L_ReadHandler(void)
{
	MMA7361L_GS_1G5();
	MMA7361L_SL_OFF();
	Delay(0x3FFF);
	if(CollectCnt < ACK_CNT)
		{
			AckBuffer[CollectCnt*6] = 0xFF & ADC_ConvertedValue[0];
			AckBuffer[CollectCnt*6 + 1] = ((0x0F & (ADC_ConvertedValue[0]>>8)) + (0xF0 & ((uint8_t)CollectCnt<<4)));
			AckBuffer[CollectCnt*6 + 2] = 0xFF & ADC_ConvertedValue[1];
			AckBuffer[CollectCnt*6 + 3] = 0xFF & (ADC_ConvertedValue[1]>>8);
			AckBuffer[CollectCnt*6 + 4] = 0xFF & ADC_ConvertedValue[2];
			AckBuffer[CollectCnt*6 + 5] = 0xFF & (ADC_ConvertedValue[2]>>8);
		}
		
//	printf("The current AD1 value = 0x%08X \r\n", ((uint16_t)(AckBuffer[CollectCnt*6 + 1])<<8) + (uint16_t)AckBuffer[CollectCnt*6]); 
//	printf("The current AD2 value = 0x%08X \r\n", ((uint16_t)(AckBuffer[CollectCnt*6 + 3])<<8) + (uint16_t)AckBuffer[CollectCnt*6 + 2]);
//	printf("The current AD3 value = 0x%08X \r\n", ((uint16_t)(AckBuffer[CollectCnt*6 + 5])<<8) + (uint16_t)AckBuffer[CollectCnt*6 + 4]);   
   
	MMA7361L_SL_ON();
	CollectCnt++;
//	printf("CollectCnt = %d\r\n", CollectCnt-1);
	if(CollectCnt == ACK_CNT)
		{
			CollectCnt = 0;
//			for(i=0; i<ACK_LENGTH; i++) // clear array
//				{AckBuffer[i] = 0;}	
		}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue=0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
static void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

/**
  * @brief  ��ӡָ��������ʾ��Ϣ
  * @param  none
  * @retval none
  */
static void Show_Message(void)
{   
	printf("\r\n CC1101 chip transfer performance test program \n");
	printf(" using USART3,configuration:%d 8-N-1 \n",DEBUG_USART_BAUDRATE);
	printf(" you need press USER button when you want transfer data\r\n");
	printf(" if choose transfer,the data must not exceed 60 bytes!!\r\n");
	printf(" PS: green led light when system in transfer mode\r\n");    
	printf("     orange led light when system in receive mode\r\n");
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  LED_Red_ON();
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
