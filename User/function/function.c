/* Includes ------------------------------------------------------------------*/
#include "./function/function.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t CollectCnt;      				// ����MMA7361L����
uint8_t	Chip_Addr	= 0;					// cc1101��ַ
uint8_t	RSSI = 0;								// RSSIֵ
uint8_t SendBuffer[SEND_LENGTH] = {0};// �������ݰ�
uint8_t RecvBuffer[RECV_LENGTH] = {0};// �������ݰ�
float ADC_ConvertedValueLocal[MMA7361L_NOFCHANEL];// ���ڱ���ת�������ĵ�ѹֵ
/* Private function prototypes -----------------------------------------------*/
extern void Delay(__IO uint32_t nCount);
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
* ���� :RF_RecvHandler() => �������ݽ��մ���                               * 
============================================================================*/
uint8_t RF_RecvHandler(void)
{
	uint8_t i=0, length=0;
	int16_t rssi_dBm;
	
	if(CC1101_IRQ_READ() == 0)         // �������ģ���Ƿ���������ж� 
		{
			printf("interrupt occur\n");
			while (CC1101_IRQ_READ() == 0);
			for (i=0; i<RECV_LENGTH; i++)   { RecvBuffer[i] = 0; } // clear array
			
			// ��ȡ���յ������ݳ��Ⱥ���������
			length = CC1101RecPacket(RecvBuffer, &Chip_Addr, &RSSI);
			// ��ӡ����
			rssi_dBm = CC1101CalcRSSI_dBm(RSSI);
			printf("RSSI = %ddBm, length = %d, address = %d\n",rssi_dBm,length,Chip_Addr);
			for(i=0; i<RECV_LENGTH; i++)
			{	
				printf("%x ",RecvBuffer[i]);
			}
			if(length == 0)
				{
					printf("receive error or Address Filtering fail\n");
					return 1;
				}
			else
				{
					if(RecvBuffer[0] == 0xAB && RecvBuffer[1] == 0xCD)
					{
						if(RecvBuffer[4] == 0xB0 && RecvBuffer[5] == 0xB0)
						{return 4;}
						else if(RecvBuffer[4] == 0xB3 && RecvBuffer[5] == 0xB3)
						{return 5;}
						else if(RecvBuffer[4] == 0xB4 && RecvBuffer[5] == 0xB4)
						{return 6;}
						else
						{	printf("receive function order error\r\n");
							return 3;}
					}
					else
					{	printf("receive package beginning error\r\n");
						return 2;}
				}
		}
	else	{return 0;}
}

