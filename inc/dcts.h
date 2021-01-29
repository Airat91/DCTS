// Library for DeCentralizedTelemetrySystem (DCTS) based on STM32 chip
// Ver 1.0
// Author Girfanov.Ayrat@yandex.ru

/*========== LIBRARY DESCRIPTION ==========
- Library contain global variables and functions according DCTS protocol
- Library use STM32F1xx_HAL_Driver
*/

#ifndef DCTS_H_
#define DCTS_H_ 1

/**
  * @defgroup DCTS
  */
//#include "dcts_config.h"
#include "stm32f1xx_hal_def.h"
#include <stdint.h>

#define CHANNEL_NAME_LEN    20
#define UNIT_NAME_LEN       5
#define DEVICE_NAME_LEN     20
#ifndef MEAS_NUM
#define MEAS_NUM    13
#endif
#ifndef ACT_NUM
#define ACT_NUM     0
#endif
#ifndef RELE_NUM
#define RELE_NUM    0
#endif
#ifndef ALRM_NUM
#define ALRM_NUM    0
#endif
#define FALSE   0
#define TRUE    1

/*========== TYPEDEFS ==========*/

/**
  * @brief DCTS Devise ID
  * @ingroup DCTS
  */
typedef enum {
    DCTS_ID_CONTROL_AND_VIEW    = 1,    // 0x01 - Устройства сбора и отображения информации. Обязательно наличие устройств
                                        //      ввода и отображения информации.
    DCTS_ID_COMBINED            = 2,    // 0x02 - Комбинированные устройства. Имеются каналы измерения и управления устройств
                                        //      релейного типа и с обратной связью.
    DCTS_ID_SECURE              = 3,    // 0x03 - Охранные устройства.
    DCTS_ID_VIDEO               = 4,    // 0x04 - Устройства аудио/видеосвязи. Устройства типа домофона.
    DCTS_ID_RELE                = 5,    // 0x05 - Исполнительные устройства релейного типа.
    DCTS_ID_ACTUATOR            = 6,    // 0x06 - Исполнительные устройства с обратной связью.
    DCTS_ID_MEASURE             = 7,    // 0x07 - Измерительные устройства.
} dcts_id_t;
/**
  * @brief DCTS RTC state
  * @ingroup DCTS
  */
typedef enum {
    RTC_STATE_READY = 0,    // готово к работе
    RTC_STATE_EDIT,         // в режиме редактирования
    RTC_STATE_SET,          // необходимо записать новые значения
} rtc_state_t;
/**
  * @brief DCTS RTC
  * @ingroup DCTS
  */
typedef struct {                // структура, содержащая дату и время
    uint8_t     day;            // число
    uint8_t     month;          // месяц
    uint16_t    year;           // год
    uint8_t     weekday;        // день недели
    uint8_t     hour;           // часы (в 24-часовом формате)
    uint8_t     minute;         // минуты
    uint8_t     second;         // секунды
    rtc_state_t state;          // состояние часов
} rtc_t;
/**
  * @brief Main DCTS struct
  * @ingroup DCTS
  */
typedef struct{
    dcts_id_t dcts_id;      // тип устройства
    char dcts_ver[9];       // версия DCTS библиотеки
    char dcts_name[15];     // имя устройства латиницей
    char dcts_name_cyr[15]; // имя устройства кириллицей
    uint8_t dcts_address;   // адрес устройства в системе (по умолчанию 0xFF)
    rtc_t dcts_rtc;         // RTC устройства
    float dcts_pwr;         // напряжение питания на входе устройства в В
    uint8_t dcts_meas_num;  // количество измерительных каналов устройства
    uint8_t dcts_rele_num;  // количество дискретно управляемых каналов устройства
    uint8_t dcts_act_num;   // количество аналогово управляемых каналов устройства
    uint8_t dcts_alrm_num;  // количество будильников устройства
} dcts_t;
/**
  * @brief Struct for measure channel
  * @ingroup DCTS
  */
typedef struct {                            // таблица структур, содержащая данные измеряемых параметров
    char        name[CHANNEL_NAME_LEN];     // строковое название измеряемого параметра латиницей
    char        name_cyr[CHANNEL_NAME_LEN]; // строковое название измеряемого параметра кириллицей
    char        unit[UNIT_NAME_LEN];        // строковая запись единиц измерения лаитиницей
    char        unit_cyr[UNIT_NAME_LEN];    // строковая запись единиц измерения кириллицей
    float       value;                      // значение измеряемого параметра
    uint8_t     valid;                      // достоверность измеренного значения
} meas_t;
/**
  * @brief Struct for rele channel state
  * @ingroup DCTS
  */
