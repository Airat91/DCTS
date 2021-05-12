#include "dcts.h"
#include "modbus.h"
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
#if (ARRAY_NUM)
array_t dcts_array[ARRAY_NUM];
#endif // ARRAY_NUM
/**
  * @}
  */

/*========== GLOBAL VARS ==========*/

/**
  * @brief var for dcts params
  * @ingroup DCTS
  */
dcts_t dcts = {0};

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

/**
 * @brief Set meas_t channel params
 * @param Channel - channel number
 * @param Name
 * @param Name_cyr
 * @param Unit
 * @param Unit_cyr
 * @return  0 - OK\n,
 *          -1 - Channel number out of range
 */
int dcts_meas_channel_init(uint8_t Channel, char Name[], char Name_cyr[], char Unit[], char Unit_cyr[]){
    int result = 0;
    if (Channel < MEAS_NUM){
        strcpy (dcts_meas[Channel].name, Name);
        strcpy (dcts_meas[Channel].name_cyr, Name_cyr);
        strcpy (dcts_meas[Channel].unit, Unit);
        strcpy (dcts_meas[Channel].unit_cyr, Unit_cyr);
        dcts_meas[Channel].value = 0.0f;
        dcts_meas[Channel].valid = 0;
    }else{
        result = -1;
    }
    return result;
}
/**
 * @brief Set act_t channel params
 * @param Channel - channel number
 * @param Name
 * @param Name_cyr
 * @param Unit
 * @param Unit_cyr
 * @return  0 - OK\n,
 *          -1 - Channel number out of range
 */
int dcts_act_channel_init(uint8_t Channel, char Name[], char Name_cyr[], char Unit[], char Unit_cyr[]){
    int result = 0;
    if (Channel < ACT_NUM){
        strcpy (dcts_act[Channel].name, Name);
        strcpy (dcts_act[Channel].name_cyr, Name_cyr);
        strcpy (dcts_act[Channel].unit, Unit);
        strcpy (dcts_act[Channel].unit_cyr, Unit_cyr);
        dcts_act[Channel].set_value = 0.0;
        dcts_act[Channel].meas_value = 0.0;
        dcts_act[Channel].hysteresis = 0.5;
        dcts_act[Channel].state.control = 0;
        dcts_act[Channel].state.pin_state = 0;
        dcts_act[Channel].state.fall = 0;
        dcts_act[Channel].state.short_cir = 0;
    }else{
        result = -1;
    }
    return result;
}

/**
 * @brief Set rele_t channel params
 * @param Channel
 * @param Name
 * @param Name_cyr
 * @return  0 - OK\n,
 *          -1 - Channel number out of range
 */
int dcts_rele_channel_init(uint8_t Channel, char Name[], char Name_cyr[]){
    int result = 0;
    if (Channel < ACT_NUM){
        strcpy (dcts_rele[Channel].name, Name);
        strcpy (dcts_rele[Channel].name_cyr, Name_cyr);
        dcts_rele[Channel].state.control = 0;
        dcts_rele[Channel].state.status = 0;
        dcts_rele[Channel].state.short_cir = 0;
        dcts_rele[Channel].state.fall = 0;
        dcts_rele[Channel].state.control_by_act = 0;
    }else{
        result = -1;
    }
    return result;
}

/**
 * @brief Set alrm_t channel params
 * @param Channel
 * @param Name
 * @param Name_cyr
 * @return  0 - OK\n,
 *          -1 - Channel number out of range
 */
int dcts_alrm_channel_init(uint8_t Channel, char Name[], char Name_cyr[]){
    int result = 0;
    if (Channel < ACT_NUM){
        strcpy (dcts_alrm[Channel].name, Name);
        strcpy (dcts_alrm[Channel].name_cyr, Name_cyr);
        dcts_alrm[Channel].time.hour = 0;
        dcts_alrm[Channel].time.minute = 0;
        dcts_alrm[Channel].time.second = 0;
        dcts_alrm[Channel].enable = 0;
    }else{
        result = -1;
    }
    return result;
}

/**
 * @brief Set dcts_array params
 * @param Number
 * @param Name
 * @param Name_cyr
 * @param Type
 * @param Array_size
 * @return  0 - OK\n,
 *          -1 - Channel number out of range\n,
 *          -2 - Type error
 */
int dcts_array_init(uint8_t Number, char Name[], char Name_cyr[], num_array_type_t  Type, uint8_t Array_size, void * p_Array){
    int result = 0;
    if(Number < ARRAY_NUM){
        strcpy (dcts_alrm[Number].name, Name);
        strcpy (dcts_alrm[Number].name_cyr, Name_cyr);
        dcts_array[Number].type = Type;
        dcts_array[Number].array_size = Array_size;
        switch(Type){
        case NUM_U8_T:
            dcts_array[Number].size_in_bytes = 1;
            dcts_array[Number].array->p_uint8 = (uint8_t*)p_Array;
            break;
        case NUM_S8_T:
            dcts_array[Number].size_in_bytes = 1;
            dcts_array[Number].array->p_int8 = (int8_t*)p_Array;
            break;
        case NUM_U16_T:
            dcts_array[Number].size_in_bytes = 2;
            dcts_array[Number].array->p_uint16 = (uint16_t*)p_Array;
            break;
        case NUM_S16_T:
            dcts_array[Number].size_in_bytes = 2;
            dcts_array[Number].array->p_int16 = (int16_t*)p_Array;
            break;
        case NUM_U32_T:
            dcts_array[Number].size_in_bytes = 4;
            dcts_array[Number].array->p_uint32 = (uint32_t*)p_Array;
            break;
        case NUM_S32_T:
            dcts_array[Number].size_in_bytes = 4;
            dcts_array[Number].array->p_int32 = (int32_t*)p_Array;
            break;
        case NUM_FLOAT_T:
            dcts_array[Number].size_in_bytes = 4;
            dcts_array[Number].array->p_float = (float*)p_Array;
            break;
        default:
            result = -2;
        }
    }else{
        result = -1;
    }
    return result;
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
