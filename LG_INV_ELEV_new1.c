

      
//#include  <p30f6010.h>
            
#include <p30fxxxx.h>      
#include  "iodef.h"
#include  "host_io.h"            
#include  "door.h"
#include  "com.h"
#include  "counter.h"
#include  "default_setup.h"


void    DspFloorSet(void);

void    CarStopCmd(void);
void    CarUpStartCmd(void);
void    CarDnStartCmd(void);
void    CarCurFloorRead(void);
void	InverterErrorCheck(void);



//extern   void   CallMeUpDnDirectionSet(void);                                                                       
extern   void   ClrUpDnWard(void);
extern   void   UpDnCallClear(void);           
extern   void   UpWard(void);
extern	 void   DnWard(void);


const   unsigned        int    seg1[]={0x0,0x06,0x0b,0x07};
const   unsigned        int    seg0[]={0x3f,0x06,0xdb,0xcf,0xe6,0xed,0xfd,0x27,0xff,0xef,0x5c};
 


void __attribute__((section(".usercode")))   DspFloorSet(void)
{
    LocalType i,j;  

    if(bDspSeq || !(bCarErr || bCarStopNoRun || bCarStop) ){    
        i=(UserDataType)(sRamDArry[mcurfloor] * 2);
        
		i=(i + F_FlrDspCh);

    	sRamDArry[S0_FLOOR]  = sRamDArry[mcurfloor]+1;
            
        sRamDArry[DSP1]  = cF_FLRDSPCH((unsigned long)i);
        sRamDArry[DSP2]  = cF_FLRDSPCH((unsigned long)(i+1)); 
    
        j=sRamDArry[mcurfloor]+1;
    
        i=(j%10);    
        SegData0=~seg0[i];
        SegData0=(SegData0 | 0x80);
    
        i=(j/10);
        SegData1=~seg0[i];
        SegData1=(SegData1 | 0x80);
    
        SegData2=0xff;
    }
    else{
/*
        i=(UserDataType)(sRamDArry[mcurfloor] * 2);
        
    	sRamDArry[S0_FLOOR]  = sRamDArry[mcurfloor]+1;
            
        sRamDArry[DSP1]  = cF_FLRDSPCH((unsigned long)i);
        sRamDArry[DSP2]  = cF_FLRDSPCH((unsigned long)(i+1)); 
        j=sRamDArry[mcurfloor]+1;
*/    

        j=SegError;
    
        i=(j%10);    
        SegData0=~seg0[i];
        SegData0=(SegData0 | 0x80);
    
        i=(j/10);
        SegData1=~seg0[i];
        SegData1=(SegData1 | 0x80);
    
        SegData2=( ~(0x79) | 0x80);
    }    
}



////////////////
/*/////////// I/O MODE////////////////////////////
000 x  zero         //
001 x  60           //fun 13    
010 x  decrease     //fun 14
011 x  90           //fun 15
100 x  manual       //fun 16
101 x  120          //fun 17
110 x  batery       //fun 18
111 x  150          //fun 19
///////////////////////////////////////*/
/*/////////// E/L MODE////////////////////////////
000 x  zero         //fun 12
001 x  manual       //fun 13
010 x  not use      //fun 14
011 x  decrease     //fun 15
100 x  auto         //fun 16
101 x  not use      //fun 17
110 x  not use      //fun 18
111 x  not use      //fun 19
///////////////////////////////////////*/

////////////////
void __attribute__((section(".usercode")))     CurSelSpeed(unsigned int spd)
{
    switch(spd){
        case    0:
            OUT_P1(0);
            OUT_P2(0);
            OUT_P3(0);    
            break;
        case    1:
            OUT_P1(1);
            OUT_P2(0);
            OUT_P3(0);    
            break;
        case    2:
            OUT_P1(0);
            OUT_P2(1);
            OUT_P3(0);    
            break;
        case    3:
            OUT_P1(1);
            OUT_P2(1);
            OUT_P3(0);    
            break;
        case    4:
            OUT_P1(0);
            OUT_P2(0);
            OUT_P3(1);    
            break;
        case    5:
            OUT_P1(1);
            OUT_P2(0);
            OUT_P3(1);    
            break;
        case    6:
            OUT_P1(0);
            OUT_P2(1);
            OUT_P3(1);    
            break;
        case    7:
            OUT_P1(1);
            OUT_P2(1);
            OUT_P3(1);    
            break;
        default:
            OUT_P1(0);
            OUT_P2(0);
            OUT_P3(0);    
            OUT_P4(0);    
            break;
    }
}




