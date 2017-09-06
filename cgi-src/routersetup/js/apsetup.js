<script>


function IPKeyUp(prefix,idx)
{
}

function CheckNum(keynum)
{
        if((keynum >= 48) && (keynum <= 57))
        {
              return true;
        }
        return false;
}

function IPKeyDown(prefix,idx)
{
        var obj=document.getElementsByName(prefix+idx);
        var nextidx = idx + 1;

        if(window.event)
                keynum = event.keyCode;
        else if(e.which) // Netscape/Firefox/Opera
                keynum = event.which;

        if(keynum == 46) /* if '.' */
        {
                if(obj[0].value.length != 0)
                {
                        obj=document.getElementsByName(prefix+nextidx);
                        if(obj[0]) obj[0].focus();
                }
        }
        return CheckNum(keynum);
}


function isDigit(c)
{
        return ((c >= '0') && (c <= '9'));
}

function isInteger(s)
{
        var i;
        for( i = 0 ; i < s.length; i++ )
                if(!isDigit(s.charAt(i))) { return false; }
        return true;
}

function checkRange(address, srange, erange)
{
        var addr;

        if(isInteger(address) == false )
                return 1;

        addr=parseInt(address);
        if (addr == 0)
        {
                if ((addr < srange) || (addr > erange))
                        return 1;
                else
                        return 0;
        }
        else
        {
                if ((!addr) || (addr < srange) || (addr > erange))
                        return 1;
                else
                        return 0;
        }
}


function GetRadioValue( radioobj )
{
        for( var i = 0 ; i <  radioobj.length; i++ )
        {
                if(radioobj[i].checked)
                        return radioobj[i].value;
        }
        return "";
}

function GetValue( obj )
{
        if(obj[0] && obj[0].type == 'radio')
                return GetRadioValue(obj);
        if(obj && obj.type == 'select-one')
                return obj.options[obj.selectedIndex].value;

        return obj.value;
}

function EnableIP(name)
{
        for( i = 1; i <= 4; i++)
        {
                obj=document.getElementsByName(name+i);
                if(obj[0])
                {
                        obj[0].style.backgroundColor = "";
                        obj[0].disabled = false;
                }
        }
}

function DisableIP(name)
{
        for( i = 1; i <= 4; i++)
        {
                obj=document.getElementsByName(name+i);
                if(obj[0])
                {
                        obj[0].style.backgroundColor = "#EEEEEE";
                        obj[0].disabled = true;
                }
        }
}


function EnableObj(obj)
{
        if(obj && !obj.type)
        {
                if(obj[0].type == 'radio')
                {
                        for( var i = 0 ; i <  obj.length; i++ )
                        {
                                obj[i].disabled = false;
                                obj[i].style.backgroundColor = "";
                        }
                }
                return;
        }


        if(obj)
        {
                obj.disabled = false;
                obj.style.backgroundColor = "";
        }
}

function DisableObj(obj)
{

        if(obj && !obj.type)
        {
                if(obj[0].type == 'radio')
                {
                        for( var i = 0 ; i <  obj.length; i++ )
                        {
                                obj[i].disabled = true;
                                if(obj[i].type == 'text' || obj[i].type == 'password')
                                        obj[i].style.backgroundColor = "#EEEEEE";
                        }
                }
                return;
        }

        if(obj)
        {
                obj.disabled = true;
                if(obj.type == 'text' || obj.type == 'password' || obj.type == 'select')
                        obj.style.backgroundColor = "#EEEEEE";
        }
}

function ByteLenUTF8CharCode(charCode)
{
    if (charCode <= 0x00007F) {
          return 1;
        } else if (charCode <= 0x0007FF) {
          return 2;
        } else if (charCode <= 0x00FFFF) {
          return 3;
        } else {
          return 4;
        }
}

function StrLenUTF8CharCode(val)
{
        var len=0, i=0;

        for(i=0;val.charCodeAt(i);i++)
                len+=ByteLenUTF8CharCode(val.charCodeAt(i));
        return len;
}


function CheckUTF8Str(val)
{
        var len=0, i=0;

        for(i=0;val.charCodeAt(i);i++)
        {
                if(val.charCodeAt(i) > 0x00007F)
                        return 1;
        }
        return 0;
}

