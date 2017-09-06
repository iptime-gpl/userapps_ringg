#ifndef __iop_dmc_in_defs_h
#define __iop_dmc_in_defs_h

/*
 * This file is autogenerated from
 *   file:           ../../inst/io_proc/rtl/iop_dmc_in.r
 *     id:           iop_dmc_in.r,v 1.26 2005/02/16 09:14:17 niklaspa Exp
 *     last modfied: Mon Apr 11 16:08:45 2005
 *
 *   by /n/asic/design/tools/rdesc/src/rdes2c --outfile iop_dmc_in_defs.h ../../inst/io_proc/rtl/iop_dmc_in.r
 *      id: $Id: iop_dmc_in_defs.h,v 1.1.1.1 2016/11/01 07:34:09 hyrtlac Exp $
 * Any changes here will be lost.
 *
 * -*- buffer-read-only: t -*-
 */
/* Main access macros */
#ifndef REG_RD
#define REG_RD( scope, inst, reg ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR
#define REG_WR( scope, inst, reg, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_VECT
#define REG_RD_VECT( scope, inst, reg, index ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_VECT
#define REG_WR_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT
#define REG_RD_INT( scope, inst, reg ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR_INT
#define REG_WR_INT( scope, inst, reg, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT_VECT
#define REG_RD_INT_VECT( scope, inst, reg, index ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_INT_VECT
#define REG_WR_INT_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_TYPE_CONV
#define REG_TYPE_CONV( type, orgtype, val ) \
  ( { union { orgtype o; type n; } r; r.o = val; r.n; } )
#endif

#ifndef reg_page_size
#define reg_page_size 8192
#endif

#ifndef REG_ADDR
#define REG_ADDR( scope, inst, reg ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_ADDR_VECT
#define REG_ADDR_VECT( scope, inst, reg, index ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg + \
    (index) * STRIDE_##scope##_##reg )
#endif

/* C-code for register scope iop_dmc_in */

/* Register rw_cfg, scope iop_dmc_in, type rw */
typedef struct {
  unsigned int sth_intr     : 3;
  unsigned int last_dis_dif : 1;
  unsigned int dummy1       : 28;
} reg_iop_dmc_in_rw_cfg;
#define REG_RD_ADDR_iop_dmc_in_rw_cfg 0
#define REG_WR_ADDR_iop_dmc_in_rw_cfg 0

/* Register rw_ctrl, scope iop_dmc_in, type rw */
typedef struct {
  unsigned int dif_en     : 1;
  unsigned int dif_dis    : 1;
  unsigned int stream_clr : 1;
  unsigned int dummy1     : 29;
} reg_iop_dmc_in_rw_ctrl;
#define REG_RD_ADDR_iop_dmc_in_rw_ctrl 4
#define REG_WR_ADDR_iop_dmc_in_rw_ctrl 4

/* Register r_stat, scope iop_dmc_in, type r */
typedef struct {
  unsigned int dif_en : 1;
  unsigned int dummy1 : 31;
} reg_iop_dmc_in_r_stat;
#define REG_RD_ADDR_iop_dmc_in_r_stat 8

/* Register rw_stream_cmd, scope iop_dmc_in, type rw */
typedef struct {
  unsigned int cmd : 10;
  unsigned int dummy1 : 6;
  unsigned int n   : 8;
  unsigned int dummy2 : 8;
} reg_iop_dmc_in_rw_stream_cmd;
#define REG_RD_ADDR_iop_dmc_in_rw_stream_cmd 12
#define REG_WR_ADDR_iop_dmc_in_rw_stream_cmd 12

/* Register rw_stream_wr_data, scope iop_dmc_in, type rw */
typedef unsigned int reg_iop_dmc_in_rw_stream_wr_data;
#define REG_RD_ADDR_iop_dmc_in_rw_stream_wr_data 16
#define REG_WR_ADDR_iop_dmc_in_rw_stream_wr_data 16

/* Register rw_stream_wr_data_last, scope iop_dmc_in, type rw */
typedef unsigned int reg_iop_dmc_in_rw_stream_wr_data_last;
#define REG_RD_ADDR_iop_dmc_in_rw_stream_wr_data_last 20
#define REG_WR_ADDR_iop_dmc_in_rw_stream_wr_data_last 20

/* Register rw_stream_ctrl, scope iop_dmc_in, type rw */
typedef struct {
  unsigned int eop     : 1;
  unsigned int wait    : 1;
  unsigned int keep_md : 1;
  unsigned int size    : 3;
  unsigned int dummy1  : 26;
} reg_iop_dmc_in_rw_stream_ctrl;
#define REG_RD_ADDR_iop_dmc_in_rw_stream_ctrl 24
#define REG_WR_ADDR_iop_dmc_in_rw_stream_ctrl 24

