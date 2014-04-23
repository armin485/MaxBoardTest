// In The Name Of God
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <linux/spi/spidev.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h> //for usleep
#include "GPIO.h"

using namespace exploringBB;
using namespace std;


#define SPI_PATH "/dev/spidev1.0"

unsigned long int setup1[10]=
{
	(1 << 0)|(1 << 1)|(0 << 2)|(0b00 << 3)|(0b001101 << 5)|(1 << 11)|(1 << 12)|(0b00 << 13)|(0 << 15)|(0b1000101001 << 16)|(0 << 26)|(1 << 27),
	(0b00 << 0)|(0b10 << 2)|(0b00 << 4)|(0b010 << 6)|(0b01 << 9)|(0b00 << 11)|(0b00 << 13)|(170 << 15)|(1 << 27),
	(0 << 0)|(0 << 0)|(1 << 2)|(1 << 3)|(0b01 << 4)|(0b111 << 6)|(0 << 9)|(0 << 10)|(0 << 11)|(0b110 << 12)|(1 << 15)|(0b1111 << 16)|(0 << 20)|(1 << 21)|(0b111010 << 22),
	(0b00 << 0)|(0 << 2)|(0 << 3)|(0b0000 << 4)|(0 << 8)|(0 << 9)|(0b0000 << 10)|(0b10000 << 14)|(0b01 << 19)|(0b11 << 21)|(1 << 23)|(1 << 24)|(0b100 << 25),
	(0b000 << 0)|(16 << 3)|(1538 << 13),
	(0b01110000 << 0)|(520192 << 8),
	(0x8000000 << 0),
	(0 << 0)|(1 << 1)|(0 << 2)|(1 << 3)|(3501 << 4)|(671 << 16),
	(0x1E0F401 << 0),
	(0 << 0)|(0x28C0402)
};

unsigned long int setup2[10]=
{
	(1 << 0)|(0 << 1)|(0 << 2)|(0b10 << 3)|(0b001101 << 5)|(1 << 11)|(1 << 12)|(0b00 << 13)|(0 << 15)|(0b1000101001 << 16)|(0 << 26)|(1 << 27),
	(0b00 << 0)|(0b10 << 2)|(0b00 << 4)|(0b010 << 6)|(0b01 << 9)|(0b00 << 11)|(0b00 << 13)|(170 << 15)|(1 << 27),
	(1 << 0)|(1 << 0)|(1 << 2)|(1 << 3)|(0b01 << 4)|(111 << 6)|(0 << 9)|(1 << 10)|(0 << 11)|(0b110 << 12)|(1 << 15)|(1111 << 16)|(0 << 20)|(1 << 21)|(0b111010 << 22),
	(0b00 << 0)|(0 << 2)|(0 << 3)|(0b0000 << 4)|(0 << 8)|(0 << 9)|(0b0000 << 10)|(0b10000 << 14)|(0b10 << 19)|(0b11 << 21)|(1 << 23)|(1 << 24)|(0b100 << 25),
	(0b000 << 0)|(1 << 3)|(96 << 13),
	(0b01110000 << 0)|(1048576 << 8),
	(0x80000000 << 0),
	(0 << 0)|(1 << 1)|(0 << 2)|(1 << 3)|(4155 << 4)|(78 << 16),
	(0x1E0F401 << 0),
	(0 << 0)|(0x28C0402)
};

unsigned long int setup3[10]=
{
	(1 << 0)|(1 << 1)|(0 << 2)|(0b00 << 3)|(0b001101 << 5)|(1 << 11)|(1 << 12)|(0b00 << 13)|(0 << 15)|(0b1000101001 << 16)|(0 << 26)|(1 << 27),
	(0b00 << 0)|(0b10 << 2)|(0b00 << 4)|(0b010 << 6)|(0b01 << 9)|(0b00 << 11)|(0b00 << 13)|(170 << 15)|(1 << 27),
	(0 << 0)|(0 << 0)|(1 << 2)|(1 << 3)|(0b01 << 4)|(0b111 << 6)|(0 << 9)|(0 << 10)|(0 << 11)|(0b110 << 12)|(1 << 15)|(0b1111 << 16)|(0 << 20)|(1 << 21)|(0b111010 << 22),
	(0b00 << 0)|(0 << 2)|(0 << 3)|(0b0000 << 4)|(0 << 8)|(0 << 9)|(0b0000 << 10)|(0b10000 << 14)|(0b01 << 19)|(0b11 << 21)|(1 << 23)|(1 << 24)|(0b100 << 25),
	(0b000 << 0)|(2 << 3)|(192 << 13),
	(0b01110000 << 0)|(327168 << 8),
	(0x80000000 << 0),
	(0 << 0)|(1 << 1)|(0 << 2)|(1 << 3)|(4155 << 4)|(78 << 16),
	(0x1E0F401 << 0),
	(0 << 0)|(0x28C0402)
};

