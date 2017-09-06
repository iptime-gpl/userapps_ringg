APSETUP_CGI_PATH:=$(USERAPPS_ROOT)/cgi-src/routersetup

routersetup_html:
# HTML CSS JS Section start
	@echo -e "\t\t--->Install HTML CSS JS GIFs"
# JS Section Start 	
	@mkdir -p $(JS_ROOT)
	@cp -ra $(APSETUP_CGI_PATH)/js/apsetup.js $(JS_ROOT)
ifeq ($(USE_UTF8),y)
	@iconv -f EUCKR -t UTF-8 $(APSETUP_CGI_PATH)/js/apsetup.lang.$(LANGUAGE_POSTFIX).js -o $(JS_ROOT)/apsetup.lang.js
else
	@cp $(APSETUP_CGI_PATH)/js/apsetup.lang.$(LANGUAGE_POSTFIX).js -o $(JS_ROOT)/apsetup.lang.js
endif
	cp -ra $(APSETUP_CGI_PATH)/images/* $(ROOT_DIR)/home/httpd/images2
	cp -ra $(APSETUP_CGI_PATH)/mimages/* $(ROOT_DIR)/home/httpd/images2
	cp -ra $(APSETUP_CGI_PATH)/css/*.css $(ROOT_DIR)/home/httpd
# JS Section End	
# HTML CSS JS Section End
ROUTERSETUP_UI_TARGET_LIST+=routersetup_html

##################################################################################################
# apsetup_cgi
##################################################################################################
APSETUP_CGIBIN_LIST := $(APSETUP_CGI_PATH)/setup.cgi $(APSETUP_CGI_PATH)/apset.cgi 
APSETUP_CGIBIN_LIST +=$(APSETUP_CGI_PATH)/msetup.cgi

routersetup_cgi:
	@echo -e "\t\t--->Install CGI"
	@mkdir -p $(ROOT_DIR)/setupbin
	@cp -ra $(APSETUP_CGIBIN_LIST) $(ROOT_DIR)/setupbin
ROUTERSETUP_UI_TARGET_LIST+=routersetup_cgi

routersetup_msg:
	@echo -e "\t--->Install routersetup UI"

routersetup_ui: routersetup_msg $(ROUTERSETUP_UI_TARGET_LIST)

TARGET_LIST+=routersetup_ui


