#ifndef MPU6050_REGS_H
#define MPU6050_REGS_H

// =============================================================================
// MPU6050 Registers Addresses And Values
// =============================================================================

// Power Management 1 Register
#define MPU6050_PWR_MGMT_1 0x6B

// Power Management 2 Register
#define MPU6050_PWR_MGMT_2 0x6C

// MPU6050 Config Register
#define MPU6050_CONFIG 0x1A

// Gyroscope Configuration Register
#define MPU6050_GYRO_CONFIG 0x1B

// Accelerometer Configuration Register
#define MPU6050_ACCEL_CONFIG 0x1C

// Temperature Data Register
#define MPU6050_TEMP_OUT_H 0x41
#define MPU6050_TEMP_OUT_L 0x42

// Sample Rate Divider Register
#define MPU6050_SMPLRT_DIV 0x19

// Gyroscope Data Registers
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_XOUT_L 0x44
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46
#define MPU6050_GYRO_ZOUT_H 0x47
#define MPU6050_GYRO_ZOUT_L 0x48

// Accelerometer Data Registers
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40

// Self Test Register
#define MPU6050_SELF_TEST_X 0x0D
#define MPU6050_SELF_TEST_Y 0x0E
#define MPU6050_SELF_TEST_Z 0x0F
#define MPU6050_SELF_TEST_A 0x10

// WHO AM I Register
#define MPU6050_WHO_AM_I 0x75
#define MPU6050_WHO_AM_I_DEFAULT_VALUE 0x68

#endif