void __attribute__((section(".usercode")))    ManualSpeedCmd_IO(void)
{
    CurSelSpeed(cF_MANUALSPEED);                        
}



void __attribute__((section(".usercode")))    BatterySpeedCmd_IO(void)
{
    CurSelSpeed(cF_BATTERYSPEED);                        
}



void __attribute__((section(".usercode")))    DecreaseSpeedCmd_IO(void)
{
    CurSelSpeed(cF_DECREASESPEED);     
    CounterTime=0;                   
}


void __attribute__((section(".usercode")))    ZeroSpeedCmd_IO(void)
{
    CurSelSpeed(0);                        
}




void  __attribute__((section(".usercode")))  CarCurFloorRead_ELEV(void)
{
    if(IN_FID){
        sRamDArry[mReqStopFloor]   = (I_FS0_bit  & 0x3f);				

        NextFTime=0;		
    }         
    else{
        sRamDArry[mcurfloor] = (I_FS0_bit & 0x1f);  		      
        DspFloorSet();
        CurFTime=0;
    }
    
    if(IN_UND1)  bUnd=1;      
    else         bUnd=0;      
}



void  __attribute__((section(".usercode"))) 	InverterErrorCheck_ELEV(void)
{
    if((CurFTime>10) || (NextFTime>10)){
        if(bAuto){
            bCarStopNoRun=1;       	
            bsInvCurNext=1;
        }         		
        CurFTime=11;
        NextFTime=11;
    }
    else{	
        bsInvCurNext=0;
    }
}					




void  __attribute__((section(".usercode")))    CarLowSpeedCmd_IO(void)
{        
    if(bAuto && bManualAuto){
        if(bMoveDnOk && bMoveUpOk){
            if(sRamDArry[mSysStatus] >= sSLOW_SPEED)   sRamDArry[mSysStatus]=sSLOW_SPEED;                                                     
            DecreaseSpeedCmd_IO();
            bVoiceReady=1;
        }
   	    else{
            sRamDArry[mCarMoveState]=0;
        }    
    }	 		     
}






void  __attribute__((section(".usercode")))   CarStopCmd_IO(void)
{        
    switch(sRamDArry[mCarMoveState]){
        case	0:
            sRamDArry[mCarMoveState]=1;
            break;							
        case  1:                                   
            sRamDArry[mCarMoveState]=2;
            bOneStep=0;               
            bTwoStep=0;                
            bThreeStep=0;              
            bFourStep=0;               
            break;
        case  2:
            CounterTime=0;
            ZeroSpeedCmd_IO();

            if(LuLdTime>iF_Bk1OffTime){
                bOneStep=1;               
                OUT_BRK(0);                     
            }              

            if(LuLdTime>iF_Bk2OffTime){
                bTwoStep=1;               
                OUT_BK2(0);
            }              

            if(LuLdTime>iF_UDOffTime){
                bThreeStep=1;               
          	    CallMeUpDnDirectionSet();                                                                       
                OUT_U_W(0);                                                               
                OUT_D_W(0);                                                        
            }              

            if(LuLdTime>iF_P4OffTime){
                bFourStep=1;               
                OUT_P4(0);
            }


            if(bOneStep && bTwoStep && bThreeStep && bFourStep){               
                ZeroSpeedCmd_IO();
                OUT_P4(0);
                OUT_U_W(0);                                                               
                OUT_D_W(0);                                                        
                OUT_BRK(0);
                OUT_BK2(0);



                bMoveCar=0;
                bCarUpMove=0;
                bCarDnMove=0;
                S2_CAR_MOVE1=0;
   				ElevMoveTime=0;
                if(!bAuto)  ClrUpDnWard();
                bVoiceReady=0;   
            }              
            break;
        default:
            sRamDArry[mCarMoveState]=0;
            break;
    }
}



