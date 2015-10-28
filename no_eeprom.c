

#include  <p30f6010.h>            
#include  "30f6010_io.h"            
#include  "iodef.h"            
#include  "host_io.h"            



#define NVMCON_WriteOneWordEEPROM   0x4004

#define NVMCON_EraseOneWordEEPROM   0x4044

#define EEP_PAGE_ADDR               0x7f
#define EEP_BASE_ADDR               0xf000


//unsigned int  EWrTime=0;


unsigned char    __attribute__((section(".usercode"))) EEPROM_write_Ready(void)
{
    return(WR_START); 
}



unsigned char    __attribute__((section(".usercode"))) EEPROM_write_Enable(void)
{
   WREN_ON=1;
   return(0);
}


unsigned char    __attribute__((section(".usercode"))) EEPROM_write_Disable(void)
{
   WREN_ON=0;
   return(0);
}




void   __attribute__((section(".usercode"))) startWriting( unsigned int Eep_Command )
{

    EepTime=0;

 	NVMCON = Eep_Command; 
 
   	asm("mov #0x55, w0");
    asm("mov w0, NVMKEY"); 
    asm("mov #0xAA, w0");
    asm("mov w0, NVMKEY"); 

    WR_START = 1;

    asm("nop"); 
    asm("nop"); 

    EepTime=0;

    asm("CLRWDT");  

    while(WR_START){  
    };

}


void   __attribute__((section(".usercode"))) EraseEepromWard( unsigned int Eep_Addr )
{

    NVMADRU = EEP_PAGE_ADDR;
   	NVMADR  = EEP_BASE_ADDR + Eep_Addr;
    startWriting( NVMCON_EraseOneWordEEPROM );

    WREN_ON = 0; 

}



unsigned int      __attribute__((section(".usercode"))) WriteDataEepromWard( unsigned int Eep_Addr ,unsigned int Eep_Data )
{ 
	return(0);


    TBLPAG = EEP_PAGE_ADDR;
    Eep_Addr  = Eep_Addr + EEP_BASE_ADDR ;  

    asm("mov.w  [w14],w0");    
    asm("mov.w  [w14+2],w1");    

    asm ("TBLWTL w1,[w0]");
    startWriting( NVMCON_WriteOneWordEEPROM  ); 		 
    WREN_ON = 0; 

}




unsigned char   __attribute__((section(".usercode"))) EEPROM_Read_Byte(unsigned int Eep_Addr)
{
	return(0);

    unsigned char value;

	TBLPAG    = EEP_PAGE_ADDR;
    Eep_Addr  = EEP_BASE_ADDR + Eep_Addr;
    asm("mov.w  [w14],w0");
	asm(" TBLRDL.B [w0],w1");
    asm("mov.b w1,[w14+2]");
	return((unsigned char)value);

}


unsigned int   __attribute__((section(".usercode"))) EEPROM_Read_Word(unsigned int Eep_Addr)
{
	return(0);

    unsigned int value;

	TBLPAG = EEP_PAGE_ADDR;
    Eep_Addr  = EEP_BASE_ADDR + Eep_Addr;

    asm("mov.w  [w14],w0");
	asm("TBLRDL [w0],w1");
    asm("mov.w w1,[w14+2]");


	return(value);

}



unsigned int    __attribute__((section(".usercode"))) EEPROM_Write_Word(unsigned int Eep_Adr,unsigned int Eep_Data)
{
	return(0);

  if(EEPROM_write_Ready())  return(1);

  EraseEepromWard(Eep_Adr);
  WriteDataEepromWard(Eep_Adr,Eep_Data);

  return(0);
}



unsigned int    __attribute__((section(".usercode"))) EEPROM_Write_Byte(unsigned int Eep_Adr,unsigned int Eep_Data)
{
	return(0);

  unsigned int tmp;

 
  tmp=EEPROM_Read_Word((Eep_Adr & 0xfffe));

  if(Eep_Adr & 0x01){
    tmp=(tmp & 0x00ff) | (Eep_Data << 8);
  }
  else{
    tmp=(tmp & 0xff00) | (Eep_Data);
  }

  if(EEPROM_write_Ready())  return(1);

  EraseEepromWard(Eep_Adr & 0xfffe);
  WriteDataEepromWard(Eep_Adr,tmp);
  EEPROM_Read_Word(Eep_Adr);


  return(0);
}





