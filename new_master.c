

/*
//수정사항
1)timer : 2004.12.19  23.15.19
2).current status:  auto/manual/remote  ,cds/timer, cds input
3).volt/current   decima digit
4).display  return value
5)등갰수

*/

#include <p30fxxxx.h>


#include  "apl_memory_map.h"
#include  "30f6010_io.h"
#include  "host_io.h"
#include  "iodef.h"
#include  "door.h"
#include  "com.h"
#include  "counter.h"
#include  "you_can1.h"
#include  "you_can2.h"
#include "you_can_lib.h"
#include  "lader.h"
#include  "default_setup.h"


typedef unsigned int	bool;


extern	UserDataType    X0Time;
extern	UserDataType    X1Time;
extern	UserDataType    X2Time;
extern	UserDataType    X3Time;
extern	UserDataType    X4Time;
extern	UserDataType    X5Time;
extern	UserDataType    X6Time;
extern	UserDataType    X7Time;



//UserDataType    sRamDArry[mOut1Status]=0;



//unsigned int	VirtualOut1,VirtualOut2;





unsigned int	__attribute__((section(".usercode"))) OtherWork(void)
{
    unsigned int  i=0,j=0;

	if(Can2RxLocalAddr != 0)		return(0);
 	if(Can2RxGroupAddr != 0)		return(0);


	Can2SaveReceiveData();
	j=(unsigned int)((Can2RxEid>>8) & 0x0f);
	j=(j * 8);


	for(i=0;i<Can2RxDlc;i++){
		TmpCan2RcvBuf[j+i]=CAN2_Buf[i];
	}

	if(DATA_BLOCK_END  & Can2RxEid){
		MasterHostLiveTime=0;
		if(TmpCan2RcvBuf[0] != 0x20)	return(0);
		if(TmpCan2RcvBuf[1] <  0x0a)	return(0);
		if(TmpCan2RcvBuf[2] != 0x00)	return(0);

		if(TmpCan2RcvBuf[mCurrentState1+4] & 0x10)		bMasterHostCDS=1;
		else											bMasterHostCDS=0;

		i=(unsigned int)(TmpCan2RcvBuf[mMasterSync+4]);
		j=PcAckTime;
		if( i < 0xff){
			if(i >= j){
				i=(i-j);
				if( (i > (LuLdTime+5)) || ( (i+5) < LuLdTime)){
					LuLdTime=i;
				}
			}
		}
	}


	if( (TmpCan2RcvBuf[0] == 0x00) && (TmpCan2RcvBuf[1] == 0x02) && (j==0)){
		PcAckTime=0;
		return(0);
	}

	return(0);
}




unsigned int __attribute__((section(".usercode"))) TimeCmpare(unsigned int i_basetime,unsigned int i)
{
    LocalType     Tmp_timeH,Tmp_timeM,Tmp_Stime,Tmp_Etime;
    LocalType     i_localtime;

/*
	Tmp_timeH=cF_FLRDSPCH((unsigned long)(F_0506 + i + 0));
	if(Tmp_timeH >= 24)	return(0);
	Tmp_timeM=cF_FLRDSPCH((unsigned long)(F_0506 + i + 1));
	if(Tmp_timeM >= 60)	return(0);
	Tmp_Stime=( (Tmp_timeH << 8) | Tmp_timeM);


	Tmp_timeH=cF_FLRDSPCH((unsigned long)(F_0506 + i + 2));
	if(Tmp_timeH >= 24)	return(0);
	Tmp_timeM=cF_FLRDSPCH((unsigned long)(F_0506 + i + 3));
	if(Tmp_timeM >= 60)	return(0);
	Tmp_Etime=( (Tmp_timeH << 8) | Tmp_timeM);
*/


	Tmp_timeH=cF_FLRDSPCH((unsigned long)(F_Day_S_H));
	Tmp_timeH=(Tmp_timeH << 8);
	Tmp_timeM=cF_FLRDSPCH((unsigned long)(F_Day_S_M));

	Tmp_Stime=(Tmp_timeH | Tmp_timeM);


	Tmp_timeH=cF_FLRDSPCH((unsigned long)(F_Day_E_H));
	Tmp_timeH=(Tmp_timeH << 8);
	Tmp_timeM=cF_FLRDSPCH((unsigned long)(F_Day_E_M));

	Tmp_Etime=(Tmp_timeH | Tmp_timeM);


/*
	Tmp_timeH=cF_FLRDSPCH((unsigned long)(F_Day_E_H));
	Tmp_timeH=(Tmp_timeH << 8);
	Tmp_Stime=cF_FLRDSPCH((unsigned long)(F_Day_E_M));

	Tmp_Etime=(Tmp_timeH | Tmp_Stime);



	Tmp_timeH= ((i_localtime & 0x00f0) >> 4);
	Tmp_timeH=(Tmp_timeH * 10);

	i_localtime=cF_FLRDSPCH((unsigned long)(F_Day_S_H));
	Tmp_timeH= ((i_localtime & 0x00f0) >> 4);
	Tmp_timeH=(Tmp_timeH * 10);



	Tmp_timeH=cF_FLRDSPCH((unsigned long)(F_Day_S_H));

	if(Tmp_timeH >= 24)	return(0);
	Tmp_timeM=cF_FLRDSPCH((unsigned long)(F_Day_S_M));
	if(Tmp_timeM >= 60)	return(0);
	Tmp_Stime=( (Tmp_timeH << 8) | Tmp_timeM);


	Tmp_timeH=cF_FLRDSPCH((unsigned long)(F_Day_E_H));
	if(Tmp_timeH >= 24)	return(0);
	Tmp_timeM=cF_FLRDSPCH((unsigned long)(F_Day_E_M));
	if(Tmp_timeM >= 60)	return(0);
	Tmp_Etime=( (Tmp_timeH << 8) | Tmp_timeM);

*/

	if(Tmp_Stime == Tmp_Etime)	return(0);

	if(Tmp_Stime > Tmp_Etime){
		if( (i_basetime >= Tmp_Etime) && (i_basetime < Tmp_Stime)){
				return(0);  // night
		}
		else	return(1);	// day
	}
	else{
		if( (i_basetime >= Tmp_Stime) && (i_basetime < Tmp_Etime)){
				return(1);  // day
		}
		else	return(0);	// night
	}

	return(0);

/*
	if( (i_basetime >= Tmp_Stime) && (i_basetime < Tmp_Etime)){
			return(1);  // day
	}
	else	return(0);

	//return(0) is night
*/
}




