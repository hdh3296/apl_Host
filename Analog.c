


#include <p30fxxxx.h>

#include	"Analog.h"
#include  	"iodef.h"
#include  "host_io.h"            


/*
#include  "apl_memory_map.h"
#include  "30f6010_io.h"            
#include  "host_io.h"            
#include  "iodef.h"
#include  "door.h"
#include  "com.h"
#include  "counter.h"
#include  "you_can1.h" 
#include  "you_can2.h" 
#include  "lader.h" 
#include  "default_setup.h" 
*/



/*
#define _DONE ADCON1bits.DONE
#define _SAMP ADCON1bits.SAMP
#define _ASAM ADCON1bits.ASAM
#define _SIMSAM ADCON1bits.SIMSAM
#define _SSRC ADCON1bits.SSRC
#define _FORM ADCON1bits.FORM
#define _ADSIDL ADCON1bits.ADSIDL
#define _ADON ADCON1bits.ADON
#define _SSRC0 ADCON1bits.SSRC0
#define _SSRC1 ADCON1bits.SSRC1
#define _SSRC2 ADCON1bits.SSRC2
#define _FORM0 ADCON1bits.FORM0
#define _FORM1 ADCON1bits.FORM1

#define _ALTS ADCON2bits.ALTS
#define _BUFM ADCON2bits.BUFM
#define _SMPI ADCON2bits.SMPI
#define _BUFS ADCON2bits.BUFS
#define _CHPS ADCON2bits.CHPS
#define _CSCNA ADCON2bits.CSCNA
#define _VCFG ADCON2bits.VCFG
#define _SMPI0 ADCON2bits.SMPI0
#define _SMPI1 ADCON2bits.SMPI1
#define _SMPI2 ADCON2bits.SMPI2
#define _SMPI3 ADCON2bits.SMPI3
#define _CHPS0 ADCON2bits.CHPS0
#define _CHPS1 ADCON2bits.CHPS1
#define _VCFG0 ADCON2bits.VCFG0
#define _VCFG1 ADCON2bits.VCFG1
#define _VCFG2 ADCON2bits.VCFG2

#define _ADCS ADCON3bits.ADCS
#define _ADRC ADCON3bits.ADRC
#define _SAMC ADCON3bits.SAMC
#define _ADCS0 ADCON3bits.ADCS0
#define _ADCS1 ADCON3bits.ADCS1
#define _ADCS2 ADCON3bits.ADCS2
#define _ADCS3 ADCON3bits.ADCS3
#define _ADCS4 ADCON3bits.ADCS4
#define _ADCS5 ADCON3bits.ADCS5
#define _SAMC0 ADCON3bits.SAMC0
#define _SAMC1 ADCON3bits.SAMC1
#define _SAMC2 ADCON3bits.SAMC2
#define _SAMC3 ADCON3bits.SAMC3
#define _SAMC4 ADCON3bits.SAMC4

#define _CH0SA ADCHSbits.CH0SA
#define _CH0NA ADCHSbits.CH0NA
#define _CH123SA ADCHSbits.CH123SA
#define _CH123NA ADCHSbits.CH123NA
#define _CH0SB ADCHSbits.CH0SB
#define _CH0NB ADCHSbits.CH0NB
#define _CH123SB ADCHSbits.CH123SB
#define _CH123NB ADCHSbits.CH123NB
#define _CH0SA0 ADCHSbits.CH0SA0
#define _CH0SA1 ADCHSbits.CH0SA1
#define _CH0SA2 ADCHSbits.CH0SA2
#define _CH0SA3 ADCHSbits.CH0SA3
#define _CH123NA0 ADCHSbits.CH123NA0
#define _CH123NA1 ADCHSbits.CH123NA1
#define _CH0SB0 ADCHSbits.CH0SB0
#define _CH0SB1 ADCHSbits.CH0SB1
#define _CH0SB2 ADCHSbits.CH0SB2
#define _CH0SB3 ADCHSbits.CH0SB3
#define _CH123NB0 ADCHSbits.CH123NB0
#define _CH123NB1 ADCHSbits.CH123NB1

#define _PCFG0 ADPCFGbits.PCFG0
#define _PCFG1 ADPCFGbits.PCFG1
#define _PCFG2 ADPCFGbits.PCFG2
#define _PCFG3 ADPCFGbits.PCFG3
#define _PCFG4 ADPCFGbits.PCFG4
#define _PCFG5 ADPCFGbits.PCFG5
#define _PCFG6 ADPCFGbits.PCFG6
#define _PCFG7 ADPCFGbits.PCFG7
#define _PCFG8 ADPCFGbits.PCFG8
#define _PCFG9 ADPCFGbits.PCFG9
#define _PCFG10 ADPCFGbits.PCFG10
#define _PCFG11 ADPCFGbits.PCFG11
#define _PCFG12 ADPCFGbits.PCFG12
#define _PCFG13 ADPCFGbits.PCFG13
#define _PCFG14 ADPCFGbits.PCFG14
#define _PCFG15 ADPCFGbits.PCFG15

#define _CSSL0 ADCSSLbits.CSSL0
#define _CSSL1 ADCSSLbits.CSSL1
#define _CSSL2 ADCSSLbits.CSSL2
#define _CSSL3 ADCSSLbits.CSSL3
#define _CSSL4 ADCSSLbits.CSSL4
#define _CSSL5 ADCSSLbits.CSSL5
#define _CSSL6 ADCSSLbits.CSSL6
#define _CSSL7 ADCSSLbits.CSSL7
#define _CSSL8 ADCSSLbits.CSSL8
#define _CSSL9 ADCSSLbits.CSSL9
#define _CSSL10 ADCSSLbits.CSSL10
#define _CSSL11 ADCSSLbits.CSSL11
#define _CSSL12 ADCSSLbits.CSSL12
#define _CSSL13 ADCSSLbits.CSSL13
#define _CSSL14 ADCSSLbits.CSSL14
#define _CSSL15 ADCSSLbits.CSSL15
*/


