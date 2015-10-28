#include <p30fxxxx.h>
//#include <p33exxxx.h>

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


#ifdef	FLOOR_64


void __attribute__((section(".usercode"))) IO_INVERTER_NewFloorLoadCheck(void)
{
    unsigned long longtemp1;

    LocalType     save_CurSpeed;


    LocalType     i,j,k,l;

	i=0;
	l=0;	
    TotalPulse=0;

//    j=(sRamDArry[mHighFloor] & UPDN_CAR_READY);
//    k=(sRamDArry[mLowFloor] & UPDN_CAR_READY);

    if(bCarUpMove && (sRamDArry[mHighFloor]==0)){    		
		NoButtonAndDefaultLoad();
	}
    else if(bCarDnMove && (sRamDArry[mLowFloor]==0)){    
		NoButtonAndDefaultLoad();
	}

   	else if((sRamDArry[mAckStopFloor] == 0)){
		NoButtonAndDefaultLoad();	
	}
	else{   
       	if(bCarUpMove && (sRamDArry[mHighFloor]==0) && ((sRamDArry[mHighFloor] & ONLY_FLR) <= cF_TOPFLR)){    
            j=(sRamDArry[mHighFloor] & ONLY_FLR);
			if(INVERTER_CHECK == D_F){
				if(j > sRamDArry[mcurfloor])	i=1;
			}
			else{
	            longtemp1=FLOOR_COUNT(j);
	            if((longtemp1 > CurPulse) && (CurPulse > StopMinimumPulse)){
	                TotalPulse=(longtemp1-CurPulse);
	                if(TotalPulse > DecreasePulse) 	i=1;
	            }
			}
        }
       	else if(bCarDnMove && (sRamDArry[mLowFloor] ==0)){
    		k=(sRamDArry[mLowFloor] & ONLY_FLR);
			if(INVERTER_CHECK == D_F){
				if(k < sRamDArry[mcurfloor])	i=1;
			}
			else{
	            longtemp1=FLOOR_COUNT(k);
	            if((longtemp1 < CurPulse) && (CurPulse < StopMinimumPulse)){
	                TotalPulse=(CurPulse-longtemp1);   
	                if(TotalPulse>DecreasePulse) i=1;
	            }
			}
        }

	    if(i==1){
	        save_CurSpeed=CurSpeed;

	        if(bSetSpeedOn==0)  SpeedChange();        
	
	        if(CurSpeed>save_CurSpeed){        
	            RunSpeedCmd_IO();
	        }
	            
	       	if(bCarUpMove){
	            sRamDArry[DEST_FLR]=sRamDArry[mAckStopFloor] = sRamDArry[mHighFloor];            																							
				OriginalHighFlr     =sRamDArry[mHighFloor];
	        }
	        else if(bCarDnMove){
	            sRamDArry[DEST_FLR]=sRamDArry[mAckStopFloor] = sRamDArry[mLowFloor];																							
        		OriginalLowFlr      =sRamDArry[mLowFloor];
	        }	 
	    }
		else{
	        sRamDArry[mHighFloor]=OriginalHighFlr;
	        sRamDArry[mLowFloor] =OriginalLowFlr;
		}
    }
}            		



void __attribute__((section(".usercode"))) LG_INVERTER_NewFloorLoadCheck(void)
{
    LocalType     j,k;

 //   j=(sRamDArry[mHighFloor] & UPDN_CAR_READY);
//   k=(sRamDArry[mLowFloor] & UPDN_CAR_READY);

    if(bCarUpMove && (sRamDArry[mHighFloor]==0)){    		
		NoButtonAndDefaultLoad();
	}
    else if(bCarDnMove && (sRamDArry[mLowFloor]==0)){    
		NoButtonAndDefaultLoad();
	}
   	else if(sRamDArry[mAckStopFloor] == 0){
		NoButtonAndDefaultLoad();	
	}

	else{
	  	if(bCarUpMove){
	        sRamDArry[DEST_FLR]=sRamDArry[mAckStopFloor] = sRamDArry[mHighFloor];            																							
	    }
	    else if(bCarDnMove){
	        sRamDArry[DEST_FLR]=sRamDArry[mAckStopFloor] = sRamDArry[mLowFloor];																							
	    }	
	}
}            		



