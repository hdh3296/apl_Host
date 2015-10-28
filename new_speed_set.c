


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



#ifdef	SPD3_USE	


void  __attribute__((section(".usercode")))    Mnanual_Speed_Sel(void)
{
       
    ManualSpeedCmd_IO();

    if( (S1_POWER_FAIL || bSearchHome) && (cF_RELEVELSPEED > 0) ){
 		RelevelSpeedCmd_IO();
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

    if(INVERTER_CHECK == LG)    CurSpeed=SPEED_HIGH;  ////////5xx
    if(INVERTER_CHECK == D_F)   CurSpeed=SPEED_HIGH;  ////////5xx

    switch(CurSpeed){
        case    SPEED_LOW:    
            CurSelSpeed(cF_SPEED_LOW_PORT);                        
            break;
        case    SPEED_MID:
            CurSelSpeed(cF_SPEED_MID_PORT);                        
            break;
        case    SPEED_HIGH:
            CurSelSpeed(cF_SPEED_HIGH_PORT);                        
            break;
        default:
            CurSelSpeed(0);                
        	CurSpeed=0;
            break;
    }
}



LocalType  __attribute__((section(".usercode")))    SpeedReSetting(unsigned int spd_sig)
{
	unsigned int spd_val,spd_change_sta;

	spd_change_sta=0;
	spd_val=NOT_USE_SPEED;

	if(bUnd)							return(0);	  
    if(!bAuto)							return(0);
    if( !bCarUpMove && !bCarDnMove)		return(0);


    if(bCarUpMove){
		if(spd_sig == 1){
			if( !IN_SU2){
				spd_val=cF_SU2SD2_VELOCITY;
				spd_change_sta=1; 
			}
		}
		else{
			if( !IN_X1){
				spd_val=cF_X0X1_VELOCITY; 
				spd_change_sta=1; 
			}
		}
    }
    else if(bCarDnMove){
		if(spd_sig == 1){
			if( !IN_SD2){
				spd_val=cF_SU2SD2_VELOCITY; 
				spd_change_sta=1; 
			}
		}
		else{
			if( !IN_X0){
				spd_val=cF_X0X1_VELOCITY; 
				spd_change_sta=1; 
			}
		}
    }



	if(spd_change_sta==0)				return(0);	
	if(spd_val >= SPEED_MAX)			return(0);
	if(spd_val == NOT_USE_SPEED)		return(0);
	if(spd_val >= CurSpeed)				return(0);
	



	switch(spd_val){
		case    CHANGE_DEC_AT_SPD_M_H:
			if(CurSpeed >= SPEED_MID){
				if(!bUnd)	OUT_DAC(1);	
				bUnd=1;
				
				CurSpeed=spd_val;
				bSetSpeedOn=1;                    
				CarLowSpeedCmd_IO();
			}
			break;
		case    CHANGE_DEC_AT_SPD_H:
			if(CurSpeed >= SPEED_HIGH){
				if(!bUnd)	OUT_DAC(1);	
				bUnd=1;
				
				CurSpeed=spd_val;
				bSetSpeedOn=1;                    
				CarLowSpeedCmd_IO();
			}
			break;
		case    SPEED_LOW:
			if(cF_SPEED_LOW_PORT > NOT_USE_SPEED){
				bSetSpeedOn=1;                    				
				CurSpeed=spd_val;

				if(BaseDecPulseX == 0){
					DecreasePulse=GET_LONG(DEC_PULSE_SPD_LOW);
				}

				if(bCarUpMove)  StopMinimumPulse=DecreasePulse+CurPulse;
				else            StopMinimumPulse=CurPulse-DecreasePulse;
				RunSpeedCmd_IO();			
			}
			break;
		case    SPEED_MID:
			if(cF_SPEED_MID_PORT > NOT_USE_SPEED){
				bSetSpeedOn=1;                    				
				CurSpeed=spd_val;

				if(BaseDecPulseX == 0){
					DecreasePulse=GET_LONG(DEC_PULSE_SPD_MID);
				}

				if(bCarUpMove)  StopMinimumPulse=DecreasePulse+CurPulse;
				else            StopMinimumPulse=CurPulse-DecreasePulse;
				RunSpeedCmd_IO();			
			}
			break;
		default:
			break;
	
	}

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


////////////////////////////////////	ForceSettingSpeed();


    ReqFlr=(LocalType)(sRamDArry[mReqStopFloor] & ONLY_FLR);     	


    if(sRamDArry[mcurfloor] > cF_TOPFLR){
        sRamDArry[mcurfloor]=0;
    }



    if(bCarUpMove){
        if(!IN_SU1){
            if(!IN_EMG)	sRamDArry[mcurfloor]=cF_TOPFLR;   //modify ?

            if(!bUnd)	OUT_DAC(1);	
            bUnd=1;
            CarLowSpeedCmd_IO();
        }


		SpeedReSetting(1);
		SpeedReSetting(2);

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
						#ifdef	FLOOR_64
						sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | DN_READY);
						#else
						sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);
						#endif
                }
            }
            else sRamDArry[mReqStopFloor]++;            
        }