unsigned int	__attribute__((section(".usercode"))) CurrentTimeRead(void)
{
    LocalType     cTime,cMin;

	cTime=0;

/*
    cTime=(((sRamDArry[mHour] & 0xf0) >> 4) * 10);
    cTime=((sRamDArry[mHour] & 0x0f) + cTime);
    cMin=(((sRamDArry[mMinuate] & 0xf0) >> 4) * 10);
    cMin=((sRamDArry[mMinuate] & 0x0f) + cMin);
	if((cTime < 24) && (cMin < 60)){
		cTime=((cTime << 8) | cMin);
	}
	else	cTime=0;

*/
    cTime=(sRamDArry[mHour]);
	cTime=(cTime << 8);
    cMin=sRamDArry[mMinuate];

	if(cTime >= 0x2400)	return(0);
	if(cMin  >= 0x60)	return(0);

	cTime=(cTime | cMin);

	return(cTime);
}



unsigned int	__attribute__((section(".usercode"))) TimerRunCheck(unsigned int day_night)
{
    LocalType     cTime;

	cTime=CurrentTimeRead();

	if(cTime > 0){
		if(TimeCmpare(cTime,0)){
			if(day_night==1)	return(0);	  	// day
			else				return(1);
		}
/*
		else if(TimeCmpare(cTime,5)){
			if(day_night==1)	return(0);		// day
			else				return(1);
		}
		else if(TimeCmpare(cTime,10)){
			if(day_night==1)	return(0);		// day
			else				return(1);
		}
*/
		else{
			if(day_night==2)	return(0);		// night
			else				return(1);
		}
	}
	else{		// every night
		if(day_night==2)	return(0);
		else				return(1);
	}

}

/////////////////////////////////////////////


unsigned int  __attribute__((section(".usercode"))) OutSet(void)
{


	if(sRamDArry[mOut1Status]	& 0x01)	OUT_Y0(1);
	else								OUT_Y0(0);

	if(sRamDArry[mOut1Status]	& 0x02)	OUT_Y1(1);
	else								OUT_Y1(0);

	if(sRamDArry[mOut1Status]	& 0x04)	OUT_Y2(1);
	else								OUT_Y2(0);

	if(sRamDArry[mOut1Status]	& 0x08)	OUT_Y3(1);
	else								OUT_Y3(0);

	if(sRamDArry[mOut1Status]	& 0x10)	OUT_Y4(1);
	else								OUT_Y4(0);

	if(sRamDArry[mOut1Status]	& 0x20)	OUT_Y5(1);
	else								OUT_Y5(0);

	if(sRamDArry[mOut1Status]	& 0x40)	OUT_Y6(1);
	else								OUT_Y6(0);

	if(sRamDArry[mOut1Status]	& 0x80)	OUT_Y7(1);
	else								OUT_Y7(0);

	if(sRamDArry[mOut2Status]	& 0x01)	OUT_Y8(1);
	else								OUT_Y8(0);

	if(sRamDArry[mOut2Status]	& 0x02)	OUT_Y9(1);
	else								OUT_Y9(0);


}




