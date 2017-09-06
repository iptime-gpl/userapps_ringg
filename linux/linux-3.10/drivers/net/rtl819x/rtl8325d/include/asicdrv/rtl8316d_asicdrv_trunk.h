/*
* Copyright (C) 2009 Realtek Semiconductor Corp.
* All Rights Reserved.
*
* This program is the proprietary software of Realtek Semiconductor
* Corporation and/or its licensors, and only be used, duplicated,
* modified or distributed under the authorized license from Realtek.
*
* ANY USE OF THE SOFTWARE OTEHR THAN AS AUTHORIZED UNDER 
* THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
* 
* $Revision: 1.1.1.1 $
* $Date: 2016/11/01 07:34:14 $
*
* Purpose :  Trunk (link aggregation) ASIC driver related Macro, structure, enumeration, 
*                variable types declaration and definition.
*
*  Feature :  This file consists of following modules:
*             1) 
*
*/

#ifndef __RTL8316D_ASICDRV_TRUNK_H__
#define __RTL8316D_ASICDRV_TRUNK_H__


#define RTK_TRUNK_GROUP_MAX         2      // trunk group number
#define RTK_TRUNK_PORT_NUM_PER_GRP  8      // port number per group

extern int32 rtk_trunk_distributionAlgorithm_get(rtk_trunk_group_t trk_gid, rtk_trunk_hashVal2Port_t *pAlgo_bitmask);
extern int32 rtk_trunk_distributionAlgorithm_set(rtk_trunk_group_t trk_gid, rtk_trunk_hashVal2Port_t algo_bitmask);

#endif /*__RTL8316D_ASICDRV_TRUNK_H__*/

