#include <stdio.h>
#include "web_voip.h"

static char cid_det[CID_MAX][25] = {
	"FSK_BELLCORE", "FSK_ETSI", "FSK_BT", "FSK_NTT", "DTMF"
};

char hookflash_type[2][20] = {"NONE", "SIP_INFO"};

static char pulse_pps[PULSE_GEN_PPS_MAX][25] = {"10 PPS", "20 PPS"};

int voip_char_replace(const char *src, char old_char, char new_char, char *result)
{
	int i;

	for (i=0; src[i]; i++)
	{
		if (src[i] == old_char)
			result[i] = new_char;
		else
			result[i] = src[i];
	}

	result[i] = 0;
	return 0;
}

#ifdef CONFIG_APP_BOA
int asp_voip_OtherGet(webs_t wp, int argc, char_t **argv)
#else
int asp_voip_OtherGet(int ejid, webs_t wp, int argc, char_t **argv)
#endif
{
	voipCfgParam_t *pVoIPCfg;
	voipCfgPortParam_t *pCfg1;
		
	char key_display[FUNC_KEY_LENGTH];
#ifdef SUPPORT_DSCP
	int i;
#endif

	if (web_flash_get(&pVoIPCfg) != 0)
		return -1;

	pCfg1 = &pVoIPCfg->ports[0];
	
	if (strcmp(argv[0], "funckey_transfer")==0)
	{
		voip_char_replace(pVoIPCfg->funckey_transfer, '.', '*', key_display);
		websWrite(wp, "%s", key_display);
	}
	else if (strcmp(argv[0], "funckey_pstn")==0)
	{
		voip_char_replace(pVoIPCfg->funckey_pstn, '.', '*', key_display);
		websWrite(wp, "%s", key_display);
	}
	else if (strcmp(argv[0], "display_funckey_pstn")==0)
	{
#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
		websWrite(wp, "%s", "");
#else
		websWrite(wp, "%s", "style=\"display:none\"");
#endif
	}
	else if (strcmp(argv[0], "auto_dial_display_title") == 0)
	{
#ifdef CONFIG_RTK_VOIP_IP_PHONE
		/* nothing */
#else
		websWrite(wp, "<p>\n<b>Dial Option</b>\n");
#endif
	}
	else if (strcmp(argv[0], "auto_dial_display") == 0)
	{
#ifdef CONFIG_RTK_VOIP_IP_PHONE
		websWrite(wp, "style=\"display:none\"");
#endif
	}
	else if (strcmp(argv[0], "auto_dial")==0)
	{
		websWrite(wp, "%d", pVoIPCfg->auto_dial & AUTO_DIAL_TIME);
	}
	else if (strcmp(argv[0], "auto_dial_always") == 0)
	{
		websWrite(wp, "%s", pVoIPCfg->auto_dial & AUTO_DIAL_ALWAYS ? "checked" : "");
	}
	else if (strcmp(argv[0], "auto_bypass_relay")==0)
	{
		// PSTN Relay - Auto bypass relay (Switch to PSTN if VoIP failed)
		websWrite(wp, "%s", pVoIPCfg->auto_bypass_relay & AUTO_BYPASS_RELAY_ENABLE ? "checked" : "");
	}
	else if (strcmp(argv[0], "auto_bypass_warning")==0)
	{
		websWrite(wp, "%s", pVoIPCfg->auto_bypass_relay & AUTO_BYPASS_WARNING_ENABLE ? "checked" : "");
	}
	else if (strcmp(argv[0], "off_hook_alarm_display_title") == 0)
	{
#ifdef CONFIG_RTK_VOIP_IP_PHONE
		/* nothing */
#else
		websWrite(wp, "<p>\n<b>Off-Hook Alarm</b>\n");
#endif
	}
	else if (strcmp(argv[0], "off_hook_alarm_display") == 0)
	{
#ifdef CONFIG_RTK_VOIP_IP_PHONE
		websWrite(wp, "style=\"display:none\"");
#endif
	}
	else if (strcmp(argv[0], "off_hook_alarm")==0)
	{
		websWrite(wp, "%d", pVoIPCfg->off_hook_alarm);
	}
	// VoIP to PSTN - one stage dialing
	else if (strcmp(argv[0], "one_stage_dial")==0)
		websWrite(wp, "%s", (pVoIPCfg->one_stage_dial) ? "checked" : "");
	// VoIP to PSTN - two stage dialing
	else if (strcmp(argv[0], "two_stage_dial")==0)
		websWrite(wp, "%s", (pVoIPCfg->two_stage_dial) ? "checked" : "");
	// PSTN to VoIP - Caller ID Detection
	else if (strcmp(argv[0], "caller_id_auto_det")==0)
	{
		websWrite(wp, "<input type=\"radio\" name=\"caller_id_auto_det\" value=0 onClick=enable_cid_det_mode() %s>Off",
			pVoIPCfg->cid_auto_det_select == 0 ? "checked" : "");
		websWrite(wp, "<input type=\"radio\" name=\"caller_id_auto_det\" value=1 onClick=enable_cid_det_mode() %s>On (NTT Support)",
			pVoIPCfg->cid_auto_det_select == 1 ? "checked" : "");
   		websWrite(wp, "<input type=\"radio\" name=\"caller_id_auto_det\" value=2 onClick=enable_cid_det_mode() %s>On (NTT Not Support)",
			pVoIPCfg->cid_auto_det_select == 2 ? "checked" : "");
	}
	else if (strcmp(argv[0], "caller_id_det")==0)
	{	
		websWrite(wp, "<select name=caller_id_det %s>", 
			pVoIPCfg->cid_auto_det_select != 0 ? "disabled" : "");

		for (i=0; i<CID_MAX ;i++)
		{
			if (i == pVoIPCfg->caller_id_det_mode)
				websWrite(wp, "<option selected>%s</option>", cid_det[i]);
			else
				websWrite(wp, "<option>%s</option>", cid_det[i]);
		}

		websWrite(wp, "</select>");
	}
	else if (strcmp(argv[0], "pulse_dial_generation")==0)
	{
		websWrite(wp, "<input type=\"radio\" name=\"pulse_dial_generation\" value=0 %s>Disable",
    				( pVoIPCfg ->pulse_dial_gen == 0 ? "checked" : "" ) );
		websWrite(wp, "<input type=\"radio\" name=\"pulse_dial_generation\" value=1 %s>Enable",
    				( pVoIPCfg ->pulse_dial_gen == 1 ? "checked" : "" ) );
	}
	else if (strcmp(argv[0], "pulse_gen_PPS")==0)
	{	
		for (i=0; i<PULSE_GEN_PPS_MAX ;i++)
		{
			if (((i+1)*10) == pVoIPCfg->pulse_gen_pps)
				websWrite(wp, "<option selected>%s</option>", pulse_pps[i]);
			else
				websWrite(wp, "<option>%s</option>", pulse_pps[i]);
		}
	}
	else if (strcmp(argv[0], "pulse_gen_Make")==0)
	{	
		websWrite(wp, "%d", pVoIPCfg->pulse_gen_make_time);
	}
	else if (strcmp(argv[0], "pulse_gen_Pause")==0)
	{
		websWrite(wp, "%d", pVoIPCfg->pulse_gen_interdigit_pause);
	}
	else if (strcmp(argv[0], "pulse_dial_detection")==0)
	{
		websWrite(wp, "<input type=\"radio\" name=\"pulse_dial_detection\" value=0 %s>Disable",
    				( pVoIPCfg ->pulse_dial_det == 0 ? "checked" : "" ) );
		websWrite(wp, "<input type=\"radio\" name=\"pulse_dial_detection\" value=1 %s>Enable",
    				( pVoIPCfg ->pulse_dial_det == 1 ? "checked" : "" ) );
	}
	else if (strcmp(argv[0], "sip_option_flag")==0)
	{
		i=pVoIPCfg->rfc_flags & SIP_OPTION;
		printf("aa sip_option_flag is %d\n",i);

		
		websWrite(wp, "<input type=\"radio\" name=\"sip_option_flag\" value=0 %s>Disable",
					( i  ? "" : "checked" ) );
		websWrite(wp, "<input type=\"radio\" name=\"sip_option_flag\" value=1 %s>Enable",
					( i ? "checked" : "" ) );
	}
	else if (strcmp(argv[0], "sip_option_HeartbeatCycle")==0)
	{
		websWrite(wp, "%d", pCfg1->proxies[0].HeartbeatCycle);
	}
	else if (strcmp(argv[0], "pulse_det_Pause")==0)
	{
		websWrite(wp, "%d", pVoIPCfg->pulse_det_pause);
	}
#if defined(CONFIG_RTK_VOIP_DRIVERS_FXO) && !defined(CONFIG_RTK_VOIP_DRIVERS_VIRTUAL_DAA)
	else if (strcmp(argv[0], "display_cid_det")==0)
		websWrite(wp, "%s", "");
	else if (strcmp(argv[0], "display_pulse_dial_gen")==0)
		websWrite(wp, "%s", "");
#else
	else if (strcmp(argv[0], "display_cid_det")==0)
		websWrite(wp, "%s", "style=\"display:none\"");
	else if (strcmp(argv[0], "display_pulse_dial_gen")==0)
		websWrite(wp, "%s", "style=\"display:none\"");
#endif
	else if (strcmp(argv[0], "sip_disable_prack") == 0)
	{
		printf("pVoIPCfg->rfc_flags is %x\n",pVoIPCfg->rfc_flags);
		websWrite(wp, "%s", pVoIPCfg->rfc_flags & SIP_DISABLE_PRACK ? "checked" : "");
	}
	else if (strcmp(argv[0], "sip_enable_redundancy") == 0)
	{
		//printf("pVoIPCfg->rfc_flags is %x\n",pVoIPCfg->rfc_flags);
		websWrite(wp, "%s", pVoIPCfg->rfc_flags & SIP_SERVER_REDUNDANCY ? "checked" : "");
	}
	else if (strcmp(argv[0], "sip_anonymouse_from") == 0)
	{
		//printf("pVoIPCfg->rfc_flags is %x\n",pVoIPCfg->rfc_flags);
		websWrite(wp, "%s", pVoIPCfg->rfc_flags & SIP_ANONYMOUSE_FROM ? "checked" : "");
	}
	else if (strcmp(argv[0], "sip_internalcall") == 0)
	{
		printf("pVoIPCfg->rfc_flags is %x\n",pVoIPCfg->rfc_flags);
		websWrite(wp, "%s", pVoIPCfg->rfc_flags & SIP_INTERNALL_CALL ? "checked" : "");
	}
	else if (strcmp(argv[0], "hookflash_type")==0)
	{
#if 0	
		for (i=0; i < 2 ;i++)
		{
			if (i == (pVoIPCfg->rfc_flags & SIP_FLASHHOOK_INFO))
				websWrite(wp, "<option selected>%s</option>", hookflash_type[i]);
			else
				websWrite(wp, "<option>%s</option>", hookflash_type[i]);
		}
#endif
		//if (pVoIPCfg->rfc_flags & SIP_FLASHHOOK_INFO){
		if(pVoIPCfg->rfc_flags & SIP_FLASHHOOK_INFO){
			websWrite(wp, "<option>%s</option>", hookflash_type[0]);
			websWrite(wp, "<option selected>%s</option>", hookflash_type[1]);

		}else{
			websWrite(wp, "<option selected>%s</option>", hookflash_type[0]);
			websWrite(wp, "<option>%s</option>", hookflash_type[1]);
		}
	}	

	else
	{
		return -1;
	}

	return 0;
}

