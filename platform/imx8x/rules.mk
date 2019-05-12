LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

WITH_SMP := 1
#LK_HEAP_IMPLEMENTATION ?= dlmalloc

MODULE_DEPS := \
	dev/timer/arm_generic \
	lib/cbuf

MODULE_SRCS += \
	$(LOCAL_DIR)/lpuart.c \
	$(LOCAL_DIR)/platform.c

MEMBASE := 0x80000000

GLOBAL_DEFINES += \
	ARM_ARCH_WAIT_FOR_SECONDARIES=1

LINKER_SCRIPT += \
	$(BUILDDIR)/system-onesegment.ld

ARCH := arm64
ARM_CPU := cortex-a35

KERNEL_LOAD_OFFSET := 0x00100000
MEMSIZE ?= 0x40000000 # 1GB
LK_DEBUGLEVEL = 10

GLOBAL_DEFINES += \
    MEMBASE=$(MEMBASE) \
    MEMSIZE=$(MEMSIZE)

MODULE_DEPS += \
		app/shell \
	    app/tests \
	    lib/fdt \
    	dev/interrupt/arm_gic \
    	dev/timer/arm_generic

WITH_CPP_SUPPORT=true

include make/module.mk