unsigned int  __attribute__((section(".usercode"))) NewUpKeyChk(void)
{
    if(sRamDArry[mHighFloor]==0){
		if( (sRamDArry[mMostLongDst] & ONLY_FLR) > (sRamDArry[mReqStopFloor]  & ONLY_FLR)){
			sRamDArry[mHighFloor]= sRamDArry[mMostLongDst];
		} 
    }
     
	return(0);
}


unsigned int  __attribute__((section(".usercode"))) NewDnKeyChk(void)
{
    if(sRamDArry[mLowFloor] == 0){
		if( (sRamDArry[mMostLongDst] & ONLY_FLR) < (sRamDArry[mReqStopFloor]  & ONLY_FLR)){
			if(sRamDArry[mMostLongDst] > 0 )		sRamDArry[mLowFloor]= sRamDArry[mMostLongDst];
		} 
    } 

    return(0);
}



void   __attribute__((section(".usercode"))) NewFloorLoad(void)
{  	
   	if(bCarUpMove){
   		if(sRamDArry[DEST_FLR] != sRamDArry[mHighFloor]){
			if(!NewUpKeyChk()){
				if(INVERTER_CHECK == LG)    LG_INVERTER_NewFloorLoadCheck();
				else                        IO_INVERTER_NewFloorLoadCheck();
			}				
   		}
   		if(sRamDArry[mSysStatus] > sUP)   sRamDArry[mSysStatus]=sUP;      
   	}
   	else if(bCarDnMove){
   		if(sRamDArry[DEST_FLR] != sRamDArry[mLowFloor]){
			if(!NewDnKeyChk()){
				if(INVERTER_CHECK == LG)    LG_INVERTER_NewFloorLoadCheck();
				else                        IO_INVERTER_NewFloorLoadCheck();
			}	
   		}
   		if(sRamDArry[mSysStatus] > sDN)   sRamDArry[mSysStatus]=sDN;                  
   	}                        

	if(sRamDArry[mAckStopFloor]==0){
		if(sRamDArry[mReqStopFloor] & UPDN_READY){	
			sRamDArry[mAckStopFloor] = sRamDArry[mReqStopFloor];		
		}
	}
}