unsigned int  __attribute__((section(".usercode")))
DayNightChk(unsigned int day_night) {

	bool night = b_S1_CDS_ONOFF;
	bool timeMode =  b_S1_CDS_TIMER;


	if(timeMode){
		if(!TimerRunCheck(day_night))	return(0);
	}
	else{
		if(night){
			if(day_night==2)			return(0);
		}
		else{
			if(day_night==1)			return(0);
		}
	}

	return(1);
}


unsigned int  __attribute__((section(".usercode"))) OneModeChk(unsigned int value,unsigned int bitvalue)
{
	unsigned int i,j,k;
	unsigned int tmp_mode,virtual_bufH,virtual_bufL;

	tmp_mode=value;

	if(tmp_mode & ROMOTE_CMD){
		virtual_bufH=sRamDArry[VIRTUAL_RAM+mOut2Status];
		virtual_bufH=(virtual_bufH << 8);
		virtual_bufL=sRamDArry[VIRTUAL_RAM+mOut1Status];
		virtual_bufH=(virtual_bufH | virtual_bufL);
		if(bitvalue & virtual_bufH){
			if( (tmp_mode & BLINCK_BIT) && bBlinck)		return(1);  // off
			else										return(0);  // on
		}
		else{
//			return(1);  // off
			return(2);  // off
		}
	}
	else if(tmp_mode & EVERY){
		if( (tmp_mode & BLINCK_BIT) && bBlinck)		return(1);
		else										return(0);
	}

	else if(tmp_mode & DAY_ONLY){
		if(!DayNightChk(1)){
			if( (tmp_mode & BLINCK_BIT) && bBlinck)		return(1);
			else										return(0);
		}
		else	return(2);

/*
		if( (tmp_mode & BLINCK_BIT) && bBlinck)		return(1);
		else{
			if(!DayNightChk(1))						return(0);
			else									return(1);
		}
*/

	}
	else if(tmp_mode & NIGHT_ONLY){
		if(!DayNightChk(2)){
			if( (tmp_mode & BLINCK_BIT) && bBlinck)		return(1);
			else										return(0);
		}
		else	return(2);

/*
		if( (tmp_mode & BLINCK_BIT) && bBlinck)		return(1);
		else{
			if(!DayNightChk(2))						return(0);
			else									return(1);
		}
*/
	}
	else{
//		return(1);
		return(2);
	}
}




unsigned int   __attribute__((section(".usercode"))) AutoModeRun(void)
{

	unsigned int i;


	i=OneModeChk(cF_0Mode,0x01);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x01);
	else			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x01);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x01);

	i=OneModeChk(cF_1Mode,0x02);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x02);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x02);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x02);

	i=OneModeChk(cF_2Mode,0x04);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x04);
	else			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x04);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x04);

	i=OneModeChk(cF_3Mode,0x08);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x08);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x08);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x08);

	i=OneModeChk(cF_4Mode,0x10);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x10);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x10);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x10);

	i=OneModeChk(cF_5Mode,0x20);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x20);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x20);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x20);

	i=OneModeChk(cF_6Mode,0x40);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x40);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x40);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x40);

	i=OneModeChk(cF_7Mode,0x80);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x80);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x80);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x80);

	i=OneModeChk(cF_8Mode,0x100);
	if(i==0)		sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] | 0x01);
	else 			sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] & ~0x01);
//	else if(i==1)	sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] & ~0x01);

	i=OneModeChk(cF_9Mode,0x200);
	if(i==0)		sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] | 0x02);
	else 			sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] & ~0x02);
//	else if(i==1)	sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] & ~0x02);

	return(0);
}




unsigned int   __attribute__((section(".usercode"))) ManualModeRun(void)
{

	unsigned int i;


	i=OneModeChk( ((cF_0Mode & 0xf0) | EVERY),0x01);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x01);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x01);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x01);

	i=OneModeChk( ((cF_1Mode & 0xf0) | EVERY),0x02);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x02);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x02);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x02);

	i=OneModeChk( ((cF_2Mode & 0xf0) | EVERY),0x04);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x04);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x04);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x04);

	i=OneModeChk( ((cF_3Mode & 0xf0) | EVERY),0x08);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x08);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x08);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x08);

	i=OneModeChk( ((cF_4Mode & 0xf0) | EVERY),0x10);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x10);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x10);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x10);

	i=OneModeChk( ((cF_5Mode & 0xf0) | EVERY),0x20);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x20);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x20);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x20);

	i=OneModeChk( ((cF_6Mode & 0xf0) | EVERY),0x40);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x40);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x40);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x40);

	i=OneModeChk( ((cF_7Mode & 0xf0) | EVERY),0x80);
	if(i==0)		sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x80);
	else 			sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x80);
