


#include <p30fxxxx.h>
#include  "..\..\system_com\memory_map.h"

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


extern	UserDataType	NoStart;




void  __attribute__((section(".usercode")))    Mnanual_Speed_Sel(void)
{
       
    ManualSpeedCmd_IO();

    if(S1_POWER_FAIL && (cF_BATTERYSPEED > 0)){
 		BatterySpeedCmd_IO();
	}
                  				
    if(bCarUpMove){
        if(!IN_LU || !IN_LD ){
            if(!IN_SU1 || bSearchHome ){     
                DecreaseSpeedCmd_IO();
            }
        } 
    }
    else if(bCarDnMove){
        if(!IN_LU || !IN_LD){
            if(!IN_SD1 || bSearchHome){
                DecreaseSpeedCmd_IO();
            }
        }
    }
}



void __attribute__((section(".usercode")))    RunSpeedCmd_IO(void)
{

    if(INVERTER_CHECK == LG)    CurSpeed=SPEED_60;
    if(INVERTER_CHECK == D_F)   CurSpeed=SPEED_60;

    switch(CurSpeed){
        case    SPEED_30:    
            CurSelSpeed(cF_SPEED30);                        
            break;
        case    SPEED_45:
            CurSelSpeed(cF_SPEED45);                        
            break;
        case    SPEED_60:
            CurSelSpeed(cF_SPEED60);                        
            break;
        case    SPEED_90:
            CurSelSpeed(cF_SPEED90);                        
            break;
        case    SPEED_105:
            CurSelSpeed(cF_SPEED105);                        
            break;
        case    SPEED_120:
            CurSelSpeed(cF_SPEED120);                        
            break;
        case    SPEED_150:
            CurSelSpeed(cF_SPEED150);                        
            break;
        case    SPEED_180:
            CurSelSpeed(cF_SPEED180);                        
            break;
        case    SPEED_210:
            CurSelSpeed(cF_SPEED210);                        
            break;
        default:
            CurSelSpeed(0);                        
            break;
    }
}




LocalType  __attribute__((section(".usercode")))    UpForceSettingSpeed(void)
{
	if(UpLimitSpeedCheck())													return(0);
    else if( sRamDArry[mcurfloor] >= cF_TOPFLR )							return(0);
	else																	return(1);
}


LocalType  __attribute__((section(".usercode")))    DnForceSettingSpeed(void)
{
	if(DnLimitSpeedCheck())											return(0);
    else if( sRamDArry[mcurfloor] == 0 )							return(0);
	else															return(1);
}
	
	
	