UserDataType  __attribute__((section(".usercode"))) CallMeUpDnDirectionSet(void)
{			

    if(bUpDnSet)    return(0);

        
    if((sRamDArry[mHighFloor]    & ONLY_FLR) <= (sRamDArry[DEST_FLR] & ONLY_FLR))  sRamDArry[mHighFloor]=0x0;
    if((sRamDArry[mLowFloor]     & ONLY_FLR) >= (sRamDArry[DEST_FLR] & ONLY_FLR))  sRamDArry[mLowFloor]=0x0;


   	if(bUpWard){
      	if(sRamDArry[mcurfloor] == cF_TOPFLR){
			TopDirectionSet();
		}      		
      	else if((sRamDArry[mAckStopFloor] & ONLY_FLR) == cF_TOPFLR){
			TopDirectionSet();
		}      		
      	else if(sRamDArry[mAckStopFloor] & UP_READY){
            UpWard();      	
      	} 
      	else if( (sRamDArry[mHighFloor] & ONLY_FLR) > (sRamDArry[mAckStopFloor] & ONLY_FLR) ){ 
            UpWard();
      	}
      	else if( (sRamDArry[mMostLongDst] & ONLY_FLR) > (sRamDArry[mAckStopFloor] & ONLY_FLR) ){ 
            UpWard();
      	} 
      	else if(sRamDArry[mAckStopFloor] & DN_READY){
            DnWard();
			bCarUpDnChButClr=1;
      	}        
      	else{
            ClrUpDnWard();
			bCarUpDnChButClr=1;
        }


/*
      	else if(sRamDArry[mAckStopFloor]){
	        if(!NextUpDnKeyChk(1)){
	            UpWard();
	            sRamDArry[mAckStopFloor] = (sRamDArry[mAckStopFloor] | UP_READY);
	        }


	      	else if( (sRamDArry[mMostLongDst] & ONLY_FLR) > (sRamDArry[DEST_FLR] & ONLY_FLR) ){ 
	            UpWard();
	            sRamDArry[mAckStopFloor] = (sRamDArry[mAckStopFloor] | UP_READY);
	      	} 
	      	else if(sRamDArry[mHighFloor] > 0){
	            UpWard();
	      	}


	      	else if(sRamDArry[mAckStopFloor] & DN_READY){
	            DnWard();
				bCarUpDnChButClr=1;
	      	}        
	      	else{
	            ClrUpDnWard();
				bCarUpDnChButClr=1;
	        } 
      	} 
      	else if(sRamDArry[mAckStopFloor] & DN_READY){
            DnWard();
			bCarUpDnChButClr=1;
      	}        
        else if(!NextUpDnKeyChk(1)){
            UpWard();
            sRamDArry[mAckStopFloor] = (sRamDArry[mAckStopFloor] | UP_READY);
        }

      	else if(sRamDArry[mDoor] & UPDN_CAR_READY){
            UpWard();
            sRamDArry[mAckStopFloor] = (sRamDArry[mAckStopFloor] | UP_READY);
      	}
      	else if( (sRamDArry[mMostLongDst] & ONLY_FLR) > (sRamDArry[DEST_FLR] & ONLY_FLR) ){ 
            UpWard();
            sRamDArry[mAckStopFloor] = (sRamDArry[mAckStopFloor] | UP_READY);
      	} 
      	else if(sRamDArry[mHighFloor] > 0){
            UpWard();
      	}
      	else if(sRamDArry[mLowFloor] > 0){
			bCarUpDnChButClr=1;
         	DnWard();
      	}

      	else{
            ClrUpDnWard();
			bCarUpDnChButClr=1;
        }
*/
   	}

   	else if(bDnWard){
      	if(sRamDArry[mcurfloor] == 0){
			LowDirectionSet();
		}
      	else if((sRamDArry[mAckStopFloor] & ONLY_FLR)== 0){
			LowDirectionSet();
		}      		
      	else if(sRamDArry[mAckStopFloor] & DN_READY){
            DnWard();      	
      	}
      	else if( ((sRamDArry[mLowFloor] & ONLY_FLR) < (sRamDArry[mAckStopFloor] & ONLY_FLR)) && (sRamDArry[mLowFloor]>0)){ 
			DnWard();
      	}
      	else if( ((sRamDArry[mMostLongDst] & ONLY_FLR) < (sRamDArry[mAckStopFloor] & ONLY_FLR)) && (sRamDArry[mMostLongDst] > 0) ){ 
			DnWard();
      	}
      	else if(sRamDArry[mAckStopFloor] & UP_READY){
            UpWard();
			bCarUpDnChButClr=1;
      	}        
      	else{
            ClrUpDnWard();
			bCarUpDnChButClr=1;
        }


/*
      	else if(sRamDArry[mAckStopFloor]){
	        if(!NextUpDnKeyChk(0)){
	            DnWard();
	            sRamDArry[mAckStopFloor] = (sRamDArry[mAckStopFloor] | DN_READY);
	      	} 

	      	else if( (sRamDArry[mMostLongDst] & UPDN_CAR_READY) && ((sRamDArry[mMostLongDst] & ONLY_FLR) < (sRamDArry[DEST_FLR] & ONLY_FLR)) ){ 
	            DnWard();
	            sRamDArry[mAckStopFloor] = (sRamDArry[mAckStopFloor] | DN_READY);
	      	} 
	      	else if(sRamDArry[mLowFloor]>0){
	            DnWard();
	      	}

	      	else if(sRamDArry[mAckStopFloor] & UP_READY){
	            UpWard();      	
	   			bCarUpDnChButClr=1;
	      	} 
	      	else{ 
	            ClrUpDnWard();
				bCarUpDnChButClr=1;
	      	}
      	} 
      	else if(sRamDArry[mAckStopFloor] & UP_READY){
            UpWard();      	
   			bCarUpDnChButClr=1;
      	}

      	else if(sRamDArry[mLowFloor] >0){
            DnWard();
      	}

        else if(!NextUpDnKeyChk(0)){
            DnWard();
            sRamDArry[mAckStopFloor] = (sRamDArry[mAckStopFloor] | DN_READY);
      	} 

      	else if(sRamDArry[mDoor] & UPDN_CAR_READY){
            DnWard();
            sRamDArry[mAckStopFloor] = (sRamDArry[mAckStopFloor] | DN_READY);
      	}      	
      	else if( (sRamDArry[mMostLongDst] & ONLY_FLR) < (sRamDArry[DEST_FLR] & ONLY_FLR) ){ 
            DnWard();
            sRamDArry[mAckStopFloor] = (sRamDArry[mAckStopFloor] | DN_READY);
      	} 

      	else if(sRamDArry[mHighFloor] & UPDN_READY){ 
			bCarUpDnChButClr=1;
         	UpWard();
      	}
      	else{ 
            ClrUpDnWard();
			bCarUpDnChButClr=1;
      	}
*/
   	}
  	else{
		if(sRamDArry[mEqualFloor] & UP_READY){
			UpWard();
      	} 
      	else if(sRamDArry[mEqualFloor] & DN_READY){
			DnWard();
      	}      
      	else if( (sRamDArry[mHighFloor] & ONLY_FLR) >= (sRamDArry[mcurfloor]) ){ 
            UpWard();
      	}
      	else if( (sRamDArry[mLowFloor] & ONLY_FLR) <= (sRamDArry[mcurfloor]) ){ 
            DnWard();
      	}

        bCarUpDnChButClr=1;	
   	}

	S3_SHIFT1=0; //shift   
    bUpDnSet=1;


    return(0);
}



