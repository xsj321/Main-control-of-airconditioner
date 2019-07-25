#include "dht11.h"

void dht11_gpio_input(void)
{
    GPIO_InitTypeDef g;
    
    RCC_APB2PeriphClockCmd(DHT11_RCC, ENABLE);
    
    g.GPIO_Pin = DHT11_GPIO_PIN;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    
    GPIO_Init(DHT11_GPIO_TYPE, &g);
}

void dht11_gpio_output(void)
{
    GPIO_InitTypeDef g;
    
    RCC_APB2PeriphClockCmd(DHT11_RCC, ENABLE);
    
    g.GPIO_Pin = DHT11_GPIO_PIN;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出

    GPIO_Init(DHT11_GPIO_TYPE, &g);
}

void dht11_reset(void)
{
    // 按照DHT11手册步骤
    dht11_gpio_output();
    DHT11_OUT_L;
    delay_us(19000);
    DHT11_OUT_H;
    delay_us(30);
    dht11_gpio_input();
}

u16 dht11_scan(void)
{
    return DHT11_IN;
}

u16 dht11_read_bit(void)
{
    while (DHT11_IN == RESET);
    delay_us(40);
    if (DHT11_IN == SET)
    {
        while (DHT11_IN == SET);
        return 1;
    }
    else
    {
        return 0;
    }
}

u16 dht11_read_byte(void)
{
    u16 i;
    u16 data = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= dht11_read_bit();
    }
    return data;
}

u16 dht11_read_data(u8 buffer[5])
{
	
		u8 checksum  =0;
    u16 i = 0;
    
    dht11_reset();
    if (dht11_scan() == RESET)
    {
        //检测到DHT11响应
        while (dht11_scan() == RESET);
        while (dht11_scan() == SET);
        for (i = 0; i < 5; i++)
        {
            buffer[i] = dht11_read_byte();
        }
        
        while (dht11_scan() == RESET);
        dht11_gpio_output();
        DHT11_OUT_H;
        
        checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
        if (checksum != buffer[4])
        {
            // checksum error
            return 1;
        }
    }
    
    return 0;
}

int dht11_hum_value(int last)
{
				u8 buffer[5];
        int hum;
        int temp;
        if (dht11_read_data(buffer) == 0)
        {
            hum = (int)(buffer[0]);
            temp = (int)(buffer[2]);
						return hum;
        }
				return last;
				
}

int dht11_temp_value(int last)
{
				u8 buffer[5];
        int hum;
        int temp;
        if (dht11_read_data(buffer) == 0)
        {
            hum = (int)(buffer[0]);
            temp = (int)(buffer[2]);
						return temp;
        }
				return last;
				
}