/*===========================================================================
* ���� : RF_SendPacket() => ���߷������ݺ���                            *
* ���� :Sendbufferָ������͵����ݣ�length���������ֽ���                     *
* ��� :0,����ʧ��;1,���ͳɹ�                                       *
============================================================================*/
void RF_SendPacket(uint8_t index)
{
	uint8_t i=0;	
		
	if(index == 4)
	{
		SendBuffer[0] = 0xAB;
		SendBuffer[1] = 0xCD;
		SendBuffer[2] = 0xB0;
		SendBuffer[3] = 0xB0;
		SendBuffer[4] = RecvBuffer[6];
		SendBuffer[5] = RecvBuffer[7];
		SendBuffer[6] = RecvBuffer[8];
		SendBuffer[7] = RecvBuffer[9];
		SendBuffer[14] = RSSI;
		for(i=0; i<SEND_PACKAGE_NUM; i++)
		{
			CC1101SendPacket(SendBuffer, SEND_LENGTH, ADDRESS_CHECK);    // ��������
			Delay(0xFFFF);									// ����õ�ƽ��27ms����һ������
//		Delay(0xFFFFF);									// ����õ�ƽ��130ms����һ������
		}
//		printf("send over\r\n");
	}
	else if(index == 5)
	{
		SendBuffer[0] = 0xAB;
		SendBuffer[1] = 0xCD;
		SendBuffer[2] = 0xC3;
		SendBuffer[3] = 0x01;
		SendBuffer[4] = RecvBuffer[6];
		SendBuffer[5] = RecvBuffer[7];
		SendBuffer[6] = RecvBuffer[8];
		SendBuffer[7] = RecvBuffer[9];
		SendBuffer[14] = RSSI;
		for(i=0; i<SEND_PACKAGE_NUM; i++)
		{
			CC1101SendPacket(SendBuffer, SEND_LENGTH, ADDRESS_CHECK);    // ��������
			Delay(0xFFFF);									// ����õ�ƽ��27ms����һ������
//		Delay(0xFFFFF);									// ����õ�ƽ��130ms����һ������
		}
	}
	else if(index == 6)
	{
		SendBuffer[0] = 0xAB;
		SendBuffer[1] = 0xCD;
		SendBuffer[2] = 0xB4;
		SendBuffer[3] = 0xB4;
		SendBuffer[4] = RecvBuffer[6];
		SendBuffer[5] = RecvBuffer[7];
		SendBuffer[6] = RecvBuffer[8];
		SendBuffer[7] = RecvBuffer[9];
		SendBuffer[14] = RSSI;
		for(i=0; i<SEND_PACKAGE_NUM; i++)
		{
			CC1101SendPacket(SendBuffer, SEND_LENGTH, ADDRESS_CHECK);    // ��������
			Delay(0xFFFF);									// ����õ�ƽ��27ms����һ������
//		Delay(0xFFFFF);									// ����õ�ƽ��130ms����һ������
		}
	}	
	else if(index == 1)
	{
		SendBuffer[0] = 0xAB;
		SendBuffer[1] = 0xCD;
		SendBuffer[2] = 0x01;
		SendBuffer[3] = 0x01;
		SendBuffer[4] = RecvBuffer[6];
		SendBuffer[5] = RecvBuffer[7];
		SendBuffer[6] = RecvBuffer[8];
		SendBuffer[7] = RecvBuffer[9];
		SendBuffer[14] = 0x01;
		for(i=0; i<SEND_PACKAGE_NUM; i++)
		{
			CC1101SendPacket(SendBuffer, SEND_LENGTH, ADDRESS_CHECK);    // ��������
			Delay(0xFFFF);									// ����õ�ƽ��27ms����һ������
//		Delay(0xFFFFF);									// ����õ�ƽ��130ms����һ������
		}
	}
	else if(index == 2)
	{
		SendBuffer[0] = 0xAB;
		SendBuffer[1] = 0xCD;
		SendBuffer[2] = 0x02;
		SendBuffer[3] = 0x02;
		SendBuffer[4] = RecvBuffer[6];
		SendBuffer[5] = RecvBuffer[7];
		SendBuffer[6] = RecvBuffer[8];
		SendBuffer[7] = RecvBuffer[9];
		SendBuffer[14] = 0x02;
		for(i=0; i<SEND_PACKAGE_NUM; i++)
		{
			CC1101SendPacket(SendBuffer, SEND_LENGTH, ADDRESS_CHECK);    // ��������
			Delay(0xFFFF);									// ����õ�ƽ��27ms����һ������
//		Delay(0xFFFFF);									// ����õ�ƽ��130ms����һ������
		}
	}
	else if(index == 3)
	{
		SendBuffer[0] = 0xAB;
		SendBuffer[1] = 0xCD;
		SendBuffer[2] = 0x03;
		SendBuffer[3] = 0x03;
		SendBuffer[4] = RecvBuffer[6];
		SendBuffer[5] = RecvBuffer[7];
		SendBuffer[6] = RecvBuffer[8];
		SendBuffer[7] = RecvBuffer[9];
		SendBuffer[14] = 0x03;
		for(i=0; i<SEND_PACKAGE_NUM; i++)
		{
			CC1101SendPacket(SendBuffer, SEND_LENGTH, ADDRESS_CHECK);    // ��������
			Delay(0xFFFF);									// ����õ�ƽ��27ms����һ������
//		Delay(0xFFFFF);									// ����õ�ƽ��130ms����һ������
		}
	}

//	Usart_SendString(&UartHandle, (uint8_t *)"Transmit OK\r\n");
	CC1101SetIdle();																	// ����ģʽ����ת��sleep״̬
	CC1101WORInit();																	// ��ʼ����Ų������
	CC1101SetWORMode();
}

/*===========================================================================
* ���� :MMA7361L_ReadHandler() => ��MMA731L�жϺ���                         * 
============================================================================*/
void MMA7361L_ReadHandler(void)
{
	MMA7361L_GS_1G5();
	MMA7361L_SL_OFF();
	Delay(0x3FFF);
	SendBuffer[8] = (uint8_t)(0xFF & ADC_ConvertedValue[0]);
	SendBuffer[9] = (uint8_t)(0x0F & (ADC_ConvertedValue[0]>>8));
	SendBuffer[10] = (uint8_t)(0xFF & ADC_ConvertedValue[1]);
	SendBuffer[11] = (uint8_t)(0xFF & (ADC_ConvertedValue[1]>>8));
	SendBuffer[12] = (uint8_t)(0xFF & ADC_ConvertedValue[2]);
	SendBuffer[13] = (uint8_t)(0xFF & (ADC_ConvertedValue[2]>>8));
  MMA7361L_SL_ON();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/