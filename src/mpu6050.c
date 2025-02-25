#include "mpu6050.h"
#include "mpu6050_regs.h"

mpu_status_t mpu6050_init(mpu6050_t *dev) {

    if (!dev || !dev->comm.read || !dev->comm.write || !dev->comm.delay) {
        return MPU_6050_ERR_PARAM;
    }

    uint8_t MPU6050_WHO_AM_I_VALUE = 0;
    if (dev->comm.read(dev->address, MPU6050_WHO_AM_I, &MPU6050_WHO_AM_I_VALUE, 1) != 0) {
        return MPU_6050_ERR_COMM;
    }

    if (MPU6050_WHO_AM_I_VALUE == MPU6050_WHO_AM_I_DEFAULT_VALUE) {
        
        uint8_t data;

        data = 0x00;
        if (dev->comm.write(dev->address, MPU6050_PWR_MGMT_1, &data, 1) != 0) {
            return MPU_6050_ERR_COMM;
        }
        
        // Set Sample Rate Divider
        // Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
        data = 0x07;
        if (dev->comm.write(dev->address, MPU6050_SMPLRT_DIV, &data, 1) != 0) {
            return MPU_6050_ERR_COMM;
        }

        // Set Gyro Range
        data = (dev->gyro_range << 3);
        if (dev->comm.write(dev->address, MPU6050_GYRO_CONFIG, &data, 1) != 0) {
            return MPU_6050_ERR_COMM;
        }

        // Set Accel Range
        data = (dev->accel_range << 3);
        if (dev->comm.write(dev->address, MPU6050_ACCEL_CONFIG, &data, 1) != 0) {
            return MPU_6050_ERR_COMM;
        }
    } else {
        return MPU_6050_ERR_WHO_AM_I;
    }

    return mpu6050_set_dlpf(dev, dev->dlpf);
}

// Set Digital Low Pass Filter (DLPF)
mpu_status_t mpu6050_set_dlpf(mpu6050_t *dev, mpu_dlpf_t dlpf) {

    if (!dev) {
        return MPU_6050_ERR_PARAM;
    }

    uint8_t data = dlpf;
    if (dev->comm.write(dev->address, MPU6050_PWR_MGMT_1, &data, 1) != 0) {
        return MPU_6050_ERR_COMM;
    }
    
    return MPU6050_OK;
}

// Self Test
mpu_status_t mpu6050_self_test(mpu6050_t *dev) {

    if (!dev) {
        return MPU_6050_ERR_PARAM;
    }

    uint8_t self_test[4];
    if (dev->comm.read(dev->address, MPU6050_SELF_TEST_X, self_test, 4) != 0) {
        return MPU_6050_ERR_COMM;
    }

    // Check self-test results 
    if (self_test[0] < 1 || self_test[1] < 1 || self_test[2] < 1 || self_test[3] < 1) {
        return MPU_6050_ERR_SELF_TEST;
    }

    return MPU6050_OK;
}

