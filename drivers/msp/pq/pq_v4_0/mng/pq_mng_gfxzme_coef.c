/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name	: pq_mng_gfxzme_coef.c
  Version	: Initial Draft
  Author	: p00203646
  Created	: 2015/10/15
  Description	:

******************************************************************************/

/* SPDX-License-Identifier: GPL-2.0 */

#include "pq_mng_gfxzme_coef.h"

//=====================Graphic ZME Coefficient START=================//
HI_S16 PQ_s16GZmeCoef_8T8P_Cubic[8][8] =
{
    {  0,  0,	0, 511,	  0,   0,  0,  0}, { -3, 11, -41, 496,	61, -16,  4,  0},
    { -4, 17, -63, 451, 138, -35,  9, -1}, { -4, 18, -70, 386, 222, -52, 14, -2},
    { -3, 17, -65, 307, 307, -65, 17, -3}, { -2, 14, -52, 222, 386, -70, 18, -4},
    { -1,  9, -35, 138, 451, -63, 17, -4}, {  0,  4, -16,  61, 496, -41, 11, -3}
};//normalized ok

HI_S16 PQ_s16GZmeCoef_4T16P_Cubic[16][4] =
{
    {	0, 511,	  0,   0}, { -37, 509,	42,  -2}, { -64, 499,  86,  -9}, { -82, 484, 129, -19},
    { -94, 463, 174, -31}, { -98, 438, 217, -45}, { -98, 409, 260, -59}, { -92, 376, 300, -72},
    { -83, 339, 339, -83}, { -72, 300, 376, -92}, { -59, 260, 409, -98}, { -45, 217, 438, -98},
    { -31, 174, 463, -94}, { -19, 129, 484, -82}, {  -9,  86, 499, -64}, {  -2,	 42, 509, -37}
};//normalized ok

//2.138M
HI_S16 PQ_s16GZmeCoef_4T16P_3M_a13[16][4] =
{
    {135, 230, 135, 12}, {124, 228, 143,  17}, {114, 225, 152,	21}, {104, 222, 160,  26},
    { 94, 218, 168, 32}, { 85, 214, 176,  37}, { 76, 208, 183,	45}, { 67, 203, 190,  52},
    { 59, 197, 197, 59}, { 52, 190, 203,  67}, { 45, 183, 208,	76}, { 37, 176, 214,  85},
    { 32, 168, 218, 94}, { 26, 160, 222, 104}, { 21, 152, 225, 114}, { 17, 143, 228, 124}
};//normalized ok

//5.0625M
HI_S16 PQ_s16GZmeCoef_2T8P_Gus2_6_75M_a0_5[8][2] =
{
    {511, 0}, {453, 59}, {390, 122}, {324, 188}, {256, 256}, {188, 324}, {122, 390}, {59, 453}
};

//3.39M
HI_S16 PQ_s16GZmeCoef_2T8P_4M_0_1[8][2] =
{
    {366, 146}, {337, 175}, {309, 203}, {282, 230}, {256, 256}, {175, 337}, {203, 309}, {230, 282},
};//normalized ok


HI_S16 PQ_s16GZmeCoef_2T16P_Gus2_6M_a0_5[16][2] =
{
    {462, 50 }, {437,  75}, {411, 101}, {386, 126},
    {360, 152}, {334, 178}, {308, 204}, {282, 230},
    {256, 256}, {230, 282}, {204, 308}, {178, 334},
    {152, 360}, {126, 386}, {101, 411}, {75,  437}
};


HI_S16 PQ_s16GZmeCoef_2T16P_4M_0_1[16][2] =
{
    {366, 146}, {351, 161}, {337, 175}, {323, 189},
    {309, 203}, {296, 216}, {282, 230}, {269, 243},
    {256, 256}, {161, 351}, {175, 337}, {189, 323},
    {203, 309}, {216, 296}, {230, 282}, {243, 269},
};//normalized ok



/**================================= begin ==================================**/
/**
 **and from HiFoneB2 4K TinyZME, 2T16P align with 0 by d00241380
 **/
HI_S16 PQ_s16TinyZmeCoef_2T16P_Gus2_6M_a0_5[16][4] =
{
    { 0, 462,  50, 0}, { 0, 437,  75, 0}, { 0, 411, 101, 0}, { 0, 386, 126, 0},
    { 0, 360, 152, 0}, { 0, 334, 178, 0}, { 0, 308, 204, 0}, { 0, 282, 230, 0},
    { 0, 256, 256, 0}, { 0, 230, 282, 0}, { 0, 204, 308, 0}, { 0, 178, 334, 0},
    { 0, 152, 360, 0}, { 0, 126, 386, 0}, { 0, 101, 411, 0}, { 0,  75, 437, 0}
};

HI_S16 PQ_s16TinyZmeCoef_2T16P_4M_0_1[16][4] =
{
    { 0, 366, 146, 0}, { 0, 351, 161, 0}, { 0, 337, 175, 0}, { 0, 323, 189, 0},
    { 0, 309, 203, 0}, { 0, 296, 216, 0}, { 0, 282, 230, 0}, { 0, 269, 243, 0},
    { 0, 256, 256, 0}, { 0, 161, 351, 0}, { 0, 175, 337, 0}, { 0, 189, 323, 0},
    { 0, 203, 309, 0}, { 0, 216, 296, 0}, { 0, 230, 282, 0}, { 0, 243, 269, 0}
};
/**================================== end ====================================**/


/*---------------- Gfx Reduce Zme Coef ----------------------------*/


//=====================Graphic ZME Coefficient END=================//
