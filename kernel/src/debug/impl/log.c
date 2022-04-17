#include <debug/log.h>
#include <util/string.h>
#include <arch/serial/serial.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define CYAN "\033[0;36m"
#define MAGENTA "\033[0;95m"
#define BOLD_BLUE "\033[1;94m"


static void write_serial_bulk(const char* str) {
    for (int i = 0; i < strlen(str); ++i) {
        write_serial(str[i]);
    }
}


void serial_log(const char* format, STATUS status, ...) {
    va_list ptr; 

    size_t args = 0;

    for (int i = 0; i < strlen(format); ++i) {
        if (format[i] == '%') {
            switch (format[i + 1]) {
                case 's':
                case 'd':
                    ++args;
                    i += 2;
                    break;
            }
        } 
    }

    // A hacky way to get rid of the warning: second parameter of 'va_start' not last named argument 
    STATUS s = status;
    status ^= status;
    status += args;
    va_start(ptr, status);

    switch (s) {
        case S_WARNING:
            write_serial_bulk(MAGENTA "[WARNING] ");
            break;
        case S_INFO:
            write_serial_bulk(CYAN "[INFO] ");
            break;
        case S_PANIC:
            write_serial_bulk(RED "** KERNEL PANIC **\n");
            break;
        case S_CRITICAL:
            write_serial_bulk(RED "[CRITICAL] ");
            break;
    }

    size_t logbufferidx = 0;

    for (int i = 0; i < strlen(format); ++i) {
        if (format[i] == '%') {
            switch (format[i + 1]) {
                case 's':
                    {
                        const char* arg = va_arg(ptr, const char*);

                        for (int j = 0; j < strlen(arg); ++j) {
                            write_serial(arg[j]);
                        }

                        ++i;

                    }

                    continue;
                case 'd':
                    {
                        int arg = va_arg(ptr, int);
                        write_serial_bulk(dec2str(arg));
                    }
                    ++i;
                    continue;
                case 'x':
                    {
                        int arg = va_arg(ptr, int);
                        write_serial_bulk(hex2str(arg));
                        ++i;
                        continue;
                    }
            }
        }

        write_serial(format[i]);
    }
}