LocalType  __attribute__((section(".usercode")))  OilTypeRunState(void)
{
    LocalType i;  
    
    i=0;

    if((INVERTER_CHECK == D_F) && (!OilLopeTypeChk) && S1_AUTO1 && !bD_F_FloorOn && IN_LU && IN_LD ){ 
        i=1;
    }
        
    return(i);
}


void  __attribute__((section(".usercode")))  UpDnRunOut(void)
{
    if(bAuto)	RunSpeedCmd_IO();
    else{
        if(OilTypeRunState()){
            RunSpeedCmd_IO();
        }
        else{
            Mnanual_Speed_Sel();
        }
    } 
   
	if(bNextFlrChk==0){
		bNextFlrChk=1;
		NextFloorTime=0;
   		CounterTime=0;
	}
}



void  __attribute__((section(".usercode")))   CarUpStartCmd_IO(void)
{
//    bManualStop=0;
    if(bMoveUpOk){
        UpWard();

        if(sRamDArry[mcurfloor]< cF_TOPFLR)  sRamDArry[mReqStopFloor]  = sRamDArry[mcurfloor]+1;
        else                                 sRamDArry[mReqStopFloor]  = cF_TOPFLR;

        if(bAuto){
            if(sRamDArry[mSysStatus] > sUP)   sRamDArry[mSysStatus]=sUP;      
        }		   
        else{       
            if(sRamDArry[mSysStatus] > sMANUAL_UP)   sRamDArry[mSysStatus]=sMANUAL_UP;               	
        }              
		
     	
        switch(sRamDArry[mCarMoveState]){
             case  0:
                ZeroSpeedCmd_IO();
                OUT_U_W(0);                                                               
                OUT_D_W(0);                                                        
                OUT_BRK(0);                     
                OUT_BK2(0);
                OUT_P4(0);    

				bNextFlrChk=0;
                sRamDArry[mCarMoveState]=1;
                break;
             case  1:
                S2_CAR_MOVE1=1; 
                bMoveCar=1;
                bCarUpMove=1;
                bCarDnMove=0;

                if(ElevMoveTime > cF_STTM1){
                    if(INVERTER_CHECK != LG)    OUT_P4(1);
                }
                if(ElevMoveTime > cF_STTM2){
                    OUT_U_W(1);
                }                                               
                if(ElevMoveTime > cF_STTM3){
                    OUT_BK2(1);
                }              
                if(ElevMoveTime > cF_STTM4){
                     OUT_BRK(1);                     
                }
                if(ElevMoveTime > cF_STTM5){
                    UpDnRunOut();
                }
                break;                  
              default:
                sRamDArry[mCarMoveState]=0;
                break;
        }
    }
    else{
        sRamDArry[mCarMoveState]=0;
    }     
}



void  __attribute__((section(".usercode")))   CarDnStartCmd_IO(void)
{
    if(bMoveDnOk){   
        DnWard();
        if(sRamDArry[mcurfloor]>0)  sRamDArry[mReqStopFloor]  = sRamDArry[mcurfloor]-1;
        else                        sRamDArry[mReqStopFloor]  = 0;

        if(bAuto){
            if(sRamDArry[mSysStatus] > sDN)   sRamDArry[mSysStatus]=sDN;      
        }		   
        else{       
            if(sRamDArry[mSysStatus] > sMANUAL_DN)   sRamDArry[mSysStatus]=sMANUAL_DN;               	
        }              

        switch(sRamDArry[mCarMoveState]){
            case  0:
                ZeroSpeedCmd_IO();
                OUT_U_W(0);                                                               
                OUT_D_W(0);                                                        
                OUT_BRK(0);                     
                OUT_BK2(0);
                OUT_P4(0);    

				bNextFlrChk=0;
                sRamDArry[mCarMoveState]=1;
                break;
             case  1:
                S2_CAR_MOVE1=1; 
                bMoveCar=1;
                bCarUpMove=0;
                bCarDnMove=1;

                if(ElevMoveTime > cF_STTM1){
                    if(INVERTER_CHECK != LG)    OUT_P4(1);
                }
                if(ElevMoveTime > cF_STTM2){
                    OUT_D_W(1);
                }                                               
                if(ElevMoveTime > cF_STTM3){
                    OUT_BK2(1);
                }              
                if(ElevMoveTime > cF_STTM4){
                    OUT_BRK(1);                     
                }
                if(ElevMoveTime > cF_STTM5){
                    UpDnRunOut();
                }
                break;                  

            default:
                sRamDArry[mCarMoveState]=0;
                break;
        }
    }
    else{
        sRamDArry[mCarMoveState]=0;
    }        
}