#define	VARIAVLE		5
#define	AVR_COUNTER		20
#define	VLOTAGE25		512


unsigned int 	AvrCnt;
unsigned int 	Analog_Data[AVR_COUNTER][16];
//unsigned int 	Voltage[16];
unsigned int 	BefVoltage=0;

unsigned long 	AvrCntSum,x_AvrCntSum;



int   __attribute__((section(".usercode"))) VlotageReturn(unsigned int ch)
{
	unsigned int i;

	AvrCntSum=0;

	if( (ch==0) || (ch==2) ){
		for(i=0;i<AVR_COUNTER;i++){
			x_AvrCntSum=(unsigned long)(Analog_Data[i][ch]);					
			AvrCntSum=(AvrCntSum + x_AvrCntSum);			
		}


		AvrCntSum=(AvrCntSum * 1000);
		AvrCntSum=(AvrCntSum/AVR_COUNTER);
		AvrCntSum=(AvrCntSum / 203);

	
		AvrCntSum=(AvrCntSum * 76);
		i=(unsigned int)(AvrCntSum/1000);

		if(ch==2){
			i=(unsigned int)(i/10);
		}

/*
		if(AvrCntSum >= 1000){
			AvrCntSum=(AvrCntSum - 1000);
			AvrCntSum=(AvrCntSum * 125)/(1000);	
			AvrCntSum=(AvrCntSum +1);
		}
		else	AvrCntSum=0;
	
	
		i=(unsigned int)(AvrCntSum);
		
*/
	}
	else{ 
		for(i=0;i<AVR_COUNTER;i++){
			x_AvrCntSum=(unsigned long)(Analog_Data[i][ch]);
			if(x_AvrCntSum >= VLOTAGE25)	x_AvrCntSum = (x_AvrCntSum-VLOTAGE25);
			else							x_AvrCntSum = (VLOTAGE25-x_AvrCntSum);
			if(x_AvrCntSum > 500)			x_AvrCntSum = 0;
					
			AvrCntSum=(AvrCntSum + x_AvrCntSum);
		}

		AvrCntSum=(AvrCntSum * 100);
		AvrCntSum=(AvrCntSum/AVR_COUNTER);
		AvrCntSum=(unsigned int)(AvrCntSum / 203);
	
		if(AvrCntSum > 2) 	AvrCntSum=(AvrCntSum + 3);
		else				AvrCntSum=0;
		
		i=(unsigned int)(AvrCntSum);
	}

	switch(ch){
		case	0:
			if(i <= 5)	i=0;
			break;
		case	1:
			if(i <= 5)	i=0;
			break;
		case	2:
			if(i <= 5)	i=0;
			break;
		case	3:
			if(i <= 2)	i=0;
			break;
	}


	return(i);	
}




int   __attribute__((section(".usercode"))) Ad_Check(void)
{
	unsigned char i;

	if(AvrCnt >= AVR_COUNTER){
		for(i=0;i<14;i++){
			sRamDArry[mMainVoltage+i]=VlotageReturn(i);
		}		
		AvrCnt = 0;
	}
	return(0);
}




