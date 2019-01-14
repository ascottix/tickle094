PLAIN_OBJECTS = \
	dib32.o \
	dx_app.o \
	dx_joystick.o \
	main.o \
	resource.o \
	win_frame.o

OBJECTS = $(addprefix $(OBJDIR),$(PLAIN_OBJECTS))

RC = windres
LD = g++
LDFLAGS = -s -mwindows -WI -Bstatic -lgcc -static-libgcc
LIBS = -ldsound -lddraw -ldinput -ldxguid -lcomctl32

EMULIBS = $(addprefix $(OBJDIR)../,machine.a cpu.a emu.a sound.a ase.a z.a)

LIBS += $(EMULIBS)

TICKLE = tickle.exe

target: $(OBJDIR)$(TICKLE)

$(OBJDIR)$(TICKLE): $(OBJECTS) $(EMULIBS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(OBJDIR)resource.o: resource.rc
	$(RC) $< -o $@ -O coff

$(OBJDIR)%.o : %.cxx
	$(CC) -DGCC -DWIN32 -DWINNT -DDIRECTINPUT_VERSION=0x800 $(CC_FLAGS) -I./.. -c $< -o $@
