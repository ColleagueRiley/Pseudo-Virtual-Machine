buildLinux:
	@gcc source/main.c -I include -lX11 -o RVM

buildWindows:
	@gcc source/main.c -I include -lgdi -lshell32 -o RVM.exe
	
test:	
	./RVM test.rop

debug:
	make buildLinux
	make test