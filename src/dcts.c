#include "dcts.h"
#include "string.h"
#include "cmsis_os.h"
#include <type_def.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f1xx_hal.h"

/*========== GLOBAL VARIABLES ==========*/

dcts_t dcts = {
    .dcts_id = DCTS_ID_COMBINED,
    .dcts_ver = {"0.0.1"},
    .dcts_name = {"Thermostat"},
    .dcts_address = 0xFF,
    .dcts_rtc = {1,1,2000,6,12,0,0},
    .dcts_pwr = 0.0,
    .dcts_meas_num = MEAS_NUM,
    .dcts_rele_num = 0,
    .dcts_act_num  = ACT_NUM,
    .dcts_alrm_num = 0,
};

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

/*========== DCTS FUNCTIONS ==========*/

/**
 * @brief Init global variables
 * @ingroup DCTS
 */
__weak void dcts_init () {

    strcpy (dcts_meas[0].name, "Floor");
    strcpy (dcts_meas[0].unit, "°C");
    dcts_meas[0].value = 0;
    
    strcpy (dcts_meas[1].name, "Reg");
    strcpy (dcts_meas[1].unit, "°C");
    dcts_meas[1].value = 0;
    
    strcpy (dcts_meas[2].name, "Rh");
    strcpy (dcts_meas[2].unit, "%");
    dcts_meas[2].value = 0;

    strcpy (dcts_meas[3].name, "ADC0");
    strcpy (dcts_meas[3].unit, "V");
    dcts_meas[3].value = 0;
    strcpy (dcts_meas[4].name, "ADC1");
    strcpy (dcts_meas[4].unit, "V");
    dcts_meas[4].value = 0;

    strcpy (dcts_act[0].name, "Floor");
    strcpy (dcts_act[0].unit, "°C");
    dcts_act[0].set_value = 27.0f;
    dcts_act[0].meas_value = 0.0f;
    dcts_act[0].state.control = TRUE;
    dcts_act[0].state.pin_state = FALSE;
    dcts_act[0].state.short_cir = FALSE;
    dcts_act[0].state.fall = FALSE;
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