LocalType  __attribute__((section(".usercode")))    ForceSettingSpeed(void)
{

	unsigned int spd_change_sta;

	spd_change_sta=0;

    if(cF_LIMIT_SPEED >= SPEED_210)	return(0);
	if(bUnd)						return(0);	


    if(bAuto && bCarUpMove){
		if(cF_ELEV_SPEED > cF_LIMIT_SPEED){ 
			if( !UpForceSettingSpeed()){
    			if(CurSpeed > cF_LIMIT_SPEED){
					spd_change_sta=1;
                }
            }
		}            
    }
    else if(bAuto && bCarDnMove){
		if(cF_ELEV_SPEED > cF_LIMIT_SPEED){ 
			if( !DnForceSettingSpeed()){
    			if(CurSpeed > cF_LIMIT_SPEED){
					spd_change_sta=1;
                }
            }
		}            
    }
        


    if( (spd_change_sta==1) && (CurSpeed > cF_LIMIT_SPEED)){
        CurSpeed=cF_LIMIT_SPEED;
        switch(cF_LIMIT_SPEED){
            case    SPEED_30:
                DecreasePulse=GET_LONG(DEC_PULSE_30);
                break;
            case    SPEED_45:
                DecreasePulse=GET_LONG(DEC_PULSE_45);
                break;
            case    SPEED_60:
                DecreasePulse=GET_LONG(DEC_PULSE_60);
                break;
            case    SPEED_90:
                DecreasePulse=GET_LONG(DEC_PULSE_90);
                break;
            case    SPEED_105:
                DecreasePulse=GET_LONG(DEC_PULSE_105);
                break;
            case    SPEED_120:
                DecreasePulse=GET_LONG(DEC_PULSE_120);
                break;
            case    SPEED_150:
                DecreasePulse=GET_LONG(DEC_PULSE_150);
                break;
            case    SPEED_180:
                DecreasePulse=GET_LONG(DEC_PULSE_180);
                break;
            case    SPEED_210:
                DecreasePulse=GET_LONG(DEC_PULSE_210);
                break;
            default:
				spd_change_sta=0;
                break;

        }


        if(spd_change_sta==1){
            if(bCarUpMove)  StopMinimumPulse=DecreasePulse+CurPulse;
            else            StopMinimumPulse=CurPulse-DecreasePulse;
	        RunSpeedCmd_IO();
			bSetSpeedOn=1;                    
        }
    }



/*
    if(bSetSpeedOn && (CurSpeed > cF_LIMIT_SPEED)){
        CurSpeed=cF_LIMIT_SPEED;

        switch(cF_LIMIT_SPEED){
            case    SPEED_30:
                DecreasePulse=GET_LONG(DEC_PULSE_30);
                break;
            case    SPEED_45:
                DecreasePulse=GET_LONG(DEC_PULSE_45);
                break;
            case    SPEED_60:
                DecreasePulse=GET_LONG(DEC_PULSE_60);
                break;
            case    SPEED_90:
                DecreasePulse=GET_LONG(DEC_PULSE_90);
                break;
            case    SPEED_105:
                DecreasePulse=GET_LONG(DEC_PULSE_105);
                break;
            case    SPEED_120:
                DecreasePulse=GET_LONG(DEC_PULSE_120);
                break;
            case    SPEED_150:
                DecreasePulse=GET_LONG(DEC_PULSE_150);
                break;
            case    SPEED_180:
                DecreasePulse=GET_LONG(DEC_PULSE_180);
                break;
            case    SPEED_210:
                DecreasePulse=GET_LONG(DEC_PULSE_210);
                break;
            default:
                bSetSpeedOn=0;
                break;

        }


        if(bSetSpeedOn){
            if(bCarUpMove)  StopMinimumPulse=DecreasePulse+CurPulse;
            else            StopMinimumPulse=CurPulse-DecreasePulse;
	        RunSpeedCmd_IO();
        }
    }
*/

	return(0);
}
	
	
	
