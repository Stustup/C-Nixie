#ifndef DHT22_H_
#define DHT22_H_

//Function prototypes
void DHT22_Start(void);
uint8_t DHT22_ReadByte(void);
void DHT22_ReadData(uint8_t* _temperature_tens, uint8_t* _temperature_ones, uint8_t* _humidity_tens, uint8_t* _humidity_ones);

#endif