typedef struct {
    uint8_t     control;        // управляющее воздействие ("0" - реле выключено, "1" - реле включено)
    uint8_t     status;         // обратная связь ("0" - ток обмотки реле отсутсвует, "1" - ток обмотки реле
                                // присутствует)
    uint8_t     short_cir;      // короткое замыкание обмотки реле ("0" - КЗ нет, "1" - было обнаружено КЗ). Данный
                                // бит устанавливается в "1" если ток обмотки реле превышает максимальное пороговое
                                // значениие в течение порогового времени, при этом управляющее воздействие реле
                                // выключается. Сброс данного бита происходит при следующей попыке включить реле.
    uint8_t     fall;           // обрыв обмотки реле ("0" - обрыва нет, "1" - обнаружен обрыв обмотки реле). Данный
                                // бит устанавливается в "1" если ток обмотки реле не превысил минимального порогового
                                // значениия в течение порогового времени, при этом управляющее воздействие реле
                                // выключается. Сброс данного бита происходит при следующей попыке включить реле.
} rele_st;
/**
  * @brief Struct for rele channel
  * @ingroup DCTS
  */
typedef struct {                            // таблица структур, содержащая данные дискретно управляемых исполнительных устройств
    char        name[CHANNEL_NAME_LEN];     // строковое название дискретно управляемого исполнительного устройства латиницей
    char        name_cyr[CHANNEL_NAME_LEN]; // строковое название дискретно управляемого исполнительного устройства кириллицей
    rele_st     state;                      // состояниие дискретно управляемого исполнительного устройства (см. rele_st)
} rele_t;
/**
  * @brief Struct for analog channel state
  * @ingroup DCTS
  */
typedef struct {
    uint8_t     control;        // осуществление управления ("0" - не управляется, "1" - управляется)
    uint8_t     pin_state;      // управляющее воздействие в рамках ПИД-регулирования ("0" - выключено, "1" - включено
    uint8_t     short_cir;      // короткое замыкание ("0" - КЗ нет, "1" - было обнаружено КЗ). Данный
                                // бит устанавливается в "1" если ток обмотки реле превышает максимальное пороговое
                                // значениие в течение порогового времени, при этом управляющее воздействие реле
                                // выключается. Сброс данного бита происходит при следующей попыке включить реле.
    uint8_t     fall;           // обрыв ("0" - обрыва нет, "1" - обнаружен обрыв обмотки реле). Данный
                                // бит устанавливается в "1" если ток обмотки реле не превысил минимального порогового
                                // значениия в течение порогового времени, при этом управляющее воздействие реле
                                // выключается. Сброс данного бита происходит при следующей попыке включить реле.
} act_st;
/**
  * @brief Struct for analog channel state
  * @ingroup DCTS
  */
typedef struct {                            // таблица структур, содержащая данные плавно управляемых исполнительных устройств
    char        name[CHANNEL_NAME_LEN];     // строковое название плавно управляемого исполнительного устройства латиницей
    char        name_cyr[CHANNEL_NAME_LEN]; // строковое название плавно управляемого исполнительного устройства кириллицей
    char        unit[UNIT_NAME_LEN];        // строковая запись единиц измерения латиницей
    char        unit_cyr[UNIT_NAME_LEN];    // строковая запись единиц измерения кириллицей
    float       set_value;                  // заданное значение плавно управляемого исполнительного устройства
    float       meas_value;                 // измеренное значение плавно управляемого исполнительного устройства
    act_st      state;                      // состояниие плавно управляемого исполнительного устройства (см. act_st)
} act_t;
/**
  * @brief Struct for alarm time
  * @ingroup DCTS
  */
typedef struct {                // структура, которая содержит время срабатывания будильника
    uint8_t     hour;           // часы (в 24-часовом формате)
    uint8_t     minute;         // минуты
    uint8_t     second;         // секунды
} alarm_time_t;
/**
  * @brief Struct for alarm
  * @ingroup DCTS
  */
typedef struct {
    char        name[CHANNEL_NAME_LEN];     // строковое название будильника латиницей
    char        name_cyr[CHANNEL_NAME_LEN]; // строковое название будильника кириллицей
    alarm_time_t time;                      // время срабатывания будильника (ежедневно)
    uint8_t     enable;                     // состояние будильника (0 - выключенб 1 - включен)
} alrm_t;

/*========== GLOBAL VARS ==========*/

extern dcts_t dcts;
extern meas_t dcts_meas[];
extern rele_t dcts_rele[];
extern act_t dcts_act[];
extern alrm_t dcts_alrm[];

/*========== FUNCTION PROTOTYPES ==========*/

__weak void dcts_init (void);
void dcts_write_meas_value (uint8_t meas_channel, float value);
void dcts_write_act_meas_value (uint8_t act_channel, float value);
void dcts_write_act_set_value (uint8_t act_channel, float value);
int dcts_packet_handle(uint8_t * buff, uint16_t len);
int dcts_meas_channel_init(uint8_t Channel, char Name[], char Name_cyr[], char Unit[], char Unit_cyr[]);
int dcts_act_channel_init(uint8_t Channel, char Name[], char Name_cyr[], char Unit[], char Unit_cyr[]);
int dcts_rele_channel_init(uint8_t Channel, char Name[], char Name_cyr[]);
int dcts_alrm_channel_init(uint8_t Channel, char Name[], char Name_cyr[]);

#endif /*DCTS*/