#define MAX2769_CONF1   0x00
#define MAX2769_CONF2   0x01
#define MAX2769_CONF3   0x02
#define MAX2769_PLLCONF 0x03
#define MAX2769_DIV     0x04
#define MAX2769_FDIV    0x05
#define MAX2769_STRM    0x06
#define MAX2769_CLK     0x07
#define MAX2769_TEST1   0x08
#define MAX2769_TEST2   0x09


int transfer(int fd, unsigned char send[], unsigned char receive[], int length){
   struct spi_ioc_transfer transfer;           //the transfer structure
   transfer.tx_buf = (unsigned long) send;     //the buffer for sending data
   transfer.rx_buf = (unsigned long) receive;  //the buffer for receiving data
   transfer.len = length;                      //the length of buffer
   transfer.speed_hz = 100000;                //the speed in Hz
   transfer.bits_per_word = 8;                 //bits per word
   transfer.delay_usecs = 5;                   //delay in us

   // send the SPI message (all of the above fields, inc. buffers)
   int status = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
   if (status < 0) {
      perror("SPI: SPI_IOC_MESSAGE Failed");
      return -1;
   }
   return status;
}

int maxSpiSend(int fd, GPIO *CsClass, unsigned char addr, unsigned long int data)
{
	unsigned char null = 0x00;
	
	data <<= 4;
	CsClass.setValue(LOW);
	if (transfer(fd, (unsigned char)(data&0xFF)|addr, &null, 1)==-1)
	{
		perror("Failed to update the display");
		return -1;
	}
	if (transfer(fd, (unsigned char)(data>>8), &null, 1)==-1)
	{
		perror("Failed to update the display");
		return -1;
	}
	if (transfer(fd, (unsigned char)(data>>16), &null, 1)==-1)
	{
		perror("Failed to update the display");
		return -1;
	}
	if (transfer(fd, (unsigned char)(data>>24), &null, 1)==-1)
	{
		perror("Failed to update the display");
		return -1;
	}
	CsClass.setValue(HIGH);
	
	return 1;
}

int maxSpiInit(GPIO *CsClass, unsigned long int SpiClock, unsigned int SpiDelayUs)
{
	int fd;
	uint8_t bits = 8, mode = 0;             //8-bits per word, SPI mode 0
	
    CsClass.setDirection(OUTPUT);
    CsClass.setValue(HIGH);

      // The following calls set up the SPI bus properties
   if ((fd = open(SPI_PATH, O_RDWR))<0){
      perror("SPI Error: Can't open device.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_WR_MODE, &mode)==-1){
      perror("SPI: Can't set SPI mode.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_RD_MODE, &mode)==-1){
      perror("SPI: Can't get SPI mode.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits)==-1){
      perror("SPI: Can't set bits per word.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits)==-1){
      perror("SPI: Can't get bits per word.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &SpiClock)==-1){
      perror("SPI: Can't set max speed HZ");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &SpiClock)==-1){
      perror("SPI: Can't get max speed HZ.");
      return -1;
   }

   // Check that the properties have been set
   printf("SPI Mode is: %d\n", mode);
   printf("SPI Bits is: %d\n", bits);
   printf("SPI Speed is: %d\n", SpiClock);	
   
   return fd;
}


int main(){
   int fd, i=0,j=0;                   //file handle and loop counter
   
   GPIO CsGPIO(48);
   
   fd = maxSpiInit(&CsGPIO, 100000, 5);
   

   unsigned char addrbuffer[]=
   {
           MAX2769_CONF1,
           MAX2769_CONF2,
           MAX2769_CONF3,
           MAX2769_PLLCONF,
           MAX2769_DIV,
           MAX2769_FDIV,
           MAX2769_STRM,
           MAX2769_CLK,
           MAX2769_TEST1,
           MAX2769_TEST2
   };

	unsigned long int *txbuffer = setup2;
	
      // Loop from 0 to 255 and light the LED bar as a binary counter
   for (j=0;j<100;j++)
   {
		for(i=0;i<10;i++)
		{
			if (maxSpiSend(fd, CsGPIO, addrbuffer[i],data[i], 1)==-1)
			{
				perror("Failed to update the display");
				return -1;
			}
			usleep(1000);       //sleep for 1ms each loop

		}
      usleep(10000);       //sleep for 10ms each loop
   };
   close(fd);               //close the file
   CsGPIO.streamClose();

   return 0;
}




