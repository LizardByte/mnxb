XBE_TITLE = Moonlight
OUTPUT_DIR = $(CURDIR)/build
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(CURDIR)/src/main.cpp
NXDK_DIR ?= $(CURDIR)/third-party/nxdk
NXDK_CXX = y

include $(NXDK_DIR)/Makefile
