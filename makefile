GPP = g++

OUTFILE = "plugins/gmtime.so"

COMPILE_FLAGS = -c -O3 -w -DLINUX -I./SDK/amx/
#COMPILE_FLAGS = -c -O3 -fno-strict-aliasing -w -DLINUX -I./SDK/amx/

all:
	$(GPP) $(COMPILE_FLAGS) ./SDK/*.cpp
	$(GPP) $(COMPILE_FLAGS) ./*.cpp
	$(GPP) -O3 -fshort-wchar -shared -o $(OUTFILE) *.o
	-rm -f *~ *.o
