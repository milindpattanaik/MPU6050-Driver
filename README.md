# **MPU6050 HAL Driver – Cross-Platform Driver**  
*A lightweight and portable driver for the MPU6050, compatible with STM32, ESP32, Arduino, and other microcontrollers.*  

## **Introduction**  
The **MPU6050** is a **6-axis motion tracking sensor** that combines a **3-axis accelerometer** and a **3-axis gyroscope** into a single compact package. It is widely used in applications like drones, robotics, gaming, and motion tracking due to its ability to measure both **linear acceleration** and **angular velocity**. The sensor communicates via **I2C (default)**, providing raw motion data that can be processed to determine orientation, tilt, and movement.
 
 ![MPU6050 Pinout](./readme-images/mpuresearchpic.png)


This driver provides **a modular and cross-platform implementation**, making it **hardware-independent** and compatible with **any HAL (Hardware Abstraction Layer).**  


## **Features**  
- **Cross-Platform** – Works with **STM32, ESP32, Arduino, and more**  
- **Supports Any HAL** – Uses function pointers for communication (I2C/SPI)  
- **Accelerometer, Gyroscope & Temperature Readings**  
- **Self-Test & Digital Low-Pass Filter (DLPF) Configuration**  
- **Minimal Dependencies** – Designed for embedded systems  

## **I2C Communication in MPU6050**  
The **MPU6050** uses the **I2C protocol** to communicate with microcontrollers. It has:  

- **SDA (Serial Data Line)** – Transfers data  
- **SCL (Serial Clock Line)** – Synchronizes data transfer  

**I2C Address:**  
- **0x68** (Default) when **AD0 = LOW**  
- **0x69** when **AD0 = HIGH**  

## **MPU6050 Pinout**  
![MPU6050 Pinout](./readme-images/mpu6050pinout.png)

| Pin  | Name  | Description |
|------|------|-------------|
| **VCC** | **Power** | Connect to **3.3V or 5V** |
| **GND** | **Ground** | Connect to **GND** |
| **SCL** | **Serial Clock** | I2C clock pin |
| **SDA** | **Serial Data** | I2C data pin |
| **XDA** | **Auxiliary Serial Data** | Used for external sensors |
| **XCL** | **Auxiliary Serial Clock** | Used for external sensors |
| **ADO** | **I2C Address Select** | Changes device I2C address |
| **INT** | **Interrupt** | Signals data availability |

## ⚙️ **How to Use the Driver**

### 1️⃣ Add Required Files
Add the following files to your project:
```
mpu6050.h
mpu6050.c
mpu6050_regs.h
```


### 2️⃣ Implement the I2C Interface (STM32 HAL Example)

```c
I2C_HandleTypeDef hi2c1;

int8_t i2c_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint32_t len) {

    if (HAL_I2C_Master_Transmit(&hi2c1, addr, &reg, 1, HAL_MAX_DELAY) != HAL_OK) {
        return -1;
    }

    if (HAL_I2C_Master_Receive(&hi2c1, addr, buf, len, HAL_MAX_DELAY) != HAL_OK) {
        return -1;
    }

    return 0;
}

int8_t i2c_write(uint8_t addr, uint8_t *buf, uint32_t len) {

    if (HAL_I2C_Master_Transmit(&hi2c1, addr, buf, len, HAL_MAX_DELAY) != HAL_OK) {
        return -1;
    }

    return 0;
}

void delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}
```


### 3️⃣ Initialize the Driver

```c
#include "mpu6050.h"

mpu6050_t mpu = {
    .address = 0x68 << 1,
    .accel_range = MPU6050_ACCEL_2G,
    .gyro_range = MPU6050_GYRO_250DPS,
    .dlpf = MPU6050_DLPF_42HZ,
    .comm = {
        .read = i2c_read,
        .write = i2c_write,
        .delay = delay_ms,
    }
};

if (mpu6050_init(&mpu) != MPU6050_OK) {
    printf("MPU6050 init failed!\n");
}
```

## 📖 API Reference

| Function | Description |
|----------|-------------|
| `mpu6050_init()`       | Initialize MPU6050 with user config |
| `mpu6050_get_accel()`  | Read accelerometer (X, Y, Z)        |
| `mpu6050_get_gyro()`   | Read gyroscope (X, Y, Z)            |
| `mpu6050_get_temp()`   | Read onboard temperature            |
| `mpu6050_set_dlpf()`   | Configure Digital Low-Pass Filter   |
| `mpu6050_self_test()`  | Run basic self-test procedure       |


### 4️⃣ Read Sensor Data

```c
mpu6050_accel_data_t accel;
mpu6050_gyro_data_t gyro;
mpu6050_temp_data_t temp;

if (mpu6050_get_accel(&mpu, &accel) == MPU6050_OK) {
    printf("Accel: X=%.2f Y=%.2f Z=%.2f\n", accel.accel_x, accel.accel_y, accel.accel_z);
}

if (mpu6050_get_gyro(&mpu, &gyro) == MPU6050_OK) {
    printf("Gyro : X=%.2f Y=%.2f Z=%.2f\n", gyro.gyro_x, gyro.gyro_y, gyro.gyro_z);
}

if (mpu6050_get_temp(&mpu, &temp) == MPU6050_OK) {
    printf("Temp : %.2f °C\n", temp.temperature);
}
```