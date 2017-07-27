//*****************************************************************************
// file		: tlv.c
// It's a tlv file ref: http://www.cnblogs.com/liping13599168/archive/2011/06/15/2081366.html
//
// Copyright (c) 2006-2020 HSAE co. Ltd. All rights reserved
//
// Change Logs:
// Date				Author		Note
// 2017/06/20   	kuan		First draft version
// 2017/07/26   	kuan		��TLV_Pack tag��С��Len��С��װ��TLV����
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup TLV
//! @{
//
//*****************************************************************************
#include "tlv.h"
#include "stdarg.h"
#include "stdio.h"
#include "malloc.h"
#include "stddef.h"
#include "string.h"
#include "stdlib.h"

/**
* \brief PcharToData charָ��󼸸��ֽ��������� ��� U32 4���ֽ�
*
* \param data ����ָ��
* \param len  ���ݳ���
*
* \return
*/
u32 PcharToData(s8 *data, u8 len)
{
    u32 i = 0, tmp = 0;
    for (i = 0; i < len; i++)
    {
        tmp |= (data[i] << (8 * i));

    }
    return tmp;
}

/**
 * \brief _TLV_Unpack TLV���ݽ��
 *
 * \param [out]data ����ָ��
 * \param [out]dataLen ���ݳ���
 * \param TLVEntity   TLV����
 *
 * \return ErrorStatus
 */
ErrorStatus TLV_Pack(s8 *data, s32 *dataLen, t_TLVEntity TLVEntity)
{
    ErrorStatus tmpStatus = SUCCESS;

    u32 offst = 0;

    TLVEntity.valLen = PcharToData(TLVEntity.len, TLVEntity.lenSize);

    offst += *dataLen;
    memcpy(data + offst, TLVEntity.tag, TLVEntity.tagSize);
    offst += TLVEntity.tagSize;
    memcpy(data + offst, TLVEntity.len, TLVEntity.lenSize);
    offst += TLVEntity.lenSize;

    memcpy(data + offst, TLVEntity.val, TLVEntity.valLen);

    *dataLen += TLVEntity.tagSize + TLVEntity.lenSize + TLVEntity.valLen;

    return SUCCESS;
}


typedef enum
{
    T_Status,     /**<   T_Status */
    L_Status,     /**<   L_Status */
    V_Status,       /**< V_Status */
}
t_TLV_Status;



/**
 * \brief _TLV_Unpack TLV���ݽ��
 *
 * \param [in]data ����ָ��
 * \param [in]dataLen ���ݳ���
 * \param offsetƫ�Ƴ���
 * \param tag Ѱ�ұ�ʶ
 * \param tlvTagSize tlvTag��С
 * \param tlvLenSize tlvLen��С
 * \param mytlvfunc �ص�����
 *
 * \return NONE
 */
ErrorStatus TLV_UnPack(s8 *tag, s8 tmpData,
    u32 tlvTagSize, u32 tlvLenSize, void(*mytlvfunc)(t_TLVEntity*))
{

    static t_TLV_Status TLV_Status = T1_Status;
    static t_TLVEntity tlv;
    static u8 tagBuf[4] = { 0 };
    static u8 tagBufCount = 0;
    static u8 lenBuf[4] = { 0 };
    static u8 lenBufCount = 0;
    
    if (tlvTagSize > 4 || tlvLenSize > 4)
    {
        PR("tlvTagSize is more than 4 or tlvLenSize is more than 4");
        return ERROR;
    }
    switch (TLV_Status)
    {

    case T1_Status:
        if (tagBufCount<tlvTagSize)
        {
            tagBuf[tagBufCount++] = tmpData;
        }
        else
        {    
            if (0 == memcmp(tag, tagBuf, tlvTagSize))
            {
               
            }
            tagBufCount = 0;
        }
        break;


    case L1_Status:
        if (1 == tlvTagSize)
        {
            TLV_Status = V_Status;
            tlv.len    = 
            tlv.valLen = PcharToData(tmpData, tlvLenSize);
        }
        break;

    case L2_Status:

        break;

    case L3_Status:

        break;

    case L4_Status:

        break;

    case V_Status:

        break;



    default:
        break;
    }





    *offset = 0;
    if (dataLen <= 0)
    {
        return;
    }
    //while(*offset<dataLen)
    {

        if (0 == memcmp(tag, tlv.tag, tlvTagSize))
        {
            tlv.tagSize = tlvTagSize;
            tlv.lenSize = tlvLenSize;
            mytlvfunc(&tlv);/****�ص�����ÿ�ν�����TLVֵ*****/
        }

        tlv.tag = data + *offset;
        tlv.len = data + *offset + tlvTagSize;
        tlv.valLen = PcharToData(tlv.len, tlvLenSize);

        /****���TLV L����Ϊ0���*****/
        if (0 == tlv.valLen)
        {
            tlv.val = NULL;
        }
        else
        {
            tlv.val = data + *offset + tlvTagSize + tlvLenSize;
        }
        *offset += tlvTagSize + tlvLenSize + tlv.valLen;


    }
    return;
}



