/*********************
 *      INCLUDES
 *********************/

#include "lvgl_port_touch.h"
#include "main.h"
#include "i2c.h"
#include "cmsis_os2.h"

/*********************
 *      DEFINES
 *********************/

#if LV_USE_OS == LV_OS_NONE
  #define DELAY_API(ms) HAL_Delay(ms)
#else
  #define DELAY_API(ms) osDelay(ms)
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

static volatile uint8_t touch_irq = 0;
static volatile int32_t last_x = 0;
static volatile int32_t last_y = 0;
static volatile lv_indev_state_t last_state = LV_INDEV_STATE_RELEASED;

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void
lvgl_touchscreen_read (lv_indev_t *indev, lv_indev_data_t *data);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void
lvgl_touchscreen_init (void)
{
  /* 'i2c1' bus and touchscreen reset pin are already configure by CubeMX,
   *  here we just need to reset touchscreen controller */
  HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_SET);
  DELAY_API(10);
  HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_RESET);
  DELAY_API(10);
  HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_SET);
  DELAY_API(10);

  /* basic LVGL driver initialization */
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, lvgl_touchscreen_read);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void
lvgl_touchscreen_read (lv_indev_t      *indev,
                       lv_indev_data_t *data)
{
  /*Use the saved coordinates if there were an interrupt*/
  if(touch_irq)
    {
      /* reset interrupt flag */
      touch_irq = 0;
	  data->point.x = last_x;
	  data->point.y = last_y;
	  data->state = last_state;
    }
    /*If there is no interrupt the touch is released*/
    else {
	  last_state = LV_INDEV_STATE_RELEASED;
	}
}

void
HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == CTP_INT_Pin) {
	  touch_irq = 1;

	  HAL_StatusTypeDef status;
	  uint8_t rx_buf[16] = {0};
	  /* read x/y coordinates */
	  status = HAL_I2C_Mem_Read(&hi2c1, (0x41 << 1), 0x10, 1, rx_buf, sizeof(rx_buf), 100);
	  if (status == HAL_OK)
	  {
		  last_x = (rx_buf[3] & 0x0F) << 8 | rx_buf[2];
		  last_y = (rx_buf[5] & 0x0F) << 8 | rx_buf[4];
		  last_state = LV_INDEV_STATE_PRESSED;
	  }
	  else
	  {
		  last_state = LV_INDEV_STATE_RELEASED;
	  }
  }
}
