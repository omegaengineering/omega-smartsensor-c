#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

int main(int argc, char** argv)
{
    int fd, ret;
    char buffer[128];
    if (argc < 3) {
        printf("junk [i2c_bus] [i2c_addr]\n");
        printf("eg: junk 0 0x6c\n");
        exit(0);
    }

    unsigned int bus_id = atoi(argv[1]);
    int i2c_addr = strtol(argv[2], NULL, 16);
    printf("bus %d addr 0x%x\n", bus_id, i2c_addr);

    char bus[128];
    sprintf(bus, "/dev/i2c-%d", bus_id);

    fd = open(bus, O_RDWR);
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }
    ret = ioctl(fd, I2C_SLAVE, i2c_addr);
    if (ret < 0) {
        perror("ioctl failed");
        exit(1);
    }
    int reg_addr = 0x38;
    uint8_t reg = reg_addr & 0xFF;
    unsigned int long n = 1;

    while (n++) {
#if 0
        if (write(fd, &reg, 1) != 1) {
            perror("write failed");
        }
        if (read(fd, buffer, 2) != 2) {
            perror("read failed");
            exit(1);
        }
#endif
#if 1
        if (i2c_smbus_read_i2c_block_data(fd, reg, 4, buffer) != 4) {
            perror("read failed");
            exit(1);
        }

#endif
        printf("%ld %d %d %d %d\n",
               n,
               buffer[0],
               buffer[1],
               buffer[2],
               buffer[3]
        );
    }


    printf("%d %d\n", buffer[0], buffer[1]);
    close(fd);

}