function MaskIt(doc,mask_name)
{
        var obj = doc.getElementById(mask_name);

        if(obj)
        {
                if (navigator.appName.indexOf("Microsoft") != -1)
                        obj.style.display = "block";
                else
                        obj.style.display = "table-row";
        }
}

function UnMaskIt(doc,mask_name)
{
        var obj;

        obj=doc.getElementById(mask_name);
        if(obj) obj.style.display = "none";
}


function ReadOnlyObj(obj,flag)
{
        if(flag == '1')
        {
                obj.readOnly=true;
                obj.style.backgroundColor='#eeeeee';
        }
        else
        {
                obj.readOnly=false;
                obj.style.backgroundColor='';
        }
}






function ToggleCheck(chkobj)
{
	chkobj.checked=(chkobj.checked == true)?false:true;
}

function InitApSetup()
{
	var F=document.setup_fm;
	OnClickIPMethod();
	F.pass_2g.type="password";
	if(F.pass_5g) F.pass_5g.type="password";
	OnChangeApSetup();
	if(F.mobile_ui)
	{
		setImageRadio('ip_method');
		if(F.port_role) setImageRadio('port_role');
	}
}

function PassView(pass_obj,check_obj,toggle_flag)
{
	if(check_obj.disabled == true)
		return;

	if(toggle_flag)
		check_obj.checked=(check_obj.checked==true)?false:true;
	pass_obj.type=(check_obj.checked==true)?"text":"password";
}

function PassView2G(pass_obj,check_obj,toggle_flag)
{
	var F=document.setup_fm;
	PassView(pass_obj,check_obj,toggle_flag);
	if(F.use_2g && F.use_2g.checked == true)
	{
		F.view_pass_5g.checked=F.view_pass_2g.checked;
		if(F.view_pass_2g.disabled == true) return;

		F.pass_5g.type=(F.view_pass_5g.checked==true)?"text":"password";
		//PassView(F.pass_5g,F.view_pass_5g,0);
	}
}
	
function OnClickIPMethod()
{
	var F=document.setup_fm;

	if(GetValue(F.ip_method) == 'manual')
	{
		EnableIP('manual_ip');
		DisableObj(F.iplist);
		DisableObj(F.ipsearch_bt);
	}
	else
	{
		DisableIP('manual_ip');
		EnableObj(F.iplist);
		EnableObj(F.ipsearch_bt);
	}
}

function OnKeyUpSSIDField()
{
	var F=document.setup_fm;
	
	if(F.use_2g && F.use_2g.checked == false)
		return;

	if(F.ssid_5g)
	{
		if(F.ssid_2g.value == '')	
			F.ssid_5g.value='';
		else
			F.ssid_5g.value=F.ssid_2g.value+'_5G';
	}

	return true;
}

function DuplicatePasswordField()
{
	var F=document.setup_fm;

	if(F.use_2g && F.use_2g.checked == false) return;
	if(F.pass_5g) F.pass_5g.value=F.pass_2g.value;
}


