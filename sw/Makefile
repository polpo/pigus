include vendor/circle-stdlib/Config.mk

CIRCLEHOME = vendor/circle-stdlib/libs/circle
NEWLIBDIR = vendor/circle-stdlib/install/$(NEWLIB_ARCH)

OBJS	= main.o kernel.o soundcardemu.o gusemu.o gustimer.o gus.o soft_limiter.o # vendor/speex/resample.o

OPTIMIZE = -O3

include $(CIRCLEHOME)/Rules.mk

# CFLAGS += -I "vendor/speex/"

# CFLAGS += -DUSE_INTERRUPTS
# CFLAGS += -DUSE_HYBRID_POLLING
# CFLAGS += -DUSE_BUFFERED_SCREEN
# CFLAGS += -D_USE_NEON -DOUTSIDE_SPEEX -DRANDOM_PREFIX=PIGUS -DFLOATING_POINT -DEXPORT=/**/

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
