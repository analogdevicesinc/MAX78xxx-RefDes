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
# $Id: sources_stack.mk 53529 2020-05-16 06:01:32Z nathan.goldstick $
#
################################################################################


ifeq "$(CORDIO_DIR)" ""
$(error "CORDIO_DIR must be specified")
endif

ifneq "$(COMPILER)" "RCS"
ifeq "$(MFLOAT_FLAGS)" "hardfp"
STACK_LIB_VARIANT=hardfp
else
ifeq "$(MFLOAT_FLAGS)" "hard"
STACK_LIB_VARIANT=hardfp
else
STACK_LIB_VARIANT=softfp
endif
endif
endif

STACK_DIR           := ${CORDIO_DIR}/stack
STACK_BUILD_DIR     := ${STACK_DIR}/build
STACK_LIB_DIR       := ${STACK_BUILD_DIR}/${COMPILER}/library
ifeq "$(STACK_LIB_VARIANT)" ""
STACK_LIB           := ${STACK_LIB_DIR}/libstack.a
else
STACK_LIB           := ${STACK_LIB_DIR}/libstack_$(STACK_LIB_VARIANT).a
endif

WSF_DIR             := ${CORDIO_DIR}/wsf

STACK_INC_DIRS = \
	$(CORDIO_DIR) \
	$(WSF_DIR)/include \
	$(WSF_DIR)/include/util \
	$(STACK_DIR)/platform \
	$(STACK_DIR)/platform/max32665 \
	$(STACK_DIR)/platform/include \
	$(STACK_DIR)/ble-profiles \
	$(STACK_DIR)/ble-profiles/include \
	$(STACK_DIR)/ble-profiles/include/app \
	$(STACK_DIR)/ble-profiles/sources \
	$(STACK_DIR)/ble-profiles/sources/apps \
	$(STACK_DIR)/ble-profiles/sources/apps/cycling \
	$(STACK_DIR)/ble-profiles/sources/apps/tag \
	$(STACK_DIR)/ble-profiles/sources/apps/medc \
	$(STACK_DIR)/ble-profiles/sources/apps/hidapp \
	$(STACK_DIR)/ble-profiles/sources/apps/meds \
	$(STACK_DIR)/ble-profiles/sources/apps/gluc \
	$(STACK_DIR)/ble-profiles/sources/apps/datc \
	$(STACK_DIR)/ble-profiles/sources/apps/wdxs \
	$(STACK_DIR)/ble-profiles/sources/apps/fit \
	$(STACK_DIR)/ble-profiles/sources/apps/app \
	$(STACK_DIR)/ble-profiles/sources/apps/app/common \
	$(STACK_DIR)/ble-profiles/sources/apps/dats \
	$(STACK_DIR)/ble-profiles/sources/apps/uribeacon \
	$(STACK_DIR)/ble-profiles/sources/apps/watch \
	$(STACK_DIR)/ble-profiles/sources/apps/sensor \
	$(STACK_DIR)/ble-profiles/sources/profiles \
	$(STACK_DIR)/ble-profiles/sources/profiles/hrps \
	$(STACK_DIR)/ble-profiles/sources/profiles/wsps \
	$(STACK_DIR)/ble-profiles/sources/profiles/cpp \
	$(STACK_DIR)/ble-profiles/sources/profiles/dis \
	$(STACK_DIR)/ble-profiles/sources/profiles/cscp \
	$(STACK_DIR)/ble-profiles/sources/profiles/udsc \
	$(STACK_DIR)/ble-profiles/sources/profiles/gatt \
	$(STACK_DIR)/ble-profiles/sources/profiles/include \
	$(STACK_DIR)/ble-profiles/sources/profiles/scpps \
	$(STACK_DIR)/ble-profiles/sources/profiles/rscp \
	$(STACK_DIR)/ble-profiles/sources/profiles/plxps \
	$(STACK_DIR)/ble-profiles/sources/profiles/blps \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxc \
	$(STACK_DIR)/ble-profiles/sources/profiles/tipc \
	$(STACK_DIR)/ble-profiles/sources/profiles/gap \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxs \
	$(STACK_DIR)/ble-profiles/sources/profiles/htpc \
	$(STACK_DIR)/ble-profiles/sources/profiles/paspc \
	$(STACK_DIR)/ble-profiles/sources/profiles/wpc \
	$(STACK_DIR)/ble-profiles/sources/profiles/glps \
	$(STACK_DIR)/ble-profiles/sources/profiles/uribeacon \
	$(STACK_DIR)/ble-profiles/sources/profiles/hrpc \
	$(STACK_DIR)/ble-profiles/sources/profiles/blpc \
	$(STACK_DIR)/ble-profiles/sources/profiles/plxpc \
	$(STACK_DIR)/ble-profiles/sources/profiles/htps \
	$(STACK_DIR)/ble-profiles/sources/profiles/fmpl \
	$(STACK_DIR)/ble-profiles/sources/profiles/hid \
	$(STACK_DIR)/ble-profiles/sources/profiles/bas \
	$(STACK_DIR)/ble-profiles/sources/profiles/anpc \
	$(STACK_DIR)/ble-profiles/sources/profiles/glpc \
	$(STACK_DIR)/ble-profiles/sources/profiles/sensor \
	$(STACK_DIR)/ble-profiles/sources/profiles/wspc \
	$(STACK_DIR)/ble-profiles/sources/services \
	$(STACK_DIR)/ble-profiles/build \
	$(STACK_DIR)/ble-host \
	$(STACK_DIR)/ble-host/include \
	$(STACK_DIR)/ble-host/sources \
	$(STACK_DIR)/ble-host/sources/sec \
	$(STACK_DIR)/ble-host/sources/sec/pal \
	$(STACK_DIR)/ble-host/sources/sec/common \
	$(STACK_DIR)/ble-host/sources/hci \
	$(STACK_DIR)/ble-host/sources/hci/dual_chip \
	$(STACK_DIR)/ble-host/sources/hci/common \
	$(STACK_DIR)/ble-host/sources/stack \
	$(STACK_DIR)/ble-host/sources/stack/att \
	$(STACK_DIR)/ble-host/sources/stack/l2c \
	$(STACK_DIR)/ble-host/sources/stack/hci \
	$(STACK_DIR)/ble-host/sources/stack/smp \
	$(STACK_DIR)/ble-host/sources/stack/dm \
	$(STACK_DIR)/ble-host/sources/stack/cfg 

