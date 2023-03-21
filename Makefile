HOST=mipsel-openwrt-linux
PLATFORM_DIR=R2
TOOLCHAINS_PATH=$(HOME)/Documents/Toolchains
TOOLCHAIN=$(TOOLCHAINS_PATH)/$(PLATFORM_DIR)/toolchain
LIBS_PATH=$(TOOLCHAIN)/lib:$(TOOLCHAIN)/lib64
BINS_PATH=$(TOOLCHAIN)/bin
COMPILER=$(HOST)-gcc
#CC=STAGING_DIR="" LD_LIBRARY_PATH=$(LIBS_PATH) PATH=$(BINS_PATH):$(PATH) $(TOOLCHAIN)/bin/$(COMPILER)

#####################################################################

TARGET=bin/calc_rate_srv bin/calc_rate_cli
CFLAGS=-I./include -Wall
LDFLAGS=
PREPARE=obj bin

all: $(PREPARE) $(TARGET)

clean:
	rm -rf bin obj

$(PREPARE):
	mkdir $@

bin/calc_rate_srv: obj/tcp_server.o obj/linked_list.o obj/traffic_gen.o obj/calc_rate_srv.o 
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

bin/calc_rate_cli:  obj/tcp_client.o obj/linked_list.o obj/traffic_gen.o obj/calc_rate_cli.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

obj/calc_rate_srv.o: src/calc_rate_srv.c include/tcp_server.h include/traffic_gen.h 
	$(CC) -c $(CFLAGS) $< -o $@

obj/tcp_server.o: src/tcp_server.c include/tcp_server.h
	$(CC) -c $(CFLAGS) $< -o $@

obj/calc_rate_cli.o: src/calc_rate_cli.c include/tcp_client.h include/traffic_gen.h 
	$(CC) -c $(CFLAGS) $< -o $@

obj/tcp_client.o: src/tcp_client.c include/tcp_client.h
	$(CC) -c $(CFLAGS) $< -o $@

obj/linked_list.o: src/linked_list.c include/linked_list.h
	$(CC) -c $(CFLAGS) $< -o $@

obj/traffic_gen.o: src/traffic_gen.c include/traffic_gen.h 
	$(CC) -c $(CFLAGS) $< -o $@
