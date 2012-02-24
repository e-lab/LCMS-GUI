# The default configuration is "debug" if invoked with just "make":
#
ifeq ($(CFG),)
    CFG=debug
endif

# The directories containing the source files, separated by ':'
VPATH=SourceCode

# The source files: regardless of where they reside in the source tree,
# VPATH will locate them...
SOURCE = \
	DeviceInterface.cpp \
	DeviceController.cpp \
	DeviceEvent.cpp \
	GraphicsImage.cpp \
	GraphicsImageCanvas.cpp \
	GraphicsInformation.cpp \
	Logo.cpp \
	MainFrame.cpp \
	Logging.cpp \
	ControlBiases.cpp \
	ControlView.cpp \
	main.cpp \
	okFrontPanelDLL.cpp

	#GraphicsPlot.cpp \
	GraphicsPlotData.cpp \
	mathplot.cpp \

# Build a Dependency list and an Object list, by replacing the .cpp
# extension to .d for dependency files, and .o for object files.
#DEPENDENCY = $(patsubst %.cpp, deps/Group0_%.d, ${SOURCE})
OBJECTS = $(patsubst %.cpp, $(CFG)/%.o, ${SOURCE})

# Your final binary
TARGET=LCMS

# What compiler to use for generating dependencies: it will be invoked with -MM
#CXXDEP = g++
CXX = g++

# What include flags to pass to the compiler
ifeq ($(CFG),debug)
INCLUDEFLAGS=-I$(WXWIN)/debug/lib/wx/include/mac-unicode-debug-2.8 -I$(WXWIN)/include -D__WXMAC__ -D__WXDEBUG__ -DMACOSX 
else
INCLUDEFLAGS=-I$(WXWIN)/release/lib/wx/include/mac-unicode-release-static-2.8 -I$(WXWIN)/include -D__WXMAC__ -DMACOSX 
endif

# Separate compile options per configuration
ifeq ($(CFG),debug)
CXXFLAGS += -O2 -Wall -g ${INCLUDEFLAGS}
else
CXXFLAGS += -O2 -Wall ${INCLUDEFLAGS}
endif

# A common link flag for all configurations
ifeq ($(CFG),debug)
LDFLAGS = -L$(WXWIN)/debug/lib  -lwx_macud_core-2.8  -lwx_base_carbonud-2.8  
else
LDFLAGS = -L$(WXWIN)/release/lib -liconv  -lwx_macu_core-2.8  -lwx_base_carbonu-2.8 -framework IOKit -framework Carbon -framework Cocoa -framework QuickTime -framework OpenGL  
endif

all:	inform Application/${TARGET}

inform:
ifneq ($(CFG),release)
ifneq ($(CFG),debug)
	@echo "Invalid configuration "$(CFG)" specified."
	@echo "You must specify a configuration when running make, e.g."
	@echo  "make CFG=debug"
	@echo  
	@echo  "Possible choices for configuration are 'release' and 'debug'"
	@exit 1
endif
endif
	@echo "Configuration "$(CFG)
	@echo "------------------------"

Application/${TARGET}: ${OBJECTS} | inform
	@mkdir -p Application
	$(CXX) -g -o $@ $^ ${LDFLAGS}
	@mkdir -p Application/$(TARGET).app/Contents
	@mkdir -p Application/$(TARGET).app/Contents/MacOS
	@mkdir -p Application/$(TARGET).app/Contents/Resources
	@cp Application/$(TARGET) Application/$(TARGET).app/Contents/MacOS/
	@cp Application/$(TARGET).bit Application/$(TARGET).app/Contents/Resources/
	@cp Application/libokFrontPanel.dylib Application/$(TARGET).app/Contents/Resources/


$(CFG)/%.o: %.cpp
	@mkdir -p $(CFG)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

#deps/Group0_%.d: %.cpp
#	@mkdir -p deps
#	@echo Generating dependencies for $<
#	@set -e ; $(CXXDEP) -MM $(INCLUDEFLAGS) $< > $@.$$$$; \
#	sed 's,\($*\)\.o[ :]*,objs.$(CFG)\/Group0_\1.o $@ : ,g' < $@.$$$$ > $@; \
#	rm -f $@.$$$$

.PHONY: clean

clean:
	@rm -rf debug/* release/* Application/$(TARGET) Application/$(TARGET).app

# Unless "make clean" is called, include the dependency files
# which are auto-generated. Don't fail if they are missing
# (-include), since they will be missing in the first invocation!
#ifneq ($(MAKECMDGOALS),clean)
#-include ${DEPENDENCY}
#endif
