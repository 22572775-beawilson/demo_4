/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"lcd.h"
#include"math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc3;

DAC_HandleTypeDef hdac1;
DMA_HandleTypeDef hdma_dac1_ch1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC3_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_DAC1_Init(void);
/* USER CODE BEGIN PFP */
#define MEASURE_STATE 1
#define MENU_STATE 2
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void Convert_to_Ascii(uint8_t num, uint8_t *disp_uart[4]);

uint32_t LCD_UART_COMMAND(uint8_t c1,uint8_t c2);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//up until demo 3 is Devin Jefferies' work. HIS STUDENT NUMBER IS 21800456. From Demo 4 is Beatrix Wilson's own work. MY STUDENT NUMBER IS 22572775//
extern uint8_t button_pressed;
extern uint8_t time_pressM;
extern uint8_t timer_interrupt;

uint8_t StudentNUM[15]  = "@,22572775,!\n";
uint8_t Txdata[20];
uint8_t Tempdata[20];
uint8_t Rxdata[1];
uint8_t idx = 0;
uint8_t move;
uint8_t size;
uint8_t display_msg;
uint8_t send_measurment;
volatile uint8_t flag;
uint8_t i;

uint8_t countt = 0;
uint8_t countt2 = 0;

//menu flags
uint8_t run = 1;
uint8_t cols = 0;
uint8_t rows = 0;
uint8_t fromStart = 0;
uint8_t measmode;
uint8_t startup;
uint8_t DCVoltagelcd[4];
uint8_t ACVoltagelcd_pp[4];
uint8_t ACVoltagelcd_off[4];
uint8_t ACVoltagelcd_freq;
uint8_t setAmplitude;
uint8_t setOffset;
uint8_t frequencyflag;
uint8_t itsDC = 0;
uint8_t itsAC = 0;
uint8_t scrollflag;
uint8_t startupp = 1;
uint8_t currentflag;
//uint8_t start = 0;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if(Rxdata[0] != 10){

		Txdata[idx] = Rxdata[0];
		idx++;

	}
	else if (Rxdata[0] == 10){
		Txdata[idx] = Rxdata[0];
		idx = 0;
		flag = 1;
		move = 1;

	}

	HAL_UART_Receive_IT(&huart2,Rxdata, 1);

}



uint32_t sine_val_DAC[1000];

#define PI 3.1415926

void get_sineval (float amplitude,float offset)
{
	for (int i=0;i<1000;i++)
	{
		sine_val_DAC[i] = offset+(((sin(i*2*PI/1000) + 1)* amplitude/3.3 *(4096/2)));
	}
}

uint32_t Pulse_array[100];

float DutyCycle = 25; //35%
float Amplitude_pulse = 1; //1V
float Offset_pulse= 1.2; //1.2V

uint16_t DutyCycleinput[100];
uint16_t Amplitude_pulseinput[100];
uint16_t Offset_pulseinput[100];

void get_Pulse_array(float DutyC, float PulseAmp, float PulseOff) {

	for(countt = 0; countt < DutyC; countt++) {

		Pulse_array[countt] = (PulseAmp + PulseOff)*(4096)/3.3; //scale
	}

	for(countt = DutyC; countt< 100; countt++) {

		Pulse_array[countt] = (PulseOff)*(4096)/3.3;
	}
}