// Read Gyroscope Data
mpu_status_t mpu6050_get_gyro(mpu6050_t *dev, mpu6050_gyro_data_t *data) {

    float gyro_sensitivity;

    // Sensitivity based on configured gyro ranges

    switch (dev->gyro_range) {
        case MPU6050_GYRO_250DPS:
            gyro_sensitivity = 131.0f;
            break;
        case MPU6050_GYRO_500DPS:
            gyro_sensitivity = 65.5f;
            break;
        case MPU6050_GYRO_1000DPS:
            gyro_sensitivity = 32.8f;
            break;
        case MPU6050_GYRO_2000DPS:
            gyro_sensitivity = 16.4f;
            break;
        default:
            return MPU_6050_ERR_PARAM;
    }

    if (!dev || !data) {
        return MPU_6050_ERR_PARAM;
    }

    // 16 bit is being stored as [15:8] and [7:0] in GYRO_XOUT_H and GYRO_XOUT_L respectively
    // Hence we need to read 6 bytes to store lower and higher values for X, Y and Z
    uint8_t buffer[6];
    if (dev->comm.read(dev->address, MPU6050_GYRO_XOUT_H, buffer, 6) != 0) {
        return MPU_6050_ERR_COMM;
    }

    // We need to do a bitwise shift operator along with a bitwise OR to get the 16 bit Raw Data
    // For Example GYRO_XOUT_H is 11000110 and GYRO_XOUT_L is 00101110
    // then GYRO_XOUT = ((11000110 << 8) | 00101110) = 1100011000101110
    int16_t raw_gyro_x = (int16_t)((buffer[0] << 8) | buffer[1]);
    int16_t raw_gyro_y = (int16_t)((buffer[2] << 8) | buffer[3]);
    int16_t raw_gyro_z = (int16_t)((buffer[4] << 8) | buffer[5]);

    data->gyro_x = raw_gyro_x / gyro_sensitivity;
    data->gyro_y = raw_gyro_y / gyro_sensitivity;
    data->gyro_z = raw_gyro_z / gyro_sensitivity;

    return MPU6050_OK;
}

// Read Accelerometer Data
mpu_status_t mpu6050_get_accel(mpu6050_t *dev, mpu6050_accel_data_t *data) {

    float accel_sensitivity;

    // Sensitivity based on configured accel ranges

    switch (dev->accel_range) {
        case MPU6050_ACCEL_2G:
            accel_sensitivity = 16384.0f;
            break;
        case MPU6050_ACCEL_4G:
            accel_sensitivity = 8192.0f;
            break;
        case MPU6050_ACCEL_8G:
            accel_sensitivity = 4096.0f;
            break;
        case MPU6050_ACCEL_16G:
            accel_sensitivity = 2048.0f;
            break;
        default:
            return MPU_6050_ERR_PARAM;
    }

    if (!dev || !data) {
        return MPU_6050_ERR_PARAM;
    }

    // 16 bit is being stored as [15:8] and [7:0] in ACCEL_XOUT_H and ACCEL_XOUT_L respectively
    // Hence we need to read 6 bytes to store lower and higher values for X, Y and Z
    uint8_t buffer[6];
    if (dev->comm.read(dev->address, MPU6050_ACCEL_XOUT_H, buffer, 6) != 0) {
        return MPU_6050_ERR_COMM;
    }

    // We need to do a bitwise shift operation along with a bitwise OR to get the 16 bit RAW data
    // For Example ACCEL_XOUT_H is 11000110 and ACCEL_XOUT_L is 00101110 
    // then ACCEL_XOUT = ((11000110 << 8) | 00101110) = 1100011000101110
    int16_t raw_accel_x = (int16_t)((buffer[0] << 8) | buffer[1]);
    int16_t raw_accel_y = (int16_t)((buffer[2] << 8) | buffer[3]);
    int16_t raw_accel_z = (int16_t)((buffer[4] << 8) | buffer[5]);

    data->accel_x = raw_accel_x / accel_sensitivity;
    data->accel_y = raw_accel_y / accel_sensitivity;
    data->accel_z = raw_accel_z / accel_sensitivity;

    return MPU6050_OK;
}

// Read Temperature Data
mpu_status_t mpu6050_get_temp(mpu6050_t *dev, mpu6050_temp_data_t *temp) {

    if (!dev || !temp) {
        return MPU_6050_ERR_PARAM;
    }

    uint8_t buffer[2];
    if (dev->comm.read(dev->address, MPU6050_TEMP_OUT_H, buffer, 2) != 0) {
        return MPU_6050_ERR_COMM;
    }

    // Temperature in degrees Celcius
    int16_t raw_temp = (int16_t)((buffer[0] << 8) | buffer[1]);
    temp->temperature = raw_temp / 340.0f + 36.53f;

    return MPU6050_OK;
}
