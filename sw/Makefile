include vendor/circle-stdlib/Config.mk

CIRCLEHOME = vendor/circle-stdlib/libs/circle
NEWLIBDIR = vendor/circle-stdlib/install/$(NEWLIB_ARCH)

OBJS	= main.o kernel.o soundcardemu.o adlibemu.o opl.o

include $(CIRCLEHOME)/Rules.mk

CFLAGS += -I "$(NEWLIBDIR)/include" -I $(STDDEF_INCPATH) -I vendor/circle-stdlib/include
LIBS := $(NEWLIBDIR)/lib/libm.a \
	$(NEWLIBDIR)/lib/libc.a \
	$(NEWLIBDIR)/lib/libcirclenewlib.a \
	$(CIRCLEHOME)/lib/sched/libsched.a \
 	$(CIRCLEHOME)/addon/fatfs/libfatfs.a \
  	$(CIRCLEHOME)/lib/usb/libusb.a \
	$(CIRCLEHOME)/lib/fs/fat/libfatfs.a \
 	$(CIRCLEHOME)/lib/fs/libfs.a \
 	$(CIRCLEHOME)/lib/input/libinput.a \
  	$(CIRCLEHOME)/lib/libcircle.a
#\
#	../../littlevgl/liblvgl.a \
#	../../littlevgl/liblv_examples.a \
# 	$(CIRCLEHOME)/addon/SDCard/libsdcard.a \
#  	$(CIRCLEHOME)/lib/usb/libusb.a \
# 	$(CIRCLEHOME)/lib/input/libinput.a \
# 	$(CIRCLEHOME)/lib/fs/fat/libfatfs.a \
# 	$(CIRCLEHOME)/lib/fs/libfs.a \
#  	$(CIRCLEHOME)/lib/net/libnet.a \
#  	$(CIRCLEHOME)/lib/sched/libsched.a \