/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();
	MX_ADC3_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	MX_DAC1_Init();
	/* USER CODE BEGIN 2 */


	HAL_UART_Transmit(&huart2, StudentNUM, 13, 20);

	//	HAL_TIM_Base_Start(&htim2);

	HAL_UART_Receive_IT(&huart2, Rxdata, 1);


	GPIO_PinState pinstate;

	uint32_t tLast1 = 0;
	uint8_t buttonState1 = 0;

	GPIO_PinState pinstatel;

	uint32_t tLast1l = 0;
	uint8_t buttonState1l = 0;

	GPIO_PinState pinstater;

	uint32_t tLast1r = 0;
	uint8_t buttonState1r = 0;

	GPIO_PinState pinstateu;

	uint32_t tLast1u = 0;
	uint8_t buttonState1u = 0;

	GPIO_PinState pinstated;

	uint32_t tLast1d = 0;
	uint8_t buttonState1d = 0;

	uint16_t DC_value[100];
	uint16_t AC_value[100];
	uint8_t measurment_mode = 1;
	uint8_t menu_mode = 0;
	uint8_t DC_measure = 1;
	uint8_t DCidx;
	uint16_t AC_measure = 0;
	uint16_t ACidx;
	uint32_t DC_CS = 0;
	uint32_t AC_CS = 0;
	uint16_t DC_avg;
	uint16_t scaled_dc_avg;
	uint16_t AC_avg;
	uint16_t AC_max;
	uint16_t AC_pk_pk;
	uint32_t AC_freq;
	uint8_t DC_uart[4];
	uint8_t AC_uart_offset[4];
	uint8_t ACdisplayoffset[20];
	uint8_t DCdisplayoffset[20] ;
	uint8_t AC_uart_frequency[4];
	uint8_t AC_disp_frequency[20];
	uint8_t AC_uart_pk_pk[4];
	uint8_t AC_disp_pk_pk[20];
	uint8_t send_status[20];
	volatile uint8_t parameter_t;
	volatile uint8_t parameter_a;
	volatile uint8_t parameter_o;
	volatile uint8_t parameter_f;
	volatile uint8_t parameter_d;
	volatile uint8_t parameter_c;
	volatile uint8_t signal_d = 1;
	volatile uint8_t signal_s = 0;
	volatile uint8_t signal_p =0;

	uint8_t output = 0;
	uint8_t count = 0;
	uint16_t f1 = 0;
	uint16_t f2 = 0;
	HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_1);
	display_msg = 1;
	//int row = 0;
	//int col = 0;

	uint8_t offset_output_value[4];
	offset_output_value[0] = 1;
	offset_output_value[1] = 2;
	offset_output_value[2] = 0;
	offset_output_value[3] = 0;

	uint8_t amplitude_output_value[4];
	amplitude_output_value[0] = 1;
	amplitude_output_value[1] = 0;
	amplitude_output_value[2] = 0;
	amplitude_output_value[3] = 0;

	uint8_t frequency_output_value[4];
	frequency_output_value[0] = 1;
	frequency_output_value[1] = 0;
	frequency_output_value[2] = 0;
	frequency_output_value[3] = 0;

	uint8_t DC_offset_output_value[4];
	DC_offset_output_value[0] = 1;
	DC_offset_output_value[1] = 2;
	DC_offset_output_value[2] = 0;
	DC_offset_output_value[3] = 0;

	float DAC_amplitude_output = 1000;
	int DAC_amplitude_output_int = 1000;
	float DAC_offset_output = 1200;
	int DAC_offset_output_int = 1200;
	int DAC_frequency_output_int =1000;
	uint16_t DAC_frequency_output = 1000;
	float DAC_DC_offset = 1200;
	int DAC_DC_Offset_int = 1200;
	float DAC_DC_val;
	TIM2->ARR = (72e6/(DAC_frequency_output*1000)) - 1;
	uint8_t write_to_lcd = 0;
	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
	uint8_t com_to_lcd = 0;
	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
	uint16_t LCD_scroll_count;
	uint8_t command_1;
	uint8_t command_2;
	uint32_t send_com_lcd;


	/////
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0);
	// Lcd_PortType ports[] = { D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port };
	Lcd_PortType ports[] = { GPIOA, GPIOA, GPIOC, GPIOC };
	// Lcd_PinType pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};
	Lcd_PinType pins[] = {GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_6, GPIO_PIN_8};
	Lcd_HandleTypeDef lcd;
	// Lcd_create(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);
	lcd = Lcd_create(ports, pins, GPIOC, GPIO_PIN_4, GPIOB, GPIO_PIN_11, LCD_4_BIT_MODE);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{


		//middle
		pinstate = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
		if (buttonState1 == 0)
		{
			if ((pinstate == GPIO_PIN_RESET) && ((HAL_GetTick() - tLast1) >= 10))
			{
				// button pushed down
				buttonState1 = 1;
				tLast1 = HAL_GetTick();
			}
		}
		else if (buttonState1 == 1)
		{
			if ((pinstate == GPIO_PIN_SET) && ((HAL_GetTick() - tLast1) >= 10))
			{
				// button released
				run = 1;
				buttonState1 = 0;
				tLast1 = HAL_GetTick();
			}
		}
		//middle


		//down
		pinstated = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
		if (buttonState1d == 0)
		{
			if ((pinstated == GPIO_PIN_RESET) && ((HAL_GetTick() - tLast1d) >= 10))
			{
				// button pushed down
				buttonState1d = 1;
				tLast1d = HAL_GetTick();
			}
		}
		else if (buttonState1d == 1)
		{
			if ((pinstated == GPIO_PIN_SET) && ((HAL_GetTick() - tLast1d) >= 10))
			{
				// button released
				run = 1;
				//				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);

				buttonState1d = 0;
				tLast1d = HAL_GetTick();
			}
		}
		//down


		//up
		pinstateu = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
		if (buttonState1u == 0)
		{
			if ((pinstateu == GPIO_PIN_RESET) && ((HAL_GetTick() - tLast1u) >= 10))
			{
				// button pushed down
				buttonState1u = 1;
				tLast1u = HAL_GetTick();
			}
		}
		else if (buttonState1u == 1)
		{
			if ((pinstateu == GPIO_PIN_SET) && ((HAL_GetTick() - tLast1u) >= 10))
			{
				// button released
				run = 1;
				//				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
				buttonState1u = 0;
				tLast1u = HAL_GetTick();
			}
		}
		//up


		//left
		pinstatel = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
		if (buttonState1l == 0)
		{
			if ((pinstatel == GPIO_PIN_RESET) && ((HAL_GetTick() - tLast1l) >= 10))
			{
				// button pushed down
				buttonState1l = 1;
				tLast1l = HAL_GetTick();
			}
		}
		else if (buttonState1l == 1)
		{
			if ((pinstatel == GPIO_PIN_SET) && ((HAL_GetTick() - tLast1l) >= 10))
			{
				// button released
				run = 1;
				//				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
				buttonState1l = 0;
				tLast1l = HAL_GetTick();
			}
		}
		//left


		//right
		pinstater = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
		if (buttonState1r == 0)
		{
			if ((pinstater == GPIO_PIN_RESET) && ((HAL_GetTick() - tLast1r) >= 10))
			{
				// button pushed down
				buttonState1r = 1;
				tLast1r = HAL_GetTick();
			}
		}
		else if (buttonState1r == 1)
		{
			if ((pinstater == GPIO_PIN_SET) && ((HAL_GetTick() - tLast1r) >= 10))
			{
				// button released
				run = 1;
				//				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
				buttonState1r = 0;
				tLast1r = HAL_GetTick();
			}
		}
		//right


		///////////////////////////////////////////////////////////////MENU//////////////////////


		//readings
		if(rows == 0 && cols == 1)
		{

			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				run = 0;
			}
			if (buttonState1 && run)
			{
				Lcd_clear(&lcd);
				rows = 0;
				cols = 0;
				run = 0;
			}
			if (buttonState1l && run)
			{
				run = 0;
			}
			if (buttonState1r && run)
			{
				run = 0;
			}
			//			start = 1;
			//run = 0;
		}

		if (rows == 0 && cols == 0 && startupp == 1) {
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Start Message");
			startupp = 0;
		}

		//startmsg default//////////////////////////////////////////////////////////
		if(rows == 0 && cols == 0)
		{
			if (buttonState1d && run)
			{
				Lcd_clear(&lcd);
				rows = 1;
				cols = 0;
				run = 0;
			}
			if (buttonState1u && run)
			{
				run = 0;
			}

			if (buttonState1 && run)
			{
				Lcd_clear(&lcd);
				rows = 0;
				cols = 1;
				run = 0;
			}
			if (buttonState1l && run)
			{
				run = 0;
			}
			if (buttonState1r && run)
			{
				run = 0;
			}

		}


		//measure mode//////////////////////////////////////////////////////////
		if (rows == 1 && cols == 0)
		{
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);

				rows = 0;
				cols = 0;
				run = 0;
			}
			if (buttonState1 && run)
			{
				run = 0;
			}
			if (buttonState1l && run)
			{
				run = 0;
			}

			if (buttonState1r && run == 1)
			{
				Lcd_clear(&lcd);
				rows = 1;
				cols = 1;
				run = 0;
			}
			if (buttonState1d && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 0;
				run = 0;
				DC_measure = 1;
				AC_measure = 0;
				//////////////////////////////////////////////////
				//				timer_interrupt = 1;
				//				measurment_mode = 1;
			}
		}


		//sig gen mode//////////////////////////////////////////////////////////////////////////////////////////////
		if (rows == 1 && cols == 1)
		{
			if (buttonState1d && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 4;
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 0;
				cols = 0;
				run = 0;
			}
			if (buttonState1 && run)
			{
				run = 0;
			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 1;
				cols = 0;
				run = 0;
			}
			if (buttonState1r && run)
			{
				run = 0;
			}
		}


		//DCvoltage///////////////////////////////////////////////////////////////////
		if (rows == 2 && cols == 0)
		{
			//			DC_measure = 1;
			//			AC_measure = 0;
			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 1;
				cols = 0;
				run = 0;
			}
			if (buttonState1 && run)
			{
				run = 0;
				currentflag = 0;
				DC_measure = 1;
			}
			if (buttonState1l && run)
			{
				run = 0;
			}
			if (buttonState1r && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 1;
				run = 0;
			}
		}


		//DCcurrent///////////////////////////////////////////////////////////////////////
		if (rows == 2 && cols == 1)
		{
			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 1;
				cols = 0;
				run = 0;
			}
			if (buttonState1 && run)
			{
				run = 0;
				DC_measure = 0;
				AC_measure = 0;
				currentflag = 1;
			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 0;
				run = 0;
			}
			if (buttonState1r && run)
			{
				DC_measure = 0;
				AC_measure = 1;
				timer_interrupt = 1;
				measurment_mode = 1;
				Lcd_clear(&lcd);
				rows = 2;
				cols = 2;
				run = 0;
			}
		}


		//ACvoltage////////////////////////////////////////////////////////////////
		if (rows == 2 && cols == 2)
		{
			//			AC_measure = 1;
			//			timer_interrupt = 1;
			//			measurment_mode = 1;

			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 1;
				cols = 0;
				run = 0;
			}
			if (buttonState1 && run)
			{
				AC_measure = 1;
				run = 0;

			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 1;
				run = 0;
			}
			if (buttonState1r && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 3;
				run = 0;
			}
		}


		//ACcurrent//////////////////////////////////////////////////////////
		if (rows == 2 && cols == 3)
		{
			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 1;
				cols = 0;
				run = 0;
			}
			if (buttonState1 && run)
			{
				run = 0;
				//				AC_measure = 1;
				currentflag = 1;
				DC_measure = 0;
				AC_measure = 0;
			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 2;
				run = 0;
			}
			if (buttonState1r && run)
			{
				run = 0;
			}
		}


		//Type/////////////////////////////////////////////////////////////////
		if (rows == 2 && cols == 4)
		{
			if (buttonState1d && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 0;
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 1;
				cols = 0;
				run = 0;
			}
			if (buttonState1 && run)
			{
				run = 0;
			}
			if (buttonState1l && run)
			{
				run = 0;
			}
			if (buttonState1r && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 5;
				run = 0;
			}
		}


		//Parameter/////////////////////////////////////////////////////////
		if (rows == 2 && cols == 5)
		{
			if (buttonState1d && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 3;
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 1;
				cols = 0;
				run = 0;
			}
			if (buttonState1 && run)
			{
				run = 0;
			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 4;
				run = 0;
			}
			if (buttonState1r && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 6;
				run = 0;
			}
		}


		//Output/////////////////////////////////////////////////////////
		if (rows == 2 && cols == 6)
		{
			if (buttonState1d && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 7;
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 1;
				cols = 0;
				run = 0;
			}
			if (buttonState1 && run)
			{
				run = 0;
			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 5;
				run = 0;
			}
			if (buttonState1r && run)
			{
				run = 0;
			}
		}


		//Type DC////////////////////////////////////////////////
		if (rows == 3 && cols == 0)
		{
			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 4;
				run = 0;
			}
			if (buttonState1 && run)
			{
				itsDC = 1;
				signal_d = 1;
				signal_s = 0;
				signal_p = 0;
				run = 0;
			}
			if (buttonState1l && run)
			{
				run = 0;
			}
			if (buttonState1r && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 1;
				run = 0;
			}
		}


		//Type sinusoidal///////////////////////////////////////////////////
		if (rows == 3 && cols == 1)
		{
			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 4;
				run = 0;
			}
			if (buttonState1 && run)
			{
				itsAC = 1;
				signal_d = 0;
				signal_p = 0;
				signal_s = 1;
				run = 0;
			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 0;
				run = 0;
			}
			if (buttonState1r && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 2;
				run = 0;
			}
		}


		//Type pulse///////////////////////////////////////////////////////////////////
		if (rows == 3 && cols == 2)
		{
			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 4;
				run = 0;
			}
			if (buttonState1 && run)
			{
				signal_p = 1;
				signal_s = 0;
				signal_d = 0;
				run = 0;
			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 1;
				run = 0;
			}
			if (buttonState1r && run)
			{
				run = 0;
			}
		}


		//Param Amplitude///////////////////////////////////////////////////////////
		if (rows == 3 && cols == 3)
		{
			if (buttonState1d && run)
			{
				Lcd_clear(&lcd);
				rows = 4;
				cols = 0;
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 4;
				run = 0;
			}
			if (buttonState1 && run)
			{
				run = 0;
			}
			if (buttonState1l && run)
			{
				run = 0;
			}
			if (buttonState1r && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 4;
				run = 0;
			}
		}


		//Param Offset//////////////////////////////////////////////////////////////////////
		if (rows == 3 && cols == 4)
		{
			if (buttonState1d && run)
			{

				Lcd_clear(&lcd);
				rows = 4;
				cols = 1;

				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 4;
				run = 0;
			}
			if (buttonState1 && run)
			{
				//itsDC = 0;
				run = 0;
			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 3;
				run = 0;
			}
			if (buttonState1r && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 5;
				run = 0;
			}
		}


		//Param Freq////////////////////////////////////////////////////////////
		if (rows == 3 && cols == 5)
		{
			if (buttonState1d && run)
			{
				//if(output)
				//{
				Lcd_clear(&lcd);
				rows = 4;
				cols = 2;

				//}
				run = 0;

			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 4;
				run = 0;
			}
			if (buttonState1 && run)
			{
				run = 0;
			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 4;
				run = 0;
			}
			if (buttonState1r && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 6;
				run = 0;
			}
		}


		//Param Duty Cycle////////////////////////////////////////////////////////////
		if (rows == 3 && cols == 6)
		{
			if (buttonState1d && run)
			{
				Lcd_clear(&lcd);
				rows = 4;
				cols = 4;
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 4;
				run = 0;
			}
			if (buttonState1 && run)
			{
				run = 0;
			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 5;
				run = 0;
			}
			if (buttonState1r && run)
			{
				run = 0;
			}
		}


		//Output on///////////////////////////////////////////////////////////////////////
		if (rows == 3 && cols == 7)
		{
			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 4;
				run = 0;
			}
			if (buttonState1 && run)
			{
				output = 1;
				run = 0;
			}
			if (buttonState1l && run)
			{
				run = 0;
			}
			if (buttonState1r && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 8;

				run = 0;
			}
		}


		//Output off/////////////////////////////////////////////////////////////////
		if (rows == 3 && cols == 8)
		{
			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 2;
				cols = 4;
				run = 0;
			}
			if (buttonState1 && run)
			{
				output = 0;
				run = 0;
			}
			if (buttonState1l && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 7;
				run = 0;
			}
			if (buttonState1r && run)
			{
				run = 0;
			}
		}


		//parameter amplitude vals////////////////////////////////////////////////////
		if (rows == 4 && cols == 0)
		{
			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 3;
				run = 0;
			}
			if (buttonState1 && run)
			{
				setAmplitude = 1;
				signal_p = 0;
				signal_s = 1;
				signal_d = 0;
				run = 0;
			}
			if (buttonState1l && run)
			{
				DAC_amplitude_output_int = DAC_amplitude_output_int - 100;
				if(DAC_amplitude_output_int <= 0)
				{
					DAC_amplitude_output_int = 0;
				}
				amplitude_output_value[0] = ((DAC_amplitude_output_int/1000) % 10);
				amplitude_output_value[1] = ((DAC_amplitude_output_int/100) % 10);
				amplitude_output_value[2] = ((DAC_amplitude_output_int/10) % 10);
				amplitude_output_value[3] = ((DAC_amplitude_output_int) % 10);
				run = 0;
			}
			if (buttonState1r && run)
			{
				DAC_amplitude_output_int = DAC_amplitude_output_int + 100;
				if(DAC_amplitude_output_int >= 3100)
				{
					DAC_amplitude_output_int = 3100;
				}
				amplitude_output_value[0] = ((DAC_amplitude_output_int/1000) % 10);
				amplitude_output_value[1] = ((DAC_amplitude_output_int/100) % 10);
				amplitude_output_value[2] = ((DAC_amplitude_output_int/10) % 10);
				amplitude_output_value[3] = ((DAC_amplitude_output_int) % 10);
				run = 0;
			}
		}


		//parameter off values///////////////////////////////////////////////////////
		if (rows == 4 && cols == 1)
		{
			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 3;
				run = 0;
			}
			if (buttonState1 && run)
			{
				setOffset = 1;
				if (itsDC == 1) {
					signal_d = 1;
					signal_p = 0;
					signal_s = 0;
				}
				if (itsAC == 1) {
					signal_d = 0;
					signal_p = 0;
					signal_s = 1;
				}
				run = 0;
			}
			if (buttonState1l && run)
			{
				if (itsDC)
				{
					DAC_DC_Offset_int = DAC_DC_Offset_int - 100;
					if(DAC_DC_Offset_int <= 100)
					{
						DAC_DC_Offset_int = 100;
					}
					DC_offset_output_value[0] = ((DAC_DC_Offset_int/1000) % 10);
					DC_offset_output_value[1] = ((DAC_DC_Offset_int/100) % 10);
					DC_offset_output_value[2] = ((DAC_DC_Offset_int/10) % 10);
					DC_offset_output_value[3] = ((DAC_DC_Offset_int/1) % 10);
				}
				if (itsAC)
				{
					DAC_offset_output_int = DAC_offset_output_int - 100;
					if(DAC_offset_output_int <= 100)
					{
						DAC_offset_output_int = 100;
					}
					offset_output_value[0] = ((DAC_offset_output_int/1000) % 10);
					offset_output_value[1] = ((DAC_offset_output_int/100) % 10);
					offset_output_value[2] = ((DAC_offset_output_int/10) % 10);
					offset_output_value[3] = ((DAC_offset_output_int/1) % 10);
				}
				run = 0;
			}
			if (buttonState1r && run)
			{
				if (itsDC)
				{
					DAC_DC_Offset_int = DAC_DC_Offset_int + 100;
					if(DAC_DC_Offset_int >= 3200)
					{
						DAC_DC_Offset_int = 3200;
					}
					DC_offset_output_value[0] = ((DAC_DC_Offset_int/1000) % 10);
					DC_offset_output_value[1] = ((DAC_DC_Offset_int/100) % 10);
					DC_offset_output_value[2] = ((DAC_DC_Offset_int/10) % 10);
					DC_offset_output_value[3] = ((DAC_DC_Offset_int/1) % 10);
				}
				if (itsAC)
				{
					DAC_offset_output_int = DAC_offset_output_int + 100;
					if(DAC_offset_output_int >= 3200)
					{
						DAC_offset_output_int = 3200;
					}
					offset_output_value[0] = ((DAC_offset_output_int/1000) % 10);
					offset_output_value[1] = ((DAC_offset_output_int/100) % 10);
					offset_output_value[2] = ((DAC_offset_output_int/10) % 10);
					offset_output_value[3] = ((DAC_offset_output_int/1) % 10);
				}
				run = 0;
			}
		}


		//parameter frequency values////////////////////////////////////////////////////////////////////
		if (rows == 4 && cols == 2)
		{
			if (buttonState1d && run)
			{
				run = 0;
			}
			if (buttonState1u && run)
			{
				Lcd_clear(&lcd);
				rows = 3;
				cols = 3;
				run = 0;
			}
			if (buttonState1 && run)
			{
				frequencyflag = 1;
				signal_d = 0;
				signal_p = 0;
				signal_s = 1;
				run = 0;
			}
			if (buttonState1l && run)
			{
				DAC_frequency_output_int = DAC_frequency_output_int -100;
				if (DAC_frequency_output_int <= 0)
				{
					DAC_frequency_output_int = 0;
				}
				frequency_output_value[0] = ((DAC_frequency_output_int/1000)%10);
				frequency_output_value[1] = ((DAC_frequency_output_int/100)%10);
				frequency_output_value[2] = ((DAC_frequency_output_int/10)%10);
				frequency_output_value[3] = ((DAC_frequency_output_int/1)%10);
				run = 0;
			}
			if (buttonState1r && run)
			{
				DAC_frequency_output_int = DAC_frequency_output_int +100;
				if (DAC_frequency_output_int >= 5000)
				{
					DAC_frequency_output_int = 5000;
				}
				frequency_output_value[0] = ((DAC_frequency_output_int/1000)%10);
				frequency_output_value[1] = ((DAC_frequency_output_int/100)%10);
				frequency_output_value[2] = ((DAC_frequency_output_int/10)%10);
				frequency_output_value[3] = ((DAC_frequency_output_int/1)%10);
				run = 0;
			}
		}

		//duty cycle stuff//////////////////////////////////////////////////////

		if (rows == 4 && cols == 3) {
			if (buttonState1d ==1 && run == 1) {
				run = 0;
			}
			if (buttonState1u ==1 && run == 1) {

				rows = 3;
				cols = 3;
				run = 0;
			}
			if (buttonState1 ==1 && run == 1) {
				run = 0;
			}
			if (buttonState1r ==1 && run == 1) {
				run = 0;
			}
			if (buttonState1l ==1 && run == 1) {

				Lcd_clear(&lcd);
				rows = 4;
				cols = 2;
				run = 0;
			}
		}