/////////////////	
#ifdef	FLOOR_64
        if(bUnd){
            CarLowSpeedCmd_IO();
        }			
        else{
            if(sRamDArry[mcurfloor] < cF_TOPFLR){
				if(BaseDecPulseX > 0){
					sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] | UPDN_READY);
				}
				else{
	               	if(CurPulse > StopMinimumPulse) sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] | UPDN_READY);
	               	else                            sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] & ~UPDN_READY);    
				}
            }
        }

		if(sRamDArry[mcurfloor] >= cF_TOPFLR){
			sRamDArry[mReqStopFloor] = (sRamDArry[mcurfloor] | DN_READY);  
	    	sRamDArry[mAckStopFloor] = (sRamDArry[mcurfloor] | DN_READY);
		}
#else
        if(bUnd){
            CarLowSpeedCmd_IO();
        }			
        else{
            if(sRamDArry[mcurfloor] < cF_TOPFLR){
				if(BaseDecPulseX > 0){
					sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] | CAR_READY);
				}
				else{
	               	if(CurPulse > StopMinimumPulse) sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] | CAR_READY);
	               	else                            sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] & ~CAR_READY);    
				}
            }
			else	sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);                        
        }

		if(sRamDArry[mcurfloor] >= cF_TOPFLR){
			sRamDArry[mReqStopFloor] = (sRamDArry[mcurfloor] | CAR_READY);  
	    	sRamDArry[mAckStopFloor] = (sRamDArry[mcurfloor] | CAR_READY);
		}