/**
  * \brief mytlvfuncUnPack TLV��Ԫ����õ���ʱ����
  *
  * \param  None
  *
  * \return None
  */
static void  mytlvfuncUnPack(t_TLVEntity *tlv)
{


}


/**
  * \brief TLV_Unit TLV��Ԫ���
  *
  * \param  None
  *
  * \return None
  */
void TLV_Unit(void)
{
    u32 i = 0;

    u8  tmpPackBuf[100] = { 0 };
    u32 tmpPackBufLen = 0;
    u32 tmpPackBufLenOffset = 0;


    u8 tmpUnPackBuf[100] = { 0x00,0x00,0x00,
                           0x01,0x00, 0x00,
                           0x02,0x00,0x01,0x08,
                           0x03,0x01,0x02,0x03,0x04
    };
    u32 tmpUnPackBufLen = ARR_SIZE(tmpUnPackBuf);
    u32 tmpUnPackBufLenOffset = 0;

    t_TLVEntity tmpTLVEntity1 =
    {
        {"\x02\x03"},//tag
        {"\x08"},    //len
        {"\x01\x03\x04\x01\x09\x04\x01\x03" },//val
        0,//valLen
        2,//tagSize
        1,//lenSize
    };

    t_TLVEntity tmpTLVEntity2 =
    {
        {"\x07\x05"},//tag
        {"\x04"},    //len
        {"\x01\x03\x04\x01" },//val
        0,//valLen
        2,//tagSize
        1,//lenSize
    };

    /************��������ͽ��**************/
    TLV_Pack(tmpPackBuf, &tmpPackBufLen, tmpTLVEntity1);

    TLV_Pack(tmpPackBuf, &tmpPackBufLen, tmpTLVEntity2);


    TLV_Pack(tmpPackBuf, &tmpPackBufLen, tmpTLVEntity2);

    TLV_UnPack(tmpPackBuf, tmpPackBufLen, &tmpPackBufLenOffset, "\x07\x05", 2, 1, mytlvfuncUnPack);
    TLV_UnPack(tmpPackBuf, tmpPackBufLen, &tmpPackBufLenOffset, "\x02\x03", 2, 1, mytlvfuncUnPack);


    /************�������Խ��**************/
    TLV_UnPack(tmpUnPackBuf + tmpUnPackBufLenOffset, tmpUnPackBufLen, &tmpUnPackBufLenOffset, "\x03\x01", 2, 1, mytlvfuncUnPack);
    TLV_UnPack(tmpUnPackBuf + tmpUnPackBufLenOffset, tmpUnPackBufLen, &tmpUnPackBufLenOffset, "\x01\x00", 2, 1, mytlvfuncUnPack);
    TLV_UnPack(tmpUnPackBuf + tmpUnPackBufLenOffset, tmpUnPackBufLen, &tmpUnPackBufLenOffset, "\x00\x00", 2, 1, mytlvfuncUnPack);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
