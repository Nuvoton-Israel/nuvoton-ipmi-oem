AM_DEFAULT_SOURCE_EXT = .cpp

libnuvoemcmdsdir = ${libdir}/ipmid-providers
libnuvoemcmds_LTLIBRARIES = libnuvoemcmds.la
libnuvoemcmds_la_SOURCES = \
  oemcommands.cpp \
  ipmi_pwm.cpp \
  ipmi_fw.cpp \
  gpio_oem.cpp
libnuvoemcmds_la_CXXFLAGS = \
	$(SDBUSPLUS_CFLAGS) \
	$(LIBIPMID_CFLAGS) \
	$(PHOSPHOR_LOGGING_CFLAGS) \
	-flto
libnuvoemcmds_la_LDFLAGS = \
	$(SDBUSPLUS_LIBS) \
	$(LIBIPMID_LIBS) \
	$(PHOSPHOR_LOGGING_LIBS) \
	-lstdc++fs \
	-version-info 0:0:0 -shared