function OnChangeApSetup()
{
	var F=document.setup_fm;

	if(F.use_2g && F.use_2g.checked == true)
	{
		F.ssid_2g.maxLength=29;
		F.ssid_5g.value=F.ssid_2g.value;
		if(F.ssid_2g.value != '') F.ssid_5g.value += "_5G";
		F.pass_5g.value=F.pass_2g.value;
		F.view_pass_5g.checked=F.view_pass_2g.checked;

		DisableObj(F.use_encrypt_5g,1);
		DisableObj(F.ssid_5g,1);
		DisableObj(F.pass_5g,1);
		DisableObj(F.view_pass_5g,1);
	}
	else
	{
		EnableObj(F.ssid_5g,1);
		EnableObj(F.pass_5g,1);
		EnableObj(F.use_encrypt_5g,1);
		EnableObj(F.view_pass_5g,1);
		F.ssid_2g.maxLength=32;
	}

	if(F.use_encrypt_2g.checked == true)
	{
		EnableObj(F.pass_2g,1);
		EnableObj(F.view_pass_2g);
		F.pass_2g.type=(F.view_pass_2g.checked==true)?"text":"password";
		if(F.pass_2g.value == MSG_NO_ENCRYPT) F.pass_2g.value="";
                F.pass_2g.style.color = "#000000";
	}
	else
	{
		//ReadOnlyObj(F.pass_2g,1);
		DisableObj(F.pass_2g,1);
		DisableObj(F.view_pass_2g);
                F.pass_2g.type = "text";
		F.pass_2g.value=MSG_NO_ENCRYPT;
                F.pass_2g.style.color = "#888888";
		
	}

	if(F.use_2g && F.use_2g.checked == true)
		F.use_encrypt_5g.checked = F.use_encrypt_2g.checked;

	if(F.use_encrypt_5g)
	{
		if(F.use_encrypt_5g.checked == true)
		{
			if(F.use_2g.checked == true)
				DisableObj(F.pass_5g,1);
				//ReadOnlyObj(F.pass_5g,1);
			else
			{
				EnableObj(F.pass_5g,1);
				//ReadOnlyObj(F.pass_5g,0);
				EnableObj(F.view_pass_5g);
				F.pass_5g.type=(F.view_pass_5g.checked==true)?"text":"password";
                                F.pass_5g.style.color = "#000000";
			}

			if(F.pass_5g.value == MSG_NO_ENCRYPT)F.pass_5g.value="";
			F.pass_5g.type=(F.view_pass_5g.checked==true)?"text":"password";
		}
		else
		{
			//ReadOnlyObj(F.pass_5g,1);
			DisableObj(F.pass_5g,1);
			DisableObj(F.view_pass_5g);
                        F.pass_5g.type = "text";
			F.pass_5g.value=MSG_NO_ENCRYPT;
                        F.pass_5g.style.color = "#888888";
		}
	}
}

function IsHex2(s)
{
        var i;

        for( i = 0 ; i < s.length; i++ )
                if( !(
                ((s.charAt(i) >= 'a') && (s.charAt(i) <= 'f')) ||
                ((s.charAt(i) >= 'A' ) && (s.charAt(i) <= 'F')) ||
                ((s.charAt(i) >= '0' ) && (s.charAt(i) <= '9')) ))
                        return 0;
        return 1;
}

var RebootTimer;
function RebootTimerCallback()
{
	var F=document.setup_fm;
	var time = document.getElementById('remain_sec').innerHTML;

        if(time > 0)
	{
                time--;
        	document.getElementById('remain_sec').innerHTML = time;
	}
        else if(time == 0)
	{
        	document.getElementById('remain_msg').style.display = 'none';
        	document.getElementById('head_msg').style.display = 'none';
        	document.getElementById('head_apply_img').style.display = 'none';
        	document.getElementById('head2_msg').style.display = 'block';
        	document.getElementById('head3_msg').style.display = 'block';
	        //MaskIt(document,'result_mask');
		clearInterval(RebootTimer);
	}
}

function CheckIP2(name)
{
        var i;
	var ra=new Array();

        for( i=1;i<= 4 ;i++)
        {
                obj=document.getElementsByName(name+i);
                if(!obj[0])
                        break;

                if(obj[0].value=='')
		{
			ra['obj']=obj[0];
			ra['msg']=MSG_BLANK_IP;
			return ra;
		}
                if ((i == 1) && (checkRange(obj[0].value, 1, 255)))
		{
			ra['obj']=obj[0];
			ra['msg']=MSG_INVALID_RANGE_IP_ERR1;
			return ra;
		}
                else if(checkRange(obj[0].value, 0, 255))
		{
			ra['obj']=obj[0];
			ra['msg']=MSG_INVALID_RANGE_IP_ERR2;
			return ra;
		}
        }
        return 0;
}


function check_login_id(s)
{
        for( i = 0 ; i < s.length; i++ )
        {
                if(((s.charAt(i) >= 'a') && (s.charAt(i) <= 'z')) ||
                        ((s.charAt(i) >= 'A' ) && (s.charAt(i) <= 'Z')) ||
                        ((s.charAt(i) >= '0' ) && (s.charAt(i) <= '9')) )
                                continue;
        else
                        return false;
       }
       return true;
}


