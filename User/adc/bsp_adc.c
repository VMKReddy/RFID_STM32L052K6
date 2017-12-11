/**
  ******************************************************************************
  * @file    bsp_bsp_adc.c
  * @author  phoenix
  * @version V1.0
  * @date    10-November-2017
  * @brief   adc driver
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */ 
#include "./adc/bsp_adc.h"

uint16_t ADC_ConvertedValue[MMA7361L_NOFCHANEL];
DMA_HandleTypeDef DMA_Init_Handle;
ADC_HandleTypeDef ADC_Handle;
ADC_ChannelConfTypeDef ADC_Config;

void MMA7361L_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// ʹ�� GPIO ʱ��
	MMA7361L_SL_GPIO_CLK_ENABLE();
	MMA7361L_GS_GPIO_CLK_ENABLE();
	MMA7361L_ADC1_GPIO_CLK_ENABLE();
	MMA7361L_ADC2_GPIO_CLK_ENABLE();
	MMA7361L_ADC3_GPIO_CLK_ENABLE();
		
	// ���� IO
	GPIO_InitStructure.Pin = MMA7361L_ADC1_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
  GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
	HAL_GPIO_Init(MMA7361L_ADC1_GPIO_PORT, &GPIO_InitStructure);

	// ���� IO
	GPIO_InitStructure.Pin = MMA7361L_ADC2_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
  GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
	HAL_GPIO_Init(MMA7361L_ADC2_GPIO_PORT, &GPIO_InitStructure);
	
	// ���� IO
	GPIO_InitStructure.Pin = MMA7361L_ADC3_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
  GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
	HAL_GPIO_Init(MMA7361L_ADC3_GPIO_PORT, &GPIO_InitStructure);

	// ���� IO
	GPIO_InitStructure.Pin = MMA7361L_SL_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(MMA7361L_SL_GPIO_PORT, &GPIO_InitStructure);
	
	// ���� IO
	GPIO_InitStructure.Pin = MMA7361L_GS_GPIO_PIN;
	HAL_GPIO_Init(MMA7361L_GS_GPIO_PORT, &GPIO_InitStructure);
}

void MMA7361L_ADC_Mode_Config(void)
{
  // ------------------DMA Init �ṹ����� ��ʼ��--------------------------
  // ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
  /* Enable DMA1 clock */
  MMA7361L_ADC_DMA_CLK_ENABLE(); 
	// ���ݴ���ͨ��
	DMA_Init_Handle.Instance = MMA7361L_ADC_DMA_CHANNEL;	
  // ���ݴ��䷽��Ϊ���赽�洢��
	DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
  // �洢����ַ�̶�
	DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE; 
  // �������ݴ�СΪ���֣��������ֽ�
	DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; 
  // �洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;	
	// ѭ������ģʽ
	DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;
  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_Init_Handle.Init.Priority = DMA_PRIORITY_HIGH;

	DMA_Init_Handle.Init.Request = DMA_REQUEST_0;
  /* Deinitialize  & Initialize the DMA for new transfer */
	HAL_DMA_DeInit(&DMA_Init_Handle);
	HAL_DMA_Init(&DMA_Init_Handle);
	
	HAL_DMA_Start(&DMA_Init_Handle,MMA7361L_ADC_DR_ADDR,(uint32_t)&ADC_ConvertedValue,MMA7361L_NOFCHANEL);
	
//	/* NVIC configuration for DMA Input data interrupt */
//  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
//  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	
	// ����ADCʱ��
	MMA7361L_ADC_CLK_ENABLE();
	
  /* ### - 1 - Initialize ADC peripheral #################################### */
  /*
   *  Instance                  = ADC1.
   *  OversamplingMode          = Disabled
   *  ClockPrescaler            = PCLK clock with no division.
   *  LowPowerAutoPowerOff      = Disabled (For this exemple continuous mode is enabled with software start)
   *  LowPowerFrequencyMode     = Enabled (To be enabled only if ADC clock is lower than 2.8MHz) 
   *  LowPowerAutoWait          = Disabled (New conversion starts only when the previous conversion is completed)       
   *  Resolution                = 12 bit (increased to 16 bit with oversampler)
   *  SamplingTime              = 3.5 cycles od ADC clock.
   *  ScanConvMode              = Forward 
   *  DataAlign                 = Right
   *  ContinuousConvMode        = Enabled
   *  DiscontinuousConvMode     = Enabled
   *  ExternalTrigConvEdge      = None (Software start)
   *  EOCSelection              = End Of Conversion event
   *  DMAContinuousRequests     = ENABLE
   */
	ADC_Handle.Instance = MMA7361L_ADC;
	ADC_Handle.Init.OversamplingMode = DISABLE;
  ADC_Handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	ADC_Handle.Init.LowPowerAutoPowerOff = DISABLE;
  ADC_Handle.Init.LowPowerFrequencyMode = ENABLE;
  ADC_Handle.Init.LowPowerAutoWait = DISABLE;
  ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
	ADC_Handle.Init.SamplingTime = ADC_SAMPLETIME_3CYCLES_5;
  ADC_Handle.Init.ScanConvMode = ADC_SCAN_ENABLE; 
  ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  ADC_Handle.Init.ContinuousConvMode = ENABLE;
	ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
  ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  // �ⲿ����ͨ����������ʹ�������������ֵ��㸳ֵ����
  //ADC_Handle1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	//ʹ������ת������
	ADC_Handle.Init.DMAContinuousRequests = ENABLE;
	//ת����ɱ�־
	ADC_Handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;    
	/* Initialize ADC peripheral according to the passed parameters */                        
	if(HAL_ADC_Init(&ADC_Handle) != HAL_OK)
	{
    Error_Handler();
  }
  /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&ADC_Handle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    Error_Handler();
  }	
  /* ### - 3 - Channel configuration ######################################## */
  /* Select Channel 5 to be converted */
  ADC_Config.Channel = MMA7361L_ADC1_CHANNEL;
	ADC_Config.Rank = ADC_RANK_CHANNEL_NUMBER; 	
  if (HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
  {
    Error_Handler();
  }
	/* Select Channel 6 to be converted */
  ADC_Config.Channel = MMA7361L_ADC2_CHANNEL;
	ADC_Config.Rank = ADC_RANK_CHANNEL_NUMBER; 	
  if (HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
  {
    Error_Handler();
  }
	/* Select Channel 7 to be converted */
  ADC_Config.Channel = MMA7361L_ADC3_CHANNEL;
	ADC_Config.Rank = ADC_RANK_CHANNEL_NUMBER; 	
  if (HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
  {
    Error_Handler();
  }
  /* ### - 4 - Start conversion in DMA mode ################################# */	
	if(HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*)&ADC_ConvertedValue,1) != HAL_OK)
  {
    Error_Handler();
  }
}


void MMA7361L_Config(void)
{
	MMA7361L_ADC_GPIO_Config();
	MMA7361L_ADC_Mode_Config();
}



