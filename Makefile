COMPILER = gcc
TARGET = dns
CFLAGS  = -g -std=c11 -Wall -Wextra -Werror -pedantic -fPIC -Iinclude -pthread
LDFLAGS = -pthread

SRCS = src/arg_parse.c src/dns.c src/dns_packet_create.c src/dns_packet_parse.c \
       src/dynamic_array.c src/linked_list.c src/log.c src/thread.c src/utils.c

OBJS = src/arg_parse.o src/dns.o src/dns_packet_create.o src/dns_packet_parse.o \
       src/dynamic_array.o src/linked_list.o src/log.o src/thread.o src/utils.o

.PHONY: all clean run test zip

all: $(TARGET)

$(TARGET): $(OBJS)
	$(COMPILER) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)

src/arg_parse.o:
	$(COMPILER) $(CFLAGS) -c src/arg_parse.c -o src/arg_parse.o

src/dns.o:
	$(COMPILER) $(CFLAGS) -c src/dns.c -o src/dns.o

src/dns_packet_create.o:
	$(COMPILER) $(CFLAGS) -c src/dns_packet_create.c -o src/dns_packet_create.o

src/dns_packet_parse.o:
	$(COMPILER) $(CFLAGS) -c src/dns_packet_parse.c -o src/dns_packet_parse.o

src/dynamic_array.o:
	$(COMPILER) $(CFLAGS) -c src/dynamic_array.c -o src/dynamic_array.o

src/linked_list.o:
	$(COMPILER) $(CFLAGS) -c src/linked_list.c -o src/linked_list.o

src/log.o:
	$(COMPILER) $(CFLAGS) -c src/log.c -o src/log.o

src/thread.o:
	$(COMPILER) $(CFLAGS) -c src/thread.c -o src/thread.o

src/utils.o:
	$(COMPILER) $(CFLAGS) -c src/utils.c -o src/utils.o

run: $(TARGET)
	./$(TARGET) -h

test:
	chmod +x tests/test_complex.sh
	bash tests/test_complex.sh

clean:
	rm -f $(OBJS) $(TARGET)

zip:
	zip -r xsvancd00.zip .