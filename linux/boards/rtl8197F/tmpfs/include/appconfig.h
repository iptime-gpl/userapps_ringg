#define CONFIG_APP_L2TPD 1
#define CONFIG_APP_PPPD 1
#define CONFIG_PPPOE_DISCONNECT_FLOW_PATCH 1
#define CONFIG_APP_PPTP 1
#define CONFIG_APP_AUTH 1
#define CONFIG_APP_BOA 1
#define CONFIG_APP_BOA_NEW_UI 1
#define CONFIG_APP_BRCTL 1
#define CONFIG_APP_BUSYBOX 1
#define CONFIG_APP_DNRD 1
#define CONFIG_APP_FWD 1
#define CONFIG_APP_IAPP 1
#define CONFIG_APP_IGMPPROXY 1
#define CONFIG_APP_IPROUTE 1
#define CONFIG_APP_IPTABLES 1
#define CONFIG_APP_IWCONTROL 1
#define CONFIG_APP_LLTDD 1
#define CONFIG_APP_MINIIGD 1
#define CONFIG_APP_MINI_UPNP 1
#define CONFIG_APP_NTPCLIENT 1
#define CONFIG_APP_RELOAD 1
#define CONFIG_APP_ROUTED 1
#define CONFIG_APP_SCRIPT 1
#define CONFIG_APP_SQUASHFS42 1
#define CONFIG_APP_UDHCPD 1
#define CONFIG_APP_UPDATEDD 1
#define CONFIG_APP_WIRELESSTOOLS 1
#define CONFIG_APP_WSC 1
#define CONFIG_LIB_XZ 1
#define CONFIG_DEBUG1 1
#define CONFIG_MTD_NAME_MAPPING 1
#define CONFIG_MTD_API 1
#define CONFIG_EFM_PATCH 1
#define CONFIG_EFM_RING_PRODUCT 1
#define CONFIG_EFM_RING_GIGA 1
#define CONFIG_MIPS 1
#define CONFIG_REALTEK 1
#define CONFIG_SOC_RTL8197F 1
#define CONFIG_ARCH_CPU_TYPE "mips"
#define CONFIG_ARCH_32BIT 1
#define CONFIG_ARCH_CPU_MIPS 1
#define CONFIG_ARCH_CPU_MIPS24K 1
#define CONFIG_ARCH_CPU_EL 1
#define CONFIG_ARCH_IRQ_CPU 1
#define CONFIG_ARCH_CACHE_WBC 1
#define CONFIG_ARCH_BUS_USB 1
#define CONFIG_ARCH_BUS_PCI 1
#define CONFIG_RTL_819X 1
#define CONFIG_RTL_8197F 1
#define CONFIG_ARCH_PLATFORM "Sheipa2"
#define CONFIG_RTL_8197F_GW 1
#define CONFIG_RTL_PCIE_SIMPLE_INIT 1
#define CONFIG_RTL_WTDOG 1
#define CONFIG_RTL_WEBPAGES_IN_ROOTFS 1
#define CONFIG_RTL819X_SPI_FLASH_SHEIPA 1
#define CONFIG_AUTO_PCIE_PHY_SCAN 1
#define CONFIG_RTL819X_GPIO 1
#define CONFIG_RTL819X_FLOAT_PIN_INIT 1
#define CONFIG_RTL_FLASH_MAPPING_ENABLE 1
#define CONFIG_RTL_FLASH_SIZE 0x800000
#define CONFIG_RTL_HW_SETTING_OFFSET 0x20000
#define CONFIG_RTL_DEFAULT_SETTING_OFFSET 0x30000
#define CONFIG_RTL_CURRENT_SETTING_OFFSET 0x38000
#define CONFIG_RTL_WEB_PAGES_OFFSET 0x30000
#define CONFIG_RTL_LINUX_IMAGE_OFFSET 0x40000
#define CONFIG_RTL_ROOT_IMAGE_OFFSET 0x220000
#define CONFIG_RTL_WAPI_PARTITION_SIZE 0x0
#define CONFIG_RTL_BT_PARTITION_SIZE 0x80000
#define CONFIG_RTL_1X_PARTITION_SIZE 0x0
#define CONFIG_RTL_HOMEKIT_PARTITION_SIZE 0x0
#define CONFIG_RTL_FLATFS_PARTITION_SIZE 0x0
#define CONFIG_KERNEL_STACK_SIZE_ORDER 3
#define CONFIG_ROOTFS_SQUASH 1
#define CONFIG_ARCH_CEVT_R4K 1
#define CONFIG_RWSEM_GENERIC_SPINLOCK 1
#define CONFIG_GENERIC_HWEIGHT 1
#define CONFIG_GENERIC_CALIBRATE_DELAY 1
#define CONFIG_SCHED_OMIT_FRAME_POINTER 1
#define CONFIG_CEVT_R4K 1
#define CONFIG_CSRC_R4K 1
#define CONFIG_DMA_NONCOHERENT 1
#define CONFIG_NEED_DMA_MAP_STATE 1
#define CONFIG_SYS_HAS_EARLY_PRINTK 1
#define CONFIG_CPU_LITTLE_ENDIAN 1
#define CONFIG_SYS_SUPPORTS_LITTLE_ENDIAN 1
#define CONFIG_IRQ_CPU 1
#define CONFIG_MIPS_L1_CACHE_SHIFT 5
#define CONFIG_CPU_MIPS32_R2 1
#define CONFIG_SYS_HAS_CPU_MIPS32_R2 1
#define CONFIG_CPU_MIPS32 1
#define CONFIG_CPU_MIPSR2 1
#define CONFIG_SYS_SUPPORTS_32BIT_KERNEL 1
#define CONFIG_CPU_SUPPORTS_32BIT_KERNEL 1
#define CONFIG_HARDWARE_WATCHPOINTS 1
#define CONFIG_32BIT 1
#define CONFIG_PAGE_SIZE_16KB 1
#define CONFIG_FORCE_MAX_ZONEORDER 11
#define CONFIG_CPU_HAS_PREFETCH 1
#define CONFIG_CPU_GENERIC_DUMP_TLB 1
#define CONFIG_CPU_R4K_FPU 1
#define CONFIG_CPU_R4K_CACHE_TLB 1
#define CONFIG_MIPS_MT_DISABLED 1
#define CONFIG_CPU_HAS_SYNC 1
#define CONFIG_CPU_SUPPORTS_HIGHMEM 1
#define CONFIG_ARCH_FLATMEM_ENABLE 1
#define CONFIG_FLATMEM 1
#define CONFIG_FLAT_NODE_MEM_MAP 1
#define CONFIG_HAVE_MEMBLOCK 1
#define CONFIG_HAVE_MEMBLOCK_NODE_MAP 1
#define CONFIG_ARCH_DISCARD_MEMBLOCK 1
#define CONFIG_PAGEFLAGS_EXTENDED 1
#define CONFIG_SPLIT_PTLOCK_CPUS 4
#define CONFIG_ZONE_DMA_FLAG 0
#define CONFIG_VIRT_TO_BUS 1
#define CONFIG_DEFAULT_MMAP_MIN_ADDR 4096
#define CONFIG_CROSS_MEMORY_ATTACH 1
#define CONFIG_NEED_PER_CPU_KM 1
#define CONFIG_HZ_100 1
#define CONFIG_SYS_SUPPORTS_ARBIT_HZ 1
#define CONFIG_HZ 100
#define CONFIG_PREEMPT_NONE 1
#define CONFIG_LOCKDEP_SUPPORT 1
#define CONFIG_STACKTRACE_SUPPORT 1
#define CONFIG_DEFCONFIG_LIST "/lib/modules/$UNAME_RELEASE/.config"
#define CONFIG_IRQ_WORK 1
#define CONFIG_BUILDTIME_EXTABLE_SORT 1
#define CONFIG_BROKEN_ON_SMP 1
#define CONFIG_INIT_ENV_ARG_LIMIT 32
#define CONFIG_CROSS_COMPILE "mips-linux-"
#define CONFIG_LOCALVERSION ""
#define CONFIG_LOCALVERSION_AUTO 1
#define CONFIG_DEFAULT_HOSTNAME "rlxlinux"
#define CONFIG_SYSVIPC 1
#define CONFIG_SYSVIPC_SYSCTL 1
#define CONFIG_HAVE_GENERIC_HARDIRQS 1
#define CONFIG_GENERIC_HARDIRQS 1
#define CONFIG_GENERIC_IRQ_PROBE 1
#define CONFIG_GENERIC_IRQ_SHOW 1
#define CONFIG_IRQ_FORCED_THREADING 1
#define CONFIG_GENERIC_CLOCKEVENTS 1
#define CONFIG_GENERIC_CLOCKEVENTS_BUILD 1
#define CONFIG_GENERIC_CMOS_UPDATE 1
#define CONFIG_HZ_PERIODIC 1
#define CONFIG_TICK_CPU_ACCOUNTING 1
#define CONFIG_TINY_RCU 1
#define CONFIG_LOG_BUF_SHIFT 12
#define CONFIG_UIDGID_CONVERTED 1
#define CONFIG_SYSCTL 1
#define CONFIG_ANON_INODES 1
#define CONFIG_HOTPLUG 1
#define CONFIG_EXPERT 1
#define CONFIG_SYSCTL_SYSCALL 1
#define CONFIG_KALLSYMS 1
#define CONFIG_KALLSYMS_ALL 1
#define CONFIG_PRINTK_FUNC 1
#define CONFIG_PRINTK 1
#define CONFIG_BUG 1
#define CONFIG_BASE_FULL 1
#define CONFIG_FUTEX 1
#define CONFIG_EPOLL 1
#define CONFIG_SIGNALFD 1
#define CONFIG_TIMERFD 1
#define CONFIG_EVENTFD 1
#define CONFIG_AIO 1
#define CONFIG_EMBEDDED 1
#define CONFIG_HAVE_PERF_EVENTS 1
#define CONFIG_PERF_USE_VMALLOC 1
#define CONFIG_COMPAT_BRK 1
#define CONFIG_SLUB 1
#define CONFIG_HAVE_OPROFILE 1
#define CONFIG_HAVE_KPROBES 1
#define CONFIG_HAVE_KRETPROBES 1
#define CONFIG_HAVE_DMA_ATTRS 1
#define CONFIG_GENERIC_SMP_IDLE_THREAD 1
#define CONFIG_HAVE_CLK 1
#define CONFIG_HAVE_DMA_API_DEBUG 1
#define CONFIG_HAVE_ARCH_JUMP_LABEL 1
#define CONFIG_ARCH_WANT_IPC_PARSE_VERSION 1
#define CONFIG_HAVE_MOD_ARCH_SPECIFIC 1
#define CONFIG_MODULES_USE_ELF_REL 1
#define CONFIG_CLONE_BACKWARDS 1
#define CONFIG_HAVE_GENERIC_DMA_COHERENT 1
#define CONFIG_RT_MUTEXES 1
#define CONFIG_BASE_SMALL 0
#define CONFIG_MODULES 1
#define CONFIG_MODULE_UNLOAD 1
#define CONFIG_BLOCK 1
#define CONFIG_LBDAF 1
#define CONFIG_BLK_DEV_BSG 1
#define CONFIG_MSDOS_PARTITION 1
#define CONFIG_EFI_PARTITION 1
#define CONFIG_IOSCHED_NOOP 1
#define CONFIG_DEFAULT_NOOP 1
#define CONFIG_DEFAULT_IOSCHED "noop"
#define CONFIG_INLINE_SPIN_UNLOCK_IRQ 1
#define CONFIG_INLINE_READ_UNLOCK 1
#define CONFIG_INLINE_READ_UNLOCK_IRQ 1
#define CONFIG_INLINE_WRITE_UNLOCK 1
#define CONFIG_INLINE_WRITE_UNLOCK_IRQ 1
#define CONFIG_MMU 1
#define CONFIG_BINFMT_ELF 1
#define CONFIG_ARCH_BINFMT_ELF_RANDOMIZE_PIE 1
#define CONFIG_ELFCORE 1
#define CONFIG_BINFMT_SCRIPT 1
#define CONFIG_TRAD_SIGNALS 1
#define CONFIG_ARCH_HIBERNATION_POSSIBLE 1
#define CONFIG_ARCH_SUSPEND_POSSIBLE 1
#define CONFIG_NET 1
#define CONFIG_LOCAL_IFNAME "br0"
#define CONFIG_PACKET 1
#define CONFIG_UNIX 1
#define CONFIG_INET 1
#define CONFIG_IP_MULTICAST 1
#define CONFIG_RTL_IPTABLES_FAST_PATH 1
#define CONFIG_RTL_FAST_PPPOE 1
#define CONFIG_RTL_PPPOE_DIRECT_REPLY 1
#define CONFIG_IP_MROUTE 1
#define CONFIG_INET_DIAG 1
#define CONFIG_INET_TCP_DIAG 1
#define CONFIG_TCP_CONG_CUBIC 1
#define CONFIG_DEFAULT_TCP_CONG "cubic"
#define CONFIG_IPCONFLICT_DETECT 1
#define CONFIG_FORWARD_SPEED_UP_PATCH 1
#define CONFIG_PASSTHROUGH_ALL_CHAIN 1
#define CONFIG_DRIVERLEVEL_REAL_IPCLONE 1
#define CONFIG_SMART_QOS_MONITOR 1
#define CONFIG_NETFILTER 1
#define CONFIG_NETFILTER_ADVANCED 1
#define CONFIG_NETFILTER_NETLINK 1
#define CONFIG_NETFILTER_NETLINK_QUEUE 1
#define CONFIG_NETFILTER_NETLINK_LOG 1
#define CONFIG_IP_CONNTRACK_LIMIT_CONTROL 1
#define CONFIG_IP_CONNTRACK_LIMIT_PER_IP 1
#define CONFIG_UDP_CONN_MAX_SIZE 2048
#define CONFIG_ICMP_CONN_MAX_SIZE 50
#define CONFIG_SIP_UDP_TIMEOUT_PATCH 600
#define CONFIG_NF_CONNTRACK 1
#define CONFIG_NF_CONNTRACK_MARK 1
#define CONFIG_NF_CONNTRACK_PROCFS 1
#define CONFIG_NF_CONNTRACK_RTCACHE 1
#define CONFIG_NF_CT_PROTO_GRE 1
#define CONFIG_NF_CONNTRACK_FTP 1
#define CONFIG_NF_CONNTRACK_PPTP 1
#define CONFIG_NF_CT_PROTO_ESP 1
#define CONFIG_NF_CONNTRACK_SIP 1
#define CONFIG_NF_CONNTRACK_BNET 1
#define CONFIG_NF_NAT 1
#define CONFIG_NF_NAT_NEEDED 1
#define CONFIG_NF_NAT_FTP 1
#define CONFIG_NF_NAT_SIP 1
#define CONFIG_NETFILTER_XTABLES 1
#define CONFIG_NETFILTER_XT_MARK 1
#define CONFIG_NETFILTER_XT_CONNMARK 1
#define CONFIG_NETFILTER_XT_TARGET_DSCP 1
#define CONFIG_NETFILTER_XT_TARGET_VLANPRIORITY 1
#define CONFIG_NETFILTER_XT_TARGET_MARK 1
#define CONFIG_NETFILTER_XT_TARGET_NFLOG 1
#define CONFIG_NETFILTER_XT_TARGET_NFQUEUE 1
#define CONFIG_NETFILTER_XT_TARGET_TCPMSS 1
#define CONFIG_NETFILTER_XT_MATCH_CONNLIMIT 1
#define CONFIG_NETFILTER_XT_MATCH_CONNMARK 1
#define CONFIG_NETFILTER_XT_MATCH_DSCP 1
#define CONFIG_RTL_DSCP_IPTABLE_CHECK 1
#define CONFIG_NETFILTER_XT_MATCH_IPRANGE 1
#define CONFIG_NETFILTER_XT_MATCH_MAC 1
#define CONFIG_NETFILTER_XT_MATCH_VLAN 1
#define CONFIG_RTL_QOS_8021P_SUPPORT 1
#define CONFIG_RTL_VLANPRI_IPTABLE_CHECK 1
#define CONFIG_NETFILTER_XT_MATCH_PHYPORT 1
#define CONFIG_NETFILTER_XT_MATCH_MARK 1
#define CONFIG_NETFILTER_XT_MATCH_MULTIPORT 1
#define CONFIG_NETFILTER_XT_MATCH_STATE 1
#define CONFIG_NETFILTER_XT_MATCH_STRING 1
#define CONFIG_CONNTRACK_STATISTICS 1
#define CONFIG_NF_DEFRAG_IPV4 1
#define CONFIG_NF_CONNTRACK_IPV4 1
#define CONFIG_NF_CONNTRACK_PROC_COMPAT 1
#define CONFIG_IP_NF_IPTABLES 1
#define CONFIG_IP_NF_FILTER 1
#define CONFIG_NF_NAT_IPV4 1
#define CONFIG_IP_NF_TARGET_MASQUERADE 1
#define CONFIG_NF_NAT_PROTO_GRE 1
#define CONFIG_NF_NAT_PROTO_ESP 1
#define CONFIG_NF_NAT_PPTP 1
#define CONFIG_NF_NAT_BNET 1
#define CONFIG_IP_NF_MANGLE 1
#define CONFIG_L2TP 1
#define CONFIG_STP 1
#define CONFIG_BRIDGE 1
#define CONFIG_PORT_BASED_VLAN_FEATURE 1
#define CONFIG_HAVE_NET_DSA 1
#define CONFIG_VLAN_8021Q 1
#define CONFIG_RTL_VLAN_8021Q 1
#define CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG 1
#define CONFIG_LLC 1
#define CONFIG_NET_SCHED 1
#define CONFIG_NET_SCH_HTB 1
#define CONFIG_NET_SCH_SFQ 1
#define CONFIG_NET_CLS 1
#define CONFIG_NET_CLS_FW 1
#define CONFIG_NET_CLS_U32 1
#define CONFIG_NET_SCH_FIFO 1
#define CONFIG_BQL 1
#define CONFIG_WIRELESS 1
#define CONFIG_WIRELESS_EXT 1
#define CONFIG_WEXT_CORE 1
#define CONFIG_WEXT_PROC 1
#define CONFIG_UEVENT_HELPER_PATH "/sbin/hotplug"
#define CONFIG_STANDALONE 1
#define CONFIG_PREVENT_FIRMWARE_BUILD 1
#define CONFIG_FW_LOADER 1
#define CONFIG_FIRMWARE_IN_KERNEL 1
#define CONFIG_EXTRA_FIRMWARE ""
#define CONFIG_FW_LOADER_USER_HELPER 1
#define CONFIG_MTD 1
#define CONFIG_MTD_CMDLINE_PARTS 1
#define CONFIG_MTD_RTKXX_PARTS 1
#define CONFIG_MTD_BLKDEVS 1
#define CONFIG_MTD_BLOCK 1
#define CONFIG_MTD_MAP_BANK_WIDTH_1 1
#define CONFIG_MTD_MAP_BANK_WIDTH_2 1
#define CONFIG_MTD_MAP_BANK_WIDTH_4 1
#define CONFIG_MTD_CFI_I1 1
#define CONFIG_MTD_CFI_I2 1
#define CONFIG_MTD_M25P80 1
#define CONFIG_M25PXX_USE_USER_MODE 1
#define CONFIG_MTD_BLOCK2MTD 1
#define CONFIG_BLK_DEV 1
#define CONFIG_BLK_DEV_LOOP 1
#define CONFIG_BLK_DEV_LOOP_MIN_COUNT 8
#define CONFIG_HAVE_IDE 1
#define CONFIG_SCSI_MOD 1
#define CONFIG_NETDEVICES 1
#define CONFIG_NET_CORE 1
#define CONFIG_PPP 1
#define CONFIG_PPPOE 1
#define CONFIG_PPP_ASYNC 1
#define CONFIG_PPP_MPPE_MPPC 1
#define CONFIG_PPP_SYNC_TTY 1
#define CONFIG_SLHC 1
#define CONFIG_WLAN 1
#define CONFIG_RTL8192CD 1
#define CONFIG_PCI_HCI 1
#define CONFIG_WLAN_HAL_8197F 1
#define CONFIG_SOC_RFE_TYPE_0 1
#define CONFIG_MAC_PHY_RF_Parameter_V702B_Skyworth 1
#define CONFIG_USE_PCIE_SLOT_0 1
#define CONFIG_SLOT_0_8822BE 1
#define CONFIG_SLOT_0_RFE_TYPE_1 1
#define CONFIG_SLOT_0_EXT_PA 1
#define CONFIG_SLOT_0_EXT_LNA 1
#define CONFIG_SLOT_0_TX_BEAMFORMING 1
#define CONFIG_WLAN_HAL 1
#define CONFIG_WLAN_HAL_88XX 1
#define CONFIG_WLAN_MACHAL_API 1
#define CONFIG_WLAN_HAL_8822BE 1
#define CONFIG_RTL_DFS_SUPPORT 1
#define CONFIG_RTL8190_PRIV_SKB 1
#define CONFIG_RTL_VAP_SUPPORT 1
#define CONFIG_RTL_CLIENT_MODE_SUPPORT 1
#define CONFIG_RTL_REPEATER_MODE_SUPPORT 1
#define CONFIG_RTL_SUPPORT_MULTI_PROFILE 1
#define CONFIG_RTL_WDS_SUPPORT 1
#define CONFIG_RTL_WPS2_SUPPORT 1
#define CONFIG_TXPWR_LMT 1
#define CONFIG_RTL_TDLS_SUPPORT 1
#define CONFIG_RTL_11W_SUPPORT 1
#define CONFIG_BAND_5G_ON_WLAN0 1
#define CONFIG_RTL_5G_SLOT_0 1
#define CONFIG_RTL_ODM_WLAN_DRIVER 1
#define CONFIG_SOC_WIFI 1
#define CONFIG_RTL_DISABLE_WLAN_MIPS16 1
#define CONFIG_RTL_819X_SWCORE 1
#define CONFIG_DEFAULTS_KERNEL_2_6 1
#define CONFIG_RTL_IGMP_PROXY 1
#define CONFIG_RTL_IGMP_SNOOPING 1
#define CONFIG_RTL_MLD_SNOOPING 1
#define CONFIG_RTL_QUERIER_SELECTION 1
#define CONFIG_RTL_MLD_PROXY 1
#define CONFIG_RTL_LINKCHG_PROCESS 1
#define CONFIG_RTL_DEBUG_TOOL 1
#define CONFIG_RTL_LAYERED_DRIVER 1
#define CONFIG_RTL_LAYERED_ASIC_DRIVER 1
#define CONFIG_RTL_LAYERED_ASIC_DRIVER_L3 1
#define CONFIG_RTL_LAYERED_ASIC_DRIVER_L4 1
#define CONFIG_RTL_LAYERED_DRIVER_ACL 1
#define CONFIG_RTL_LAYERED_DRIVER_L2 1
#define CONFIG_RTL_LAYERED_DRIVER_L3 1
#define CONFIG_RTL_LAYERED_DRIVER_L4 1
#define CONFIG_RTL_EXTPORT_VLANID 9
#define CONFIG_RTL_HW_NAPT 1
#define CONFIG_RTL_TSO 1
#define CONFIG_RTL_GSO 1
#define CONFIG_RTL865X_MBUF_HEADROOM 128
#define CONFIG_RTL_ETH_PRIV_SKB 1
#define CONFIG_EXTRTL8212_PHYID_P1 18
#define CONFIG_EXTRTL8212_PHYID_P3 20
#define CONFIG_EXTRTL8212_PHYID_P5 22
#define CONFIG_RTL_HARDWARE_MULTICAST 1
#define CONFIG_RTL_HARDWARE_NAT 1
#define CONFIG_RTL_PPPOE_HWACC 1
#define CONFIG_RTL_PROC_DEBUG 1
#define CONFIG_RTL_EEE_DISABLED 1
#define CONFIG_RT_MULTIPLE_BR_SUPPORT 1
#define CONFIG_RTL_MULTICAST_PORT_MAPPING 1
#define CONFIG_RTL_DISABLE_ETH_MIPS16 1
#define CONFIG_RTL_8211F_SUPPORT 1
#define CONFIG_TTY 1
#define CONFIG_LEGACY_PTYS 1
#define CONFIG_LEGACY_PTY_COUNT 256
#define CONFIG_SERIAL_8250 1
#define CONFIG_SERIAL_8250_CONSOLE 1
#define CONFIG_SERIAL_8250_NR_UARTS 2
#define CONFIG_SERIAL_8250_RUNTIME_UARTS 1
#define CONFIG_SERIAL_8250_DW 1
#define CONFIG_SERIAL_CORE 1
#define CONFIG_SERIAL_CORE_CONSOLE 1
#define CONFIG_RTL8686_ASIC 1
#define CONFIG_SPI 1
#define CONFIG_SPI_MASTER 1
#define CONFIG_SPI_SHEIPA 1
#define CONFIG_ARCH_REQUIRE_GPIOLIB 1
#define CONFIG_GPIO_DEVRES 1
#define CONFIG_GPIOLIB 1
#define CONFIG_GPIO_SYSFS 1
#define CONFIG_SSB_POSSIBLE 1
#define CONFIG_BCMA_POSSIBLE 1
#define CONFIG_RTC_LIB 1
#define CONFIG_CLKDEV_LOOKUP 1
#define CONFIG_FILE_LOCKING 1
#define CONFIG_PROC_FS 1
#define CONFIG_PROC_SYSCTL 1
#define CONFIG_PROC_PAGE_MONITOR 1
#define CONFIG_SYSFS 1
#define CONFIG_CONFIGFS_FS 1
#define CONFIG_MISC_FILESYSTEMS 1
#define CONFIG_SQUASHFS 1
#define CONFIG_SQUASHFS_XZ 1
#define CONFIG_SQUASHFS_EMBEDDED 1
#define CONFIG_SQUASHFS_FRAGMENT_CACHE_SIZE 3
#define CONFIG_TRACE_IRQFLAGS_SUPPORT 1
#define CONFIG_DEFAULT_MESSAGE_LOGLEVEL 4
#define CONFIG_ENABLE_WARN_DEPRECATED 1
#define CONFIG_ENABLE_MUST_CHECK 1
#define CONFIG_FRAME_WARN 1024
#define CONFIG_STRIP_ASM_SYMS 1
#define CONFIG_DEBUG_KERNEL 1
#define CONFIG_PANIC_ON_OOPS_VALUE 0
#define CONFIG_SCHED_DEBUG 1
#define CONFIG_HAVE_DEBUG_KMEMLEAK 1
#define CONFIG_HAVE_FUNCTION_TRACER 1
#define CONFIG_HAVE_FUNCTION_GRAPH_TRACER 1
#define CONFIG_HAVE_FUNCTION_TRACE_MCOUNT_TEST 1
#define CONFIG_HAVE_DYNAMIC_FTRACE 1
#define CONFIG_HAVE_FTRACE_MCOUNT_RECORD 1
#define CONFIG_HAVE_C_RECORDMCOUNT 1
#define CONFIG_TRACING_SUPPORT 1
#define CONFIG_HAVE_ARCH_KGDB 1
#define CONFIG_EARLY_PRINTK 1
#define CONFIG_CMDLINE_BOOL 1
#define CONFIG_CMDLINE "console=ttyS0,38400 root=/dev/mtdblock1"
#define CONFIG_CMDLINE_OVERRIDE 1
#define CONFIG_DEFAULT_SECURITY_DAC 1
#define CONFIG_DEFAULT_SECURITY ""
#define CONFIG_CRYPTO 1
#define CONFIG_CRYPTO_ALGAPI 1
#define CONFIG_CRYPTO_ALGAPI2 1
#define CONFIG_CRYPTO_BLKCIPHER 1
#define CONFIG_CRYPTO_BLKCIPHER2 1
#define CONFIG_CRYPTO_HASH 1
#define CONFIG_CRYPTO_HASH2 1
#define CONFIG_CRYPTO_RNG2 1
#define CONFIG_CRYPTO_WORKQUEUE 1
#define CONFIG_CRYPTO_SHA1 1
#define CONFIG_CRYPTO_AES 1
#define CONFIG_CRYPTO_ARC4 1
#define CONFIG_CRYPTO_HW 1
#define CONFIG_BITREVERSE 1
#define CONFIG_GENERIC_PCI_IOMAP 1
#define CONFIG_GENERIC_IO 1
#define CONFIG_CRC_CCITT 1
#define CONFIG_CRC32 1
#define CONFIG_CRC32_SLICEBY8 1
#define CONFIG_XZ_DEC 1
#define CONFIG_TEXTSEARCH 1
#define CONFIG_TEXTSEARCH_KMP 1
#define CONFIG_TEXTSEARCH_BM 1
#define CONFIG_TEXTSEARCH_FSM 1
#define CONFIG_HAS_IOMEM 1
#define CONFIG_HAS_IOPORT 1
#define CONFIG_HAS_DMA 1
#define CONFIG_DQL 1
#define CONFIG_NLATTR 1
#define CONFIG_GENERIC_ATOMIC64 1
#define CONFIG_ARCH_HAS_ATOMIC64_DEC_IF_POSITIVE 1
#define CONFIG_HAVE_KVM 1
#define CONFIG_HAVE_DOT_CONFIG 1
#define CONFIG_FEATURE_BUFFERS_USE_MALLOC 1
#define CONFIG_FEATURE_PREFER_APPLETS 1
#define CONFIG_BUSYBOX_EXEC_PATH "/proc/self/exe"
#define CONFIG_FEATURE_SYSLOG 1
#define CONFIG_CROSS_COMPILER_PREFIX "rsdk-linux-"
#define CONFIG_NO_DEBUG_LIB 1
#define CONFIG_INSTALL_APPLET_SYMLINKS 1
#define CONFIG_PREFIX "../../romfs"
#define CONFIG_PASSWORD_MINLEN 6
#define CONFIG_MD5_SIZE_VS_SPEED 2
#define CONFIG_FEATURE_EDITING 1
#define CONFIG_FEATURE_EDITING_MAX_LEN 1024
#define CONFIG_FEATURE_EDITING_HISTORY 15
#define CONFIG_FEATURE_TAB_COMPLETION 1
#define CONFIG_FEATURE_COPYBUF_KB 4
#define CONFIG_IOCTL_HEX2STR_ERROR 1
#define CONFIG_BUNZIP2 1
#define CONFIG_CAT 1
#define CONFIG_CP 1
#define CONFIG_CUT 1
#define CONFIG_DATE 1
#define CONFIG_FEATURE_DATE_ISOFMT 1
#define CONFIG_ECHO 1
#define CONFIG_FEATURE_FANCY_ECHO 1
#define CONFIG_EXPR 1
#define CONFIG_FALSE 1
#define CONFIG_HEAD 1
#define CONFIG_LN 1
#define CONFIG_LS 1
#define CONFIG_FEATURE_LS_FILETYPES 1
#define CONFIG_FEATURE_LS_FOLLOWLINKS 1
#define CONFIG_FEATURE_LS_RECURSIVE 1
#define CONFIG_FEATURE_LS_SORTFILES 1
#define CONFIG_FEATURE_LS_TIMESTAMPS 1
#define CONFIG_FEATURE_LS_USERNAME 1
#define CONFIG_MKDIR 1
#define CONFIG_RM 1
#define CONFIG_SLEEP 1
#define CONFIG_TAIL 1
#define CONFIG_TRUE 1
#define CONFIG_WC 1
#define CONFIG_DEFAULT_SETFONT_DIR ""
#define CONFIG_FEATURE_VI_MAX_LEN 
#define CONFIG_GREP 1
#define CONFIG_INIT 1
#define CONFIG_FEATURE_USE_INITTAB 1
#define CONFIG_FEATURE_KILL_DELAY 0
#define CONFIG_FEATURE_INIT_SCTTY 1
#define CONFIG_FEATURE_INIT_SYSLOG 1
#define CONFIG_HALT 1
#define CONFIG_DEFAULT_MODULES_DIR "/lib/modules"
#define CONFIG_DEFAULT_DEPMOD_FILE "modules.dep"
#define CONFIG_INSMOD 1
#define CONFIG_RMMOD 1
#define CONFIG_LSMOD 1
#define CONFIG_FEATURE_LSMOD_PRETTY_2_6_OUTPUT 1
#define CONFIG_MODPROBE 1
#define CONFIG_FEATURE_MODPROBE_BLACKLIST 1
#define CONFIG_DEPMOD 1
#define CONFIG_FEATURE_MODUTILS_ALIAS 1
#define CONFIG_FEATURE_MODUTILS_SYMBOLS 1
#define CONFIG_FEATURE_USE_TERMIOS 1
#define CONFIG_MOUNT 1
#define CONFIG_FEATURE_MOUNT_FAKE 1
#define CONFIG_FEATURE_MOUNT_VERBOSE 1
#define CONFIG_UMOUNT 1
#define CONFIG_FEATURE_LESS_MAXLINES 
#define CONFIG_HOSTNAME 1
#define CONFIG_IFCONFIG 1
#define CONFIG_FEATURE_IFCONFIG_STATUS 1
#define CONFIG_FEATURE_IFCONFIG_HW 1
#define CONFIG_IFUPDOWN_IFSTATE_PATH ""
#define CONFIG_IP 1
#define CONFIG_FEATURE_IP_TUNNEL 1
#define CONFIG_PING 1
#define CONFIG_FEATURE_FANCY_PING 1
#define CONFIG_ROUTE 1
#define CONFIG_DHCPD_LEASES_FILE ""
#define CONFIG_UDHCPC_DEFAULT_SCRIPT ""
#define CONFIG_UDHCPC_SLACK_FOR_BUGGY_SERVERS 
#define CONFIG_VCONFIG 1
#define CONFIG_FEATURE_MIME_CHARSET ""
#define CONFIG_FREE 1
#define CONFIG_KILL 1
#define CONFIG_KILLALL 1
#define CONFIG_PS 1
#define CONFIG_RENICE 1
#define CONFIG_TOP 1
#define CONFIG_FEATURE_TOP_CPU_USAGE_PERCENTAGE 1
#define CONFIG_FEATURE_TOP_CPU_GLOBAL_PERCENTS 1
#define CONFIG_FEATURE_TOP_SMP_CPU 1
#define CONFIG_FEATURE_TOP_DECIMALS 1
#define CONFIG_FEATURE_TOP_SMP_PROCESS 1
#define CONFIG_FEATURE_TOPMEM 1
#define CONFIG_UPTIME 1
#define CONFIG_SV_DEFAULT_SERVICE_DIR ""
#define CONFIG_FEATURE_SH_IS_ASH 1
#define CONFIG_ASH 1
#define CONFIG_ASH_BASH_COMPAT 1
#define CONFIG_ASH_JOB_CONTROL 1
#define CONFIG_ASH_ALIAS 1
#define CONFIG_ASH_MATH_SUPPORT 1
#define CONFIG_ASH_BUILTIN_ECHO 1
#define CONFIG_ASH_BUILTIN_PRINTF 1
#define CONFIG_ASH_BUILTIN_TEST 1
#define CONFIG_ASH_OPTIMIZE_FOR_SIZE 1
#define CONFIG_FEATURE_SH_EXTRA_QUIET 1
#define CONFIG_SYSLOGD 1
#define CONFIG_FEATURE_ROTATE_LOGFILE 1
#define CONFIG_FEATURE_REMOTE_LOG 1
#define CONFIG_FEATURE_SYSLOGD_DUP 1
#define CONFIG_FEATURE_IPC_SYSLOG 1
#define CONFIG_FEATURE_IPC_SYSLOG_BUFFER_SIZE 16
#define CONFIG_KLOGD 1