void  __attribute__((section(".usercode")))  CarCurFloorRead_IO(void)
{
    unsigned long decrease_pulse;
    unsigned long tmppulse1,tmppulse2,newFloor;

    LocalType ReqFlr;  


    CurFTime=0;
	NextFTime=0;

    if(bFhmRun) return;


    ForceSettingSpeed();


    ReqFlr=(LocalType)(sRamDArry[mReqStopFloor] & ONLY_FLR);     	


    if(sRamDArry[mcurfloor] > cF_TOPFLR){
        sRamDArry[mcurfloor]=0;
    }



    if(bCarUpMove){
        if(!IN_SU1){
            if(!IN_EMG)	sRamDArry[mcurfloor]=cF_TOPFLR;   //modify ?
            bUnd=1;
            CarLowSpeedCmd_IO();
        }


        if(!IN_SU2){
			if(CurMeterPerMin > (cF_SU2SD2_VELOCITY * 10)){
	            bUnd=1;
	   	        CarLowSpeedCmd_IO();
			}
		}


        if(UpLimitSpeedCheck() && (cF_X0X1_VELOCITY > cF_SU2SD2_VELOCITY)){   //x1
			if(CurMeterPerMin > (cF_X0X1_VELOCITY * 10)){
	            bUnd=1;
	   	        CarLowSpeedCmd_IO();
			}
		}



        if(sRamDArry[mcurfloor] >= cF_TOPFLR){
            tmppulse1=FLOOR_COUNT(cF_TOPFLR);
            tmppulse2=FLOOR_COUNT(cF_TOPFLR-1);
        }
        else{
            tmppulse1=FLOOR_COUNT(sRamDArry[mcurfloor]+1);
            tmppulse2=FLOOR_COUNT(sRamDArry[mcurfloor]);
        }
            
        newFloor=(tmppulse1-tmppulse2);
        newFloor=(unsigned long)(newFloor >> 1);
        newFloor=(unsigned long)(tmppulse2 + newFloor);

        if((unsigned long)CurPulse >= (unsigned long)newFloor){
            if(sRamDArry[mcurfloor] < cF_TOPFLR) sRamDArry[mcurfloor]++;
        }


        if(ReqFlr >= cF_TOPFLR){
            tmppulse1=FLOOR_COUNT(cF_TOPFLR);
        }
        else{
            tmppulse1=FLOOR_COUNT(ReqFlr);
        }
            
        tmppulse2=DecreasePulse;
        decrease_pulse=(tmppulse1-tmppulse2);

        if((unsigned long)CurPulse >= (unsigned long)decrease_pulse){

            if(bDac || (sRamDArry[mcurfloor] >= cF_TOPFLR) || (ReqFlr >= cF_TOPFLR)){   
                bUnd=1;
                CarLowSpeedCmd_IO();
                if(sRamDArry[mcurfloor] >= cF_TOPFLR){
						sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);
                }
            }
            else sRamDArry[mReqStopFloor]++;            
        }



        if(bUnd){
            CarLowSpeedCmd_IO();
        }			
        else{
            if(sRamDArry[mcurfloor] < cF_TOPFLR){
               	if(CurPulse > StopMinimumPulse) sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] | CAR_READY);
               	else                            sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] & ~CAR_READY);    
            }
			else	sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);                        
        }

		if(sRamDArry[mcurfloor] >= cF_TOPFLR){
			sRamDArry[mReqStopFloor] = (sRamDArry[mcurfloor] | CAR_READY);  
	    	sRamDArry[mAckStopFloor] = (sRamDArry[mcurfloor] | CAR_READY);
		}

    }

    else if(bCarDnMove){
        if(!IN_SD1){                              //modify ?
            if(!IN_EMG)	sRamDArry[mcurfloor]=0;
            bUnd=1;
            CarLowSpeedCmd_IO();
     	}


        if(!IN_SD2){
			if(CurMeterPerMin > (cF_SU2SD2_VELOCITY * 10)){
	            bUnd=1;
	   	        CarLowSpeedCmd_IO();
			}
		}


        if(DnLimitSpeedCheck() && (cF_X0X1_VELOCITY > cF_SU2SD2_VELOCITY)){  //x0
			if(CurMeterPerMin > (cF_X0X1_VELOCITY * 10)){
	            bUnd=1;
	   	        CarLowSpeedCmd_IO();
			}
		}

        if(sRamDArry[mcurfloor] == 0){
            tmppulse1=FLOOR_COUNT(1);
            tmppulse2=FLOOR_COUNT(0);
        }
        else{
            tmppulse1=FLOOR_COUNT(sRamDArry[mcurfloor]);
            tmppulse2=FLOOR_COUNT(sRamDArry[mcurfloor]-1);
        }
            
        newFloor=(tmppulse1-tmppulse2);
        newFloor=(unsigned long)(newFloor >> 1);
        newFloor=(unsigned long)(tmppulse2 + newFloor);

        if((unsigned long)CurPulse <= (unsigned long)newFloor){
            if(sRamDArry[mcurfloor] > 0 ) sRamDArry[mcurfloor]--;
        }


        if(ReqFlr == 0){
            tmppulse1=FLOOR_COUNT(0);
        }
        else{
            tmppulse1=FLOOR_COUNT(ReqFlr);
        }

        tmppulse2=DecreasePulse;
        decrease_pulse=(tmppulse1 + tmppulse2);


        if((unsigned long)CurPulse <= (unsigned long)decrease_pulse){
            if(bDac || (sRamDArry[mcurfloor] == 0) || (ReqFlr == 0)){   
                bUnd=1;
                CarLowSpeedCmd_IO();
                if(sRamDArry[mcurfloor] == 0){
	                    sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);
                }
            }
            else sRamDArry[mReqStopFloor]--;
        }


	        if(bUnd){
	            CarLowSpeedCmd_IO();
	        }			
	        else{
	            if(sRamDArry[mcurfloor] > 0){
	                if(CurPulse < StopMinimumPulse) sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] | CAR_READY);
	                else                            sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] & ~CAR_READY);    
	            }
	            else                                sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);                        
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
    
    return;
}





