#include "ds18b20.h"

void dq_low(){
    GPIOA-> BSRR |=(1<<(dq*4)+16);
}

void dq_output(){
    GPIOA-> CRL |=(0X5<<dq*4);
}

void dq_input(){
    GPIOA-> CRL |=(0X4<<dq*4);
}

uint8_t dq_read(){
    GPIOA-> IDR & (1<<dq);
}

void ds18b20_init(){
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
    GPIOA-> CRL &=~(0XF<<dq*4);

}

uint8_t ds18b20_reset(){
    void dq_output();
    void dq_low();
    delay_us(480);
    GPIOA-> BSRR |= (1<<dq*4);
    void dq_input();
    delay_us(60);
    int precencia =! dq_read();
    delay_us(410);
    return precencia;

}

void ds18b20_write_bit(uint8_t bit){
    void dq_output();
    void dq_low();
    if(bit)delay_us(15);
    else delay_us(60);
    GPIOA-> BSRR |=(1<<dq*4);
}

uint8_t ds18b20_read_bit(){
    void dq_output();
    void dq_low();
    delay_us(1);
    GPIOA-> BSRR|=(1<<dq*4);
    void dq_input();
    int valor= dq_read();
    delay_us(15);
    return valor;

}

void ds18b20_write_byte(uint8_t data){
    int d;
    for(int i=0;i<8;i++){
        if((data>>i)&0x01) ds18b20_write_bit(1);
        else ds18b20_write_bit(0);
    }
}

uint8_t ds18b20_read_byte(){
   uint8_t byte;
    for(int i=0;i<8;i++){
        byte=(ds18b20_read_bit()<<i);
    }
    return byte;
}

void ds18b20_start_conversion(){
    if(ds18b20_reset()==0){
    return;
    }else{
       ds18b20_write_byte(0xCC);
    } 
}

float ds18b20_get_temperature(){
        if(ds18b20_reset()==0){
            return;
        }else{
           ds18b20_write_byte(0xCC);
           ds18b20_write_byte(0x44);
        }
    }