LocalType  __attribute__((section(".usercode")))  CurFloorRead_OnOff(void)
{
    LocalType   j;

    j=0;

    if(!IN_SD1 && !IN_EMG){				//modify ?
        sRamDArry[mcurfloor]=0;
        j++;
    }
    if(!IN_SU1 && !IN_EMG){               //modify ?
        sRamDArry[mcurfloor]=cF_TOPFLR;
        j++;
    }
    if(!IN_X0 && (cF_TOPFLR >= 2)){
        sRamDArry[mcurfloor]=1;
        j++;
    }
    if(!IN_X1 && (cF_TOPFLR >= 3)){
        sRamDArry[mcurfloor]=2;
        j++;
    }
    if(!IN_X2 && (cF_TOPFLR >= 4)){
        sRamDArry[mcurfloor]=3;
        j++;
    }
    if(!IN_X3 && (cF_TOPFLR >= 5)){
        sRamDArry[mcurfloor]=4;
        j++;
    }
    if(!IN_X4 && (cF_TOPFLR >= 6)){
        sRamDArry[mcurfloor]=5;
        j++;
    }
    if(!IN_X5 && (cF_TOPFLR >= 7)){
        sRamDArry[mcurfloor]=6;
        j++;
    }
    if(!IN_X6 && (cF_TOPFLR >= 8)){
        sRamDArry[mcurfloor]=7;
        j++;
    }



    bD_F_FloorOn=0;

    if(j == 1){
        bD_F_FloorOn=1;        
    }
    else if(j >= 2){
        bCarErr=1;
    }
    
    return(j);
}




int  __attribute__((section(".usercode")))  Up_Direct_Dec_Chk(void)
{
  
	if(sRamDArry[mcurfloor] >= cF_TOPFLR)	return(0);
    else if( (sRamDArry[mcurfloor] & ONLY_FLR) == (sRamDArry[mAckStopFloor] & ONLY_FLR)){
		if(bDac)	return(0);
	}

	return(1);
}

int  __attribute__((section(".usercode")))  Dn_Direct_Dec_Chk(void)
{
  
	if(sRamDArry[mcurfloor] == 0)	return(0);
    else if( (sRamDArry[mcurfloor] & ONLY_FLR) == (sRamDArry[mAckStopFloor] & ONLY_FLR)){
		if(bDac)	return(0);
	}

	return(1);
}