void  __attribute__((section(".usercode"))) DefaultEncoderRpmMpm(void)
{
#ifndef	TEST_SIMULATION

    unsigned int i;
    unsigned long rpm,mpm,encoder_pulse,mm_per_pulse,dec_mm_rpm,tmp_dec_mm_rpm,tmp_dec_pulse,tmp_value;

    for(i=0;i<32;i++){
        parameter_mirror[i]=GET_LONG(ENCODER_PULSE+i);
    }    

    parameter_mirror[MPM-ENCODER_PULSE]             = SET_ELEV_SPEED_PER_MIM; //m/min
    parameter_mirror[ENCODER_PULSE-ENCODER_PULSE]   = SET_ENCODER_PULSE;      //pulse/rotate
    parameter_mirror[RPM-ENCODER_PULSE]             = SET_MOTOR_RPM;          //r/min

    parameter_mirror[DEC_LENGTH_30 -ENCODER_PULSE]  = SPEED_30_DEC_LENGTH;
    parameter_mirror[DEC_LENGTH_45 -ENCODER_PULSE]  = SPEED_45_DEC_LENGTH;
    parameter_mirror[DEC_LENGTH_60 -ENCODER_PULSE]  = SPEED_60_DEC_LENGTH;
    parameter_mirror[DEC_LENGTH_90 -ENCODER_PULSE]  = SPEED_90_DEC_LENGTH;
    parameter_mirror[DEC_LENGTH_105-ENCODER_PULSE]  = SPEED_105_DEC_LENGTH;
    parameter_mirror[DEC_LENGTH_120-ENCODER_PULSE]  = SPEED_120_DEC_LENGTH;
    parameter_mirror[DEC_LENGTH_150-ENCODER_PULSE]  = SPEED_150_DEC_LENGTH;
    parameter_mirror[DEC_LENGTH_180-ENCODER_PULSE]  = SPEED_180_DEC_LENGTH;
    parameter_mirror[DEC_LENGTH_210-ENCODER_PULSE]  = SPEED_210_DEC_LENGTH;

    flash_write(ENCODER_PULSE);
    for(i=0;i<16;i++)   parameter_mirror[i]=parameter_mirror[i+16];
    flash_write(ENCODER_PULSE + 16);

#endif	
}       



#ifndef	TEST_SIMULATION
void  __attribute__((section(".usercode"))) CaluDecreasePulseCommon(unsigned int i)
{
    unsigned long rpm,mpm,encoder_pulse,mm_per_pulse,dec_mm_rpm,tmp_dec_mm_rpm,tmp_dec_pulse,tmp_value;

    mm_per_pulse=parameter_mirror[MM_PULSE-ENCODER_PULSE];


    dec_mm_rpm     =parameter_mirror[(DEC_LENGTH_30 + i) - ENCODER_PULSE];
    tmp_dec_mm_rpm =(dec_mm_rpm * 10000); 
    tmp_dec_pulse  = (tmp_dec_mm_rpm/mm_per_pulse);
    parameter_mirror[(DEC_PULSE_30 + i) - ENCODER_PULSE]=tmp_dec_pulse;

/*
    tmp_value      = (tmp_dec_pulse/2);
    tmp_dec_pulse  = (tmp_dec_pulse << 1);
    parameter_mirror[ (MIN_DEC_PULSE_30 + i) - ENCODER_PULSE]=tmp_dec_pulse;
*/

}
#endif




void  __attribute__((section(".usercode"))) CaluDecreasePulse(void)
{

#ifndef	TEST_SIMULATION

    unsigned int i;
    unsigned long rpm,mpm,encoder_pulse,mm_per_pulse,dec_mm_rpm,tmp_dec_mm_rpm,tmp_dec_pulse,tmp_value;

    for(i=0;i<32;i++){
        parameter_mirror[i]=GET_LONG(ENCODER_PULSE+i);
    }    


    //move_length/pulse = (MPM * 1000)/(RPM * PULSE_PER_ROTATE)

    mpm             = parameter_mirror[MPM-ENCODER_PULSE];
    mpm             = mpm * 1000 * 10000;                       //length=mm * 10000 ==1200000000
    rpm             = parameter_mirror[RPM-ENCODER_PULSE];
  
    encoder_pulse   = parameter_mirror[ENCODER_PULSE-ENCODER_PULSE];


    mm_per_pulse    = mpm / ( rpm *  encoder_pulse);  // 1200000000/1523712


   	mm_per_pulse    = (mm_per_pulse / (unsigned long)4);
    parameter_mirror[MM_PULSE-ENCODER_PULSE]     = mm_per_pulse;
    parameter_mirror[MPM_VARIABLE-ENCODER_PULSE] = (mm_per_pulse * (unsigned long)600);



///////////////////////////////////////
	CaluDecreasePulseCommon(0);    
	CaluDecreasePulseCommon(1);
	CaluDecreasePulseCommon(2);
	CaluDecreasePulseCommon(3);
	CaluDecreasePulseCommon(4);
	CaluDecreasePulseCommon(5);
	CaluDecreasePulseCommon(6);
	CaluDecreasePulseCommon(7);
	CaluDecreasePulseCommon(8);

	SaveVerify = 0x55;

    flash_write(ENCODER_PULSE);
    for(i=0;i<16;i++)   parameter_mirror[i]=parameter_mirror[i+16];
    flash_write(ENCODER_PULSE + 16);

	SaveVerify = 0x0;

#endif
}                                                                     



