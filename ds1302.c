/***************************************************************************/
/* DS1302.C                                                                */
/***************************************************************************/
//sbit IO   = P0^2;
//sbit SCLK = P0^1;
//sbit RSTB = P0^0;
/***************************************************************************/
/* Prototypes                                                              */
/***************************************************************************/

#include <p30fxxxx.h>
#include  "30f6010_io.h"            
#include  "host_io.h"            
#include  "iodef.h"
#include  "door.h"
//#include  "eeprom.h"
#include  "com.h"
#include  "counter.h"


//void	        clkwritebyte(void);
//void	        ramwritebyte(void);
void	        reset_3w(void);
void	        wbyte_3w(UserDataType W_Byte);
void	        Initialize_DS1302(void);
void	        disp_clk_regs(void);

//void	        burstramrd(void);
//void	        burstramwr(void);

UserDataType    rbyte_3w(void);
UserDataType	R_Byte = 0x00;


void reset_3w(void)
{
    SCLK = 0;
    SCLK = 0;
    SCLK = 0;
    SCLK = 0;
    RSTB = 0;
    RSTB = 0;
    RSTB = 0;
    RSTB = 0;

    RSTB = 1;
    RSTB = 1;
    RSTB = 1;
    RSTB = 1;
    RSTB = 1;
    RSTB = 1;
    RSTB = 1;
    RSTB = 1;
}

void   __attribute__((section(".usercode"))) wbyte_3w(UserDataType W_Byte)
{
    unsigned int i;
    
	for(i = 0; i < 8; ++i)
	{        
		SDA = 0;
		if(W_Byte & 0x01)
		{
			SDA = 1;	/* set port pin high to read data */
		}
		SCLK = 0;
		SCLK = 0;
		SCLK = 0;
		SCLK = 0;

		SCLK = 0;
		SCLK = 0;
		SCLK = 0;
		SCLK = 0;

		SCLK = 1;
		SCLK = 1;
		SCLK = 1;
		SCLK = 1;
		SCLK = 1;
		SCLK = 1;
		SCLK = 1;
		SCLK = 1;
		W_Byte >>= 1;
      }
}

UserDataType   __attribute__((section(".usercode"))) rbyte_3w(void)
{
    unsigned int i;

    TRISG = (TRISG | 0x0008);

	R_Byte = 0x00;

	for(i=0; i<8; ++i)
	{
		SCLK = 1;
		SCLK = 1;
		SCLK = 1;
		SCLK = 1;
		SCLK = 1;
		SCLK = 1;
		SCLK = 1;
		SCLK = 1;

		SCLK = 0;
		SCLK = 0;
		SCLK = 0;
		SCLK = 0;
		SCLK = 0;
		SCLK = 0;
		SCLK = 0;
		SCLK = 0;

		R_Byte = R_Byte >> 1; 
        
        if(IN_DATA){
          R_Byte= R_Byte | 0x80;
        }
	}

    TRISG = (TRISG & 0xfff7);

	return R_Byte;
}


void   __attribute__((section(".usercode"))) disp_clk_regs(void)
{
	reset_3w();

	wbyte_3w(0xBF);	/* clock burst */
    sRamDArry[msec] = rbyte_3w();
    sRamDArry[mMinuate] = rbyte_3w();
    sRamDArry[mHour] = rbyte_3w();
    sRamDArry[mDay] = rbyte_3w();
    sRamDArry[mMonth] = rbyte_3w();
    rbyte_3w();    ///day
    sRamDArry[mYear]  = rbyte_3w();

	reset_3w();
}





void   __attribute__((section(".usercode"))) Initialize_DS1302(void)	/* ----------------------------------------- */
{
	TRISF=(TRISF & 0xff7f);		//PORTF7			//RST
	TRISG=(TRISG & 0xfff3);     //PORTG2,PORTG3		//CLK, DATA


	reset_3w();
	wbyte_3w(0x8e);	/* control register */
	wbyte_3w(0);	/* disable write protect */
	reset_3w();
	wbyte_3w(0x90);	/* trickle charger register */
	wbyte_3w(0xab);	/* enable, 2 diodes, 8K resistor */
	reset_3w();
	wbyte_3w(0xbe);	/* clock burst write (eight registers) */

	wbyte_3w(sRamDArry[msec]);
	wbyte_3w(sRamDArry[mMinuate]);
	wbyte_3w(sRamDArry[mHour]);
	wbyte_3w(sRamDArry[mDay]);
	wbyte_3w(sRamDArry[mMonth]);
	wbyte_3w(0x03);
	wbyte_3w(sRamDArry[mYear]);
	wbyte_3w(0);		/* must write control register in burst mode */
	reset_3w();
}


