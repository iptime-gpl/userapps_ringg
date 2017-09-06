/*
* Copyright (C) 2012 Realtek Semiconductor Corp.
* All Rights Reserved.
*
* This program is the proprietary software of Realtek Semiconductor
* Corporation and/or its licensors, and only be used, duplicated,
* modified or distributed under the authorized license from Realtek.
*
* ANY USE OF THE SOFTWARE OTEHR THAN AS AUTHORIZED UNDER 
* THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
* 
* $Revision: 1.3 $
* $Date: 2017/07/04 04:18:52 $
*
* Purpose : realtek common API
*
* Feature :  This file consists of following modules:
*     		1)	Packet length
*			2)	Phy
*			3)    Port isolation
*			4)	Rate
*			5)	Qos
*			6)	IGMP/MLD
*			7)	IPV4/IPV6 unknown multicast 
*			8)	Vlan
*			9)	Stp
*			10)	Lookup table
*			11)	CPU
*			12)	Mirror
*			13)	Dot1x
*			14)	ACL
*			15)  Storm filter
*/

#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/version.h>

#ifdef __KERNEL__
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
#define CONFIG_RTL_PROC_NEW	1
#endif
#endif

#include "rtl8309n_types.h"
#include "rtl8309n_asicdrv.h"
#include "rtl8309n_asicdrv_ext.h"
#include "rtk_api.h"
#include "rtk_api_ext.h"


/* Function Name:
 *      rtk_switch_init
 * Description:
 *       Set chip to default configuration enviroment
 * Input:
 *      none
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                     -  Success
 *      RT_ERR_FAILED               -   Failure
 * Note:
 *      The API can set chip registers to default configuration for
 *      different release chip model.
 */