unsigned int    __attribute__((section(".usercode"))) Speed210Check(void)
{
    unsigned long MinPulse;

    if(cF_SPEED210 == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_210);
    if(MinPulse==0) 		return(1);

    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}


unsigned int    __attribute__((section(".usercode"))) Speed180Check(void)
{
    unsigned long MinPulse;

    if(cF_SPEED180 == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_180);
    if(MinPulse==0) 		return(1);

    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}


unsigned int    __attribute__((section(".usercode"))) Speed150Check(void)
{
    unsigned long MinPulse;

    if(cF_SPEED150 == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_150);
    if(MinPulse==0) 		return(1);

    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}


unsigned int    __attribute__((section(".usercode"))) Speed120Check(void)
{

    unsigned long MinPulse;

    if(cF_SPEED120 == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_120);
    if(MinPulse==0) 		return(1);

    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}


unsigned int    __attribute__((section(".usercode"))) Speed105Check(void)
{

    unsigned long MinPulse;

    if(cF_SPEED105 == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_105);
    if(MinPulse==0) 		return(1);

    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}



unsigned int    __attribute__((section(".usercode"))) Speed90Check(void)
{

    unsigned long MinPulse;

    if(cF_SPEED90 == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_90);
    if(MinPulse==0) 		return(1);

    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}


unsigned int    __attribute__((section(".usercode"))) Speed60Check(void)
{
    unsigned long MinPulse;

    if(cF_SPEED60 == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_60);

    if(MinPulse==0) 		return(1);

    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}


unsigned int    __attribute__((section(".usercode"))) Speed45Check(void)
{

    unsigned long MinPulse;

    if(cF_SPEED45 == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_45);

    if(MinPulse==0) 		return(1);

    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}

unsigned int    __attribute__((section(".usercode"))) Speed30Check(void)
{
    unsigned long MinPulse;

    if(cF_SPEED30 == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_30);
    if(MinPulse==0) 		return(1);

    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}






