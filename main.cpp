#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <zconf.h>
#include "lib_bme280/bme280.h"
#include "lib_i2c/i2c_driver.h"

int i2c_init();
int i2c_read(uint8_t reg, int read_len, uint8_t read_res[]);
int i2c_write(uint8_t reg, uint8_t data);

I2C_Driver *i2c_driver = new I2C_Driver(I2C_ADAPTER_1);

int main() {
// char * device = "/dev/i2c-0";
    char *device = "/dev/i2c-1";
// char * device = "/dev/i2c-2";
// char * device = "/dev/i2c-3";
    int devId = BME280_I2C_ADDRESS1;
    try {
        BME280 * bme280 = new BME280(device, devId, i2c_init, i2c_read, i2c_write);
        int fd = bme280->init();
        bme280->reset();

        if (fd < 0) {
            printf("Device not found");
            return -1;
        }

        printf("fd       : 0x%02x\n", fd);
        printf("chip id  : 0x%02x\n", bme280->getChipId());
        printf("chip ver : 0x%02x\n", bme280->getChipVersion());

        bme280->reset();
        bme280->setPowerMode(BME280_NORMAL_MODE);
        bme280->setTemperatureOversampling(BME280_ULTRAHIGHRESOLUTION_OVERSAMP_TEMPERATURE);
        bme280->setPressureOversampling(BME280_ULTRAHIGHRESOLUTION_OVERSAMP_PRESSURE);
        bme280->setHumidityOversampling(BME280_ULTRAHIGHRESOLUTION_OVERSAMP_HUMIDITY);
        bme280->setIrrFilter(BME280_FILTER_COEFF_16);
        bme280->setStandbyTime(BME280_STANDBY_TIME_250_MS);

        printf("---------------\n");
        printf("pw mode  : 0x%02x\n", bme280->getPowerMode());
        printf("osrs_p   : 0x%02x\n", bme280->getPressureOversampling());
        printf("osrs_t   : 0x%02x\n", bme280->getTemperatureOversampling());
        printf("osrs_h   : 0x%02x\n", bme280->getHumidityOversampling());
        printf("---------------\n");
        printf("filter   : 0x%02x\n", bme280->getIrrFilter());
        printf("t_sb     : 0x%02x\n", bme280->getStandbyTime());
        printf("---------------\n");
        printf("spi3w sts: 0x%02x\n", bme280->getSpi3w());
        printf("measuring: 0x%02x\n", bme280->getMeasuringStatus());
        printf("im_update: 0x%02x\n", bme280->getImUpdateStatus());
        printf("---------------\n");

        while (1) {
            sleep(1);
            BMP280Data * bme280Data = bme280->getBMP280Data();
            printf("pressure   : %.2f \tmm Hg\n", bme280Data->getPressure() / 1.3332239);
            printf("humidity   : %.2f \t%c\n", bme280Data->getHumidity(), '%');
            printf("temperature: %.2f \t°C\n", bme280Data->getTemperature());
            printf("altitude   : %.2f \tm\n\n", bme280Data->getAltitude());
        }
        delete bme280;
    } catch (std::exception & e) {
        printf("%s\n", e.what());
    }
    return 0;
}


int i2c_init() {
    return i2c_driver->open_i2c_file();
}

int i2c_read(uint8_t reg, int read_len, uint8_t read_res[]){
    return i2c_driver->read_buffer_i2c(BME280_ADDRESS, reg, read_len, read_res);
}

int i2c_write(uint8_t reg, uint8_t data) {
    return i2c_driver->write_buffer_i2c(BME280_ADDRESS, reg, data);
}