function ApplyAPSetup()
{
	var F=document.setup_fm;
	var ssidlen;

	if(GetValue(F.ip_method) == 'manual')
	{
		var ra;

		if(ra=CheckIP2('manual_ip'))
		{
			alert(ra['msg']);
			ra['obj'].focus();
			ra['obj'].select();
			return;
		}
	}

	if((ssidlen=StrLenUTF8CharCode(F.ssid_2g.value)) == 0)
	{
		alert("[2.4GHz] "+MSG_SET_SSID);
		F.ssid_2g.focus();
		F.ssid_2g.select();
		return;
	}

	if((ssidlen=StrLenUTF8CharCode(F.ssid_2g.value)) > 32)
	{
		alert("[2.4GHz] "+MSG_TOO_LONG_SSID+ssidlen+'bytes');
		F.ssid_2g.focus();
		F.ssid_2g.select();
		return;
	}

	if(F.ssid_5g && ((ssidlen=StrLenUTF8CharCode(F.ssid_5g.value)) == 0))
	{
		alert("[5GHz] "+MSG_SET_SSID);
		F.ssid_5g.focus();
		F.ssid_5g.select();
		return;
	}

	if(F.ssid_5g && ((ssidlen=StrLenUTF8CharCode(F.ssid_5g.value)) > 32))
	{
		alert("[5GHz] "+MSG_TOO_LONG_SSID+ssidlen+'bytes');
		F.ssid_5g.focus();
		F.ssid_5g.select();
		return;
	}

	if(F.use_encrypt_2g.checked == true )
	{
		pass_2gobj=F.pass_2g;

		if(CheckUTF8Str(pass_2gobj.value))	
		{
			alert("[2.4GHz] "+MSG_INVALID_UTF8_PASSWORD);
			pass_2gobj.select();
			pass_2gobj.focus();
			return;
		}


		if(pass_2gobj.value.length < 8 || pass_2gobj.value.length >64)
		{
			alert("[2.4GHz] "+MSG_INVALID_PASSWORD);
			pass_2gobj.select();
			pass_2gobj.focus();
			return;
		}
        
		if((pass_2gobj.value.length == 64) && !IsHex2(pass_2gobj.value))
		{
			alert("[2.4GHz] "+MSG_INVALID_PASSWORD_HEX_STR);
			pass_2gobj.focus();
			pass_2gobj.select();
			return;
		}
	}

	if(F.use_encrypt_5g && (F.use_encrypt_5g.checked == true))
	{
		pass_5gobj=F.pass_5g;

		if(CheckUTF8Str(pass_5gobj.value))	
		{
			alert("[5GHz] "+MSG_INVALID_UTF8_PASSWORD);
			pass_5gobj.select();
			pass_5gobj.focus();
			return;
		}

		if(pass_5gobj.value.length < 8 || pass_5gobj.value.length >64)
		{
			alert("[5GHz] "+MSG_INVALID_PASSWORD);
			pass_5gobj.focus();
			pass_5gobj.select();
			return;
		}
        
		if((pass_5gobj.value.length == 64) && !IsHex2(pass_5gobj.value))
		{
			alert("[5GHz] "+MSG_INVALID_PASSWORD_HEX_STR);
			pass_5gobj.focus();
			pass_5gobj.select();
			return;
		}
	}

	if(F.use_account && F.use_account.checked == true) 
	{
		if(F.id_admin.value == '')
		{
			alert(MSG_SET_ADMIN_ID);
			F.id_admin.focus();
			F.id_admin.select();
			return;
		}

		if(!check_login_id(F.id_admin.value))
		{
			alert(MSG_INVALID_ADMIN_ID);
			F.id_admin.focus();
			F.id_admin.select();
			return;
		}


		if(F.pass_admin.value == '')
		{
			alert(MSG_SET_ADMIN_PASS);
			F.pass_admin.focus();
			F.pass_admin.select();
			return;
		}

		if(CheckUTF8Str(F.pass_admin.value))
		{
			alert(MSG_INVALID_UTF8_PASSWORD);
			F.pass_admin.select();
			F.pass_admin.focus();
			return;
		}

	}


	if(confirm(MSG_RECONNECT_AFTER_SETTING))
	{
		var cF=iframe_apset.commit_fm;

		cF.act.value='apply';
		if(cF.port_role) 
			cF.port_role.value=GetValue(F.port_role);
		
		if(GetValue(F.ip_method) == 'manual')
			cF.ipaddr.value = F.manual_ip1.value+'.'+ F.manual_ip2.value+'.'+F.manual_ip3.value+'.'+F.manual_ip4.value;
		else
			cF.ipaddr.value = F.iplist.value;


		if(F.use_account)
		{
			if(F.use_account.checked == true)
			{
				cF.use_account.value="1";
				cF.id_admin.value=F.id_admin.value;
				cF.pass_admin.value=F.pass_admin.value;
			}
			else
			{
				cF.use_account.value="0";
				cF.id_admin.value='';
				cF.pass_admin.value='';
			}
		}

		cF.ssid_2g.value = F.ssid_2g.value;
		cF.use_encrypt_2g.value = (F.use_encrypt_2g.checked)?"1":"0";
		if(F.use_encrypt_2g.checked == true)
			cF.pass_2g.value = F.pass_2g.value;

		if(F.ssid_5g)
		{
			cF.ssid_5g.value = F.ssid_5g.value;
			cF.use_encrypt_5g.value = (F.use_encrypt_5g.checked)?"1":"0";
			if(F.use_encrypt_5g.checked == true)
				cF.pass_5g.value = F.pass_5g.value;
		}

        	document.getElementById('config_ip').innerHTML = cF.ipaddr.value;
        	document.getElementById('config_ssid_2g').innerHTML = F.ssid_2g.value;
        	if(F.ssid_5g) document.getElementById('config_ssid_5g').innerHTML = F.ssid_5g.value;

		cF.submit();

	        MaskIt(document,'total_mask');
	        MaskIt(document,'apply_mask');

		RebootTimer=setInterval(RebootTimerCallback,1000);
	}
}