//	else if(i==1)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x80);

	i=OneModeChk( ((cF_8Mode & 0xf0) | EVERY),0x100);
	if(i==0)		sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] | 0x01);
	else 			sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] & ~0x01);
//	else if(i==1)	sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] & ~0x01);

	i=OneModeChk( ((cF_9Mode & 0xf0) | EVERY),0x200);
	if(i==0)		sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] | 0x02);
	else 			sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] & ~0x02);
//	else if(i==1)	sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] & ~0x02);

	return(0);
}





void  __attribute__((section(".usercode"))) RomoteModeRun(void)
{

//	unsigned int i;
//	unsigned int tmp_buf,tmp_buf1,tmp_buf2;

	unsigned int virtual_bufH,virtual_bufL;

	virtual_bufH=sRamDArry[VIRTUAL_RAM+mOut2Status];
	virtual_bufH=(virtual_bufH << 8);
	virtual_bufL=sRamDArry[VIRTUAL_RAM+mOut1Status];
	virtual_bufH=(virtual_bufH | virtual_bufL);



	if(virtual_bufH	& 0x01)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x01);
	else				sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x01);

	if(virtual_bufH	& 0x02)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x02);
	else				sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x02);

	if(virtual_bufH	& 0x04)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x04);
	else				sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x04);

	if(virtual_bufH	& 0x08)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x08);
	else				sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x08);

	if(virtual_bufH	& 0x10)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x10);
	else				sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x10);

	if(virtual_bufH	& 0x20)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x20);
	else				sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x20);

	if(virtual_bufH	& 0x40)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x40);
	else				sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x40);

	if(virtual_bufH	& 0x80)	sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] | 0x80);
	else				sRamDArry[mOut1Status] = (sRamDArry[mOut1Status] & ~0x80);

	if(virtual_bufH	& 0x100)	sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] | 0x01);
	else						sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] & ~0x01);

	if(virtual_bufH	& 0x200)	sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] | 0x02);
	else						sRamDArry[mOut2Status] = (sRamDArry[mOut2Status] & ~0x02);

}






unsigned int  __attribute__((section(".usercode"))) OffCountChk(unsigned char SetValue,unsigned char CurValue,unsigned char Cnt)
{

	unsigned char i;

	i= (CurValue/SetValue);

	if(Cnt > i){
		i=(Cnt-i);
	}
	else	i=0;

	return(i);
}



unsigned int  __attribute__((section(".usercode"))) ErrWarningChk(unsigned char SetValue,unsigned char CurValue,unsigned char WarningVal,unsigned char ErrVal)
{
	unsigned int val;

	if(CurValue < ErrVal)			return(2);
	else if(CurValue < WarningVal)	return(1);
	else							return(0);
}