unsigned int  __attribute__((section(".usercode")))   SpeedChange(void)
{
    LocalType     ret;
    ret=1;

    switch(cF_ELEV_SPEED){
        case    SPEED_45:
            if(!Speed45Check() && (CurSpeed < SPEED_45)){
                CurSpeed=SPEED_45;
                DecreasePulse=GET_LONG(DEC_PULSE_45);
                ret=0;
            }
            break;     
        case    SPEED_60:
            if(!Speed60Check() && (CurSpeed < SPEED_60)){
                CurSpeed=SPEED_60;
                DecreasePulse=GET_LONG(DEC_PULSE_60);
                ret=0;
            }
            else if(!Speed45Check() && (CurSpeed < SPEED_45)){
                CurSpeed=SPEED_45;
                DecreasePulse=GET_LONG(DEC_PULSE_45);
                ret=0;
            }
            break;     
        case    SPEED_90:
            if(!Speed90Check() && (CurSpeed < SPEED_90)){
                CurSpeed=SPEED_90;
                DecreasePulse=GET_LONG(DEC_PULSE_90);
                ret=0;
            }
            else if(!Speed60Check() && (CurSpeed < SPEED_60)){
                CurSpeed=SPEED_60;
                DecreasePulse=GET_LONG(DEC_PULSE_60);
                ret=0;
            }
            break;     
        case    SPEED_105:
            if(!Speed105Check() && (CurSpeed < SPEED_105)){
                CurSpeed=SPEED_105;
                DecreasePulse=GET_LONG(DEC_PULSE_105);
                ret=0;
            }
            else if(!Speed90Check() && (CurSpeed < SPEED_90)){
                CurSpeed=SPEED_90;
                DecreasePulse=GET_LONG(DEC_PULSE_90);
                ret=0;
            }
            else if(!Speed60Check() && (CurSpeed < SPEED_60)){
                CurSpeed=SPEED_60;
                DecreasePulse=GET_LONG(DEC_PULSE_60);
                ret=0;
            }
            break;     
        case    SPEED_120:
            if(!Speed120Check() && (CurSpeed < SPEED_120)){
                CurSpeed=SPEED_120;
                DecreasePulse=GET_LONG(DEC_PULSE_120);
                ret=0;
            }
            else if(!Speed90Check() && (CurSpeed < SPEED_90)){
                CurSpeed=SPEED_90;
                DecreasePulse=GET_LONG(DEC_PULSE_90);
                ret=0;
            }
            else if(!Speed60Check() && (CurSpeed < SPEED_60)){
                CurSpeed=SPEED_60;
                DecreasePulse=GET_LONG(DEC_PULSE_60);
                ret=0;
            }
            break;     
        case    SPEED_150:
            if(!Speed150Check() && (CurSpeed < SPEED_150)){
                CurSpeed=SPEED_150;
                DecreasePulse=GET_LONG(DEC_PULSE_150);
                ret=0;
            }
            else if(!Speed120Check() && (CurSpeed < SPEED_120)){
                CurSpeed=SPEED_120;
                DecreasePulse=GET_LONG(DEC_PULSE_120);
                ret=0;
            }
            else if(!Speed90Check() && (CurSpeed < SPEED_90)){
                CurSpeed=SPEED_90;
                DecreasePulse=GET_LONG(DEC_PULSE_90);
                ret=0;
            }
            else if(!Speed60Check() && (CurSpeed < SPEED_60)){
                CurSpeed=SPEED_60;
                DecreasePulse=GET_LONG(DEC_PULSE_60);
                ret=0;
            }
            break;     
        case    SPEED_180:
            if(!Speed180Check() && (CurSpeed < SPEED_180)){
                CurSpeed=SPEED_180;
                DecreasePulse=GET_LONG(DEC_PULSE_180);
                ret=0;
            }
            else if(!Speed150Check() && (CurSpeed < SPEED_150)){
                CurSpeed=SPEED_150;
                DecreasePulse=GET_LONG(DEC_PULSE_150);
                ret=0;
            }
            else if(!Speed120Check() && (CurSpeed < SPEED_120)){
                CurSpeed=SPEED_120;
                DecreasePulse=GET_LONG(DEC_PULSE_120);
                ret=0;
            }
            else if(!Speed90Check() && (CurSpeed < SPEED_90)){
                CurSpeed=SPEED_90;
                DecreasePulse=GET_LONG(DEC_PULSE_90);
                ret=0;
            }
            else if(!Speed60Check() && (CurSpeed < SPEED_60)){
                CurSpeed=SPEED_60;
                DecreasePulse=GET_LONG(DEC_PULSE_60);
                ret=0;
            }
            break;     
        case    SPEED_210:
            break;     
        default:
            break;
    }        


    if(ret==0){
        if(bCarUpMove)  StopMinimumPulse=DecreasePulse+CurPulse;
        else            StopMinimumPulse=CurPulse-DecreasePulse;
    }

    return(ret);
}




