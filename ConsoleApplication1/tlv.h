//*****************************************************************************
// file		: tlv.h
// It's a tlv file ref: http://www.cnblogs.com/liping13599168/archive/2011/06/15/2081366.html
//
// Copyright (c) 2006-2020 HSAE co. Ltd. All rights reserved
//
// Change Logs:
// Date				Author		Note
// 2017/06/20   	kuan		First draft version
// 2017/07/26   	kuan		将TLV_Pack tag大小和Len大小封装在TLV当中
//*****************************************************************************


#ifndef __TLV_H__
#define __TLV_H__

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;


#ifdef __cplusplus
#define    __I    volatile             /*!< Defines 'read only' permissions                 */
#else
#define    __I    volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define    __O    volatile             /*!< Defines 'write only' permissions                */
#define   __IO    volatile             /*!< Defines 'read / write' permissions              */


/*!< STM32F10x Standard Peripheral Library old types (maintained for legacy purpose) */

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */



                            /** 一些状态枚举 */
typedef enum { RESET = 0, SET = !RESET } FlagStatus, ITStatus;
#define IS_STATUS_STATE(STATE) (((STATE) == RESET) || ((STATE) == SET))
typedef enum { DISABLE = 0, ENABLE = !DISABLE } FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
typedef enum { ERROR = 0, SUCCESS = !ERROR } ErrorStatus;
#define IS_ERROR_STATE(STATE) (((STATE) == ERROR) || ((STATE) == SUCCESS))


#ifndef NULL
#define NULL ((void *)0)
#endif
#define ARR_SIZE(a)  (sizeof((a))/sizeof((a[0])))

#define PR(fmt,...) printf(fmt,##__VA_ARGS__)

//*****************************************************************************
//
//! \addtogroup TLV
//! @{
//
//*****************************************************************************




//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif


    /** describe TLV information */
    typedef struct
    {
        u8* tag;        /**< 标记    最大支持4个字节    */
        u8* len;        /**< 数据长度 最大支持4个字节    */
        s8* val;        /**< 数据                     */
        u8 tagSize;    /**< 标记占用字节数             */
        u8 lenSize;    /**< 数据长度占用字节数          */
        u32 valLen;     /**< 数据长度                  */
    } t_TLVEntity;


//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
    ErrorStatus TLV_Pack(s8 *data, s32 *dataLen, t_TLVEntity TLVEntity);
    void TLV_UnPack(s8 *data, s32 dataLen, s32 *offset, s8 *tag,
        u32 tlvTagSize, u32 tlvLenSize, void(*mytlvfunc)(t_TLVEntity*));
    void TLV_Unit(void);
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

#endif //  __TLV_H__
