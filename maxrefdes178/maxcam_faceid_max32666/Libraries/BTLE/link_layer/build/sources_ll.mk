################################################################################
# Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
# OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# Except as contained in this notice, the name of Maxim Integrated
# Products, Inc. shall not be used except as stated in the Maxim Integrated
# Products, Inc. Branding Policy.
#
# The mere transfer of this software does not imply any licenses
# of trade secrets, proprietary technology, copyrights, patents,
# trademarks, maskwork rights, or any other form of intellectual
# property whatsoever. Maxim Integrated Products, Inc. retains all
# ownership rights.
#
# $Id: sources_ll.mk 53529 2020-05-16 06:01:32Z nathan.goldstick $
#
################################################################################

ifeq "$(CORDIO_DIR)" ""
$(error "CORDIO_DIR must be specified")
endif

ifneq "$(COMPILER)" "RCS"
ifeq "$(MFLOAT_FLAGS)" "hardfp"
LINKLAYER_LIB_VARIANT=hardfp
else
ifeq "$(MFLOAT_FLAGS)" "hard"
LINKLAYER_LIB_VARIANT=hardfp
else
LINKLAYER_LIB_VARIANT=softfp
endif
endif
endif

LINKLAYER_DIR       := ${CORDIO_DIR}/link_layer
LINKLAYER_BUILD_DIR := ${LINKLAYER_DIR}/build
LINKLAYER_LIB_DIR   := ${LINKLAYER_BUILD_DIR}/${COMPILER}/library
ifeq "$(LINKLAYER_LIB_VARIANT)" ""
LINKLAYER_LIB       := ${LINKLAYER_LIB_DIR}/liblinklayer.a
else
LINKLAYER_LIB       := ${LINKLAYER_LIB_DIR}/liblinklayer_$(LINKLAYER_LIB_VARIANT).a
endif

WSF_DIR             := ${CORDIO_DIR}/wsf

LINKLAYER_INC_DIRS += \
	$(WSF_DIR)/include \
	$(WSF_DIR)/include/util \
	$(LINKLAYER_DIR) \
	$(LINKLAYER_DIR)/platform \
	$(LINKLAYER_DIR)/platform/max32665 \
	$(LINKLAYER_DIR)/platform/common \
	$(LINKLAYER_DIR)/platform/common/include \
	$(LINKLAYER_DIR)/platform/common/sources \
	$(LINKLAYER_DIR)/platform/common/sources/chci \
	$(LINKLAYER_DIR)/platform/common/sources/bb \
	$(LINKLAYER_DIR)/platform/common/sources/bb/ble \
	$(LINKLAYER_DIR)/platform/common/sources/ll \
	$(LINKLAYER_DIR)/controller \
	$(LINKLAYER_DIR)/controller/include \
	$(LINKLAYER_DIR)/controller/include/common \
	$(LINKLAYER_DIR)/controller/include/ble \
	$(LINKLAYER_DIR)/controller/sources \
	$(LINKLAYER_DIR)/controller/sources/common \
	$(LINKLAYER_DIR)/controller/sources/common/bb \
	$(LINKLAYER_DIR)/controller/sources/common/sch \
	$(LINKLAYER_DIR)/controller/sources/ble \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci \
	$(LINKLAYER_DIR)/controller/sources/ble/init \
	$(LINKLAYER_DIR)/controller/sources/ble/bb \
	$(LINKLAYER_DIR)/controller/sources/ble/sch \
	$(LINKLAYER_DIR)/controller/sources/ble/include \
	$(LINKLAYER_DIR)/controller/sources/ble/ll \
	$(LINKLAYER_DIR)/controller/sources/ble/lmgr \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr \
	$(LINKLAYER_DIR)/thirdparty \
	$(LINKLAYER_DIR)/thirdparty/uecc

LINKLAYER_H_FILES += $(shell find $(LINKLAYER_INC_DIRS) -name '*.h')

