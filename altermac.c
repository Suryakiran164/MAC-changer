/* altermac.c */

#define _GNU_SOURCE
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long int int64;

struct s_mac
{
        int64 addr:48;
};
typedef struct s_mac Mac;

Mac generatemac(void);
int main(int, char**);

Mac generatemac()
{
        int64 a, b;
        Mac mac;

        a = random();
        b = random();
        mac.addr = ((a * b) % 281474976710656);

        return mac;
}

int main(int argc, char *argv[])
{
        srand(getpid());

        return 0;
}
