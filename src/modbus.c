#include <stdio.h>
#include <string.h>
#include "modbus.h"
#include "dcts.h"
#include "main.h"

/**
  * @defgroup modbus
  * @brief mudbus functions for communicate
  */

/*  "01 - Read Coils":                ('1',  'req_input', 2000, "BOOL",  1, "Q", "X", "Coil"),
    "02 - Read Input Discretes":      ('2',  'req_input', 2000, "BOOL",  1, "I", "X", "Input Discrete"),
    "03 - Read Holding Registers":    ('3',  'req_input',  125, "WORD", 16, "Q", "W", "Holding Register"),
    "04 - Read Input Registers":      ('4',  'req_input',  125, "WORD", 16, "I", "W", "Input Register"),
    "05 - Write Single coil":         ('5', 'req_output',    1, "BOOL",  1, "Q", "X", "Coil"),
    "06 - Write Single Register":     ('6', 'req_output',    1, "WORD", 16, "Q", "W", "Holding Register"),
   ?"15 - Write Multiple Coils":     ('15', 'req_output', 1968, "BOOL",  1, "Q", "X", "Coil"),
    "16 - Write Multiple Registers": ('16', 'req_output',  123, "WORD", 16, "Q", "W", "Holding Register")}
*/

/*  DCTS params in ModBus area:

    3XXXX - strings

    30000 - DCTS.dcts_name          (char)*2
    30100 - DCTS.dcts_name_cyr      (char)*2
    30200 - DCTS.dcts_ver           (char)*2
    30300 -                         (char)*2
    30400 -                         (char)*2
    30500 -                         (char)*2
    30600 -                         (char)*2
    30700 -                         (char)*2
    30800 -                         (char)*2
    30900 -                         (char)*2

    310X0 - dcts_meas[X].name       (char)*2
    311X0 - dcts_meas[X].name_cyr   (char)*2
    312X0 - dcts_meas[X].unit       (char)*2
    313X0 - dcts_meas[X].unit_cyr   (char)*2
    314X0 -                         (char)*2
    315X0 -                         (char)*2
    316X0 -                         (char)*2
    317X0 -                         (char)*2
    318X0 -                         (char)*2
    319X0 -                         (char)*2

    320X0 - dcts_rele[X].name       (char)*2
    321X0 - dcts_rele[X].name_cyr   (char)*2
    322X0 -                         (char)*2
    323X0 -                         (char)*2
    324X0 -                         (char)*2
    325X0 -                         (char)*2
    326X0 -                         (char)*2
    327X0 -                         (char)*2
    328X0 -                         (char)*2
    329X0 -                         (char)*2

    330X0 - dcts_act[X].name        (char)*2
    331X0 - dcts_act[X].name_cyr    (char)*2
    332X0 - dcts_act[X].unit        (char)*2
    333X0 - dcts_act[X].unit_cyr    (char)*2
    334X0 -                         (char)*2
    335X0 -                         (char)*2
    336X0 -                         (char)*2
    337X0 -                         (char)*2
    338X0 -                         (char)*2
    339X0 -                         (char)*2

    340X0 - dcts_alrm[X].name       (char)*2
    341X0 - dcts_alrm[X].name_cyr   (char)*2
    342X0 -                         (char)*2
    343X0 -                         (char)*2
    344X0 -                         (char)*2
    345X0 -                         (char)*2
    346X0 -                         (char)*2
    347X0 -                         (char)*2
    348X0 -                         (char)*2
    349X0 -                         (char)*2

    350X0 - dcts_num_array[X].name      (char)*2
    351X0 - dcts_num_array[X].name_cyr  (char)*2
    352X0 -                         (char)*2
    353X0 -                         (char)*2
    354X0 -                         (char)*2
    355X0 -                         (char)*2
    356X0 -                         (char)*2
    357X0 -                         (char)*2
    358X0 -                         (char)*2
    359X0 -                         (char)*2

    4XXXX - values
    4X0XX, 4X1XX, 4X2XX - (float)/2
    4X3XX, 4X4XX, 4X5XX, 4X6XX, 4X7XX, 4X8XX, 4X9XX - (uint8_t)

    40000 - DCTS.dcts_pwr                   (float)/2 HIGH
    40001 - DCTS.dcts_pwr                   (float)/2 LOW
    4010X -                                 (float)/2 HIGH
    4010X+1                                 (float)/2 LOW
    4020X -                                 (float)/2 HIGH
    4020X+1                                 (float)/2 LOW
    40300 - DCTS.rtc.day                    (uint8_t)
    40301 - DCTS.rtc.month                  (uint8_t)
    40302 - DCTS.rtc.year                   (uint8_t) HIGH
    40303 - DCTS.rtc.year                   (uint8_t) LOW
    40304 - DCTS.rtc.weekday                (uint8_t)
    40305 - DCTS.rtc.hour                   (uint8_t)
    40306 - DCTS.rtc.minute                 (uint8_t)
    40307 - DCTS.rtc.second                 (uint8_t)
    ...
    40400 - DCTS.dcts_addr                  (uint8_t)
    40401 - DCTS.dcts_id                    (uint8_t)
    40402 - DCTS.meas_num                   (uint8_t)
    40403 - DCTS.rele_num                   (uint8_t)
    40404 - DCTS.act_num                    (uint8_t)
    40405 - DCTS.alrm_num                   (uint8_t)
    ...
    40500 -                                 (uint8_t)
    40600 -                                 (uint8_t)
    40700 -                                 (uint8_t)
    40800 -                                 (uint8_t)
    40900 -                                 (uint8_t)

    4100X - dcts_meas[X/2].value            (float)/2 HIGH
    4100X+1 - dcts_meas[X/2].value          (float)/2 LOW
    4110X -                                 (float)/2 HIGH
    4110X+1                                 (float)/2 LOW
    4120X -                                 (float)/2 HIGH
    4120X+1                                 (float)/2 LOW
    4130X - dcts_meas[X].valid              (uint8_t)
    4140X -                                 (uint8_t)
    4150X -                                 (uint8_t)
    4160X -                                 (uint8_t)
    4170X -                                 (uint8_t)
    4180X -                                 (uint8_t)
    4190X -                                 (uint8_t)

    4200X -                                 (float)/2 HIGH
    4200X+1                                 (float)/2 LOW
    4210X -                                 (float)/2 HIGH
    4210X+1                                 (float)/2 LOW
    4220X -                                 (float)/2 HIGH
    4220X+1                                 (float)/2 LOW
    4230X - dcts_rele[X].state.control      (uint8_t)
    4240X - dcts_rele[X].state.status       (uint8_t)
    4250X - dcts_rele[X].state.short_cir    (uint8_t)
    4260X - dcts_rele[X].state.fall         (uint8_t)
    4270X - dcts_rele[X].state.control_by_act   (uint8_t)
    4280X -                                 (uint8_t)
    4290X -                                 (uint8_t)

    4300X - dcts_act[X/2].set_value         (float)/2 HIGH
    4300X+1 - dcts_act[X/2].set_value       (float)/2 LOW
    4310X - dcts_act[X/2].meas_value        (float)/2 HIGH
    4310X+1 - dcts_act[X/2].meas_value      (float)/2 LOW
    4320X - dcts_act[X/2].hysteresis        (float)/2 HIGH
    4320X+1 - dcts_act[X/2].hysteresis      (float)/2 LOW
    4330X - dcts_act[X].state.control       (uint8_t)
    4340X - dcts_act[X].state.pin_state     (uint8_t)
    4350X - dcts_act[X].state.short_cir     (uint8_t)
    4360X - dcts_act[X].state.fall          (uint8_t)
    4370X -                                 (uint8_t)
    4380X -                                 (uint8_t)
    4390X -                                 (uint8_t)

    4400X -                                 (float)/2 HIGH
    4400X+1                                 (float)/2 LOW
    4410X -                                 (float)/2 HIGH
    4410X+1                                 (float)/2 LOW
    4420X -                                 (float)/2 HIGH
    4420X+1                                 (float)/2 LOW
    4430X - dcts_alrm[X].time.hour          (uint8_t)
    4440X - dcts_alrm[X].time.minute        (uint8_t)
    4450X - dcts_alrm[X].time.second        (uint8_t)
    4460X - dcts_alrm[X].enable             (uint8_t)
    4470X -                                 (uint8_t)
    4480X -                                 (uint8_t)
    4490X -                                 (uint8_t)

    45X0Y - dcts_num_array[X].array[Y]      (float,uint32_t,int32_t)/2 HIGH
    45X0Y+1 - dcts_num_array[X].array[Y]    (float,uint32_t,int32_t)/2 LOW
    or
    45X0Y - dcts_num_array[X].array[Y]      (uint8_t,int8_t,uint16_t,int16_t)
    4570X - dcts_num_array[X].type          (uint8_t)
    4580X - dcts_num_array[X].size_in_bytes (uint8_t)
    4590X - dcts_num_array[X].array_size    (uint8_t)
*/