STACK_H_FILES += $(shell find $(STACK_INC_DIRS) -name '*.h')

STACK_C_FILES = \
	$(STACK_DIR)/platform/max32665/wsf_os.c \
	$(STACK_DIR)/platform/max32665/wsf_cs.c \
	$(STACK_DIR)/platform/max32665/pal_rtc.c \
	$(STACK_DIR)/platform/max32665/pal_stubs.c \
	$(STACK_DIR)/platform/max32665/pal_sys.c \
	$(STACK_DIR)/ble-profiles/sources/apps/cycling/cycling_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/tag/tag_main_wdxs.c \
	$(STACK_DIR)/ble-profiles/sources/apps/tag/tag_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/medc/medc_blp.c \
	$(STACK_DIR)/ble-profiles/sources/apps/medc/medc_plx.c \
	$(STACK_DIR)/ble-profiles/sources/apps/medc/medc_wsp.c \
	$(STACK_DIR)/ble-profiles/sources/apps/medc/medc_htp.c \
	$(STACK_DIR)/ble-profiles/sources/apps/medc/medc_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/medc/medc_hrp.c \
	$(STACK_DIR)/ble-profiles/sources/apps/medc/medc_glp.c \
	$(STACK_DIR)/ble-profiles/sources/apps/hidapp/hidapp_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/meds/meds_htp.c \
	$(STACK_DIR)/ble-profiles/sources/apps/meds/meds_wsp.c \
	$(STACK_DIR)/ble-profiles/sources/apps/meds/meds_glp.c \
	$(STACK_DIR)/ble-profiles/sources/apps/meds/meds_blp.c \
	$(STACK_DIR)/ble-profiles/sources/apps/meds/meds_plx.c \
	$(STACK_DIR)/ble-profiles/sources/apps/meds/meds_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/gluc/gluc_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/datc/datc_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/fit/fit_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/app_disc.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/app_terminal.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/app_master_leg.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/app_master_ae.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/app_master.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/app_slave_leg.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/app_slave_ae.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/app_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/common/app_hw.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/common/app_db.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/common/app_ui.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/app_slave.c \
	$(STACK_DIR)/ble-profiles/sources/apps/app/app_server.c \
	$(STACK_DIR)/ble-profiles/sources/apps/dats/dats_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/uribeacon/uribeacon_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/watch/watch_main.c \
	$(STACK_DIR)/ble-profiles/sources/apps/sensor/sensor_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/hrps/hrps_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wsps/wsps_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/cpp/cpps_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/dis/dis_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/cscp/cscps_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/udsc/udsc_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/gatt/gatt_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/scpps/scpps_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/rscp/rscps_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/plxps/plxps_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/plxps/plxps_db.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/blps/blps_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxc/wdxc_stream.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxc/wdxc_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxc/wdxc_ft.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxc/wdxc_dc.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/tipc/tipc_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/gap/gap_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxs/wdxs_ft.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxs/wdxs_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxs/wdxs_au.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxs/wdxs_dc.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxs/wdxs_stream.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wdxs/wdxs_phy.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/htpc/htpc_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/paspc/paspc_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wpc/wpc_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/glps/glps_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/glps/glps_db.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/uribeacon/uricfg_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/hrpc/hrpc_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/blpc/blpc_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/plxpc/plxpc_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/htps/htps_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/fmpl/fmpl_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/hid/hid_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/bas/bas_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/anpc/anpc_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/glpc/glpc_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/sensor/temp_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/sensor/gyro_main.c \
	$(STACK_DIR)/ble-profiles/sources/profiles/wspc/wspc_main.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_hrs.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_gls.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_rscs.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_ipss.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_wdxs.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_bps.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_temp.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_uricfg.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_cps.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_batt.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_core.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_scpss.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_hts.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_px.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_gyro.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_wp.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_alert.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_cscs.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_time.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_plxs.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_hid.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_dis.c \
	$(STACK_DIR)/ble-profiles/sources/services/svc_wss.c \
	$(STACK_DIR)/ble-host/sources/sec/pal/sec_ccm.c \
	$(STACK_DIR)/ble-host/sources/sec/common/sec_ccm_hci.c \
	$(STACK_DIR)/ble-host/sources/sec/common/sec_aes.c \
	$(STACK_DIR)/ble-host/sources/sec/common/sec_cmac_hci.c \
	$(STACK_DIR)/ble-host/sources/sec/common/sec_main.c \
	$(STACK_DIR)/ble-host/sources/sec/common/sec_ecc_hci.c \
	$(STACK_DIR)/ble-host/sources/hci/dual_chip/hci_evt.c \
	$(STACK_DIR)/ble-host/sources/hci/dual_chip/hci_vs_ae.c \
	$(STACK_DIR)/ble-host/sources/hci/dual_chip/hci_cmd_ae.c \
	$(STACK_DIR)/ble-host/sources/hci/dual_chip/hci_core_ps.c \
	$(STACK_DIR)/ble-host/sources/hci/dual_chip/hci_cmd_phy.c \
	$(STACK_DIR)/ble-host/sources/hci/dual_chip/hci_cmd_past.c \
	$(STACK_DIR)/ble-host/sources/hci/dual_chip/hci_cmd_cte.c \
	$(STACK_DIR)/ble-host/sources/hci/dual_chip/hci_cmd.c \
	$(STACK_DIR)/ble-host/sources/hci/common/hci_core.c \
	$(STACK_DIR)/ble-host/sources/stack/att/atts_write.c \
	$(STACK_DIR)/ble-host/sources/stack/att/atts_dyn.c \
	$(STACK_DIR)/ble-host/sources/stack/att/attc_main.c \
	$(STACK_DIR)/ble-host/sources/stack/att/atts_csf.c \
	$(STACK_DIR)/ble-host/sources/stack/att/att_uuid.c \
	$(STACK_DIR)/ble-host/sources/stack/att/atts_read.c \
	$(STACK_DIR)/ble-host/sources/stack/att/attc_disc.c \
	$(STACK_DIR)/ble-host/sources/stack/att/attc_sign.c \
	$(STACK_DIR)/ble-host/sources/stack/att/attc_write.c \
	$(STACK_DIR)/ble-host/sources/stack/att/atts_sign.c \
	$(STACK_DIR)/ble-host/sources/stack/att/att_main.c \
	$(STACK_DIR)/ble-host/sources/stack/att/attc_proc.c \
	$(STACK_DIR)/ble-host/sources/stack/att/atts_ccc.c \
	$(STACK_DIR)/ble-host/sources/stack/att/atts_ind.c \
	$(STACK_DIR)/ble-host/sources/stack/att/attc_read.c \
	$(STACK_DIR)/ble-host/sources/stack/att/atts_main.c \
	$(STACK_DIR)/ble-host/sources/stack/att/atts_proc.c \
	$(STACK_DIR)/ble-host/sources/stack/l2c/l2c_slave.c \
	$(STACK_DIR)/ble-host/sources/stack/l2c/l2c_coc.c \
	$(STACK_DIR)/ble-host/sources/stack/l2c/l2c_master.c \
	$(STACK_DIR)/ble-host/sources/stack/l2c/l2c_main.c \
	$(STACK_DIR)/ble-host/sources/stack/hci/hci_main.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smp_db.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smpi_sm.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smpr_sm.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smp_main.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smpr_sc_act.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smpr_act.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smp_sc_act.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smp_non.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smp_sc_main.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smpi_act.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smpr_sc_sm.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smpi_sc_act.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smpi_sc_sm.c \
	$(STACK_DIR)/ble-host/sources/stack/smp/smp_act.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_conn_cte.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_conn_sm.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_conn_slave_leg.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_scan.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_dev.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_priv.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_dev_priv.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_conn.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_adv.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_past.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_adv_ae.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_scan_leg.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_sec.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_sec_lesc.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_conn_slave_ae.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_conn_slave.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_sec_master.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_conn_master_ae.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_scan_ae.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_sync_ae.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_sec_slave.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_adv_leg.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_phy.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_conn_master_leg.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_main.c \
	$(STACK_DIR)/ble-host/sources/stack/dm/dm_conn_master.c \
	$(STACK_DIR)/ble-host/sources/stack/cfg/cfg_stack.c
