# PAM_LIB_DIR = /lib/x86_64-linux-gnu/security
PAM_LIB_DIR = /lib/security

pam_twauth.so: pam_twauth.o
	ld -lcurl -x --shared -o pam_twauth.so pam_twauth.o twl-ops.o twl-gecos.o twl-curl.o twl-code.o

pam_twauth.o: pam_twauth.c twl-ops.c twl-gecos.c twl-curl.c twl-code.c
	gcc -Wall -fPIC -lcurl -c pam_twauth.c twl-ops.c twl-gecos.c twl-curl.c twl-code.c -static

clean:
	rm -f *.o *.so

install:
	cp pam_twauth.so $(PAM_LIB_DIR)/