/* Register r_stream_stat, scope iop_dmc_in, type r */
typedef struct {
  unsigned int sth            : 7;
  unsigned int dummy1         : 9;
  unsigned int full           : 1;
  unsigned int last_pkt       : 1;
  unsigned int data_md_valid  : 1;
  unsigned int ctxt_md_valid  : 1;
  unsigned int group_md_valid : 1;
  unsigned int stream_busy    : 1;
  unsigned int cmd_rdy        : 1;
  unsigned int dummy2         : 9;
} reg_iop_dmc_in_r_stream_stat;
#define REG_RD_ADDR_iop_dmc_in_r_stream_stat 28

/* Register r_data_descr, scope iop_dmc_in, type r */
typedef struct {
  unsigned int ctrl : 8;
  unsigned int stat : 8;
  unsigned int md   : 16;
} reg_iop_dmc_in_r_data_descr;
#define REG_RD_ADDR_iop_dmc_in_r_data_descr 32

/* Register r_ctxt_descr, scope iop_dmc_in, type r */
typedef struct {
  unsigned int ctrl : 8;
  unsigned int stat : 8;
  unsigned int md0  : 16;
} reg_iop_dmc_in_r_ctxt_descr;
#define REG_RD_ADDR_iop_dmc_in_r_ctxt_descr 36

/* Register r_ctxt_descr_md1, scope iop_dmc_in, type r */
typedef unsigned int reg_iop_dmc_in_r_ctxt_descr_md1;
#define REG_RD_ADDR_iop_dmc_in_r_ctxt_descr_md1 40

/* Register r_ctxt_descr_md2, scope iop_dmc_in, type r */
typedef unsigned int reg_iop_dmc_in_r_ctxt_descr_md2;
#define REG_RD_ADDR_iop_dmc_in_r_ctxt_descr_md2 44

/* Register r_group_descr, scope iop_dmc_in, type r */
typedef struct {
  unsigned int ctrl : 8;
  unsigned int stat : 8;
  unsigned int md   : 16;
} reg_iop_dmc_in_r_group_descr;
#define REG_RD_ADDR_iop_dmc_in_r_group_descr 56

/* Register rw_data_descr, scope iop_dmc_in, type rw */
typedef struct {
  unsigned int dummy1 : 16;
  unsigned int md : 16;
} reg_iop_dmc_in_rw_data_descr;
#define REG_RD_ADDR_iop_dmc_in_rw_data_descr 60
#define REG_WR_ADDR_iop_dmc_in_rw_data_descr 60

/* Register rw_ctxt_descr, scope iop_dmc_in, type rw */
typedef struct {
  unsigned int dummy1 : 16;
  unsigned int md0 : 16;
} reg_iop_dmc_in_rw_ctxt_descr;
#define REG_RD_ADDR_iop_dmc_in_rw_ctxt_descr 64
#define REG_WR_ADDR_iop_dmc_in_rw_ctxt_descr 64

/* Register rw_ctxt_descr_md1, scope iop_dmc_in, type rw */
typedef unsigned int reg_iop_dmc_in_rw_ctxt_descr_md1;
#define REG_RD_ADDR_iop_dmc_in_rw_ctxt_descr_md1 68
#define REG_WR_ADDR_iop_dmc_in_rw_ctxt_descr_md1 68

/* Register rw_ctxt_descr_md2, scope iop_dmc_in, type rw */
typedef unsigned int reg_iop_dmc_in_rw_ctxt_descr_md2;
#define REG_RD_ADDR_iop_dmc_in_rw_ctxt_descr_md2 72
#define REG_WR_ADDR_iop_dmc_in_rw_ctxt_descr_md2 72

/* Register rw_group_descr, scope iop_dmc_in, type rw */
typedef struct {
  unsigned int dummy1 : 16;
  unsigned int md : 16;
} reg_iop_dmc_in_rw_group_descr;
#define REG_RD_ADDR_iop_dmc_in_rw_group_descr 84
#define REG_WR_ADDR_iop_dmc_in_rw_group_descr 84

/* Register rw_intr_mask, scope iop_dmc_in, type rw */
typedef struct {
  unsigned int data_md  : 1;
  unsigned int ctxt_md  : 1;
  unsigned int group_md : 1;
  unsigned int cmd_rdy  : 1;
  unsigned int sth      : 1;
  unsigned int full     : 1;
  unsigned int dummy1   : 26;
} reg_iop_dmc_in_rw_intr_mask;
#define REG_RD_ADDR_iop_dmc_in_rw_intr_mask 88
#define REG_WR_ADDR_iop_dmc_in_rw_intr_mask 88