#endif

    }

    else if(bCarDnMove){
        if(!IN_SD1){                              //modify ?
            if(!IN_EMG)	sRamDArry[mcurfloor]=0;

            if(!bUnd)	OUT_DAC(1);	
            bUnd=1;
            CarLowSpeedCmd_IO();
     	}

		SpeedReSetting(1);
		SpeedReSetting(2);



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
					#ifdef	FLOOR_64
                    sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | UP_READY);
					#else
                    sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);
					#endif
                }
            }
            else sRamDArry[mReqStopFloor]--;
        }


		#ifdef	FLOOR_64
	        if(bUnd){
	            CarLowSpeedCmd_IO();
	        }			
	        else{
	            if(sRamDArry[mcurfloor] > 0){
					if(BaseDecPulseX > 0){
						sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] | UPDN_READY);
					}
					else{
		                if(CurPulse < StopMinimumPulse) sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] | UPDN_READY);
		                else                            sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] & ~UPDN_READY);    
					}
	            }
	        }
	
			if(sRamDArry[mcurfloor] == 0){
				sRamDArry[mReqStopFloor] = (sRamDArry[mcurfloor] | UP_READY);  
		    	sRamDArry[mAckStopFloor] = (sRamDArry[mcurfloor] | UP_READY);				                      
			}
		#else
	        if(bUnd){
	            CarLowSpeedCmd_IO();
	        }			
	        else{
	            if(sRamDArry[mcurfloor] > 0){
					if(BaseDecPulseX > 0){
						sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] | CAR_READY);
					}
					else{
		                if(CurPulse < StopMinimumPulse) sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] | CAR_READY);
		                else                            sRamDArry[mReqStopFloor]  = (sRamDArry[mReqStopFloor] & ~CAR_READY);    
					}
	            }
	            else                                sRamDArry[mReqStopFloor]  = (sRamDArry[mcurfloor] | CAR_READY);                        
	        }
	
			if(sRamDArry[mcurfloor] == 0){
				sRamDArry[mReqStopFloor] = (sRamDArry[mcurfloor] | CAR_READY);  
		    	sRamDArry[mAckStopFloor] = (sRamDArry[mcurfloor] | CAR_READY);				                      
			}
		#endif
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

    parameter_mirror[DEC_LENGTH_SPD_LOW -ENCODER_PULSE]  	= DEFAULT_SPEED1_DEC_LENGTH;
    parameter_mirror[DEC_LENGTH_SPD_MID -ENCODER_PULSE]  	= DEFAULT_SPEED2_DEC_LENGTH;
    parameter_mirror[DEC_LENGTH_SPD_HIGH -ENCODER_PULSE]	= DEFAULT_SPEED3_DEC_LENGTH;

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


    dec_mm_rpm     =parameter_mirror[(DEC_LENGTH_SPD_LOW + i) - ENCODER_PULSE];
    tmp_dec_mm_rpm =(dec_mm_rpm * 10000); 
    tmp_dec_pulse  = (tmp_dec_mm_rpm/mm_per_pulse);
    parameter_mirror[(DEC_PULSE_SPD_LOW + i) - ENCODER_PULSE]=tmp_dec_pulse;


/*
    tmp_value      = (tmp_dec_pulse/2);
    tmp_dec_pulse  = (tmp_dec_pulse << 1);
    parameter_mirror[ (MIN_DEC_PULSE_SPD_LOW + i) - ENCODER_PULSE]=tmp_dec_pulse;
*/

}	



unsigned int  __attribute__((section(".usercode"))) CaluDecreasePulseCommon_Auto(void)
{
    unsigned long tmp_mpm,tmp_dec_time,tmp_mm,tmp_var;

    tmp_mpm         = parameter_mirror[MPM-ENCODER_PULSE];
    tmp_dec_time    = parameter_mirror[BASE_DEC_TIME-ENCODER_PULSE];

	if(tmp_dec_time==0){
    	parameter_mirror[BASE_DEC_LENGTH - ENCODER_PULSE]=0;
		parameter_mirror[BASE_DEC_TIME-ENCODER_PULSE]=0;
		return(0);
	}

	if(tmp_dec_time > 5000){
    	parameter_mirror[BASE_DEC_LENGTH - ENCODER_PULSE]=0;
		parameter_mirror[BASE_DEC_TIME-ENCODER_PULSE]=0;
		return(0);
	}

	tmp_var=10;
	tmp_mpm=(tmp_mpm * tmp_var);						// (m/m) * 10

	tmp_var=60;
	tmp_mpm=(tmp_mpm / tmp_var);						//  m/sec
	tmp_mm=(tmp_mpm * tmp_dec_time)/2; 			//  

	tmp_var=10;
	tmp_mm=(tmp_mm/tmp_var); 							//  base decrease mm  

    parameter_mirror[BASE_DEC_LENGTH - ENCODER_PULSE]=tmp_mm;
	

// if mpm= 60
// if dec_time= 1800 = 1.8sce
// move lenth  60mpm = (60m/m)/60 = 1m/sec
// dec_length= ((60(m/m)/60sec) * 1.8sec)/ 2 = 0.9m=900mm
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


	CaluDecreasePulseCommon_Auto();

	CaluDecreasePulseCommon(0);    
	CaluDecreasePulseCommon(1);
	CaluDecreasePulseCommon(2);
	CaluDecreasePulseCommon(3);


	SaveVerify = 0x55;

    flash_write(ENCODER_PULSE);
    for(i=0;i<16;i++)   parameter_mirror[i]=parameter_mirror[i+16];
    flash_write(ENCODER_PULSE + 16);

	SaveVerify = 0x0;

	BaseDecPulseX=GET_LONG(BASE_DEC_PULSE);

#endif
}                                                                     