unsigned int  __attribute__((section(".usercode"))) SystemCheck(void)
{


	unsigned int val;

	if(bBlinck)				return(0);
    if(WarmingUpTime < 50)	return(0);

/////////////////////////////////////////////////////////////////


	if(cF_MainVolt > 0){
		val=ErrWarningChk(cF_MainVolt,sRamDArry[mMainVoltage],(unsigned char)((cF_MainVolt * 95)/100),(unsigned char)((cF_MainVolt * 90)/100));
		if(val==2){
			b_MainVoltErr=1;
			b_MainVoltWarnning=0;
		}
		else if(val==1){
			b_MainVoltErr=0;
			b_MainVoltWarnning=1;
		}
		else{
			b_MainVoltErr=0;
			b_MainVoltWarnning=0;
		}
	}
	else{
		b_MainVoltErr=0;
		b_MainVoltWarnning=0;
	}
/////////////////////////////////////////////////////////////////


	if(cF_MainCurrent > 0){
		val=ErrWarningChk(cF_MainCurrent,sRamDArry[mMainCurrent],(unsigned char)((cF_MainCurrent * 90)/100),(unsigned char)((cF_MainCurrent * 80)/100));
		if(val==2){
			b_MainCurrentErr=1;
			b_MainCurrentWarnning=0;
		}
		else if(val==1){
			b_MainCurrentErr=0;
			b_MainCurrentWarnning=1;
		}
		else{
			b_MainCurrentErr=0;
			b_MainCurrentWarnning=0;
		}
	}
	else{
		b_MainCurrentErr=0;
		b_MainCurrentWarnning=0;
	}

/////////////////////////////////////////////////////////////////

	if(cF_TrVolt > 0){
		val=ErrWarningChk(cF_TrVolt,sRamDArry[mTrVoltage],(unsigned char)((cF_TrVolt * 90)/100),(unsigned char)((cF_TrVolt * 80)/100));
		if(val==2){
			b_TrVoltErr=1;
			b_TrVoltWarnning=0;
		}
		else if(val==1){
			b_TrVoltErr=0;
			b_TrVoltWarnning=1;
		}
		else{
			b_TrVoltErr=0;
			b_TrVoltWarnning=0;
		}
	}
	else{
		b_TrVoltErr=0;
		b_TrVoltWarnning=0;
	}
/////////////////////////////////////////////////////////////////

	if(cF_TrCurrent > 0){
		val=ErrWarningChk(cF_TrCurrent,sRamDArry[mTrCurrent],(unsigned char)((cF_TrCurrent * 90)/100),(unsigned char)((cF_TrCurrent * 80)/100));
		if(val==2){
			b_TrCurrentErr=1;
			b_TrCurrentWarnning=0;
		}
		else if(val==1){
			b_TrCurrentErr=0;
			b_TrCurrentWarnning=1;
		}
		else{
			b_TrCurrentErr=0;
			b_TrCurrentWarnning=0;
		}
	}
	else{
		b_TrCurrentErr=0;
		b_TrCurrentWarnning=0;
	}
/////////////////////////////////////////////////////////////////

	val=OneModeChk(cF_0Mode,0x01);
	if( (cF_0Amp > 0) && (cF_0Count>0) && ((cF_0Mode & 0x0f) !=0) && ( val < 2) ){
		if(val==0){
			sRamDArry[mOut0OffCnt]=OffCountChk( (unsigned char)(cF_0Amp),(unsigned char)sRamDArry[mOut0Current],(unsigned char)(cF_0Count));
			if(sRamDArry[mOut0OffCnt] >=  cF_0Count){
				b_Out0Err=1;
				b_Out0Warning=0;
			}
			else if(sRamDArry[mOut0OffCnt] >=  1){
				b_Out0Err=0;
				b_Out0Warning=1;
			}
			else{
				b_Out0Err=0;
				b_Out0Warning=0;
			}
		}
	}
	else{
		sRamDArry[mOut0Current]=0;
		sRamDArry[mOut0OffCnt]=0;
		b_Out0Err=0;
		b_Out0Warning=0;
	}


/////////////////////////////////////////////////////////////////
	val=OneModeChk(cF_1Mode,0x02);
	if( (cF_1Amp > 0) && (cF_1Count>0) && ((cF_1Mode & 0x0f) != 0) && ( val < 2) ){
		if(val==0){
			sRamDArry[mOut1OffCnt]=OffCountChk( (unsigned char)(cF_1Amp),(unsigned char)sRamDArry[mOut1Current],(unsigned char)(cF_1Count));
			if(sRamDArry[mOut1OffCnt] >=  cF_1Count){
				b_Out1Err=1;
				b_Out1Warning=0;
			}
			else if(sRamDArry[mOut1OffCnt] >=  1){
				b_Out1Err=0;
				b_Out1Warning=1;
			}
			else{
				b_Out1Err=0;
				b_Out1Warning=0;
			}
		}
	}
	else{
		sRamDArry[mOut1Current]=0;
		sRamDArry[mOut1OffCnt]=0;
		b_Out1Err=0;
		b_Out1Warning=0;
	}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
	val=OneModeChk(cF_2Mode,0x04);
	if( (cF_2Amp > 0) && (cF_2Count>0) && ((cF_2Mode & 0x0f) != 0) && ( val < 2) ){
		if(val==0){
			sRamDArry[mOut2OffCnt]=OffCountChk( (unsigned char)(cF_2Amp),(unsigned char)sRamDArry[mOut2Current],(unsigned char)(cF_2Count));
			if(sRamDArry[mOut2OffCnt] >=  cF_2Count){
				b_Out2Err=1;
				b_Out2Warning=0;
			}
			else if(sRamDArry[mOut2OffCnt] >=  1){
				b_Out2Err=0;
				b_Out2Warning=1;
			}
			else{
				b_Out2Err=0;
				b_Out2Warning=0;
			}
		}
	}
	else{
		sRamDArry[mOut2Current]=0;
		sRamDArry[mOut2OffCnt]=0;
		b_Out2Err=0;
		b_Out2Warning=0;
	}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
	val=OneModeChk(cF_3Mode,0x08);
	if( (cF_3Amp > 0) && (cF_3Count>0) && ((cF_3Mode & 0x0f) != 0) && ( val < 2) ){
		if(val==0){
			sRamDArry[mOut3OffCnt]=OffCountChk( (unsigned char)(cF_3Amp),(unsigned char)sRamDArry[mOut3Current],(unsigned char)(cF_3Count));
			if(sRamDArry[mOut3OffCnt] >=  cF_3Count){
				b_Out3Err=1;
				b_Out3Warning=0;
			}
			else if(sRamDArry[mOut3OffCnt] >=  1){
				b_Out3Err=0;
				b_Out3Warning=1;
			}
			else{
				b_Out3Err=0;
				b_Out3Warning=0;
			}
		}
	}
	else{
		sRamDArry[mOut3Current]=0;
		sRamDArry[mOut3OffCnt]=0;
		b_Out3Err=0;
		b_Out3Warning=0;
	}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
	val=OneModeChk(cF_4Mode,0x10);
	if( (cF_4Amp > 0) && (cF_4Count>0) && ((cF_4Mode & 0x0f) != 0) && ( val < 2) ){
		if(val==0){
			sRamDArry[mOut4OffCnt]=OffCountChk( (unsigned char)(cF_4Amp),(unsigned char)sRamDArry[mOut4Current],(unsigned char)(cF_4Count));
			if(sRamDArry[mOut4OffCnt] >=  cF_4Count){
				b_Out4Err=1;
				b_Out4Warning=0;
			}
			else if(sRamDArry[mOut4OffCnt] >=  1){
				b_Out4Err=0;
				b_Out4Warning=1;
			}
			else{
				b_Out4Err=0;
				b_Out4Warning=0;
			}
		}
	}
	else{
		sRamDArry[mOut4Current]=0;
		sRamDArry[mOut4OffCnt]=0;
		b_Out4Err=0;
		b_Out4Warning=0;
	}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
	val=OneModeChk(cF_5Mode,0x20);
	if( (cF_5Amp > 0) && (cF_5Count>0) && ((cF_5Mode & 0x0f) != 0) && ( val < 2) ){
		if(val==0){
			sRamDArry[mOut5OffCnt]=OffCountChk( (unsigned char)(cF_5Amp),(unsigned char)sRamDArry[mOut5Current],(unsigned char)(cF_5Count));
			if(sRamDArry[mOut5OffCnt] >=  cF_5Count){
				b_Out5Err=1;
				b_Out5Warning=0;
			}
			else if(sRamDArry[mOut5OffCnt] >=  1){
				b_Out5Err=0;
				b_Out5Warning=1;
			}
			else{
				b_Out5Err=0;
				b_Out5Warning=0;
			}
		}
	}
	else{
		sRamDArry[mOut5Current]=0;
		sRamDArry[mOut5OffCnt]=0;
		b_Out5Err=0;
		b_Out5Warning=0;
	}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
	val=OneModeChk(cF_6Mode,0x40);
	if( (cF_6Amp > 0) && (cF_6Count>0) && ((cF_6Mode & 0x0f) != 0) && ( val < 2) ){
		if(val==0){
			sRamDArry[mOut6OffCnt]=OffCountChk( (unsigned char)(cF_6Amp),(unsigned char)sRamDArry[mOut6Current],(unsigned char)(cF_6Count));
			if(sRamDArry[mOut6OffCnt] >=  cF_6Count){
				b_Out6Err=1;
				b_Out6Warning=0;
			}
			else if(sRamDArry[mOut6OffCnt] >=  1){
				b_Out6Err=0;
				b_Out6Warning=1;
			}
			else{
				b_Out6Err=0;
				b_Out6Warning=0;
			}
		}
	}
	else{
		sRamDArry[mOut6Current]=0;
		sRamDArry[mOut6OffCnt]=0;
		b_Out6Err=0;
		b_Out6Warning=0;
	}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
	val=OneModeChk(cF_7Mode,0x80);
	if( (cF_7Amp > 0) && (cF_7Count>0) && ((cF_7Mode & 0x0f) != 0) && ( val < 2) ){
		if(val==0){
			sRamDArry[mOut7OffCnt]=OffCountChk( (unsigned char)(cF_7Amp),(unsigned char)sRamDArry[mOut7Current],(unsigned char)(cF_7Count));
			if(sRamDArry[mOut7OffCnt] >=  cF_7Count){
				b_Out7Err=1;
				b_Out7Warning=0;
			}
			else if(sRamDArry[mOut7OffCnt] >=  1){
				b_Out7Err=0;
				b_Out7Warning=1;
			}
			else{
				b_Out7Err=0;
				b_Out7Warning=0;
			}
		}
	}
	else{
		sRamDArry[mOut7Current]=0;
		sRamDArry[mOut7OffCnt]=0;
		b_Out7Err=0;
		b_Out7Warning=0;
	}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
	val=OneModeChk(cF_8Mode,0x100);
	if( (cF_8Amp > 0) && (cF_8Count>0) && ((cF_8Mode & 0x0f) != 0) && ( val < 2) ){
		if(val==0){
			sRamDArry[mOut8OffCnt]=OffCountChk( (unsigned char)(cF_8Amp),(unsigned char)sRamDArry[mOut8Current],(unsigned char)(cF_8Count));
			if(sRamDArry[mOut8OffCnt] >=  cF_8Count){
				b_Out8Err=1;
				b_Out8Warning=0;
			}
			else if(sRamDArry[mOut8OffCnt] >=  1){
				b_Out8Err=0;
				b_Out8Warning=1;
			}
			else{
				b_Out8Err=0;
				b_Out8Warning=0;
			}
		}
	}
	else{
		sRamDArry[mOut8Current]=0;
		sRamDArry[mOut8OffCnt]=0;
		b_Out8Err=0;
		b_Out8Warning=0;
	}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
	val=OneModeChk(cF_9Mode,0x200);
	if( (cF_9Amp > 0) && (cF_9Count>0) && ((cF_9Mode & 0x0f) != 0) && ( val < 2) ){
		if(val==0){
			sRamDArry[mOut9OffCnt]=OffCountChk( (unsigned char)(cF_9Amp),(unsigned char)sRamDArry[mOut9Current],(unsigned char)(cF_9Count));
			if(sRamDArry[mOut9OffCnt] >=  cF_9Count){
				b_Out9Err=1;
				b_Out9Warning=0;
			}
			else if(sRamDArry[mOut9OffCnt] >=  1){
				b_Out9Err=0;
				b_Out9Warning=1;
			}
			else{
				b_Out9Err=0;
				b_Out9Warning=0;
			}
		}
	}
	else{
		sRamDArry[mOut9Current]=0;
		sRamDArry[mOut9OffCnt]=0;
		b_Out9Err=0;
		b_Out9Warning=0;
	}
/////////////////////////////////////////////////////////////////

	if(b_Out0Err || b_Out1Err || b_Out2Err || b_Out3Err || b_Out4Err || b_Out5Err || b_Out6Err || b_Out7Err || b_Out8Err || b_Out9Err || b_MainVoltErr || b_MainCurrentErr || b_TrVoltErr || b_TrCurrentErr){
		bSystemErr=1;
		bSystemWarning=0;
	}
	else if(b_Out0Warning || b_Out1Warning || b_Out2Warning || b_Out3Warning || b_Out4Warning || b_Out5Warning || b_Out6Warning || b_Out7Warning || b_Out8Warning || b_Out9Warning || b_MainVoltWarnning || b_MainCurrentWarnning || b_TrVoltWarnning || b_TrCurrentWarnning){
		bSystemErr=0;
		bSystemWarning=1;
	}
	else{
		bSystemErr=0;
		bSystemWarning=0;
	}

	return(0);
}