//now LCD messages:

		if (cols == 0 && rows == 0)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Start Message");
		}
		if(cols == 0 && rows == 1)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Measure");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "Mode");
		}
		if(cols == 1 && rows == 1)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Sig gen");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "Mode");
		}

		if(cols == 0 && rows == 2)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "DC Voltage");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_int(&lcd, (DCdisplayoffset[6] - 48));
			Lcd_string(&lcd, "." );
			Lcd_int(&lcd, (DCdisplayoffset[7]-48));
			Lcd_int(&lcd, (DCdisplayoffset[8]-48));
			Lcd_int(&lcd, (DCdisplayoffset[9] - 48));
			Lcd_string(&lcd, "V");
		}

		if(cols == 1 && rows == 2)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "DC Current");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "-");
		}

		if(cols == 2 && rows == 2 && AC_measure==1 && timer_interrupt==1 && measurment_mode==1)
		{
			//DC_measure = 0;
			//AC_measure = 1;

			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "AC Voltage");
			Lcd_cursor(&lcd, 1, 0);

			LCD_scroll_count++;

			///// SEND TO LCD /////////
			//offset//
			//Lcd_clear(&lcd);
			Lcd_string(&lcd, "O:");
			Lcd_int(&lcd, (ACdisplayoffset[6]-48));
			Lcd_string(&lcd, "." );
			Lcd_int(&lcd, (ACdisplayoffset[7]-48));
			Lcd_int(&lcd, (ACdisplayoffset[8]-48));
			Lcd_int(&lcd, (ACdisplayoffset[9]-48));
			Lcd_string(&lcd, "V");
			Lcd_string(&lcd, ",");

			//Amplitude //
			Lcd_string(&lcd, "A:");
			Lcd_int(&lcd, (AC_disp_pk_pk[6]-48));
			Lcd_string(&lcd, "." );
			Lcd_int(&lcd, (AC_disp_pk_pk[7]-48));
			Lcd_int(&lcd, (AC_disp_pk_pk[8]-48));
			Lcd_int(&lcd, (AC_disp_pk_pk[9] -48));
			Lcd_string(&lcd, "V");
			Lcd_string(&lcd, ",");

			//Frequency//
			if(AC_freq<1000)
			{
				Lcd_string(&lcd, "0");
			}
			Lcd_string(&lcd, "F:");
			Lcd_int(&lcd, AC_freq);
			Lcd_string(&lcd, "Hz");
			if(LCD_scroll_count > 8){
				lcd_write_command(&lcd, 0x1A);
				LCD_scroll_count = 0;
			}

		}

		if(cols == 3 && rows == 2)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "AC Current");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "-");
		}

		if(cols == 4 && rows == 2)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Type");
			Lcd_cursor(&lcd, 1, 0);
			//			Lcd_string(&lcd, "-");
		}

		if(cols == 5 && rows == 2)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Parameter");
			Lcd_cursor(&lcd, 1, 0);
			//			Lcd_string(&lcd, "-");
		}

		if(cols == 6 && rows == 2)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Output");
			Lcd_cursor(&lcd, 1, 0);
			//			Lcd_string(&lcd, "-");
		}

		if(cols == 0 && rows == 3)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Type");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "DC");
		}

		if(cols == 1 && rows == 3)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Type");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "Sinusoidal");
		}

		if(cols == 2 && rows == 3)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Type");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "Pulse");
		}

		if(cols == 3 && rows == 3)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Amplitude");
			Lcd_cursor(&lcd, 1, 0);
			//			Lcd_string(&lcd, "-");
		}

		if(cols == 4 && rows == 3)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Offset");
			Lcd_cursor(&lcd, 1, 0);
			//			Lcd_string(&lcd, "-");
		}

		if(cols == 5 && rows == 3)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Frequency");
			Lcd_cursor(&lcd, 1, 0);
			//			Lcd_string(&lcd, "-");
		}

		if(cols == 6 && rows == 3)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Parameter");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "Duty Cycle");
		}

		if(cols == 7 && rows == 3)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Output");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "On");
		}

		if(cols == 8 && rows == 3)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Output");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "Off");
		}

		if(cols == 1 && rows == 0)
		{
			Lcd_cursor(&lcd, 0, 0);
			if(currentflag)
			{
				Lcd_string(&lcd, "None");
			}
			if (DC_measure) {

				Lcd_int(&lcd, (DCdisplayoffset[6] - 48));
				Lcd_string(&lcd, "." );
				Lcd_int(&lcd, (DCdisplayoffset[7]-48));
				Lcd_int(&lcd, (DCdisplayoffset[8]-48));
				Lcd_int(&lcd, (DCdisplayoffset[9] - 48));
				Lcd_string(&lcd, "V");
			}
			if (AC_measure) {
				LCD_scroll_count++;
				//				Lcd_cursor(&lcd, 0, 0);
				Lcd_string(&lcd, "O:");
				Lcd_int(&lcd, (ACdisplayoffset[6]-48));
				Lcd_string(&lcd, "." );
				Lcd_int(&lcd, (ACdisplayoffset[7]-48));
				Lcd_int(&lcd, (ACdisplayoffset[8]-48));
				Lcd_int(&lcd, (ACdisplayoffset[9]-48));
				Lcd_string(&lcd, "V");

				//Amplitude //
				Lcd_string(&lcd, "A:");
				Lcd_int(&lcd, (AC_disp_pk_pk[6]-48));
				Lcd_string(&lcd, "." );
				Lcd_int(&lcd, (AC_disp_pk_pk[7]-48));
				Lcd_int(&lcd, (AC_disp_pk_pk[8]-48));
				Lcd_int(&lcd, (AC_disp_pk_pk[9] -48));
				Lcd_string(&lcd, "V");

				//Frequency//
				if(AC_freq<1000)
				{
					Lcd_string(&lcd, "0");
				}
				Lcd_string(&lcd, "F:");
				Lcd_int(&lcd, AC_freq);
				Lcd_string(&lcd, "Hz");
				if(LCD_scroll_count > 8){
					lcd_write_command(&lcd, 0x1A);
					LCD_scroll_count = 0;
				}
			}
			Lcd_cursor(&lcd, 1, 0);
			if(output == 0)
			{
				Lcd_cursor(&lcd, 1, 0);
				Lcd_string(&lcd, "OUTPUT OFF");
			}
			if(output == 1)
			{
				if(itsDC)
				{
					scrollflag = 0;
					//Lcd_cursor(&lcd, 1, 0);
					Lcd_string(&lcd, "O:");
					Lcd_int(&lcd, DC_offset_output_value[0]);
					Lcd_string(&lcd, ".");
					Lcd_int(&lcd, DC_offset_output_value[1]);
					Lcd_int(&lcd, DC_offset_output_value[2]);
					Lcd_int(&lcd, DC_offset_output_value[3]);
					Lcd_string(&lcd, "V");
					itsDC = 0;
				}
				if(itsAC)
				{
					scrollflag = 1;
					//Lcd_cursor(&lcd, 1, 0);
					//										LCD_scroll_count++;

					Lcd_string(&lcd, "O:");
					Lcd_int(&lcd, DC_offset_output_value[0]);
					Lcd_string(&lcd, ".");
					Lcd_int(&lcd, DC_offset_output_value[1]);
					Lcd_int(&lcd, DC_offset_output_value[2]);
					Lcd_int(&lcd, DC_offset_output_value[3]);
					Lcd_string(&lcd, "V");
					Lcd_string(&lcd, ",");

					Lcd_string(&lcd, "A:");
					Lcd_int(&lcd, (amplitude_output_value[0]));
					Lcd_string(&lcd, "." );
					Lcd_int(&lcd, (amplitude_output_value[1]));
					Lcd_int(&lcd, (amplitude_output_value[2]));
					Lcd_int(&lcd, (amplitude_output_value[3]));
					Lcd_string(&lcd, "V");
					Lcd_string(&lcd, ",");

					Lcd_string(&lcd, "F:");
					Lcd_int(&lcd, (frequency_output_value[0]));
					Lcd_int(&lcd, (frequency_output_value[1]));
					Lcd_int(&lcd, (frequency_output_value[2]));
					Lcd_int(&lcd, (frequency_output_value[3]));

					Lcd_string(&lcd, "Hz");
					itsAC = 0;
				}

				if (signal_p == 1) {
					scrollflag = 1;
					Lcd_cursor(&lcd, 1, 0);
					Lcd_string(&lcd, "O:1200V;A:1000V;F:1000Hz;D:025%");
				}

				if(scrollflag)
				{
					LCD_scroll_count++;
					if(LCD_scroll_count > 8){
						lcd_write_command(&lcd, 0x1A);
						LCD_scroll_count = 0;
					}
				}

			}


		}

		if(cols == 0 && rows == 4)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Amplitude");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_int(&lcd, (amplitude_output_value[0]));
			//			Lcd_string(&lcd, "." );
			Lcd_int(&lcd, (amplitude_output_value[1]));
			Lcd_int(&lcd, (amplitude_output_value[2]));
			Lcd_int(&lcd, (amplitude_output_value[3]));
			Lcd_string(&lcd, "mV");
			if (setAmplitude == 1)
			{
				Lcd_string(&lcd, " ");
				Lcd_string(&lcd, "S");
				Lcd_string(&lcd, "e");
				Lcd_string(&lcd, "t");
				DAC_amplitude_output = (amplitude_output_value[0])*1000 + (amplitude_output_value[1])*100 + (amplitude_output_value[2])*10 + (amplitude_output_value[3]);
				setAmplitude = 0;
			}


		}
		if(cols == 1 && rows == 4)
		{
			//			Lcd_clear(&lcd);
			if (itsDC)
			{
				Lcd_cursor(&lcd, 0, 0);
				Lcd_string(&lcd, "Offset");
				Lcd_cursor(&lcd, 1, 0);
				Lcd_int(&lcd, DC_offset_output_value[0]);
				//				Lcd_string(&lcd, ".");
				Lcd_int(&lcd, DC_offset_output_value[1]);
				Lcd_int(&lcd, DC_offset_output_value[2]);
				Lcd_int(&lcd, DC_offset_output_value[3]);
				Lcd_string(&lcd, "mV");

				if (setOffset == 1)
				{
					//signal_d = 1;
					//					DCDACMENU = 1;
					Lcd_string(&lcd, " ");
					Lcd_string(&lcd, "S");
					Lcd_string(&lcd, "e");
					Lcd_string(&lcd, "t");
					//					DC_offset_output_value[0] = DC_offset_output_value[0] - 48;
					//					DC_offset_output_value[1] = DC_offset_output_value[1] - 48;
					//					DC_offset_output_value[2] = DC_offset_output_value[2] - 48;
					//					DC_offset_output_value[3] = DC_offset_output_value[3] - 48;
					DAC_DC_offset = DC_offset_output_value[0]*1000 + DC_offset_output_value[1]*100 + DC_offset_output_value[2]*10 + DC_offset_output_value[3];
					setOffset = 0;
				}
			}

			if (itsAC)
			{
				//				signal_s = 1;
				Lcd_cursor(&lcd, 0, 0);
				Lcd_string(&lcd, "Offset");
				Lcd_cursor(&lcd, 1, 0);
				Lcd_int(&lcd, (offset_output_value[0]));
				//				Lcd_string(&lcd, "." );
				Lcd_int(&lcd, (offset_output_value[1]));
				Lcd_int(&lcd, (offset_output_value[2]));
				Lcd_int(&lcd, (offset_output_value[3]));
				Lcd_string(&lcd, "mV");
				if (setOffset == 1)
				{
					Lcd_string(&lcd, " ");
					Lcd_string(&lcd, "S");
					Lcd_string(&lcd, "e");
					Lcd_string(&lcd, "t");
					DAC_offset_output = offset_output_value[0]*1000 + offset_output_value[1]*100 + offset_output_value[2]*10 + offset_output_value[3];
					setOffset = 0;

				}
			}

		}

		if(cols == 2 && rows == 4)
		{
			//			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Frequency");
			Lcd_cursor(&lcd, 1, 0);

			Lcd_int(&lcd, (frequency_output_value[0]));
			Lcd_int(&lcd, (frequency_output_value[1]));
			Lcd_int(&lcd, (frequency_output_value[2]));
			Lcd_int(&lcd, (frequency_output_value[3]));


			Lcd_string(&lcd, "Hz");
			if (frequencyflag == 1)
			{
				Lcd_string(&lcd, " ");
				Lcd_string(&lcd, "S");
				Lcd_string(&lcd, "e");
				Lcd_string(&lcd, "t");
				DAC_frequency_output = frequency_output_value[0]*1000 + frequency_output_value[1]*100 + frequency_output_value[2]*10 + frequency_output_value[3];

				frequencyflag = 0;
			}

		}

		if (rows == 4 && cols == 3) { //duty cycle default

			Lcd_cursor(&lcd, 0, 0);
			Lcd_string(&lcd, "Duty Cycle");
			Lcd_cursor(&lcd, 1, 0);
			Lcd_string(&lcd, "25%");

		}



		//////////////////////////LEDS//////////////////////////

		if(rows==0 && cols==1) //D2
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
		}

		if(rows==0 && cols==1) //D3
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);
		}

		if(currentflag) //D4
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 0);
		}

		if(output == 1) //D5
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
		}




		/////////////////////////




		////////////////////   UART Processing    /////////////////////
		if(move){
			for(i = 0; i < 21; i++){
				Tempdata[i] = Txdata[i];
				Txdata[i] = 0;
			}
			move = 0;
		}


		if (flag==1) {

			//if (signal_d == 1) {
			//	signal_p =0;
			//	signal_s = 0;
			//}
			if (Tempdata[2] == 35){
				/// Display byte to screen
				if(Tempdata[4] == 49){
					if(write_to_lcd == 0){
						Lcd_clear(&lcd);
					}
					uint8_t disp_sc;
					disp_sc = Tempdata[6];
					//HAL_UART_Transmit(&huart2,disp_sc,1, 50);
					lcd_write_data(&lcd, disp_sc);
					write_to_lcd = 1;
					menu_mode = 0;
					measurment_mode = 0;
					output = 0;
					com_to_lcd = 0;
				}
				else if(Tempdata[4] == 40){
					/// send command
					lcd_write_command(&lcd, Tempdata[6]);

					write_to_lcd = 0;
					com_to_lcd = 1;
				}

				flag = 0;
			}
			else if(Tempdata[2] != 35 && write_to_lcd ==1){
				Lcd_clear(&lcd);
				write_to_lcd = 0;
				com_to_lcd = 0;
			}



			else if(Tempdata[2] == 36){
				// Measurement mode (DAC)

				if((Tempdata[4] == 68) && (Tempdata[5] == 86)){
					// DC - Voltage
					DC_measure = 1;
					measurment_mode = 1;
					menu_mode = 0;
					parameter_o = 1;
					signal_d = 1;
					flag = 0;
					AC_measure = 0;
				}
				else if((Tempdata[4] == 65) && (Tempdata[5] == 86)){
					//AC - Voltage
					AC_measure = 1;
					measurment_mode = 1;
					menu_mode = 0;
					DC_measure = 0;
					flag = 0;
				}
				else if((Tempdata[4] == 68) && (Tempdata[5] == 73)){
					currentflag = 1;
					DC_measure = 0;
					AC_measure = 0;//DC - Current

				}
				else if((Tempdata[4] == 65) && (Tempdata[5] == 73)){
					currentflag = 1;
					DC_measure = 0;
					AC_measure = 0;// AC - Current
				}
				else if((Tempdata[4] == 84) && (Tempdata[5] == 67)){
					// Temperature
				}
			}
			else if(Tempdata[2] == 94){
				// set output parameter

				if(Tempdata[4] == 116){
					//type

					if(Tempdata[6] == 100){
						// DC - Signal type
						signal_d = 1;
						signal_s = 0;
						signal_p = 0;
						HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
						HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
						Lcd_clear(&lcd);
						//						output = 1;
					}
					else if(Tempdata[6] == 115){ //s
						// Sinusoidal signal type
						signal_s = 1;
						signal_d = 0;
						signal_p = 0;
						HAL_DAC_Stop_DMA(&hdac1, DAC1_CHANNEL_1);
						HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
						Lcd_clear(&lcd);

					}
					else if(Tempdata[6] == 112){
						// Pulse signal type
						signal_p = 1;
						signal_d = 0;
						signal_s = 0;
						HAL_DAC_Stop_DMA(&hdac1, DAC1_CHANNEL_1);
						HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
						//output = 1;
						Lcd_clear(&lcd);
					}

				}
				else if(Tempdata[4] == 97){
					// Amplitude
					amplitude_output_value[0] = Tempdata[6] - 48;
					amplitude_output_value[1] = Tempdata[7] - 48;
					amplitude_output_value[2] = Tempdata[8] - 48;
					amplitude_output_value[3] = Tempdata[9] - 48;
					//DAC_amplitude_output = amplitude_output_value[0]*1 + amplitude_output_value[1]*0.1 + amplitude_output_value[2]*0.01 + amplitude_output_value[3]*0.001;
					DAC_amplitude_output = amplitude_output_value[0]*1000 + amplitude_output_value[1]*100 + amplitude_output_value[2]*10 + amplitude_output_value[3]*0.001;

				}
				else if(Tempdata[4] == 111){
					//offset
					if(signal_d){

						DC_offset_output_value[0] = Tempdata[6] - 48;
						DC_offset_output_value[1] = Tempdata[7] - 48;
						DC_offset_output_value[2] = Tempdata[8] - 48;
						DC_offset_output_value[3] = Tempdata[9] - 48;
						DAC_DC_offset = DC_offset_output_value[0]*1000 + DC_offset_output_value[1]*100 + DC_offset_output_value[2]*10 + DC_offset_output_value[3];
					}
					else if(signal_s){

						offset_output_value[0] = Tempdata[6] - 48;
						offset_output_value[1] = Tempdata[7] - 48;
						offset_output_value[2] = Tempdata[8] - 48;
						offset_output_value[3] = Tempdata[9] - 48;
						DAC_offset_output = offset_output_value[0]*1000 + offset_output_value[1]*100 + offset_output_value[2]*10 + offset_output_value[3];
					}

				}
				else if(Tempdata[4] == 102){
					// frequency
					frequency_output_value[0] = Tempdata[6] - 48;
					frequency_output_value[1] = Tempdata[7] - 48;
					frequency_output_value[2] = Tempdata[8] - 48;
					frequency_output_value[3] = Tempdata[9] - 48;
					DAC_frequency_output = frequency_output_value[0]*1000 + frequency_output_value[1]*100 + frequency_output_value[2]*10 + frequency_output_value[3];
					TIM2->ARR = (72e6/(DAC_frequency_output*1000)) - 1;
				}
				else if (signal_p){
					if(Tempdata[4] == 100){	 //d
						//Duty Cycle
						DutyCycleinput[0] = Tempdata[6] - 48;
						DutyCycleinput[1] = Tempdata[7] - 48;
						DutyCycleinput[2] = Tempdata[8] - 48;
						DutyCycleinput[3] = Tempdata[9] - 48;
						DutyCycle = DutyCycleinput[0]*1000 + DutyCycleinput[1]*100 + DutyCycleinput[2]*10 + DutyCycleinput[3];
					}
					else if (Tempdata[4] == 'a'){
						//Amplitude_pulse
						Amplitude_pulseinput[0] = Tempdata[6] - 48;
						Amplitude_pulseinput[1] = Tempdata[7] - 48;
						Amplitude_pulseinput[2] = Tempdata[8] - 48;
						Amplitude_pulseinput[3] = Tempdata[9] - 48;
						Amplitude_pulse = Amplitude_pulseinput[0]*1000 + Amplitude_pulseinput[1]*100 + Amplitude_pulseinput[2]*10 + Amplitude_pulseinput[3];
					}
					else if (Tempdata[4] == 'o') {
						//Offset_pulse
						Offset_pulseinput[0] = Tempdata[6] - 48;
						Offset_pulseinput[1] = Tempdata[7] - 48;
						Offset_pulseinput[2] = Tempdata[8] - 48;
						Offset_pulseinput[3] = Tempdata[9] - 48;
						Offset_pulse = Offset_pulseinput[0]*1000 + Offset_pulseinput[1]*100 + Offset_pulseinput[2]*10 + Offset_pulseinput[3];

					}
				}
				else if(Tempdata[4] == 99){
					//Temperature

				}
				flag = 0;
			}


			else if(Tempdata[2] == 42){
				if(Tempdata[4] == 115){
					//request status
					send_status[0] = 64;
					send_status[1] = 44;
					if(DC_measure){

						send_status[2] = 68;
						send_status[3] = 86;
						send_status[4] = 44;
						//send_status[5] = 100;
					}
					else if(AC_measure){
						send_status[2] = 65;
						send_status[3] = 86;
						send_status[4] = 44;
						//send_status[5] = 115;
					}
					if(signal_s){

						send_status[5] = 115;

					}
					else if(signal_d){

						send_status[5] = 100;
					}

					send_status[6] = 44;
					if(Tempdata[6] == 48){
						send_status[7] = 48;
					}
					else if(Tempdata[6] == 49){
						send_status[7] = 49;
					}

					send_status[8] = 44;
					send_status[9] = 33;
					send_status[10] = 10;

					HAL_UART_Transmit(&huart2, send_status, 11, 50);

					if(Tempdata[6] == 48){
						//output off
						output = 0;
						//stop AC DAC
						HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
						// stop DC DAC
						HAL_DAC_Stop(&hdac1, DAC1_CHANNEL_1);
					}
					if(Tempdata[6] == 49){
						// output on
						output = 1;
						Lcd_clear(&lcd);
						menu_mode = 0;
						measurment_mode =0;
					}
				}
				else if(Tempdata[4] == 109){
					// request measurement

					send_measurment = 1;


					if(Tempdata[6] == 116){
						//type
						parameter_t = 1;
						parameter_a = 0;
						parameter_c = 0;
						parameter_d = 0;
						parameter_f = 0;
						parameter_o = 0;

					}
					else if(Tempdata[6] == 97){
						// Amplitude
						parameter_a = 1;
						parameter_c = 0;
						parameter_d = 0;
						parameter_f = 0;
						parameter_o = 0;
						parameter_t = 0;
					}
					else if(Tempdata[6] == 111){
						//offset
						parameter_o = 1;
						parameter_a = 0;
						parameter_c = 0;
						parameter_d = 0;
						parameter_f = 0;
						parameter_t = 0;
					}
					else if(Tempdata[6] == 102){
						// frequency
						parameter_f = 1;
						parameter_a = 0;
						parameter_c = 0;
						parameter_d = 0;
						parameter_o = 0;
						parameter_t = 0;
					}
					else if(Tempdata[6] == 100){
						//Duty Cycle
						parameter_d = 1;
						parameter_a = 0;
						parameter_c = 0;
						parameter_f = 0;
						parameter_o = 0;
						parameter_t = 0;
					}
					//					else if(Tempdata[6] == 99){
					//						//Temperature
					//						parameter_c = 1;
					//						parameter_a = 0;
					//						parameter_d = 0;
					//						parameter_f = 0;
					//						parameter_o = 0;
					//						parameter_t = 0;
					//					}
				}
				flag = 0;
			}

		}
		// Handles DC and AC measurements based on timer interrupt//////////////
		if(DC_measure && timer_interrupt && measurment_mode){

			if(DCidx > 99){

				for(uint8_t c = 0; c<100;c++){
					DC_CS = DC_CS + DC_value[c];
				}

				DC_avg = DC_CS/100;

				DC_CS = 0;
				DCidx = 0;


				scaled_dc_avg = (DC_avg*3300)/(3800);


				//// ASSIGN values to uart array but dont send /////
				DCdisplayoffset[0] = 0x40;
				DCdisplayoffset[1] = 44;
				DCdisplayoffset[2] = 109;
				DCdisplayoffset[3] = 44;
				DCdisplayoffset[4] = 111;//o
				DCdisplayoffset[5] = 44;//,
				DCdisplayoffset[6] = (scaled_dc_avg/1000) + 48;
				DCdisplayoffset[7] = (scaled_dc_avg/100) % 10 + 48;
				DCdisplayoffset[8] = (scaled_dc_avg/10) % 10 + 48;
				DCdisplayoffset[9] = (scaled_dc_avg/1) % 10 + 48;
				DCdisplayoffset[10] = 44;//,
				DCdisplayoffset[11] = 33;//!
				DCdisplayoffset[12] = 10;//\n

				// send lcd /////
				//				Lcd_clear(&lcd);
				//				Lcd_cursor(&lcd, 0, 0);
				//				Lcd_int(&lcd, (DCdisplayoffset[6] - 48));
				//				Lcd_string(&lcd, "." );
				//				Lcd_int(&lcd, (DCdisplayoffset[7]-48));
				//				Lcd_int(&lcd, (DCdisplayoffset[8]-48));
				//				Lcd_int(&lcd, (DCdisplayoffset[9] - 48));
				//				Lcd_string(&lcd, "V");

			}

			if(send_measurment == 1){
				HAL_UART_Transmit(&huart2, DCdisplayoffset, 13, 50);
				send_measurment = 0;

			}
			HAL_ADC_Start(&hadc3);
			HAL_ADC_PollForConversion(&hadc3, 20);
			DC_value[DCidx] = HAL_ADC_GetValue(&hadc3);
			HAL_ADC_Stop(&hadc3);
			DCidx ++ ;
			timer_interrupt = 0;

		}

		else if(AC_measure && timer_interrupt && measurment_mode){

			if(ACidx > 99){

				for(uint8_t c = 0; c<100;c++){
					AC_CS = AC_CS + AC_value[c];
				}
				AC_avg = (AC_CS/100)*0.86;
				AC_CS = 0;
				ACidx = 0;
				for( uint8_t q = 0; q < 99; q ++){
					if(AC_value[q+1] > AC_value[q]){
						AC_max = AC_value[q+1];
					}
				}
				AC_pk_pk = 2*(AC_max - AC_avg) *0.53;

				for(uint8_t s = 0; s < 99; s++){
					if ((AC_value[s+1] > AC_avg) && (AC_value[s] < AC_avg)){

						if(count == 0){
							f1 = s;
							count = 1;
						}
						else if (count == 1){
							f2 = s;
							count = 2;
							AC_freq = 1/((f2-f1)*0.0001);
							count = 0;
						}

					}
				}
				////////Assign values to uart array but not sending them//////

				//// AMPLITUDE/////

				AC_disp_pk_pk[0] = 64;//@
				AC_disp_pk_pk[1] = 44;//,
				AC_disp_pk_pk[2] = 109;//m
				AC_disp_pk_pk[3] = 44;//,
				AC_disp_pk_pk[4] = 97;//a
				AC_disp_pk_pk[5] = 44;//,
				AC_disp_pk_pk[6] = (AC_pk_pk/1000) + 48;
				AC_disp_pk_pk[7] = (AC_pk_pk/100) %10 + 48;
				AC_disp_pk_pk[8] = (AC_pk_pk/10) %10 + 48;
				AC_disp_pk_pk[9] = (AC_pk_pk/1) %10 + 48;
				AC_disp_pk_pk[10] = 44;//,
				AC_disp_pk_pk[11] = 33;//!
				AC_disp_pk_pk[12] = 10;//\n

				/////offset/////////////

				ACdisplayoffset[0] = 64;//@
				ACdisplayoffset[1] = 44;//,
				ACdisplayoffset[2] = 109;//m
				ACdisplayoffset[3] = 44;//,
				ACdisplayoffset[4] = 111;//o
				ACdisplayoffset[5] = 44;//,
				ACdisplayoffset[6] = (AC_avg/1000) %10 + 48;
				ACdisplayoffset[7] = (AC_avg/100) %10 + 48;
				ACdisplayoffset[8] = (AC_avg/10) %10 + 48;
				ACdisplayoffset[9] = (AC_avg/1) %10 + 48;
				ACdisplayoffset[10] = 44;//,
				ACdisplayoffset[11] = 33;//!
				ACdisplayoffset[12] = 10;//\n



				////// FREQUENCY /////////

				AC_disp_frequency[0] = 64;//@
				AC_disp_frequency[1] = 44;//,
				AC_disp_frequency[2] = 109;//m
				AC_disp_frequency[3] = 44;//,
				AC_disp_frequency[4] = 102;//f
				AC_disp_frequency[5] = 44;//,
				AC_disp_frequency[6] = (AC_freq/1000) %10 + 48;
				AC_disp_frequency[7] = (AC_freq/100) %10 + 48;
				AC_disp_frequency[8] = (AC_freq/10) %10 + 48;
				AC_disp_frequency[9] = (AC_freq/1) %10 + 48;
				AC_disp_frequency[10] = 44;//,
				AC_disp_frequency[11] = 33;//!
				AC_disp_frequency[12] = 10;//\n


				///// SEND TO LCD /////////
				//offset//
				//Lcd_clear(&lcd);

				//				Lcd_cursor(&lcd, 0, 0);
				//				Lcd_string(&lcd, "O:");
				//				Lcd_int(&lcd, (ACdisplayoffset[6]-48));
				//				Lcd_string(&lcd, "." );
				//				Lcd_int(&lcd, (ACdisplayoffset[7]-48));
				//				Lcd_int(&lcd, (ACdisplayoffset[8]-48));
				//				Lcd_int(&lcd, (ACdisplayoffset[9]-48));
				//				Lcd_string(&lcd, "V");
				//
				//				//Amplitude //
				//				Lcd_string(&lcd, "A:");
				//				Lcd_int(&lcd, (AC_disp_pk_pk[6]-48));
				//				Lcd_string(&lcd, "." );
				//				Lcd_int(&lcd, (AC_disp_pk_pk[7]-48));
				//				Lcd_int(&lcd, (AC_disp_pk_pk[8]-48));
				//				Lcd_int(&lcd, (AC_disp_pk_pk[9] -48));
				//				Lcd_string(&lcd, "V");
				//
				//				//Frequency//
				//				Lcd_string(&lcd, "F:");
				//				Lcd_int(&lcd, AC_freq);
				//				Lcd_string(&lcd, "Hz");
				//				if(LCD_scroll_count > 8){
				//					lcd_write_command(&lcd, 0x1A);
				//					LCD_scroll_count = 0;
				//				}

				//////////////////////////////////////////////////////////

			}

			if(parameter_a){


				if(send_measurment == 1){
					HAL_UART_Transmit(&huart2, AC_disp_pk_pk, 13, 50);
					send_measurment = 0;
				}

			}
			if(parameter_o){


				if(send_measurment == 1){
					HAL_UART_Transmit(&huart2, ACdisplayoffset, 13, 50);
					send_measurment = 0;
				}

			}
			if(parameter_f){


				if(send_measurment == 1){
					HAL_UART_Transmit(&huart2, AC_disp_frequency, 13, 50);
					send_measurment = 0;
				}

			}

			HAL_ADC_Start(&hadc3);
			HAL_ADC_PollForConversion(&hadc3, 20);
			AC_value[ACidx] = HAL_ADC_GetValue(&hadc3);
			HAL_ADC_Stop(&hadc3);
			ACidx++;
			timer_interrupt = 0;
		}

		///////////////// OUTPUT MODE /////////////////////

		// DC output
		if(signal_d && output){

			HAL_TIM_Base_Start(&htim2);
			HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
			DAC_DC_val = (DAC_DC_offset/1000)*(4096)/3.3;
			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DAC_DC_val);

			//			HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);

			//// SEND LCD ///////
			//			Lcd_clear(&lcd);
			//						Lcd_cursor(&lcd, 0, 0);
			//						Lcd_int(&lcd, DC_offset_output_value[0]);
			//						Lcd_string(&lcd, ".");
			//						Lcd_int(&lcd, DC_offset_output_value[1]);
			//						Lcd_int(&lcd, DC_offset_output_value[2]);
			//						Lcd_int(&lcd, DC_offset_output_value[3]);
			//						Lcd_string(&lcd, "V");

		}

		//AC output

		if(signal_s && output){

			HAL_TIM_Base_Start(&htim2);
			//			HAL_DAC_Stop(&hdac1, DAC1_CHANNEL_1);
			//TIM2->ARR = (72e6/(DAC_frequency_output*1000)) - 1;
			for (int i=0;i<1000;i++)
			{
				//sine_val_DAC[i] = DAC_offset_output+(((sin(i*2*PI/1000) + 1)* DAC_amplitude_output/3.3 *(4096/2)));
				//				sine_val_DAC[i] = (DAC_offset_output*4096/3.3)+((sin(i*2*PI/1000))*((DAC_amplitude_output/2)*4096/3.3));
				sine_val_DAC[i] = (DAC_offset_output*4096/3300)+((sin(i*2*PI/1000))*((DAC_amplitude_output/2)*4096/3300));
			}
			//			get_sineval(DAC_amplitude_output,DAC_offset_output);
			HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, sine_val_DAC, 1000, DAC_ALIGN_12B_R);
			LCD_scroll_count++;
			//			signal_s = 0;

			///// SEND TO LCD /////////
			//offset//

			//Lcd_clear(&lcd);

			//			Lcd_cursor(&lcd, 1, 0);
			//			Lcd_string(&lcd, "O:" );
			//			Lcd_int(&lcd,offset_output_value[0]);
			//			Lcd_string(&lcd, "." );
			//			Lcd_int(&lcd, offset_output_value[1]);
			//			Lcd_int(&lcd, offset_output_value[2]);
			//			Lcd_int(&lcd, offset_output_value[3]);
			//			Lcd_string(&lcd, "V");
			//			//Amplitude //
			//			Lcd_string(&lcd, ",A:" );
			//			Lcd_int(&lcd, amplitude_output_value[0]);
			//			Lcd_string(&lcd, "." );
			//			Lcd_int(&lcd, amplitude_output_value[1]);
			//			Lcd_int(&lcd, amplitude_output_value[2]);
			//			Lcd_int(&lcd, amplitude_output_value[3]);
			//			Lcd_string(&lcd, "V");
			//			//Frequency//
			//			Lcd_string(&lcd, ",F:" );
			//			Lcd_int(&lcd, DAC_frequency_output);
			//			Lcd_string(&lcd, "Hz");
			//
			//
			//			if(LCD_scroll_count > 8){
			//
			//				lcd_write_command(&lcd, 0x1A);
			//
			//				LCD_scroll_count = 0;
			//			}


			//lcd_write_command(&lcd, 0x1A);



			//////////////////////////////////////////////////////////

		}
		else if (signal_p && output) {

			HAL_TIM_Base_Start(&htim2);
			//			TIM2-> EGR = ((TIM2->EGR)|0b01);
			TIM2->ARR = (72e6/(DAC_frequency_output*100)) - 1;
			//HAL_DAC_Stop(&hdac1, DAC1_CHANNEL_1);
			//HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
			get_Pulse_array(DutyCycle, Amplitude_pulse, Offset_pulse);
			HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, Pulse_array, 100, DAC_ALIGN_12B_R);
			LCD_scroll_count++;

			//	TIM2-> EGR = ((TIM2->EGR)|0b01);
			//			signal_p = 0;
		}


		//pulse output

		//		if(signal_p){
		//
		//			get_Pulse_array(DutyCycle, Amplitude_pulse, Offset_pulse);
		//		}


	};
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */

	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_TIM1
			|RCC_PERIPHCLK_ADC34|RCC_PERIPHCLK_TIM2;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.Adc34ClockSelection = RCC_ADC34PLLCLK_DIV1;
	PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
	PeriphClkInit.Tim2ClockSelection = RCC_TIM2CLK_HCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief ADC3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC3_Init(void)
{

	/* USER CODE BEGIN ADC3_Init 0 */

	/* USER CODE END ADC3_Init 0 */

	ADC_MultiModeTypeDef multimode = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC3_Init 1 */

	/* USER CODE END ADC3_Init 1 */
	/** Common config
	 */
	hadc3.Instance = ADC3;
	hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc3.Init.Resolution = ADC_RESOLUTION_12B;
	hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc3.Init.ContinuousConvMode = ENABLE;
	hadc3.Init.DiscontinuousConvMode = DISABLE;
	hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc3.Init.NbrOfConversion = 1;
	hadc3.Init.DMAContinuousRequests = DISABLE;
	hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc3.Init.LowPowerAutoWait = DISABLE;
	hadc3.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	if (HAL_ADC_Init(&hadc3) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure the ADC multi-mode
	 */
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc3, &multimode) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC3_Init 2 */

	/* USER CODE END ADC3_Init 2 */

}

/**
 * @brief DAC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_DAC1_Init(void)
{

	/* USER CODE BEGIN DAC1_Init 0 */

	/* USER CODE END DAC1_Init 0 */

	DAC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN DAC1_Init 1 */

	/* USER CODE END DAC1_Init 1 */
	/** DAC Initialization
	 */
	hdac1.Instance = DAC1;
	if (HAL_DAC_Init(&hdac1) != HAL_OK)
	{
		Error_Handler();
	}
	/** DAC channel OUT1 config
	 */
	sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
	if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN DAC1_Init 2 */

	/* USER CODE END DAC1_Init 2 */

}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void)
{

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 3599;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 1;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_ACTIVE;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */
	HAL_TIM_MspPostInit(&htim1);

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 100-1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11
			|GPIO_PIN_12, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_4|GPIO_PIN_9, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : LD2_Pin PA6 */
	GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PC4 PC6 PC8 */
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PB11 PB12 */
	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PB14 PB15 */
	GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PC7 */
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : PA10 */
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PA11 PA12 */
	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB4 PB9 */
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
