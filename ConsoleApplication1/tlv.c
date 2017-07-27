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
    T_Status,              /**<   T_Status */
    L_Status,              /**<   L_Status */
    V_Status,              /**< V_Status */
}
t_TLV_Status;



/**
 * \brief _TLV_Unpack TLV���ݽ��
 *
 * \param TagCmp TAG��־�Ƚϻص�
 * \param tmpData ������������
 * \param tlvTagSize tlvTag��С
 * \param tlvLenSize tlvLen��С
 * \param mytlvfunc �ص�����
 *
 * \return NONE
 */
ErrorStatus TLV_UnPack(ErrorStatus( *TagCmp)(s8 *tagBuf,u8 tagBufCount),s8 tmpData,
    u32 tlvTagSize, u32 tlvLenSize, void(*mytlvfunc)(t_TLVEntity*))
{
    ErrorStatus tmpStatus = SUCCESS;

    static t_TLV_Status TLV_Status = T_Status;
    static t_TLVEntity tlv;

    static s8 tagBuf[10] = { 0 };
    static u8 tagBufCount = 0;
    static s8 lenBuf[4] = { 0 };
    static u8 lenBufCount = 0;
    static s8  *val = NULL;
    static u32 valCount = 0;

    if (tlvTagSize > 4 || tlvLenSize > 4)
    {
        PR("tlvTagSize is more than 4 or tlvLenSize is more than 4");
        return ERROR;
    }

    switch (TLV_Status)
    {

    case T_Status:
        tagBuf[tagBufCount++] = tmpData;
        if (tagBufCount >= tlvTagSize)
        {
            if (SUCCESS == TagCmp(tagBuf, tlvTagSize))
            {
                TLV_Status = L_Status;
                tlv.tagSize = tlvTagSize;
                tlv.tag = tagBuf;
                tagBufCount = 0;
            }
            else
            {
                /* ��ǰ�ƶ�һ�� */
                memcpy(tagBuf, tagBuf + 1, --tagBufCount);
            }
        }
        break;


    case L_Status:
        lenBuf[lenBufCount++] = tmpData;
        if (lenBufCount >= tlvLenSize)
        {
            tlv.len = lenBuf;
            tlv.valLen = PcharToData(lenBuf, tlvLenSize);
            tlv.lenSize = tlvLenSize;


            /****���TLV L����Ϊ0���*****/
            if (0 == tlv.valLen)
            {
                TLV_Status = T_Status;
                tlv.val = NULL;
                mytlvfunc(&tlv);/****�ص�����ÿ�ν�����TLVֵ*****/
            
            }
            else
            {
                TLV_Status = V_Status;
                val = calloc(tlv.valLen, sizeof(s8));
            }
            lenBufCount = 0;
        }
        break;
    case V_Status:
        val[valCount++] = tmpData;
        if (valCount >= tlv.valLen)
        {
            tlv.val = val;
            TLV_Status = T_Status;
            valCount = 0;
            mytlvfunc(&tlv);/****�ص�����ÿ�ν�����TLVֵ*****/
            free(val);
        }
        break;

    default:
        break;
    }
    return tmpStatus;

}


s8 tagArr[][2] =
{
    { "\x07\x05" },
    { "\x02\x03" },
};
u8 tagArrLen = ARR_SIZE(tagArr);

/**
* \brief TagCmp TAG�Ƚ�
*
* \param  None
*
* \return None
*/
static ErrorStatus TagCmp (s8 *tagBuf, u8 tagBufCount)
{
    u32 i = 0;
    ErrorStatus tmpStatus = ERROR;
    for (i = 0; i < tagArrLen; i++)
    {
        if (0 == memcmp(tagArr[i], tagBuf, tagBufCount))
        {
            tmpStatus = SUCCESS;
            break;
        }
    }
    return tmpStatus;
}


s8 tagArrUnPack[][2] =
{
    { "\x02\x00" },
    { "\x01\x00" },
    { "\x03\x01" },
};
u8 tagArrUnPackLen = ARR_SIZE(tagArrUnPack);


/**
* \brief TagCmpUnPack TAG�Ƚ�
*
* \param  None
*
* \return None
*/
static ErrorStatus TagCmpUnPack(s8 *tagBuf, u8 tagBufCount)
{
    u32 i = 0;
    ErrorStatus tmpStatus = ERROR;
    for (i = 0; i < tagArrUnPackLen; i++)
    {
        if (0 == memcmp(tagArrUnPack[i], tagBuf, tagBufCount))
        {
            tmpStatus = SUCCESS;
            break;
        }
    }
    return tmpStatus;
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


    u8 tmpUnPackBuf[100] = {
                           0x00,0x01,0x00,0x00,
                           0x03,0x02,0x00,0x01,0x08,0x05,
                           0x03,0x01,0x02,0x03,0x04,0x06,
                           0x02,0x00,0x01,0x08,0x09
    };
    u32 tmpUnPackBufLen = ARR_SIZE(tmpUnPackBuf);
    u32 tmpUnPackBufLenOffset = 0;

    t_TLVEntity tmpTLVEntity1 =
    {
        {"\x02\x03"},//tag
        {"\x08"},    //len
        {"\x01\x03\x04\x01\x09\x04\x01\x03" },//val
        2,//tagSize
        1,//lenSize
        0,//valLen

    };

    t_TLVEntity tmpTLVEntity2 =
    {
        {"\x07\x05"},//tag
        {"\x04"},    //len
        {"\x01\x03\x04\x01" },//val
        2,//tagSize
        1,//lenSize
        0,//valLen
    };




    /************��������ͽ��**************/
    TLV_Pack(tmpPackBuf, &tmpPackBufLen, tmpTLVEntity1);
    TLV_Pack(tmpPackBuf, &tmpPackBufLen, tmpTLVEntity2);
    TLV_Pack(tmpPackBuf, &tmpPackBufLen, tmpTLVEntity2);
    TLV_Pack(tmpPackBuf, &tmpPackBufLen, tmpTLVEntity1);

    for (i = 0; i < tmpPackBufLen; i++)
    {
        TLV_UnPack(TagCmp, tmpPackBuf[i], 2, 1, mytlvfuncUnPack);
    }

    /************�������Խ��**************/
    for (i = 0; i < tmpUnPackBufLen; i++)
    {
        TLV_UnPack(TagCmpUnPack, tmpUnPackBuf[i], 2, 1, mytlvfuncUnPack);
    }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
