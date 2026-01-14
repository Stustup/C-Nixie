#include "stm32g0xx_hal.h" // Anpassen an deinen STM32-Typ
#include <stdint.h>
#include "dht22.h"

#define DHT22_PORT GPIOA
#define DHT22_PIN  GPIO_PIN_1

void DHT22_Start(void) {
    // 1. Host sendet Start-Signal (DATA-Pin auf LOW für 1–10 ms)
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, GPIO_PIN_SET);

    // 2. Warten auf Antwort des Sensors (DATA-Pin wird LOW gezogen)
    uint32_t timeout = 1000;
    while (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN) == GPIO_PIN_SET && timeout--) {
        HAL_Delay(1);
    }

    // 3. Warten, bis der Sensor DATA-Pin wieder auf HIGH zieht
    timeout = 1000;
    while (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN) == GPIO_PIN_RESET && timeout--) {
        HAL_Delay(1);
    }
}

uint8_t DHT22_ReadByte(void) {
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        // Warten, bis DATA-Pin auf LOW geht (Start eines Bits)
        while (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN) == GPIO_PIN_SET);

        // Nach ~50 µs: DATA-Pin geht auf HIGH für die Dauer des Bits
        HAL_Delay(1); // Kurze Wartezeit
        byte <<= 1;
        if (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN) == GPIO_PIN_SET) {
            byte |= 1;
        }

        // Warten, bis DATA-Pin wieder auf LOW geht (Ende des Bits)
        while (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN) == GPIO_PIN_SET);
    }
    return byte;
}

void DHT22_ReadData(uint8_t* _temperature_tens, uint8_t* _temperature_ones, uint8_t* _humidity_tens, uint8_t* _humidity_ones) {
    uint8_t data[5] = {0};
    DHT22_Start();

    uint8_t _temperature = 0;
    uint8_t _humidity = 0;
    // 40 Bits (5 Bytes) auslesen
    for (uint8_t i = 0; i < 5; i++) {
        data[i] = DHT22_ReadByte();
    }

    // Prüfen, ob die Prüfsumme stimmt
    if (data[0] + data[1] + data[2] + data[3] == data[4]) {
        _humidity = (data[0] * 256 + data[1]) / 10; // Ganzzahl (z. B. 45%)
        _temperature = ((data[2] & 0x7F) * 256 + data[3]) / 10; // Ganzzahl (z. B. 23°C)
        if (data[2] & 0x80) {
            _temperature = -_temperature; // Negativ, falls Bit 7 gesetzt
        }
    }
    *_temperature_tens = _temperature / 10;
    *_temperature_ones = _temperature % 10;
    *_humidity_tens = _humidity / 10;
    *_humidity_ones = _humidity % 10;
}