void  __attribute__((section(".usercode")))  CarCurFloorRead_OnOff(void)
{

    LocalType ReqFlr;  


    CurFTime=0;
	NextFTime=0;

    if(bFhmRun) return;

    CurFloorRead_OnOff();


    ReqFlr=(LocalType)(sRamDArry[mReqStopFloor] & ONLY_FLR);     	



    if(sRamDArry[mcurfloor] > cF_TOPFLR){
        sRamDArry[mcurfloor]=0;
    }


    if(bCarUpMove){
        if(!IN_SU1){							//modify ?
            if(!IN_EMG)	sRamDArry[mcurfloor]=cF_TOPFLR;   
            bUnd=1;
            CarLowSpeedCmd_IO();
        }

        if(bD_F_FloorOn){
			if( Up_Direct_Dec_Chk() == 0){
                bUnd=1;
                CarLowSpeedCmd_IO();
                if(sRamDArry[mcurfloor] >= cF_TOPFLR){
                    sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);
                }
            }
        }



	        if(bUnd){
	            CarLowSpeedCmd_IO();
	        }			
	        else{
	            if(sRamDArry[mcurfloor] < cF_TOPFLR){
	                if(!bD_F_FloorOn)    sRamDArry[mReqStopFloor]  = ((sRamDArry[mcurfloor]+1) | CAR_READY);                
	    			else if(((sRamDArry[mReqStopFloor] & ONLY_FLR) != (sRamDArry[mAckStopFloor] & ONLY_FLR))  ||  !(sRamDArry[mReqStopFloor] & CAR_READY)){
	                    sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] & ~UPDN_READY);
	                }
	            }
	            else    sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);                        
	        }

			if(sRamDArry[mcurfloor] >= cF_TOPFLR){
				sRamDArry[mReqStopFloor] = (sRamDArry[mcurfloor] | CAR_READY);  
		    	sRamDArry[mAckStopFloor] = (sRamDArry[mcurfloor] | CAR_READY);
			}
    }


    else if(bCarDnMove){
        if(!IN_SD1){                             //modify ?
            if(!IN_EMG)	sRamDArry[mcurfloor]=0;

            bUnd=1;
            CarLowSpeedCmd_IO();
        }

        if(bD_F_FloorOn){
			if( Dn_Direct_Dec_Chk() == 0){
                bUnd=1;
                CarLowSpeedCmd_IO();
                if(sRamDArry[mcurfloor] == 0){
                    	sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);
                }
            }
        }

	        if(bUnd){
	            CarLowSpeedCmd_IO();
	        }			
	        else{
	            if(sRamDArry[mcurfloor] > 0){
	                if(!bD_F_FloorOn)    sRamDArry[mReqStopFloor]  = ((sRamDArry[mcurfloor]- 1) | CAR_READY);                
	    			else if(((sRamDArry[mReqStopFloor] & ONLY_FLR) != (sRamDArry[mAckStopFloor] & ONLY_FLR))  ||  !(sRamDArry[mReqStopFloor] & CAR_READY)){
	                    sRamDArry[mReqStopFloor]  = sRamDArry[mcurfloor];                        
	                }
	            }
	            else    sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);                        
	        }

			if(sRamDArry[mcurfloor] == 0){
				sRamDArry[mReqStopFloor] = (sRamDArry[mcurfloor] | CAR_READY);  
		    	sRamDArry[mAckStopFloor] = (sRamDArry[mcurfloor] | CAR_READY);
			}

    }

    else{
        bUnd=0;
        sRamDArry[mReqStopFloor]  = sRamDArry[mcurfloor];            
    }

    DspFloorSet();    
}




void  __attribute__((section(".usercode"))) 	InverterErrorCheck_IO(void)
{
    bsInvCurNext=0;
}



void   __attribute__((section(".usercode")))   CarStopCmd(void)
{
    if(IN_LU && IN_LD)    bCarErr=1;

    CarStopCmd_IO();
	S3_SHIFT1=0; //shift   
}


void  __attribute__((section(".usercode")))    CarUpStartCmd(void)
{
    bSaveFlash=1;
    CarUpStartCmd_IO();
}


void  __attribute__((section(".usercode")))    CarDnStartCmd(void)
{
    bSaveFlash=1;
    CarDnStartCmd_IO();
}



void  __attribute__((section(".usercode")))    CarCurFloorRead(void)
{
    if(INVERTER_CHECK == LG)                                CarCurFloorRead_ELEV();                                             
    else if(INVERTER_CHECK == D_F)                          CarCurFloorRead_OnOff();                                             
    else                                                    CarCurFloorRead_IO();

}



void   __attribute__((section(".usercode"))) InverterErrorCheck(void)
{
    if(INVERTER_CHECK == LG)            InverterErrorCheck_ELEV();                                             
    else                                InverterErrorCheck_IO();
}