void asp_voip_OtherSet(webs_t wp, char_t *path, char_t *query)
{
	voipCfgParam_t *pVoIPCfg;
	voipCfgPortParam_t *pCfg;
	char key_display[FUNC_KEY_LENGTH];
//#if defined(CONFIG_RTK_VOIP_DRIVERS_FXO) && !defined(CONFIG_RTK_VOIP_DRIVERS_VIRTUAL_DAA)
	char *ptr;
	int i;
//#endif
//eric merge
	unsigned itf_idx = 0;
	char itf_name[16] = {0};
	int voip_port;	
		
	if (web_flash_get(&pVoIPCfg) != 0)
		return;

	strcpy(key_display, websGetVar(wp, T("funckey_transfer"), T("*1")));
	voip_char_replace(key_display, '*', '.', pVoIPCfg->funckey_transfer);

#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
	strcpy(key_display, websGetVar(wp, T("funckey_pstn"), T("*0")));
	voip_char_replace(key_display, '*', '.', pVoIPCfg->funckey_pstn);
#endif

	pVoIPCfg->auto_dial = (atoi(websGetVar(wp, T("auto_dial"), T("5")))) & AUTO_DIAL_TIME;
	if (gstrcmp(websGetVar(wp, T("auto_dial_always"), T("")), "on") == 0)
		pVoIPCfg->auto_dial |= AUTO_DIAL_ALWAYS;

	//support PRACK on/off
	if (gstrcmp(websGetVar(wp, T("sip_disable_prack"), T("")), "on") == 0){
		 pVoIPCfg->rfc_flags |= SIP_DISABLE_PRACK;
	}else{
		pVoIPCfg->rfc_flags &= ~SIP_DISABLE_PRACK;
	}

	//support SIP internal call (sip packet not send to proxy)
	if (gstrcmp(websGetVar(wp, T("sip_internalcall"), T("")), "on") == 0){
		 pVoIPCfg->rfc_flags |= SIP_INTERNALL_CALL;
	}else{
		pVoIPCfg->rfc_flags &= ~SIP_INTERNALL_CALL;
	}
	//support SIP server redundancy
	if (gstrcmp(websGetVar(wp, T("sip_enable_redundancy"), T("")), "on") == 0){
		 pVoIPCfg->rfc_flags |= SIP_SERVER_REDUNDANCY;
	}else{
		pVoIPCfg->rfc_flags &= ~SIP_SERVER_REDUNDANCY;
	}
	//support SIP ANONYMOUSE_FROM (some isp SIP from must use anonymouse for CLIR feature)
	if (gstrcmp(websGetVar(wp, T("sip_anonymouse_from"), T("")), "on") == 0){
		 pVoIPCfg->rfc_flags |= SIP_ANONYMOUSE_FROM;
	}else{
		pVoIPCfg->rfc_flags &= ~SIP_ANONYMOUSE_FROM;
	}

//support flash hook by SIP INFO
	ptr = websGetVar(wp, T("hookflash_type"), T(""));

	for(i=0; i < 2; i++)
	{
		if (!gstrcmp(ptr, hookflash_type[i]))
			break;
	}
	printf("hookflash_type i is %d\n",i);
	if(i==1){
		 pVoIPCfg->rfc_flags |= SIP_FLASHHOOK_INFO;
	}else{
		pVoIPCfg->rfc_flags &= ~SIP_FLASHHOOK_INFO;
	
	}

#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
	// PSTN Relay - Auto bypass relay (Switch to PSTN if VoIP failed)
	pVoIPCfg->auto_bypass_relay = !gstrcmp(websGetVar(wp, T("auto_bypass_relay"), T("")), "on");
	if (pVoIPCfg->auto_bypass_relay && !gstrcmp(websGetVar(wp, T("auto_bypass_warning"), T("")), "on"))
		pVoIPCfg->auto_bypass_relay |= AUTO_BYPASS_WARNING_ENABLE;
#endif

	pVoIPCfg->off_hook_alarm = atoi(websGetVar(wp, T("off_hook_alarm"), T("30")));

#if defined(CONFIG_RTK_VOIP_DRIVERS_FXO) && !defined(CONFIG_RTK_VOIP_DRIVERS_VIRTUAL_DAA)
	// VoIP to PSTN - one stage dialing
	pVoIPCfg->one_stage_dial = !gstrcmp(websGetVar(wp, T("one_stage_dial"), T("")), "on");

	// VoIP to PSTN - two stage dialing
	pVoIPCfg->two_stage_dial = !gstrcmp(websGetVar(wp, T("two_stage_dial"), T("")), "on");

	// PSTN to VoIP - Caller ID Detection
	pVoIPCfg->cid_auto_det_select = atoi(websGetVar(wp, T("caller_id_auto_det"), T("")));
	ptr	 = websGetVar(wp, T("caller_id_det"), T(""));
	for(i=0; i<CID_MAX; i++)
		if (!gstrcmp(ptr, cid_det[i]))
			break;

	pVoIPCfg->caller_id_det_mode = (i == CID_MAX) ? CID_DTMF : i;
	
	pVoIPCfg->pulse_dial_gen = atoi(websGetVar(wp, T("pulse_dial_generation"), T("")));
	
	ptr = websGetVar(wp, T("pulse_gen_PPS"), T(""));
	for(i=0; i<PULSE_GEN_PPS_MAX; i++)
		if (!gstrcmp(ptr, pulse_pps[i]))
			break;
	if (i == PULSE_GEN_PPS_10)
		pVoIPCfg->pulse_gen_pps = 10;
	else if (i == PULSE_GEN_PPS_20)
		pVoIPCfg->pulse_gen_pps = 20;
	else if (i == PULSE_GEN_PPS_MAX)
		pVoIPCfg->pulse_gen_pps = 10;
	pVoIPCfg->pulse_gen_make_time = atoi(websGetVar(wp, T("pulse_gen_Make"), T("33")));
		
	pVoIPCfg->pulse_gen_interdigit_pause = atoi(websGetVar(wp, T("pulse_gen_Pause"), T("700")));
	
#endif

	pVoIPCfg->pulse_dial_det = atoi(websGetVar(wp, T("pulse_dial_detection"), T("")));
	pVoIPCfg->pulse_det_pause = atoi(websGetVar(wp, T("pulse_det_Pause"), T("450")));

	//printf("sip_option_flag is %d\n",atoi(boaGetVar(wp, "sip_option_flag", "")));
	
	if(atoi(websGetVar(wp, T("sip_option_flag"), T("")))==1){
		pVoIPCfg->rfc_flags |= SIP_OPTION;		
	}else{
		pVoIPCfg->rfc_flags &= ~SIP_OPTION;	
	}

	for (voip_port=0; voip_port< VOIP_PORTS; voip_port++)
	{
		pCfg = &pVoIPCfg->ports[voip_port];		
		//heart beat cycle
		pCfg->proxies[0].HeartbeatCycle = atoi(websGetVar(wp, T("sip_option_HeartbeatCycle"), T("")));
		pCfg->proxies[1].HeartbeatCycle = atoi(websGetVar(wp, T("sip_option_HeartbeatCycle"), T("")));
	}

	web_flash_set(pVoIPCfg);
#if 0//#ifdef REBOOT_CHECK
	OK_MSG("/voip_other.asp");
#else
	web_restart_solar();
	websRedirect(wp, T("/voip_other.asp"));
#endif
}