int   __attribute__((section(".usercode"))) ADInit(void)
{
///////////////////// ADPCFG /////////////////////////////////
	ADPCFG=0x03;   // an0,an1=digital

	_VCFG=0;   // ref+ = avdd ,ref-=avss  // ad con2(13-15)
	_VCFG0=0;
	_VCFG1=0;
	_VCFG2=0;

/////////////////////ADCON3(bit 0-5) /////////////////////////////////
	_ADCS=0x20;
	_ADCS0=0;
	_ADCS1=0;
	_ADCS2=0;
	_ADCS3=0;
	_ADCS4=0;
	_ADCS5=1;


/////////////////////ADCON2(bit 8-9) /////////////////////////////////
	_CHPS=0;
	_CHPS0=0;
	_CHPS1=0;


/////////////////////ADCON1(bit(3) /////////////////////////////////
	_SIMSAM=0; 	// not multi sample hold

///////////////////// ADCSSL /////////////////////////////////
	ADCSSL=0x04;	// an2 scan select


/////////////////////ADCHS /////////////////////////////////
	_CH0SA=4;
	_CH0SA0=0;
	_CH0SA1=0;
	_CH0SA2=1;		//ad ch2
	_CH0SA3=0;
	_CH0NA=0;  		// cha0 negative == vref=vss
	_CH123SA=0;
	_CH123NA=0;	  	// cha123 negative == vref=vss


	_CH0SB=4;
	_CH0SB0=0;
	_CH0SB1=0;
	_CH0SB2=1;		//ad ch2
	_CH0SB3=0;
	_CH0NB=0;  		// cha0 negative == vref=vss
	_CH123SB=0;
	_CH123NB=0;	  	// cha123 negative == vref=vss



/////////////////////ADCON1 /////////////////////////////////
	_DONE=0; 
	_SAMP=0;
	_ASAM=0;   	// when samp=1  start 
	_SIMSAM=0; 	// not multi sample hold
	_ADSIDL=1;  // stop idel mode 

//	_SSRC=7		// auto conversion 
	_SSRC0=1; 
	_SSRC1=1;
	_SSRC2=1;

//	_FORM=0;	// data format = unsigned int 
	_FORM0=0;
	_FORM1=0;
	


/////////////////////ADCON2 /////////////////////////////////
	_ALTS=1;	// muxa and muxb  use
	_BUFM=0;    // 16bit buf use
	_BUFS=0;
	_CSCNA=1;   //?
	
	_SMPI=0;    // individual interrupt
	_SMPI0=0;
	_SMPI1=0;
	_SMPI2=0;
	_SMPI3=0;
	
	_CHPS=0;
	_CHPS0=0;
	_CHPS1=0;
	
	_VCFG=0;   // ref+ = avdd ,ref-=avss
	_VCFG0=0;
	_VCFG1=0;
	_VCFG2=0;



/////////////////////ADCON3 /////////////////////////////////
	_ADCS=0x20;
	_ADCS0=0;
	_ADCS1=0;
	_ADCS2=0;
	_ADCS3=0;
	_ADCS4=0;
	_ADCS5=1;
	
	_ADRC=0;

	_SAMC=0x10;
	_SAMC0=0;
	_SAMC1=0;
	_SAMC2=0;
	_SAMC3=0;
	_SAMC4=1;



	_ADCS=0x3f;

	ADCON3=0xffff;
	_ADRC=0;

/////////////////////ADCHS /////////////////////////////////
	_CH0SA=4;
	_CH0SA0=0;
	_CH0SA1=0;
	_CH0SA2=1;		//ad ch2
	_CH0SA3=0;
	_CH0NA=0;  		// cha0 negative == vref=vss
	_CH123SA=0;
	_CH123NA=0;	  	// cha123 negative == vref=vss


	_CH0SB=4;
	_CH0SB0=0;
	_CH0SB1=0;
	_CH0SB2=1;		//ad ch2
	_CH0SB3=0;
	_CH0NB=0;  		// cha0 negative == vref=vss
	_CH123SB=0;
	_CH123NB=0;	  	// cha123 negative == vref=vss


///////////////////// ADCSSL /////////////////////////////////






///////////////////
	_ASAM=1;   	// when samp=1  start 
	ADCSSL=0xfffc;	// an2 scan select

_SMPI=13;  // 14 conversion

_SIMSAM=0;
_BUFM=0;
_ALTS=0;

//////////////////////
	_ADIF=0;	// interrupt flag disable 
	_ADON=1;	// AD module on 
	_ADIE=1;	// AD interrupt on

	return(0);    
}




void _ISR_X _ADCInterrupt(void)
{
	unsigned int i;
	unsigned int *pt;

	_ADIF=0;	// interrupt flag disable 
	
	pt = (unsigned int *)(&ADCBUF0);


	if( (LuLdTime >= 50) && (!bBlinck)){
		if(AvrCnt < AVR_COUNTER){
			for(i=0;i<14;i++){
				Analog_Data[AvrCnt][i]= *(pt +i);		
			}
			AvrCnt++;
		}
	}

}