/*========= GLOBAL VARIABLES ==========*/

/*========= FUNCTIONS ==========*/

dcts_mdb_t modbus_get_dcts_by_mdb_addr (u16 mdb_addr){
    dcts_mdb_t result;
    result.type = DCTS_VAL_UNKNOWN;
    result.value.p_f = 0;
    result.error = DCTS_ERR_OK;
    u8 channel = mdb_addr%100;
    u8 group = GROUP_NONE;
    if(((mdb_addr >= 30000)&&(mdb_addr <= 35000))||\
    ((mdb_addr >= 40000)&&(mdb_addr <= 45000))){
        switch ((mdb_addr%10000)/1000){
        case 0:    //DCTS params
            group = GROUP_DCTS;
            break;
        case 1:    //dcts_meas texts
            group = GROUP_MEAS;
            break;
        case 2:    //dcts_rele texts
            group = GROUP_RELE;
            break;
        case 3:    //dcts_act texts
            group = GROUP_ACT;
            break;
        case 4:    //dcts_alrm texts
            group = GROUP_ALRM;
            break;
        case 5:    //dcts_array texts
            group = GROUP_ARRAY;
            break;
        }
        if(mdb_addr/10000 == 3){    //30000..35000 - string data
            u8 string_pos = channel % 10;
            channel /= 10;
            switch (group) {
            case GROUP_DCTS:
                result.type = DCTS_VAL_CHAR;
                if(mdb_addr%1000 < 100){
                    result.value.p_char = &dcts.dcts_name[string_pos*2];
                }else if((mdb_addr%1000 >= 100)&&(mdb_addr%1000 < 200)){
                    result.value.p_char = &dcts.dcts_name_cyr[string_pos*2];
                }else if((mdb_addr%1000 >= 200)&&(mdb_addr%1000 < 300)){
                    result.value.p_char = &dcts.dcts_ver[string_pos*2];
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#if MEAS_NUM
            case GROUP_MEAS:
                result.type = DCTS_VAL_CHAR;
                if(mdb_addr%1000 < 100){
                    result.value.p_char = &dcts_meas[channel].name[string_pos*2];
                }else if((mdb_addr%1000 >= 100)&&(mdb_addr%1000 < 200)){
                    result.value.p_char = &dcts_meas[channel].name_cyr[string_pos*2];
                }else if((mdb_addr%1000 >= 200)&&(mdb_addr%1000 < 300)){
                    result.value.p_char = &dcts_meas[channel].unit[string_pos*2];
                }else if((mdb_addr%1000 >= 300)&&(mdb_addr%1000 < 400)){
                    result.value.p_char = &dcts_meas[channel].unit_cyr[string_pos*2];
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#endif //MEAS_NUM
#if RELE_NUM
            case GROUP_RELE:
                result.type = DCTS_VAL_CHAR;
                if(mdb_addr%1000 < 100){
                    result.value.p_char = &dcts_rele[channel].name[string_pos*2];
                }else if((mdb_addr%1000 >= 100)&&(mdb_addr%1000 < 200)){
                    result.value.p_char = &dcts_rele[channel].name_cyr[string_pos*2];
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#endif //RELE_NUM
#if ACT_NUM
            case GROUP_ACT:
                result.type = DCTS_VAL_CHAR;
                if(mdb_addr%1000 < 100){
                    result.value.p_char = &dcts_act[channel].name[string_pos*2];
                }else if((mdb_addr%1000 >= 100)&&(mdb_addr%1000 < 200)){
                    result.value.p_char = &dcts_act[channel].name_cyr[string_pos*2];
                }else if((mdb_addr%1000 >= 200)&&(mdb_addr%1000 < 300)){
                    result.value.p_char = &dcts_act[channel].unit[string_pos*2];
                }else if((mdb_addr%1000 >= 300)&&(mdb_addr%1000 < 400)){
                    result.value.p_char = &dcts_act[channel].unit_cyr[string_pos*2];
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#endif //ACT_NUM
#if ALRM_NUM
            case GROUP_ALRM:
                result.type = DCTS_VAL_CHAR;
                if(mdb_addr%1000 < 100){
                    result.value.p_char = &dcts_alrm[channel].name[string_pos*2];
                }else if((mdb_addr%1000 >= 100)&&(mdb_addr%1000 < 200)){
                    result.value.p_char = &dcts_alrm[channel].name_cyr[string_pos*2];
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#endif //ALRM_NUM
#if ARRAY_NUM
            case GROUP_ARRAY:
                result.type = DCTS_VAL_CHAR;
                if(mdb_addr%1000 < 100){
                    result.value.p_char = &dcts_array[channel].name[string_pos*2];
                }else if((mdb_addr%1000 >= 100)&&(mdb_addr%1000 < 200)){
                    result.value.p_char = &dcts_array[channel].name_cyr[string_pos*2];
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#endif //ARRAY_NUM
            default:
                result.error = DCTS_ADDR_ERR;
            }
        }else if(mdb_addr/10000 == 4){  //40000...45000 - values
            switch (group) {
            case GROUP_DCTS:
                if(mdb_addr%1000 < 2){  //dcts.dcts_pwr
                    result.type = DCTS_VAL_FLOAT;
                    result.value.p_f = &dcts.dcts_pwr;
                    if(channel%2 == 0){
                        result.value.p_word++;
                    }
                }else if((mdb_addr%1000 >= 300)&&(mdb_addr%1000 <= 307)){    //dcts.rtc
                    result.type = DCTS_VAL_BYTE;
                    switch (channel){
                    case 0:
                        result.value.p_byte = &dcts.dcts_rtc.day;
                        break;
                    case 1:
                        result.value.p_byte = &dcts.dcts_rtc.month;
                        break;
                    case 2:
                        result.value.p_word = &dcts.dcts_rtc.year;
                        result.value.p_byte++;
                        break;
                    case 3:
                        result.value.p_word = &dcts.dcts_rtc.year;
                        break;
                    case 4:
                        result.value.p_byte = &dcts.dcts_rtc.weekday;
                        break;
                    case 5:
                        result.value.p_byte = &dcts.dcts_rtc.hour;
                        break;
                    case 6:
                        result.value.p_byte = &dcts.dcts_rtc.minute;
                        break;
                    case 7:
                        result.value.p_byte = &dcts.dcts_rtc.second;
                        break;
                    }
                }else if((mdb_addr%1000 >= 400)&&(mdb_addr%1000 < 406)){    //dcts params
                    result.type = DCTS_VAL_BYTE;
                    switch (channel){
                    case 0:
                        result.value.p_byte = &dcts.dcts_address;
                        break;
                    case 1:
                        result.value.p_byte = (uint8_t*)dcts.dcts_id;
                        break;
                    case 2:
                        result.value.p_byte = &dcts.dcts_meas_num;
                        break;
                    case 3:
                        result.value.p_byte = &dcts.dcts_rele_num;
                        break;
                    case 4:
                        result.value.p_byte = &dcts.dcts_act_num;
                        break;
                    case 5:
                        result.value.p_byte = &dcts.dcts_alrm_num;
                        break;
                    }
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#if MEAS_NUM
            case GROUP_MEAS:
                if((mdb_addr%1000 < 100)&&(channel < MEAS_NUM*2)){
                    result.type = DCTS_VAL_FLOAT;
                    result.value.p_f = &dcts_meas[channel/2].value;
                    if(channel%2 == 0){
                        result.value.p_word++;
                    }
                }else if((mdb_addr%1000 >= 300)&&(mdb_addr%1000 < 400)&&(channel < MEAS_NUM)){
                    result.type = DCTS_VAL_BYTE;
                    result.value.p_byte = &dcts_meas[channel].valid;
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#endif //MEAS_NUM
#if RELE_NUM
            case GROUP_RELE:
                result.type = DCTS_VAL_BYTE;
                if((mdb_addr%1000 >= 300)&&(mdb_addr%1000 < 400)&&(channel < RELE_NUM)){
                    result.value.p_byte = &dcts_rele[channel].state.control;
                }else if((mdb_addr%1000 >= 400)&&(mdb_addr%1000 < 500)&&(channel < RELE_NUM)){
                    result.value.p_byte = &dcts_rele[channel].state.status;
                }else if((mdb_addr%1000 >= 500)&&(mdb_addr%1000 < 600)&&(channel < RELE_NUM)){
                    result.value.p_byte = &dcts_rele[channel].state.short_cir;
                }else if((mdb_addr%1000 >= 600)&&(mdb_addr%1000 < 700)&&(channel < RELE_NUM)){
                    result.value.p_byte = &dcts_rele[channel].state.fall;
                }else if((mdb_addr%1000 >= 700)&&(mdb_addr%1000 < 800)&&(channel < RELE_NUM)){
                    result.value.p_byte = &dcts_rele[channel].state.control_by_act;
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#endif //RELE_NUM
#if ACT_NUM
            case GROUP_ACT:
                result.type = DCTS_VAL_BYTE;
                if((mdb_addr%1000 < 700)&&(channel < ACT_NUM)){
                    uint8_t elem_nmb = mdb_addr%100;
                    channel = (mdb_addr%1000)/100;
                    if(elem_nmb < dcts_array[channel].array_size*2){
                        switch(dcts_array[channel].type){
                        case NUM_U8_T:
                        case NUM_S8_T:
                            result.value.p_byte = dcts_array[channel].array[elem_nmb].p_uint8;
                            break;
                        case NUM_U16_T:
                        case NUM_S16_T:
                            result.type = DCTS_VAL_WORD;
                            result.value.p_word = dcts_array[channel].array[elem_nmb].p_uint16;
                            break;
                        case NUM_U32_T:
                        case NUM_S32_T:
                        case NUM_FLOAT_T:
                            result.type = DCTS_VAL_FLOAT;
                            result.value.p_f = dcts_array[channel].array[elem_nmb/2].p_float;
                            if(elem_nmb%2 == 0){
                                result.value.p_word++;
                            }
                            break;
                        }
                    }else{
                        result.error = DCTS_ADDR_ERR;
                    }
                }else if((mdb_addr%1000 >= 700)&&(mdb_addr%1000 < 800)&&(channel < ACT_NUM)){
                    result.value.p_byte = (uint8_t*)dcts_array[channel].type;
                }else if((mdb_addr%1000 >= 800)&&(mdb_addr%1000 < 900)&&(channel < ACT_NUM)){
                    result.value.p_byte = &dcts_array[channel].size_in_bytes;
                }else if((mdb_addr%1000 >= 900)&&(channel < ACT_NUM)){
                    result.value.p_byte = &dcts_array[channel].array_size;
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#endif //ACT_NUM
#if ALRM_NUM
            case GROUP_ALRM:
                if((mdb_addr%1000 >= 300)&&(mdb_addr%1000 < 400)&&(channel < ALRM_NUM)){
                    result.value.p_byte = &dcts_alrm[channel].time.hour;
                }else if((mdb_addr%1000 >= 400)&&(mdb_addr%1000 < 500)&&(channel < ALRM_NUM)){
                    result.value.p_byte = &dcts_alrm[channel].time.minute;
                }else if((mdb_addr%1000 >= 500)&&(mdb_addr%1000 < 600)&&(channel < ALRM_NUM)){
                    result.value.p_byte = &dcts_alrm[channel].time.second;
                }else if((mdb_addr%1000 >= 600)&&(mdb_addr%1000 < 700)&&(channel < ALRM_NUM)){
                    result.value.p_byte = &dcts_alrm[channel].enable;
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#endif //ALRM_NUM
#if ARRAY_NUM
            case GROUP_ARRAY:
                result.type = DCTS_VAL_BYTE;
                if((mdb_addr%1000 >= 300)&&(mdb_addr%1000 < 400)&&(channel < ARRAY_NUM)){
                    result.value.p_byte = &dcts_rele[channel].state.control;
                }else if((mdb_addr%1000 >= 400)&&(mdb_addr%1000 < 500)&&(channel < ARRAY_NUM)){
                    result.value.p_byte = &dcts_rele[channel].state.status;
                }else if((mdb_addr%1000 >= 500)&&(mdb_addr%1000 < 600)&&(channel < ARRAY_NUM)){
                    result.value.p_byte = &dcts_rele[channel].state.short_cir;
                }else if((mdb_addr%1000 >= 600)&&(mdb_addr%1000 < 700)&&(channel < ARRAY_NUM)){
                    result.value.p_byte = &dcts_rele[channel].state.fall;
                }else if((mdb_addr%1000 >= 700)&&(mdb_addr%1000 < 800)&&(channel < ARRAY_NUM)){
                    result.value.p_byte = &dcts_rele[channel].state.control_by_act;
                }else{
                    result.error = DCTS_ADDR_ERR;
                }
                break;
#endif //ARRAY_NUM
            default:
                result.error = DCTS_ADDR_ERR;
            }
        }
    }
    return result;
}

/**
 * @brief htons for buffers lenths
 * @param word_numm - number uint16 words(uint8 * 2)
 * @return number of replaced world(uint16)
 * @ingroup modbus
 **/
u8 htons_buff(u16 *buff,u8 word_numm){
    u8 i;
    for (i = 0;i<word_numm;i++){
        buff[i] = htons(buff[i]);
    }
    return i;
}

/**
 * @brief Count crc16 for packet modbus
 * @param pckt - pointer to counted buffer
 * @param len  - length  packet with out two bytes crc
 * len type u16 for use not only modbus
 * @return crc
 * @ingroup modbus
 * */
u16 modbus_crc16(u8* pckt, u16 len){
    u16  result;
    u16 i, j;
    len = len > 254?254:len;
    result = 0xFFFF;
    for (i = 0; i < len; i++)  {
        result ^= pckt[i];
        for (j = 0; j < 8; j++) {
            if ((result & 0x01) == 1){
                result = (result >> 1) ^ 0xA001;
            }else{
                result >>= 1;
            }
        }
    }
    return result;
}

/**
 * @brief Control crc16 for packet modbus, count self and compare with tail
 * @param pckt - pointer to counted buffer
 * @param len - length  packet with two bytes len
 * len type u16 for use not only modbus
 * @return  1 - OK,\n
 *          0 - not compared
 * @ingroup modbus
 * */
u8 modbus_crc16_check(u8* pckt,u16 lenght){
    if (lenght < 3){
        return 0;
    }
    uint16_t test_1 = modbus_crc16 (pckt, lenght-2);
    uint16_t test_2 = *(u16*)(void*)(pckt + lenght - 2);
    if (modbus_crc16 (pckt, lenght-2) != *(u16*)(void*)(pckt + lenght - 2)) {
        return 0;
    }
    return 1;
}

/**
 * @brief Check modbus address in packet with device address
 * @param pckt - pointer to counted buffer
 * @param len - length  packet with two bytes len
 * @return  1 - for me,\n
 *          0 - not for me
 * @ingroup modbus
 * */
u8 modbus_packet_for_me(u8* pckt,u16 lenght){
    if (lenght){
        if ((pckt[0] != config.params.mdb_address)  &&
            (pckt[0] != MODBUS_BROADCAST_ADDRESS)){
            return 0;
        }else{
            return 1;
        }
    }else{
        return 0;
    }
}

/**
 * @brief
 * 01-02 and 0x00 for read coils regs
 * 03 function for read regs
 * 04 function for read regs
 * 05 write one coil
 * 06 write one reg
 * 15 write coils
 * 16 write regs
 * @param pckt - pointer to counted buffer
 * @param len_in - length incoming packet
 * @return length reply packet
 * @ingroup modbus
 */
u16 modbus_rtu_packet (u8* pckt,u16 len_in){
    u16 len_reply;
    u8 error, function;
    u8 reinit_rtc = 0;
    u16 start_address, regs_numm, num_bit;
    dcts_mdb_t data = {0};
    len_reply = 0;
    error = 0;
    if (it_modbus_request_check(pckt, len_in)==1){
        function = pckt[1];
        switch (function) {
        /*case 0:
        case 1:
        case 2:
        */
            /*read coil regs only for dinamic space coil to byte type
            in user space used one byte for one coil*/
            /*{
                u16 coil_number;
                coil_number = (u16)(pckt[2] << 8) + pckt[3];		// 2-3 bit start
                num_bit = (u16)(pckt[4] << 8) + pckt[5];		// 4-5num bit
                pool_id = modbus_dinamic_addr_check(coil_number,function,num_bit);
                if (pool_id >= 0){
                    base_address = modbus_dinamic_addr_get(pool_id);
                    if (base_address != NULL){
                        coil_number -= base_address->address;
                        len_reply = (u8)((num_bit+7)/8);
                        if (num_bit == 0) {
                            error = ILLEGAL_DATA_VALUE;
                        } else {
                            regs_access_t reg;
                            reg.flag = U8_REGS_FLAG;
                            u8 bit_n,byte_n;
                            for (u16 i=0; i<num_bit; i++){
                                osMutexWait(user_regs_access_mutex, portMAX_DELAY );{
                                    memcpy(&reg.value.op_u8,base_address->data + coil_number + i , 1);
                                }osMutexRelease(user_regs_access_mutex);
                                bit_n = i%8;
                                byte_n = (u8)(i/8);
                                if(reg.value.op_u8 & BIT(0)){
                                    *(u8 *)(pckt+3+byte_n)|=BIT(bit_n);
                                }else{
                                    *(u8 *)(pckt+3+byte_n)&=~BIT(bit_n);
                                }
                            }
                            if (!error) {
                                pckt[2] = (u8)len_reply;
                                len_reply += 5;
                            }
                        }
                    }
                }
            }
            break;*/
        case 3:
        case 4:
            /*should read only regs*/
            start_address = (u16)(pckt[2] << 8) + pckt[3];
            regs_numm = (u16)(pckt[4] << 8) + pckt[5];
            if (regs_numm < 1) {
                error = ILLEGAL_DATA_VALUE;
            }else if(!error){
                len_reply = (u8)(regs_numm << 1);
                pckt[2] = (u8)len_reply;
                for(uint8_t i = 0; i < regs_numm; i++){
                    data = modbus_get_dcts_by_mdb_addr(start_address + i);
                    for(uint8_t byte_nmb = 0; byte_nmb < 2; byte_nmb++){
                        if(data.error == DCTS_ADDR_ERR){
                            error = ILLEGAL_DATA_ADDRESS;
                        }else{
                            switch (data.type){
                            case DCTS_VAL_BYTE:
                                if(byte_nmb == 1){
                                    pckt[3+2*i+byte_nmb] = data.value.p_byte[1-byte_nmb];
                                }else{
                                    pckt[3+2*i+byte_nmb] = 0x00;
                                }
                                break;
                            default:
                                pckt[3+2*i+byte_nmb] = data.value.p_byte[1-byte_nmb];
                            }
                        }
                    }
                }
                len_reply += 5;
            }
            break;
        //case 5:
            /*write bit (coil) only for dinamic space coil to byte type
            in user space used one byte for one coil*/
            /*{
                u16 coil_number,status;
                regs_access_t reg_temp;
                coil_number= (u16)((u16)pckt[2] << 8) + (u16)pckt[3];
                status  = (u16)((u16)pckt[4] << 8) + ((u16)pckt[5]);
                pool_id = modbus_dinamic_addr_check(coil_number, MDB_COILS_RW, 1);
                if (pool_id >= 0){
                    base_address = modbus_dinamic_addr_get(pool_id);
                    if (base_address!=NULL){
                        coil_number -= base_address->address;
                        */
                        /*for coil functions start address is byte address not mdb*/
                        /*reg_temp.flag = U8_REGS_FLAG;
                        if (!error) {
                            if(status == 0xff00){
                                reg_temp.value.op_u8 = BIT(0);
                            }else if (status == 0x0000){
                                reg_temp.value.op_u8 = 0;
                            }else{
                                error = ILLEGAL_DATA_VALUE;
                            }*/
                            /*writing*/
                            /*if (!error){
                                osMutexWait(user_regs_access_mutex, portMAX_DELAY );{
                                    memcpy(base_address->data + coil_number,&reg_temp.value.op_u8,1);
                                }osMutexRelease(user_regs_access_mutex);
                            }
                            len_reply = 8;*/
                            /*dont change first byte they will be in answear transaction*/
                        /*}
                    }
                }
            }
            break;*/
        case 6:
            /*write one word*/
            start_address = ((u16)(pckt[2] << 8) + pckt[3]);
            data = modbus_get_dcts_by_mdb_addr(start_address );
            if (data.error == DCTS_ADDR_ERR){
                error = ILLEGAL_DATA_ADDRESS;
            }else{
                switch (data.type) {
                case DCTS_VAL_BYTE:
                    if((start_address >= 40300)&&(start_address <= 40307)){
                        dcts.dcts_rtc.state = RTC_STATE_EDIT;
                        reinit_rtc = 1;
                    }
                    taskENTER_CRITICAL();
                    *data.value.p_byte = pckt[5];
                    taskEXIT_CRITICAL();
                    break;
                case DCTS_VAL_WORD:
                    taskENTER_CRITICAL();
                    *data.value.p_word = ((uint16_t)(pckt[4] << 8) + pckt[5]);
                    taskEXIT_CRITICAL();
                    break;
                default:
                    error = ILLEGAL_DATA_ADDRESS;
                    break;
                }
                len_reply = 8;
            }
            if(reinit_rtc == 1){
                dcts.dcts_rtc.state = RTC_STATE_SET;
            }
            break;
        /*case 15:*/
            /*write coils only for dinmaic space "coil to byte type"
            in user space used one byte for one coil*/
        /*{
            u8 bytes_num;
            u16 coil_number;
            coil_number = (u16)(pckt[2] << 8) + pckt[3];		// 2-3 bit start
            num_bit = (u16)(pckt[4] << 8) + pckt[5];		// 4-5num bit
            pool_id = modbus_dinamic_addr_check(coil_number, MDB_COILS_RW, num_bit);
            if (pool_id >= 0){
                base_address = modbus_dinamic_addr_get(pool_id);
                if (base_address!=NULL){
                    coil_number -= base_address->address;
                    bytes_num = pckt[6];
                    if (num_bit == 0) {
                        error = ILLEGAL_DATA_VALUE;
                    } else {
                        u16 write_bit = 0;
                        for (u16 i=0; i<bytes_num; i++){
                            regs_access_t reg;
                            reg.flag = U8_REGS_FLAG;
                            for(u8 j=0; (j<8) && num_bit; j++){
                                if(pckt[7+i] & BIT(j)){
                                    reg.value.op_u8 = BIT(0);
                                }else{
                                    reg.value.op_u8 = 0;
                                }*/
                                /*writing*/
                                /*osMutexWait(user_regs_access_mutex, portMAX_DELAY );{
                                    memcpy(base_address->data + coil_number + write_bit ,&reg.value.op_u8,1);
                                }osMutexRelease(user_regs_access_mutex);
                                write_bit++;
                                num_bit--;
                            }
                        }
                        if (!error) {
                            len_reply = 8;*/
                            /*dont change first byte they will be in answear transaction*/
                        /*}
                    }
                }else{
                    error = ILLEGAL_DATA_ADDRESS;
                }
            }else{
                error = ILLEGAL_DATA_ADDRESS;
            }
        }
        break;*/
        case 16:
            /*write words*/
            start_address = ((u16)(pckt[2] << 8) + pckt[3]);
            regs_numm = ((u16)(pckt[4] << 8) + pckt[5]);
            for(u8 i = 0; i < regs_numm; i++){
                data = modbus_get_dcts_by_mdb_addr(start_address + i);
                if (data.error == DCTS_ADDR_ERR){
                    error = ILLEGAL_DATA_ADDRESS;
                }else{
                    switch (data.type) {
                    case DCTS_VAL_BYTE:
                        if(((start_address + i) >= 40300)&&((start_address + i) <= 40307)){
                            dcts.dcts_rtc.state = RTC_STATE_EDIT;
                            reinit_rtc = 1;
                        }
                        taskENTER_CRITICAL();
                        *data.value.p_byte = pckt[8+i*2];
                        taskEXIT_CRITICAL();
                        break;
                    case DCTS_VAL_WORD:
                        taskENTER_CRITICAL();
                        *data.value.p_word = ((uint16_t)(pckt[8+i*2] << 8) + pckt[9+i*2]);
                        taskEXIT_CRITICAL();
                        break;
                    default:
                        error = ILLEGAL_DATA_ADDRESS;
                        break;
                    }
                }
            }
            if(reinit_rtc == 1){
                dcts.dcts_rtc.state = RTC_STATE_SET;
            }
            if (!error){
                len_reply = 8;
            }
            break;
        default:
            error = ILLEGAL_FUNCTION;
            break;
        }
        if(error){
            pckt[1] |= 0x80;
            pckt[2] = error;
            len_reply = 5;
        }
        *((u16*)(void*)(&pckt[len_reply-2])) = modbus_crc16 (pckt, len_reply-2);
    }
    return len_reply;
}
u8 genenerate_error_packet(u8* packet,u8 error_code){
    packet[0] = (u8)config.params.mdb_address;
    packet[1] |=0x80;
    packet[2] = error_code;
    *(u16 *)(void*)(&packet[3]) = modbus_crc16 (packet, 3);
    return 5;
}
u8 modbus_err_packet_type(u8 rtu_flag,u8* packet,u8 error_code){
    u8 len;
    if(rtu_flag == MODBUS_TCP_PACKET){
        genenerate_error_packet(&packet[6],error_code);
        len = 9;
        packet[5] = len;
    }else{
        genenerate_error_packet(packet,error_code);
        len = 5;
    }
    return len;
}
/**
 * @brief simply check packet for modbus,
 *  control crc and function number
 *
 * */

u8 it_modbus_simply_check(u8* pckt,u16 length){
    vu8 ret;
    ret = 0;
    if ((pckt[1]<=16)||(pckt[1]==0x41)){
        if (modbus_crc16_check(pckt, length)){
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}
/**
 * @brief verbose check packet for modbus rtu, all used function
 * @return 1 if checked
 * */
int it_modbus_request_check(u8* buff,u16 length){
    int result=0;
    u8 first,command;
    //SOFI_ASSERT("modbus check buff error", (buff!=NULL));
    if(length <= 256){
        first = 0;
        command = buff[first+1];
        switch (command){
        case 0:
        case 1:
        case 2:
            {
                u16 bit_quant;
                bit_quant = (u16)((u16)buff[first+4] << 8) + ((u16)buff[first+5]);
                if(bit_quant<254){
                    if(it_modbus_simply_check(buff,8)){
                        result = 1;
                    }
                }
            }
            break;
        case 3:
        case 4:
            {
                u16 reg_quant;
                reg_quant = (u16)((u16)buff[first+4] << 8) + ((u16)buff[first+5]);
                if(reg_quant<127){
                    if(it_modbus_simply_check(buff,8)){
                        result = 1;
                    }
                }
            }
            break;
        case 5:
            {
                u16 status;
                status  = (u16)((u16)buff[first+4] << 8) + ((u16)buff[first+5]);
                if((status == 0xff00) || (status == 0x0000)){
                    if(it_modbus_simply_check(buff,8)){
                        result = 1;
                    }
                }
            }
        case 6:
            if(it_modbus_simply_check(buff,8)){
                result = 1;
            }
            break;
        case 15:
            {
                u16 coil_item,coil_quant;
                u8 coil_bytes;
                coil_item = (u16)((u16)buff[first+2] << 8) + (u16)buff[first+3];
                coil_quant = (u16)((u16)buff[first+4] << 8) + ((u16)buff[first+5]);
                coil_bytes = buff[first+6];
                if (coil_bytes && coil_quant){
                    if(it_modbus_simply_check(buff,9+coil_bytes)){
                        result = 1;
                    }
                }
            }
            break;
        case 16:
            {
                u16 reg_quant;
                u8 reg_quant_bytes;
                reg_quant = (u16)((u16)buff[first+4] << 8) + ((u16)buff[first+5]);
                reg_quant_bytes = buff[first+6];
                if ((reg_quant_bytes == reg_quant*2) && reg_quant && (reg_quant<126)){
                    if(it_modbus_simply_check(buff,9+reg_quant_bytes)){
                        result = 1;
                    }
                }
            }
            break;
        case 0x41:
            if(it_modbus_simply_check(buff,length)){
                result = 1;
            }
            break;
        default:
            result = -1;
            break;
        }
    }
    return result;
}
/**
 * @brief verbose check packet for modbus rtu, all used function
 *
 * */
int it_modbus_responde_check(u8* buff,u16 length){
    int result=0;
    u8 first,command;
    //SOFI_ASSERT("modbus check buff error", (buff!=NULL));
    if (length<=256){
        first = 0;
        //address = buff[first];
        command = buff[first+1];
        switch (command){
        case 0:
        case 1:
        case 2:
            {
                u8 byte_number;
                byte_number = buff[first+2];
                if(byte_number<251){
                    if(it_modbus_simply_check(buff,(byte_number + 5))){
                        result = 1;
                    }
                }
            }
            break;
        case 3:
        case 4:
            {
                u8 byte_number;
                byte_number = buff[first+2];
                if(byte_number<251){
                    if(it_modbus_simply_check(buff,(byte_number + 5))){
                        result = 1;
                    }
                }
            }
            break;
        case 5:
            {
                u16 status;
                status  = (u16)((u16)buff[first+4] << 8) + ((u16)buff[first+5]);
                if((status == 0xff00) || (status == 0x0000)){
                    if(it_modbus_simply_check(buff,8)){
                        result = 1;
                    }
                }
            }
        case 6:
            if(it_modbus_simply_check(buff,8)){
                result = 1;
            }
            break;
        case 15:
            if(it_modbus_simply_check(buff,8)){
                result = 1;
            }
            break;
        case 16:
            if(it_modbus_simply_check(buff,8)){
                result = 1;
            }
            break;
        case 0x41:
            result = 1;
            break;
        default:
            result = 0;
            break;
        }
    }
    return result;
}

/**
 * @brief make packet from parametrs
 * @param slave_address simply will add to packet
 * @param function simply will add to packet
 * @param start_addr will add to packet with htons
 * @param reg_num will add to packet with htons for functions - 1,2,3,4,15,16
 * @param data_to_write pointer data from to for functions 15,16
 * @param packet where packet will safe
 * @return len made packet
 * @ingroup modbus
 * */
int modbus_make_packet (u8  slave_address,u8  function, u16 start_addr,
                         u16 reg_num, u8 * data_to_write, u8 * packet){
    int byte;
    byte = 0;
    union {
        u16 u16;
        u8  u8[2];
    } tmp;
    if(packet !=NULL){
        packet[byte++] = slave_address;
        packet[byte++] = function;
        tmp.u16 = htons(start_addr);
        packet[byte++] = tmp.u8[0];
        packet[byte++] = tmp.u8[1];
        if (function == 3 || function == 4 || function == 1 || function == 2){
            tmp.u16 = htons(reg_num);
            packet[byte++] = tmp.u8[0];
            packet[byte++] = tmp.u8[1];
        }else if((function == 16 ) && (reg_num <= 127)){
            if(data_to_write!=NULL){
                tmp.u16 = htons(reg_num);
                /*safe word num */
                packet[byte++] = tmp.u8[0];
                packet[byte++] = tmp.u8[1];
                u8 byte_numm = (u8)(reg_num << 1);
                /*safe byte num use only in 16 command*/
                packet[byte++] = byte_numm;
                u8 data_first = (u8)byte;
                /*safe data*/
                for (u8 i=0;i<byte_numm; i++){
                    packet[byte++] = data_to_write[i];
                }
                /*change to modbus indian*/
                htons_buff((u16*)(void*)(&packet[data_first]),(u8)reg_num);
            }else{
                byte = -1;
            }
        }else if((function == 15) && (reg_num <= 254)){
            /*set bits,all coil  regs have bit to byte addreses*/
            if(data_to_write!=NULL){
                u8 byte_count,bit_n,byte_n;
                tmp.u16 = htons(reg_num);
                /*safe word num */
                packet[byte++] = tmp.u8[0];
                packet[byte++] = tmp.u8[1];
                byte_count = (u8)((reg_num+7)/8);
                packet[byte++] = byte_count;
                for (u8 i=0;i<reg_num; i++){
                    bit_n = i%8;
                    byte_n = i/8;
                    if (data_to_write[i]&BIT(0)){
                        packet[byte + byte_n] |= BIT(bit_n);
                    }else{
                        packet[byte + byte_n] &= ~BIT(bit_n);
                    }
                }
                byte += byte_count;
            }else{
                byte = -1;
            }
        }else if(function == 6){
            /*write word*/
            if(data_to_write!=NULL){
                packet[byte++] = data_to_write[1];
                packet[byte++] = data_to_write[0];
            }else{
                byte = -1;
            }
        }else if(function == 5){
            /*set single bits,all coil  regs have bit to byte addreses*/
            u16 status;
            if (data_to_write[0]&BIT(0)){
                status = 0xff00;
            }else{
                status = 0x0000;
            }
            packet[byte++] = ((status>>8)&0xff);
            packet[byte++] = (status&0xff);
        }else{
            byte = -1;
        }
        /*add modbus crc*/
        if(byte > 0){
            *(u16*)(void*)(&packet[byte]) = modbus_crc16(packet,(u16)byte);
            byte +=2;
        }
    }else{
        byte = -1;
    }
    return byte;
}