LINKLAYER_C_FILES += \
	$(LINKLAYER_DIR)/platform/max32665/chci_drv.c \
	$(LINKLAYER_DIR)/platform/max32665/ll_tester.c \
	$(LINKLAYER_DIR)/platform/max32665/lhci_vs.c \
	$(LINKLAYER_DIR)/platform/max32665/ll_math_aes.c \
	$(LINKLAYER_DIR)/platform/common/sources/chci/chci_tr_serial.c \
	$(LINKLAYER_DIR)/platform/common/sources/chci/chci_tr.c \
	$(LINKLAYER_DIR)/platform/common/sources/bb/ble/bb_ble_pdufilt.c \
	$(LINKLAYER_DIR)/platform/common/sources/bb/ble/bb_ble_reslist.c \
	$(LINKLAYER_DIR)/platform/common/sources/bb/ble/bb_ble_whitelist.c \
	$(LINKLAYER_DIR)/platform/common/sources/bb/ble/bb_ble_periodiclist.c \
	$(LINKLAYER_DIR)/platform/common/sources/ll/ll_math_ecc.c \
	$(LINKLAYER_DIR)/platform/common/sources/ll/lhci_cmd_vs_ext.c \
	$(LINKLAYER_DIR)/controller/sources/common/bb/bb_main.c \
	$(LINKLAYER_DIR)/controller/sources/common/sch/sch_main.c \
	$(LINKLAYER_DIR)/controller/sources/common/sch/sch_list.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_vs.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_phy.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_adv_priv.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_adv_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_adv_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_enc_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_adv_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_phy.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_main.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_adv_priv.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_conn_cs2.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_conn_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_adv_slave_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_sc.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_enc_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_conn_priv.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_enc_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_conn_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_conn_cs2.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_enc_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_adv_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_conn_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_conn_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_sc.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_enc_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_conn_priv.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_sc.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_conn.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_conn.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_conn_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_conn.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_enc_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_adv_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_cmd_conn_priv.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_adv_slave_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_adv_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_init_phy.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_adv_slave_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lhci/lhci_evt_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/bb/bb_ble_main.c \
	$(LINKLAYER_DIR)/controller/sources/ble/bb/bb_ble_adv_slave_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/bb/bb_ble_adv_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/bb/bb_ble_adv_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/bb/bb_ble_conn_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/bb/bb_ble_conn_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/bb/bb_ble_test.c \
	$(LINKLAYER_DIR)/controller/sources/ble/bb/bb_ble_conn.c \
	$(LINKLAYER_DIR)/controller/sources/ble/bb/bb_ble_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/sch/sch_ble.c \
	$(LINKLAYER_DIR)/controller/sources/ble/sch/sch_rm.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_conn_cs2.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_adv_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_phy.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_adv_slave_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_sc.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_sc.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_master_phy.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_conn_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_priv.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_adv_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_conn_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_conn_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_conn_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_adv_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_init_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_dtm.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_priv.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_conn_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_adv_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_conn.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_enc_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_diag.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_slave_phy.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_enc_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_math.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_enc_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_adv_slave_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_main_enc_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/ll/ll_init_init_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lmgr/lmgr_main_priv.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lmgr/lmgr_main_adv_slave_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lmgr/lmgr_main_conn.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lmgr/lmgr_main.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lmgr/lmgr_events.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lmgr/lmgr_main_sc.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lmgr/lmgr_main_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lmgr/lmgr_main_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lmgr/lmgr_main_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_conn_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_adv_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_conn_cs2.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_conn.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_phy.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_isr_init_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_isr_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_isr_conn.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_pdu_enc.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_pdu_conn.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_adv_slave_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_enc_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_conn_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_adv_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_priv.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_isr_init_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_adv_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_conn_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_slave_phy.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_conn_data.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_llcp_conn_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_pdu_adv_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_isr_adv_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_conn.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_pdu_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_adv_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_init_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_init_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_conn_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_enc_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_adv_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_adv_slave_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_llcp_master_phy.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_llcp_slave_phy.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_init_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_adv_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_conn_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_init_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_llcp_conn.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_llcp_conn_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_init_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_adv_master_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_isr_adv_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_isr_conn_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_master_phy.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_llcp_enc_slave.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_enc_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_isr_conn_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_enc.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_init_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_conn_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_main_sc.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_isr_adv_slave_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_pdu_adv_slave_ae.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_sm_llcp_enc_master.c \
	$(LINKLAYER_DIR)/controller/sources/ble/lctr/lctr_act_adv_slave_ae.c \
	$(LINKLAYER_DIR)/thirdparty/uecc/uECC_ll.c 