/* Register rw_ack_intr, scope iop_dmc_in, type rw */
typedef struct {
  unsigned int data_md  : 1;
  unsigned int ctxt_md  : 1;
  unsigned int group_md : 1;
  unsigned int cmd_rdy  : 1;
  unsigned int sth      : 1;
  unsigned int full     : 1;
  unsigned int dummy1   : 26;
} reg_iop_dmc_in_rw_ack_intr;
#define REG_RD_ADDR_iop_dmc_in_rw_ack_intr 92
#define REG_WR_ADDR_iop_dmc_in_rw_ack_intr 92

/* Register r_intr, scope iop_dmc_in, type r */
typedef struct {
  unsigned int data_md  : 1;
  unsigned int ctxt_md  : 1;
  unsigned int group_md : 1;
  unsigned int cmd_rdy  : 1;
  unsigned int sth      : 1;
  unsigned int full     : 1;
  unsigned int dummy1   : 26;
} reg_iop_dmc_in_r_intr;
#define REG_RD_ADDR_iop_dmc_in_r_intr 96

/* Register r_masked_intr, scope iop_dmc_in, type r */
typedef struct {
  unsigned int data_md  : 1;
  unsigned int ctxt_md  : 1;
  unsigned int group_md : 1;
  unsigned int cmd_rdy  : 1;
  unsigned int sth      : 1;
  unsigned int full     : 1;
  unsigned int dummy1   : 26;
} reg_iop_dmc_in_r_masked_intr;
#define REG_RD_ADDR_iop_dmc_in_r_masked_intr 100


/* Constants */
enum {
  regk_iop_dmc_in_ack_pkt                  = 0x00000100,
  regk_iop_dmc_in_array                    = 0x00000008,
  regk_iop_dmc_in_burst                    = 0x00000020,
  regk_iop_dmc_in_copy_next                = 0x00000010,
  regk_iop_dmc_in_copy_up                  = 0x00000020,
  regk_iop_dmc_in_dis_c                    = 0x00000010,
  regk_iop_dmc_in_dis_g                    = 0x00000020,
  regk_iop_dmc_in_lim1                     = 0x00000000,
  regk_iop_dmc_in_lim16                    = 0x00000004,
  regk_iop_dmc_in_lim2                     = 0x00000001,
  regk_iop_dmc_in_lim32                    = 0x00000005,
  regk_iop_dmc_in_lim4                     = 0x00000002,
  regk_iop_dmc_in_lim64                    = 0x00000006,
  regk_iop_dmc_in_lim8                     = 0x00000003,
  regk_iop_dmc_in_load_c                   = 0x00000200,
  regk_iop_dmc_in_load_c_n                 = 0x00000280,
  regk_iop_dmc_in_load_c_next              = 0x00000240,
  regk_iop_dmc_in_load_d                   = 0x00000140,
  regk_iop_dmc_in_load_g                   = 0x00000300,
  regk_iop_dmc_in_load_g_down              = 0x000003c0,
  regk_iop_dmc_in_load_g_next              = 0x00000340,
  regk_iop_dmc_in_load_g_up                = 0x00000380,
  regk_iop_dmc_in_next_en                  = 0x00000010,
  regk_iop_dmc_in_next_pkt                 = 0x00000010,
  regk_iop_dmc_in_no                       = 0x00000000,
  regk_iop_dmc_in_restore                  = 0x00000020,
  regk_iop_dmc_in_rw_cfg_default           = 0x00000000,
  regk_iop_dmc_in_rw_ctxt_descr_default    = 0x00000000,
  regk_iop_dmc_in_rw_ctxt_descr_md1_default = 0x00000000,
  regk_iop_dmc_in_rw_ctxt_descr_md2_default = 0x00000000,
  regk_iop_dmc_in_rw_data_descr_default    = 0x00000000,
  regk_iop_dmc_in_rw_group_descr_default   = 0x00000000,
  regk_iop_dmc_in_rw_intr_mask_default     = 0x00000000,
  regk_iop_dmc_in_rw_stream_ctrl_default   = 0x00000000,
  regk_iop_dmc_in_save_down                = 0x00000020,
  regk_iop_dmc_in_save_up                  = 0x00000020,
  regk_iop_dmc_in_set_reg                  = 0x00000050,
  regk_iop_dmc_in_set_w_size1              = 0x00000190,
  regk_iop_dmc_in_set_w_size2              = 0x000001a0,
  regk_iop_dmc_in_set_w_size4              = 0x000001c0,
  regk_iop_dmc_in_store_c                  = 0x00000002,
  regk_iop_dmc_in_store_descr              = 0x00000000,
  regk_iop_dmc_in_store_g                  = 0x00000004,
  regk_iop_dmc_in_store_md                 = 0x00000001,
  regk_iop_dmc_in_update_down              = 0x00000020,
  regk_iop_dmc_in_yes                      = 0x00000001
};
#endif /* __iop_dmc_in_defs_h */