#define CDS_SET			0
#define TIME_SET		1


// @@ 이부분 수정해야 한다.
bool isMasterCdsUse(void) {
/*
	로더 셋팅 값 반환 : MasterCds 여부
	* CDS
	* Timer
	* MasterCds
*/
	return IN_X5;
}

void  __attribute__((section(".usercode"))) SystemRun(void) {

	bool masterCds = isMasterCdsUse();

	if (masterCds) {
		if(bMasterHostCDS)		b_S1_CDS_ONOFF=1;
		else					b_S1_CDS_ONOFF=0;
	} else {
		if(IN_X6)				b_S1_CDS_ONOFF=1;
		else					b_S1_CDS_ONOFF=0;
	}

	if(cF_SystemMode == TIME_SET)		b_S1_CDS_TIMER=1;    //timer mode
	else								b_S1_CDS_TIMER=0;    // cds mode


	if(b_S1_Remote){
		b_S1_Auto=0;
		b_S1_Manual=0;
		RomoteModeRun();
	}
	else{
		if( IN_X0){
			if(b_S1_Auto==0){
				b_S1_Auto=1;
				b_S1_Manual=0;
//				sRamDArry[mOut1Status]=VirtualOut1;
//				sRamDArry[mOut1Status]=VirtualOut2;
			}
			AutoModeRun();
		}
		else{
			if(b_S1_Manual==0){
				b_S1_Auto=0;
				b_S1_Manual=1;
//				VirtualOut1=sRamDArry[mOut1Status];
//				VirtualOut2=sRamDArry[mOut2Status];
			}
			ManualModeRun();

//			sRamDArry[mOut1Status] =0xff;
//			sRamDArry[mOut2Status] =0x03;
		}
	}

	SystemCheck();

	OutSet();

}




