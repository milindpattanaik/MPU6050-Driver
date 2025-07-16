#ifndef MPU6050_H
#define MPU6050_H

#include "stdint.h"
#include "stddef.h"

// =============================================================================
// MPU6050 Class
// =============================================================================

// Error Codes
typedef enum {
    MPU6050_OK = 0,
    MPU_6050_ERR_INIT,
    MPU_6050_ERR_COMM,
    MPU_6050_ERR_PARAM,
    MPU_6050_ERR_SELF_TEST,
    MPU_6050_ERR_WHO_AM_I,
} mpu_status_t;

// Gyro Scale Range
typedef enum {
    MPU6050_GYRO_250DPS = 0,
    MPU6050_GYRO_500DPS,
    MPU6050_GYRO_1000DPS,
    MPU6050_GYRO_2000DPS,
} mpu_gyro_range_t;

// Accelerometer Scale Range
typedef enum {
    MPU6050_ACCEL_2G = 0,
    MPU6050_ACCEL_4G,
    MPU6050_ACCEL_8G,
    MPU6050_ACCEL_16G,
} mpu_accel_range_t;

// Digital Low Pass Filter (DLPF) for Gyro
typedef enum {
    MPU6050_DLPF_256HZ = 0,
    MPU6050_DLPF_188HZ,
    MPU6050_DLPF_98HZ,
    MPU6050_DLPF_42HZ,
    MPU6050_DLPF_20HZ,
    MPU6050_DLPF_10HZ,
    MPU6050_DLPF_5HZ,
} mpu_dlpf_t;

// Power Management 1 Bit Fields Configuration
typedef enum {
    MPU6050_PWR_MGMT_1_RESET = 0x80,
    MPU6050_PWR_MGMT_1_SLEEP = 0x40,
    MPU6050_PWR_MGMT_1_CYCLE = 0x20,
    MPU6050_PWR_MGMT_1_TEMP_DIS = 0x01,
} mpu6050_pwr_mgmt_1_t;


// MPU6050 Data Structures
typedef struct {
    float gyro_x;
    float gyro_y;
    float gyro_z;
} mpu6050_gyro_data_t;

typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
} mpu6050_accel_data_t;

typedef struct {
    float temperature;
} mpu6050_temp_data_t;

// Communication Interface
typedef struct {
    int8_t (*read)(uint8_t addr, uint8_t start_reg, uint8_t *buf, uint32_t len);
    int8_t (*write)(uint8_t addr, uint8_t *buf, uint32_t len);
    void (*delay)(uint32_t ms);
} mpu6050_comm_t;

// MPU6050 Configuration Structure
typedef struct {
    uint8_t address;
    mpu_accel_range_t accel_range;
    mpu_gyro_range_t gyro_range;
    mpu_dlpf_t dlpf;
    mpu6050_comm_t comm;
    mpu6050_pwr_mgmt_1_t pwr_mgmt_1;
} mpu6050_t;

// =============================================================================
// Public API Functions
// =============================================================================

mpu_status_t mpu6050_init(mpu6050_t *dev);
mpu_status_t mpu6050_self_test(mpu6050_t *dev);
mpu_status_t mpu6050_get_gyro(mpu6050_t *dev, mpu6050_gyro_data_t *data);
mpu_status_t mpu6050_get_accel(mpu6050_t *dev, mpu6050_accel_data_t *data);
mpu_status_t mpu6050_set_dlpf(mpu6050_t *dev, mpu_dlpf_t dlpf);
mpu_status_t mpu6050_get_temp(mpu6050_t *dev, mpu6050_temp_data_t *temp);


#endif