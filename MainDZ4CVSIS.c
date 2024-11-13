#include "stm32f407xx.h"
#include "stm32f4xx.h" // Device header

// LED2 PA6 31pin соотв. GPIO port mode register (GPIOx_MODER) 12-13 битам
// LED3 PA7 32pin соотв. GPIO port mode register (GPIOx_MODER) 14-15 битам
#define PA6_2bit (6 * 2) // 12-13 биты
#define PA7_2bit (7 * 2) // 14-15 биты
#define PA6_1bit (6)     // 6 бит
#define PA7_1bit (7)     // 7 бит


void initClk(void);
void initGPIO(void);

int main(void) {
  
initClk();
initGPIO();


  while (1) {
    // Согласно GPIO port output data register (GPIOx_ODR), отправим в регист уровень
    GPIOA->ODR ^= (1 << PA6_1bit);
    for (int i = 0; i < 200000; i++) {};
    GPIOA->ODR ^= (1 << PA7_1bit);
  }
}

void initClk()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // В описании RCC register map - находим GPIOAEN который
																				// принадлежит регистру RCC_AHB1ENR, для подачи клока надо установить 1 в
																				// 0бит, или RCC_AHB1ENR_GPIOAEN соотв. 0x00000001
}

void initGPIO()
{
/*Согласно GPIO introduction, требуется настроить 4 регистра
Each general-purpose I/O port has four 32-bit configuration registers (GPIOx_MODER,
GPIOx_OTYPER, GPIOx_OSPEEDR and GPIOx_PUPDR)*/

  GPIO_TypeDef MYPORT; // Создадим свой экземпляр структуры настроек MYPORT

  MYPORT.MODER = 0x01; // Согласно GPIO port mode register (GPIOx_MODER) 01: General purpose output mode
  MYPORT.OSPEEDR = 0x00; // Согласно GPIO port output speed register (GPIOx_OSPEEDR) 00: 2 MHz Low speed
  MYPORT.OTYPER = 0x01;  // Согласно GPIO port output type register (GPIOx_OTYPER)  1: Output open-drain
  MYPORT.PUPDR = 0x01; // Согласно GPIO port pull-up/pull-down register (GPIOx_PUPDR) 01: Pull-up

  // Согласно GPIO port mode register (GPIOx_MODER) 00: Input (reset state)
  GPIOA->MODER &= ~(GPIO_MODER_MODER0 << PA6_2bit); // Сброс значение 0x00000003(11) отправим в 12-13 биты
  GPIOA->MODER &= ~(GPIO_MODER_MODER0 << PA7_2bit); // Сброс значение 0x00000003(11) отправим в 14-15 биты
  GPIOA->MODER |= (MYPORT.MODER << PA6_2bit); // Установим значение 0x01(01) отправим в 12-13 биты
  GPIOA->MODER |= (MYPORT.MODER << PA7_2bit); // Установим значение 0x01(01) отправим в 14-15 биты

  // Согласно GPIO port output speed register (GPIOx_OSPEEDR) 00: 2 MHz Low speed
  GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << PA6_2bit); // Сброс, 0x00000003(11) отправим в 12-13 биты
  GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << PA7_2bit); // Сброс, 0x00000003(11) отправим в 14-15 биты
  GPIOA->OSPEEDR |= (MYPORT.OSPEEDR << PA6_2bit);
  GPIOA->OSPEEDR |= (MYPORT.OSPEEDR << PA7_2bit);

  // Согласно GPIO port output type register (GPIOx_OTYPER) 0: Output push-pull (reset state)
  GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_0 << PA6_1bit); //Сброс 0x00000001
  GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_0 << PA7_1bit); //Сброс 0x00000001
  GPIOA->OTYPER |= (MYPORT.OTYPER << PA6_1bit);
  GPIOA->OTYPER |= (MYPORT.OTYPER << PA7_1bit);

  // Согласно GPIO port pull-up/pull-down register (GPIOx_PUPDR) 00: No pull-up, pull-down
  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << PA6_2bit); //Сброс 0x00000003
  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << PA7_2bit); //Сброс 0x00000003
  GPIOA->PUPDR |= (MYPORT.PUPDR << PA6_2bit);
  GPIOA->PUPDR |= (MYPORT.PUPDR << PA7_2bit);
}
