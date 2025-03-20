/* *********************************************************************** *
 * Basic Mac Address Changer for Linux/Debian based system using syscall   *
 * Still incomplete, needs more flags and all!                             *
 * 20/03/2025 - by @Suryakiran164                                          *
 * *********************************************************************** */

#define _GNU_SOURCE
#include <net/if.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if_arp.h>

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
int main(int,char**);
bool chmac(int8 *,Mac);

Mac generatemac()
{
        int64 a, b;
        Mac mac;

        a = (int64)random();
        b = (int64)random();
        mac.addr = ((a * b) % 281474976710656LL) & 0xFFFFFFFFFFFF;

        return mac;
}

bool chmac(int8 *ifname, Mac mac)
{
        struct ifreq ir;
        int fd, ret;

        fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        assert(fd > 0);

        strncpy(ir.ifr_ifrn.ifrn_name, (char *)ifname, (IFNAMSIZ-1));
        ir.ifr_ifru.ifru_hwaddr.sa_family = ARPHRD_ETHER;
        memcpy(ir.ifr_ifru.ifru_hwaddr.sa_data, &mac, 6);

        ret = ioctl(fd, SIOCSIFHWADDR, &ir);
        close(fd);

        return (!ret) ? true : false;
}

int main(int argc, char *argv[])
{
        Mac mac;
        int8 *ifname;
        if (argc < 2)
        {
                fprintf(stderr, "[+] Usage: %s <INTERFACE>\n", argv[0]);
                return 0;
        }
        else
                ifname = (int8 *)argv[1];

        srand(getpid());
        mac = generatemac();
        if(chmac(ifname, mac))
                printf("0x%lx\n", mac.addr);
        else
                assert_perror(errno);
        return 0;
}