unsigned int    __attribute__((section(".usercode"))) SpeedHighCheck(void)
{
    unsigned long MinPulse;

    if(cF_SPEED_HIGH_PORT == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_SPD_HIGH);
    MinPulse= (MinPulse * 2);

    if(MinPulse==0) 		return(1);


    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}


unsigned int    __attribute__((section(".usercode"))) SpeedMidCheck(void)
{

    unsigned long MinPulse;

    if(cF_SPEED_MID_PORT == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_SPD_MID);
    MinPulse= (MinPulse * 2);

    if(MinPulse==0) 		return(1);

    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}

unsigned int    __attribute__((section(".usercode"))) SpeedLowCheck(void)
{
    unsigned long MinPulse;

    if(cF_SPEED_LOW_PORT == 0)    return(2);     

    MinPulse=GET_LONG(DEC_PULSE_SPD_LOW);
    MinPulse= (MinPulse * 2);


    if(MinPulse==0) 		return(1);

    if(TotalPulse>MinPulse) return(0);
    else                    return(1);
}






unsigned int  __attribute__((section(".usercode")))   SpeedChange(void)
{
	unsigned long	tmppulse1;	
    LocalType     ret;
    ret=1;

////////////////SPD3_USE
	if(BaseDecPulseX > 0){
		return(1);
	}
//////////////


	if(!SpeedHighCheck() && (CurSpeed < SPEED_HIGH)){   ////5xx
	    CurSpeed=SPEED_HIGH;
	    DecreasePulse=GET_LONG(DEC_PULSE_SPD_HIGH);
	    ret=0;
	}
	else if(!SpeedMidCheck() && (CurSpeed < SPEED_MID)){
	    CurSpeed=SPEED_MID;
	    DecreasePulse=GET_LONG(DEC_PULSE_SPD_MID);
	    ret=0;
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

	if((cF_SPEED_LOW_PORT == 0) && (cF_SPEED_MID_PORT == 0) && (cF_SPEED_HIGH_PORT == 0)){
		ret=2;
		NoStart=ret;

		if(sRamDArry[mSysStatus] > sSpeedPortError) sRamDArry[mSysStatus]=sSpeedPortError;      

		return(ret);
	}


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


////////////////SPD3_USE
	if(ret==0){	
		if(BaseDecPulseX > 0){
		    CurSpeed=SPEED_HIGH;
		    DecreasePulse=BaseDecPulseX;
			if(TotalPulse <= DecreasePulse){
				if(cF_SPEED_MID_PORT > 0){
		    		CurSpeed=SPEED_MID;
				}
				else if(cF_SPEED_LOW_PORT > 0){
		    		CurSpeed=SPEED_LOW;
				}
			} 			

	        if(want_updn==1){           //up want
	            StopMinimumPulse=DecreasePulse + CurPulse;
			}
	        else if(want_updn==2){           //dn want
	            StopMinimumPulse=CurPulse-DecreasePulse;
			}
			return(0);
		}
	}
//////////////


    if(ret==0){
		ret=5;   //yousangyong
		if(!SpeedHighCheck()){
		    CurSpeed=SPEED_HIGH;
		    DecreasePulse=GET_LONG(DEC_PULSE_SPD_HIGH);
		    ret=0;
		}
		else if(!SpeedMidCheck()){
		    CurSpeed=SPEED_MID;
		    DecreasePulse=GET_LONG(DEC_PULSE_SPD_MID);
		    ret=0;
		}
		else if(!SpeedLowCheck()){
		    CurSpeed=SPEED_LOW;
		    DecreasePulse=GET_LONG(DEC_PULSE_SPD_LOW);
		    ret=0;
		}
		else	ret=4;		
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

/********************
//ret==1    : SpeedSetError
//ret==2    : SpeedPortError
//ret==3    : No Use
//ret==4    : MinLengthErr
//ret==5    : SystemErr
********************/
}






/******************************
* 최고속도 = 60m/m = 1m/sec		
* RPM      = 1460 
* encoder  = 1024

m/pulse= 60 / (1460 * (1024 * 4)) = 0.00001 m/pulse   		

감속 Pulse = (감속시간) * (( 최고속도 - 목표속도 )/2) * ( 1 / (m/pulse))

* 감속시간(T) = (설정감속시간(t1)) + ((S3 + s4)/2)     
	ex: t1= 1.6,  s3=1 ,s4=1  인 경우
	 T = 1.6 + (1 + 1)/2 = 2.6sec


1/0.00001 = 100000
감속 pulse = 2.6 * 1/2 * 100000= 130,000 
*******************************/



/*

unsigned long  __attribute__((section(".usercode"))) BaseSpeedCacu(unsigned int var)
{
	unsigned long	vPeak,alpha,v0,v1; 
	unsigned long	s1,s2,s3; 
	unsigned long	tmpv1,tmpv2; 

	if(var == 1)			vPeak=(unsigned long)cF_SPEED105;
	else if(var == 2)		vPeak=(unsigned long)cF_SPEED120;
	else if(var == 3)		vPeak=(unsigned long)cF_SPEED150;
	else if(var == 4)		vPeak=(unsigned long)cF_SPEED180;

	if(vPeak == 0)	return(0);

	if(cF_SPEED210 == 0)	return(0);
	if(cF_ETC1SPEED == 0)	return(0);
	if(cF_ETC2SPEED == 0)	return(0);

	
	alpha=(unsigned long)(vPeak / cF_SPEED210);   // c1-02		// 625


	vPeak=(unsigned long)(vPeak * 10000);						
	vPeak=(unsigned long)(vPeak / 60);		// speed/sec		// 10000

	alpha=(unsigned long)(vPeak / cF_SPEED210);   // c1-02		// 625
 
	tmpv1=(unsigned long)((alpha * cF_ETC1SPEED)/2); //c2-03		
	v0=(unsigned long)(vPeak - tmpv1); 							// 6875

	v1=(unsigned long)((alpha * cF_ETC2SPEED)/2);  // c2-04		// 4687


/////s1//////////////
	tmpv2=(unsigned long)(cF_ETC1SPEED);
	tmpv1= (unsigned long)( vPeak *  tmpv2);
	s1=tmpv1;

	tmpv2=(unsigned long)(tmpv2 * tmpv2);	
	tmpv1= (unsigned long)( (alpha *  tmpv2)/6 );

	s1=(s1-tmpv1);
/////////////////

/////s2//////////////
	tmpv1= (unsigned long)( (v0-v1));
	tmpv1= (unsigned long)( tmpv1 * 1000);
	tmpv1= (unsigned long)( tmpv1/vPeak);
	tmpv1= (unsigned long)(tmpv1 * (v0 + v1) * (unsigned long)(cF_SPEED210) );
	s2= (unsigned long)(tmpv1 / 2);
	s2= (unsigned long)(s2 / 1000);
/////////////////


/////s3//////////////
	tmpv1= (unsigned long)( cF_ETC2SPEED *  cF_ETC2SPEED);
	tmpv1= (unsigned long)( tmpv1 *  alpha);
	s3= (unsigned long)( tmpv1 /  6);
/////////////////

	s1=s1+s2+s3;

	s1=(s1/100);
	
	return(s1);
}
*/


#endif