unsigned int  __attribute__((section(".usercode")))   SpeedSet(void)
{
    unsigned long tmppulse1,tmppulse2;
    LocalType     j,want_updn,ret;


NoStart=0;

    if(INVERTER_CHECK == LG)    return(0);
    if(INVERTER_CHECK == D_F)   return(0);


    ret=0;
    want_updn=0;

    j=(sRamDArry[mAckStopFloor] & ONLY_FLR);
    
    TotalPulse=1;

    StartPulse=CurPulse;    

    tmppulse1=FLOOR_COUNT(j);
    tmppulse2=FLOOR_COUNT(sRamDArry[mcurfloor]);

    if(j>sRamDArry[mcurfloor]){
        TotalPulse=(tmppulse1-tmppulse2);   
        want_updn=1;            //up want
    }
    else if(j<sRamDArry[mcurfloor]){
        TotalPulse=(tmppulse2-tmppulse1);   
        want_updn=2;            //dn want
    }
    else    ret=5;    

    
    if(ret==0){
		ret=5;   //yousangyong

        switch(cF_ELEV_SPEED){
            case    SPEED_30:
                if(!Speed30Check()){
                    CurSpeed=SPEED_30;
                    DecreasePulse=GET_LONG(DEC_PULSE_30);
					ret=0;
                }    
				else	ret=4;

                if((cF_SPEED30 == 0))	ret=2;

                break;     
            case    SPEED_45:
                if(!Speed45Check()){
                    CurSpeed=SPEED_45;
                    DecreasePulse=GET_LONG(DEC_PULSE_45);
                    ret=0;
                }
                else if(!Speed30Check()){
                    CurSpeed=SPEED_30;
                    DecreasePulse=GET_LONG(DEC_PULSE_30);
                    ret=0;
                }
				else	ret=4;
				
               if((cF_SPEED30 == 0) && (cF_SPEED45 == 0))	ret=2;

                break;     
            case    SPEED_60:
                if(!Speed60Check()){
                    CurSpeed=SPEED_60;
                    DecreasePulse=GET_LONG(DEC_PULSE_60);
                    ret=0;
                }
                else if(!Speed45Check()){
                    CurSpeed=SPEED_45;
                    DecreasePulse=GET_LONG(DEC_PULSE_45);
                    ret=0;
                }
                else if(!Speed30Check()){
                    CurSpeed=SPEED_30;
                    DecreasePulse=GET_LONG(DEC_PULSE_30);
                    ret=0;
                }
				else	ret=4;

                if((cF_SPEED30 == 0) && (cF_SPEED45 == 0) && (cF_SPEED60 == 0))	ret=2;

                break;     
            case    SPEED_90:
                if(!Speed90Check()){
                    CurSpeed=SPEED_90;
                    DecreasePulse=GET_LONG(DEC_PULSE_90);
                    ret=0;
                }
                else if(!Speed60Check()){
                    CurSpeed=SPEED_60;
                    DecreasePulse=GET_LONG(DEC_PULSE_60);
                    ret=0;
                }
                else if(!Speed45Check()){
                    CurSpeed=SPEED_45;
                    DecreasePulse=GET_LONG(DEC_PULSE_45);
                    ret=0;
                }
 				else	ret=4;

                if((cF_SPEED45 == 0) && (cF_SPEED60 == 0) && (cF_SPEED90 == 0))	ret=2;

                break;     
            case    SPEED_105:
                if(!Speed105Check()){
                    CurSpeed=SPEED_105;
                    DecreasePulse=GET_LONG(DEC_PULSE_105);
                    ret=0;
                }
                else if(!Speed90Check()){
                    CurSpeed=SPEED_90;
                    DecreasePulse=GET_LONG(DEC_PULSE_90);
                    ret=0;
                }
                else if(!Speed60Check()){
                    CurSpeed=SPEED_60;
                    DecreasePulse=GET_LONG(DEC_PULSE_60);
                    ret=0;
                }
				else	ret=4;

                if((cF_SPEED60 == 0) && (cF_SPEED90 == 0) && (cF_SPEED105 == 0))	ret=2;
                break;     
            case    SPEED_120:
                if(!Speed120Check()){
                    CurSpeed=SPEED_120;
                    DecreasePulse=GET_LONG(DEC_PULSE_120);
                    ret=0;
                }
                else if(!Speed90Check()){
                    CurSpeed=SPEED_90;
                    DecreasePulse=GET_LONG(DEC_PULSE_90);
                    ret=0;
                }
                else if(!Speed60Check()){
                    CurSpeed=SPEED_60;
                    DecreasePulse=GET_LONG(DEC_PULSE_60);
                    ret=0;
                }
				else	ret=4;

                if((cF_SPEED60 == 0) && (cF_SPEED90 == 0) && (cF_SPEED120 == 0))	ret=2;
                break;     
            case    SPEED_150:
                if(!Speed150Check()){
                    CurSpeed=SPEED_150;
                    DecreasePulse=GET_LONG(DEC_PULSE_150);
                    ret=0;
                }
                else if(!Speed120Check()){
                    CurSpeed=SPEED_120;
                    DecreasePulse=GET_LONG(DEC_PULSE_120);
                    ret=0;
                }
                else if(!Speed90Check()){
                    CurSpeed=SPEED_90;
                    DecreasePulse=GET_LONG(DEC_PULSE_90);
                    ret=0;
                }
                else if(!Speed60Check()){
                    CurSpeed=SPEED_60;
                    DecreasePulse=GET_LONG(DEC_PULSE_60);
                    ret=0;
                }
				else	ret=4;

                if((cF_SPEED60 == 0) && (cF_SPEED90 == 0) && (cF_SPEED120 == 0) && (cF_SPEED150 == 0))	ret=2;
                break;     
            case    SPEED_180:
                if(!Speed180Check()){
                    CurSpeed=SPEED_180;
                    DecreasePulse=GET_LONG(DEC_PULSE_180);
                    ret=0;
                }
                else if(!Speed150Check()){
                    CurSpeed=SPEED_150;
                    DecreasePulse=GET_LONG(DEC_PULSE_150);
                    ret=0;
                }
                else if(!Speed120Check()){
                    CurSpeed=SPEED_120;
                    DecreasePulse=GET_LONG(DEC_PULSE_120);
                    ret=0;
                }
                else if(!Speed90Check()){
                    CurSpeed=SPEED_90;
                    DecreasePulse=GET_LONG(DEC_PULSE_90);
                    ret=0;
                }
                else if(!Speed60Check()){
                    CurSpeed=SPEED_60;
                    DecreasePulse=GET_LONG(DEC_PULSE_60);
                    ret=0;
                }
				else	ret=4;

                if((cF_SPEED60 == 0) && (cF_SPEED90 == 0) && (cF_SPEED120 == 0) && (cF_SPEED150 == 0)  && (cF_SPEED180 == 0))	ret=2;
        
                ret=3;  //// not use
    
                break;     
            case    SPEED_210:
                if(!Speed210Check()){
                    CurSpeed=SPEED_210;
                    DecreasePulse=GET_LONG(DEC_PULSE_210);
                    ret=0;
                }
                else if(!Speed180Check()){
                    CurSpeed=SPEED_180;
                    DecreasePulse=GET_LONG(DEC_PULSE_180);
                    ret=0;
                }
                else if(!Speed150Check()){
                    CurSpeed=SPEED_150;
                    DecreasePulse=GET_LONG(DEC_PULSE_150);
                    ret=0;
                }
                else if(!Speed120Check()){
                    CurSpeed=SPEED_120;
                    DecreasePulse=GET_LONG(DEC_PULSE_120);
                    ret=0;
                }
                else if(!Speed90Check()){
                    CurSpeed=SPEED_90;
                    DecreasePulse=GET_LONG(DEC_PULSE_90);
                    ret=0;
                }
                else if(!Speed60Check()){
                    CurSpeed=SPEED_60;
                    DecreasePulse=GET_LONG(DEC_PULSE_60);
                    ret=0;
                }

                if((cF_SPEED60 == 0) && (cF_SPEED90 == 0) && (cF_SPEED120 == 0) && (cF_SPEED150 == 0)  && (cF_SPEED180 == 0)   && (cF_SPEED210 == 0))	ret=2;

                ret=3;      //// not use
                break;     
            default:
                ret=1;
                break;
        }        
    }


    if(ret==0){
        if(want_updn==1){           //up want
            StopMinimumPulse=DecreasePulse + CurPulse;
            
            j=cF_TOPFLR;
            tmppulse1=FLOOR_COUNT(j);    
            if(StopMinimumPulse > (tmppulse1 - DecreasePulse)){
                ret=4;      //false
            }       
        }
        else if(want_updn==2){           //dn want
            StopMinimumPulse=CurPulse-DecreasePulse;

            tmppulse1=FLOOR_COUNT(0);    
            if(StopMinimumPulse < (tmppulse1 + DecreasePulse)){
                ret=4;      //false
            }       
        }
        else    ret=5;
    }
    

    if(ret){
        switch(ret){
            case    1:
			    if(sRamDArry[mSysStatus] > sSpeedSetError)  sRamDArry[mSysStatus]=sSpeedSetError;      
                break;
            case    2:
			    if(sRamDArry[mSysStatus] > sSpeedPortError) sRamDArry[mSysStatus]=sSpeedPortError;      
                break;
            case    3:
			    if(sRamDArry[mSysStatus] > sNoUseSpeed)     sRamDArry[mSysStatus]=sNoUseSpeed;      
                break;
            case    4:
			    if(sRamDArry[mSysStatus] > sMinLengthErr)   sRamDArry[mSysStatus]=sMinLengthErr;      
                break;
            case    5:
			    if(sRamDArry[mSysStatus] > sSystemErr)      sRamDArry[mSysStatus]=sSystemErr;      
                break;
        }

		NoStart=ret;
    }


    return(ret);

//ret==1    : SpeedSetError
//ret==2    : SpeedPortError
//ret==3    : No Use
//ret==4    : MinLengthErr
//ret==5    : SystemErr
}