rtk_api_ret_t rtk_switch_init(void)
{
	rtk_api_ret_t retVal;
	
    retVal = rtl8309n_asic_init();
	if (retVal != SUCCESS)
		return RT_ERR_FAILED;
	
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_switch_maxPktLen_set
 * Description:
 *      Set the max packet length
 * Input:
 *	  type	-	  max packet length type
 *      len       -       max packet length
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                    -  Success
 *      RT_ERR_FAILED               -  Failure
 *      RT_ERR_INPUT                -  Invalid input parameter
 * Note:
 *      The API can set max packet length. The len would be values as follows:
 *          	- MAX_PKTLEN_1522B
 *		- MAX_PKTLEN_1526B
 *          	- MAX_PKTLEN_2048B
 *          	- MAX_PKTLEN_16000B
 *          	- MAX_PKTLEN_USER
 */
rtk_api_ret_t rtk_switch_maxPktLen_set(rtk_switch_maxPktLen_t type, rtk_switch_len_t len)
{
    rtk_api_ret_t retVal;
	uint32 rtl8309n_type;
    uint32 maxlen;

    if (type >= MAX_PKTLEN_END)
        return RT_ERR_INPUT;

    switch(type)
    {
    	case MAX_PKTLEN_1518B:
		
        case MAX_PKTLEN_1522B:
			
		case MAX_PKTLEN_1526B:
			rtl8309n_type = RTL8309N_MAX_PKTLEN_1518;
            maxlen = 1518;
            break;
			
        case MAX_PKTLEN_2048B:
			rtl8309n_type = RTL8309N_MAX_PKTLEN_2048;
            maxlen = 2048;
            break;

		case MAX_PKTLEN_USER:
			rtl8309n_type = RTL8309N_MAX_PKTLEN_USER;
			maxlen = len;
			break; 
			
		case MAX_PKTLEN_16000B:
			rtl8309n_type = RTL8309N_MAX_PKTLEN_16k;
			maxlen = 16000;
			break;
			
        default:
            return FAILED;
    }

    if((retVal = rtl8309n_switch_maxPktLen_set(rtl8309n_type, maxlen)) != SUCCESS)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_switch_maxPktLen_get
 * Description:
 *      Get the max packet length
 * Input:
 *      none
 * Output:
 *	  pType	  -	 the pointer of max packet length type	
 *      pLen       -    the pointer of max packet length
 * Return: 
 *      RT_ERR_OK                    -  Success
 *      RT_ERR_FAILED               -  Failure
 *      RT_ERR_NULL_POINTER     -  Input parameter is null pointer 
 * Note:
 *      The API can get max packet length.The len would be values as follows:
 *          	- MAX_PKTLEN_1522B
 *		- MAX_PKTLEN_1526B
 *          	- MAX_PKTLEN_2048B
 *          	- MAX_PKTLEN_16000B
 *          	- MAX_PKTLEN_USER
 */
rtk_api_ret_t rtk_switch_maxPktLen_get(rtk_switch_maxPktLen_t *pType, rtk_switch_len_t *pLen)
{
    rtk_api_ret_t retVal;
    uint32 maxlen;
	uint32 rtl8309n_type;

    if (NULL == pLen)
        return RT_ERR_NULL_POINTER;
    
    if((retVal = rtl8309n_switch_maxPktLen_get(&rtl8309n_type, &maxlen)) != SUCCESS)
        return retVal;

    switch(rtl8309n_type)
    {
        case RTL8309N_MAX_PKTLEN_1518:
			*pType = MAX_PKTLEN_1522B;
            *pLen = 1522;
            break;

        case RTL8309N_MAX_PKTLEN_2048:
			*pType = MAX_PKTLEN_2048B;
            *pLen = 2048;
            break;
            
        case RTL8309N_MAX_PKTLEN_USER:
			*pType = MAX_PKTLEN_USER;			
            *pLen = maxlen;
            break;
            
        case RTL8309N_MAX_PKTLEN_16k:
            *pType = MAX_PKTLEN_16000B;
			*pLen = 16000;
            break;

        default:
            return FAILED;
    }
    
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_port_phyReg_set
 * Description:
 *      Set PHY register data of the specific port
 * Input:
 *      phy               - phy ID(0 ~ 7) 
 *      reg                - Register id
 *      regData         - Register data
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK         -  Success
 *      RT_ERR_FAILED   -   Failure
 *	  RT_ERR_PORT_ID	-	invalid port id	
 * Note:
 *      This API can be called to write a phy register provided by IEEE standard.
 *      RTL8309N switch has 8 PHYs(PHY 0-7). 
 */
rtk_api_ret_t rtk_port_phyReg_set(rtk_port_t phy, rtk_port_phy_reg_t reg, rtk_port_phy_data_t regData)
{
    rtk_api_ret_t retVal;
    
    if(phy > RTK_PHY_ID_MAX)
        return RT_ERR_PORT_ID;

    retVal = rtl8309n_phyReg_set((uint32)phy, (uint32)reg, 0, (uint32)regData);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;
	
    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_port_phyReg_get
 * Description:
 *      Get PHY register data of the specific port
 * Input:
 *      phy                  - phy number, 0 ~ 7
 *      reg                  - Register id
 * Output:
 *      pData               - the pointer of Register data
 * Return:
 *     RT_ERR_OK        	-  Success
 *     RT_ERR_FAILED   	-  Failure
 *	 RT_ERR_PORT_ID		-	invalid port id
 *	 RT_ERR_NULL_POINTER		-	input parameter is null pointer 
 * Note:
 *      This API can be called to read a PHY register data provided by IEEE standard.
 *      RTL8309N switch has 8 PHYs(PHY 0-7).  
 */
rtk_api_ret_t rtk_port_phyReg_get(rtk_port_t phy, rtk_port_phy_reg_t reg, rtk_port_phy_data_t *pData) 
{
    rtk_api_ret_t retVal;

    if(phy > RTK_PHY_ID_MAX)
        return RT_ERR_PORT_ID;
	if (NULL == pData)
		return RT_ERR_NULL_POINTER;

    retVal = rtl8309n_phyReg_get((uint32)phy, (uint32)reg, 0, (uint32 *)pData);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_port_phyAutoNegoAbility_set
 * Description:
 *      Set ethernet PHY auto-negotiation desired ability
 * Input:
 *      port       -  phy id,0~7
 *      pAbility   -  pointer point to Ability structure
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK       			-  	Success
 *      RT_ERR_FAILED   		-  	Failure
 *	  RT_ERR_PORT_ID			-  	invalid port id
 *	  RT_ERR_NULL_POINTER	-  	input parameter is null pointer 
 *	  RT_ERR_INPUT			-  	invalid input parameter 
 * Note:
 *      (1) RTL8309N switch only has 8 phy, so the input phy id should be 0 ~ 7.
 *      (2) In auto-negotiation mode, phy autoNegotiation ability must be enabled
 */ 
rtk_api_ret_t rtk_port_phyAutoNegoAbility_set(rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    rtk_api_ret_t retVal;     
    uint32 phyData;    
    uint32 phyEnMsk0;
    uint32 phyEnMsk4;

    if(port > RTK_PHY_ID_MAX)
        return RT_ERR_PORT_ID;         

    if(NULL == pAbility)
        return RT_ERR_NULL_POINTER;

    if(pAbility->Half_10 >= RTK_ENABLE_END  || pAbility->Full_10 >= RTK_ENABLE_END ||
       pAbility->Half_100 >= RTK_ENABLE_END || pAbility->Full_100 >= RTK_ENABLE_END ||
       pAbility->AutoNegotiation >= RTK_ENABLE_END ||pAbility->AsyFC >= RTK_ENABLE_END ||
       pAbility->FC >= RTK_ENABLE_END)
        return RT_ERR_INPUT; 

    /*for PHY auto mode setup*/
    pAbility->AutoNegotiation = ENABLED; 

    phyEnMsk0 = 0;
    phyEnMsk4 = 0;
    
    if(1 == pAbility->Half_10)
    {
        /*10BASE-TX half duplex capable in reg 4.5*/
        phyEnMsk4 = phyEnMsk4 | (1<<5);
    }

    if(1 == pAbility->Full_10)
    {
        /*10BASE-TX full duplex capable in reg 4.6*/
        phyEnMsk4 = phyEnMsk4 | (1<<6);        
    }

    if(1 == pAbility->Half_100)
    {
        /*100BASE-TX half duplex capable in reg 4.7*/
        phyEnMsk4 = phyEnMsk4 | (1<<7);
    }

    if(1 == pAbility->Full_100)
    {
        /*100BASE-TX full duplex capable in reg 4.8*/
        phyEnMsk4 = phyEnMsk4 | (1<<8);
    }
    
    if(1 == pAbility->AutoNegotiation)
    {
        /*Auto-Negotiation enable and restart in reg 0.12*/
        phyEnMsk0 |= (1 << 12);

    }

    if(1 == pAbility->AsyFC)
    {
        /*Asymetric flow control in reg 4.11*/
        phyEnMsk4 = phyEnMsk4 | (1<<11);
    }
    
    if(1 == pAbility->FC)
    {
        /*Flow control in reg 4.10*/
        phyEnMsk4 = phyEnMsk4 | (1<<10);
    }

    /*Auto-Negotiation control register setting*/
    if((retVal = rtk_port_phyReg_get(port, (rtk_port_phy_reg_t)PHY_AN_ADVERTISEMENT_REG, &phyData)) != RT_ERR_OK)
        return retVal;

    phyData = (phyData & (~0x0DE0)) | phyEnMsk4;
    if((retVal = rtk_port_phyReg_set(port, (rtk_port_phy_reg_t)PHY_AN_ADVERTISEMENT_REG, phyData)) != RT_ERR_OK)
        return retVal;

    /*Control register setting and restart auto*/
    if((retVal = rtk_port_phyReg_get(port, (rtk_port_phy_reg_t)PHY_CONTROL_REG, &phyData)) != RT_ERR_OK)
        return retVal;
    phyData = (phyData & (~0x3100)) | phyEnMsk0;
    
    /*If have auto-negotiation capable, then restart auto negotiation*/
    if(1 == pAbility->AutoNegotiation)
    {
        phyData = phyData | (1 << 9);
    }

    if((retVal = rtk_port_phyReg_set(port, (rtk_port_phy_reg_t)PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
        return retVal;    

    return RT_ERR_OK;

    
}

/* Function Name:
 *      rtk_port_phyAutoNegoAbility_get
 * Description:
 *      Get ethernet PHY auto-negotiation ability configurations
 * Input:
 *      port       -  phy id,0~7
 * Output:
 *      pAbility   -  pointer point to Ability structure
 * Return:
 *      RT_ERR_OK        		-  Success
 *      RT_ERR_FAILED   	-  Failure
 *	  RT_ERR_PORT_ID		-  invalid port id
 *	  RT_ERR_NULL_POINTER			-	input parameter is null pointer
 *	  RT_ERR_PHY_AUTO_NEGO_MODE	-	invalid PHY auto-negotiation mode
 * Note:
 *      (1) RTL8309N switch only has 8 phy, so the input phy id should be 0~7.
 *      (2) In auto-negotiation mode, phy autoNegotiation ability must be enabled.
 */   
rtk_api_ret_t rtk_port_phyAutoNegoAbility_get(rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
	uint32 phyData0;
	uint32 phyData4;
	rtk_api_ret_t retVal;  
	
	if(port > RTK_PHY_ID_MAX)
		return RT_ERR_PORT_ID; 

	if(NULL == pAbility)
		return RT_ERR_NULL_POINTER;

	/*Control register setting and restart auto*/
	if((retVal = rtk_port_phyReg_get(port, (rtk_port_phy_reg_t)PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
		return retVal;

	/*Auto-Negotiation control register setting*/
	if((retVal = rtk_port_phyReg_get(port, (rtk_port_phy_reg_t)PHY_AN_ADVERTISEMENT_REG, &phyData4)) != RT_ERR_OK)
		return retVal;


	if(phyData4 & (1<<11))
		pAbility->AsyFC = 1;
	else
		pAbility->AsyFC = 0;

	if(phyData4 & (1<<10))
		pAbility->FC = 1;
	else
		pAbility->FC = 0;
	
	
	if(phyData4 & (1<<8))
		pAbility->Full_100= 1;
	else
		pAbility->Full_100= 0;
	
	if(phyData4 & (1<<7))
		pAbility->Half_100= 1;
	else
		pAbility->Half_100= 0;

	if(phyData4 & (1<<6))
		pAbility->Full_10= 1;
	else
		pAbility->Full_10= 0;
	
	if(phyData4 & (1<<5))
		pAbility->Half_10= 1;
	else
		pAbility->Half_10= 0;


	if(phyData0 & (1<<12))
		pAbility->AutoNegotiation= 1;
	else
		pAbility->AutoNegotiation= 0;

	if(!(phyData0 & (1<<12)))
		return RT_ERR_PHY_AUTO_NEGO_MODE;

	return RT_ERR_OK;
}


/* Function Name:
 *     rtk_port_phyForceModeAbility_set
 * Description:
 *      Set ethernet PHY force mode desired ability
 * Input:
 *      port       -  Port id
 *      pAbility   -  pointer point to Ability structure
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK        				-  Success
 *      RT_ERR_FAILED  	 			-  Failure
 *	  RT_ERR_PORT_ID				-  invalid port id
 *	  RT_ERR_NULL_POINTER		-  input parameter is null pointer 
 *	  RT_ERR_INPUT				-  invalid input parameter 
 * Note:
 *      (1) RTL8309N switch only has 8 phy, so the input phy id should be 0~7.
 *      (2) In force mode, phy autoNegotiation ability must be disabled.
 */
rtk_api_ret_t rtk_port_phyForceModeAbility_set(rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    rtk_api_ret_t retVal;           
    uint32 phyData;
    uint32 phyEnMsk0,phyEnMsk4;
    
    if(port > RTK_PHY_ID_MAX)
        return RT_ERR_PORT_ID;  
    
	if(NULL == pAbility)
		return RT_ERR_NULL_POINTER;

	if(pAbility->Half_10 >= RTK_ENABLE_END || pAbility->Full_10 >= RTK_ENABLE_END ||
	   pAbility->Half_100 >= RTK_ENABLE_END || pAbility->Full_100 >= RTK_ENABLE_END ||
	   pAbility->AutoNegotiation >= RTK_ENABLE_END ||pAbility->AsyFC >= RTK_ENABLE_END ||
	   pAbility->FC >= RTK_ENABLE_END)
		return RT_ERR_INPUT; 

	/*for PHY force mode setup*/
	pAbility->AutoNegotiation = 0;
	
	phyEnMsk0 = 0;
	phyEnMsk4 = 0;
	
	if(1 == pAbility->Half_10)
	{
		/*Speed selection in reg 0.13 */
		/* 1= 100Mpbs*/
		/* 0= 10Mpbs*/		  
		phyEnMsk0 = phyEnMsk0 & (~(1<<13));

		/*Half duplex mode in reg 0.8*/
		phyEnMsk0 = phyEnMsk0 & (~(1<<8));
	}

	if(1 == pAbility->Full_10)
	{
		/*Speed selection in reg 0.13 */
		/* 1= 100Mpbs*/
		/* 0= 10Mpbs*/		  
		phyEnMsk0 = phyEnMsk0 & (~(1<<13));

		/*Full duplex mode in reg 0.8*/
		phyEnMsk0 = phyEnMsk0 | (1<<8);
		
	}

	if(1 == pAbility->Half_100)
	{
		/*Speed selection in reg 0.13 */
		/* 1= 100Mpbs*/
		/* 0= 10Mpbs*/		 
		phyEnMsk0 = phyEnMsk0 | (1<<13);
		
		/*Half duplex mode in reg 0.8*/
		phyEnMsk0 = phyEnMsk0 & (~(1<<8));
	}


	if(1 == pAbility->Full_100)
	{
		/*Speed selection in reg 0.13 */
		/* 1= 100Mpbs*/
		/* 0= 10Mpbs*/		 
		phyEnMsk0 = phyEnMsk0 | (1<<13);
		
		/*Full duplex mode in reg 0.8*/
		phyEnMsk0 = phyEnMsk0 | (1<<8);
	}
		
	if(1 == pAbility->AsyFC)
	{
		/*Asymetric flow control in reg 4.11*/
		phyEnMsk4 = phyEnMsk4 | (1<<11);
	}
	if(1 == pAbility->FC)
	{
		/*Flow control in reg 4.10*/
		phyEnMsk4 = phyEnMsk4 | (1<<10);
	}
	
	/*Auto-Negotiation control register setting*/
	if((retVal = rtk_port_phyReg_get(port, (rtk_port_phy_reg_t)PHY_AN_ADVERTISEMENT_REG, &phyData))!= RT_ERR_OK)
		return retVal;

	phyData = (phyData & (~0x0DE0)) | phyEnMsk4;
	if((retVal = rtk_port_phyReg_set(port, (rtk_port_phy_reg_t)PHY_AN_ADVERTISEMENT_REG, phyData)) != RT_ERR_OK)
		return retVal;

	/*Control register setting */
	if((retVal = rtk_port_phyReg_get(port, (rtk_port_phy_reg_t)PHY_CONTROL_REG, &phyData)) != RT_ERR_OK)
		return retVal;
	phyData = (phyData & (~0x3100)) | phyEnMsk0;
	if((retVal = rtk_port_phyReg_set(port, (rtk_port_phy_reg_t)PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
		return retVal;
	
	   
	return RT_ERR_OK;
}



/* Function Name:
 *      rtk_port_phyForceModeAbility_get
 * Description:
 *      Get ethernet PHY force mode ability configuration
 * Input:
 *      port       -  Port id
 * Output:
 *      pAbility   -  pointer point to Ability structure
 * Return:
 *      RT_ERR_OK        					-   Success
 *	  RT_ERR_PORT_ID					-   invalid port id
 *	  RT_ERR_NULL_POINTER			-   input parameter is null pointer  
 *	  RT_ERR_PHY_AUTO_NEGO_MODE	-   invalid PHY auto-negotiation mode
 * Note:
 *      (1) RTL8309N switch only has 8 phy, so the input phy id should be 0~7.
 *      (2) In force mode, phy autoNegotiation ability must be disabled.
 */
rtk_api_ret_t rtk_port_phyForceModeAbility_get(rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    uint32 phyData0,phyData4;
    rtk_api_ret_t retVal;    
    
    if(port > RTK_PHY_ID_MAX)
        return RT_ERR_PORT_ID; 

 
	if(NULL == pAbility)
		 return RT_ERR_NULL_POINTER;
				  
 
	 /*Control register setting and restart auto*/
	 if((retVal = rtk_port_phyReg_get(port, (rtk_port_phy_reg_t)PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
		 return retVal;
 
	 /*Auto-Negotiation control register setting*/
	 if((retVal = rtk_port_phyReg_get(port, (rtk_port_phy_reg_t)PHY_AN_ADVERTISEMENT_REG, &phyData4)) != RT_ERR_OK)
		 return retVal;
 
	 if(phyData0 & (1<<12))
		 return RT_ERR_PHY_FORCE_ABILITY;
 
	 if(phyData4 & (1<<11))
		 pAbility->AsyFC = 1;
	 else
		 pAbility->AsyFC = 0;
 
	 if(phyData4 & (1<<10))
		 pAbility->FC = 1;
	 else
		 pAbility->FC = 0;
	 
 
	 pAbility->Full_100 = 0;
	 pAbility->Half_100 = 0;
	 pAbility->Full_10 = 0;    
	 pAbility->Half_10 = 0;
	 pAbility->Full_1000 = 0;
 
	 if(phyData0 & (1<<8) && phyData0 & (1<<13))
		 pAbility->Full_100= 1;
	 
	 if(!(phyData0 & (1<<8)) && phyData0 & (1<<13))
		 pAbility->Half_100= 1;
 
	 if(phyData0 & (1<<8) && !(phyData0 & (1<<13)))
		 pAbility->Full_10= 1;
	 
	 if(!(phyData0 & (1<<8)) && !(phyData0 & (1<<13)))
		 pAbility->Half_10= 1;
 
	 if(phyData0 & (1<<12))
		 pAbility->AutoNegotiation= 1;
	 else
		 pAbility->AutoNegotiation= 0;
 
	 return RT_ERR_OK;
 }
 
 /* Function Name:
  * 	 rtk_port_phyLinkStatus_get
  * Description:
  * 	 Get ethernet PHY link status
  * Input:
  * 	 phy		-  phy id,0-7
  * Output:
  * 	 pLinkStatus	-  pointer point to phy link status 
  * Return:
  * 	 RT_ERR_OK		  		-  	Success
  * 	 RT_ERR_PORT_ID  		-	 invalid port id
  * 	 RT_ERR_NULL_POINTER	 -	 input parameter is null pointer  
  * Note:
  * 	 (1) The link status of phy would be values as follows:
  *		-	PORT_LINKUP
  *		-	PORT_LINKDOWN
  *	 (2) The speed could be values as  follows:
  *		-	PORT_SPEED_10M
  *		-	PORT_SPEED_100M
  *	 (3) The duplex could be values as follows:
  *		-	PORT_HALF_DUPLEX
  *		-	PORT_FULL_DUPLEX
  */
rtk_api_ret_t rtk_port_phyStatus_get(rtk_port_t phy, rtk_port_linkStatus_t *pLinkStatus, rtk_port_speed_t *pSpeed, rtk_port_duplex_t *pDuplex)
{
	uint32 regVal;
	int32 retVal;
	
    if(phy > RTK_PHY_ID_MAX)
        return RT_ERR_PORT_ID; 

	if(NULL == pLinkStatus)
		return RT_ERR_NULL_POINTER;
	
    if((retVal= rtl8309n_reg_get(2, 18,14,&regVal)) != SUCCESS)
        return RT_ERR_FAILED;

    if(regVal & (0x1 << phy))
    {
        *pLinkStatus = PORT_LINKUP;
		 if((retVal= rtl8309n_reg_get(2, 19,14,&regVal)) != SUCCESS)
        	return RT_ERR_FAILED;
         
        *pSpeed = (regVal & (0x1 << phy)) ? PORT_SPEED_100M : PORT_SPEED_10M; 

		if((retVal= rtl8309n_reg_get(2, 20,14,&regVal)) != SUCCESS)
        	return RT_ERR_FAILED;
		
        *pDuplex = (regVal & (0x1 << phy)) ? PORT_FULL_DUPLEX : PORT_HALF_DUPLEX;
    }
    else
    {
        *pLinkStatus = PORT_LINKDOWN;
        *pSpeed = PORT_SPEED_10M;
        *pDuplex = PORT_HALF_DUPLEX; 
    }
	rtlglue_printf("phy = %d, link = %s,speed=%s duplex =%s\n",phy, *pLinkStatus==PORT_LINKUP?"up":"down", *pSpeed==PORT_SPEED_100M?"100M":"10M",*pDuplex==PORT_FULL_DUPLEX?"FULL":"HALF");
	return RT_ERR_OK;
}

#if 0
/* Function Name:
 *      rtk_port_macForceLinkExt0_set
 * Description:
 *      Set MAC 8 force mode ability.
 * Input:
 *      mode			- 	external interface mode
 *      pPortability		- 	pointer point to port ability configuration
 * Output:
 *      None 
 * Return:
 *      RT_ERR_OK               - 	Success
 *      RT_ERR_FAILED           - 	Failure
 *      RT_ERR_INPUT       		- 	Invalid input parameters.
 *	 RT_ERR_NULL_POINTER		-	input parameter is null pointer
 * Note:
 *	 (1)This API can set MAC 8 force mode properties,
 *      	including link status, speed, duplex, tx pause and tx pause ability.
 *      	In force mode, autoNegotiation ability must be disabled.
 *	(2) Port mac mode could be:
 *		-	MODE_EXT_TMII_MAC
 *		-	MODE_EXT_TMII_PHY
 *		-	MODE_EXT_MII_MAC
 *		-	MODE_EXT_MII_PHY
 *		-	MODE_EXT_RMII
 *	(3) The port mac link status could be:
 *		-	PORT_LINKUP
 *		-	PORT_LINKDOWN
 *	(4)The port mac speed could be:
 *		-	PORT_SPEED_10M
 *		-	PORT_SPEED_20M
 *		-	PORT_SPEED_100M	
 *		-	PORT_SPEED_200M	
 *    (5)The port mac duplex could be:
 *           -	PORT_FULL_DUPLEX
 *           -	PORT_HALF_DUPLEX
 *              
 */
rtk_api_ret_t rtk_port_macForceLinkExt0_set(rtk_mode_ext_t mode, rtk_port_mac_ability_t *pPortability)
{
    rtk_api_ret_t retVal;
	rtl8309n_mode_ext_t rtl8309n_mode;
	rtl8309n_port_mac_ability_t rtl8309n_portAbility;

	if (mode > MODE_EXT_END)
		return RT_ERR_INPUT;
	
	if (NULL == pPortability)
		return RT_ERR_NULL_POINTER;
	
    if ((pPortability->speed >= PORT_SPEED_END) || (pPortability->duplex >= PORT_DUPLEX_END) || 
		(pPortability->txpause >= RTK_ENABLE_END) || (pPortability->rxpause >= RTK_ENABLE_END) || 
		(pPortability->link >= PORT_LINKSTATUS_END) || (pPortability->nway >= RTK_ENABLE_END))
        return RT_ERR_INPUT;

	if((pPortability->speed == PORT_SPEED_20M|| pPortability->speed == PORT_SPEED_200M)
		&& mode != MODE_EXT_TMII_MAC && MODE_EXT_TMII_PHY)
		return FAILED;

	if(pPortability->speed != PORT_SPEED_20M && pPortability->speed != PORT_SPEED_200M
		&& (mode == MODE_EXT_TMII_MAC || mode == MODE_EXT_TMII_PHY))
		return FAILED;
	
	/*set interfeace mode ,MII/TMII/RMII*/
	/*set Speed, when mac8 is at TMII mode, then 10M means 20Mbps; 100M means 200Mbps*/
	switch(mode)
	{
		case MODE_EXT_TMII_MAC:		/*TMII MAC, 200M/20M*/
			rtl8309n_mode = RTL8309N_MODE_EXT_TMII_MAC;
			break;
			
		case MODE_EXT_TMII_PHY:		/*TMII PHY, 200M/20M*/
			rtl8309n_mode = RTL8309N_MODE_EXT_TMII_PHY;
			break;
			
		case MODE_EXT_MII_MAC:		/*MII MAC, 100M/10M*/
			rtl8309n_mode = RTL8309N_MODE_EXT_MII_MAC;
			break;
			
		case MODE_EXT_MII_PHY:		/*MII PHY, 100M/10M*/
			rtl8309n_mode = RTL8309N_MODE_EXT_MII_PHY;
			break;
			
		case MODE_EXT_RMII:			/*RMII MAC, 100M/10M*/
			rtl8309n_mode = RTL8309N_MODE_EXT_RMII;	
			break;

		default:
			rtl8309n_mode = RTL8309N_MODE_EXT_END;
			break;
	}
	
	/*disabled MAC 8 Nway ability*/
	rtl8309n_portAbility.nway = pPortability->nway;
	/*set MAC 8 link status*/
	rtl8309n_portAbility.link = (PORT_LINKUP == pPortability->link) ? RTL8309N_PORT_LINK_UP : RTL8309N_PORT_LINK_DOWN;
    /*set MAC 8 duplex, when mac8 is at TMII mode*/
	switch(pPortability->speed)
	{
		case PORT_SPEED_10M:
			rtl8309n_portAbility.speed = RTL8309N_PORT_SPEED_10M;
			break;
		case PORT_SPEED_20M:
			rtl8309n_portAbility.speed = RTL8309N_PORT_SPEED_20M;
			break;	
		case PORT_SPEED_100M:
			rtl8309n_portAbility.speed = RTL8309N_PORT_SPEED_100M;
			break;
		case PORT_SPEED_200M:
			rtl8309n_portAbility.speed = RTL8309N_PORT_SPEED_200M;
			break;	
		default:
			break;
	}
	switch(pPortability->duplex)
	{
		case PORT_HALF_DUPLEX:
			rtl8309n_portAbility.duplex = RTL8309N_PORT_HALF_DUPLEX;
			break;
		case PORT_FULL_DUPLEX:
			rtl8309n_portAbility.duplex = RTL8309N_PORT_FULL_DUPLEX;
			break;	
		default:
			break;
	}
	/*Set rx pause and rx pause for MAC 8*/
	rtl8309n_portAbility.txpause = (ENABLED == pPortability->txpause) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
	rtl8309n_portAbility.rxpause = (ENABLED == pPortability->rxpause) ? RTL8309N_ENABLED : RTL8309N_DISABLED;

	/*call driver function*/
	retVal = rtl8309n_port_macForceLinkExt0_set(rtl8309n_mode, &rtl8309n_portAbility);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	
	return RT_ERR_OK;
}

/* Function Name:
 *      rtk_port_macForceLinkExt0_get
 * Description:
 *      Get MAC 8 force mode ability configuration.
 * Input:
 *      None
 * Output:
 *      pMode 		- 	external interface mode
 *      pPortability 	- 	pointer point to port ability configuration
 * Return:
 *      RT_ERR_OK               		- 	Success
 *      RT_ERR_FAILED          	- 	Failure 
 *      RT_ERR_INPUT 			- 	Invalid input parameters.
 *	  RT_ERR_NULL_POINTER	-	input parameter is null pointer 
 * Note:
 *	(1)This API can get MAC 8 force mode ability configuration.
 *	(2) Port mac mode could be:
 *		-	MODE_EXT_TMII_MAC
 *		-	MODE_EXT_TMII_PHY
 *		-	MODE_EXT_MII_MAC
 *		-	MODE_EXT_MII_PHY
 *		-	MODE_EXT_RMII
 *	(3) The port mac link status could be:
 *		-	PORT_LINKUP
 *		-	PORT_LINKDOWN
 *	(4)The port mac speed could be:
 *		-	PORT_SPEED_10M
 *		-	PORT_SPEED_20M
 *		-	PORT_SPEED_100M	
 *		-	PORT_SPEED_200M	
 *    (5)The port mac duplex could be:
 *           -	PORT_FULL_DUPLEX
 *           -	PORT_HALF_DUPLEX
 *              
 */
rtk_api_ret_t rtk_port_macForceLinkExt0_get(rtk_mode_ext_t *pMode, rtk_port_mac_ability_t *pPortability)
{
    rtk_api_ret_t retVal;
	rtl8309n_mode_ext_t rtl8309n_mode;
	rtl8309n_port_mac_ability_t rtl8309n_portAbility;
	
	if ((NULL == pMode) || (NULL == pPortability))
		return RT_ERR_NULL_POINTER;

	/*call driver function*/
	retVal = rtl8309n_port_macForceLinkExt0_get(&rtl8309n_mode, &rtl8309n_portAbility); 
	if (retVal != SUCCESS)
		return RT_ERR_FAILED;
	
	/*interfeace mode*/
	switch(rtl8309n_mode)
	{
		case RTL8309N_MODE_EXT_TMII_MAC:
			*pMode = MODE_EXT_TMII_MAC;
			break;

		case RTL8309N_MODE_EXT_TMII_PHY:
			*pMode = MODE_EXT_TMII_PHY;
			break;

		case RTL8309N_MODE_EXT_MII_MAC:
			*pMode = MODE_EXT_MII_MAC;
			break;

		case RTL8309N_MODE_EXT_MII_PHY:
			*pMode = MODE_EXT_MII_PHY;
			break;

		case RTL8309N_MODE_EXT_RMII:
			*pMode = MODE_EXT_RMII;
			break;

		default:
			*pMode = MODE_EXT_END;
			break;
	}

	/*get Nway status*/
	pPortability->nway = rtl8309n_portAbility.nway;

	/*get link status*/
	pPortability->link = (RTL8309N_PORT_LINK_UP == rtl8309n_portAbility.link) ? PORT_LINKUP : PORT_LINKDOWN;
	/*get speed and duplex*/
	switch(rtl8309n_portAbility.speed)
	{
		case RTL8309N_PORT_SPEED_10M:
			pPortability->speed = PORT_SPEED_10M;
			break;
			
		case RTL8309N_PORT_SPEED_20M:
			pPortability->speed = PORT_SPEED_20M;
			break;
			
		case RTL8309N_PORT_SPEED_100M:
			pPortability->speed = PORT_SPEED_100M;
			break;
			
		case RTL8309N_PORT_SPEED_200M:
			pPortability->speed = PORT_SPEED_200M;
			break;	
		default:
			break;
	}
	switch(rtl8309n_portAbility.duplex)
	{
		case RTL8309N_PORT_HALF_DUPLEX:
			pPortability->duplex = PORT_HALF_DUPLEX;
			break;
			
		case RTL8309N_PORT_FULL_DUPLEX:
			pPortability->duplex = PORT_FULL_DUPLEX;
			break;
		default:
			break;
	}

	/*get RX pause*/
	pPortability->rxpause = rtl8309n_portAbility.rxpause;
	/*get TX pause*/
	pPortability->txpause = rtl8309n_portAbility.txpause;

	return RT_ERR_OK;
}
#endif

/* Function Name:
 *      rtk_port_isolation_set
 * Description:
 *      Set permitted port isolation portmask
 * Input:
 *      port                - 		port id
 *      portmask         - 		Permit port mask
 * Output:
 *      none
 * Return:
 *      RT_ERR_FAILED         -   Failure
 *      RT_ERR_OK               -   Success
 *      RT_ERR_PORT_ID       -   Invalid port number
 *      RT_ERR_PORT_MASK 	-   Invalid portmask
 * Note:
 *      This API can be called to set port isolation mask for port 0~8.
 *      A port can only transmit packet to ports included in permitted portmask.  
 */
rtk_api_ret_t rtk_port_isolation_set(rtk_port_t port, rtk_portmask_t portmask)
{
    rtk_api_ret_t retVal;
	uint32 isomask;
	
    if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;

    if( portmask.bits[0] > RTK_MAX_PORT_MASK)
        return RT_ERR_PORT_MASK;
	
    isomask = portmask.bits[0];
    if((retVal= rtl8309n_port_isolation_set(port, isomask)) != SUCCESS)
        return RT_ERR_FAILED;
    
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_port_isolation_get
 * Description:
 *      Get permitted port isolation portmask
 * Input:
 *      port                - port id
 * Output:
 *      pPortmask       - the pointer of permit port mask
 * Return:
 *      RT_ERR_OK                  -   Success
 *      RT_ERR_FAILED             -   Failure
 *      RT_ERR_PORT_ID          -   Invalid port number
 *      RT_ERR_NULL_POINTER  -   Input parameter is a null pointer
 * Note:
 *      This API can be called to get port isolation mask.
 *      A port can only transmit packet to ports included in permitted portmask  
 */
rtk_api_ret_t rtk_port_isolation_get(rtk_port_t port, rtk_portmask_t *pPortmask)
{
	uint32 isoMask;
	uint32 retVal;
    
    if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;
    if (NULL == pPortmask)
        return RT_ERR_NULL_POINTER;
    
    if((retVal = rtl8309n_port_isolation_get(port, &isoMask)) != RT_ERR_OK)
		return RT_ERR_FAILED;
    
    pPortmask->bits[0] = isoMask & 0x1FF;
    
    return RT_ERR_OK;
}



/* Function Name:
 *	  rtk_rate_igrBandwidthCtrlRate_set
 * Description:
 *      Set port ingress bandwidth control.
 * Input:
 *      port            		-  Port id
 *      rate            		-  Rate of share meter
 *      ifg_include     	-  Rate calculation including IFG
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK              		-  Success
 *      RT_ERR_FAILED         		-  Failure
 *      RT_ERR_PORT_ID         	-  Invalid port id
 *      RT_ERR_ENABLE          	-  invalid enable parameter
 *      RT_ERR_INBW_RATE       	-  invalid input bandwidth 
 * Note:
 *	  For RTL8309N, port0 and port 8's max speed could be 100Mbps, 
 *	  and max speed could only be 100Mbps for port1 to port 7.
 *      The rate unit is 64Kbps and the range is from 64Kbps to 100Mbps. 
 *	  The granularity of rate is 64Kbps. 
 *      The ifg_include parameter is used for rate calculation with or without 
 *	  interframe gap and preamble. 
 */
rtk_api_ret_t rtk_rate_igrBandwidthCtrlRate_set(rtk_port_t port, rtk_rate_t rate, rtk_enable_t ifg_include)
{
    rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    
    
    if ((RTL8309N_PORT0 == port || RTL8309N_PORT8 == port) && rate > RTL8309N_QOS_RATE_INPUT_MAX_P0_P8) 
        return RT_ERR_INBW_RATE;
	if(RTL8309N_PORT1 <= port && RTL8309N_PORT7 >= port &&  rate > RTL8309N_QOS_RATE_INPUT_MAX)
		return RT_ERR_INBW_RATE;
    if(ifg_include >= RTK_ENABLE_END)
        return RT_ERR_ENABLE;

	/*ingress rate*/
	retVal = rtl8309n_qos_portLeakyBktEnable_set(port, RTL8309N_PORT_RX, RTL8309N_ENABLED);
	if (retVal != SUCCESS)
		return RT_ERR_FAILED;
    if((retVal = rtl8309n_qos_portRate_set(port, rate, RTL8309N_PORT_RX)) != RT_ERR_OK)
        return RT_ERR_FAILED;
	/*IFG include*/
	rtl8309n_enabled = (ENABLED == ifg_include) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
    retVal= rtl8309n_qos_portRate_IfgIncludeEnable_set(RTL8309N_PORT_RX, port, rtl8309n_enabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	
    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_rate_igrBandwidthCtrlRate_get
 * Description:
 *      Get port ingress bandwidth control
 * Input:
 *      port               -  Port id, 0~8
 * Output:
 *      pRate             -  the pointer of rate of share meter
 *      pIfg_include    -  Rate's calculation including IFG
 * Return:
 *      RT_ERR_OK                      	-  Success
 *      RT_ERR_FAILED                  	-  Failure
 *      RT_ERR_PORT_ID                 	-  Invalid port id
 *      RT_ERR_NULL_POINTER      	-  input parameter is null pointer
 * Note:
 *	  For RTL8309N, port0 and port 8's max speed could be 100Mbps, 
 *	  and max speed could only be 100Mbps for port1 to port 7.
 *      The rate unit is 64Kbps and the range is from 64Kbps to 100Mbps. 
 *	  The granularity of rate is 64Kbps. 
 *      The ifg_include parameter is used for rate calculation with or without 
 *	  interframe gap and preamble. 
 */
rtk_api_ret_t rtk_rate_igrBandwidthCtrlRate_get(rtk_port_t port, rtk_rate_t *pRate, rtk_enable_t *pIfg_include)
{
    rtk_api_ret_t retVal;
    uint32 rtl8309n_enabled;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    
    if((NULL == pRate) || (NULL == pIfg_include))
        return RT_ERR_NULL_POINTER;
	
	/*ingress rate*/
    if((retVal = rtl8309n_qos_portRate_get(port, pRate, RTL8309N_PORT_RX)) != SUCCESS)
        return retVal;
	/*IFG include*/
    retVal= rtl8309n_qos_portRate_IfgIncludeEnable_get(RTL8309N_PORT_RX, port, &rtl8309n_enabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	*pIfg_include = (RTL8309N_ENABLED == rtl8309n_enabled) ? ENABLED : DISABLED;

    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_rate_egrBandwidthCtrlRate_set
 * Description:
 *      Set port egress bandwidth control
 * Input:
 *      port            -  Port id
 *      rate            -  Rate of bandwidth control
 *      ifg_include   -  Rate's calculation including IFG
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                   -  Success
 *      RT_ERR_FAILED             -  Failure
 *      RT_ERR_PORT_ID           -  invalid port id  
 *      RT_ERR_ENABLE             -  invalid enable parameter  
 *      RT_ERR_QOS_EBW_RATE       -  invalid egress bandwidth rate
 * Note:
 *	  For RTL8309N, port0 and port 8's max speed could be 100Mbps, 
 *	  and max speed could only be 100Mbps for port from port 1 to port 7.
 *      The rate unit is 64Kbps and the range is from 64Kbps to 100Mbps.
 *	  The granularity of rate is 64Kbps. The ifg_include parameter is used 
 *	  for rate calculation with or without interframe gap and preamble. 
 */
rtk_api_ret_t rtk_rate_egrBandwidthCtrlRate_set(rtk_port_t port, rtk_rate_t rate, rtk_enable_t ifg_include)
{
    rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;
	
    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    
    
    if ((RTL8309N_PORT0 == port || RTL8309N_PORT8 == port) && rate > RTL8309N_QOS_RATE_INPUT_MAX_P0_P8) 
        return RT_ERR_QOS_EBW_RATE;
	if (RTL8309N_PORT1 <= port && RTL8309N_PORT7 >= port &&  rate > RTL8309N_QOS_RATE_INPUT_MAX)
		return RT_ERR_QOS_EBW_RATE;
    if(ifg_include >= RTK_ENABLE_END)
        return RT_ERR_ENABLE;

	retVal = rtl8309n_qos_portLeakyBktEnable_set(port, RTL8309N_PORT_TX, RTL8309N_ENABLED);
	if (retVal != SUCCESS)
		return RT_ERR_FAILED;
    if((retVal = rtl8309n_qos_portRate_set(port, rate, RTL8309N_PORT_TX)) != RT_ERR_OK)
        return RT_ERR_FAILED;

	rtl8309n_enabled = (ENABLED == ifg_include) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
    retVal= rtl8309n_qos_portRate_IfgIncludeEnable_set(RTL8309N_PORT_TX, port, rtl8309n_enabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;

    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_rate_egrBandwidthCtrlRate_get
 * Description:
 *      Get port egress bandwidth control
 * Input:
 *      port             -  Port id
 * Output:
 *      pRate           -  the pointer of rate of bandwidth control
 *      pIfg_include    -  Rate's calculation including IFG, enable or disable
 * Return:
 *      RT_ERR_OK                     -  Success
 *      RT_ERR_PORT_ID             -  Invalid port number
 *      RT_ERR_FAILED                -  Failure
 *      RT_ERR_NULL_POINTER      -  null pointer
 * Note:
 *	  For RTL8309N, port0 and port 8's max speed could be 100Mbps, 
 *	  and max speed could only be 100Mbps for port from port 1 to port 7.
 *      The rate unit is 64Kbps and the range is from 64Kbps to 100Mbps.
 *	  The granularity of rate is 64Kbps. The ifg_include parameter is used 
 *	  for rate calculation with or without interframe gap and preamble. 
 */
rtk_api_ret_t rtk_rate_egrBandwidthCtrlRate_get(rtk_port_t port, rtk_rate_t *pRate, rtk_enable_t *pIfg_include)
{
    rtk_api_ret_t retVal;
    uint32 enabled;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    

    if((NULL == pRate) || (NULL == pIfg_include))
        return RT_ERR_NULL_POINTER;
    
    if((retVal = rtl8309n_qos_portRate_get(port, pRate, RTL8309N_PORT_TX)) != SUCCESS)
        return retVal;

    retVal= rtl8309n_qos_portRate_IfgIncludeEnable_get(RTL8309N_PORT_TX, port, &enabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	*pIfg_include = (RTL8309N_ENABLED == enabled) ? ENABLED : DISABLED;

    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_qos_init
 * Description:
 *      Configure Qos with default settings
 * Input:
 *      queueNum     -  Queue number of each port(from 1 to 4)
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                -  Success
 *      RT_ERR_FAILED            -  Failure
 *      RT_ERR_QUEUE_NUM         -  invalid queue number 
 * Note:
 *    This API will initialize related Qos function. First it will set the ASIC's queue number 
 *	  globally for all port. Then it will set priority to queue mapping table based on the queue number
 *	  for all ports. And it will enable output and input flow control abilities.
 */
rtk_api_ret_t rtk_qos_init(rtk_queue_num_t queueNum)
{
    uint32 queue, port;
	
    /*queue number set*/
    if (rtl8309n_qos_queueNum_set(queueNum) == FAILED)
        return RT_ERR_FAILED;
	
    /*priority to queue mapping*/
    switch(queueNum)
    {
        case 1:
			for (port = 0; port < RTK_MAX_NUM_OF_PORT; port++)
			{
            	rtl8309n_qos_priToQueMap_set(port, 0, 0);
            	rtl8309n_qos_priToQueMap_set(port, 1, 0);       
            	rtl8309n_qos_priToQueMap_set(port, 2, 0);                   
            	rtl8309n_qos_priToQueMap_set(port, 3, 0);  
			}
            break;
            
        case 2:
			for (port = 0; port < RTK_MAX_NUM_OF_PORT; port++)
			{
            	rtl8309n_qos_priToQueMap_set(port, 0, 0);
            	rtl8309n_qos_priToQueMap_set(port, 1, 0);       
            	rtl8309n_qos_priToQueMap_set(port, 2, 1);                   
            	rtl8309n_qos_priToQueMap_set(port, 3, 1); 
			}
            break;
            
        case 3:
			for (port = 0; port < RTK_MAX_NUM_OF_PORT; port++)
			{
            	rtl8309n_qos_priToQueMap_set(port, 0, 0);
            	rtl8309n_qos_priToQueMap_set(port, 1, 1);       
            	rtl8309n_qos_priToQueMap_set(port, 2, 1);                   
            	rtl8309n_qos_priToQueMap_set(port, 3, 2); 
			}
            break;            
            
        case 4:
			for (port = 0; port < RTK_MAX_NUM_OF_PORT; port++)
			{
            	rtl8309n_qos_priToQueMap_set(port, 0, 0);
            	rtl8309n_qos_priToQueMap_set(port, 1, 1);       
            	rtl8309n_qos_priToQueMap_set(port, 2, 2);                   
            	rtl8309n_qos_priToQueMap_set(port, 3, 3); 
			}
            break;
            
        default:
            return RT_ERR_QUEUE_NUM;
            
    }

	/*output flow control enable*/
    for (port = 0; port < RTK_MAX_NUM_OF_PORT; port++)
    {			
    	/*port discriptor based output flow control enable*/
		rtl8309n_qos_portDscFlcEnable_set(port, RTL8309N_ENABLED);
		/*queue based output flow control enble*/
		for(queue = 0; queue < queueNum; queue++)
        {
		  rtl8309n_qos_queueFlcEnable_set(port, queue, RTL8309N_ENABLED);
        }

		/*output flow control enble*/
		rtl8309n_qos_egrFlcEnable_set(port, RTL8309N_ENABLED);
    }
	
	/*input flow control enable*/
	rtl8309n_qos_igrFlcEnable_set(RTL8309N_ENABLED);
	
	/*software reset*/
    rtl8309n_qos_softReset_set();    

    return RT_ERR_OK;
}


#if 0

/* Function Name:
 *      rtk_qos_flcEnable_set
 * Description:
 *      Enable/disable Qos flow control ability for the flow control type defined by the user. 
 * Input:
 *		type	 -  flow control type
 *      port     -  port id
 *		queue	 -  queue id
 *		enabled   -  DISABLED or ENABLED
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                     -  Success
 *      RT_ERR_FAILED                -  Failure
 *		RT_ERR_INPUT				-	invalid input parameter
 *      RT_ERR_PORT_ID				-  Invalid port id
 *		RT_ERR_QUEUE_ID				-  	invalid queue id  
 * Note:
 *    This API will enable Qos flow control ability for the flow control type defined by the user.
 *	  The type parameter could be one of 4 values below:
 *        	0 : RTL8309N_EGR_PDSCFLC, egress port discriptor based flow control type.
 *			1 : RTL8309N_EGR_QFLC, egress queue based flow control type. This type will enable or disable 
 *				all the queue flow control ability, whether it's queue discriptor based or queue packet based.
 *			2 : RTL8309N_EGR_PFLC, egress port based flow control type. This type will enable the whole output flow control
 *				ability per port, whether it's queue based or port discriptor based.
 *			3 : RTL8309N_IGR_FLC, ingress flow control type. This type will enable the whole ingress flow control ability
 *				for RTL8309N.
 *    The port id is from 0 to 8. The queue id is from 0 to 3. 
 */
rtk_api_ret_t rtk_qos_flcEnable_set(rtk_qos_flctype_t type, rtk_port_t port, rtk_queue_num_t queue, rtk_enable_t enabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;

	if(type >= RTK_QOS_FLCTYPE_END)
		return RT_ERR_INPUT;
	if(port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
	if(queue > RTK_QUEUE_ID_MAX)
		return RT_ERR_QUEUE_ID;

	rtl8309n_enabled = (ENABLED == enabled) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
	switch(type)
	{
		case RTK_EGR_PDSCFLC:
			if((retVal = rtl8309n_qos_portDscFlcEnable_set(port, rtl8309n_enabled)) != SUCCESS)
			   return RT_ERR_FAILED;
			 break;
		case RTK_EGR_QFLC:
			 if((retVal = rtl8309n_qos_queueFlcEnable_set(port, queue, rtl8309n_enabled)) != SUCCESS)
        		return RT_ERR_FAILED;
			 break;
		case RTK_EGR_PFLC:
			if((retVal = rtl8309n_qos_egrFlcEnable_set(port, rtl8309n_enabled)) != SUCCESS)
			   return RT_ERR_FAILED;
			 break;
		case RTK_IGR_FLC:
			if((retVal = rtl8309n_qos_igrFlcEnable_set(rtl8309n_enabled)) != SUCCESS)
			   return RT_ERR_FAILED;
			break;
		default: 
			 return RT_ERR_FAILED;
			 break;
	}
	
	return RT_ERR_OK;
}


/* Function Name:
 *      rtk_qos_flcEnable_get
 * Description:
 *      get the status of Qos flow control ability for the flow control type defined by the user. 
 * Input:
 *		type	 -  flow control type
 *      port     -  port id
 *		queue	 -  queue id
 *		enable   -  DISABLED or ENABLED
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                     -  Success
 *      RT_ERR_FAILED                -  Failure
 *		RT_ERR_INPUT			-	invalid input parameter
 *      RT_ERR_PORT_ID				-  invalid port id 
 *		RT_ERR_QUEUE_ID				-  	invalid queue id
 *		RT_ERR_ENABLE			   -  invalid enable parameter
 * Note:
 *    This API will the status of Qos flow control ability for the flow control type defined by the user.
 *	  The type parameter could be one of 4 values below:
 *        	0 : RTL8309N_EGR_PDSCFLC, egress port discriptor based flow control type.
 *			1 : RTL8309N_EGR_QFLC, egress queue based flow control type. This type will enable or disable 
 *				all the queue flow control ability, whether it's queue discriptor based or queue packet based.
 *			2 : RTL8309N_EGR_PFLC, egress port based flow control type. This type will enable the whole output flow control
 *				ability per port, whether it's queue based or port discriptor based.
 *			3 : RTL8309N_IGR_FLC, ingress flow control type. This type will enable the whole ingress flow control ability
 *				for RTL8309N.
 *    The port id is from 0 to 8. The queue id is from 0 to 3. 
 */
rtk_api_ret_t rtk_qos_flcEnable_get(rtk_qos_flctype_t type, rtk_port_t port, rtk_queue_num_t queue, rtk_enable_t *pEnabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;

	if(type >= RTK_QOS_FLCTYPE_END)
		return RT_ERR_INPUT;
	if(port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
	if(queue > RTK_QUEUE_ID_MAX)
		return RT_ERR_QUEUE_ID;

	switch(type)
	{
		case RTK_EGR_PDSCFLC:
			if((retVal = rtl8309n_qos_portDscFlcEnable_get(port, &rtl8309n_enabled)) != SUCCESS)
			   return RT_ERR_FAILED;
			 break;
		case RTK_EGR_QFLC:
			 if((retVal = rtl8309n_qos_queueFlcEnable_get(port, queue, &rtl8309n_enabled)) != SUCCESS)
        		return RT_ERR_FAILED;
			 break;
		case RTK_EGR_PFLC:
			if((retVal = rtl8309n_qos_egrFlcEnable_get(port, &rtl8309n_enabled)) != SUCCESS)
			   return RT_ERR_FAILED;
			 break;
		case RTK_IGR_FLC:
			if((retVal = rtl8309n_qos_igrFlcEnable_get(&rtl8309n_enabled)) != SUCCESS)
			   return RT_ERR_FAILED;
			break;
		default: 
			 break;
	}

	*pEnabled = (RTL8309N_ENABLED == rtl8309n_enabled) ? ENABLED : DISABLED;
	
	return RT_ERR_OK;

}
#endif

/* Function Name:
 *      rtk_qos_priSrcEnable_set
 * Description:
 *      Enable/disable Qos priority source for ingress port
 * Input:
 *      port     -  port id
 *	  priSrc	 -  priority source id
 *	  enable   -  DISABLED or ENABLED
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                     -  Success
 *      RT_ERR_FAILED                -  Failure
 *      RT_ERR_PORT_ID				-  invalid port id
 *	  RT_ERR_INPUT			   -  Invalid input parameter
 * Note:
 *    This API will enable Qos priority source for ingress port.
 *    The port id is from 0 to 8. priSrc are Port, 1Q, DSCP, IP adress, and CPU tag basded priority. 
 */
rtk_api_ret_t rtk_qos_priSrcEnable_set(rtk_port_t port, rtk_qosPriSrc_t priSrc, rtk_enable_t enabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;
	uint32 rtl8309n_priSrc;

	if(port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
	if(priSrc >= RTK_QOS_PRISRC_END)
		return RT_ERR_INPUT;

	switch(priSrc)
	{
		case RTK_CPUTAG_PRI:
			rtl8309n_priSrc = RTL8309N_CPUTAG_PRI;
			break;
		case RTK_IP_PRI:
			rtl8309n_priSrc = RTL8309N_IP_PRI;
			break;

		case RTK_DSCP_PRI:
			rtl8309n_priSrc = RTL8309N_DSCP_PRI;
			break;

		case RTK_1Q_PRI:
			rtl8309n_priSrc = RTL8309N_1Q_PRI;
			break;

		case RTK_PORT_PRI:
			rtl8309n_priSrc = RTL8309N_PORT_PRI;
			break;

		default:
			break;
	}
	rtl8309n_enabled = (ENABLED == enabled) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
	
	if((retVal = rtl8309n_qos_priSrcEnable_set(port, rtl8309n_priSrc, rtl8309n_enabled)) != SUCCESS)
		return RT_ERR_FAILED;
	
	return RT_ERR_OK;
}

/* Function Name:
 *      rtk_qos_priSrcEnable_set
 * Description:
 *      Enable/disable Qos priority source for ingress port
 * Input:
 *      port     -  port id
 *		priSrc	 -  priority source id
 * Output:
 *      pEnable  -  Point to the status of qos priority source
 * Return:
 *      RT_ERR_OK                     -  Success
 *      RT_ERR_FAILED                -  Failure
 *      RT_ERR_PORT_ID				-  error port id
 *		RT_ERR_INPUT			   -  Invalid input parameter
 * Note:
 *    This API will get the status of Qos priority source for ingress port.
 *    The port id is from 0 to 8. priSrc are Port, 1Q, DSCP, IP adress, and CPU tag basded priority. 
 */
rtk_api_ret_t rtk_qos_priSrcEnable_get(rtk_port_t port, rtk_qosPriSrc_t priSrc, rtk_enable_t *pEnabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;
	uint32 rtl8309n_priSrc;

	if(port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
	if(priSrc >= RTK_QOS_PRISRC_END)
		return RT_ERR_INPUT;

	switch(priSrc)
	{
		case RTK_CPUTAG_PRI:
			rtl8309n_priSrc = RTL8309N_CPUTAG_PRI;
			break;
		case RTK_IP_PRI:
			rtl8309n_priSrc = RTL8309N_IP_PRI;
			break;

		case RTK_DSCP_PRI:
			rtl8309n_priSrc = RTL8309N_DSCP_PRI;
			break;

		case RTK_1Q_PRI:
			rtl8309n_priSrc = RTL8309N_1Q_PRI;
			break;

		case RTK_PORT_PRI:
			rtl8309n_priSrc = RTL8309N_PORT_PRI;
			break;

		default:
			break;
	}
	
	if((retVal = rtl8309n_qos_priSrcEnable_get(port, rtl8309n_priSrc, &rtl8309n_enabled)) != SUCCESS)
		return RT_ERR_FAILED;
	*pEnabled = (RTL8309N_ENABLED == rtl8309n_enabled) ?  ENABLED : DISABLED;
	
	return RT_ERR_OK;

}



/* Function Name:
 *      rtk_qos_priSel_set
 * Description:
 *      Configure the priority order among different priority mechanisms.
 * Input:
 *      pPriDec		- pointer point to priority level structure. 
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - 	success
 *      RT_ERR_FAILED           - 	failure 
 *      RT_ERR_NULL_POINTER  	-   Input parameter is null pointer
 *      RT_ERR_INPUT 			- 	Invalid input parameter.
 * Note: 
 *      (1)For 8309N, there are 4 types of priority source that could be set arbitration level, which are 
 *      	ACL-based, DSCP-based, 1Q-based, Port-based priority. Each one could be set to level from 0 to 4. 
 *      (2)ASIC will follow user's arbitration level setting to select internal priority for receiving frame. 
 *      	If two priority mechanisms are the same level, the ASIC will choose the higher priority to assign for the receiving frame.
 */
rtk_api_ret_t rtk_qos_priSel_set(rtk_priority_select_t *pPriDec)
{ 
    rtk_api_ret_t retVal;
	rtl8309n_qos_priArbitPara_t rtl8309n_priDec; 
	
    if (NULL == pPriDec)
        return RT_ERR_NULL_POINTER;
    if (pPriDec->port_pri_weight >= RTK_MAX_NUM_OF_INTERNAL_PRIORITY || pPriDec->dot1q_pri_weight >= RTK_MAX_NUM_OF_INTERNAL_PRIORITY ||
		pPriDec->acl_pri_weight >= RTK_MAX_NUM_OF_INTERNAL_PRIORITY || pPriDec->dscp_pri_weight >= RTK_MAX_NUM_OF_INTERNAL_PRIORITY)
        return RT_ERR_INPUT;

	rtl8309n_priDec.acl_pri_weight = pPriDec->acl_pri_weight;
	rtl8309n_priDec.dot1q_pri_weight = pPriDec->dot1q_pri_weight;
	rtl8309n_priDec.dscp_pri_weight = pPriDec->dscp_pri_weight;
	rtl8309n_priDec.port_pri_weight = pPriDec->port_pri_weight;
	
    if ((retVal = rtl8309n_qos_priSrcArbit_set(&rtl8309n_priDec)) != SUCCESS)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_qos_priSel_get
 * Description:
 *      Get the priority order configuration among different priority mechanism.
 * Input:
 *      None
 * Output:
 *      pPriDec		- pointer point to priority level structure. 
 * Return:
 *      RT_ERR_OK              		- success
 *      RT_ERR_FAILED          	- failure 
 *      RT_ERR_NULL_POINTER  	- Input parameter is null pointer
 * Note: 
 *      (1)For 8309N, there are 4 types of priority mechanisms that could be set arbitration level, which are 
 *      	ACL-based, DSCP-based, 1Q-based, Port-based priority. Each one could be set to level from 1 to 4. 
 *      (2)ASIC will follow user's arbitration level setting to select internal priority for receiving frame. 
 *      	If two priority mechanisms are the same level, the ASIC will choose the higher priority to assign for the receiving frame.
 */
rtk_api_ret_t rtk_qos_priSel_get(rtk_priority_select_t *pPriDec)
{
    rtk_api_ret_t retVal;
    rtl8309n_qos_priArbitPara_t rtl8309n_priArbit;  
	    
    if (NULL == pPriDec)
        return RT_ERR_NULL_POINTER;
    
    if ((retVal = rtl8309n_qos_priSrcArbit_get(&rtl8309n_priArbit)) != SUCCESS)
        return RT_ERR_FAILED;
	
    pPriDec->port_pri_weight = rtl8309n_priArbit.port_pri_weight;
    pPriDec->dot1q_pri_weight = rtl8309n_priArbit.dot1q_pri_weight;
    pPriDec->dscp_pri_weight = rtl8309n_priArbit.dscp_pri_weight;
    pPriDec->acl_pri_weight = rtl8309n_priArbit.acl_pri_weight;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_qos_1pPriRemap_set
 * Description:
 *      Configure 1Q priority mapping to internal absolute priority
 * Input:
 *      dot1p_pri     -  802.1p priority value, 0~7
 *      int_pri       	-  internal priority value, 0~3
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                         -  Success
 *      RT_ERR_FAILED                    -  Failure
 *      RT_ERR_VLAN_PRIORITY        -  Invalid 1p priority
 *      RT_ERR_QOS_INT_PRIORITY   -  invalid internal priority  
 * Note:
 *	  When DOT1Q tagged packet has been received, 1Q tag priority has 3 bits, and RTL8309N only support 2 bit priority internally.
 *	  So 3 bit 1Q tag priority has to be mapped to a 2 bit internal priority for further QOS operations.	
 */
rtk_api_ret_t rtk_qos_1pPriRemap_set(rtk_pri_t dot1p_pri, rtk_pri_t int_pri)
{
    rtk_api_ret_t retVal;

    if (int_pri >= RTK_MAX_NUM_OF_INTERNAL_PRIORITY)
        return RT_ERR_QOS_INT_PRIORITY;
    
    if (dot1p_pri >= RTK_MAX_NUM_OF_DOT1Q_PRIORITY)
        return  RT_ERR_VLAN_PRIORITY;
    
    if ((retVal = rtl8309n_qos_1pPriRemap_set(dot1p_pri, int_pri)) != SUCCESS)
        return RT_ERR_FAILED;
    
    return RT_ERR_OK;    
}

/* Function Name:
 *      rtk_qos_1pPriRemap_get
 * Description:
 *      Get 1Q priorities mapping to internal absolute priority
 * Input:
 *      dot1p_pri    -  802.1p priority value
 * Output:
 *      pInt_pri      -  the pointer of internal priority value
 * Return:
 *      RT_ERR_OK                         -  Success
 *      RT_ERR_FAILED                    -  Failure
 *      RT_ERR_VLAN_PRIORITY        -  Invalid 1p priority
 *      RT_ERR_NULL_POINTER         -  Input parameter is null pointer
 * Note:
 *      Priority of 802.1Q assignment for internal asic priority, and it is used for queue usage 
 *      and packet scheduling.
 */
rtk_api_ret_t rtk_qos_1pPriRemap_get(rtk_pri_t dot1p_pri, rtk_pri_t *pInt_pri)
{
    rtk_api_ret_t retVal;
    
    if(NULL == pInt_pri)
        return RT_ERR_NULL_POINTER;

    if (dot1p_pri > RTK_MAX_NUM_OF_DOT1Q_PRIORITY)
        return  RT_ERR_VLAN_PRIORITY;
    
    if((retVal = rtl8309n_qos_1pPriRemap_get(dot1p_pri, (uint32*)pInt_pri)) != SUCCESS)
        return RT_ERR_FAILED;
    
    return RT_ERR_OK;    
}



/* Function Name:
 *      rtk_qos_dscpPriRemap_set
 * Description:
 *      Set DSCP-based priority
 * Input:
 *      dscp_value      -  dscp value(0~63)
 *      int_pri    		-  internal priority value
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                         -  Success
 *      RT_ERR_FAILED                     -  Failure
 *      RT_ERR_QOS_INT_PRIORITY   -   invalid internal priority
 *      RT_ERR_QOS_DSCP_VALUE     -   invalid DSCP value  
 * Note:  
 *	  This API can be called to configure a dscp value to a 2-bit internal priority value.
 *	  RTL8309N support 64 DSCP values and 2-bit internal priority.
 */ 
rtk_api_ret_t rtk_qos_dscpPriRemap_set(rtk_dscp_t dscp_value, rtk_pri_t int_pri)
{
    rtk_api_ret_t retVal;

    if (dscp_value >= RTK_MAX_DSCP_VALUE)
        return RT_ERR_QOS_DSCP_VALUE; 
    if (int_pri > RTK_MAX_NUM_OF_INTERNAL_PRIORITY)
        return RT_ERR_QOS_INT_PRIORITY; 

    if((retVal = rtl8309n_qos_dscpPriRemap_set(dscp_value, int_pri)) != SUCCESS)
        return RT_ERR_FAILED;
    
    return RT_ERR_OK;    
}


/* Function Name:
 *      rtk_qos_dscpPriRemap_get
 * Description:
 *      Get DSCP-based priority
 * Input:
 *      dscp_value      	-  dscp code
 * Output:
 *      pInt_pri  		-  the pointer of internal priority value
 * Return:
 *      RT_ERR_OK                           -  Success
 *      RT_ERR_FAILED                     -  Failure
 *      RT_ERR_QOS_DSCP_VALUE      -  Invalid DSCP value
 *      RT_ERR_NULL_POINTER           -  Input parameter is null pointer
 * Note:  
 *	  This API can be called to get a 2-bit internal priority value for a specified dscp value.
 *	  RTL8309N support 64 DSCP values and 2-bit internal priority.
 */ 
rtk_api_ret_t rtk_qos_dscpPriRemap_get(rtk_dscp_t dscp_value, rtk_pri_t *pInt_pri)
{
    rtk_api_ret_t retVal;
    
    if (dscp_value >= RTK_MAX_DSCP_VALUE)
        return RT_ERR_QOS_DSCP_VALUE; 
    if(NULL == pInt_pri)
        return RT_ERR_NULL_POINTER;

    if((retVal = rtl8309n_qos_dscpPriRemap_get(dscp_value, pInt_pri)) != SUCCESS)
        return RT_ERR_FAILED;
    

    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_qos_portPri_set
 * Description:
 *      Configure priority usage to each port
 * Input:
 *      port                - 		Port id.                
 *      int_pri             -  	internal priority value
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                        -  Success
 *      RT_ERR_FAILED                  -   Failure
 *      RT_ERR_PORT_ID                -   invalid port id
 *      RT_ERR_QOS_INT_PRIORITY  -   invalid internal priority
 * Note:
 *     The API can set port-based priority.
 */
rtk_api_ret_t rtk_qos_portPri_set(rtk_port_t port, rtk_pri_t int_pri)
{
    rtk_api_ret_t retVal;
    
    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    

    if (int_pri >= RTK_MAX_NUM_OF_INTERNAL_PRIORITY)
        return RT_ERR_QOS_INT_PRIORITY; 

    if((retVal = rtl8309n_qos_portPri_set(port, int_pri)) != SUCCESS)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_qos_portPri_get
 * Description:
 *      Get priority usage to each port
 * Input:
 *      port                  - Port id.                
 * Output:
 *      pInt_pri             -  the pointer of internal priority value
 * Return:
 *      RT_ERR_OK                        -  Success
 *      RT_ERR_FAILED                  -   Failure
 *      RT_ERR_PORT_ID                -   invalid port id
 *      RT_ERR_NULL_POINTER        -   input parameter is null pointer
 * Note:
 *     This API can be called to get port-based priority
 */
rtk_api_ret_t rtk_qos_portPri_get(rtk_port_t port, rtk_pri_t *pInt_pri)
{
    rtk_api_ret_t retVal;
    
    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;        
    if(NULL == pInt_pri)
        return RT_ERR_NULL_POINTER;
    
    if((retVal = rtl8309n_qos_portPri_get(port, (uint32*)pInt_pri)) != SUCCESS)
        return RT_ERR_FAILED;

    return RT_ERR_OK;

}

#if 0
/* Fcuntion Name:
 *		rtk_qos_queueNum_set
 * Description:
 *		Set ASIC's queue number
 * Input:
 *		port		-	port id, no usage for RTL8309N
 *		queue_num	-	queue number
 * Output:
 *		none
 * Return:
 *      RT_ERR_QUEUE_NUM	-	invalid queue number
 *		RT_ERR_FAILED	-	Failure
 *		RT_ERR_OK		-	Success
 * Note:
 *		Queue number configuration is global for RTL8309N. Queue number value could be from 1 to 4.
 */
rtk_api_ret_t rtk_qos_queueNum_set(rtk_port_t port, rtk_queue_num_t queue_num)
{
	rtk_api_ret_t retVal;

	if (queue_num > RTK_MAX_NUM_OF_QUEUE)
		return RT_ERR_QUEUE_NUM;
	
	retVal = rtl8309n_qos_queueNum_set((uint32)queue_num);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;

	return RT_ERR_OK;
}

/* Fcuntion Name:
 *		rtk_qos_queueNum_get
 * Description:
 *		Get ASIC's queue number
 * Input:
 *      port	-	port id, no usage for RTL8309N
 * Output:
 *		pQueue_num	-	queue number
 * Return:
 *		RT_ERR_NULL_POINTER		-	input parameter is null input pointer 
 *		RT_ERR_FAILED	-	Failure
 *		RT_ERR_OK		-	Success
 * Note:
 *		Queue number configuration is global for RTL8309N
 */
rtk_api_ret_t rtk_qos_queueNum_get(rtk_port_t port, rtk_queue_num_t *pQueue_num)
{	
	rtk_api_ret_t retVal;
	uint32 rtl8309n_queue_num;

	if (NULL == pQueue_num)
		return RT_ERR_NULL_POINTER;
	
	retVal = rtl8309n_qos_queueNum_get(&rtl8309n_queue_num);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	*pQueue_num = rtl8309n_queue_num;
	
	return RT_ERR_OK;
	
}
#endif

/* Function Name:
 *      rtk_qos_priMap_set
 * Description:
 *      Set internal priority mapping to queue ID for different queue number
 * Input:
 *	port				-	port id
 *    queue_num       	- 	Queue number usage
 *    pPri2qid            - 	pointer point to Priority and queue ID mapping table               
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                        -  Success
 *      RT_ERR_FAILED                  -   Failure
 *		RT_ERR_PORT_ID				-	invalid port id
 *      RT_ERR_QUEUE_NUM              -  invalid queue number
 *      RT_ERR_NULL_POINTER        -   input parameter is null pointer
 * Note:
 *      ASIC supports priority mapping to queue with different queue number from 1 to 4.
 *      For different queue numbers usage, ASIC supports different internal available queue IDs.
 *	  pPri2qid has 4 members, which is from queue id for priority 0 to queue id for priority 3.
 */
rtk_api_ret_t rtk_qos_priMap_set(rtk_port_t port, rtk_queue_num_t queue_num, rtk_qos_pri2queue_t *pPri2qid)
{
    rtk_api_ret_t retVal;
    uint32 pri, qid;

	if (port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
    if((queue_num > RTK_MAX_NUM_OF_QUEUE) || (queue_num == 0))
        return RT_ERR_QUEUE_NUM;           
    if(NULL == pPri2qid)
        return RT_ERR_NULL_POINTER;

    if((retVal = rtl8309n_qos_queueNum_set(queue_num)) != SUCCESS)
        return RT_ERR_FAILED;

    for(pri = RTL8309N_PRIO0; pri <= RTL8309N_PRIO3; pri++)
    {
    	qid = pPri2qid->pri2queue[pri];
        if(qid > (queue_num - 1)) 
            return RT_ERR_QUEUE_ID;
        if((retVal = rtl8309n_qos_priToQueMap_set(port, pri, qid)) != SUCCESS)
            return RT_ERR_FAILED;
    }
            
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_qos_priMap_get
 * Description:
 *      Get priority to queue ID mapping table parameters
 * Input:
 *	  port			-  port id
 *      queue_num       	- queue number usage          
 * Output:
 *      pPri2qid            - pointer point to Priority and queue ID mapping table
 * Return:
 *      RT_ERR_OK                        -  Success
 *      RT_ERR_FAILED                  -   Failure
 *      RT_ERR_PORT_ID              -   invalid port id
 *	  RT_ERR_QUEUE_NUM			-	invalid queue number
 *      RT_ERR_NULL_POINTER        -  input parameter is null pointer
 * Note:
 *      ASIC supports priority mapping to queue with different queue number from 1 to 4.
 *      For different queue numbers usage, ASIC supports different internal available queue IDs.
 *	  pPri2qid has 4 members, which is from queue id for priority 0 to queue id for priority 3.
 */

rtk_api_ret_t rtk_qos_priMap_get(rtk_port_t port, rtk_queue_num_t queue_num, rtk_qos_pri2queue_t *pPri2qid)
{
    rtk_api_ret_t retVal;
    uint32 pri, qid;        

	if (port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
    if((queue_num > RTK_MAX_NUM_OF_QUEUE) || (queue_num == 0))
        return RT_ERR_QUEUE_NUM;            
    if(NULL == pPri2qid)
        return RT_ERR_NULL_POINTER;

    for(pri = RTL8309N_PRIO0; pri <= RTL8309N_PRIO3; pri++)
    {
        if((retVal = rtl8309n_qos_priToQueMap_get(port, pri, &qid)) != SUCCESS)
            return RT_ERR_FAILED;
		pPri2qid->pri2queue[pri] = qid;
    }
    
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_qos_1pRemarkEnable_set
 * Description:
 *      Enable 802.1P remarking ability
 * Input:
 *      port       -  port number
 *      enabled    -  DISABLED or ENABLED
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK              -  Success
 *      RT_ERR_FAILED        -   Failure
 *      RT_ERR_PORT_ID      -   Invalid port id
 * Note:
 *      This API can be called to enable or disable 802.1P remarking ability for a port of RTL8309N.
 *	  The 802.1P remarking function here is used to assign a new 3-bit priority for a tx packet instead of
 *	  its old 2-bit priority. The assignment is based on the user's definition.
 */
rtk_api_ret_t rtk_qos_1pRemarkEnable_set(rtk_port_t port, rtk_enable_t enabled)
{
    rtk_api_ret_t retVal;
    uint32  rtl8309n_enabled;
	
    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;       

	rtl8309n_enabled = (ENABLED == enabled) ? RTL8309N_ENABLED : RTL8309N_DISABLED; 
    if((retVal = rtl8309n_qos_1pRemarkEnable_set(port, rtl8309n_enabled)) != SUCCESS)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_qos_1pRemarkEnable_get
 * Description:
 *      Get enabled status of 802.1P remarking ability
 * Input:
 *      port        -  port number
 * Output:
 *      pEnabled  	-  pointer point to the ability status
 * Return:
 *      RT_ERR_OK                     -  Success
 *      RT_ERR_FAILED               -   Failure
 *      RT_ERR_PORT_ID             -   Invalid port id
 *      RT_ERR_NULL_POINTER     -   Input parameter is null pointer
 * Note:
 *      This API can be called to get the enabled status of  802.1P remarking ability for a port of RTL8309N.
 *	  The 802.1P remarking function here is used to assign a new 3-bit priority for a tx packet instead of
 *	  its old 2-bit priority. The assignment is based on the user's definition.
 */
rtk_api_ret_t rtk_qos_1pRemarkEnable_get(rtk_port_t port, rtk_enable_t *pEnable)
{
    rtk_api_ret_t retVal;
    uint32 rtl8309n_enabled;
	
    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;        
    if(NULL == pEnable)
        return RT_ERR_NULL_POINTER;
	
    if((retVal = rtl8309n_qos_1pRemarkEnable_get(port, &rtl8309n_enabled)) != SUCCESS)
        return RT_ERR_FAILED;    
	*pEnable = (RTL8309N_ENABLED == rtl8309n_enabled) ? ENABLED : DISABLED;
	
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_qos_1pRemark_set
 * Description:
 *      Set 802.1P remarking priority
 * Input:
 *      int_pri        -  Packet internal priority(0~4)
 *      dot1p_pri      -  802.1P priority(0~7)
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                           - Success
 *      RT_ERR_FAILED                      - Failure
 *      RT_ERR_VLAN_PRIORITY          - Invalid 1p priority
 *      RT_ERR_QOS_INT_PRIORITY     - Invalid internal priority 
 * Note:
 *      RTL8309N support 2-bit internal priority and 3-bit dot1p priotiy. User can use this API to map 
 *	  a 2-bit internal priority to a 3-bit dot1p priority.
 */
rtk_api_ret_t rtk_qos_1pRemark_set(rtk_pri_t int_pri, rtk_pri_t dot1p_pri)
{
    rtk_api_ret_t retVal;

    if (int_pri >= RTK_MAX_NUM_OF_INTERNAL_PRIORITY)
        return RT_ERR_QOS_INT_PRIORITY; 
    if (dot1p_pri > RTK_MAX_NUM_OF_DOT1Q_PRIORITY)
        return RT_ERR_VLAN_PRIORITY; 

    if((retVal = rtl8309n_qos_1pRemark_set(int_pri, dot1p_pri)) != SUCCESS)
        return RT_ERR_FAILED;
    
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_qos_1pRemark_get
 * Description:
 *      Get 802.1P remarking priority
 * Input:
 *      int_pri        	-  Packet priority(0~4)
 * Output:
 *      pDot1p_pri  	-  the pointer of 802.1P priority(0~7)
 * Return:
 *      RT_ERR_OK                           -  Success
 *      RT_ERR_FAILED                      -  Failure
 *      RT_ERR_NULL_POINTER            -  Input parameter is null pointer
 *      RT_ERR_QOS_INT_PRIORITY     -  Invalid internal priority 
 * Note:
 *      This API can be called to get a 2-bit internal priority and a 3-bit dot1p priority mapping. 
 */
rtk_api_ret_t rtk_qos_1pRemark_get(rtk_pri_t int_pri, rtk_pri_t *pDot1p_pri)
{
    rtk_api_ret_t retVal;
    
    if (int_pri >= RTK_MAX_NUM_OF_INTERNAL_PRIORITY)
        return RT_ERR_QOS_INT_PRIORITY; 
    if(NULL == pDot1p_pri)
        return RT_ERR_NULL_POINTER;
    
    if((retVal = rtl8309n_qos_1pRemark_get(int_pri, pDot1p_pri)) != SUCCESS)
        return RT_ERR_FAILED;
    

    return RT_ERR_OK;
}

#if 0
/* Function Name:
 *		rtk_igmp_enble_set
 * Description:
 *		Enable IGMP/MLD snooping, PPPOE bypass
 * Input:
 *		type	-	IGMP/MLD, PPPOE
 *		enabled -	enable or disable
 * Output:
 *		none
 * Return:
 *		RT_ERR_FAILED	-	Failure
 *		RT_ERR_OK		-	Success
 *		RT_ERR_INPUT	-	invalid input parameter 
 * Note:
 *		add at 2011-0905, for RTL8309N only
 */
rtk_api_ret_t rtk_igmp_enable_set(rtk_igmp_type_t type, rtk_enable_t enabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;
	
    if(type >= IGMP_TYPE_END)
        return RT_ERR_INPUT;

	rtl8309n_enabled = (ENABLED == enabled) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
	retVal = rtl8309n_igmp_enable_set(type, rtl8309n_enabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;

	return RT_ERR_OK;

}

/* Function Name:
 *		rtk_igmp_enble_get
 * Description:
 *		Get enabled status of IGMP/MLD snooping, PPPOE bypass function
 * Input:
 *		type	-	IGMP/MLD, PPPOE
 * Output:
 *		pEnabled -	enable or disable
 * Return:
 *		RT_ERR_FAILED	-	Failure
 *		RT_ERR_OK		-	Success
 *		RT_ERR_INPUT	-	invalid input parameter 
 * Note:
 *		add at 2011-0905, for RTL8309N only
 */
rtk_api_ret_t rtk_igmp_enable_get(rtk_igmp_type_t type, rtk_enable_t *pEnabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;
	
    if(type >= IGMP_TYPE_END)
        return RT_ERR_INPUT;

	retVal = rtl8309n_igmp_enable_get(type, &rtl8309n_enabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	
	*pEnabled = (RTL8309N_ENABLED == rtl8309n_enabled) ? ENABLED : DISABLED;

	return RT_ERR_OK;
}
#endif

/* Function Name:
 *      rtk_trap_unknownMcastPktAction_set
 * Description:
 *      Set behavior of unknown multicast
 * Input:
 *      port                -   port id
 *      type               -   unknown multicast packet type
 *      mcast_action    -  unknown multicast action
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                      -  Success
 *      RT_ERR_FAILED                -  Failure
 *      RT_ERR_PORT_ID              -  Invalid port id    
 *      RT_ERR_INPUT                 -  Invalid input parameter 
 * Note:
 *      When receives an unknown multicast packet, switch may forward, drop this packet
 *      The unknown multicast packet type is as following:
 *               - MCAST_IPV4
 *               - MCAST_IPV6
 *      The unknown multicast action is as following:
 *               - MCAST_ACTION_FORWARD
 *               - MCAST_ACTION_DROP
 */
rtk_api_ret_t rtk_trap_unknownMcastPktAction_set(rtk_port_t port, rtk_mcast_type_t type, rtk_trap_mcast_action_t mcast_action)
{
    rtk_api_ret_t retVal;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    
    if((type < MCAST_IPV4) || (type > MCAST_IPV6 ))
        return RT_ERR_INPUT;
    if(mcast_action > MCAST_ACTION_DROP)
        return RT_ERR_INPUT;

    if(MCAST_IPV4 == type)
    {
        retVal = rtl8309n_trap_unknownIPMcastPktAction_set(RTL8309N_MULCAST_IPV4, 
                   (MCAST_ACTION_FORWARD == mcast_action) ? RTL8309N_ACT_PERMIT: RTL8309N_ACT_DROP);
        if(retVal != SUCCESS)
            return RT_ERR_FAILED;
    }
    else if(MCAST_IPV6 == type)
    {
        retVal = rtl8309n_trap_unknownIPMcastPktAction_set(RTL8309N_MULCAST_IPV6, 
                   (MCAST_ACTION_FORWARD == mcast_action) ? RTL8309N_ACT_PERMIT: RTL8309N_ACT_DROP);
        if(retVal != SUCCESS)
            return RT_ERR_FAILED;        
    }
	
    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_trap_unknownMcastPktAction_get
 * Description:
 *      Get behavior of unknown multicast
 * Input:
 *      port                  -   port id
 *      type                 -   unknown multicast packet type
 * Output:
 *      pMcast_action    -   the pointer of unknown multicast action
 * Return:
 *      RT_ERR_OK                      -  Success
 *      RT_ERR_FAILED                -  Failure
 *      RT_ERR_PORT_ID              -  Invalid port id 
 *	  RT_ERR_INPUT			-	invalid input parameter
 *      RT_ERR_NULL_POINTER      -  Input parameter is null pointer
 * Note:
 *      When receives an unknown multicast packet, switch may forward, drop this packet.
 *      The unknown multicast packet type is as following:
 *               - MCAST_IPV4
 *               - MCAST_IPV6
 *      The unknown multicast action is as following:
 *               - MCAST_ACTION_FORWARD
 *               - MCAST_ACTION_DROP
 */
rtk_api_ret_t rtk_trap_unknownMcastPktAction_get(rtk_port_t port, rtk_mcast_type_t type, rtk_trap_mcast_action_t *pMcast_action)
{
    rtk_api_ret_t retVal;
    uint32 action;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    
    if((MCAST_L2 == type) || (type > MCAST_IPV6 ))
        return RT_ERR_INPUT;
    if(NULL == pMcast_action)
        return RT_ERR_NULL_POINTER;

    if(MCAST_IPV4 == type)
    {
        retVal = rtl8309n_trap_unknownIPMcastPktAction_get(RTL8309N_MULCAST_IPV4, &action);
        if(retVal != SUCCESS)
            return RT_ERR_FAILED;                        
    }
    else if(MCAST_IPV6 == type)
    {
        retVal = rtl8309n_trap_unknownIPMcastPktAction_get(RTL8309N_MULCAST_IPV6,  &action);
        if(retVal != SUCCESS)
            return RT_ERR_FAILED;
                        
    }
    *pMcast_action = (RTL8309N_ACT_PERMIT == action) ? MCAST_ACTION_FORWARD : MCAST_ACTION_DROP; 

	
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_trap_igmpCtrlPktAction_set
 * Description:
 *      Set IGMP/MLD trap function
 * Input:
 *      type                -   IGMP/MLD packet type
 *      igmp_action         -   IGMP/MLD action
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                      -  Success
 *      RT_ERR_FAILED                -  Failure
 *      RT_ERR_INPUT                 -  Invalid input parameter
 *      RT_ERR_NOT_ALLOWED     -  Actions not allowed by the function
 * Note:
 *      This API can set both IPv4 IGMP/IPv6 MLD with/without PPPoE header trapping function.
 *      All 4 kinds of IGMP/MLD function can be set separately.
 *      The IGMP/MLD packet type is as following:
 *          - IGMP_IPV4
 *          - IGMP_MLD
 *          - IGMP_PPPOE_IPV4
 *          - IGMP_PPPOE_MLD
 *      The IGMP/MLD action is as following:
 *          - IGMP_ACTION_FORWARD
 *	      - IGMP_ACTION_COPY2CPU
 *          - IGMP_ACTION_TRAP2CPU
 *	      - IGMP_ACTION_DROP
 */ 
rtk_api_ret_t rtk_trap_igmpCtrlPktAction_set(rtk_igmp_type_t type, rtk_trap_igmp_action_t igmp_action)
{
    rtk_api_ret_t retVal;
    uint32 igmp_type, action;

    if(type >= IGMP_TYPE_END)
        return RT_ERR_INPUT;
    if(igmp_action >= IGMP_ACTION_END)
        return RT_ERR_INPUT;
 
	switch(type)
	{
		case IGMP_IPV4:
			igmp_type = RTL8309N_IGMP;
			break;
			
		case IGMP_MLD:
			igmp_type = RTL8309N_MLD;
			break;
			
		case IGMP_PPPOE_IPV4:
			igmp_type = RTL8309N_PPPOE_IPV4;
			break;
			
		case IGMP_PPPOE_MLD:
			igmp_type = RTL8309N_PPPOE_MLD;
			break;

		default:
			break;
	}
	switch(igmp_action)
	{
		case IGMP_ACTION_FORWARD:
			action = RTL8309N_ACT_PERMIT;
			break;
		case IGMP_ACTION_COPY2CPU:
			action = RTL8309N_ACT_COPY2CPU;
			break;
		case IGMP_ACTION_TRAP2CPU:
			action = RTL8309N_ACT_TRAP2CPU;
			break;
		case IGMP_ACTION_DROP:
			action = RTL8309N_ACT_DROP;
			break;
		default:
			break;
	}     
    retVal = rtl8309n_trap_igmpCtrlPktAction_set(igmp_type, action);    
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_trap_igmpCtrlPktAction_get
 * Description:
 *      Get IGMP/MLD trap function
 * Input:
 *      type                -   IGMP/MLD packet type
 * Output:
 *      pIgmp_action    -   the pointer of IGMP/MLD action
 * Return:
 *      RT_ERR_OK                      -  Success
 *      RT_ERR_FAILED                -  Failure
 *      RT_ERR_INPUT                 -  Invalid input parameter
 *      RT_ERR_NULL_POINTER      -  Input parameter is null pointer
 * Note:
 *      This API can get both IPv4 IGMP/IPv6 MLD with/without PPPoE header trapping function.
 *      All 4 kinds of IGMP/MLD function can be set seperately.
 *      The IGMP/MLD packet type is as following:
 *          - IGMP_IPV4
 *          - IGMP_MLD
 *          - IGMP_PPPOE_IPV4
 *          - IGMP_PPPOE_MLD
 *      The IGMP/MLD action is as following:
 *          - IGMP_ACTION_FORWARD
 *          - IGMP_ACTION_TRAP2CPU
 */
rtk_api_ret_t rtk_trap_igmpCtrlPktAction_get(rtk_igmp_type_t type, rtk_trap_igmp_action_t *pIgmp_action)
{
    rtk_api_ret_t retVal;
    uint32 igmp_type, action;

    if(type >= IGMP_TYPE_END)
        return RT_ERR_INPUT;
    if(NULL == pIgmp_action)
        return RT_ERR_NULL_POINTER;

	switch(type)
	{
		case IGMP_IPV4:
			igmp_type = RTL8309N_IGMP;
			break;
			
		case IGMP_MLD:
			igmp_type = RTL8309N_MLD;
			break;
			
		case IGMP_PPPOE_IPV4:
			igmp_type = RTL8309N_PPPOE_IPV4;
			break;
			
		case IGMP_PPPOE_MLD:
			igmp_type = RTL8309N_PPPOE_MLD;
			break;

		default:
			break;
	}

    retVal = rtl8309n_trap_igmpCtrlPktAction_get(igmp_type, &action);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;
	switch(action)
	{
		case RTL8309N_ACT_PERMIT:
			*pIgmp_action = IGMP_ACTION_FORWARD;
			break;
		case RTL8309N_ACT_COPY2CPU:
			*pIgmp_action = IGMP_ACTION_COPY2CPU;
			break;
		case RTL8309N_ACT_TRAP2CPU:
			*pIgmp_action = IGMP_ACTION_TRAP2CPU;
			break;
		case RTL8309N_ACT_DROP:
			*pIgmp_action = IGMP_ACTION_DROP;
			break;
		default:
			break;
	}
    
    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_vlan_init
 * Description:
 *      Initialize VLAN
 * Input:
 *      none
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                     -  Success
 *      RT_ERR_FAILED                -  Failure
 *      RT_ERR_FAILED                -  Failure
 * Note:
 *      VLAN function is disabled for ASIC after reset by default. User has to call this API to enable VLAN before
 *      using it. And It will set a default VLAN(vid 1) including all ports and set all ports's vlan index pointed 
 *	  to the default VLAN. So all port's PVID are 1.
 */
rtk_api_ret_t rtk_vlan_init(void)
{
	rtl8309n_vlan_init();
    
    return RT_ERR_OK;   
}

/* Function Name:
 *      rtk_vlan_set
 * Description:
 *      Set a VLAN entry
 * Input:
 *      vid              - VLAN ID to configure, should be 1~4094
 *      mbrmsk        - VLAN member set portmask
 *      untagmsk     - VLAN untag set portmask
 *      fid          - filtering database id, should be 0 - 3 
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                     -  Success
 *      RT_ERR_FAILED                 -  Failure
 *      RT_ERR_VLAN_VID               -  Invalid vid
 *		RT_ERR_PORT_MASK			  -	 invalid port mask
 *      RT_ERR_INPUT                  -  Invalid input parameter 
 *      RT_ERR_TBL_FULL               -  Input table full 
 * Note:
 *     There are 16 VLAN entry supported for RTL8309N. User could configure the member port set and untag member port set for
 *     specified vid through this API. The vid is from 0 to 4095. The vid 0 is used for priority tagged frames which is treated as untagged frames.
 *	 The vid 4095 is reserved for further usage. 
 *	 The portmask's bit N means port N. For example, mbrmask 0x17 = 010111 means that port 0,1,2,4 are in the vlan's member port set.
 *     FID is for SVL/IVL usage, and the range is from 0 to 3. RTL8309N can only support 4 filtering database with the use of FID. 
 */
rtk_api_ret_t rtk_vlan_set(rtk_vlan_t vid, rtk_portmask_t mbrmsk, rtk_portmask_t untagmsk, rtk_fid_t fid)
{
    uint32 vid_val, mbrmsk_val, untagmsk_val, fid_val;
    uint32 index, hit_index, hit_flag;
    uint32 fullflag;
    int32 i;
	
    /* vid must be 1~4094 */
    if ((vid == 0) || (vid > (RTK_MAX_VID - 1)))
        return RT_ERR_VLAN_VID;
    if (mbrmsk.bits[0] > RTL8309N_MAX_PORTMASK)
        return RT_ERR_PORT_MASK;
    if (untagmsk.bits[0] > RTL8309N_MAX_PORTMASK)
        return RT_ERR_PORT_MASK;
	if(fid > RTK_MAX_FID - 1)
		return RT_ERR_INPUT;

    /*check if vid exists and check if vlan is full*/
    fullflag = TRUE;
    hit_flag = FALSE;
    index = 16;
    hit_index= 16;
    for(i = 15; i >= 0; i--)
    {	
        rtl8309n_vlan_entry_get(i, &vid_val, &mbrmsk_val, &untagmsk_val, &fid_val);
		if(vid_val == 0)
        {
            index = i; 
            fullflag = FALSE;
            continue;
        }
        
        if (vid_val == vid)
        {
            hit_flag = TRUE;
            hit_index = i;
            fullflag = FALSE;
            break;
        }            
    }        
    
    if (fullflag)       
        return RT_ERR_TBL_FULL;
    else 
    {
        vid_val           = vid;
        mbrmsk_val     = mbrmsk.bits[0];
        untagmsk_val  = untagmsk.bits[0];
		fid_val		  = fid;

        /*both mbrmsk_val and untagmsk_val are zero will clear the vlan*/
        if( (0 == mbrmsk_val) && (0 == untagmsk_val))
            vid_val = 0;
            
        if (hit_flag)
            rtl8309n_vlan_entry_set(hit_index, vid_val, mbrmsk_val, untagmsk_val, fid_val);            
        else
            rtl8309n_vlan_entry_set(index, vid_val, mbrmsk_val, untagmsk_val, fid_val); 
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_vlan_get
 * Description:
 *      Get a VLAN entry
 * Input:
 *      vid             - VLAN ID to configure
 * Output:
 *      pMbrmsk     - VLAN member set portmask
 *      pUntagmsk  - VLAN untag set portmask
 *      pFid           -  filtering database id
 * Return:
 *      RT_ERR_OK                                   -  Success
 *      RT_ERR_FAILED                              -  Failure
 *      RT_ERR_VLAN_VID                          -  Invalid vid
 *      RT_ERR_NULL_POINTER                    -  Input parameter is null pointer
 *      RT_ERR_VLAN_ENTRY_NOT_FOUND    -   specified vlan entry not found 
 * Note:
 *     There are 16 VLAN entry supported for RTL8309N. User could configure the member port set and untag member port set for
 *     specified vid through this API. The vid is from 0 to 4095. The vid 0 is used for priority tagged frames which is treated as untagged frames.
 *	 The vid 4095 is reserved for further usage. 
 *	 The portmask's bit N means port N. For example, mbrmask 0x17 = 010111 means that port 0,1,2,4 are in the vlan's member port set.
 *     FID is for SVL/IVL usage, and the range is from 0 to 3. RTL8309N can only support 4 filtering database with the use of FID. 
 */
rtk_api_ret_t rtk_vlan_get(rtk_vlan_t vid, rtk_portmask_t *pMbrmsk, rtk_portmask_t *pUntagmsk, rtk_fid_t *pFid)
{
    uint32 vid_val, mbrmsk_val, untagmsk_val, fid_val;
    uint32 hit_flag;
    int32 i;

    /* vid must be 1~4094 */
    if ((vid == 0) || (vid > (RTK_MAX_VID - 1)))
        return RT_ERR_VLAN_VID;

    if ((NULL == pMbrmsk) || (NULL == pUntagmsk) || (NULL == pFid))
        return RT_ERR_NULL_POINTER;

    /*seach the vlan table*/
    hit_flag = FALSE;
    for (i = 15; i >= 0; i--)
    {
        rtl8309n_vlan_entry_get(i, &vid_val, &mbrmsk_val, &untagmsk_val, &fid_val);
        if(vid_val == vid)
        {
            hit_flag = TRUE;
            pMbrmsk->bits[0]    = mbrmsk_val;
            pUntagmsk->bits[0]  = untagmsk_val;         
            *pFid = fid_val;
			
            return RT_ERR_OK;
        }
    }

    if(!hit_flag)
        return RT_ERR_VLAN_ENTRY_NOT_FOUND;
        
    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_vlan_destroy
 * Description:
 *      delete a vlan entry from vlan table with specified vid
 * Input:
 *      vid             - VLAN ID to configure
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                                   -  Success
 *      RT_ERR_VLAN_VID                          -  Invalid vid
 *      RT_ERR_VLAN_ENTRY_NOT_FOUND    			 -  Specified vlan entry not found
 * Note:
 * 	This API can be called to delet a vlan entry from vlan table with specified vid. After it is called,
 *	the content of vlan entry will set to zero.
 */
rtk_api_ret_t rtk_vlan_destroy(rtk_vlan_t vid)
{
    uint32 vid_val, mbrmsk_val, untagmsk_val, fid;
    uint32 hit_flag;
    int32 i;
    
    /* vid must be 1~4094, 0 and 4095 are not used at here*/
    if ((vid == 0) || (vid > (RTL8309N_VIDMAX - 1)))
        return RT_ERR_VLAN_VID;

    hit_flag = FALSE;
    for (i = 15; i >= 0; i--)
    {
        rtl8309n_vlan_entry_get(i, &vid_val, &mbrmsk_val, &untagmsk_val,&fid);
        if(vid_val == vid)
        {
            hit_flag = TRUE;
            vid_val = 0;
            mbrmsk_val = 0;
            untagmsk_val = 0;
			fid = 0;
            rtl8309n_vlan_entry_set(i, vid_val, mbrmsk_val, untagmsk_val, fid);
            return RT_ERR_OK;
        }        
    }

    if(!hit_flag)
        return RT_ERR_VLAN_ENTRY_NOT_FOUND;    
    else
        return RT_ERR_OK;        
}
    

/* Function Name:
 *      rtk_vlan_portPvid_set
 * Description:
 *      Set port to specified VLAN ID(PVID)
 * Input:
 *      port             - Port id
 *      pvid             - Specified VLAN ID
 *      priority         - 802.1p priority for the PVID, 0~3 for RTL8309N 
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                                   -  Success
 *      RT_ERR_FAILED                              -  Failure
 *	  RT_ERR_PORT_ID							-	invalid port id
 *      RT_ERR_VLAN_VID                          -  Invalid vid
 *      RT_ERR_VLAN_PRIORITY                  -  Invalid 1p priority 
 *      RT_ERR_VLAN_ENTRY_NOT_FOUND    -  Specified vlan entry not found
 * Note:
 *      The API is used for Port-based VLAN. The untagged frame received from the
 *      port will be classified to the specified port-based VLAN and assigned to the specified priority.
 */
rtk_api_ret_t rtk_vlan_portPvid_set(rtk_port_t port, rtk_vlan_t pvid, rtk_pri_t priority)
{
    uint32 vid_val, mbrmsk_val, untagmsk_val, fid;
    uint32 hit_flag;
    int32 i;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    
    if ((pvid == 0) || (pvid >= RTL8309N_VIDMAX))
        return RT_ERR_VLAN_VID;
    if (priority >= RTK_MAX_NUM_OF_INTERNAL_PRIORITY)
        return RT_ERR_VLAN_PRIORITY;

    /*seach the vlan table*/
    hit_flag = FALSE;
    for (i = 15; i >= 0; i--)
    {
        rtl8309n_vlan_entry_get(i, &vid_val, &mbrmsk_val, &untagmsk_val, &fid);
        if(vid_val == pvid)
        {
            hit_flag = TRUE;
			/*port pvid*/
            rtl8309n_vlan_portPvidIndex_set(port, i);			
            /*802.1Q port based priority for untag pkt*/ 
			rtl8309n_qos_1QPortPri_set(port, priority);
			
            return RT_ERR_OK;
        }
    }
    if (!hit_flag)
        return RT_ERR_VLAN_ENTRY_NOT_FOUND;
    
    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_vlan_portPvid_get
 * Description:
 *      Get VLAN ID(PVID) on specified port
 * Input:
 *      port             - Port id
 * Output:
 *      pPvid            - Specified VLAN ID
 *      pPriority        - 802.1p priority for the PVID
 * Return:
 *      RT_ERR_OK                                   - Success
 *      RT_ERR_FAILED                             -  Failure
 *      RT_ERR_PORT_ID                           -  Invalid port id
 *      RT_ERR_NULL_POINTER                   -  Input parameter is null pointer
 * Note:
 *      The API is used for Port-based VLAN. The untagged frame received from the
 *      port will be classified to the specified port-based VLAN and assigned to the specified priority.
 */
rtk_api_ret_t rtk_vlan_portPvid_get(rtk_port_t port, rtk_vlan_t *pPvid, rtk_pri_t *pPriority)
{
    rtk_api_ret_t retVal;    
    uint32 vid_val, mbrmsk_val, untagmsk_val, fid;
    uint32 index;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    

    if((NULL == pPvid) || (NULL == pPriority))
        return RT_ERR_NULL_POINTER;

    retVal = rtl8309n_vlan_portPvidIndex_get(port, &index);
    if (retVal != SUCCESS)
        return RT_ERR_FAILED;
    
    retVal = rtl8309n_vlan_entry_get(index, &vid_val, &mbrmsk_val, &untagmsk_val, &fid);    
    if (retVal != SUCCESS)
        return RT_ERR_FAILED;
    *pPvid = vid_val;

    /*get 802.1Q default priority for untag pkt*/
    retVal = rtl8309n_qos_1QPortPri_get(port, pPriority);   
	if (retVal != SUCCESS)
        return RT_ERR_FAILED;
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_vlan_portIFilterEnable_set
 * Description:
 *      Set VLAN ingress for each port
 * Input:
 *      port             - Port id, no use for RTL8309N
 *      igr_filter       - VLAN ingress function enable status
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                        - Success
 *      RT_ERR_FAILED                  -  Failure
 *	  RT_ERR_PORT_ID				-	invalid port id
 * Note:
 *      RTL8309N use one vlan ingress filter configuration for whole system, not for each port, so 
 *      any port you set will affect all ports's ingress filter setting.
 *      While VLAN function is enabled, ASIC will decide VLAN ID for each received frame 
 *      and get member ports for this vlan from VLAN table. If packets ingress port is in VLAN, 
 *	  ASIC will drop the received frame if VLAN ingress filter function is enabled.
 */
rtk_api_ret_t rtk_vlan_portIFilterEnable_set(rtk_port_t port, rtk_enable_t igr_filter)
{
    rtk_api_ret_t retVal;    
    uint32 rtl8309n_enabled;
	
    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    

	rtl8309n_enabled = (ENABLED == igr_filter) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
    retVal = rtl8309n_vlan_igrFilterEnable_set(rtl8309n_enabled);
    if (retVal != SUCCESS)
        return RT_ERR_FAILED;
            
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_vlan_portIFilterEnable_get
 * Description:
 *      get VLAN ingress for each port
 * Input:
 *      port             - Port id, no use for RTL8309N
 * Output:
 *      pIgr_filter     - the pointer of VLAN ingress function enable status
 * Return:
 *      RT_ERR_OK                 - Success
 *      RT_ERR_FAILED           -  Failure
 *	  RT_ERR_PORT_ID			-	invalid port id
 *	  RT_ERR_NULL_POINTER		-	input parameter is null pointer
 * Note:
 *      RTL8309N use one ingress filter configuration for whole system, not for each port, so 
 *      any port you set will affect all ports ingress filter setting.
 *      While VLAN function is enabled, ASIC will decide VLAN ID for each received frame 
 *      and get belonged member ports from VLAN table. If received port is not belonged 
 *      to VLAN member ports, ASIC will drop received frame if VLAN ingress function is enabled.
 */
rtk_api_ret_t rtk_vlan_portIFilterEnable_get(rtk_port_t port, rtk_enable_t *pIgr_filter)
{
    rtk_api_ret_t retVal;    
    uint32 rtl8309n_enabled;
    
    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    
    
    if (NULL == pIgr_filter)
        return RT_ERR_NULL_POINTER;

    retVal = rtl8309n_vlan_igrFilterEnable_get(&rtl8309n_enabled);
    if (retVal != SUCCESS)
        return RT_ERR_FAILED;
    *pIgr_filter = (RTL8309N_ENABLED == rtl8309n_enabled) ? ENABLED : DISABLED;
        
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_vlan_portAcceptFrameType_set
 * Description:
 *      Set VLAN support frame type
 * Input:
 *      port                          - Port id
 *      accept_frame_type             - accept frame type
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                  - Success
 *      RT_ERR_FAILED            -  Failure
 *      RT_ERR_PORT_ID          -  Invalid port id
 *	  RT_ERR_VLAN_ACCEPT_FRAME_TYPE	-	invalid accept frame type 
 * Note:
 *    The API is used for ingress port to check 802.1Q tagged frames.
 *    The ingress ports's accept frame type could be set to values as follows:
 *          -	ACCEPT_FRAME_TYPE_AL
 *          -	ACCEPT_FRAME_TYPE_TAG_ONLY
 *          -	ACCEPT_FRAME_TYPE_UNTAG_ONLY
 */
rtk_api_ret_t rtk_vlan_portAcceptFrameType_set(rtk_port_t port, rtk_vlan_acceptFrameType_t accept_frame_type)
{
    rtk_api_ret_t retVal;
    rtl8309n_acceptFrameType_t accfrm_type;    

    if ( port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;

    switch(accept_frame_type)
    {
        case ACCEPT_FRAME_TYPE_ALL:
            accfrm_type = RTL8309N_ACCEPT_ALL;
            break;
        case ACCEPT_FRAME_TYPE_TAG_ONLY:
            accfrm_type = RTL8309N_ACCEPT_TAG_ONLY;
            break;
        case ACCEPT_FRAME_TYPE_UNTAG_ONLY:
            accfrm_type = RTL8309N_ACCEPT_UNTAG_ONLY;
            break;
        default:
            return RT_ERR_VLAN_ACCEPT_FRAME_TYPE;
    }

    retVal = rtl8309n_vlan_portAcceptFrameType_set(port, accfrm_type);
    if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	
    return retVal;
}

/* Function Name:
 *      rtk_vlan_portAcceptFrameType_get
 * Description:
 *      Get VLAN support frame type
 * Input:
 *      port                          - Port id
 *      accept_frame_type             - accept frame type
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                                   - Success
 *      RT_ERR_FAILED                             -  Failure
 *      RT_ERR_PORT_ID                           -  Invalid port id
 *	  RT_ERR_NULL_POINTER				-	input parameter is null pointer
 *      RT_ERR_VLAN_ACCEPT_FRAME_TYPE -  Invalid accept frame type 
 * Note:
 *    The API is used for ingress port to check 802.1Q tagged frames.
 *    The ingress ports's accept frame type could be set to values as follows:
 *          -	ACCEPT_FRAME_TYPE_AL
 *          -	ACCEPT_FRAME_TYPE_TAG_ONLY
 *          -	ACCEPT_FRAME_TYPE_UNTAG_ONLY
 */
rtk_api_ret_t rtk_vlan_portAcceptFrameType_get(rtk_port_t port, rtk_vlan_acceptFrameType_t *pAccept_frame_type)
{
    rtk_api_ret_t retVal;
    rtl8309n_acceptFrameType_t accfrm_type;    

    if ( port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;

    if (NULL == pAccept_frame_type)
        return RT_ERR_NULL_POINTER;

    retVal = rtl8309n_vlan_portAcceptFrameType_get(port, &accfrm_type);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	
    switch(accfrm_type)
    {
        case RTL8309N_ACCEPT_ALL:
            *pAccept_frame_type = ACCEPT_FRAME_TYPE_ALL;
            break;
        case RTL8309N_ACCEPT_TAG_ONLY:
            *pAccept_frame_type = ACCEPT_FRAME_TYPE_TAG_ONLY;
            break;
        case RTL8309N_ACCEPT_UNTAG_ONLY:
            *pAccept_frame_type = ACCEPT_FRAME_TYPE_UNTAG_ONLY;
            break;
        default:
            return RT_ERR_VLAN_ACCEPT_FRAME_TYPE;
    }

    return retVal;
}    

/* Function Name:
 *      rtk_stp_mstpState_set
 * Description:
 *      Configure spanning tree state per port
 * Input:
 *      msti              - Multiple spanning tree instance, no use for RTL8309N
 *      port              - Port id
 *      stp_state         - Spanning tree state
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                      -  Success
 *      RT_ERR_FAILED                -  Failure
 *      RT_ERR_PORT_ID              -  Invalid port id
 *      RT_ERR_MSTP_STATE        -  Invalid spanning tree status
 * Note:
 *      Because RTL8309N does not support multiple spanning tree, so msti is no use. 
 *      There are four states supported by ASIC.
 *          -	STP_STATE_DISABLED
 *          -	STP_STATE_BLOCKING
 *          -	STP_STATE_LEARNING
 *          -	STP_STATE_FORWARDING
 */
rtk_api_ret_t rtk_stp_mstpState_set(rtk_stp_msti_id_t msti, rtk_port_t port, rtk_stp_state_t stp_state)
{
    rtk_api_ret_t retVal;
    uint32 rtl8309n_stp_state;
	
    if(port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;
    if(stp_state >= STP_STATE_END)
        return RT_ERR_MSTP_STATE;

    msti = 0;

	switch(stp_state)
	{
		case STP_STATE_DISABLED:
			rtl8309n_stp_state = RTL8309N_SPAN_DISABLE;
			break;
			
		case STP_STATE_BLOCKING:
			rtl8309n_stp_state = RTL8309N_SPAN_BLOCK;
			break;

		case STP_STATE_LEARNING:
			rtl8309n_stp_state = RTL8309N_SPAN_LEARN;
			break;

		case STP_STATE_FORWARDING:
			rtl8309n_stp_state = RTL8309N_SPAN_FORWARD;
			break;

		default:
			break;
	}
    retVal = rtl8309n_stp_set(port, rtl8309n_stp_state);
	if(retVal != SUCCESS)
		return FAILED;
	
    return retVal;

}    

/* Function Name:
 *      rtk_stp_mstpState_get
 * Description:
 *      Get Configuration of spanning tree state per port
 * Input:
 *      msti              - Multiple spanning tree instance, no use for RTL8309N
 *      port              - Port id
 * Output:
 *      pStp_state     - the pointer of Spanning tree state
 * Return:
 *      RT_ERR_OK                      -  Success
 *      RT_ERR_FAILED                -  Failure
 *      RT_ERR_PORT_ID              -  Invalid port id
 *      RT_ERR_NULL_POINTER      -  Input parameter is null pointer
 * Note:
 *      Because RTL8309N does not support multiple spanning tree, so msti is no use. 
 *      There are four states supported by ASIC.
 *          -	STP_STATE_DISABLED
 *          -	STP_STATE_BLOCKING
 *          -	STP_STATE_LEARNING
 *          -	STP_STATE_FORWARDING
 */
rtk_api_ret_t rtk_stp_mstpState_get(rtk_stp_msti_id_t msti, rtk_port_t port, rtk_stp_state_t *pStp_state)
{
    rtk_api_ret_t retVal;
    uint32 rtl8309n_stp_state;
    
    if(port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;
    if(NULL == pStp_state)
        return RT_ERR_NULL_POINTER;

    msti = 0;


    retVal = rtl8309n_stp_get(port, &rtl8309n_stp_state);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	
	switch(rtl8309n_stp_state)
	{
		case RTL8309N_SPAN_DISABLE :
			*pStp_state = STP_STATE_DISABLED;
			break;
				
		case RTL8309N_SPAN_BLOCK:
			*pStp_state = STP_STATE_BLOCKING;
			break;
		
		case RTL8309N_SPAN_LEARN:
			*pStp_state = STP_STATE_LEARNING;
			break;
		
		case RTL8309N_SPAN_FORWARD:
			*pStp_state = STP_STATE_FORWARDING;
			break;
		
		default:
			break;
	}

    return retVal;
}

/* Function Name:
 *      rtk_l2_addr_add
 * Description:
 *      Add a unicast entry into LUT table
 * Input:
 *		pMac			-	pointer point to structure of unicastmac address
 *		fid				-	fid value(0~3)
 *      pL2_data        -   the mac address attributes, include fid,auth,da_block,sa_block and ext.
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                       -  Success
 *      RT_ERR_FAILED                 -   Failure
 *      RT_ERR_INPUT                  -   Invalid input parameter
 *      RT_ERR_MAC                    -   invalid mac address      
 *      RT_ERR_NULL_POINTER       -  Input parameter is null pointer    
 *      RT_ERR_L2_INDEXTBL_FULL -  The L2 index table is full
 * Note:
 *      (1)The lut has a 4-way entry due to an index. If the macAddress has existed in the lut, it will update the entry 
 *		with the user's defined entry content, otherwise the function will find an empty entry to put it.
 *          When the index is full, it will find a dynamic & unauth unicast macAddress entry to replace with it. 
 *      (2)If the mac address has been added into LUT, function return value is SUCCESS,  *pEntryaddr is recorded the 
 *          entry address of the Mac address stored.
 *          If all the four entries can not be replaced, it will return a  RTL8309N_LUT_FULL error, you can delete one of them 
 *          manually and rewrite the unicast address. 
 *      (3) The age of the look up table entry could be:
 *              -	AGE_TIME_OUT
 *              -	AGE_TIME_100S
 *              -	AGE_TIME_200S
 *              -	AGE_TIME_300S
 */
rtk_api_ret_t rtk_l2_addr_add(rtk_mac_t *pMac, rtk_fid_t fid, rtk_l2_ucastAddr_t *pL2_data)
{
    rtk_api_ret_t retVal;
    uint32 entryAddr;
	rtl8309n_l2_ucastEntry_t rtl8309n_l2_data;
        
    /* check input parameter */
    if((NULL == pL2_data) || (NULL == pMac))
        return RT_ERR_NULL_POINTER;
	if(pMac->octet[0] & 0x1)
		return RT_ERR_MAC;
    if (fid >= RTK_MAX_FID)
		return RT_ERR_L2_FID;	        
	if(pL2_data->auth >= RTK_ENABLE_END)
        return RT_ERR_INPUT; 
	if(pL2_data->da_block >= RTK_ENABLE_END)
        return RT_ERR_INPUT; 	
    if(pL2_data->sa_block >= RTK_ENABLE_END)
        return RT_ERR_INPUT;
	if(pL2_data->isStatic >= RTK_ENABLE_END)
		return RT_ERR_INPUT;
	if(pL2_data->age >= AGE_TIME_END)
		return RT_ERR_INPUT; 
    if(pL2_data->port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID; 
	if(pL2_data->fid >= RTK_MAX_FID)
		return RT_ERR_INPUT;	
	if(pL2_data->mac.octet[0] & 0x1)
        return RT_ERR_MAC;

	/*translate RTK parameter to driver parameter*/
	/*age*/
	switch(pL2_data->age)
	{
		case AGE_TIME_100S:
			rtl8309n_l2_data.age = RTL8309N_LUT_AGE100;
			break;

		case AGE_TIME_200S:
			rtl8309n_l2_data.age = RTL8309N_LUT_AGE200;
			break;

		case AGE_TIME_300S:
			rtl8309n_l2_data.age = RTL8309N_LUT_AGE300;
			break;

		case AGE_TIME_OUT:
			rtl8309n_l2_data.age = RTL8309N_LUT_AGEOUT;
			break;

		default:
			break;

	}
	rtl8309n_l2_data.auth = (ENABLED == pL2_data->auth) ? RTL8309N_LUT_AUTH : RTL8309N_LUT_UNAUTH;
	rtl8309n_l2_data.da_block = (ENABLED == pL2_data->da_block) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
	rtl8309n_l2_data.sa_block = (ENABLED == pL2_data->sa_block) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
	rtl8309n_l2_data.isStatic = (ENABLED == pL2_data->isStatic) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
	rtl8309n_l2_data.port = pL2_data->port;
	rtl8309n_l2_data.fid = pL2_data->fid;
	rtl8309n_l2_data.mac.octet[0] = pL2_data->mac.octet[0];
	rtl8309n_l2_data.mac.octet[1] = pL2_data->mac.octet[1];
	rtl8309n_l2_data.mac.octet[2] = pL2_data->mac.octet[2];
	rtl8309n_l2_data.mac.octet[3] = pL2_data->mac.octet[3];
	rtl8309n_l2_data.mac.octet[4] = pL2_data->mac.octet[4];
	rtl8309n_l2_data.mac.octet[5] = pL2_data->mac.octet[5];
	
	/*call ASIC driver function*/
	retVal = rtl8309n_l2_ucastAddr_add(pMac->octet, fid, &rtl8309n_l2_data, &entryAddr);
	if(RTL8309N_LUT_FULL == retVal)
    /* four way are all full, return RT_ERR_L2_INDEXTBL_FULL*/
    	return RT_ERR_L2_INDEXTBL_FULL;
	else if(retVal != SUCCESS)
		return RT_ERR_FAILED;

	return RT_ERR_OK;
}


/* Function Name:
 *      rtk_l2_addr_get
 * Description:
 *      Get a unicast entry from LUT table
 * Input:
 *      pMac               -   6 bytes unicast(I/G bit is 0) mac address to be gotten
 *      fid                   -   filtering database id, could be any value for RTL8309N switch
 * Output:
 *      pL2_data          -   the mac address attributes
 * Return:
 *      RT_ERR_OK                               -  Success
 *      RT_ERR_FAILED                         -   Failure
 *	  RT_ERR_L2_FID						-	invalid fid
 *      RT_ERR_MAC                            -   invalid mac address        
 *      RT_ERR_NULL_POINTER              -   Input parameter is null pointer    
 *      RT_ERR_L2_ENTRY_NOTFOUND    -   Specified entry not found
 * Note:
 *      (1)The lut has a 4-way entry due to an index. If the macAddress has existed in the lut, 
 *		This API will return the entry content.		
 */
rtk_api_ret_t rtk_l2_addr_get(rtk_mac_t *pMac, rtk_fid_t fid, rtk_l2_ucastAddr_t *pL2_data)
{
    rtk_api_ret_t retVal;
	uint32 entryaddr;    
	rtl8309n_l2_ucastEntry_t rtl8309n_l2_data;

    /* check input parameter */
	if(pMac->octet[0] & 0x1)
		return RT_ERR_MAC;
    if (fid >= RTK_MAX_FID)
		return RT_ERR_L2_FID;	
    if((NULL == pMac) || (NULL == pL2_data))
        return RT_ERR_NULL_POINTER;	

	/*call driver function*/
	retVal = rtl8309n_l2_ucastAddr_get(pMac->octet, fid, &rtl8309n_l2_data, &entryaddr);
    if(RTL8309N_LUT_NOTEXIST == retVal)
    {        
        return RT_ERR_L2_ENTRY_NOTFOUND;        
    }
    else if(retVal != SUCCESS)
    {
        return RT_ERR_FAILED;
    }

	/*translate RTK parameter to driver parameter*/
	switch(rtl8309n_l2_data.age)
	{
		case RTL8309N_LUT_AGE100:
			pL2_data->age = AGE_TIME_100S;
			break;

		case RTL8309N_LUT_AGE200:
			pL2_data->age = AGE_TIME_200S;
			break;

		case RTL8309N_LUT_AGE300:
			pL2_data->age = AGE_TIME_300S;
			break;

		case RTL8309N_LUT_AGEOUT:
			pL2_data->age = AGE_TIME_OUT;
			break;

		default:
			break;
	}
	pL2_data->auth = rtl8309n_l2_data.auth;
	pL2_data->da_block = rtl8309n_l2_data.da_block;
	pL2_data->sa_block = rtl8309n_l2_data.sa_block;
	pL2_data->isStatic = rtl8309n_l2_data.isStatic;
	pL2_data->port = rtl8309n_l2_data.port;
	pL2_data->fid = rtl8309n_l2_data.fid;
	pL2_data->mac.octet[0] = rtl8309n_l2_data.mac.octet[0];
	pL2_data->mac.octet[1] = rtl8309n_l2_data.mac.octet[1];
	pL2_data->mac.octet[2] = rtl8309n_l2_data.mac.octet[2];
	pL2_data->mac.octet[3] = rtl8309n_l2_data.mac.octet[3];
	pL2_data->mac.octet[4] = rtl8309n_l2_data.mac.octet[4];
	pL2_data->mac.octet[5] = rtl8309n_l2_data.mac.octet[5];

    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_l2_addr_del
 * Description:
 *      Delete a LUT unicast entry
 * Input:
 *      pMac               -   6 bytes unicast mac address to be deleted
 *      fid                   -   filtering database id, could be any value for RTL8309N switch
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                               -  Success
 *      RT_ERR_FAILED                         -   Failure
 *      RT_ERR_MAC                            -   Wrong mac address, must be unicast mac 
 *	  RT_ERR_L2_FID						-	invalid fid
 *      RT_ERR_NULL_POINTER              -   Input parameter is null pointer    
 *      RT_ERR_L2_ENTRY_NOTFOUND    -   Specified entry not found 
 * Note:
 *      If the mac has existed in the LUT, it will be deleted.
 *      Otherwise, it will return RT_ERR_L2_ENTRY_NOTFOUND.
 */
rtk_api_ret_t rtk_l2_addr_del(rtk_mac_t *pMac, rtk_fid_t fid)
{
    rtk_api_ret_t retVal;
    uint32 entryAddr;

    /* must be unicast address */
    if((NULL == pMac) || (pMac->octet[0] & 0x1))
        return RT_ERR_MAC;  
    if (fid > RTK_MAX_FID)
		return RT_ERR_L2_FID;

    retVal = rtl8309n_l2_ucastAddr_del(pMac->octet, fid, &entryAddr);
    if(RTL8309N_LUT_NOTEXIST == retVal)
    {
        return RT_ERR_L2_ENTRY_NOTFOUND;
    }
    else if(retVal != SUCCESS)
    {
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_l2_mcastAddr_add
 * Description:
 *      Add a LUT multicast entry
 * Input:
 *      pMac               -   6 bytes unicast mac address to be deleted
 *      fid                   -   filtering database id, could be any value for RTL8309N switch
 *      portmask          -   Port mask to be forwarded to 
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                               -  Success
 *      RT_ERR_FAILED                         -   Failure
 *      RT_ERR_PORT_MASK                  -   Invalid port mask
 *      RT_ERR_MAC                            -   invalid mac address 
 *	  RT_ERR_NULL_POINTER			-	input parameter is null pointer
 *      RT_ERR_L2_INDEXTBL_FULL         -   the L2 index table is full
 * Note:
 *      If the multicast mac address already existed in the LUT, it will udpate the
 *      port mask of the entry. Otherwise, it will find an empty or asic auto learned
 *      entry to write. If all the entries with the same hash value can't be replaced, 
 *      ASIC will return a RT_ERR_L2_INDEXTBL_FULL error.
 */
rtk_api_ret_t rtk_l2_mcastAddr_add(rtk_mac_t *pMac, rtk_fid_t fid, rtk_portmask_t portmask)
{
    rtk_api_ret_t retVal;
    uint32 entryAddr;
    
	if (NULL == pMac)
		return RT_ERR_NULL_POINTER;
    /* must be L2 multicast address */
    if (!(pMac->octet[0] & 0x1))
        return RT_ERR_MAC;
	if (fid >= RTK_MAX_FID)
		return RT_ERR_L2_FID;
    if(portmask.bits[0]> RTK_MAX_PORT_MASK)
        return RT_ERR_PORT_MASK;    

    retVal = rtl8309n_l2_mcastAddr_add(pMac->octet, fid, portmask.bits[0], &entryAddr);
    /*If four way are all full, return RTL8309N_LUT_FULL*/
    if (RTL8309N_LUT_FULL == retVal) 
        return RT_ERR_L2_INDEXTBL_FULL;
	else if(FAILED == retVal)
		return RT_ERR_FAILED;
	
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_l2_mcastAddr_get
 * Description:
 *      Get a LUT multicast entry
 * Input:
 *      pMac               -   6 bytes multicast(I/G bit is 0) mac address to be gotten
 *      fid                   -   filtering database id, could be any value for RTL8309N switch
 * Output:
 *      pPortmask         -   the pointer of port mask      
 * Return:
 *      RT_ERR_OK                               -  Success
 *      RT_ERR_FAILED                         -   Failure
 *      RT_ERR_NULL_POINTER              -   Input parameter is null pointer    
 *      RT_ERR_MAC                            -   invalid mac address        
 *      RT_ERR_L2_ENTRY_NOTFOUND         -   specified entry not found
 * Note:
 *      If the multicast mac address existed in LUT, it will return the port mask where
 *      the packet should be forwarded to, Otherwise, it will return a 
 *      RT_ERR_L2_ENTRY_NOTFOUND error.
 */
rtk_api_ret_t rtk_l2_mcastAddr_get(rtk_mac_t *pMac, rtk_fid_t fid, rtk_portmask_t *pPortmask)
{
    rtk_api_ret_t retVal;
    uint32 entryAddr; 
	uint32 rtl8309n_portMask;

	if ((NULL == pMac) || (NULL == pPortmask))
		return RT_ERR_NULL_POINTER;
    /* must be multicast address */
    if(!(pMac->octet[0] & 0x1))
        return RT_ERR_MAC;  
	if(fid >= RTK_MAX_FID)
		return RT_ERR_L2_FID;
	
    retVal = rtl8309n_l2_mcastAddr_get(pMac->octet, fid, &rtl8309n_portMask, &entryAddr);
    if(RTL8309N_LUT_NOTEXIST == retVal)
    {        
        return RT_ERR_L2_ENTRY_NOTFOUND;        
    }
    else if(retVal != SUCCESS)
    {
        return RT_ERR_FAILED;
    }

	pPortmask->bits[0] = rtl8309n_portMask;
    return SUCCESS;
}

/* Function Name:
 *      rtk_l2_mcastAddr_del
 * Description:
 *      Delete a LUT unicast entry
 * Input:
 *      pMac               -   6 bytes multicast(I/G bit is 1) mac address to be gotten
 *      fid                   -   filtering database id, could be any value for RTL8309N switch
 * Output:
*       none
 * Return:
 *      RT_ERR_OK                               -  Success
 *      RT_ERR_FAILED                         -   Failure
 *      RT_ERR_MAC                            -   invalid mac address
 *	  RT_ERR_L2_FID				-	invalid fid
 *      RT_ERR_L2_ENTRY_NOTFOUND     -   specified entry not found
 * Note:
 *      If the mac has existed in the LUT, it will be deleted.
 *      Otherwise, it will return RT_ERR_L2_ENTRY_NOTFOUND.
 */
rtk_api_ret_t rtk_l2_mcastAddr_del(rtk_mac_t *pMac, rtk_fid_t fid)
{
    rtk_api_ret_t retVal;
    uint32 entryAddr;

    /* must be multicast address */
    if(NULL == pMac)
		return RT_ERR_FAILED;
	if(!(pMac->octet[0] & 0x1))
        return RT_ERR_MAC;  
	if(fid >= RTK_MAX_FID)
		return RT_ERR_L2_FID;
	
    retVal = rtl8309n_l2_mcastAddr_del(pMac->octet, fid, &entryAddr);
    if(RTL8309N_LUT_NOTEXIST == retVal)
    {
        return RT_ERR_L2_ENTRY_NOTFOUND;
    }
    else if(retVal != SUCCESS)
    {
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

#if 0
/* Function Name:
 *		rtk_l2_hashMethod_set
 * Description:
 *		Set l2 table hash algorithm
 * Input:
 *		mode	-	hash mode
 * Output:
 *		none
 * Return:
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 *		RT_ERR_INPUT	-	invalid input parameter
 * Note:
 *		There are 2 types of hash algorithm for RTL8309N:
 *		Type I and Type II. 
 *		The value of input parameter mode would be:
 *		    -	HASH_OPT0
 *		    -	HASH_OPT1
 */
rtk_api_ret_t rtk_l2_hashMethod_set(rtk_hash_method_t mode)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_hash_mode;

	if(mode >= HASH_END)
		return RT_ERR_INPUT;
	
	rtl8309n_hash_mode = (HASH_OPT1 == mode) ? RTL8309N_HASH_OPT1 : RTL8309N_HASH_OPT0;
	retVal = rtl8309n_l2_hashmode_set(rtl8309n_hash_mode);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	
	return RT_ERR_OK;
}

/* Function Name:
 *		rtk_l2_hashMethod_get
 * Description:
 *		Get l2 table hash algorithm
 * Input:
 *		none
 * Output:
 *		pMode	-	pointer point to hash algorithm
 * Return:
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 * Note:
 *		There are 2 types of hash algorithm for RTL8309N:
 *		Type I and Type II. 
 *		The value of output parameter would be:
 *		The value of input parameter mode would be:
 *		    -	HASH_OPT0
 *		    -	HASH_OPT1
 */
rtk_api_ret_t rtk_l2_hashMethod_get(rtk_hash_method_t *pMode)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_hash_mode;

	retVal = rtl8309n_l2_hashmode_get(&rtl8309n_hash_mode);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	*pMode = ( RTL8309N_HASH_OPT1 == rtl8309n_hash_mode) ? HASH_OPT1 : HASH_OPT0;

	return RT_ERR_OK;
}
#endif

/* Function Name:
 *		rtk_l2_limitLearningSysCntEnable_set
 * Description:
 *		Enable system mac learning limit function
 * Input:
 *		enabled		-	ENABLED or DISABLED
 * Output:
 *		none
 * Return:
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 * Note:
 *		For RTL8309N, mac learning limit function can be enabled or disabled for a whole system.
 */
rtk_api_ret_t rtk_l2_limitLearningSysCntEnable_set(rtk_enable_t enabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;

	rtl8309n_enabled = (ENABLED == enabled)? RTL8309N_ENABLED : RTL8309N_DISABLED;
	retVal = rtl8309n_l2_systemMacLimitEnable_set(rtl8309n_enabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	
	return  RT_ERR_OK;
}

/* Function Name:
 *		rtk_l2_limitLearningSysCntEnable_set
 * Description:
 *		Get enabled status of system mac learning limit function
 * Input:
 *		none
 * Output:
 *		pEnabled	-	pointer point to enabled status of system mac learning limit function
 * Return:
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 * Note:
 *		For RTL8309N, mac learning limit function can be enabled or disabled for a whole system.
 */
rtk_api_ret_t rtk_l2_limitLearningSysCntEnable_get(rtk_enable_t *pEnabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;

	retVal = rtl8309n_l2_systemMacLimitEnable_get(&rtl8309n_enabled);
	if(retVal != RT_ERR_OK)
		return RT_ERR_FAILED;
	*pEnabled = (RTL8309N_ENABLED == rtl8309n_enabled) ? ENABLED : DISABLED;
	
	return  RT_ERR_OK;
}


/* Function Name:
 *		rtk_l2_limitLearningSysCnt_set
 * Description:
 *      Set system mac limitting max value and port merge mask
 * Input:
 *		mac_cnt		-	system mac limitting value
 *		mergeMask	-	a set describing the ports whose port mac limitting value are counted into system mac limitting value
 * Output:
 *      none
 * Return:
 *      RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 *		RT_ERR_INPUT	-	invalid input parameter 
 * Note:
 *		(1) This API can be called to set system mac limitting max value and port merge mask.
 *		(2)	mac_cnt: the whole system mac limitting max value, it's value is from 0 - 0xFF;
 *		(3) mergeMask: the ports whose mac limitting counter value are counted into the system mac limitting counter value,
 *			it's value is from 0 - 0x1FF. If bit n is 1, it means that port n is counted.
 */
rtk_api_ret_t rtk_l2_limitLearningSysCnt_set(rtk_mac_cnt_t mac_cnt, rtk_portmask_t mergeMask)
{
	rtk_api_ret_t retVal;

	if((mac_cnt > RTK_MAX_NUM_OF_SYSTEM_LEARN_LIMIT) || (mergeMask.bits[0] > RTK_MAX_PORT_MASK))
		return RT_ERR_INPUT;

	retVal = rtl8309n_l2_systemMacLimit_set(mac_cnt, mergeMask.bits[0]);
	if(retVal != RT_ERR_OK)
		return RT_ERR_FAILED;
	
    return RT_ERR_OK;
}

/* Fcuntion Name:
 *		rtk_l2_limitLearningSysCnt_get
 * Description:
 *		Get system mac limitting max value and merge mask.
 * Input:
 *		none
 * Output:
 *		pMac_cnt	-	pointer point to system mac limitting max value
 *		pMergeMask	-	pointer point to merge mask
 * Return:
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 *		RT_ERR_NULL_POINTER		-	pointer is NULL
 * Note:
 *		(1) This API can be called to get system mac limitting max value and port merge mask.
 *		(2)	mac_cnt: the whole system mac limitting max value, it's value is from 0 - 0xFF;
 *		(3) mergeMask: the ports whose mac limitting counter value are counted into the system mac limitting counter value,
 *			it's value is from 0 - 0x1FF. If bit n is 1, it means that port n is counted.
 */
rtk_api_ret_t rtk_l2_limitLearningSysCnt_get(rtk_mac_cnt_t *pMac_cnt, rtk_portmask_t *pMergeMask)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_mac_cnt;
	uint32 rtl8309n_portMask;
	
	if((NULL == pMac_cnt) || (NULL == pMergeMask))
		return RT_ERR_NULL_POINTER;
	
	retVal = rtl8309n_l2_systemMacLimit_get(&rtl8309n_mac_cnt, &rtl8309n_portMask);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	*pMac_cnt = rtl8309n_mac_cnt;
	pMergeMask->bits[0] = rtl8309n_portMask;
	
    return RT_ERR_OK;
}

/* Function Nmae:
 *		rtk_l2_limitLearningCntEnable_set
 * Description:
 *		Enable port mac limit learning function
 * Input:
 *		port	-	port id
 *		enabled	-	enable or disable
 * Output:
 *		none
 * Return:
 *		RT_ERR_PORT_ID	-	invalid port id
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 * Note:
 *		This API can be called to enable or disable mac limit learning function for a port.
 */
rtk_api_ret_t rtk_l2_limitLearningCntEnable_set(rtk_port_t port, rtk_enable_t enabled)
{
	rtk_api_ret_t retVal;

	if (port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;

	retVal = rtl8309n_l2_portMacLimitEnable_set(port, (enabled == ENABLED) ? RTL8309N_ENABLED : RTL8309N_DISABLED);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;

	return RT_ERR_OK;
}

/* Function Nmae:
 *		rtk_l2_limitLearningCntEnable_get
 * Description:
 *		Get enabled status of port mac limit learning function
 * Input:
 *		port	-	port id
 * Output:
 *		pEnabled	-	enable or disable
 * Return:
 *		RT_ERR_PORT_ID			-	invalid port id
 *		RT_ERR_NULL_POINTER		-	input parameter is null pointer
 *		RT_ERR_FAILED			-	failure
 *		RT_ERR_OK				-	success
 * Note:
 *		This API can be called to get the enabled status of mac limit learning function for a port.
 */
rtk_api_ret_t rtk_l2_limitLearningCntEnable_get(rtk_port_t port, rtk_enable_t *pEnabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;

	if (port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;

	if (NULL == pEnabled)
		return RT_ERR_NULL_POINTER;

	retVal = rtl8309n_l2_portMacLimitEnable_get(port, &rtl8309n_enabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;

	*pEnabled = (RTL8309N_ENABLED == rtl8309n_enabled) ? ENABLED : DISABLED;

	return RT_ERR_OK;
}

/* Function Name:
 *      rtk_l2_limitLearningCnt_set
 * Description:
 *      Set per-Port auto learning limit counter max value
 * Input:
 *      port 		- 	Port id	
 *      mac_cnt		- 	mac limit counter value
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK              - 	set shared meter successfully
 *      RT_ERR_FAILED          -	 FAILED to iset shared meter
 *      RT_ERR_PORT_ID 		- 	Invalid port id.
 *      RT_ERR_LIMITED_L2ENTRY_NUM 	- 	invalid limited L2 entry number 
 * Note:
 *      (1) Per port mac learning limit function can be enabled or disabled independently;
 *      (2) mac_cnt: port mac learning limit max value, it's value is from 0 - 0x1F;
 */
rtk_api_ret_t rtk_l2_limitLearningCnt_set(rtk_port_t port, rtk_mac_cnt_t mac_cnt)
{
    rtk_api_ret_t retVal;
    
    if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;

    if (mac_cnt > RTK_MAX_NUM_OF_PORT_LEARN_LIMIT)
        return RT_ERR_LIMITED_L2ENTRY_NUM;

    if ((retVal = rtl8309n_l2_portMacLimit_set(port, (uint32)mac_cnt)) != SUCCESS)
        return RT_ERR_FAILED; 
    
    return RT_ERR_OK;
}    

/* Function Name:
 *      rtk_l2_limitLearningCnt_get
 * Description:
 *      Get per-Port auto learning limit counter max value
 * Input:
 *      port - Port id.
 * Output:
 *      pMac_cnt 	- 	mac limit counter value
 * Return:
 *      RT_ERR_OK              - 	Success
 *      RT_ERR_FAILED          - 	Failure 
 *      RT_ERR_PORT_ID 		   -    Invalid port id. 
 *      RT_ERR_NULL_POINTER    - 	input parameter is NULL pointer
 * Note:
 *      (1) Per port mac learning limit function can be enabled or disabled independently;
 *      (2) mac_cnt: port mac learning limit max value, it's value is from 0 - 0x1F;
 */
rtk_api_ret_t rtk_l2_limitLearningCnt_get(rtk_port_t port, rtk_mac_cnt_t *pMac_cnt)
{
    rtk_api_ret_t retVal;

    if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;
	if (NULL == pMac_cnt)
		return RT_ERR_NULL_POINTER;
	
    if ((retVal = rtl8309n_l2_portMacLimit_get(port, (uint32 *)pMac_cnt)) != RT_ERR_OK)
        return RT_ERR_FAILED;
	
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_l2_limitLearningCntAction_set
 * Description:
 *      Configure auto learn over limit number action.
 * Input:
 *		port	-	port id, no usage for RTL8309N
 *      action 	- Auto learning entries limit number
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK              - Success
 *      RT_ERR_FAILED          - Failure 
 *      RT_ERR_PORT_ID - Invalid port number.
 *      RT_ERR_NOT_ALLOWED - Invalid learn over action
 * Note:
 *      (1)The API can set SA unknown packet action while auto learn limit number is over. 
 *      The action symbol as following:
 *          -	LIMIT_LEARN_CNT_ACTION_DROP
 *          -	LIMIT_LEARN_CNT_ACTION_TO_CPU
 */
rtk_api_ret_t rtk_l2_limitLearningCntAction_set(rtk_port_t port, rtk_l2_limitLearnCntAction_t action)
{
    rtk_api_ret_t retVal;
    uint32 rtl8309n_action;
	
	
    if ( LIMIT_LEARN_CNT_ACTION_DROP == action )
        rtl8309n_action = RTL8309N_ACT_DROP;
    else if ( LIMIT_LEARN_CNT_ACTION_TO_CPU == action )
        rtl8309n_action = RTL8309N_ACT_TRAP2CPU;
    else
        return RT_ERR_NOT_ALLOWED;    

    port = RTL8309N_PORT0;
    
    if ((retVal = rtl8309n_l2_macLimitAction_set(rtl8309n_action)) != RT_ERR_OK)
        return RT_ERR_FAILED; 

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_l2_limitLearningCntAction_get
 * Description:
 *      Get auto learn over limit number action.
 * Input:
 *		port	-	port id, no usage for RTL8309N
 * Output:
 *      pAction - Learn over action
 * Return:
 *      RT_ERR_OK              - Success
 *      RT_ERR_FAILED          - Failure 
 *      RT_ERR_PORT_ID 	- Invalid port id. 
 *	  RT_ERR_NULL_POINTER	-	null pointer
 *	  
 * Note:
 *      (1)The API can get SA unknown packet action while auto learn limit number is over. 
 *      The action symbol as following:
 *          -	LIMIT_LEARN_CNT_ACTION_DROP  
 *          -	LIMIT_LEARN_CNT_ACTION_TO_CPU 
 */
rtk_api_ret_t rtk_l2_limitLearningCntAction_get(rtk_port_t port, rtk_l2_limitLearnCntAction_t *pAction)
{
    rtk_api_ret_t retVal;
    uint32 rtl8309n_action; 


	if(NULL == pAction)
		return RT_ERR_NULL_POINTER;

    port = RTL8309N_PORT0;

    if ((retVal = rtl8309n_l2_macLimitAction_get(&rtl8309n_action)) != RT_ERR_OK)
        return retVal; 

    if (RTL8309N_ACT_DROP == rtl8309n_action)
        *pAction = LIMIT_LEARN_CNT_ACTION_DROP;
    else if (RTL8309N_ACT_TRAP2CPU == rtl8309n_action)
        *pAction = LIMIT_LEARN_CNT_ACTION_TO_CPU;
    else
        return RT_ERR_FAILED; 

    return RT_ERR_OK;
}

/* Function Name:
 *		rtk_l2_learningSysCnt_get
 * Description:
 *		Get current value of system auto learning mac counter
 * Input:
 *		none
 * Output:
 *		pMac_cnt	-	mac counter
 * Return:
 *		RT_ERR_FAILED
 *		RT_ERR_OK
 *		RT_ERR_NULL_POINTER
 * Note:
 */
rtk_api_ret_t rtk_l2_learningSysCnt_get(rtk_mac_cnt_t *pMac_cnt)
{
	rtk_api_ret_t retVal;

	if (NULL == pMac_cnt)
		return RT_ERR_NULL_POINTER;

	retVal = rtl8309n_l2_systemLearningCnt_get((uint32 *)pMac_cnt);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	return RT_ERR_OK;
}

/* Function Name:
 *      rtk_l2_learningCnt_get
 * Description:
 *      Get current value of per-Port auto learning counter
 * Input:
 *      port - Port id.
 * Output:
 *      pMac_cnt - ASIC auto learning entries number
 * Return:
 *      RT_ERR_OK              - 	Success
 *	  RT_ERR_FAILED		-	Failure
 *      RT_ERR_PORT_ID 	- 	Invalid port number. 
 *      RT_ERR_NULL_POINTER   -   Input parameter is null pointer  
 * Note:
 *      The API can get per-port ASIC auto learning number
 */
rtk_api_ret_t rtk_l2_learningCnt_get(rtk_port_t port, rtk_mac_cnt_t *pMac_cnt)
{
    uint32 retVal;

    
    if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;
    if (NULL == pMac_cnt)
        return RT_ERR_NULL_POINTER;

 	retVal = rtl8309n_l2_portlearningCnt_get(port, (uint32 *)pMac_cnt);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;

    return RT_ERR_OK;
}

/* Function Name:
 *		rtk_cpu_enable_set
 * Description:
 *		Enable cpu port ability
 * Input:
 *		enabled	-	enable or disable	
 * Output:
 *		none
 * Return:
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 * Note:
 *		
 */
rtk_api_ret_t rtk_cpu_enable_set(rtk_enable_t enabled)
{
	rtk_api_ret_t retVal;

	retVal = rtl8309n_cpu_enable_set((ENABLED == enabled) ? RTL8309N_ENABLED : RTL8309N_DISABLED);
	if (retVal != SUCCESS)
		return RT_ERR_FAILED;

	return RT_ERR_OK;
}

/* Function Name:
 *		rtk_cpu_enable_get
 * Description:
 *		Enable cpu port ability
 * Input:
 *		none	
 * Output:
 *		pEnabled	-	enable or disable	
 * Return:
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 *		RT_ERR_NULL_POINTER	-	input parameter is null pointer
 * Note:
 *			
 */
rtk_api_ret_t rtk_cpu_enable_get(rtk_enable_t *pEnabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;

	if (NULL == pEnabled)
		return RT_ERR_NULL_POINTER;
	
	retVal = rtl8309n_cpu_enable_get(&rtl8309n_enabled);
	if (retVal != SUCCESS)
		return RT_ERR_FAILED;
	*pEnabled = (RTL8309N_ENABLED == rtl8309n_enabled) ? ENABLED : DISABLED;
	
	return RT_ERR_OK;	
}

/* Function Name:
 *		rtk_cpu_tagPort_set
 * Description:
 *		Set cpu port and insert cpu tag
 * Input:
 *		port		-	port id
 *		enTag	-	enable insert cpu tag, enable or disable	
 * Output:
 *		none
 * Return:
 *		RT_ERR_PORT_ID	-	invalid port id
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 * Note:
 *		
 */
rtk_api_ret_t rtk_cpu_tagPort_set(rtk_port_t port, rtk_enable_t enTag)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;
	
	if (port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;

	rtl8309n_enabled = (ENABLED == enTag) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
	retVal = rtl8309n_cpu_port_set(port, rtl8309n_enabled);
	if (retVal != SUCCESS)
		return RT_ERR_FAILED;

	return RT_ERR_OK;
}

/* Function Name:
 *		rtk_cpu_tagPort_get
 * Description:
 *		Get cpu port and insert cpu tag status
 * Input:
 *		port	-	port id(0 - 8)
 *		enTag	-	enable insert cpu tag, enable or disable	
 * Output:
 *		none
 * Return:
 *		RT_ERR_PORT_ID	-	invalid port id
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 * Note:
 *		
 */
rtk_api_ret_t rtk_cpu_tagPort_get(rtk_port_t *pPort, rtk_enable_t *pEnTag)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;

	if ((NULL == pPort) || (NULL == pEnTag))
		return RT_ERR_NULL_POINTER;

	retVal = rtl8309n_cpu_port_get(pPort, &rtl8309n_enabled);
	if (retVal != SUCCESS)
		return RT_ERR_FAILED;
	
	if (RTL8309N_NOCPUPORT == *pPort)/*CPU port is disabled*/
		return RT_ERR_FAILED;
	*pEnTag = (RTL8309N_ENABLED == rtl8309n_enabled) ? ENABLED : DISABLED;
	
	return RT_ERR_OK;	
}

#if 0
/* Function Name:
 *		rtk_cpu_tagInsertMask_set
 * Description:
 *		Enable insert cpu tag for specail packets
 * Input:
 *		type	-	special packet type
 *      enabled -   enable , disable
 * Output:
 *		none
 * Return:
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 *		RT_ERR_INPUT	-	invalid input parameter
 * Note:
 *		This API can be called to get status of inserting cpu tag for special packets
 *		The type parameter could be value as follows:
 *		    -	CPU_TRAP_DOT1X
 *		    -	CPU_TRAP_VID
 *		    -	CPU_TRAP_ISPMAC
 *		    -	CPU_TRAP_ACL
 *		    -	CPU_TRAP_IGMPMLD
 *		    -	CPU_TRAP_RMA
 *		    -	CPU_TRAP_EXMACLIMIT 	
 *		    -	CPU_TRAP_RLPP
 *		    -	CPU_TRAP_LUTMACBLOCK
 *		    -	CPU_TRAP_ALL
 */
rtk_api_ret_t rtk_cpu_tagInsertMask_set(rtk_cpu_tagTrapPkt_t type, rtk_enable_t enabled)
{
	rtk_api_ret_t retVal;
    rtl8309n_cpu_trapPkt_t rtl8309n_type, rtl8309n_enabled;
    
    if (type >= CPU_TRAP_END || enabled > ENABLED)
        return RT_ERR_INPUT;

    rtl8309n_type = type;
    rtl8309n_enabled = enabled;
	retVal = rtl8309n_cpu_tagInsertMask_set(rtl8309n_type, rtl8309n_enabled);
	if (retVal != SUCCESS)
		return RT_ERR_FAILED;

	return RT_ERR_OK;

}

/* Function Name:
 *		rtk_cpu_tagInsertMask_get
 * Description:
 *		Get enabled status of inserting cpu tag for specail packets
 * Input:
 *		type - special packet type
 * Output:
 *		pEnabled    -   enable,disable
 * Return:
 *		RT_ERR_FAILED	-	failure
 *		RT_ERR_OK		-	success
 * Note:
 *		This API can be called to get status of inserting cpu tag for special packets
 *		The type parameter could be value as follows:
 *		    -	CPU_TRAP_DOT1X
 *		    -	CPU_TRAP_VID
 *		    -	CPU_TRAP_ISPMAC
 *		    -	CPU_TRAP_ACL
 *		    -	CPU_TRAP_IGMPMLD
 *		    -	CPU_TRAP_RMA
 *		    -	CPU_TRAP_EXMACLIMIT 	
 *		    -	CPU_TRAP_RLPP
 *		    -	CPU_TRAP_LUTMACBLOCK
 *		    -	CPU_TRAP_ALL
 */
rtk_api_ret_t rtk_cpu_tagInsertMask_get(rtk_cpu_tagTrapPkt_t type, rtk_enable_t *pEnabled)
{
	rtk_api_ret_t retVal;
	rtl8309n_cpu_trapPkt_t asic_type;
    uint32 asic_renabled;
	
	if(type >= CPU_TRAP_END || pEnabled == NULL)
        return RT_ERR_INPUT;

    asic_type = type;
	retVal = rtl8309n_cpu_tagInsertMask_get(asic_type, &asic_renabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
    *pEnabled = asic_renabled;
    
	return RT_ERR_OK;
}
#endif

/* Function Name:
 *      rtk_mirror_portBased_set
 * Description:
 *      Set port mirror function parameters
 * Input:
 *      mirroring_port             - Monitor port, 7 means no monitor port
 *      pMirrored_rx_portmask      - the pointer of Rx mirror port mask
 *      pMirrored_tx_portmask      - the pointer of Tx mirror port mask
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                      -  Success
 *      RT_ERR_FAILED                -   Failure
 *      RT_ERR_PORT_MASK         -   Invalid port mask
 *	  RT_ERR_PORT_ID			-	invalid port id
 *	  RT_ERR_NULL_POINTER		-	input parameter is null pointer
 * Note:
 *      The API is called to set mirroring port and mirrored rx and tx port mask. 
 */
rtk_api_ret_t rtk_mirror_portBased_set(rtk_port_t mirroring_port, rtk_portmask_t *pMirrored_rx_portmask, rtk_portmask_t *pMirrored_tx_portmask)
{
    rtk_api_ret_t retVal;

	if (mirroring_port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
	
    if ((NULL == pMirrored_rx_portmask) || (NULL == pMirrored_tx_portmask))
        return RT_ERR_NULL_POINTER;
    
    if (pMirrored_rx_portmask->bits[0] > RTL8309N_MAX_PORTMASK)
        return RT_ERR_PORT_MASK; 

    if (pMirrored_tx_portmask->bits[0] > RTL8309N_MAX_PORTMASK)
        return RT_ERR_PORT_MASK;

    retVal = rtl8309n_mirror_portBased_set(mirroring_port, pMirrored_rx_portmask->bits[0], pMirrored_tx_portmask->bits[0]);
	if (retVal != SUCCESS)
		return RT_ERR_FAILED;
	
	return RT_ERR_OK;
}

/* Function Name:
 *      rtk_mirror_portBased_get
 * Description:
 *      Get port mirror function parameters
 * Input:
 *      none 
 * Output:
 *      pMirroring_port             - the pointer Monitor port, 7 means no monitor port
 *      pMirrored_rx_portmask   - the pointer of Rx mirror port mask
 *      pMirrored_tx_portmask   - the pointer of Tx mirror port mask 
 * Return:
 *      RT_ERR_OK                      -  Success
 *      RT_ERR_FAILED                -   Failure
 *      RT_ERR_NULL_POINTER      -   Input parameter is null pointer 
 * Note:
 *      The API is to set mirror function of source port and mirror port. 
 */
rtk_api_ret_t rtk_mirror_portBased_get(rtk_port_t *pMirroring_port, rtk_portmask_t *pMirrored_rx_portmask, rtk_portmask_t *pMirrored_tx_portmask)
{
    rtk_api_ret_t retVal;
    
    if ((NULL == pMirrored_rx_portmask) || (NULL == pMirrored_tx_portmask) ||
        (NULL == pMirroring_port))
        return RT_ERR_NULL_POINTER;
    
    retVal = rtl8309n_mirror_portBased_get(pMirroring_port, 
                &(pMirrored_rx_portmask->bits[0]),
                &(pMirrored_tx_portmask->bits[0]));
	if (retVal != SUCCESS)
		return RT_ERR_FAILED;
	
	return RT_ERR_OK;

}

/* Function Name:
 *      rtk_mirror_macBased_set
 * Description:
 *      Set Mac address for mirror packet
 * Input:
 *      macAddr   - mirrored mac address, it could be SA or DA of the packet 
 *      enabled   - enable mirror packet by mac address
 * Output:
 *      none
 * Return:
 *      RT_ERR_FAILED	-	failure
 *      RT_ERR_OK		-	success
 * Note:
 */
rtk_api_ret_t rtk_mirror_macBased_set(rtk_mac_t *macAddr, rtk_enable_t enabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;
	
	if(NULL == macAddr)
		return RT_ERR_FAILED;

	rtl8309n_enabled = (ENABLED == enabled) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
	if((retVal = rtl8309n_mirror_macBased_set(macAddr->octet, rtl8309n_enabled)) != SUCCESS)
		return RT_ERR_FAILED;

	return RT_ERR_OK;	
}

/* Function Name:
 *      rtk_mirror_macBased_get
 * Description:
 *      get Mac address for mirror packet
 * Input:
 *      none 
 * Output:
 *      macAddr    - mirrored mac address, it could be SA or DA of the packet 
 *      pEnabled   - the pointer of enable mirror packet by mac address 
 * Return:
 *      RT_ERR_FAILED	-	failure
 *      RT_ERR_OK		-	success
 * Note:
 */
rtk_api_ret_t rtk_mirror_macBased_get(rtk_mac_t *macAddr, uint32 *pEnabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_enabled;

	if(NULL == macAddr)
		return RT_ERR_FAILED;
	
	if((retVal = rtl8309n_mirror_macBased_get(macAddr->octet, &rtl8309n_enabled)) != SUCCESS)
		return RT_ERR_FAILED;
	
	*pEnabled = (RTL8309N_ENABLED == rtl8309n_enabled) ? ENABLED : DISABLED;
	
	return RT_ERR_OK;	
}


/* Function Name:
 *      rtk_dot1x_unauthPacketOper_set
 * Description:
 *      Set 802.1x unauth action configuration
 * Input:
 *      port                 -   Port id, no use for RTL8309N switch
 *      unauth_action        -   802.1X unauth action    
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                      -  Success
 *      RT_ERR_FAILED                -   Failure
 *      RT_ERR_DOT1X_PROC        -   Unauthorized behavior error
 * Note:
 *      This API can set 802.1x unauth action configuration, 
 *      for RTL8309N switch, the action is by whole system,
 *      so port could be any value of 0~8.
 *      The unauth action is as following:
 *          -	DOT1X_ACTION_DROP
 *          -	DOT1X_ACTION_TRAP2CPU
 */
rtk_api_ret_t rtk_dot1x_unauthPacketOper_set(rtk_port_t port, rtk_dot1x_unauth_action_t unauth_action)
{
    rtk_api_ret_t retVal;

    if(unauth_action > DOT1X_ACTION_TRAP2CPU)
        return RT_ERR_DOT1X_PROC;

    port = RTL8309N_PORT0;

    if(DOT1X_ACTION_DROP == unauth_action)
    {
        retVal = rtl8309n_trap_abnormalPktAction_set(RTL8309N_DOT1X_UNAUTH, RTL8309N_ACT_DROP);
    }
    else if(DOT1X_ACTION_TRAP2CPU == unauth_action)
    {
        retVal = rtl8309n_trap_abnormalPktAction_set(RTL8309N_DOT1X_UNAUTH, RTL8309N_ACT_TRAP2CPU);
    }
    else
        retVal = RT_ERR_DOT1X_PROC;

	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_dot1x_unauthPacketOper_get
 * Description:
 *      Get 802.1x unauth action configuration
 * Input:
 *      port                  -   Port id, no use for RTL8309N switch
 * Output:
 *      pUnauth_action   -  the pointer of 802.1X unauth action    
 * Return:
 *      RT_ERR_OK                      -  Success
 *      RT_ERR_FAILED                -   Failure
 *      RT_ERR_NULL_POINTER      -   Input parameter is null pointer
 * Note:
 *      This API can set 802.1x unauth action configuration, for RTL8309N switch, the action is by whole system,
 *      so port could be any value of 0~8.
 *      The unauth action is as following:
 *          -	DOT1X_ACTION_DROP
 *          -	DOT1X_ACTION_TRAP2CPU
 */
rtk_api_ret_t rtk_dot1x_unauthPacketOper_get(rtk_port_t port, rtk_dot1x_unauth_action_t *pUnauth_action)
{
    rtk_api_ret_t retVal;
    uint32 action;
    
    if(NULL == pUnauth_action)
        return RT_ERR_NULL_POINTER;

    port = RTL8309N_PORT0;

    
    retVal = rtl8309n_trap_abnormalPktAction_get(RTL8309N_DOT1X_UNAUTH, &action);

    if (retVal != SUCCESS)
        return RT_ERR_FAILED;
    
    if(RTL8309N_ACT_DROP == action)
    {
        *pUnauth_action = DOT1X_ACTION_DROP;
    }
    else 
    {
        *pUnauth_action = DOT1X_ACTION_TRAP2CPU;
    }
    
    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_dot1x_portBasedEnable_set
 * Description:
 *      Set 802.1x port-based enable configuration
 * Input:
 *      port                  -   Port id
 *      enable               -   enable or disable
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                      -  Success
 *      RT_ERR_FAILED                -   Failure
 *      RT_ERR_PORT_ID              -   Invalid port id
 * Note:
 *      The API can update the port-based port enable register content. If a port is 802.1x 
 *      port based network access control "enabled", it should be authenticated so packets 
 *      from that port won't be dropped or trapped to CPU. 
 *      The status of 802.1x port-based network access control is as following:
 *          - DISABLED
 *          - ENABLED
 */    
rtk_api_ret_t rtk_dot1x_portBasedEnable_set(rtk_port_t port, rtk_enable_t enabled)
{
    rtk_api_ret_t retVal;
    uint32 enDot1x;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    

	enDot1x = (ENABLED == enabled) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
    retVal = rtl8309n_dot1x_portBasedEnable_set(port, enDot1x);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;
    
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_dot1x_portBasedEnable_get
 * Description:
 *      Get 802.1x port-based enable configuration
 * Input:
 *      port                  -   Port id
 * Output:
 *      pEnable             -   the pointer of enable or disable
 * Return:
 *      RT_ERR_OK                -  Success
 *      RT_ERR_FAILED          -   Failure
 *      RT_ERR_PORT_ID        -   Invalid port id
 * Note:
 *      The API can update the port-based port enable register content. If a port is 802.1x 
 *      port based network access control "enabled", it should be authenticated so packets 
 *      from that port won't be dropped or trapped to CPU. 
 *      The status of 802.1x port-based network access control is as following:
 *          - DISABLED
 *          - ENABLED
 */    
rtk_api_ret_t rtk_dot1x_portBasedEnable_get(rtk_port_t port, rtk_enable_t *pEnable)
{
    rtk_api_ret_t retVal;
    uint32 enDot1x;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    

    if(NULL == pEnable)
        return RT_ERR_NULL_POINTER;

    retVal = rtl8309n_dot1x_portBasedEnable_get(port, &enDot1x);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;
    
    *pEnable =(RTL8309N_ENABLED == enDot1x) ? ENABLED : DISABLED;
            
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_dot1x_portBasedAuthStatus_set
 * Description:
 *      Set 802.1x port-based enable configuration
 * Input:
 *      port                  -   Port id
 *      port_auth          -  The status of 802.1x port
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                                    -  Success
 *      RT_ERR_FAILED                              -   Failure
 *      RT_ERR_PORT_ID                            -   Invalid port id
 *      RT_ERR_DOT1X_PORTBASEDAUTH      -   Port-based auth port error
 * Note:
 *      The authenticated status of 802.1x port-based network access control is as following:
 *          -	UNAUTH
 *          -	AUTH
 */    
rtk_api_ret_t rtk_dot1x_portBasedAuthStatus_set(rtk_port_t port, rtk_dot1x_auth_status_t port_auth)
{
    rtk_api_ret_t retVal;
    uint32 isAuth, direction;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    

    if(port_auth > AUTH_STATUS_END)
        return RT_ERR_DOT1X_PORTBASEDAUTH;

    retVal = rtl8309n_dot1x_portBased_get(port, &isAuth, &direction);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;

	isAuth = (AUTH == port_auth) ? RTL8309N_PORT_AUTH : RTL8309N_PORT_UNAUTH;
    retVal = rtl8309n_dot1x_portBased_set(port, isAuth, direction);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_dot1x_portBasedAuthStatus_set
 * Description:
 *      Set 802.1x port-based enable configuration
 * Input:
 *      port                  -   Port id
 *      port_auth          -  The status of 802.1x port
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                                    -  Success
 *      RT_ERR_FAILED                              -   Failure
 *      RT_ERR_PORT_ID                            -   Invalid port id
 *      RT_ERR_DOT1X_PORTBASEDAUTH      -   Port-based auth port error
 * Note:
 *      The authenticated status of 802.1x port-based network access control is as following:
 *          -	UNAUTH
 *          -	AUTH
 */   
rtk_api_ret_t rtk_dot1x_portBasedAuthStatus_get(rtk_port_t port, rtk_dot1x_auth_status_t *pPort_auth)
{
    rtk_api_ret_t retVal;
    uint32 isAuth, direction;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    
    if (NULL == pPort_auth)
        return RT_ERR_NULL_POINTER;

    retVal = rtl8309n_dot1x_portBased_get(port, &isAuth, &direction);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;
    *pPort_auth = (RTL8309N_PORT_AUTH == isAuth) ? AUTH: UNAUTH;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_dot1x_portBasedDirection_set
 * Description:
 *      Set 802.1x port-based operational direction configuration
 * Input:
 *      port                -   Port id
 *      port_direction    	-   Operation direction
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                                 -  Success
 *      RT_ERR_FAILED                           -  Failure
 *      RT_ERR_PORT_ID                         -  Invalid port id
 *      RT_ERR_DOT1X_PORTBASEDOPDIR  -  Port-based opdir error
 * Note:
 *      The operate controlled direction of 802.1x port-based network access control is as following:
 *          -	BOTH 
 *          -	IN
 */   
rtk_api_ret_t rtk_dot1x_portBasedDirection_set(rtk_port_t port, rtk_dot1x_direction_t port_direction)
{
    rtk_api_ret_t retVal;
    uint32 isAuth, direction;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    
    
    if(port_direction >= DIRECTION_END)
        return RT_ERR_DOT1X_PORTBASEDOPDIR;

    retVal = rtl8309n_dot1x_portBased_get(port, &isAuth, &direction);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;
    direction = (BOTH == port_direction) ? RTL8309N_PORT_BOTHDIR : RTL8309N_PORT_INDIR;
    retVal = rtl8309n_dot1x_portBased_set(port, isAuth, direction);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;
    
    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_dot1x_portBasedDirection_get
 * Description:
 *      Get 802.1x port-based operational direction configuration
 * Input:
 *      port                  -   Port id
 * Output:
 *      pPort_direction    -   the pointer of Operation direction
 * Return:
 *      RT_ERR_OK                         -  Success
 *      RT_ERR_FAILED                    -  Failure
 *      RT_ERR_PORT_ID                  -  Invalid port id
 *      RT_ERR_NULL_POINTER          -  Input parameter is null pointer
 * Note:
 *      The operate controlled direction of 802.1x port-based network access control is as following:
 *          -	BOTH
 *          -	IN
 */   
rtk_api_ret_t rtk_dot1x_portBasedDirection_get(rtk_port_t port, rtk_dot1x_direction_t *pPort_direction)
{
    rtk_api_ret_t retVal;
    uint32 isAuth, direction;

    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    
    if(NULL == pPort_direction)
        return RT_ERR_NULL_POINTER;
    
    retVal = rtl8309n_dot1x_portBased_get(port, &isAuth, &direction);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;
	
    *pPort_direction = (RTL8309N_PORT_BOTHDIR == direction) ? BOTH : IN;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_dot1x_macBasedEnable_set
 * Description:
 *      Set 802.1x mac-based port enable configuration
 * Input:
 *      port                  -   Port id
 *      enable                -   The status of 802.1x mac-base funtion
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                         -  Success
 *      RT_ERR_FAILED                    -  Failure
 *      RT_ERR_PORT_ID                  -  Invalid port id
 * Note:
 *     If a port is 802.1x MAC based network access control "enabled", the incoming packets should 
 *     be authenticated so packets from that port won't be dropped or trapped to CPU.
 */    
rtk_api_ret_t rtk_dot1x_macBasedEnable_set(rtk_port_t port, rtk_enable_t enabled)
{
    rtk_api_ret_t retVal;
    uint32 enMacBase;
    
    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    
    
	enMacBase = (ENABLED == enabled) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
    retVal = rtl8309n_dot1x_macBasedEnable_set(port, enMacBase);
    if (retVal != SUCCESS)
        return RT_ERR_FAILED;
       
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_dot1x_macBasedEnable_get
 * Description:
 *      Get 802.1x mac-based port enable configuration
 * Input:
 *      port                -   Port id
 * Output:
 *      pEnable             -   the pointer of the status of 802.1x mac-base funtion
 * Return:
 *      RT_ERR_OK               -  Success
 *      RT_ERR_FAILED         -   Failure
 *		RT_ERR_PORT_ID		-	invalid port id
 *		RT_ERR_NULL_POINTER		-	input parameter is null pointer
 * Note:
 *     If a port is 802.1x MAC based network access control "enabled", the incoming packets should 
 *     be authenticated so packets from that port won't be dropped or trapped to CPU.
 */    
rtk_api_ret_t rtk_dot1x_macBasedEnable_get(rtk_port_t port, rtk_enable_t *pEnable)
{
    rtk_api_ret_t retVal;
    uint32 enMacBase;
    
    if(port > RTK_PORT_ID_MAX) 
        return RT_ERR_PORT_ID;    

    if(NULL == pEnable)
        return RT_ERR_NULL_POINTER;
    
    retVal = rtl8309n_dot1x_macBasedEnable_get(port, &enMacBase); 
    if (retVal != SUCCESS)
        return RT_ERR_FAILED;

    *pEnable = (RTL8309N_ENABLED == enMacBase) ? ENABLED: DISABLED;
    
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_dot1x_macBasedDirection_set
 * Description:
 *      Set 802.1x mac-based operational direction configuration
 * Input:
 *      mac_direction    -   Operation direction
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                                    -  Success
 *      RT_ERR_FAILED                              -  Failure
 *      RT_ERR_DOT1X_MACBASEDOPDIR      -  MAC-based opdir error
 * Note:
 *      The operate controlled direction of 802.1x mac-based network access control is as following:
 *          -	BOTH
 *          -	IN
 */    
rtk_api_ret_t rtk_dot1x_macBasedDirection_set(rtk_dot1x_direction_t mac_direction)
{
    rtk_api_ret_t retVal;
	rtk_dot1x_direction_t direction;
        
    if(mac_direction > DIRECTION_END) 
        return RT_ERR_DOT1X_MACBASEDOPDIR;    

	direction = (BOTH == mac_direction) ? RTL8309N_PORT_BOTHDIR : RTL8309N_PORT_INDIR;
    retVal = rtl8309n_dot1x_macBased_set(direction);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;
    
    return RT_ERR_OK;

}
    
/* Function Name:
 *      rtk_dot1x_macBasedDirection_get
 * Description:
 *      Get 802.1x mac-based operational direction configuration
 * Input:
 *      none
 * Output:
 *      pMac_direction    -   the pointer of Operation direction
 * Return:
 *      RT_ERR_OK                                    -  Success
 *      RT_ERR_FAILED                              -  Failure
 *      RT_ERR_NULL_POINTER                    -  Input parameter is null pointer
 *		RT_ERR_PORT_ID				-	Wrong port ID
 * Note:
 *      The operate controlled direction of 802.1x mac-based network access control is as following:
 *          -	BOTH
 *          -	IN
 */
rtk_api_ret_t rtk_dot1x_macBasedDirection_get(rtk_dot1x_direction_t *pMac_direction)
{
    rtk_api_ret_t retVal;
	rtk_dot1x_direction_t direction;

	if(NULL == pMac_direction)
        return RT_ERR_NULL_POINTER;
	
    retVal = rtl8309n_dot1x_macBased_get(&direction);
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;
    
    *pMac_direction = (RTL8309N_MAC_BOTHDIR == direction) ? BOTH : IN;
    if(retVal != SUCCESS)
        return RT_ERR_FAILED;
    
    return RT_ERR_OK;

}


/* Function Name:
 *      rtk_dot1x_macBasedAuthMac_add
 * Description:
 *      Add an authenticated MAC to ASIC
 * Input:
 *      pAuth_mac       - The authenticated MAC
 *      fid             - no use for RTL8309N   
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                                       - Success
 *      RT_ERR_FAILED                                 -  Failure
 *      RT_ERR_L2_ENTRY_NOTFOUND             -  Specified entry not found
 *	  RT_ERR_L2_FID						-	invalid	fid
 *	  RT_ERR_MAC							-	invalid mac address
 *      RT_ERR_DOT1X_MAC_PORT_MISMATCH 		 - Auth MAC and port mismatch eror 
 *	  RT_ERR_PORT_ID						-	invalid port id
 * Note:
 *     The API can add a 802.1x authorised MAC address to port. If the MAC does not exist in LUT, 
 *     user can't add this MAC with authorised status.
 */    
rtk_api_ret_t rtk_dot1x_macBasedAuthMac_add(rtk_mac_t *pAuth_mac, rtk_fid_t fid)
{
    int32 i;
    uint32 index,entryAddr;
    uint32 isHit;
    
	rtl8309n_l2_ucastEntry_t rtl8309n_l2_data;
	rtk_api_ret_t retVal;
      

    if(NULL == pAuth_mac)
        return RT_ERR_NULL_POINTER;

    if(pAuth_mac->octet[0] & 0x1)
        return RT_ERR_MAC;

	if(fid > RTK_MAX_FID)
		return RT_ERR_L2_FID;

    rtl8309n_l2_MacToIdx_get(pAuth_mac->octet, fid, &index);   
    
    isHit = FALSE;
    for (i = 3; i >= 0; i--) 
    {
        entryAddr = (i << 9) | index;
        if (rtl8309n_l2_ucastEntry_get(entryAddr, &rtl8309n_l2_data) != SUCCESS) 
        {
            return RT_ERR_FAILED;
        }
        else if ((pAuth_mac->octet[0] == rtl8309n_l2_data.mac.octet[0]) && (pAuth_mac->octet[1] == rtl8309n_l2_data.mac.octet[1]) && 
                   (pAuth_mac->octet[2] == rtl8309n_l2_data.mac.octet[2]) && (pAuth_mac->octet[3] == rtl8309n_l2_data.mac.octet[3]) &&
                   (pAuth_mac->octet[4] == rtl8309n_l2_data.mac.octet[4]) && (pAuth_mac->octet[5] == rtl8309n_l2_data.mac.octet[5])) 
        {

			rtl8309n_l2_data.auth = AUTH;
			retVal  = rtl8309n_l2_ucastEntry_set(entryAddr, &rtl8309n_l2_data);
			if(retVal != SUCCESS)
            	return RT_ERR_FAILED;

            isHit = TRUE;
			break;
        }
    }

    if(!isHit)
        return RT_ERR_L2_ENTRY_NOTFOUND;
    
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_dot1x_macBasedAuthMac_del
 * Description:
 *      Delete an authenticated MAC to ASIC
 * Input:
 *      pAuth_mac       - The authenticated MAC
 *      fid             -  no use for RTL8309N   
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK                                       - Success
 *      RT_ERR_FAILED                                 -  Failure
 *      RT_ERR_L2_ENTRY_NOTFOUND             -  Specified entry not found
 *      RT_ERR_DOT1X_MAC_PORT_MISMATCH  - Auth MAC and port mismatch eror 
 *	  RT_ERR_L2_FID		-	invalid fid 
 * Note:
 *     The API can delete a 802.1x authenticated MAC address to port. It only change the auth status of
 *     the MAC and won't delete it from LUT.
 */
rtk_api_ret_t rtk_dot1x_macBasedAuthMac_del(rtk_mac_t *pAuth_mac, rtk_fid_t fid)
{
    int32 i;
    uint8 macAddr[6];
    uint32 index,entryAddr;
    uint32 isHit;
	rtl8309n_l2_ucastEntry_t rtl8309n_l2_data;
	     
    if(NULL == pAuth_mac)
        return RT_ERR_NULL_POINTER;
    if(pAuth_mac->octet[0] & 0x1)
        return RT_ERR_DOT1X_MAC_PORT_MISMATCH;
	if (fid > RTK_MAX_FID)
		return RT_ERR_L2_FID;

    rtl8309n_l2_MacToIdx_get(pAuth_mac->octet, fid, &index);   
    
    isHit = FALSE;
    for (i = 3; i >= 0; i--) 
    {
        entryAddr = (i << 9) | index;
        if (rtl8309n_l2_ucastEntry_get(entryAddr, &rtl8309n_l2_data) != SUCCESS) 
        {
            return RT_ERR_FAILED;
        }
        else if ((pAuth_mac->octet[0] == macAddr[0]) && (pAuth_mac->octet[1] == macAddr[1]) && 
                   (pAuth_mac->octet[2] == macAddr[2]) && (pAuth_mac->octet[3] == macAddr[3]) &&
                   (pAuth_mac->octet[4] == macAddr[4]) && (pAuth_mac->octet[5] == macAddr[5])) 
        {
			rtl8309n_l2_data.auth = UNAUTH;
            if (rtl8309n_l2_ucastEntry_set(entryAddr, &rtl8309n_l2_data) != SUCCESS)
                return RT_ERR_FAILED;
            isHit = TRUE;
            break;
        }
    }

    if(!isHit)
        return RT_ERR_L2_ENTRY_NOTFOUND;
    
    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_filter_igrAcl_init
 * Description:
 *      Initialize ACL 
 * Input:
 *      none
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK		-	success
 *      RT_ERR_FAILED	-	failure
 * Note:
 *      The API init ACL module.
 */
rtk_api_ret_t rtk_filter_igrAcl_init(void)
{
    uint32 entAddr;
	rtl8309n_acl_entry_t rtl8309n_aclEntry;
	
    /*empty the acl table*/
	rtl8309n_aclEntry.action = RTL8309N_ACT_DROP;
	rtl8309n_aclEntry.data = 0;
	rtl8309n_aclEntry.phyPort = RTL8309N_ACL_INVALID;
	rtl8309n_aclEntry.priority = 0;
	rtl8309n_aclEntry.protocol = RTL8309N_ACL_ETHER;
    for (entAddr = 0; entAddr < RTL8309N_ACL_ENTRYNUM; entAddr++ ) 
    {
        if (rtl8309n_acl_entry_set(entAddr, &rtl8309n_aclEntry) != SUCCESS)
            return RT_ERR_FAILED;
    }
    
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_filter_igrAcl_rule_add
 * Description:
 *      Add an acl rule into acl table
 * Input:
 *      pRule    -  the pointer of rule structure
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK	-	success
 *      RT_ERR_FAILED	-	failure
 *      RT_ERR_TBL_FULL		-	input table full  	
 *      RT_ERR_NULL_POINTER		-	input parameter is null pointer
 * Note:
 *      (1)The API add an  ACL rule. 
 *          phyport could be port 0~8, RTK_ACL_INVALID_PORT, RTK_ACL_ANYPORT;
 *      (2)protocol could be :
 *           -	ACL_PRO_ETHER
 *           -	ACL_PRO_TCP
 *           -	ACL_PRO_UDP
 *           -	ACL_PRO_TCPUDP
 *      (3)prority could be 0-3;
 *      (4)action could be :
 *           -	ACL_ACT_DROP
 *           -	ACL_ACT_PERMIT
 *           -	ACL_ACT_TRAP2CPU
 *           -	ACL_ACT_MIRROR
 */
rtk_api_ret_t rtk_filter_igrAcl_rule_add(rtk_filter_rule_t *pRule)
{
    uint32 entryAddr;
    uint32 isFull ;
	rtl8309n_acl_entry_t rtl8309n_aclEntry, rtl8309n_aclEntry_r;
	
    if(NULL == pRule)
        return RT_ERR_NULL_POINTER;
	if ((pRule->action >= ACL_ACT_END) || (pRule->phyport > RTK_ACL_ANYPORT) || (pRule->priority > 3) ||
		(pRule->protocol >= ACL_PRO_END))
		return RT_ERR_INPUT;
	
	rtl8309n_aclEntry.phyPort = pRule->phyport;
	rtl8309n_aclEntry.data = pRule->data;
	switch(pRule->protocol)
	{
		case ACL_PRO_ETHERTYPE:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_ETHER;
			break;

		case ACL_PRO_TCP:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_TCP;
			break;
			
		case ACL_PRO_UDP:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_UDP;
			break;
			
		case ACL_PRO_TCPUDP:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_TCPUDP;
			break;
		default:
			break;
	}
	rtl8309n_aclEntry.priority = pRule->priority;	
	switch(pRule->action)
	{
		case ACL_ACT_DROP:
			rtl8309n_aclEntry.action = RTL8309N_ACT_DROP;
			break;

		case ACL_ACT_PERMIT:
			rtl8309n_aclEntry.action = RTL8309N_ACT_PERMIT;
			break;	

		case ACL_ACT_TRAP2CPU:
			rtl8309n_aclEntry.action = RTL8309N_ACT_TRAP2CPU;
			break;

		case ACL_ACT_MIRROR: 
			rtl8309n_aclEntry.action = RTL8309N_ACT_MIRROR;
			break;	
		default:
			break;
	}
    /*if  exist an  acl entry of the same rule according by phyport, protocol, data,
     *only update priority and action
     */
    for (entryAddr = 0; entryAddr < RTL8309N_ACL_ENTRYNUM; entryAddr ++ ) 
    {
        if (rtl8309n_acl_entry_get(entryAddr, &rtl8309n_aclEntry_r) != SUCCESS)
            return RT_ERR_FAILED;

        if ((rtl8309n_aclEntry.phyPort == rtl8309n_aclEntry_r.phyPort) && (rtl8309n_aclEntry.protocol == rtl8309n_aclEntry_r.protocol) && (rtl8309n_aclEntry.data == rtl8309n_aclEntry_r.data)) 
        {
        	rtl8309n_aclEntry_r.priority = rtl8309n_aclEntry.priority;
			rtl8309n_aclEntry_r.action = rtl8309n_aclEntry.action;
            if (rtl8309n_acl_entry_set(entryAddr, &rtl8309n_aclEntry_r) != SUCCESS )
                 return RT_ERR_FAILED;
            else
                 return RT_ERR_OK;
        }            
    }    
    /*if not exist the rule, find an invalid entry to write it , else return table full */
    isFull = TRUE;
    for (entryAddr = 0; entryAddr < RTL8309N_ACL_ENTRYNUM; entryAddr ++ )
    {
        if (rtl8309n_acl_entry_get(entryAddr, &rtl8309n_aclEntry_r) != SUCCESS)
            return RT_ERR_FAILED;
        
        if (rtl8309n_aclEntry_r.phyPort == RTL8309N_ACL_INVALID) 
        {
        	rtl8309n_aclEntry_r.action = rtl8309n_aclEntry.action;
			rtl8309n_aclEntry_r.data = rtl8309n_aclEntry.data;
			rtl8309n_aclEntry_r.phyPort = rtl8309n_aclEntry.phyPort;
			rtl8309n_aclEntry_r.priority = rtl8309n_aclEntry.priority;
			rtl8309n_aclEntry_r.protocol = rtl8309n_aclEntry.protocol;
            if (rtl8309n_acl_entry_set(entryAddr, &rtl8309n_aclEntry_r) != SUCCESS)
                 return RT_ERR_FAILED;
            else 
            {
                isFull = FALSE;
                break;
            }                        
        }            
    }    
    if (isFull)
    {
        return RT_ERR_TBL_FULL;
    }
    else
        return SUCCESS;
}


/* Function Name:
 *      rtk_filter_igrAcl_rule_get
 * Description:
 *      Get ACL rule priority and action 
 * Input:
 *      pRule    -  the pointer of rule structure
 * Output:
 *      pRule    -  the pointer of rule structure
 * Return:
 *      RT_ERR_OK			-	success
 *      RT_ERR_FAILED			-	failure
 *		RT_ERR_NULL_POINTER		-	input parameter is null pointer
 * Note:
 *      (1)The API add an  ACL rule. 
 *          phyport could be port 0~8, RTK_ACL_INVALID_PORT and  RTK_ACL_ANYPORT;
 *      (2)protocol could be :
 *           -	ACL_PRO_ETHER
 *           -	ACL_PRO_TCP
 *           -	ACL_PRO_UDP
 *           -	ACL_PRO_TCPUDP
 *      (3)prority could be 0-3;
 *      (4)action could be :
 *           -	ACL_ACT_DROP
 *           -	ACL_ACT_PERMIT
 *           -	ACL_ACT_TRAP2CPU
 *           -	ACL_ACT_MIRROR
 */
rtk_api_ret_t rtk_filter_igrAcl_rule_get(rtk_filter_rule_t *pRule)
{
    uint32 entryAddr;
	rtl8309n_acl_entry_t rtl8309n_aclEntry, rtl8309n_aclEntry_r;

    if(NULL == pRule)
        return RT_ERR_NULL_POINTER;
	if ((pRule->phyport > RTK_PORT_ID_MAX) || (pRule->protocol >= ACL_PRO_END))
		return RT_ERR_INPUT;
	
	rtl8309n_aclEntry.phyPort = pRule->phyport;
	rtl8309n_aclEntry.data = pRule->data;
	switch(pRule->protocol)
	{
		case ACL_PRO_ETHERTYPE:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_ETHER;
			break;

		case ACL_PRO_TCP:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_TCP;
			break;
			
		case ACL_PRO_UDP:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_UDP;
			break;
			
		case ACL_PRO_TCPUDP:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_TCPUDP;
			break;
		default:
			break;
	}
	rtl8309n_aclEntry.priority = 0;
	rtl8309n_aclEntry.action = 0;

    /*if  exist an  acl entry of the same rule according by phyport, protocol,data,
     *get the priority and action
     */
    for (entryAddr = 0; entryAddr < RTL8309N_ACL_ENTRYNUM; entryAddr ++ ) 
    {
        if (rtl8309n_acl_entry_get(entryAddr, &rtl8309n_aclEntry_r) != SUCCESS)
            return RT_ERR_FAILED;

        if ((rtl8309n_aclEntry_r.phyPort == rtl8309n_aclEntry.phyPort) && (rtl8309n_aclEntry_r.protocol == rtl8309n_aclEntry.protocol) && (rtl8309n_aclEntry_r.data == rtl8309n_aclEntry.data)) 
        {
                 rtl8309n_aclEntry.priority = rtl8309n_aclEntry_r.priority;
                 rtl8309n_aclEntry.action = rtl8309n_aclEntry_r.action;
				 pRule->priority = rtl8309n_aclEntry.priority;
				 switch(rtl8309n_aclEntry.action)
				 {
				 	case RTL8309N_ACT_DROP:
						pRule->action = ACL_ACT_DROP;
						break;
						
					case RTL8309N_ACT_PERMIT:
						pRule->action = ACL_ACT_PERMIT;
						break;
						
					case RTL8309N_ACT_TRAP2CPU:
						pRule->action = ACL_ACT_TRAP2CPU;
						break;
						
					case RTL8309N_ACT_MIRROR:
						pRule->action = ACL_ACT_MIRROR;
						break;
					default:
						break;
						
				 }
				 
                 return RT_ERR_OK;
        }            
    }    

    return RT_ERR_FAILED;
}

/* Function Name:
 *      rtk_filter_igrAcl_rule_del
 * Description:
 *      Delete an acl rule from acl table
 * Input:
 *      pRule    -  the pointer of rule structure
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK				-	success
 *      RT_ERR_FAILED			-	failure
 *      RT_ERR_INPUT			-	invalid input parameter
 *      RT_ERR_NULL_POINTER		-	input parameter is null pointer
 * Note:
 *      (1)The API delet an ACL rule. 
 *          phyport could be port 0~8, RTK_ACL_INVALID_PORT and  RTK_ACL_ANYPORT;
 *      (2)protocol could be :
 *           -	ACL_PRO_ETHER
 *           -	ACL_PRO_TCP
 *           -	ACL_PRO_UDP
 *           -	ACL_PRO_TCPUDP
 *      (3)prority could be 0-3;
 *      (4)action could be :
 *           -	ACL_ACT_DROP
 *           -	ACL_ACT_PERMIT
 *           -	ACL_ACT_TRAP2CPU
 *           -	ACL_ACT_MIRROR
 */
rtk_api_ret_t rtk_filter_igrAcl_rule_del(rtk_filter_rule_t *pRule)
{
    uint32 entaddr;
    uint32 isHit;
	rtl8309n_acl_entry_t rtl8309n_aclEntry, rtl8309n_aclEntry_r;

    if(NULL == pRule)
        return RT_ERR_NULL_POINTER;
	if ((pRule->action >= ACL_ACT_END) || (pRule->phyport > RTK_PORT_ID_MAX) || (pRule->priority > 3) ||
		(pRule->protocol >= ACL_PRO_END))
		return RT_ERR_INPUT;
	
	rtl8309n_aclEntry.phyPort = pRule->phyport;
	rtl8309n_aclEntry.data = pRule->data;
	switch(pRule->protocol)
	{
		case ACL_PRO_ETHERTYPE:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_ETHER;
			break;

		case ACL_PRO_TCP:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_TCP;
			break;
			
		case ACL_PRO_UDP:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_UDP;
			break;
			
		case ACL_PRO_TCPUDP:
			rtl8309n_aclEntry.protocol = RTL8309N_ACL_TCPUDP;
			break;
		default:
			break;
	}
	rtl8309n_aclEntry.priority = 0;
	rtl8309n_aclEntry.action = RTL8309N_ACT_DROP;

    isHit = FALSE;
    
    if (rtl8309n_aclEntry.phyPort == RTL8309N_ACL_INVALID)
        return RT_ERR_INPUT;
    
    for (entaddr = 0; entaddr < RTL8309N_ACL_ENTRYNUM; entaddr ++ ) 
    {
        if (rtl8309n_acl_entry_get(entaddr, &rtl8309n_aclEntry_r) != SUCCESS)
            return RT_ERR_FAILED;
        
        if ((rtl8309n_aclEntry_r.phyPort == rtl8309n_aclEntry.phyPort) && (rtl8309n_aclEntry_r.protocol == rtl8309n_aclEntry.protocol) && (rtl8309n_aclEntry_r.data == rtl8309n_aclEntry.data))
        {
        	rtl8309n_aclEntry_r.action = RTL8309N_ACT_DROP;
			rtl8309n_aclEntry_r.data = 0;
			rtl8309n_aclEntry_r.phyPort = RTL8309N_ACL_INVALID;
			rtl8309n_aclEntry_r.priority = 0;
			rtl8309n_aclEntry_r.protocol = RTL8309N_ACL_ETHER;
            if (rtl8309n_acl_entry_set(entaddr, &rtl8309n_aclEntry_r) != SUCCESS)
                return RT_ERR_FAILED;
            isHit = TRUE;
            break;
        }                
    }

    if (isHit)
        return RT_ERR_OK;
    else
        return RT_ERR_INPUT;
    
}


/* Function Name:
 *		rtk_storm_filterEnable_set
 * Description:
 *		Enable storm filter
 * Input:
 *		port			-	port id
 *		storm_type	-	storm filter type
 *		enabled		-	enable or disable
 * Output:
 *		none
 * Return:
 *		RT_ERR_FAILED		-	failure
 *		RT_ERR_OK			-	success
 *		RT_ERR_PORT_ID		-	invalid port id
 *		RT_ERR_INPUT		-	invalid input parameter
 * Note:
 */
rtk_api_ret_t rtk_storm_filterEnable_set(rtk_port_t port, rtk_rate_storm_group_t storm_type, rtk_enable_t enabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_stormType, rtl8309n_enabled;
	
	if(port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
	if(storm_type >= STORM_GROUP_END)
		return RT_ERR_INPUT;

	switch(storm_type)
	{
		case STORM_GROUP_UNKNOWN_UNICAST: 
			rtl8309n_stormType = RTL8309N_UNKOWN_UNIDAPKT;
			break;
			
		case STORM_GROUP_UNKNOWN_MULTICAST:
			rtl8309n_stormType = RTL8309N_UNKOWN_MULTDAPKT;
			break;

		case STORM_GROUP_MULTICAST:
			rtl8309n_stormType = RTL8309N_MULTICASTPKT;
			break;

		case STORM_GROUP_BROADCAST:
			rtl8309n_stormType = RTL8309N_BROADCASTPKT;
			break;

		default:
			break;

	}
	rtl8309n_enabled = ((ENABLED == enabled) ? RTL8309N_ENABLED : RTL8309N_DISABLED);
	retVal = rtl8309n_storm_filterEnable_set(port, rtl8309n_stormType, rtl8309n_enabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	
	return RT_ERR_OK;
}

/* Function Name:
 *		rtk_storm_filterEnable_get
 * Description:
 *		Enable storm filter
 * Input:
 *		port			-	port id
 *		storm_type	-	storm filter type
 * Output:
 *		pEnabled		-	enable or disable
 * Return:
 *		RT_ERR_FAILED		-	failure
 *		RT_ERR_OK			-	success
 *		RT_ERR_PORT_ID		-	invalid port id
 *		RT_ERR_INPUT		-	invalid input parameter
 * Note:
 */
rtk_api_ret_t rtk_storm_filterEnable_get(rtk_port_t port, rtk_rate_storm_group_t storm_type, rtk_enable_t *pEnabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_stormType, rtl8309n_enabled;
	
	if(port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
	if(storm_type >= STORM_GROUP_END)
		return RT_ERR_INPUT;
	if(NULL == pEnabled)
		return RT_ERR_NULL_POINTER;

	switch(storm_type)
	{
		case STORM_GROUP_UNKNOWN_UNICAST: 
			rtl8309n_stormType = RTL8309N_UNKOWN_UNIDAPKT;
			break;
			
		case STORM_GROUP_UNKNOWN_MULTICAST:
			rtl8309n_stormType = RTL8309N_UNKOWN_MULTDAPKT;
			break;

		case STORM_GROUP_MULTICAST:
			rtl8309n_stormType = RTL8309N_MULTICASTPKT;
			break;

		case STORM_GROUP_BROADCAST:
			rtl8309n_stormType = RTL8309N_BROADCASTPKT;
			break;

		default:
			break;

	}
	retVal = rtl8309n_storm_filterEnable_get(port, storm_type, &rtl8309n_enabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	*pEnabled = ((RTL8309N_ENABLED == rtl8309n_enabled) ? ENABLED : DISABLED); 

	
	return RT_ERR_OK;	
}

/* Function Name:
 *		rtk_storm_filterAttr_set
 * Description:
 *		Set storm filter attributes
 * Input:
 *		port	-	port id(0 - 8)
 *		storm_type	-	storm filter type
 *		pStorm_data	-	storm filter data
 * Output:
 *		none
 * Return:
 *		RT_ERR_PORT_ID		-	invalid port id
 *		RT_ERR_NULL_POINTER		-	input parameter is null pointer
 *		RT_ERR_FAILED		-	failure
 *		RT_ERR_OK		-	success
 * Note:
 */
rtk_api_ret_t rtk_storm_filterAttr_set(rtk_port_t port, rtk_rate_storm_group_t storm_type, rtk_storm_attr_t *pStorm_data)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_stormType;
	rtl8309n_storm_attr_t rtl8309n_storm_data;
	
	if(port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
	if(storm_type >= STORM_GROUP_END)
		return RT_ERR_INPUT;
	if(NULL == pStorm_data)
		return RT_ERR_NULL_POINTER;
	
	switch(storm_type)
	{
		case STORM_GROUP_UNKNOWN_UNICAST: 
			rtl8309n_stormType = RTL8309N_UNKOWN_UNIDAPKT;
			break;
			
		case STORM_GROUP_UNKNOWN_MULTICAST:
			rtl8309n_stormType = RTL8309N_UNKOWN_MULTDAPKT;
			break;

		case STORM_GROUP_MULTICAST:
			rtl8309n_stormType = RTL8309N_MULTICASTPKT;
			break;

		case STORM_GROUP_BROADCAST:
			rtl8309n_stormType = RTL8309N_BROADCASTPKT;
			break;

		default:
			break;
	}
	rtl8309n_storm_data.unit = pStorm_data->unit;
	rtl8309n_storm_data.rate = pStorm_data->rate;
	rtl8309n_storm_data.burst = pStorm_data->burst;
	rtl8309n_storm_data.ifg_include = (ENABLED == pStorm_data->ifg_include) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
	retVal = rtl8309n_storm_filterAttr_set(port, rtl8309n_stormType, &rtl8309n_storm_data);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;

	return RT_ERR_OK;
}

/* Function Name:
 *		rtk_storm_filterAttr_get
 * Description:
 *		Get storm filter attributes
 * Input:
 *		port	-	port id(0 - 8)
 *		storm_type	-	storm filter type
 * Output:
 *		pStorm_data	-	pointer point to structure describing storm filter data
 * Return:
 *		RT_ERR_PORT_ID		-	invalid port id
 *		RT_ERR_NULL_POINTER		-	input parameter is null pointer
 *		RT_ERR_INPUT			-	invalid input parameter
 *		RT_ERR_FAILED		-	failure
 *		RT_ERR_OK		-	success
 */
rtk_api_ret_t rtk_storm_filterAttr_get(rtk_port_t port, rtk_rate_storm_group_t storm_type, rtk_storm_attr_t *pStorm_data)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_stormType;
	rtl8309n_storm_attr_t rtl8309n_storm_data;


	if(port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
	if(storm_type >= STORM_GROUP_END)
		return RT_ERR_INPUT;
	if(NULL == pStorm_data)
		return RT_ERR_NULL_POINTER;

	switch(storm_type)
	{
		case STORM_GROUP_UNKNOWN_UNICAST: 
			rtl8309n_stormType = RTL8309N_UNKOWN_UNIDAPKT;
			break;
			
		case STORM_GROUP_UNKNOWN_MULTICAST:
			rtl8309n_stormType = RTL8309N_UNKOWN_MULTDAPKT;
			break;

		case STORM_GROUP_MULTICAST:
			rtl8309n_stormType = RTL8309N_MULTICASTPKT;
			break;

		case STORM_GROUP_BROADCAST:
			rtl8309n_stormType = RTL8309N_BROADCASTPKT;
			break;

		default:
			break;
	}
	retVal = rtl8309n_storm_filterAttr_get(port, rtl8309n_stormType, &rtl8309n_storm_data);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;

	pStorm_data->unit = rtl8309n_storm_data.unit;
	pStorm_data->rate = rtl8309n_storm_data.rate;
	pStorm_data->burst = rtl8309n_storm_data.burst;
	pStorm_data->ifg_include = (RTL8309N_ENABLED == rtl8309n_storm_data.ifg_include) ? ENABLED : DISABLED;
	
	return RT_ERR_OK;
}

/* Function Name:
 *		rtk_storm_filterStatus_set
 * Description:
 *		Clearing storm filter flag
 * Input:
 *		port			-	port id
 *		storm_type	-	storm filter type
 *		enabled		-	enable or disable
 * Output:
 *		none
 * Return:
 *		RT_ERR_PORT_ID		-	invalid port id
 *		RT_ERR_INPUT		-	invalid input parameter
 *		RT_ERR_FAILED		-	failure
 *		RT_ERR_OK			-	success
 * Note:
 */
rtk_api_ret_t rtk_storm_filterStatus_set(rtk_port_t port, rtk_rate_storm_group_t storm_type, rtk_enable_t enabled)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_stormType;
	uint32 rtl8309n_enabled;
	
	if(port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
	if(storm_type >= STORM_GROUP_END)
		return RT_ERR_INPUT;

	switch(storm_type)
	{
		case STORM_GROUP_UNKNOWN_UNICAST: 
			rtl8309n_stormType = RTL8309N_UNKOWN_UNIDAPKT;
			break;
			
		case STORM_GROUP_UNKNOWN_MULTICAST:
			rtl8309n_stormType = RTL8309N_UNKOWN_MULTDAPKT;
			break;

		case STORM_GROUP_MULTICAST:
			rtl8309n_stormType = RTL8309N_MULTICASTPKT;
			break;

		case STORM_GROUP_BROADCAST:
			rtl8309n_stormType = RTL8309N_BROADCASTPKT;
			break;

		default:
			break;

	}

	rtl8309n_enabled = (ENABLED == enabled) ? RTL8309N_ENABLED : RTL8309N_DISABLED;
	retVal = rtl8309n_storm_filterStatus_set(port, rtl8309n_stormType, rtl8309n_enabled);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;

	return RT_ERR_OK;
}

/* Function Name:
 *		rtk_storm_filterStatus_get
 * Description:
 *		Get storm filter flag status
 * Input:
 *		port	-	port id
 *		storm_type	-	storm filter type
 * Output:
 *		pExceed	-	exceed storm filter, exceed or not
 * Return:
 *		RT_ERR_PORT_ID		-	invalid port id
 *		RT_ERR_INPUT		-	invalid input parameter
 *		RT_ERR_NULL_POINTER		-	input parameter is null pointer
 *		RT_ERR_FAILED		-	failure
 *		RT_ERR_OK			-	success
 */
rtk_api_ret_t rtk_storm_filterStatus_get(rtk_port_t port, rtk_rate_storm_group_t storm_type, uint32 *pExceed)
{
	rtk_api_ret_t retVal;
	uint32 rtl8309n_stormType;
	
	if (port > RTK_PORT_ID_MAX)
		return RT_ERR_PORT_ID;
	if (storm_type >= STORM_GROUP_END)
		return RT_ERR_INPUT;
    if (NULL == pExceed)
		return RT_ERR_NULL_POINTER;
	
	switch(storm_type)
	{
		case STORM_GROUP_UNKNOWN_UNICAST: 
			rtl8309n_stormType = RTL8309N_UNKOWN_UNIDAPKT;
			break;
			
		case STORM_GROUP_UNKNOWN_MULTICAST:
			rtl8309n_stormType = RTL8309N_UNKOWN_MULTDAPKT;
			break;

		case STORM_GROUP_MULTICAST:
			rtl8309n_stormType = RTL8309N_MULTICASTPKT;
			break;

		case STORM_GROUP_BROADCAST:
			rtl8309n_stormType = RTL8309N_BROADCASTPKT;
			break;

		default:
			break;

	}	
	
	retVal = rtl8309n_storm_filterStatus_get(port, rtl8309n_stormType, pExceed);
	if(retVal != SUCCESS)
		return RT_ERR_FAILED;
	
	return RT_ERR_OK;
}

/* Function Name:
 *		rtk_mib_get
 * Description:
 *		Get mib counter value
 * Input:
 *		port	-	port id
 *		counter	-	mib counter type
 * Output:
 *		pValue	-	pointer point to mib counter value
 * Return:
 *		RT_ERR_PORT_ID		-	invalid port id
 *		RT_ERR_INPUT		-	invalid input parameter
 *		RT_ERR_NULL_POINTER		-	input parameter is null pointer
 *		RT_ERR_FAILED		-	failure
 *		RT_ERR_OK			-	success
 *Note:
 *		mib counter named MIB_TXBYTECNT and MIB_RXBYTECNT are counted by unit of byte. 
 *		And the counter values are 64bits long. So when these mib counter value are needed to 
 *		read out,  pValue should be pointed to a array with 2 unsigned 32bits data elements.
 *		To read out other mib counter, the unit is packet and pValue is pointed to a unsigned 32
 *		bits value.
 */
rtk_api_ret_t rtk_mib_get(rtk_port_t port, rtk_mib_counter_t counter, rtk_mib_cntValue_t *pValue)
{
     rtk_api_ret_t retVal;

     if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;
     if (counter >= MIB_END)
        return RT_ERR_INPUT;
     if (NULL == pValue)
        return RT_ERR_NULL_POINTER;

	 if(counter == MIB_TXBYTECNT || counter == MIB_RXBYTECNT)
	 {
	 	if ((retVal = rtl8309n_mib_byteCnt_get(port, counter, pValue)) != SUCCESS)
        	return RT_ERR_FAILED;
	 }
	 else if(counter >= MIB_TXPKTCNT && counter <= MIB_RXSYMBLCNT)
	 {
	 	if ((retVal = rtl8309n_mib_pktCnt_get(port, counter, pValue)) != SUCCESS)
        	return RT_ERR_FAILED;
	 }
     

     return RT_ERR_OK;
}


/* Function Name:
 *      rtk_stat_port_reset
 * Description:
 *      Reset per port MIB counter by port, and enable mib counter start to count.
 * Input:
 *      port 	- 	port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK   					-	success           
 *      RT_ERR_PORT_ID					-	invalid port id          
 *	  RT_ERR_STAT_PORT_CNTR_FAIL 	- 	Could not retrieve/reset Port Counter
 * Note:
 *	  This API can be called to enable mib counter, and reset port's mib counter to run.
 */
rtk_api_ret_t rtk_stat_port_reset(rtk_port_t port)
{
    rtk_api_ret_t retVal;

    if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID; 
    
    if ((retVal = rtl8309n_mib_reset(port)) != RT_ERR_OK)
        return RT_ERR_STAT_PORT_CNTR_FAIL; 
        
    return RT_ERR_OK;
}
/* Function Name:
 *      rtk_eee_portEnable_set
 * Description:
 *      Set enable ability of 100M  EEE function.
 * Input:
 *      port - port id.
 *      enable - enable 100M EEE ability.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_PORT_ID - Invalid port number.
 *      RT_ERR_ENABLE - Invalid enable input.
 * Note:
 *      This API can set 100M EEE function to the specific port.
 *      The configuration of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
rtk_api_ret_t rtk_eee_portEnable_set(rtk_port_t port, rtk_enable_t enable)
{
	rtk_api_ret_t retVal;
	uint32 phyData; 

    if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;
	
	if(enable >= RTK_ENABLE_END)
        return RT_ERR_ENABLE;
	
	if((retVal = rtl8309n_phyReg_get((uint32)port, 16, 4, &phyData)) != RT_ERR_OK)
        return retVal;
	
	if(ENABLED == enable)
		phyData |= (0x1<<12);
	else
		phyData &= ~(0x1<<12);

	if((retVal = rtl8309n_phyReg_set((uint32)port, 16, 4, phyData)) != RT_ERR_OK)
        return retVal;
	
	if ((retVal = rtl8309n_phyReg_get((uint32)port, 0, 0, &phyData))!=RT_ERR_OK)
        return retVal;
	
    phyData |= 0x0200;
	
    if ((retVal = rtl8309n_phyReg_set((uint32)port, 0, 0, phyData))!=RT_ERR_OK)
        return retVal;
	
	return RT_ERR_OK;
}
/* Function Name:
 *      rtk_eee_portEnable_get
 * Description:
 *      Get EEE status of 100M EEE function
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - status.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_PORT_ID - Invalid port number.
 * Note:
 *      This API can get 100M EEE function to the specific port.
 *      The EEE status of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
rtk_api_ret_t rtk_eee_portEnable_get(rtk_port_t port, rtk_enable_t *pEnable)
{
    rtk_api_ret_t retVal;
    uint32    regData;

    if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;

    if(NULL == pEnable)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8309n_reg_get(port,16, 1, &regData))!=RT_ERR_OK)
        return retVal;

    if ((regData&(0x1<<13))&&(regData&(0x1<<14)))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_port_phyMdx_set(rtk_port_t port, rtk_port_phy_mdix_mode_t mode)
{
    uint32 regData;
    rtk_api_ret_t retVal;

    switch(mode)
    {
        case PHY_AUTO_CROSSOVER_MODE:
            if((retVal = rtk_port_phyReg_get(port, 28, &regData)) != RT_ERR_OK)
                return retVal;

            regData |= (0x0001 << 2);

            if ((retVal = rtk_port_phyReg_set(port, 28, regData)) != RT_ERR_OK)
                return retVal;

            break;
        case PHY_FORCE_MDI_MODE:
            if((retVal = rtk_port_phyReg_get(port, 28, &regData)) != RT_ERR_OK)
                return retVal;

            regData &= ~(0x0001 << 2);
            regData |= (0x0001 << 1);

            if ((retVal = rtk_port_phyReg_set(port, 28, regData)) != RT_ERR_OK)
                return retVal;
            break;
        case PHY_FORCE_MDIX_MODE:
            if((retVal = rtk_port_phyReg_get(port, 28, &regData)) != RT_ERR_OK)
                return retVal;

            regData &= ~(0x0001 << 2);
            regData &= ~(0x0001 << 1);

            if ((retVal = rtk_port_phyReg_set(port, 28, regData)) != RT_ERR_OK)
                return retVal;
            break;
        default:
            return RT_ERR_INPUT;
            break;

    }

    if((retVal = rtk_port_phyReg_get(port, 0, &regData)) != RT_ERR_OK)
        return retVal;

    regData |= (0x0001 << 9);

    if ((retVal = rtk_port_phyReg_set(port, 0, regData)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_port_phyMdx_get(rtk_port_t port, rtk_port_phy_mdix_mode_t* pmode)
{
    uint32 regData;
    rtk_api_ret_t retVal;

	if((retVal = rtk_port_phyReg_get(port, 28, &regData)) != RT_ERR_OK)
		return retVal;

	if(regData &(0x0001 << 2))
		*pmode = PHY_AUTO_CROSSOVER_MODE;
	else if(regData &(0x0001 << 1))
		*pmode = PHY_FORCE_MDI_MODE;
	else
		*pmode = PHY_FORCE_MDIX_MODE;

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_port_phyMdxStatus_get(rtk_port_t port, 
rtk_port_phy_mdix_status_t *pStatus)
{
    uint32 regData;
    rtk_api_ret_t retVal;

	if((retVal = rtk_port_phyReg_get(port, 28, &regData)) != RT_ERR_OK)
		return retVal;
	
	if(regData &(0x0001 << 2))
	{
		if((retVal = rtk_port_phyReg_get(port, 30, &regData)) != RT_ERR_OK)
        	return retVal;
		if (regData & (0x0001 << 7))
	    {
	        *pStatus = PHY_STATUS_AUTO_MDI_MODE;
	    }
	    else
	    {
	        *pStatus = PHY_STATUS_AUTO_MDIX_MODE;
	    }
	}
	else
	{
		if((retVal = rtk_port_phyReg_get(port, 30, &regData)) != RT_ERR_OK)
	    	return retVal;
		if (regData & (0x0001 << 7))
	    {
	        *pStatus = PHY_STATUS_FORCE_MDI_MODE;
	    }
	    else
	    {
	        *pStatus = PHY_STATUS_FORCE_MDIX_MODE;
	    }
	}

    return RT_ERR_OK;
}




/* Function Name:
 *      rtk_port_adminEnable_set
 * Description:
 *      Set port admin configuration of the specific port.
 * Input:
 *      port    - port id.
 *      enable  - Back pressure status.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_PORT_ID      - Invalid port number.
 *      RT_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      This API can set port admin configuration of the specific port.
 *      The port admin configuration of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
rtk_api_ret_t rtk_port_adminEnable_set(rtk_port_t port, rtk_enable_t enable)
{
    rtk_api_ret_t retVal;
    uint32      data;


    if (enable >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if ((retVal = rtk_port_phyReg_get(port, PHY_CONTROL_REG, &data)) != 
RT_ERR_OK)
        return retVal;

    if (ENABLED == enable)
    {
        data &= 0xF7FF;
        data |= 0x0200;
    }
    else if (DISABLED == enable)
    {
        data |= 0x0800;
    }

    if ((retVal = rtk_port_phyReg_set(port, PHY_CONTROL_REG, data)) != 
RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_port_adminEnable_get
 * Description:
 *      Get port admin configurationof the specific port.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - Back pressure status.
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can get port admin configuration of the specific port.
 *      The port admin configuration of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
rtk_api_ret_t rtk_port_adminEnable_get(rtk_port_t port, rtk_enable_t *pEnable)
{
    rtk_api_ret_t retVal;
    uint32      data;

    if(NULL == pEnable)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtk_port_phyReg_get(port, PHY_CONTROL_REG, &data)) != 
RT_ERR_OK)
        return retVal;

    if ( (data & 0x0800) == 0x0800)
    {
        *pEnable = DISABLED;
    }
    else
    {
        *pEnable = ENABLED;
    }

    return RT_ERR_OK;
}


/*======================================================================================*/
/*
Following are not in the origianl switch API SDK.
*/

#define REG1301_8367R_VB				0x1010
#define REG1301_8367RB					0x1000

#define CONFIG_LAN_WAN_ISOLATION 1
#define CONFIG_RTK_REFINE_PORT_DUPLEX_MODE 0 //1

//#define RTL8367RB_USE_ONE_LED_PER_PORT		1

#if defined(CONFIG_RTL_8367MB_SUPPORT)
// now the HW is EXT0, it should be changed to EXT1 for cpu tag feature
#define EXT_PORT_FOR_HOST			EXT_PORT_0
#define EXT_PORT_FOR_RGMII2			EXT_PORT_1
#define EXT_MAC_FOR_HOST			RTK_EXT_0_MAC
#define LED_GROUP_X					LED_GROUP_2

#else // 8367RB, 8367R_VB
#define EXT_PORT_FOR_HOST			EXT_PORT_1
#define EXT_PORT_FOR_RGMII2			EXT_PORT_2
#define EXT_MAC_FOR_HOST			RTK_EXT_1_MAC
#define LED_GROUP_X					LED_GROUP_1
#endif

#ifndef CONFIG_RTL_8197F
#define ENABLE_8367RB_RGMII2	1
#endif

#ifdef ENABLE_8367RB_RGMII2
// for tr181
int rtk_rgmii_set(int enable)
{
    rtk_port_mac_ability_t mac_cfg;

    mac_cfg.forcemode = MAC_FORCE;
    mac_cfg.speed = SPD_1000M;
    mac_cfg.duplex = FULL_DUPLEX;
    if (enable == TRUE)
        mac_cfg.link = PORT_LINKUP;
    else		
        mac_cfg.link = PORT_LINKDOWN;
    mac_cfg.nway = DISABLED;
    mac_cfg.txpause = ENABLED;
    mac_cfg.rxpause = ENABLED;
    rtk_port_macForceLinkExt_set(EXT_PORT_FOR_RGMII2, MODE_EXT_RGMII, &mac_cfg);

    return 0;
}
#endif

#define		MAX_PORT_NUM = 8;
uint32 r8309m_cpu_port = -1;

enum 
{
	PORT_DOWN=0,
	HALF_DUPLEX_10M,
	HALF_DUPLEX_100M,
	HALF_DUPLEX_1000M,
	DUPLEX_10M,
	DUPLEX_100M,
	DUPLEX_1000M,
	PORT_AUTO
};

rtk_api_ret_t set_83XX_speed_mode(int port, int mode)
{
	rtk_port_phy_ability_t phyAbility;

	memset(&phyAbility, 0, sizeof(rtk_port_phy_ability_t));

	phyAbility.FC = 1;
	phyAbility.AsyFC = 1;
	phyAbility.AutoNegotiation = 1;

	if (mode == HALF_DUPLEX_10M) //10M half
	{
		phyAbility.Half_10 = 1;
	}
	else if (mode == DUPLEX_10M)	//10M full
	{
		phyAbility.Full_10 = 1;
	}
	else if (mode == HALF_DUPLEX_100M) // 100M half
	{
		phyAbility.Half_100 = 1;
	}
	else if (mode == DUPLEX_100M) // 100M full
	{
		phyAbility.Full_100 = 1;
	}
	else if (mode == DUPLEX_1000M) // 1000M
	{
		phyAbility.Full_1000 = 1;
	}
#ifdef CONFIG_EFM_PATCH
        else if (mode == PORT_DOWN) // 1000M
        {
                phyAbility.Full_1000 = 1;
        }
#endif
	else
	{
		phyAbility.Half_10 = 1;
		phyAbility.Full_10 = 1;
		phyAbility.Half_100 = 1;
		phyAbility.Full_100 = 1;
		phyAbility.Full_1000 = 1;
	}

	return (rtk_port_phyAutoNegoAbility_set(port, &phyAbility));
}


#if 0
int RTL8309M_init(void)
{
    rtk_portmask_t portmask;
    unsigned int ret;
	int32 port;
    //unsigned int regData;

    /* Set external interface 0 to RGMII with Force mode, 1000M, Full-duple, enable TX&RX pause*/
    rtl8309n_port_mac_ability_t mac_cfg;
    rtl8309n_mode_ext_t mode ;

	// do reset to all phy in case the 8309m may be set in boot loader
	for (port=0; port<RTL8309N_PHY_NUMBER; port++){
		rtl8309n_phyRegBit_set(port, 0, 15, 0, 1);
		rtk_stat_port_reset(port);
	}
	
    /* Initial Chip */
    ret = rtk_switch_init();

	/*  Initail number of queues for each port*/
// 	ret = rtk_qos_init(1);
	
    /* Enable LED Group 0&1 from P0 to P4 */
    portmask.bits[0]=0x1F;
    rtk_led_enable_set(LED_GROUP_0, portmask);
    rtk_led_enable_set(LED_GROUP_X, portmask);

	/*8309M has two LED indicaters, 0 and 1*/
//	rtl8309n_led_led1Enable_set(RTL8309N_ENABLED); 


    mode = RTL8309N_MODE_EXT_RMII ;
    mac_cfg.speed = RTL8309N_PORT_SPEED_100M;
    mac_cfg.duplex = RTL8309N_PORT_FULL_DUPLEX;
    mac_cfg.link = RTL8309N_PORT_LINK_UP;
    mac_cfg.nway = RTL8309N_DISABLED;
    mac_cfg.txpause = RTL8309N_ENABLED;
    mac_cfg.rxpause = RTL8309N_ENABLED;
    //rtk_port_macForceLinkExt_set(EXT_PORT_FOR_HOST,mode,&mac_cfg);
 	rtl8309n_port_macForceLinkExt0_set(mode, &mac_cfg);
	
#ifdef ENABLE_8367RB_RGMII2
    mode = MODE_EXT_RGMII ;
    mac_cfg.forcemode = MAC_FORCE;
    mac_cfg.speed = SPD_1000M;
    mac_cfg.duplex = FULL_DUPLEX;
    mac_cfg.link = PORT_LINKUP;
    mac_cfg.nway = DISABLED;
    mac_cfg.txpause = ENABLED;
    mac_cfg.rxpause = ENABLED;
    rtk_port_macForceLinkExt_set(EXT_PORT_FOR_RGMII2,mode,&mac_cfg);

    // the tx/rx delay is depend on the board
    //rtk_port_rgmiiDelayExt_set(EXT_PORT_FOR_RGMII2, 1, 3);
#endif

    /* Set RGMII Interface 0 TX delay to 2ns and RX to step 4 */
    // set the tx/rx delay in 8197D site
    //rtk_port_rgmiiDelayExt_set(EXT_PORT_1, 1, 4);
    //rtk_port_rgmiiDelayExt_set(EXT_PORT_1, 0, 0);
    rtk_port_rgmiiDelayExt_set(EXT_PORT_FOR_HOST, 0, 2); // change rxDelay to 2 to enhance the compatibility of 8197D and 8367RB


    /* set port 5 as CPU port */
    rtk_cpu_enable_set(ENABLE);
    rtk_cpu_tagPort_set(RTK_EXT_0_MAC, CPU_INSERT_TO_NONE);
 
    // for LED on Realtek 8197D+8367RB
#if defined(CONFIG_RTL_8367MB_SUPPORT)
	// group2 (bit11-8): 1001:Spd100(10)/Act
	// group0 (bit3-0): 0110:Spd1000/Act
    rtl8367b_setAsicReg(RTL8367B_REG_LED_CONFIGURATION, 0x906);
#elif defined(CONFIG_RTL_8309M_SUPPORT)
	rtl8309n_led_modeBlinkRate_set(RTL8309N_LED_MODE_0, RTL8309N_LED_BLINKRATE_32MS);
#else // 8367RB, 8367R_VB

    if (r8367_cpu_port == RTL8367B_PORT6_ENABLE_OFFSET) {    // 8367RB
#ifdef RTL8367RB_USE_ONE_LED_PER_PORT
        rtl8367b_setAsicReg(RTL8367B_REG_LED_CONFIGURATION, 0x0222);
#else
        /* demo board use 2 LEDs for each port */	
        rtl8367b_setAsicReg(RTL8367B_REG_LED_CONFIGURATION, 0x0936);
#endif
    }
#endif

#if 1
    // for 802.11ac logo 4.2.40 test (udp test item)
    rtl8367b_setAsicReg(0x121f, 0x01D6);
    rtl8367b_setAsicReg(0x1220, 0x01B8);
    rtl8367b_setAsicReg(0x1221, 0x01CC);
    rtl8367b_setAsicReg(0x1222, 0x01AE);
    rtl8367b_setAsicReg(0x1223, 0x0302);
    rtl8367b_setAsicReg(0x1224, 0x02E4);
    rtl8367b_setAsicReg(0x1225, 0x02D0);
    rtl8367b_setAsicReg(0x1226, 0x02A8);	
#endif

#ifdef CONFIG_RTL_8197F
	// for EMI
    rtl8367b_setAsicReg(0x13C3, 0x0000);	
    rtl8367b_setAsicReg(0x13C4, 0x0000);	
    rtl8367b_setAsicReg(0x1d53, 0x0001);	
    rtl8367b_setAsicReg(0x1d55, 0x000f);	
    rtl8367b_setAsicReg(0x1d54, 0x05fa);	
    rtl8367b_setAsicReg(0x1d52, 0x2473);	
    rtl8367b_setAsicReg(0x1d5a, 0x0001);	
    rtl8367b_setAsicReg(0x1d5c, 0x000f);	
    rtl8367b_setAsicReg(0x1d5b, 0x05fa);	
    rtl8367b_setAsicReg(0x1d59, 0x2473);	

    rtk_port_rgmiiDelayExt_set(EXT_PORT_FOR_HOST, 0, 5); // change rxDelay to 5 to enhance the compatibility of 8197F and 8367RB	
#endif

#if defined(CONFIG_RTK_REFINE_PORT_DUPLEX_MODE)
	rtk_forceFull_init();
#endif

#if defined(CONFIG_LAN_WAN_ISOLATION)
	{
	rtk_portmask_t pm;
	
	pm.bits[0] = 0xff;
	rtk_port_isolation_set(0, pm);
	rtk_port_isolation_set(1, pm);
	rtk_port_isolation_set(2, pm);
	rtk_port_isolation_set(3, pm);
	rtk_port_isolation_set(4, pm);
	}
#endif

    return ret; 
}
#endif

int RTL8309M_init_switch_mode(void)
{
//    rtk_portmask_t portmask;
    unsigned int ret;
	int port;

    /* Set external interface 0 to RGMII with Force mode, 1000M, Full-duple, enable TX&RX pause*/
    rtl8309n_port_mac_ability_t mac_cfg;
    rtl8309n_mode_ext_t mode ;

	// do reset to all phy in case the 8309m may be set in boot loader
//	for (port=0; port<RTL8309N_PORT_NUMBER; port++)
//		rtl8309n_phyRegBit_set(port, 0, 15, 0, 1);
 
    /* Initial Chip */
    ret = rtk_switch_init();
		
    /* Enable LED indicater 0*/
	ret = rtl8309n_led_modeBlinkRate_set(RTL8309N_LED_MODE_0, RTL8309N_LED_BLINKRATE_32MS);

    //8309m does not have RGMII
	mode = RTL8309N_MODE_EXT_MII_MAC;
    mac_cfg.speed = RTL8309N_PORT_SPEED_100M;
    mac_cfg.duplex = RTL8309N_PORT_FULL_DUPLEX;
    mac_cfg.link = RTL8309N_PORT_LINK_UP;
    mac_cfg.nway = RTL8309N_DISABLED;
    mac_cfg.txpause = RTL8309N_ENABLED;
    mac_cfg.rxpause = RTL8309N_ENABLED;
 	ret = rtl8309n_port_macForceLinkExt0_set(mode, &mac_cfg);

	ret = rtk_switch_maxPktLen_set(MAX_PKTLEN_16000B, 16000);

    return ret; 
}


#if defined(CONFIG_RTL_DNS_TRAP)
int rtl_8309_add_acl_for_dns(void)
{
	int port, retVal;
	rtk_filter_rule_t rule;

	/* disable cpu port's mac addr learning ability */
	//rtl8367b_setAsicLutLearnLimitNo(r8367_cpu_port,0);

	/* disable unknown unicast/mcast/bcast flooding between LAN ports */
	//smi_write(RTL8367B_REG_UNDA_FLOODING_PMSK, BIT(r8367_cpu_port));

	for(port=0; port<8; port++){
		memset(&rule, 0, sizeof(rtk_filter_rule_t));
		rule.phyport = port;
		rule.protocol = ACL_PRO_UDP;
		rule.data = 0x53;
		rule.priority = 0x3;
		rule.action = ACL_ACT_TRAP2CPU;
		rtk_filter_igrAcl_rule_add(&rule);  

	    if ((retVal = rtk_filter_igrAcl_rule_add(&rule)) != RT_ERR_OK)
	        return RT_ERR_FAILED;
	}
	
    return RT_ERR_OK;			
}

int rtl_8309_remove_acl_for_dns(void)
{
	int port, retVal;
	rtk_filter_rule_t rule;

	for(port=0; port<8; port++){
		memset(&rule, 0, sizeof(rtk_filter_rule_t));
		rule.phyport = port;
		rule.protocol = ACL_PRO_UDP;
		rule.data = 0x53;
		rule.priority = 0x3;
		rule.action = ACL_ACT_TRAP2CPU;
		rtk_filter_igrAcl_rule_add(&rule);  

	    if ((retVal = rtk_filter_igrAcl_rule_del(&rule)) != RT_ERR_OK)
	        return RT_ERR_FAILED;
	}

	return RT_ERR_OK;
}
#endif

#ifdef CONFIG_EFM_PATCH_XXX
#define BIT(nr)			(1UL << (nr))
#define WAN_VID			8
#define LAN_VID			9
#if defined(CONFIG_RTL_EXCHANGE_PORTMASK)
#define	RTL8367R_WAN			0		// WAN port is set to 8367R port 0
#else
#define	RTL8309M_WAN			-1		// WAN port is not in 8309M
#endif

#define	RTL_WANPORT_MASK		(0x1 << RTL8309M_WAN)
#define	RTL_LANPORT_MASK		(0x1ff & (~RTL_WANPORT_MASK))

#define GATEWAY_MODE				0
#define BRIDGE_MODE				1

#define PASSTHRU_VLAN_ID 100

static  int rtl8197d_op_mode = 0;

struct _vlan_conf {
	rtk_vlan_t 		vid;
	uint32		mbrmsk;
	uint32		untagmsk;
	rtk_fid_t			fid;
	rtk_pri_t 			priority;
};

#define RTL8309M_VIDMAX                     0xFFF
#define _VID_END	(RTL8309M_VIDMAX+1)
#define _8309M_RGMII2		8
#define BIT(nr)			(1UL << (nr))

// please assign different fid for them
struct _vlan_conf vc_gateway[] = {
#ifdef ENABLE_8367RB_RGMII2
	{ 	LAN_VID,	 	(RTL_LANPORT_MASK | BIT(_8367RB_RGMII2)),   	(RTL_LANPORT_MASK | BIT(_8367RB_RGMII2)),	0, 0 },
#else
#if defined(CONFIG_EFM_PATCH) && defined(CONFIG_RTL_8309M_SUPPORT)	/*EFM_DEBUG*/
	{ 	LAN_VID,	 	(RTL_LANPORT_MASK | BIT(_8309M_RGMII2)),   	RTL_LANPORT_MASK,	0, 0 },
#else
	{ 	LAN_VID,	 	RTL_LANPORT_MASK,   	RTL_LANPORT_MASK,	0, 0 },
#endif
#endif	
#if defined(CONFIG_EFM_PATCH) && defined(CONFIG_RTL_8309M_SUPPORT)	/*EFM_DEBUG*/
	{	WAN_VID,	0,   0,	1, 0},
#else
	{	WAN_VID,	RTL_WANPORT_MASK,   RTL_WANPORT_MASK,	1, 0},
#endif
	{	PASSTHRU_VLAN_ID,	(RTL_LANPORT_MASK|RTL_WANPORT_MASK),   (RTL_LANPORT_MASK|RTL_WANPORT_MASK), 0, 0},//for IPv6	
	{	_VID_END,	0, 0, 0, 0}
};

struct _vlan_conf vc_bridge_svl[] = {
	{ 	LAN_VID,	 	(RTL_LANPORT_MASK | RTL_WANPORT_MASK),   	(RTL_LANPORT_MASK | RTL_WANPORT_MASK),	2, 0 },
	{	_VID_END,	0, 0, 0, 0}
};


int _vlan_setting(struct _vlan_conf vc[])
{
	int i, j, retval;
	rtk_portmask_t mbrmsk, untagmsk;

	for(i=0; vc[i].vid <= RTL8309M_VIDMAX; i++)
	{
		mbrmsk.bits[0] = (vc[i].mbrmsk);
		untagmsk.bits[0] = (vc[i].untagmsk);
		retval = rtk_vlan_set(vc[i].vid, mbrmsk, untagmsk, vc[i].fid);

		if(vc[i].vid == PASSTHRU_VLAN_ID)		
			continue;
		
		/* set pvid*/	
		for(j=0;j<9;j++)
		{
			if  ((1<<j)& (vc[i].mbrmsk))
				rtk_vlan_portPvid_set(j, vc[i].vid, vc[i].priority);			
		}     	  
	}	
	return 0;
}

int RTL83XX_vlan_init(void)
{
	rtk_vlan_init();
	_vlan_setting(vc_gateway);
	rtl8197d_op_mode = GATEWAY_MODE;
	return 0;
}

int RTL83XX_vlan_reinit(int mode)
{
#if defined (CONFIG_RTL_IVL_SUPPORT)
	// when CONFIG_RTL_IVL_SUPPORT is defined, keep vc_gateway setting for gateway and bridge mode both
	
	rtk_l2_flushType_set(FLUSH_TYPE_BY_PORT, WAN_VID, r8367_cpu_port);				
#else

	if (mode==rtl8197d_op_mode) // no need tio do the re-initialization
		return 0;

	rtk_vlan_init();

	if (mode==GATEWAY_MODE)
		_vlan_setting(vc_gateway);
	
	else
		_vlan_setting(vc_bridge_svl);
		
#endif

	rtl8197d_op_mode = mode;	
	return 0;
}

#endif

#if 0

#define BIT(nr)			(1UL << (nr))
#define WAN_VID			8
#define LAN_VID			9
#if defined(CONFIG_RTL_EXCHANGE_PORTMASK)
#define	RTL8367R_WAN			0		// WAN port is set to 8367R port 0
#else
#define	RTL8367R_WAN			8		// WAN port is set to 8309M port 8
#endif

#define	RTL_WANPORT_MASK		(0x1 << RTL8367R_WAN)
#define	RTL_LANPORT_MASK		(0xff & (~RTL_WANPORT_MASK))

#define GATEWAY_MODE				0
#define BRIDGE_MODE				1

static  int rtl8197d_op_mode = 0;

// refine RTL8367R_vlan_init() and RTL8367R_vlan_reinit()
#if 1

struct _vlan_conf {
	rtk_vlan_t 		vid;
	rtk_uint32		mbrmsk;
	rtk_uint32		untagmsk;
	rtk_fid_t			fid;
	rtk_pri_t 			priority;
};

#define _VID_END	(RTL8367B_VIDMAX+1)
#define _8367RB_RGMII2		7
#define BIT(nr)			(1UL << (nr))

// please assign different fid for them
struct _vlan_conf vc_gateway[] = {
#ifdef ENABLE_8367RB_RGMII2
	{ 	LAN_VID,	 	(RTL_LANPORT_MASK | BIT(_8367RB_RGMII2)),   	(RTL_LANPORT_MASK | BIT(_8367RB_RGMII2)),	0, 0 },
#else
	{ 	LAN_VID,	 	RTL_LANPORT_MASK,   	RTL_LANPORT_MASK,	0, 0 },
#endif	
	{	WAN_VID,	RTL_WANPORT_MASK,   RTL_WANPORT_MASK,	1, 0},
	{	PASSTHRU_VLAN_ID,	(RTL_LANPORT_MASK|RTL_WANPORT_MASK),   (RTL_LANPORT_MASK|RTL_WANPORT_MASK), 0, 0},//for IPv6	
	{	_VID_END,	0, 0, 0, 0}
};

struct _vlan_conf vc_bridge_svl[] = {
	{ 	LAN_VID,	 	(RTL_LANPORT_MASK | RTL_WANPORT_MASK),   	(RTL_LANPORT_MASK | RTL_WANPORT_MASK),	2, 0 },
	{	_VID_END,	0, 0, 0, 0}
};


int _vlan_setting(struct _vlan_conf vc[])
{
	int i, j, retval;
	rtk_portmask_t mbrmsk, untagmsk;

	for(i=0; vc[i].vid <= RTL8367B_VIDMAX; i++)
	{
		mbrmsk.bits[0] = (vc[i].mbrmsk) |BIT(r8367_cpu_port);
		mbrmsk.bits[0] = (vc[i].mbrmsk);
		untagmsk.bits[0] = (vc[i].untagmsk);
		retval = rtk_vlan_set(vc[i].vid, mbrmsk, untagmsk, vc[i].fid);

		if(vc[i].vid == PASSTHRU_VLAN_ID)		
			continue;
		
		/* set pvid*/	
		for(j=0;j<9;j++)
		{
			if  ((1<<j)& (vc[i].mbrmsk))
				rtk_vlan_portPvid_set(j, vc[i].vid, vc[i].priority);			
		}     	  
	}	
	return 0;
}

int RTL83XX_vlan_init(void)
{
	_vlan_setting(vc_gateway);

	rtl8197d_op_mode = GATEWAY_MODE;
	return 0;
}

int RTL83XX_vlan_reinit(int mode)
{
#if defined (CONFIG_RTL_IVL_SUPPORT)
	// when CONFIG_RTL_IVL_SUPPORT is defined, keep vc_gateway setting for gateway and bridge mode both
	
	rtk_l2_flushType_set(FLUSH_TYPE_BY_PORT, WAN_VID, r8367_cpu_port);				
#else

	if (mode==rtl8197d_op_mode) // no need tio do the re-initialization
		return 0;

	rtk_vlan_init();

	if (mode==GATEWAY_MODE)
		_vlan_setting(vc_gateway);
	
	else
		_vlan_setting(vc_bridge_svl);
		
#endif

	rtl8197d_op_mode = mode;	
	return 0;
}

#else
int RTL83XX_vlan_init(void)
{
	int i, retval;
	rtk_portmask_t mbrmsk, untagmsk;

	/* for lan */
	mbrmsk.bits[0] = RTL_LANPORT_MASK|BIT(r8367_cpu_port);
	untagmsk.bits[0] = RTL_LANPORT_MASK;
	retval = rtk_vlan_set(LAN_VID, mbrmsk, untagmsk, 0);

	for(i=0;i<5;i++) {
		if  ((1<<i)&RTL_LANPORT_MASK)
		{
			retval = rtk_vlan_portPvid_set(i, LAN_VID, 0);			
		}
	}
		
	/* for wan */
	mbrmsk.bits[0] = RTL_WANPORT_MASK|BIT(r8367_cpu_port);
	untagmsk.bits[0] = RTL_WANPORT_MASK;
	retval = rtk_vlan_set(WAN_VID, mbrmsk, untagmsk, 1);

	for(i=0;i<5;i++) {
		if  ((1<<i)&RTL_WANPORT_MASK)
		{
			retval = rtk_vlan_portPvid_set(i, WAN_VID, 0);
		}
	}

	rtl8197d_op_mode = GATEWAY_MODE;
	return 0;
}

int RTL83XX_vlan_reinit(int mode)
{
	int i, retval;
	rtk_portmask_t mbrmsk, untagmsk;

	if (mode==rtl8197d_op_mode) // no need tio do the re-initialization
		return 0;

	rtk_vlan_init();
	
	if (mode==GATEWAY_MODE)
	{
		/* for lan */
		mbrmsk.bits[0] = RTL_LANPORT_MASK|BIT(r8367_cpu_port);
		untagmsk.bits[0] = RTL_LANPORT_MASK;
		retval = rtk_vlan_set(LAN_VID, mbrmsk, untagmsk, 0);

		for(i=0;i<5;i++) {
			if  ((1<<i)&RTL_LANPORT_MASK)
			{
				retval = rtk_vlan_portPvid_set(i, LAN_VID, 0);			
			}
		}
	
		/* for wan */
		mbrmsk.bits[0] = RTL_WANPORT_MASK|BIT(r8367_cpu_port);
		untagmsk.bits[0] = RTL_WANPORT_MASK;
		retval = rtk_vlan_set(WAN_VID, mbrmsk, untagmsk, 1);

		for(i=0;i<5;i++) {
			if  ((1<<i)&RTL_WANPORT_MASK)
			{
				retval = rtk_vlan_portPvid_set(i, WAN_VID, 0);
			}
		}		
	} 
	else {
		/* for lan */
		mbrmsk.bits[0] = (RTL_LANPORT_MASK | RTL_WANPORT_MASK) |BIT(r8367_cpu_port);
		untagmsk.bits[0] = (RTL_LANPORT_MASK | RTL_WANPORT_MASK);
		retval = rtk_vlan_set(LAN_VID, mbrmsk, untagmsk, 0);

		for(i=0;i<5;i++) {
			if  ((1<<i)&(RTL_LANPORT_MASK | RTL_WANPORT_MASK))
			{
				retval = rtk_vlan_portPvid_set(i, LAN_VID, 0);			
			}
		}		
	}
	rtl8197d_op_mode = mode;
	
	return 0;
}
#endif

#define RTL8367R_WAN_PORT_BITMAP 		(1<<RTL8367R_WAN)
#define RTL8367R_LAN_PORT_BITMAP 		(0xff - RTL8367R_WAN_PORT_BITMAP)
#define RTL8367R_LAN_EFID				2
extern int rtl865x_curOpMode;

#if defined(CONFIG_RTL_VLAN_8021Q) && defined(CONFIG_RTL_8309M_SUPPORT)
#if 0
int rtl_vlan_RTL8367R_set(unsigned short vid, unsigned int tagmask, unsigned int mask)
{
	rtk_portmask_t mbrmsk, untag;
	rtk_api_ret_t retVal;
	int i;
    unsigned untagmask;

    if(vid==WAN_VID || vid == LAN_VID)
        return 0;

    untagmask = mask&(~tagmask);
    
    mbrmsk.bits[0] = (BIT(r8367_cpu_port)|RTL8367R_WAN_PORT_BITMAP|RTL8367R_LAN_PORT_BITMAP);

    //panic_printk("untagmsk is 0x%x\n", (untagmask&RTL8367R_LAN_PORT_BITMAP));
    
    if(tagmask&RTL8367R_WAN_PORT_BITMAP)
        untag.bits[0] = 0;
    else
        untag.bits[0] = (BIT(r8367_cpu_port)|RTL8367R_WAN_PORT_BITMAP|(untagmask&RTL8367R_LAN_PORT_BITMAP));

    retVal=rtk_vlan_set(vid, mbrmsk, untag, 0); 
    
	return 0;
}
#else
int rtl_vlan_RTL8367R_set(unsigned short vid, unsigned int tagmask, unsigned int mask, unsigned int fid)
{
	int retval;
	rtk_portmask_t mbrmsk, untagmsk;
    
    unsigned untagmask;
    untagmask = mask&(~tagmask);
    if (mask == 0 && tagmask == 0)//clear
    {
		mbrmsk.bits[0] = mask;
		untagmsk.bits[0] = untagmask;
    }
    else
	{
		mbrmsk.bits[0] = (mask) |BIT(r8367_cpu_port);
		mbrmsk.bits[0] = (mask);
		//mbrmsk.bits[0] = (mask);
		untagmsk.bits[0] = untagmask;
        //panic_printk("%s %d vid = %u mbrmsk.bits[0]=%u untagmsk.bits[0]=%u\n", __FUNCTION__, __LINE__, vid, mbrmsk.bits[0], untagmsk.bits[0]);
	}
    
    retval = rtk_vlan_set(vid, mbrmsk, untagmsk, fid);        
    
	return 0;
}

#endif

int rtl_8367r_vlan_get(unsigned int i, unsigned int *mbrmsk, unsigned int *untagmsk, unsigned int *fid)
{
	rtk_portmask_t Mbrmsk = {{0}}, Untagmsk = {{0}};
	rtk_fid_t Fid = 0;
	rtk_api_ret_t ret = 0;
	
	if (!mbrmsk || !untagmsk || !fid)
		return -1;
	
	ret = rtk_vlan_get(i, &Mbrmsk, &Untagmsk, &Fid);
	if (ret == RT_ERR_OK)
	{
		*mbrmsk = Mbrmsk.bits[0];
		*untagmsk = Untagmsk.bits[0];
		*fid = Fid;
		return 0;
	}

	return -1;
}
#endif

int RTL8367R_vlan_set(void)
{
	rtk_portmask_t mbrmsk, untag;
	rtk_api_ret_t retVal;
	int i;
	
 	for(i=0;i<4096;i++)
 	{ 	
 		//if (i==WAN_VID ||i==LAN_VID)
 		#ifdef CONFIG_RTL_VLAN_8021Q
 		if (i==WAN_VID || i==LAN_VID
		#if defined(CONFIG_RTL_CUSTOM_PASSTHRU) 
			||i==PASSTHRU_VLAN_ID
		#endif	
			)
        #else
 		if (i==WAN_VID ||i==LAN_VID || (i==10) || (i==11) || (i==12)	
		#if defined(CONFIG_RTL_CUSTOM_PASSTHRU)
			||i==PASSTHRU_VLAN_ID
		#endif	
		) /* RTK VLAN */
        #endif
 		{
 			mbrmsk.bits[0] = (BIT(r8367_cpu_port)|RTL8367R_WAN_PORT_BITMAP|RTL8367R_LAN_PORT_BITMAP);
 			untag.bits[0] = (BIT(r8367_cpu_port)|RTL8367R_WAN_PORT_BITMAP|RTL8367R_LAN_PORT_BITMAP);
 			mbrmsk.bits[0] = RTL8367R_WAN_PORT_BITMAP|RTL8367R_LAN_PORT_BITMAP);
 			untag.bits[0] = RTL8367R_WAN_PORT_BITMAP|RTL8367R_LAN_PORT_BITMAP);

 		}
 		else
 		{
 			mbrmsk.bits[0] = (BIT(r8367_cpu_port)|RTL8367R_WAN_PORT_BITMAP|RTL8367R_LAN_PORT_BITMAP);
 			mbrmsk.bits[0] = RTL8367R_WAN_PORT_BITMAP|RTL8367R_LAN_PORT_BITMAP);
 			untag.bits[0] = 0;
 		} 	
 		retVal=rtk_vlan_set(i, mbrmsk, untag, 0); //all vlan's fid is 0 
 	}

	/* set pvid :  wan:8   lan:9  */	
	for(i=0;i<9;i++)
	{
		#if defined (CONFIG_RTL_IVL_SUPPORT)
		if (i == RTL8367R_WAN)
			retVal=rtk_vlan_portPvid_set(i, WAN_VID,0);
		else
			retVal=rtk_vlan_portPvid_set(i, LAN_VID,0);
		#else
		if(rtl865x_curOpMode==GATEWAY_MODE){
			if (i == RTL8367R_WAN)
				retVal=rtk_vlan_portPvid_set(i, WAN_VID,0);
			else
				retVal=rtk_vlan_portPvid_set(i, LAN_VID,0);
		}else{
			retVal=rtk_vlan_portPvid_set(i, LAN_VID,0);
		}
		#endif
	}       
	
	/* set wan port efid=1, other ports efid=2 */
	for(i=0;i<9;i++)
	{
		#if defined (CONFIG_RTL_IVL_SUPPORT)
		if (i == RTL8367R_WAN)
			retVal = rtk_port_efid_set(i,1);
		else
			retVal = rtk_port_efid_set(i,RTL8367R_LAN_EFID);
		#else
		if(rtl865x_curOpMode==GATEWAY_MODE){
			if (i == RTL8367R_WAN)
				retVal = rtk_port_efid_set(i,1);
			else
				retVal = rtk_port_efid_set(i,RTL8367R_LAN_EFID);
		}else{
			retVal = rtk_port_efid_set(i,RTL8367R_LAN_EFID);
		}
		#endif
	}

	// suggested by HM-Chung
#if defined (CONFIG_RTL_IVL_SUPPORT)	
	for (i=0; i<9; i++)
	{
		if (i == RTL8367R_WAN) 
			mbrmsk.bits[0] = BIT(r8367_cpu_port);    
		else
			mbrmsk.bits[0] = 0xff & ~BIT(RTL8367R_WAN); 
		rtk_port_isolation_set(i, mbrmsk);
	}    
#else	
	for (i=0; i<9; i++)
	{
		if (rtl865x_curOpMode == GATEWAY_MODE) {
			if (i == RTL8367R_WAN) 
				mbrmsk.bits[0] = BIT(r8367_cpu_port);    
			else
				mbrmsk.bits[0] = 0xff & ~BIT(RTL8367R_WAN); 
		}
		else
			mbrmsk.bits[0] = 0xff; 
			
		rtk_port_isolation_set(i, mbrmsk);
	}    
#endif

#if defined (CONFIG_RTL_IVL_SUPPORT)
	//if (rtl865x_curOpMode != GATEWAY_MODE) 
	{	// no matter Gateway or Bridge mode, always disable wan port L2 learning
		rtk_l2_limitLearningCnt_set(RTL8367R_WAN, 0);
		rtk_l2_flushType_set(FLUSH_TYPE_BY_PORT, WAN_VID, RTL8367R_WAN);		
	}
#else
	if (rtl865x_curOpMode == GATEWAY_MODE) {
		rtk_l2_limitLearningCnt_set(RTL8367R_WAN, 0);
		rtk_l2_flushType_set(FLUSH_TYPE_BY_PORT, WAN_VID, RTL8367R_WAN);		
		rtk_l2_flushType_set(FLUSH_TYPE_BY_PORT, LAN_VID, RTL8367R_WAN);		
	}
	else {
		rtk_l2_limitLearningCnt_set(RTL8367R_WAN, 2112);
		rtk_l2_flushType_set(FLUSH_TYPE_BY_PORT, WAN_VID, RTL8367R_WAN);		
		rtk_l2_flushType_set(FLUSH_TYPE_BY_PORT, LAN_VID, RTL8367R_WAN);		
	}
#endif

	/* disable cpu port's mac addr learning ability */
	rtl8367b_setAsicLutLearnLimitNo(r8367_cpu_port,0);
	
	/* disable unknown unicast/mcast/bcast flooding between LAN ports */
	smi_write(RTL8367B_REG_UNDA_FLOODING_PMSK, BIT(r8367_cpu_port));
	smi_write(RTL8367B_REG_UNMCAST_FLOADING_PMSK, BIT(r8367_cpu_port));
	smi_write(RTL8367B_REG_BCAST_FLOADING_PMSK, BIT(r8367_cpu_port));

	return 0;
}

void RTL8367R_cpu_tag(int enable)
{
	if(enable){
		rtl8367b_setAsicReg(RTL8367B_REG_CPU_PORT_MASK,1<<r8367_cpu_port); //set CPU port
		rtl8367b_setAsicReg(RTL8367B_REG_CPU_CTRL,0x281|(r8367_cpu_port)<<RTL8367B_CPU_TRAP_PORT_OFFSET);
	}
	else{
		rtl8367b_setAsicReg(RTL8367B_REG_CPU_CTRL,0x280|(r8367_cpu_port)<<RTL8367B_CPU_TRAP_PORT_OFFSET);
	}
}

void set_8367r_L2(unsigned int *mac, int intf_wan, int is_static)
{
	rtk_mac_t Mac;
	rtk_l2_ucastAddr_t L2_data;
		
	memset(&L2_data, 0, sizeof(rtk_l2_ucastAddr_t));
	
	L2_data.efid= (intf_wan)? 1 : 2;
	L2_data.port= RTL8309N_PORT8;
	L2_data.is_static=is_static;

	memcpy(&Mac.octet[0], mac, 6);
	rtk_l2_addr_add(&Mac,  &L2_data);
}

void del_83XX_L2(rtk_mac_t *pMac)
{
	rtk_l2_ucastAddr_t L2_data;

	memset(&L2_data, 0, sizeof(rtk_l2_ucastAddr_t));
	L2_data.fid = 0;
	L2_data.efid = RTL8367R_LAN_EFID;
	
	if (rtk_l2_addr_get(pMac, &L2_data) == RT_ERR_OK)
		rtk_l2_addr_del(pMac, &L2_data);

	return;
}


#if 0
void get_all_L2(void)
{
	int i, ret;
	rtk_l2_addr_table_t p;
		
	for (i=1; i<=RTK_MAX_NUM_OF_LEARN_LIMIT;i++)
	{
		p.index = i;
		ret = rtk_l2_entry_get(&p);
		if (ret == RT_ERR_OK)
		{
			printk(" [%d] mac: %02x:%02x:%02x:%02x:%02x:%02x, portmask: 0x%x, age: %d, fid: %d\n", i,
				p.mac.octet[0],p.mac.octet[1],p.mac.octet[2],p.mac.octet[3],p.mac.octet[4],p.mac.octet[5],
				p.portmask, p.age, p.fid);			
		}
	}
	return;
}
#endif

enum 
{
	PORT_DOWN=0,
	HALF_DUPLEX_10M,
	HALF_DUPLEX_100M,
	HALF_DUPLEX_1000M,
	DUPLEX_10M,
	DUPLEX_100M,
	DUPLEX_1000M,
	PORT_AUTO
};

void rtl83XX_reset(void)
{
	rtl8367b_setAsicReg(RTL8367B_REG_CHIP_RESET, (1<<RTL8367B_CHIP_RST_OFFSET));
	rtk_int32 port;
	// do reset to all phy in case the 8309m may be set in boot loader
	for (port=0; port<RTL8309N_PORT_NUMBER; port++)
		rtl8309n_phyRegBit_set(port, 0, 15, 0, 1);
	return;
}

#if defined(CONFIG_RTL_CUSTOM_PASSTHRU)
int rtl8367_setProtocolBasedVLAN(rtk_vlan_proto_type_t proto_type,rtk_vlan_t cvid, int cmdFlag)
{
	rtk_port_t port;
	int ret;
	rtk_vlan_protoAndPortInfo_t info;
	info.proto_type=proto_type;
   	info.frame_type=FRAME_TYPE_ETHERNET;
    info.cvid=cvid;
    info.cpri=0;
	
	if(cmdFlag==TRUE)
	{
		//printk("ADD[%s]:[%d].\n",__FUNCTION__,__LINE__);
		/*add */
		for(port=0;port<;port++){
			
			ret=rtk_vlan_protoAndPortBasedVlan_add(port, info);
			
		}
	}
	else
	{
		//printk("DEL[%s]:[%d].\n",__FUNCTION__,__LINE__);
		/*delete */
		for(port=0;port<9;port++){
			
			ret=rtk_vlan_protoAndPortBasedVlan_del(port,  proto_type, FRAME_TYPE_ETHERNET);
		}
	}
	return ret;
}
#endif

#if defined(CONFIG_RTK_VLAN_SUPPORT) || defined(CONFIG_RTL_VLAN_8021Q) || defined(CONFIG_OPENWRT_SDK) || defined(CONFIG_RTL_HW_VLAN_SUPPORT)
int rtl865x_enableRtl8367ToCpuAcl(void)
{
	int retVal;
	rtk_filter_field_t	filter_field[2];
	rtk_filter_cfg_t	cfg;
	rtk_filter_action_t	act;
	rtk_filter_number_t	ruleNum = 0;

         memset(filter_field, 0, 2*sizeof(rtk_filter_field_t));
         memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
         memset(&act, 0, sizeof(rtk_filter_action_t));
         filter_field[0].fieldType  = FILTER_FIELD_DMAC;
         if ((retVal = rtk_filter_igrAcl_field_add(&cfg,	&filter_field[0])) != RT_ERR_OK)
         	return retVal;

         /*add all ports to active ports*/
         cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
         cfg.activeport.value = 0x1F;
         cfg.activeport.mask = 0xFF;
         cfg.invert = FALSE;
         act.actEnable[FILTER_ENACT_TRAP_CPU] = TRUE;
         if ((retVal = rtk_filter_igrAcl_cfg_add(0, &cfg, &act, &ruleNum)) != RT_ERR_OK)
              	return retVal;
         return RT_ERR_OK;
}

int rtl865x_disableRtl8367ToCpuAcl(void)
{
	return rtk_filter_igrAcl_cfg_del(0);
}
#endif

#if defined(CONFIG_RTK_REFINE_PORT_DUPLEX_MODE)
#define MIB_STATE_FRAG_VAL_UPDATE_BASE		0x13A0
#define MIB_STATE_FRAG_CTL_UPDATE_BASE	0x13A4
int rtk_refinePortDuplexMode(void)
{
	rtk_stat_counter_t stateFragCounters = 0;
	int port;
	
	for(port=0; port<9; port++)
	{
		rtk_stat_port_get(port, STAT_EtherStatsFragments, &stateFragCounters);
		//if(port == 1)
			//printk("stateFragCounters is %d\n", (unsigned short)stateFragCounters);
		rtl8367b_setAsicReg(MIB_STATE_FRAG_VAL_UPDATE_BASE+port, (unsigned short)stateFragCounters);
		rtl8367b_setAsicReg(MIB_STATE_FRAG_CTL_UPDATE_BASE+port, 0x1);
	}
	return RT_ERR_OK;
}


rtk_api_ret_t rtk_forceFull_init(void)
{
    rtk_api_ret_t retVal;
//    rtk_uint32 busyFlag;    
    rtk_uint16 i,length;
    static rtk_uint8 iromCode[] = {
    0x02,0x05,0xFE,0x02,0x04,0x23,0x7D,0x06,
    0x7C,0x11,0x7F,0xC2,0x7E,0x12,0x12,0x05,
    0xE2,0x7D,0x05,0x7C,0x20,0x7F,0xC3,0x7E,
    0x12,0x12,0x05,0xE2,0x7D,0x51,0x7C,0x00,
    0x7F,0x36,0x7E,0x13,0x12,0x05,0xE2,0xE4,
    0x90,0x00,0x00,0xF0,0xA3,0xF0,0x90,0x00,
    0x06,0xF0,0xA3,0xF0,0xA3,0xF0,0xA3,0xF0,
    0x90,0x00,0x1A,0xF0,0x90,0x00,0x1A,0xE0,
    0xFF,0xC3,0x94,0x04,0x50,0x59,0x74,0x01,
    0x7E,0x00,0xA8,0x07,0x08,0x80,0x05,0xC3,
    0x33,0xCE,0x33,0xCE,0xD8,0xF9,0x54,0x0F,
    0x60,0x11,0x90,0x00,0x1A,0xE0,0x24,0x12,
    0xFF,0xE4,0x34,0x13,0xFE,0xE4,0xFD,0xFC,
    0x12,0x05,0xE2,0x90,0x00,0x1A,0xE0,0xFF,
    0x25,0xE0,0x24,0x0A,0xF5,0x82,0xE4,0x34,
    0x00,0xF5,0x83,0xE4,0xF0,0xA3,0xF0,0x74,
    0x02,0x2F,0xF5,0x82,0xE4,0x34,0x00,0xF5,
    0x83,0xE4,0xF0,0x74,0x12,0x2F,0xF5,0x82,
    0xE4,0x34,0x00,0xF5,0x83,0xE4,0xF0,0x90,
    0x00,0x1A,0xE0,0x04,0xF0,0x80,0x9D,0x12,
    0x06,0x14,0x7D,0x01,0x7C,0x00,0x7F,0x18,
    0x7E,0x11,0x12,0x05,0xE2,0x90,0x00,0x00,
    0xE0,0x70,0x02,0xA3,0xE0,0x70,0x03,0x02,
    0x02,0xB0,0xE4,0x90,0x00,0x1A,0xF0,0x90,
    0x00,0x1A,0xE0,0xFF,0xC3,0x94,0x04,0x40,
    0x03,0x02,0x02,0xB0,0x74,0x01,0x7E,0x00,
    0xA8,0x07,0x08,0x80,0x05,0xC3,0x33,0xCE,
    0x33,0xCE,0xD8,0xF9,0xFF,0x90,0x00,0x00,
    0xE0,0x5E,0xFE,0xA3,0xE0,0x5F,0x4E,0x70,
    0x03,0x02,0x02,0xA7,0x90,0x00,0x1A,0xE0,
    0xFF,0x24,0x02,0xF5,0x82,0xE4,0x34,0x00,
    0xF5,0x83,0xE0,0x70,0x67,0xEF,0x24,0xA4,
    0xFF,0xE4,0x34,0x13,0xFE,0x12,0x05,0xBE,
    0x90,0x00,0x1F,0xEE,0xF0,0xA3,0xEF,0xF0,
    0x4E,0x70,0x03,0x02,0x02,0xA7,0x90,0x00,
    0x1A,0xE0,0xFF,0x24,0x02,0xF5,0x82,0xE4,
    0x34,0x00,0xF5,0x83,0x74,0x01,0xF0,0xEF,
    0x24,0xA0,0xFF,0xE4,0x34,0x13,0xFE,0x12,
    0x05,0xBE,0x90,0x00,0x1A,0xE0,0xFD,0x25,
    0xE0,0x24,0x0A,0xF5,0x82,0xE4,0x34,0x00,
    0xF5,0x83,0xEE,0xF0,0xA3,0xEF,0xF0,0x74,
    0x12,0x2D,0xF5,0x82,0xE4,0x34,0x00,0xF5,
    0x83,0xE4,0xF0,0xED,0x24,0xA4,0xFF,0xE4,
    0x34,0x13,0xFE,0xE4,0xFD,0xFC,0x12,0x05,
    0xE2,0x02,0x02,0xA7,0xE4,0x90,0x00,0x21,
    0xF0,0x90,0x00,0x1A,0xE0,0x24,0xA4,0xFF,
    0xE4,0x34,0x13,0xFE,0x12,0x05,0xBE,0x90,
    0x00,0x1F,0xEE,0xF0,0xA3,0xEF,0xF0,0x4E,
    0x70,0x03,0x02,0x02,0x4C,0x90,0x00,0x1A,
    0xE0,0x24,0xA0,0xFF,0xE4,0x34,0x13,0xFE,
    0x12,0x05,0xBE,0x90,0x00,0x1B,0xEE,0xF0,
    0xA3,0xEF,0xF0,0x90,0x00,0x1A,0xE0,0xFD,
    0x25,0xE0,0x24,0x0A,0xF5,0x82,0xE4,0x34,
    0x00,0xF5,0x83,0xE0,0xFA,0xA3,0xE0,0x6F,
    0x70,0x02,0xEA,0x6E,0x60,0x59,0x90,0x00,
    0x1A,0xE0,0xFF,0x24,0x12,0xF5,0x82,0xE4,
    0x34,0x00,0xF5,0x83,0xE0,0x04,0xF0,0xEF,
    0x24,0xD4,0xFF,0xE4,0x34,0x12,0xFE,0x74,
    0x12,0x2D,0xF5,0x82,0xE4,0x34,0x00,0xF5,
    0x83,0xE0,0xFD,0x7C,0x00,0x12,0x05,0xE2,
    0x90,0x00,0x1A,0xE0,0x24,0xD0,0xFF,0xE4,
    0x34,0x12,0xFE,0xA3,0xE0,0xFC,0xA3,0xE0,
    0xFD,0x12,0x05,0xE2,0x90,0x00,0x1A,0xE0,
    0x24,0x12,0xF5,0x82,0xE4,0x34,0x00,0xF5,
    0x83,0xE0,0xD3,0x94,0x0A,0x40,0x34,0x90,
    0x00,0x21,0x74,0x01,0xF0,0x80,0x2C,0x90,
    0x00,0x1A,0xE0,0xFF,0x24,0x12,0xF5,0x82,
    0xE4,0x34,0x00,0xF5,0x83,0xE4,0xF0,0xEF,
    0x24,0xD4,0xFF,0xE4,0x34,0x12,0xFE,0x90,
    0x00,0x1A,0xE0,0x24,0x12,0xF5,0x82,0xE4,
    0x34,0x00,0xF5,0x83,0xE0,0xFD,0x7C,0x00,
    0x12,0x05,0xE2,0x90,0x00,0x1A,0xE0,0x24,
    0xA4,0xFF,0xE4,0x34,0x13,0xFE,0xE4,0xFD,
    0xFC,0x12,0x05,0xE2,0x90,0x00,0x21,0xE0,
    0x60,0x55,0x90,0x00,0x1A,0xE0,0x24,0x52,
    0xFF,0xE4,0x34,0x13,0xFE,0x12,0x05,0xBE,
    0xEE,0x44,0x10,0xAD,0x07,0xFC,0xED,0x54,
    0xFB,0xFD,0x90,0x00,0x1A,0xE0,0x24,0x12,
    0xFF,0xE4,0x34,0x13,0xFE,0x12,0x05,0xE2,
    0x90,0x00,0x1A,0xE0,0xFF,0x74,0x01,0x7E,
    0x00,0xA8,0x07,0x08,0x80,0x05,0xC3,0x33,
    0xCE,0x33,0xCE,0xD8,0xF9,0xFF,0xF4,0xFD,
    0xEE,0xF4,0xFC,0x90,0x00,0x00,0xE0,0x5C,
    0xF0,0xA3,0xE0,0x5D,0xF0,0x90,0x00,0x06,
    0xE0,0x4E,0xF0,0xA3,0xE0,0x4F,0xF0,0x90,
    0x00,0x1A,0xE0,0x04,0xF0,0x02,0x00,0xBF,
    0x90,0x00,0x08,0xE0,0x70,0x02,0xA3,0xE0,
    0x70,0x03,0x02,0x03,0x6A,0xE4,0x90,0x00,
    0x1A,0xF0,0x90,0x00,0x1A,0xE0,0xFF,0xC3,
    0x94,0x04,0x40,0x03,0x02,0x03,0x6A,0x74,
    0x01,0x7E,0x00,0xA8,0x07,0x08,0x80,0x05,
    0xC3,0x33,0xCE,0x33,0xCE,0xD8,0xF9,0xFF,
    0x90,0x00,0x08,0xE0,0x5E,0xFE,0xA3,0xE0,
    0x5F,0x4E,0x60,0x75,0x90,0x00,0x1A,0xE0,
    0x75,0xF0,0x20,0xA4,0x24,0x01,0xFF,0xE5,
    0xF0,0x34,0x20,0xFE,0x12,0x05,0xBE,0xEF,
    0x20,0xE2,0x5E,0x90,0x00,0x1A,0xE0,0x24,
    0x12,0xFF,0xE4,0x34,0x13,0xFE,0xE4,0xFD,
    0xFC,0x12,0x05,0xE2,0x90,0x00,0x1A,0xE0,
    0xF9,0x74,0x01,0x7E,0x00,0xA8,0x01,0x08,
    0x80,0x05,0xC3,0x33,0xCE,0x33,0xCE,0xD8,
    0xF9,0xF4,0xFF,0xEE,0xF4,0xFE,0x90,0x00,
    0x00,0xE0,0x5E,0xF0,0xA3,0xE0,0x5F,0xF0,
    0x90,0x00,0x08,0xE0,0x5E,0xF0,0xA3,0xE0,
    0x5F,0xF0,0x74,0x01,0x7E,0x00,0xA8,0x01,
    0x08,0x80,0x05,0xC3,0x33,0xCE,0x33,0xCE,
    0xD8,0xF9,0xF4,0xFF,0xEE,0xF4,0xFE,0x90,
    0x00,0x06,0xE0,0x5E,0xF0,0xA3,0xE0,0x5F,
    0xF0,0x90,0x00,0x1A,0xE0,0x04,0xF0,0x02,
    0x02,0xC2,0x90,0x00,0x00,0xE0,0xFC,0xA3,
    0xE0,0xFD,0x7F,0xC4,0x7E,0x12,0x12,0x05,
    0xE2,0x90,0x00,0x08,0xE0,0xFC,0xA3,0xE0,
    0xFD,0x7F,0xC5,0x7E,0x12,0x12,0x05,0xE2,
    0x90,0x00,0x06,0xE0,0xFC,0xA3,0xE0,0xFD,
    0x7F,0xC6,0x7E,0x12,0x12,0x05,0xE2,0x90,
    0x00,0x02,0xE0,0xFD,0x7C,0x00,0x7F,0xC8,
    0x7E,0x12,0x12,0x05,0xE2,0x90,0x00,0x03,
    0xE0,0xFD,0x7C,0x00,0x7F,0xC9,0x7E,0x12,
    0x12,0x05,0xE2,0x90,0x00,0x04,0xE0,0xFD,
    0x7C,0x00,0x7F,0xCA,0x7E,0x12,0x12,0x05,
    0xE2,0x90,0x00,0x05,0xE0,0xFD,0x7C,0x00,
    0x7F,0xCB,0x7E,0x12,0x12,0x05,0xE2,0x90,
    0x00,0x0A,0xE0,0xFC,0xA3,0xE0,0xFD,0x7F,
    0xCC,0x7E,0x12,0x12,0x05,0xE2,0x90,0x00,
    0x0C,0xE0,0xFC,0xA3,0xE0,0xFD,0x7F,0xCD,
    0x7E,0x12,0x12,0x05,0xE2,0x90,0x00,0x0E,
    0xE0,0xFC,0xA3,0xE0,0xFD,0x7F,0xCE,0x7E,
    0x12,0x12,0x05,0xE2,0x90,0x00,0x10,0xE0,
    0xFC,0xA3,0xE0,0xFD,0x7F,0xCF,0x7E,0x12,
    0x12,0x05,0xE2,0x7F,0xC1,0x7E,0x12,0x12,
    0x05,0xBE,0xEF,0x24,0x01,0xFD,0xE4,0x3E,
    0xFC,0x7F,0xC1,0x7E,0x12,0x12,0x05,0xE2,
    0x02,0x00,0xAD,0xC0,0xE0,0xC0,0xF0,0xC0,
    0x83,0xC0,0x82,0xC0,0xD0,0x75,0xD0,0x00,
    0xC0,0x00,0xC0,0x01,0xC0,0x02,0xC0,0x03,
    0xC0,0x04,0xC0,0x05,0xC0,0x06,0xC0,0x07,
    0xC2,0xAF,0x7F,0x24,0x7E,0x11,0x12,0x05,
    0xBE,0x90,0x00,0x18,0xEE,0xF0,0xA3,0xEF,
    0xF0,0x7F,0x23,0x7E,0x11,0x12,0x05,0xBE,
    0x90,0x00,0x16,0xEE,0xF0,0xA3,0xEF,0xF0,
    0x78,0x0C,0x76,0x00,0x78,0x0C,0xE6,0xFF,
    0xC3,0x94,0x04,0x40,0x03,0x02,0x05,0x5C,
    0x74,0x01,0x7E,0x00,0xA8,0x07,0x08,0x80,
    0x05,0xC3,0x33,0xCE,0x33,0xCE,0xD8,0xF9,
    0xFF,0x90,0x00,0x18,0xE0,0x5E,0xFC,0xA3,
    0xE0,0x5F,0x4C,0x70,0x03,0x02,0x05,0x56,
    0xEF,0x54,0x0F,0x70,0x03,0x02,0x05,0x56,
    0x78,0x0C,0xE6,0x75,0xF0,0x20,0xA4,0x24,
    0x05,0xFF,0xE5,0xF0,0x34,0x20,0xFE,0x12,
    0x05,0xBE,0xEE,0x30,0xE6,0x03,0x02,0x05,
    0x56,0x78,0x0C,0xE6,0x24,0x52,0xFF,0xE4,
    0x34,0x13,0xFE,0x12,0x05,0xBE,0xAD,0x07,
    0xAC,0x06,0xED,0x30,0xE2,0x03,0x02,0x05,
    0x56,0x20,0xE7,0x03,0x02,0x05,0x56,0x78,
    0x0C,0xE6,0xF9,0x74,0x01,0x7E,0x00,0xA8,
    0x01,0x08,0x80,0x05,0xC3,0x33,0xCE,0x33,
    0xCE,0xD8,0xF9,0xFF,0x90,0x00,0x06,0xE0,
    0x5E,0xFE,0xA3,0xE0,0x5F,0x4E,0x70,0x66,
    0x04,0x7E,0x00,0xA8,0x01,0x08,0x80,0x05,
    0xC3,0x33,0xCE,0x33,0xCE,0xD8,0xF9,0xFF,
    0x90,0x00,0x00,0xE0,0x4E,0xF0,0xA3,0xE0,
    0x4F,0xF0,0x74,0x01,0x7E,0x00,0xA8,0x01,
    0x08,0x80,0x05,0xC3,0x33,0xCE,0x33,0xCE,
    0xD8,0xF9,0xFF,0x90,0x00,0x08,0xE0,0x4E,
    0xF0,0xA3,0xE0,0x4F,0xF0,0xE9,0x24,0x12,
    0xFF,0xE4,0x34,0x13,0xFE,0xEC,0x44,0x10,
    0xFC,0xED,0x44,0x04,0xFD,0x12,0x05,0xE2,
    0x78,0x0C,0xE6,0x24,0xA4,0xFF,0xE4,0x34,
    0x13,0xFE,0xE4,0xFD,0xFC,0x12,0x05,0xE2,
    0x78,0x0C,0xE6,0x24,0x02,0xF5,0x82,0xE4,
    0x34,0x00,0xF5,0x83,0xE4,0xF0,0x78,0x0C,
    0x06,0x02,0x04,0x64,0x90,0x00,0x18,0xE0,
    0xFC,0xA3,0xE0,0xFD,0x7F,0x24,0x7E,0x11,
    0x12,0x05,0xE2,0x90,0x00,0x16,0xE0,0xFC,
    0xA3,0xE0,0xFD,0x7F,0x23,0x7E,0x11,0x12,
    0x05,0xE2,0x7F,0x19,0x7E,0x11,0x12,0x05,
    0xBE,0xAC,0x06,0xAD,0x07,0x7F,0x19,0x7E,
    0x11,0x12,0x05,0xE2,0x7F,0xC0,0x7E,0x12,
    0x12,0x05,0xBE,0xEF,0x24,0x01,0xFD,0xE4,
    0x3E,0xFC,0x7F,0xC0,0x7E,0x12,0x12,0x05,
    0xE2,0xD2,0xAF,0xD0,0x07,0xD0,0x06,0xD0,
    0x05,0xD0,0x04,0xD0,0x03,0xD0,0x02,0xD0,
    0x01,0xD0,0x00,0xD0,0xD0,0xD0,0x82,0xD0,
    0x83,0xD0,0xF0,0xD0,0xE0,0x32,0xC2,0xAF,
    0xAD,0x07,0xAC,0x06,0x8C,0xA2,0x8D,0xA3,
    0x75,0xA0,0x01,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0xAE,0xA1,
    0xBE,0x00,0xF0,0xAE,0xA6,0xAF,0xA7,0xD2,
    0xAF,0x22,0xC2,0xAF,0xAB,0x07,0xAA,0x06,
    0x8A,0xA2,0x8B,0xA3,0x8C,0xA4,0x8D,0xA5,
    0x75,0xA0,0x03,0x00,0x00,0x00,0xAA,0xA1,
    0xBA,0x00,0xF8,0xD2,0xAF,0x22,0x78,0x7F,
    0xE4,0xF6,0xD8,0xFD,0x75,0x81,0x0C,0x02,
    0x00,0x06,0xE4,0xF5,0xA8,0xF5,0xE8,0xD2,
    0xAF,0xD2,0xA8,0x22,0x12,0x06,0x1A,0x02,
    0x06,0x0A,0xE4,0xF5,0x8E,0x22};

    length  = (sizeof(iromCode) / sizeof(rtk_uint8));
	
    if ((retVal = rtl8367b_setAsicRegBit(0x1322, 4, 1))!=RT_ERR_OK)
        return retVal;
    if ((retVal = rtl8367b_setAsicRegBit(0x130c, 5, 1))!=RT_ERR_OK)
        return retVal;
    if ((retVal = rtl8367b_setAsicRegBit(0x1336, 1, 1))!=RT_ERR_OK)
        return retVal;
    if ((retVal = rtl8367b_setAsicRegBit(0x1322, 2, 0))!=RT_ERR_OK)
        return retVal;

    for(i=0; i<length; i++)
    {
        if(i == 0x2000)
        {
            if ((retVal = rtl8367b_setAsicRegBit(0x1336, 2, 1))!=RT_ERR_OK)
                return retVal;
        }
        rtl8367b_setAsicReg((rtk_uint32)(0xE000 + i % 0x2000), (rtk_uint32)iromCode[i]);
    }

    if ((retVal = rtl8367b_setAsicRegBit(0x1336, 2, 0))!=RT_ERR_OK)
        return retVal;
    if ((retVal = rtl8367b_setAsicRegBit(0x1336, 1, 0))!=RT_ERR_OK)
        return retVal;
    if ((retVal = rtl8367b_setAsicRegBit(0x1322, 4, 0))!=RT_ERR_OK)
        return retVal;
        
    return RT_ERR_OK;

}
#endif

#if 1//defined(CONFIG_RTL_HW_VLAN_SUPPORT)
#if defined(CONFIG_RTL_PROC_NEW)
int rtl_8367r_vlan_read(struct seq_file *s, void *v)
{
	int  i = 0, ret = 0;
    unsigned int pvid = 0, priority = 0, fid = 0;
    rtk_portmask_t Mbrmsk = {{0}}, Untagmsk={{0}};
    rtk_l2_addr_table_t l2_entry;
    
	seq_printf(s, "%s\n", "vlan:");
    for (i=1; i <= 1000; i++)
    {
		if(i!=1 && i!=8 && i!=9 && i%100!=0)
			continue;

		memset(&Mbrmsk, 0x00, sizeof(Mbrmsk));        
        memset(&Untagmsk, 0x00, sizeof(Untagmsk));
        fid =0;
        if ((rtk_vlan_get(i, &Mbrmsk, &Untagmsk, &fid)== 0) && (Mbrmsk.bits[0] != 0))
            seq_printf(s, "vid %d Mbrmsk 0x%x Untagmsk 0x%x fid %u\n", i, Mbrmsk.bits[0], Untagmsk.bits[0], fid);           
    }

	seq_printf(s, "\n%s\n", "pvid:");
    for(i=0;i<8;i++)
    {
        pvid = priority = 0;        
        if (rtk_vlan_portPvid_get(i, &pvid, &priority)==0)
            seq_printf(s, "port %d pvid %u pri %u\n", i, pvid, priority);           
    }

    
	seq_printf(s, "\n%s\n", "l2:");
     /*Get All Lookup Table and Print the valid entry*/
 
    for (i=1;i<=2112;i++)
    {
        memset(&l2_entry,0,sizeof(rtk_l2_addr_table_t));
        l2_entry.index = i;
        ret = rtk_l2_entry_get(&l2_entry);
        if (ret==RT_ERR_OK)
        {
            if(l2_entry.is_ipmul)
            {
                
                seq_printf(s, "\r\nIndex SourceIP DestinationIP MemberPort State\n");           
                seq_printf(s, "%4d ", l2_entry.index);
                seq_printf(s,"%0x ",(l2_entry.sip));
                seq_printf(s,"%0x ",(l2_entry.dip));
                seq_printf(s,"%-8x ",l2_entry.portmask);
                seq_printf(s,"%s \n",(l2_entry.is_static? "Static" : "Auto"));
            }
            else if(l2_entry.mac.octet[0]&0x01)
            {
                seq_printf(s,"%4d %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X %-8x %-4d %-4s %-5s %s %d\n",
                l2_entry.index,
                l2_entry.mac.octet[0],l2_entry.mac.octet[1],l2_entry.mac.octet[2],
                l2_entry.mac.octet[3],l2_entry.mac.octet[4],l2_entry.mac.octet[5],
                l2_entry.portmask, l2_entry.fid, (l2_entry.auth ? "Auth" : "x"),
                (l2_entry.sa_block? "Block" : "x"), (l2_entry.is_static? "Static" : "Auto"),
                l2_entry.age);
            }
            else if((l2_entry.age!=0)||(l2_entry.is_static==1))
            {
                seq_printf(s,"%4d %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X %-8x %-4d %-4s %-5s %s %d\n",
                l2_entry.index,l2_entry.mac.octet[0],l2_entry.mac.octet[1],l2_entry.mac.octet[2],
                l2_entry.mac.octet[3],l2_entry.mac.octet[4],l2_entry.mac.octet[5],
                l2_entry.portmask, l2_entry.fid, (l2_entry.auth ? "Auth" : "x"),
                (l2_entry.sa_block? "Block" : "x"), (l2_entry.is_static? "Static" : "Auto"),
                l2_entry.age);
            }
        }
    }


	return 0;
}

#else
int rtl_8367r_vlan_read( char *page, char **start, off_t off, int count, int *eof, void *data )
{
	int len = 0, i = 0, ret = 0;
    unsigned int pvid = 0, priority = 0, fid = 0;
    rtk_portmask_t Mbrmsk = {0}, Untagmsk={0};
    
	len = sprintf(page, "%s\n", "vlan:");
    for (i=0; i < 4096; i++)
    {
        memset(&Mbrmsk, 0x00, sizeof(Mbrmsk));        
        memset(&Untagmsk, 0x00, sizeof(Untagmsk));
        fid =0;
        if ((rtk_vlan_get(i, &Mbrmsk, &Untagmsk, &fid)== 0) && (Mbrmsk.bits[0] != 0))
            len += sprintf(page+len, "vid %d Mbrmsk 0x%x Untagmsk 0x%x fid %u\n", i, Mbrmsk.bits[0], Untagmsk.bits[0], fid);           
    }

	len += sprintf(page+len, "\n%s\n", "pvid:");
    for(i=0;i<8;i++)
    {
        pvid = priority = 0;        
        if (rtk_vlan_portPvid_get(i, &pvid, &priority)==0)
            len += sprintf(page+len, "port %d pvid %u pri %u\n", i, pvid, priority);           
    }

    
	len += sprintf(page+len, "\n%s\n", "l2:");
     /*Get All Lookup Table and Print the valid entry*/
    rtk_l2_addr_table_t l2_entry; 
    for (i=1;i<=2112;i++)
    {
        memset(&l2_entry,0,sizeof(rtk_l2_addr_table_t));
        l2_entry.index = i;
        ret = rtk_l2_entry_get(&l2_entry);
        if (ret==RT_ERR_OK)
        {
            if(l2_entry.is_ipmul)
            {
                
                len += sprintf(page+len, "\r\nIndex SourceIP DestinationIP MemberPort State\n");           
                len += sprintf(page+len, "%4d ", l2_entry.index);
                len += sprintf(page+len,"%0x ",(l2_entry.sip));
                len += sprintf(page+len,"%0x ",(l2_entry.dip));
                len += sprintf(page+len,"%-8x ",l2_entry.portmask);
                len += sprintf(page+len,"%s \n",(l2_entry.is_static? "Static" : "Auto"));
            }
            else if(l2_entry.mac.octet[0]&0x01)
            {
                len += sprintf(page+len,"%4d %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X %-8x %-4d %-4s %-5s %s %d\n",
                l2_entry.index,
                l2_entry.mac.octet[0],l2_entry.mac.octet[1],l2_entry.mac.octet[2],
                l2_entry.mac.octet[3],l2_entry.mac.octet[4],l2_entry.mac.octet[5],
                l2_entry.portmask, l2_entry.fid, (l2_entry.auth ? "Auth" : "x"),
                (l2_entry.sa_block? "Block" : "x"), (l2_entry.is_static? "Static" : "Auto"),
                l2_entry.age);
            }
            else if((l2_entry.age!=0)||(l2_entry.is_static==1))
            {
                len += sprintf(page+len,"%4d %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X %-8x %-4d %-4s %-5s %s %d\n",
                l2_entry.index,l2_entry.mac.octet[0],l2_entry.mac.octet[1],l2_entry.mac.octet[2],
                l2_entry.mac.octet[3],l2_entry.mac.octet[4],l2_entry.mac.octet[5],
                l2_entry.portmask, l2_entry.fid, (l2_entry.auth ? "Auth" : "x"),
                (l2_entry.sa_block? "Block" : "x"), (l2_entry.is_static? "Static" : "Auto"),
                l2_entry.age);
            }
        }
    }

     
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count)
		len = count;
	if (len<0)
	  	len = 0;

	return len;
}
#endif
#endif


#if defined(CONFIG_RTL_DNS_TRAP)
int rtl_8367_add_acl_for_dns(unsigned int acl_idx)
{
	int retVal; //, acl_idx=0;
	rtk_filter_field_t	filter_field[2];
	rtk_filter_cfg_t	cfg;
	rtk_filter_action_t	act;
	rtk_filter_number_t	ruleNum = 0;

	/* disable cpu port's mac addr learning ability */
	//rtl8367b_setAsicLutLearnLimitNo(r8367_cpu_port,0);

	/* disable unknown unicast/mcast/bcast flooding between LAN ports */
	//smi_write(RTL8367B_REG_UNDA_FLOODING_PMSK, BIT(r8367_cpu_port));

    memset(&filter_field, 0, 2*sizeof(rtk_filter_field_t));
    memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
    memset(&act, 0, sizeof(rtk_filter_action_t));
    /*Search all MAC (data & mask are all "0")*/
    filter_field[0].fieldType = FILTER_FIELD_UDP_DPORT;                    
    filter_field[0].filter_pattern_union.udpDstPort.dataType = FILTER_FIELD_DATA_MASK;
    filter_field[0].filter_pattern_union.udpDstPort.value = 53;
    filter_field[0].filter_pattern_union.udpDstPort.mask = 0xFFFF;
    filter_field[0].next = NULL;        
    if ((retVal = rtk_filter_igrAcl_field_add(&cfg, &filter_field[0])) != RT_ERR_OK)
       return RT_ERR_FAILED;   

    cfg.careTag.tagType[CARE_TAG_UDP].value = TRUE;
    cfg.careTag.tagType[CARE_TAG_UDP].mask = TRUE;
    /*Add port0~port4 to active ports*/
    cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
    cfg.activeport.value = 0x1F;
    cfg.activeport.mask = 0xFF; 
    cfg.invert =FALSE;         
    /*Set Action to Trap to CPU*/
    act.actEnable[FILTER_ENACT_TRAP_CPU] = TRUE;        
    if ((retVal = rtk_filter_igrAcl_cfg_add(acl_idx, &cfg, &act, &ruleNum)) != RT_ERR_OK)
        return RT_ERR_FAILED;

    return RT_ERR_OK;			
}
int rtl_8367_remove_acl_for_dns(unsigned int acl_idx)
{
	rtk_filter_igrAcl_cfg_del(acl_idx);
}
#endif
#if defined (IMPROVE_MCAST_PERFORMANCE_WITH_RTL8367)
int rtl_initMcastImprove(void)
{
	return rtk_qos_init(1);
}
int rtl865x_enableRtl8367McastPriorityAcl(int priority)
{
	int retVal;
	rtk_filter_field_t	filter_field[2];
	rtk_filter_cfg_t	cfg;
	rtk_filter_action_t	act;
	rtk_filter_number_t	ruleNum = 0;

	
	memset(filter_field, 0, 2*sizeof(rtk_filter_field_t));
	memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
	memset(&act, 0, sizeof(rtk_filter_action_t));

	filter_field[0].fieldType = FILTER_FIELD_DMAC;
	filter_field[0].filter_pattern_union.dmac.dataType = FILTER_FIELD_DATA_MASK;
	filter_field[0].filter_pattern_union.dmac.value.octet[0] = 0x01;
	filter_field[0].filter_pattern_union.dmac.value.octet[1] = 0x00;
	filter_field[0].filter_pattern_union.dmac.value.octet[2] = 0x5e;
	filter_field[0].filter_pattern_union.dmac.mask.octet[0] = 0xFF;
	filter_field[0].filter_pattern_union.dmac.mask.octet[1] = 0xFF;
	filter_field[0].filter_pattern_union.dmac.mask.octet[1] = 0xFF;
	filter_field[0].next = NULL;
	if ((retVal = rtk_filter_igrAcl_field_add(&cfg, &filter_field[0])) != RT_ERR_OK)
		return retVal;
	
	
	cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
	cfg.activeport.value = 0x1F;
	cfg.activeport.mask = 0xFF;
	cfg.invert = FALSE;

	act.actEnable[FILTER_ENACT_PRIORITY] = TRUE;
	act.filterPriority = priority;

	if ((retVal = rtk_filter_igrAcl_cfg_add(1, &cfg, &act, &ruleNum)) != RT_ERR_OK)
        return retVal;

	return RT_ERR_OK;

}
int rtl865x_disableRtl8367McastPriorityAcl(void)
{
	return rtk_filter_igrAcl_cfg_del(1);
}

int rtl_enable_mCast_improve(int enable)
{
	if(enable)
	{
		rtk_qos_queueNum_set(r8367_cpu_port, 2);
		rtl865x_enableRtl8367McastPriorityAcl(7);
	}
	else
	{
		rtk_qos_queueNum_set(r8367_cpu_port, 1);
		rtl865x_disableRtl8367McastPriorityAcl();
	}
	return 0;
}
#endif

#if defined CONFIG_RTL_8367_QOS_SUPPORT || defined CONFIG_RTL_8367_QOS_TEST
#define MAX_PHY_PORT_NUM 8
#if defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F)
#define MAX_RTL8367_QOS_QUEUE_NUM 8
#else
#define MAX_RTL8367_QOS_QUEUE_NUM 6
#endif
#define	QOS_VALID_MASK	0x2
#define	QOS_TYPE_MASK		0x1
#define	QOS_TYPE_STR		0x0	/*0x0|QOS_VALID_MASK*/
#define	QOS_TYPE_WFQ		0x1	/*0x1|QOS_VALID_MASK*/
#if defined CONFIG_RTL_8367_QOS_SUPPORT
#define CPU_METER_ID1        16
#define CPU_METER_ID2        17
#define UNKNOWN_OWNER_METER  0
#define WAN_METER            1
#define LAN_METER            2
#define CPU_METER            3
int qos_meter_owner[RTL8367B_METERNO];
#endif

#if defined CONFIG_RTL_8367_QOS_SUPPORT || defined CONFIG_RTL_8367_QOS_TEST
#if defined CONFIG_RTL_PROC_NEW
int rtl_8367QosReadProc(struct seq_file *s, void *v)
{
	int port, queueNum, i, queue;	
	rtk_qos_queue_weights_t qWeights;
	rtk_meter_id_t meterId;
	rtk_rate_t qRate, pRate;
	rtk_enable_t qIfg_include, pIfg_include, qEnable;
	rtk_uint32 Bucket_size;
    rtk_port_mac_ability_t mac_cfg;
    rtk_mode_ext_t mode ;
	rtk_rate_t inRate;
	rtk_enable_t inIfg_include, inFc_enable;

    rtk_port_macForceLinkExt_get(EXT_PORT_1,&mode,&mac_cfg);

	seq_printf(s, "%s\n", "rtl8367 cpu flow control");
	seq_printf(s, "\ttx pause:%d\n", mac_cfg.txpause);
	seq_printf(s, "\trx pause:%d\n", mac_cfg.rxpause);

	seq_printf(s, "%s\n", "rtl8367 qos related parameters");

	for(port=0; port<RTK_PORT_ID_MAX; port++)
	{
		rtk_qos_queueNum_get(port,&queueNum);
		rtk_rate_egrBandwidthCtrlRate_get(port, &pRate, &pIfg_include); 

		rtk_qos_schedulingQueue_get(port, &qWeights);
		rtk_rate_egrQueueBwCtrlEnable_get(port, RTK_WHOLE_SYSTEM, &qEnable); 

		seq_printf(s,"<%d> queueNum:%d portRate:%d Ifg include:%d queueBwCtrl:%s\n", 
			port, queueNum, pRate, pIfg_include, (qEnable==ENABLED)?"enabled":"disable");

		rtk_rate_igrBandwidthCtrlRate_get(port, &inRate, &inIfg_include, &inFc_enable);
		seq_printf(s,"\tingressBw:%d Ifg include:%d flowControl:%d\n", 
			inRate, inIfg_include, inFc_enable);
		
		seq_printf(s,"\tQueue Parameters:\n ");
		for(queue=0; queue<RTK_MAX_NUM_OF_QUEUE; queue++)
		{
			rtk_rate_egrQueueBwCtrlRate_get(port, queue, &meterId); 
			 
			seq_printf(s,"\t[%d] type:%s, weight:%d, MeterId:%d\n", 
				queue, (qWeights.weights[queue]==0)?"SP":"WFQ", qWeights.weights[queue], meterId);
		}

		seq_printf(s,"\tMeter Parameters:\n ");
		if(port<4)
		{
			for(i=port*8; i<=port*8+7; i++)
			{			
				rtk_rate_shareMeter_get(i, &qRate, &qIfg_include);
				rtk_rate_shareMeterBucket_get(i,&Bucket_size);
				if(i==port*8)
					seq_printf(s,"\t");
				if(i==port*8+4)
					seq_printf(s,"\n\t");
				seq_printf(s,"[%d]%d,%d,%d ", i, qRate, qIfg_include,Bucket_size);
			}
			seq_printf(s,"\n");
		}
		else
		{
			for(i=(port-4)*8; i<=(port-4)*8+7; i++)
			{	
				if(i==(port-4)*8)
					seq_printf(s,"\t");
				if(i==(port-4)*8+4)
					seq_printf(s,"\n\t");
				rtk_rate_shareMeter_get(i, &qRate, &qIfg_include);
				rtk_rate_shareMeterBucket_get(i,&Bucket_size);
				seq_printf(s,"[%d]%d,%d,%d ", i, qRate, qIfg_include, Bucket_size);
			}
			seq_printf(s,"\n");
		}

	}
	return 0;
}
#else
int rtl_8367QosReadProc(char *page, char **start, off_t off, int count, int *eof, void *data)	
{
	int len;
	
	int port, queueNum, i, queue;	
	rtk_qos_queue_weights_t qWeights;
	rtk_meter_id_t meterId;
	rtk_rate_t qRate, pRate;
	rtk_enable_t qIfg_include, pIfg_include, qEnable;
	rtk_uint32 Bucket_size;
	rtk_port_mac_ability_t mac_cfg;
    rtk_mode_ext_t mode ;
	rtk_rate_t inRate;
	rtk_enable_t inIfg_include, inFc_enable;

    rtk_port_macForceLinkExt_get(EXT_PORT_1,&mode,&mac_cfg);

	len = sprintf(page, "%s\n", "rtl8367 cpu flow control");
	len += sprintf(page+len, "\ttx pause:%d\n", mac_cfg.txpause);
	len += sprintf(page+len, "\trx pause:%d\n", mac_cfg.rxpause);
	len += sprintf(page+len, "%s\n", "rtl8367 qos related parameters");

	for(port=0; port<RTK_PORT_ID_MAX; port++)
	{
		rtk_qos_queueNum_get(port,&queueNum);
		rtk_rate_egrBandwidthCtrlRate_get(port, &pRate, &pIfg_include); 

		rtk_qos_schedulingQueue_get(port, &qWeights);
		rtk_rate_egrQueueBwCtrlEnable_get(port, RTK_WHOLE_SYSTEM, &qEnable); 

		len += sprintf(page+len,"<%d> queueNum:%d portRate:%d Ifg include:%d queueBwCtrl:%s\n", 
			port, queueNum, pRate, pIfg_include, (qEnable==ENABLED)?"enabled":"disable");
		rtk_rate_igrBandwidthCtrlRate_get(port, &inRate, &inIfg_include, &inFc_enable);
		len += sprintf(page+len,"\tingressBw:%d Ifg include:%d flowControl:%d\n", 
			inRate, inIfg_include, inFc_enable);

		len += sprintf(page+len,"\tQueue Parameters:\n ");
		for(queue=0; queue<RTK_MAX_NUM_OF_QUEUE; queue++)
		{
			rtk_rate_egrQueueBwCtrlRate_get(port, queue, &meterId); 
			 
			len += sprintf(page+len,"\t[%d] type:%s, weight:%d, MeterId:%d\n", 
				queue, (qWeights.weights[queue]==0)?"SP":"WFQ", qWeights.weights[queue], meterId);
		}

		len += sprintf(page+len,"\tMeter Parameters:\n ");
		if(port<4)
		{
			for(i=port*8; i<=port*8+7; i++)
			{			
				rtk_rate_shareMeter_get(i, &qRate, &qIfg_include);
				rtk_rate_shareMeterBucket_get(i,&Bucket_size);
				if(i==port*8)
					len += sprintf(page+len,"\t");
				if(i==port*8+4)
					len += sprintf(page+len,"\n\t");
				len += sprintf(page+len,"[%d]%d,%d,%d ", i, qRate, qIfg_include,Bucket_size);
			}
			len += sprintf(page+len,"\n");
		}
		else
		{
			for(i=(port-4)*8; i<=(port-4)*8+7; i++)
			{	
				if(i==(port-4)*8)
					len += sprintf(page+len,"\t");				
				if(i==(port-4)*8+4)
					len += sprintf(page+len,"\n\t");
				rtk_rate_shareMeter_get(i, &qRate, &qIfg_include);
				rtk_rate_shareMeterBucket_get(i,&Bucket_size);
				len += sprintf(page+len,"[%d]%d,%d,%d ", i, qRate, qIfg_include, Bucket_size);
			}
			len += sprintf(page+len,"\n");
		}

	}
				
	if (len <= off+count)
		*eof = 1;
			
	*start = page + off;
		len -= off;
			
	if (len>count)
		len = count;
			
	if (len<0) len = 0;
			
	return len;
}
#endif

int rtl_8367QosWriteProc(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
#if 0
	char tmp[256];
		
	char		*strptr, *cmd_addr;
	char		*tokptr;
	int port, qid, page;
	unsigned int queue_page[RTK_MAX_NUM_OF_QUEUE];

	if (count < 2)
		return -EFAULT;
	
	if (buffer && !copy_from_user(tmp, buffer, count)) {
	
		tmp[count] = '\0';
		strptr=tmp;
		tokptr = strsep(&strptr," ");
		if (tokptr==NULL)
		{
			goto errout;
		}
		
		if (!memcmp(tokptr, "current", 7))
		{				
			printk( "Current Page for Egress Port and Queues\n");
			printk( "PortNo.  Port	 Q0 	Q1	   Q2	  Q3	 Q4 	Q5	   Q6	  Q7\n");
			for(port=0; port<RTK_PORT_ID_MAX; port++)
			{
				/*Egress Port page number*/
				rtl8367b_setAsicReg(RTL8367B_REG_FLOWCTRL_DEBUG_CTRL0,port);
				rtl8367b_getAsicReg(RTL8367B_REG_FLOWCTRL_PORT_PAGE_COUNT,&page);
				
				for(qid=0;qid<=7;qid++) 				   
				{						 
					rtl8367b_getAsicReg(RTL8367B_REG_FLOWCTRL_QUEUE0_PAGE_COUNT+qid,&queue_page[qid]);
				}
		
				printk("%2d		 %4d  %4d	%4d   %4d	%4d   %4d	%4d   %4d	%4d\n", port+1,page,
				queue_page[0],queue_page[1],queue_page[2],queue_page[3],queue_page[4],queue_page[5],queue_page[6],queue_page[7]);
			}
			
		}
		else if(!memcpy(tokptr, "max",3))
		{
			printk( "Maximum Page for Egress Port and Queues\n");
			printk( "PortNo.  Port	 Q0 	Q1	   Q2	  Q3	 Q4 	Q5	   Q6	  Q7\n");
			for(port=0; port<RTK_PORT_ID_MAX; port++)
			{
				/*Egress Port page number*/
				rtl8367b_setAsicReg(RTL8367B_REG_FLOWCTRL_DEBUG_CTRL0,port);
				rtl8367b_getAsicReg(RTL8367B_REG_FLOWCTRL_PORT_MAX_PAGE_COUNT,&page);
				 
				for(qid=0;qid<=7;qid++) 				   
				{						 
					rtl8367b_getAsicReg(RTL8367B_REG_FLOWCTRL_QUEUE0_MAX_PAGE_COUNT+qid,&queue_page[qid]);
				}
		
				printk("%2d		 %4d  %4d	%4d   %4d	%4d   %4d	%4d   %4d	%4d\n", port+1,page,
				queue_page[0],queue_page[1],queue_page[2],queue_page[3],queue_page[4],queue_page[5],queue_page[6],queue_page[7]);
		
			}
		}
		else
		{
errout:
			printk("error input!\n");
		}
	}
#endif
	return count;	
}
int rtl8367_qos_init()
{
    rtk_uint32 priority;
    rtk_api_ret_t retVal;
	rtk_queue_num_t queueNum;
	rtk_uint32 priDec;
	int i;

	//the priority to qid matrix should be equal to 97F/9xd...
	//change this matrix can't work!!!!??
	#if 0
	CONST_T rtk_uint16 g_prioritytToQid[8][8]= {
		{0,0,0,0,0,0,0,0}, 
		{0,0,0,0,5,5,5,5},	
		{0,0,0,0,1,1,5,5},
		{0,0,0,1,2,2,5,5},
		{0,0,0,1,2,3,5,5},
		{0,0,1,2,3,4,5,5},
		{0,0,1,2,3,4,5,6},
		{0,1,2,3,4,5,6,7}
		};
	#endif

	//802.1p priority based should be heighest priority
    CONST_T rtk_uint32 g_priorityDecision[8] = {0x01, 0x80,0x04,0x80,0x20,0x40,0x10,0x08};

	rtk_qos_init(1);
	#if 0
	 /*Set Priority to Qid*/
	for(queueNum = 0; priority <RTK_MAX_NUM_OF_QUEUE; priority++)
	{
	   	for (priority = 0; priority <= RTL8367B_PRIMAX; priority++)
	   	{
	      	if ((retVal = rtl8367b_setAsicPriorityToQIDMappingTable(queueNum, priority, g_prioritytToQid[queueNum][priority])) != RT_ERR_OK)
	         	return retVal;
	  	}

	}
	#endif

	/*Change Priority Decision Order*/
    for (priDec = 0;priDec < PRIDEC_END;priDec++)
    {
        if ((retVal = rtl8367b_setAsicPriorityDecision(priDec, g_priorityDecision[priDec])) != RT_ERR_OK)
            return retVal;
    }

	for(i=0; i<RTL8367B_METERNO; i++)
	{
		if(i==CPU_METER_ID1 || i==CPU_METER_ID2)
			qos_meter_owner[i] = CPU_METER;
		else
			qos_meter_owner[i] = UNKNOWN_OWNER_METER;
	}
}
#endif

#if defined CONFIG_RTL_8367_QOS_TEST
extern void rtl865x_qos_set(void);
int rtl8367_qos_test()
{
	int port, qid;
	rtk_priority_select_t PriDec;
	rtk_qos_queue_weights_t qweights;
	int prio;

	//97D 
	rtl865x_qos_set();
	
	//init qos
	rtl8367_qos_init();

	//set wan port queue num=2
	rtk_qos_queueNum_set(RTL8367B_PORT4_ENABLE_OFFSET, 2);
	//rtk_qos_queueNum_set(r8367_cpu_port, 2);

	//set port priority
	#if 0
	rtk_qos_portPri_set(0,0);
	rtk_qos_portPri_set(1,0);
	rtk_qos_portPri_set(2,7);
	rtk_qos_portPri_set(3,7);
	#endif

	//set queue parameter on CPU port
	//strict priority
	for (qid = 0; qid < RTK_MAX_NUM_OF_QUEUE; qid ++)
	{
		if(qid==0 || qid==7)
			qweights.weights[qid] = 0;
		else
			qweights.weights[qid] = 0;
	}
	#if 1
	rtk_qos_schedulingQueue_set(RTL8367B_PORT4_ENABLE_OFFSET,&qweights);	
	//rtk_rate_egrBandwidthCtrlRate_set(RTL8367B_PORT4_ENABLE_OFFSET, 204800, 1);
	
	//set queue 0 rate to 10M and queue 7 rate to 5M
	rtk_rate_shareMeter_set(0, 15360, ENABLE);
	rtk_rate_shareMeter_set(1, 10240, ENABLE);
	rtk_rate_shareMeterBucket_set(0,10000);
	rtk_rate_shareMeterBucket_set(1,30000);
	rtk_rate_egrQueueBwCtrlEnable_set(RTL8367B_PORT4_ENABLE_OFFSET,0xFF,ENABLE);
	rtk_rate_egrQueueBwCtrlRate_set(RTL8367B_PORT4_ENABLE_OFFSET,0,0);
	rtk_rate_egrQueueBwCtrlRate_set(RTL8367B_PORT4_ENABLE_OFFSET,7,1);
	#else
	rtk_qos_schedulingQueue_set(r8367_cpu_port,&qweights);	
	rtk_rate_egrBandwidthCtrlRate_set(r8367_cpu_port, 204800, 1);
	
	//set queue 0 rate to 10M and queue 7 rate to 5M
	rtk_rate_shareMeter_set(16, 204800, ENABLE);
	rtk_rate_shareMeter_set(17, 153600, ENABLE);
	rtk_rate_shareMeterBucket_set(16,10000);
	rtk_rate_shareMeterBucket_set(17,30000);
	rtk_rate_egrQueueBwCtrlEnable_set(r8367_cpu_port,0xFF,ENABLE);
	rtk_rate_egrQueueBwCtrlRate_set(r8367_cpu_port,0,16);
	rtk_rate_egrQueueBwCtrlRate_set(r8367_cpu_port,7,17);
	#endif

}
#endif

#if defined CONFIG_RTL_8367_QOS_SUPPORT
int rtl8367_qosSetIngressBandwidth(unsigned int memberPort, unsigned int Kbps)
{
	unsigned int	port;

	//todo: wait output queue empty?
	for(port=0;port<MAX_PHY_PORT_NUM;port++)
	{
		if(((1<<port)&memberPort)==0)
			continue;
		if(Kbps==0)
			rtk_rate_igrBandwidthCtrlRate_set(port, RTL8367B_QOS_RATE_INPUT_MAX, DISABLED, ENABLED);
		else
			rtk_rate_igrBandwidthCtrlRate_set(port, Kbps, ENABLED, ENABLED);		
	}
	return 0;

}

int rtl8367_qosSetBandwidth(unsigned int memberPort, unsigned int Kbps)
{
	unsigned int	port;
	int ret;

	//todo: wait output queue empty?
	for(port=0;port<MAX_PHY_PORT_NUM;port++)
	{
		if(((1<<port)&memberPort)==0)
			continue;
		ret=rtk_rate_egrBandwidthCtrlRate_set(port, Kbps, ENABLED);
	}
	return 0;
}
int rtl8367_qosFlushBandwidth(unsigned int memberPort)
{
	unsigned int port;

	//todo: wait output queue empty?
	for(port=0;port<MAX_PHY_PORT_NUM;port++)
	{
		if(((1<<port)&memberPort)==0)
			continue;
		rtk_rate_egrBandwidthCtrlRate_set(port, RTL8367B_QOS_RATE_INPUT_MAX, ENABLED);
	}
	return 0;
}

int rtl8367_qosGetMeterId(unsigned int port, unsigned int bw, unsigned int wanPortMask)
{
	rtk_meter_id_t idbegin, idend;
	rtk_meter_id_t i;
	rtk_rate_t Rate;
	rtk_data_t Ifg_include;
	
	if(port<0 || port>=MAX_PHY_PORT_NUM)
		return -1;

	if(port<4)
	{
		idbegin = port*8;
		idend = port*8+7;
	}
	else
	{
		idbegin = (port-4)*8;
		idend = (port-4)*8+7;
	}

	for(i=idbegin; i<=idend; i++)
	{
		if(qos_meter_owner[i] == UNKNOWN_OWNER_METER)
			break;
	}
	if(i > idend)
	{
		return -1;
	}

	//printk("get meter of port %d, id:%d, bw:%d, [%s:%d]\n", port, i, bw, __FUNCTION__, __LINE__);
	if(wanPortMask & (1<<port))
	{
		qos_meter_owner[i]=WAN_METER;
	}
	else
	{
		qos_meter_owner[i]=LAN_METER;
	}

	rtk_rate_shareMeter_set(i, bw, ENABLED);
	//rtk_rate_shareMeterBucket_set(j,QOS_BUCKET_SIZE);

	return i;
		
}
//ok
int rtl8367_qosFlushMeter(int port, unsigned int wanPortMask)
{
	int meterId,idbegin,idend;

	if(port<0 || port>=MAX_PHY_PORT_NUM)
		return -1;
	
	if(port<4)
	{
		idbegin = port*8;
		idend = port*8+7;
	}
	else
	{
		idbegin = (port-4)*8;
		idend = (port-4)*8+7;
	}

	//printk("flush meter of port %d, [%s:%d]\n", port, __FUNCTION__, __LINE__);

	for(meterId=idbegin; meterId<=idend; meterId++)
	{
		if(((wanPortMask & (1<<port)) && (qos_meter_owner[meterId]==WAN_METER)) 
			||(!(wanPortMask & (1<<port)) && (qos_meter_owner[meterId]==LAN_METER)))
		{
			rtk_rate_shareMeter_set(meterId, RTL8367B_QOS_RATE_INPUT_MAX, DISABLED);
			qos_meter_owner[meterId]=UNKNOWN_OWNER_METER;
			//always use default bucket size
			//rtk_rate_shareMeterBucket_set(meterId,DEFAULT_QOS_BUCKET_SIZE);
		}
	}
	return 0;
}

int rtl8367_qosProcessQueue(unsigned int memberPort, unsigned int queueNum, unsigned int *queueFlag, unsigned int *queueId, unsigned int *queueBw, unsigned int *queueWeight, unsigned int wanPortMask)
{
	unsigned int port, queue, qid, meterId;
	rtk_qos_queue_weights_t qweights;
	unsigned int set_error=0;
	unsigned int all_phyPortMask = 0x1F;

	memset(&qweights, 0, sizeof(qweights));
	
	for(port = 0; port < MAX_PHY_PORT_NUM; port++)
	{
		if(((1<<port)&memberPort)==0)
			continue;
		rtk_qos_queueNum_set(port, queueNum);
		rtk_rate_egrQueueBwCtrlEnable_set(port,RTK_WHOLE_SYSTEM, ENABLED);
		
		for (queue=0;queue<MAX_RTL8367_QOS_QUEUE_NUM;queue++)
		{
			if((queueFlag[queue]&QOS_VALID_MASK)==0 || queueBw[queue] == 0)
				continue;

			qid = queueId[queue];
			if((queueFlag[queue]&QOS_TYPE_MASK)==QOS_TYPE_STR)
			{
				qweights.weights[qid] = 0;
			}
			else
			{
				qweights.weights[qid] = queueWeight[queue];
			}

			meterId = rtl8367_qosGetMeterId(port, queueBw[queue], wanPortMask);

			if(meterId != -1)
			{
				rtk_rate_egrQueueBwCtrlRate_set(port,qid,meterId);
			}
			else
			{
				set_error = 1;
			}
				
		}
		rtk_qos_schedulingQueue_set(port,&qweights);
	}
	if(set_error)
	{
		printk("sharemeter is not enough!\n");
		rtl8367_qosFlushBandwidth(all_phyPortMask);
		rtl8367_closeQos(all_phyPortMask, wanPortMask);
	}
	return 0;
}
//ok
int rtl8367_closeQos(unsigned int memberPort, unsigned int wanPortMask)
{
	unsigned int port, qid;	
	rtk_qos_queue_weights_t qweights;

	//todo: wait output queue empty?
	for(port=0;port<MAX_PHY_PORT_NUM;port++)
	{
		if(((1<<port)&memberPort)==0)
			continue;

		
		for (qid = 0; qid < RTK_MAX_NUM_OF_QUEUE; qid ++)
		{
			qweights.weights[qid] = 0;
			if(port<4)
				rtk_rate_egrQueueBwCtrlRate_set(port, qid, port*8);
			else
				rtk_rate_egrQueueBwCtrlRate_set(port, qid, (port-4)*8);
		}

		rtk_qos_queueNum_set(port, 1);
		rtk_qos_schedulingQueue_set(port,&qweights);		
		rtk_rate_egrQueueBwCtrlEnable_set(port,RTK_WHOLE_SYSTEM,DISABLED);
		rtl8367_qosFlushMeter(port, wanPortMask);
	}

	//close 8367 cpu port flow control
	

	return 0;
	
}
void rtl8367_setFlowControl(int qosEnable)
{
    /* Set external interface 0 to RGMII with Force mode, 1000M, Full-duple, enable TX&RX pause*/
    rtk_port_mac_ability_t mac_cfg;
    rtk_mode_ext_t mode ;

    rtk_port_macForceLinkExt_get(EXT_PORT_1,&mode,&mac_cfg);
	if(((mac_cfg.txpause==ENABLED) || (mac_cfg.rxpause==ENABLED)) && qosEnable==1)
	{
		mac_cfg.txpause = DISABLED;
		mac_cfg.rxpause = DISABLED;
		rtk_port_macForceLinkExt_set(EXT_PORT_1,mode,&mac_cfg);
	}
	else if(((mac_cfg.txpause==DISABLED) || (mac_cfg.rxpause==DISABLED)) && qosEnable==0)
	{
		mac_cfg.txpause = ENABLED;
		mac_cfg.rxpause = ENABLED;		
		rtk_port_macForceLinkExt_set(EXT_PORT_1,mode,&mac_cfg);
	}
}
#endif
#endif

#endif

#ifdef CONFIG_EFM_PATCH
void rtk_set_portled_all(int flag)
{
	rtl8309n_led_cpu_set(RTL8309N_LED_GROUP_0, flag?0x0:0xFF, 0x0);
}
#endif

