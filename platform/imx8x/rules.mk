LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

WITH_SMP := 0
LK_HEAP_IMPLEMENTATION ?= dlmalloc


MODULE_DEPS := \
	dev/timer/arm_generic \
    dev/interrupt/arm_gic \
	lib/cbuf \
	lib/watchdog

MODULE_SRCS += \
	$(LOCAL_DIR)/lpuart.c \
	$(LOCAL_DIR)/wdog.c \
	$(LOCAL_DIR)/platform.c \
	$(LOCAL_DIR)/sci/mx8_mu.c \
	$(LOCAL_DIR)/sci/ipc.c \
	$(LOCAL_DIR)/sci/svc/misc/misc_rpc_clnt.c \
	$(LOCAL_DIR)/sci/svc/pad/pad_rpc_clnt.c \
	$(LOCAL_DIR)/sci/svc/pm/pm_rpc_clnt.c \
	$(LOCAL_DIR)/sci/svc/rm/rm_rpc_clnt.c \
	$(LOCAL_DIR)/sci/svc/timer/timer_rpc_clnt.c

MEMBASE := 0x80000000

GLOBAL_DEFINES += \
	ARM_ARCH_WAIT_FOR_SECONDARIES=0

LINKER_SCRIPT += \
	$(BUILDDIR)/system-onesegment.ld

ARCH := arm64
ARM_CPU := cortex-a35

KERNEL_LOAD_OFFSET := 0x00100000
MEMSIZE ?= 0x40000000 # 1GB
LK_DEBUGLEVEL = 2

GLOBAL_DEFINES += \
    MEMBASE=$(MEMBASE) \
    MEMSIZE=$(MEMSIZE) \
    MMU_WITH_TRAMPOLINE=1

MODULE_DEPS += \
		app/shell \
	    app/tests \
	    lib/fdt \
    	dev/interrupt/arm_gic \
    	dev/timer/arm_generic

WITH_CPP_SUPPORT=true

include make/module.mk
