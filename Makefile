LIBBUTANOABS := $(CURDIR)/butano

TARGET := hextris_advance_gba
ROMTITLE := HEXTRISADV
ROMCODE := HXTA

BUILD := build
SOURCES := gba/src common/src
INCLUDES := gba/src common/include

# BGM is intentionally omitted on GBA port.
AUDIO :=
DMGAUDIO :=

include $(LIBBUTANOABS)/butano.mak