void __attribute__((section(".usercode")))  HighLowCallClear(void)
{
    if(bUpWard){
		sRamDArry[mLowFloor] = 0x0;	

    	if(((sRamDArry[mHighFloor] & ONLY_FLR) >= (sRamDArry[mMostLongDst] & ONLY_FLR))){
            sRamDArry[mMostLongDst]=0x0;
        }


    	if(bUnd && bDac){
			if((sRamDArry[mHighFloor] & ONLY_FLR) <= sRamDArry[mcurfloor]){
				sRamDArry[mHighFloor] = 0x0;	
			}				
		}



    	if( (sRamDArry[mMostLongDst] & ONLY_FLR) > (sRamDArry[mHighFloor] & ONLY_FLR)){
			if(sRamDArry[mHighFloor] & DN_READY){
				sRamDArry[mHighFloor] = (sRamDArry[mHighFloor] & ~DN_READY); 
			}
        }


/*
    	if(((sRamDArry[mHighFloor] & ONLY_FLR) < (sRamDArry[mReqStopFloor] & ONLY_FLR))){
            sRamDArry[mHighFloor]=0x0;
        }

    	if(((sRamDArry[mMostLongDst] & ONLY_FLR) < (sRamDArry[mReqStopFloor] & ONLY_FLR))){
            sRamDArry[mMostLongDst]=0x0;
        }
*/
    }	
    else if(bDnWard){
		sRamDArry[mHighFloor] = 0x0;	

    	if((sRamDArry[mLowFloor] & ONLY_FLR) <= (sRamDArry[mMostLongDst] & ONLY_FLR)){
            sRamDArry[mMostLongDst]=0x0;
        }

    	if(bUnd && bDac){
			if((sRamDArry[mLowFloor] & ONLY_FLR) >= sRamDArry[mcurfloor]){
				sRamDArry[mLowFloor] = 0x0;	
			}				
		}

    	if( ((sRamDArry[mMostLongDst] & ONLY_FLR) < (sRamDArry[mLowFloor] & ONLY_FLR)) && (sRamDArry[mMostLongDst] >0 ) ){
			if(sRamDArry[mLowFloor] & UP_READY){
				sRamDArry[mLowFloor] = (sRamDArry[mLowFloor] & ~UP_READY); 
			}
        }


/*
    	if((sRamDArry[mLowFloor] & ONLY_FLR) > (sRamDArry[mReqStopFloor] & ONLY_FLR)){
            sRamDArry[mLowFloor]=0x0;
        }

    	if(((sRamDArry[mMostLongDst] & ONLY_FLR) > (sRamDArry[mReqStopFloor] & ONLY_FLR))){
            sRamDArry[mMostLongDst]=0x0;
        }
*/
    }
	else{
    	sRamDArry[mMostLongDst]=0x0;
	}


	if((sRamDArry[mEqualFloor] & ONLY_FLR) != sRamDArry[mcurfloor]){
		sRamDArry[mEqualFloor] = 0x0;	
	}				


    if((sRamDArry[mHighFloor] & ONLY_FLR) > cF_TOPFLR)	sRamDArry[mHighFloor]=0;
     
    if( ((sRamDArry[mLowFloor] & ONLY_FLR) == 0) && (sRamDArry[mLowFloor] & DN_READY) ){
        sRamDArry[mLowFloor]=(sRamDArry[mLowFloor] & ~DN_READY);
	}

    if((sRamDArry[mHighFloor] & ONLY_FLR) >= cF_TOPFLR){
        sRamDArry[mHighFloor]=(sRamDArry[mHighFloor] & ~UP_READY);
    }	

}