function StartIPScan()
{
	var F=document.setup_fm;
	var cF=iframe_apset.commit_fm;

	if(GetValue(F.ip_method) == "manual")
		return;

	if(F.iplist.selectedIndex !=-1)
		F.iplist[F.iplist.selectedIndex].text = "";

	if(cF.port_role) cF.port_role.value=GetValue(F.port_role);
	cF.act.value='ipscan';
	cF.submit();


	MaskIt(document,'total_mask');
	//MaskIt(document,'ipscan_mask');

	/* For Mobile UI*/
	if(F.mobile_ui)
	{
		DisableObj(F.iplist);
	}
	//document.getElementById('loading_img').style.display = 'block';
        if (navigator.appName.indexOf("Microsoft") != -1)
                document.getElementById('loading_img').style.display = "block";
        else
                document.getElementById('loading_img').style.display = "";

}


function InitScanList()
{
	var F=parent.setup_fm;
	var scanlist=document.getElementsByName('ipscan_list');

	F.iplist.length=scanlist.length;
	for(i=0;i<F.iplist.length;i++)
		F.iplist[i]=new Option(scanlist[i].value,scanlist[i].value);
	if(F.mobile_ui)
	{
		EnableObj(F.iplist);
	}
	parent.document.getElementById('loading_img').style.display = 'none';
}



function setImageRadio(name)
{
        document.getElementById(name+'_img1').src=
        (document.getElementsByName(name)[0].checked==true)?"/images2/on.png":"/images2/off.png";

        document.getElementById(name+'_img2').src=
        (document.getElementsByName(name)[1].checked==true)?"/images2/on.png":"/images2/off.png";

	OnClickIPMethod();
}

function ClickRadio(name,idx)
{
	document.getElementsByName(name)[idx].checked=true; 
	setImageRadio(name);
}



function OnClickAdminCheck()
{
	var F=document.setup_fm;

	if(F.use_account.checked == true) 
	{
		ReadOnlyObj(F.id_admin,0);
		EnableObj(F.pass_admin);
		EnableObj(F.admin_passwd_view);
		F.id_admin.value=F.default_id.value;
                F.id_admin.style.color = "#000000";
	}
	else
	{
		ReadOnlyObj(F.id_admin,1);
		DisableObj(F.pass_admin);
		DisableObj(F.admin_passwd_view);
		F.id_admin.value=MSG_NO_ADMIN_USE;
                F.id_admin.style.color = "#888888";
		F.pass_admin.value="";
	}

}

</script>