void  __attribute__((section(".usercode"))) OutData(void)
{

    unsigned int tmpio;
    unsigned int tmpiobuf;

    tmpio=(LATD & 0xff00);
    tmpiobuf=(O_Y_0_bit);
    tmpio=(tmpiobuf | tmpio);
    LATD=tmpio;

	sRamDArry[mOut1Status]=	O_Y_0_bit;

    tmpio=(LATC & 0x9fff);
    tmpiobuf=(O_OP_bit << 8);
    tmpio=(tmpiobuf | tmpio);
    LATC=tmpio;

    sRamDArry[mOut2Status] = (O_OP_bit >> 5);

}




void __attribute__((section(".usercode")))  InPutRead(void)
{
    unsigned int i;


	X0Byte=0;

	if(!_RD8)	X0Byte=(X0Byte | 0x01);
	if(!_RD9)	X0Byte=(X0Byte | 0x02);
	if(!_RD10)	X0Byte=(X0Byte | 0x04);
	if(!_RD11)	X0Byte=(X0Byte | 0x08);
	if(!_RD12)	X0Byte=(X0Byte | 0x10);
	if(!_RD13)	X0Byte=(X0Byte | 0x20);
	if(!_RD14)	X0Byte=(X0Byte | 0x40);
	if(!_RD15)	X0Byte=(X0Byte | 0x80);

	X0Byte= ~X0Byte;

    if(BEF_IN_X0_IN == IN_X0_IN){
        if(X0Time>10){
            IN_X0_PORT=BEF_IN_X0_IN;
            X0Time=0;
        }
    }
    else{
        BEF_IN_X0_IN=IN_X0_IN;
        X0Time=0;
    }

    if(BEF_IN_X1_IN == IN_X1_IN){
        if(X1Time>10){
            IN_X1_PORT=BEF_IN_X1_IN;
            X1Time=0;
        }
    }
    else{
        BEF_IN_X1_IN=IN_X1_IN;
        X1Time=0;
    }


    if(BEF_IN_X2_IN == IN_X2_IN){
        if(X2Time>10){
            IN_X2_PORT=BEF_IN_X2_IN;
            X2Time=0;
        }
    }
    else{
        BEF_IN_X2_IN=IN_X2_IN;
        X2Time=0;
    }
    if(BEF_IN_X3_IN == IN_X3_IN){
        if(X3Time>10){
            IN_X3_PORT=BEF_IN_X3_IN;
            X3Time=0;
        }
    }
    else{
        BEF_IN_X3_IN=IN_X3_IN;
        X3Time=0;
    }


    if(BEF_IN_X4_IN == IN_X4_IN){
        if(X4Time>10){
            IN_X4_PORT=BEF_IN_X4_IN;
            X4Time=0;
        }
    }
    else{
        BEF_IN_X4_IN=IN_X4_IN;
        X4Time=0;
    }

    if(BEF_IN_X5_IN == IN_X5_IN){
        if(X5Time>10){
            IN_X5_PORT=BEF_IN_X5_IN;
            X5Time=0;
        }
    }
    else{
        BEF_IN_X5_IN=IN_X5_IN;
        X5Time=0;
    }

    if(BEF_IN_X6_IN == IN_X6_IN){
        if(X6Time>10){
            IN_X6_PORT=BEF_IN_X6_IN;
            X6Time=0;
        }
    }
    else{
        BEF_IN_X6_IN=IN_X6_IN;
        X6Time=0;
    }

    if(BEF_IN_X7_IN == IN_X7_IN){
        if(X7Time>10){
            IN_X7_PORT=BEF_IN_X7_IN;
            X7Time=0;
        }
    }
    else{
        BEF_IN_X7_IN=IN_X7_IN;
        X7Time=0;
    }

    sRamDArry[mInStatus]  = I_X0_bit;
}


void  __attribute__((section(".usercode"))) OutAllClear(void)
{

   	O_Y_0_bit=0x0;
	sRamDArry[mOut1Status]=	O_Y_0_bit;

	O_OP_bit=0x0;
	sRamDArry[mOut1Status]=	( O_OP_bit >> 5);

	OutData();
}


void  __attribute__((section(".usercode")))   new_IO_Check(void)
{

////////////   	sRamDArry[mSysStatus]=sREADY;

   	InPutRead();
	SystemRun();
    OutData();

	if(MasterHostLiveTime > 40){
		bMasterHostCDS=0;
		MasterHostLiveTime =0;
	}
}