void  __attribute__((section(".usercode"))) UpDnCallClear(void)
{
/*
	if(bDnWard)	sRamDArry[mHighFloor]=0x0;	
	if(bUpWard)	sRamDArry[mLowFloor] =0x0;	


	if((sRamDArry[mHighFloor] & ONLY_FLR) <= sRamDArry[mcurfloor]){
		sRamDArry[mHighFloor]=0x0;	
	}						
					
	if((sRamDArry[mLowFloor] & ONLY_FLR) >= sRamDArry[mcurfloor]){
		sRamDArry[mLowFloor] = 0x0;	
	}				


    if((sRamDArry[mHighFloor] & ONLY_FLR) > cF_TOPFLR)	sRamDArry[mHighFloor]=0;

    if((sRamDArry[mLowFloor] & ONLY_FLR) == 0){
        sRamDArry[mLowFloor]=(sRamDArry[mLowFloor] & ~DN_READY);
	}
     
    if((sRamDArry[mHighFloor] & ONLY_FLR) >= cF_TOPFLR){
        sRamDArry[mHighFloor]=(sRamDArry[mHighFloor] & ~UP_READY);
    }
*/	
}


void __attribute__((section(".usercode")))  UpDnDirectionSet(void)
{
    if(!bUpWard && !bDnWard){ 
		if(sRamDArry[mEqualFloor] & UP_READY){
			UpWard();
      	} 
      	else if(sRamDArry[mEqualFloor] & DN_READY){
			DnWard();
      	} 

		else if(sRamDArry[mHighFloor] > 0 ){
			UpWard();
     	} 
     	else if(sRamDArry[mLowFloor] > 0 ){
  			DnWard();
     	}   	

   	}   	
}

#endif




