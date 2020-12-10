#include "dcts.h"
#include "dcts_config.h"
#include "string.h"
#include "cmsis_os.h"
#include <type_def.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f1xx_hal.h"

/**
  * @addtogroup button
  * @{
  */
#if (MEAS_NUM)
meas_t dcts_meas[MEAS_NUM];
#endif // MEAS_NUM
#if (RELE_NUM)
rele_t dcts_rele[RELE_NUM];
#endif // RELE_NUM
#if (ACT_NUM)
act_t dcts_act[ACT_NUM];
#endif // ACT_NUM
#if (ALRM_NUM)
alrm_t dcts_alrm[ALRM_NUM];
#endif // ALRM_NUM
/**
  * @}
  */

/*========== GLOBAL VARS ==========*/

/**
  * @brief var for dcts params
  * @ingroup DCTS
  */
dcts_t dcts = {
    .dcts_id = DCTS_ID_COMBINED,
    .dcts_ver = {"0.0.2"},
    .dcts_name = {"Banya"},
    .dcts_name_cyr = {"Баня"},
    .dcts_address = 0xFF,
    .dcts_rtc = {1,1,2000,6,12,0,0},
    .dcts_pwr = 0.0,
    .dcts_meas_num = MEAS_NUM,
    .dcts_rele_num = 0,
    .dcts_act_num  = ACT_NUM,
    .dcts_alrm_num = 0,
};

/*========== FUNCTIONS ==========*/

/**
 * @brief Init global variables
 * @ingroup DCTS
 */
__weak void dcts_init () {

    strcpy (dcts_meas[0].name, "Water level\0");
    strcpy (dcts_meas[0].name_cyr, "Уровень\0");
    strcpy (dcts_meas[0].unit, "l\0");
    strcpy (dcts_meas[0].unit_cyr, "л\0");
    dcts_meas[0].value = 0;

    strcpy (dcts_meas[1].name, "Water level ADC\0");
    strcpy (dcts_meas[1].name_cyr, "Уровень АЦП\0");
    strcpy (dcts_meas[1].unit, "adc\0");
    strcpy (dcts_meas[1].unit_cyr, "adc\0");
    dcts_meas[1].value = 0;

    strcpy (dcts_meas[2].name, "Water level V\0");
    strcpy (dcts_meas[2].name_cyr, "Уровень В\0");
    strcpy (dcts_meas[2].unit, "V\0");
    strcpy (dcts_meas[2].unit_cyr, "В\0");
    dcts_meas[2].value = 0;

    strcpy (dcts_meas[3].name, "Water tmpr\0");
    strcpy (dcts_meas[3].name_cyr, "Темп. воды\0");
    strcpy (dcts_meas[3].unit, "°C\0");
    strcpy (dcts_meas[3].unit_cyr, "°C\0");
    dcts_meas[3].value = 0;

    strcpy (dcts_meas[4].name, "Water tmpr ADC\0");
    strcpy (dcts_meas[4].name_cyr, "Темп. воды АЦП\0");
    strcpy (dcts_meas[4].unit, "adc\0");
    strcpy (dcts_meas[4].unit_cyr, "adc\0");
    dcts_meas[4].value = 0;

    strcpy (dcts_meas[5].name, "Water tmpr V\0");
    strcpy (dcts_meas[5].name_cyr, "Темп. воды В\0");
    strcpy (dcts_meas[5].unit, "V\0");
    strcpy (dcts_meas[5].unit_cyr, "В\0");
    dcts_meas[5].value = 0;

    strcpy (dcts_meas[6].name, "Predbannik tmpr\0");
    strcpy (dcts_meas[6].name_cyr, "Темп. предбанник\0");
    strcpy (dcts_meas[6].unit, "°C\0");
    strcpy (dcts_meas[6].unit_cyr, "°C\0");
    dcts_meas[6].value = 0;

    strcpy (dcts_meas[7].name, "Predbannik hum\0");
    strcpy (dcts_meas[7].name_cyr, "Вл. предбанник\0");
    strcpy (dcts_meas[7].unit, "%\0");
    strcpy (dcts_meas[7].unit_cyr, "%\0");
    dcts_meas[7].value = 0;

    strcpy (dcts_meas[8].name, "Moyka tmpr\0");
    strcpy (dcts_meas[8].name_cyr, "Темп. мойка\0");
    strcpy (dcts_meas[8].unit, "°C\0");
    strcpy (dcts_meas[8].unit_cyr, "°C\0");
    dcts_meas[8].value = 0;

    strcpy (dcts_meas[9].name, "Moyka hum\0");
    strcpy (dcts_meas[9].name_cyr, "Вл. мойка\0");
    strcpy (dcts_meas[9].unit, "%\0");
    strcpy (dcts_meas[9].unit_cyr, "%\0");
    dcts_meas[9].value = 0;

    strcpy (dcts_meas[10].name, "Parilka tmpr\0");
    strcpy (dcts_meas[10].name_cyr, "Темп. парилка\0");
    strcpy (dcts_meas[10].unit, "°C\0");
    strcpy (dcts_meas[10].unit_cyr, "°C\0");
    dcts_meas[10].value = 0;

    strcpy (dcts_meas[11].name, "Vref ADC\0");
    strcpy (dcts_meas[11].name_cyr, "ИОН АЦП\0");
    strcpy (dcts_meas[11].unit, "adc\0");
    strcpy (dcts_meas[11].unit_cyr, "adc\0");
    dcts_meas[11].value = 0;

    strcpy (dcts_meas[12].name, "Vref V\0");
    strcpy (dcts_meas[12].name_cyr, "ИОН В\0");
    strcpy (dcts_meas[12].unit, "V\0");
    strcpy (dcts_meas[12].unit_cyr, "В\0");
    dcts_meas[12].value = 0;
}

void dcts_write_meas_value (uint8_t meas_channel, float value){
    taskENTER_CRITICAL();
    dcts_meas[meas_channel].value = value;
    taskEXIT_CRITICAL();
}
void dcts_write_act_meas_value (uint8_t act_channel, float value){
    taskENTER_CRITICAL();
    dcts_act[act_channel].meas_value = value;
    taskEXIT_CRITICAL();
}
void dcts_write_act_set_value (uint8_t act_channel, float value){
    taskENTER_CRITICAL();
    dcts_act[act_channel].set_value = value;
    taskEXIT_CRITICAL();
}

int dcts_packet_handle(uint8_t * buff, uint16_t len){
    int result = 1;

    return result;
}
