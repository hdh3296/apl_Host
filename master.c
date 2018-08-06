
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///2014,3,14, ver 5.31 : screen door "fs1" ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//door open wait
//over heat
//door select(running open)
//handok gbr

#include <p30fxxxx.h>
//#include <p33exxxx.h>

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



_FOSC(CSW_FSCM_OFF & XT_PLL8 & 0xc30f);
_FWDT(WDT_ON & WDTPSA_64 & WDTPSB_1 & 0x803f);
//_FWDT(WDT_OFF & WDTPSA_64 & WDTPSB_1 & 0x803f);

_FBORPOR(PBOR_ON & BORV27 & PWRT_64 & MCLR_EN & 0x87b3);
_FGS(0x0007);

//_FGS(CODE_PROT_OFF & 0x0003);
//_FGS(0x0004);

/*
#define _CONFIG4(x) __attribute__((section("__CONFIG4.sec,x"))) int _CONFIG4 = (x);
#define _CONFIG5(x) __attribute__((section("__CONFIG5.sec,x"))) int _CONFIG5 = (x);

_CONFIG4(0x0000);
_CONFIG5(0x0000);
*/


/*
extern void   DspFloorSet(void);
extern void   CarStopCmd(void);
extern void   CarUpStartCmd(void);
extern void   CarDnStartCmd(void);
extern void   CarCurFloorRead(void);
extern void   InverterErrorCheck(void);
extern void   ManualHomeSpeedCmd(void);
*/



extern void	Initialize_DS1302(void);
extern void disp_clk_regs(void);
extern void	FireKeyCheck(void);
extern void erase_flash_row(unsigned int);
extern void program_flash(unsigned int, unsigned int);


UserDataType    YourKey0[8]={0,0,0,0,0,0,0,0};
UserDataType    YourKey1[8]={0,0,0,0,0,0,0,0};
UserDataType    YourKey2[8]={0,0,0,0,0,0,0,0};
UserDataType    YourKey3[8]={0,0,0,0,0,0,0,0};
UserDataType    BaggageFlrOnOff[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,};

UserDataType    errbd[12]={0};
UserDataType    Olderrbd[12]={0};
UserDataType    LadderBuf[16]={0};
UserDataType    LadderData[16]={0};
UserDataType    sRamDArry[END_SRAM]={0};
UserDataType    BdErrCntSeq[ABS_ADDRESS]={0};
UserDataType    BDid[ABS_ADDRESS]={0};
UserDataType    eDArry[E_END]={0};
UserDataType    Cancle=0;
UserDataType    BefRcvAdr=0;
UserDataType    CurRcvAdr=0;
UserDataType    LogAddress=0;
UserDataType    AbsAddress=0;
UserDataType    LogAddressLoop=0;
UserDataType    TimeOutBd=0;
UserDataType    src=0;
UserDataType    firstdata=0;
UserDataType    seconddata=0;
UserDataType    Bef_I_EMG=0;
UserDataType    Bef_I_FS0=0;
UserDataType    Bef_I_GR=0;
UserDataType    Bef_I_SU1=0;
UserDataType    Bef_I_X_0=0;
UserDataType    Bef_I_FIRE=0;
UserDataType    I_EMG_Cht=0;
UserDataType    I_FS0_Cht=0;
UserDataType    I_GR_Cht=0;
UserDataType    I_SU1_Cht=0;
UserDataType    I_X_0_Cht=0;
UserDataType    I_FIRE_Cht=0;
UserDataType    NoCloseCnt=0;
UserDataType    errpt=0;
UserDataType    ErrBdNamePt=0;
UserDataType    BefErrBdNamePt=0;

//UserDataType    eMvCounterCnt=0;
//UserDataType    OpenOutOffTime=0;

UserDataType    CloseOutOffTime=0;
UserDataType    VoiceTime=0;
UserDataType    BellTime=0;
UserDataType    FhmLoaderTime=0;
UserDataType    AccTimer=0;

//UserDataType    RstTime=0;

UserDataType    UpBttonTime=0;
UserDataType    DnBttonTime=0;
UserDataType    DoorCloseTime=0;
UserDataType    DoorOpenOnTime=0;
UserDataType    DoorCloseOnTime=0;
UserDataType    msec10=0;
UserDataType    sec1=0;
UserDataType    EepTime=0;
UserDataType    FanTime=0;
UserDataType    ManualStopTime=0;
UserDataType    WarmingUpTime=0;
UserDataType    FixFlrTimeCnt=0;
UserDataType    errtime=0;
UserDataType    FhmTime=0;
UserDataType    CurFTime=0;
UserDataType    NextFTime=0;
UserDataType    RestartTime=0;
UserDataType    ExtCmdFlrONOFFCnt=0;
UserDataType    SerialLoopMaxTime=0;
UserDataType    RelevelTime=0;
UserDataType    FireBaseFloor=0;
UserDataType    CurSpeed=0;
UserDataType    OriginalHighFlr=0;
UserDataType    OriginalLowFlr=0;
UserDataType    CountDn=0;
UserDataType    CountUp=0;
UserDataType    UseBaseTime=0;
UserDataType    PowerOnTime=0;
UserDataType    NewDoorSelect=0;
UserDataType    CurDoorSelect=0;
/////////UserDataType    SaveMoveCounter=0;
////////////////UserDataType    LuLdCounter=0;


UserDataType    LuLdErrNm=0;
UserDataType    LuLdSeqPt=0;
UserDataType    LuLdSeqMinCnt=0;
UserDataType    LuLdSeq[5];

UserDataType    CrtMoveFlr;


UserDataType    EncoderReloadTime;


UserDataType    NotRecoveryData;

UserDataType    ReLevelAnounceTime;

unsigned	int    	    NextFloorTime=0;
unsigned	int     	MinuteTime=0;
unsigned	int   	    LuLdTime=0;
//unsigned	int   	    LuOrLdOnTime=0;
unsigned	int   	    CounterTime=0;
unsigned	int   	    CounterChkTime=0;
unsigned	int    	    DoorOpenTime=0;
unsigned	int    	    LightTime=0;
unsigned	int    	    AutoNoMoveTime=0;
unsigned	int    	    SolTimer=0;
unsigned	int    	    SolWaitTimer=0;
unsigned	int    	    ExIOTimer=0;

unsigned	int    	    BreakTime=0;


//unsigned	int    	    zz=0;



UserDataType    OldFireBuf=0;
UserDataType    AutoBit=0;
UserDataType    EmerBit=0;
UserDataType    SlipBit=0;
UserDataType    LuLdBit=0;
UserDataType    OpenEndBit=0;
UserDataType    BefDlsBit=0;
UserDataType    BefDestFBit=0;
UserDataType    EepRWBit=0;
UserDataType    Etc1Bit=0;
UserDataType    Etc2Bit=0;
UserDataType    Etc3Bit=0;
UserDataType    PC1Bit=0;
UserDataType    StateBit0=0;
UserDataType    StateBit1=0;
UserDataType    StateBit2=0;
UserDataType    StateBit3=0;
UserDataType    StateBit4=0;
UserDataType    StateBit5=0;
UserDataType    StateBit6=0;
UserDataType    Vip_Floor=0;
UserDataType    S0_STATE_bit=0;
UserDataType    S1_STATE_bit=0;
UserDataType    S2_STATE_bit=0;
UserDataType    S3_STATE_bit=0;
UserDataType    S4_STATE_bit=0;
UserDataType    I_SU1_bit=0;
UserDataType    I_EMG_bit=0;
UserDataType    I_GR_bit=0;
UserDataType    I_FIRE_bit=0;
UserDataType    I_FS0_bit=0;
UserDataType    I_X0_bit=0;
UserDataType    O_OP_bit=0;
UserDataType    O_U_W_bit=0;
UserDataType    O_Y_0_bit=0;
UserDataType    DoorStatus_bit=0;
UserDataType    FhmToggle=0;
UserDataType    PowerFailTime=0;
UserDataType    CarOnceStopTime=0;
UserDataType    LadderKeyTime=0;
UserDataType    ElevStopTime=0;
UserDataType    ElevMoveTime=0;
UserDataType    MyGroupAddr;
UserDataType    MyLocalAddr;

UserDataType    CarBdButtonFlr=0;

//UserDataType    MyNc_Addr;
//UserDataType    MyLowDip;

UserDataType    SegData0;
UserDataType    SegData1;
UserDataType    SegData2;
UserDataType    SegSel;
UserDataType    LadderTime;
UserDataType    SegError;
UserDataType    ClrCnt=0;
UserDataType    SaveVerify=0;
UserDataType	lu_ld_state;
UserDataType	bef_lu_ld_state;
UserDataType	DoorJumperNm;
UserDataType	NoStart=0;

///////////////
//20005


UserDataType    EmgByte;
UserDataType    Su1Byte;
UserDataType    GrByte;
UserDataType    FireByte;
UserDataType    Fs_Byte;
UserDataType    X0Byte;
UserDataType    BefEmgByte;
UserDataType    BefSu1Byte;
UserDataType    BefGrByte;
UserDataType    BefFireByte;
UserDataType    BefFs_Byte;
UserDataType    BefX0Byte;



UserDataType    EmgTime;
UserDataType    PrkTime;
UserDataType    AtTime;
UserDataType    UbTime;
UserDataType    DbTime;
UserDataType    OlsTime;
UserDataType    GsTime;
UserDataType    DsTime;
UserDataType    Su1Time;
UserDataType    Sd1Time;
UserDataType    SftTime;
UserDataType    OvlTime;
UserDataType    UlsTime;
UserDataType    DlsTime;
UserDataType    LuTime;
UserDataType    LdTime;

UserDataType    RgTime;
UserDataType    BatTime;
UserDataType    PassTime;
UserDataType    FireTime;
UserDataType    WaterTime;
UserDataType    FullTime;
UserDataType    MmTime;
UserDataType    FhmInTime;

UserDataType    Fr1Time;
UserDataType    Fr2Time;
UserDataType    Gs2Time;
UserDataType    Ds2Time;
UserDataType    Su2Time;
UserDataType    Sd2Time;
//UserDataType    Su3Time;
UserDataType    BmTime;
UserDataType    InvTime;

UserDataType    X0Time;
UserDataType    X1Time;
UserDataType    X2Time;
UserDataType    X3Time;
UserDataType    X4Time;
UserDataType    X5Time;
UserDataType    X6Time;
UserDataType    X7Time;


UserDataType    Fs0Time;
UserDataType    Fs1Time;
UserDataType    Fs2Time;
UserDataType    Fs3Time;
UserDataType    Fs4Time;
UserDataType    DerTime;
UserDataType    FidTime;
UserDataType    UndTime;

///////////////
//20005

UserDataType    LoopTime=0;


UserDataType    RstTime=0;
UserDataType    InvErrTime=0;
UserDataType    RstCnt=0;

//UserDataType    ErrChkNm=0;


UserDataType    HibSet[10]={0,0,0,0,0,0,0,0,0,0};
UserDataType    CmdFixFlrTime=0;

UserDataType    TmpBuzor;
UserDataType    ExtKeyCnt=0;


unsigned	int		FrequenceTime,FrequenceMainTime;



UserDataType    MasterHostLiveTime=0;
UserDataType    PcAckTime=0;
UserDataType    CommonExtraRam[10];


unsigned long 	BefCurEncoderPulse=0;
unsigned long   TmpEncoderPulse=0;

unsigned long 	LevelEncoderPulse1,LevelEncoderPulse2,LevelEncoderPulse3;
unsigned long 	TestPulse1,LuLdEncoder,UpDnEncoder;


unsigned long 	xPulse1,xPulse2,BaseDecPulseX;
unsigned long	tmplong;



unsigned long  parameter_mirror[MAX_LONG_BUF]; // = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


long_field  FlashDspCharBuf[512]    __attribute__((aligned(512) , space(psv)));


const unsigned char DftFlrName[] = {"B7B6B5B4B3B2B101020304050607080910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364"};

const unsigned char StatusMessage[][16]={
                                      "Inspection Stop ",      //000
                                      "Inverter  Error ",      //001
                                      "INV Floor Error ",      //002
                                      "Emergency Stop  ",      //003
                                      "GOV Error       ",      //004
                                      "Motor Overheat  ",      //005
                                      "Slip Stop       ",      //006
                                      "ULS Stop        ",      //007
                                      "DLS Stop        ",      //008
                                      "LuLd Off Stop   ",      //009
                                      "Next Floor Stop ",      //010
                                      "InPort Error    ",      //011
                                      "Hall Door Stop  ",      //012
                                      "Car Door  Stop  ",      //013
                                      "Relvel Error    ",      //014
                                      "No Encoder Pulse",      //015
                                      "Encoder A-B Err ",      //016
                                      "ElevSpeed Error ",      //017
                                      "Speed Port Error",      //018
                                      "No Use Speed    ",      //019
                                      "Min Dec Length  ",      //020
                                      "Flr Match Error ",      //021
                                      "System Error    ",      //022
                                      "Break Mgt Error ",      //023
                                      "Break On/Off Err",      //024
                                      "Sus Error       ",      //025
                                      "Sds Error       ",      //026
									  "LuLd ChangeError",      //027
									  "Ld On Error	   ",      //028
									  "Ld Off Error    ",      //029
									  "Lu On Error     ",      //030
									  "Lu Off Error    ",      //031
									  "LuLd OnOff Error",      //032
                                      "CarDoor Jumper  ",      //033
                                      "HallDoor Jumper ",      //034
                                      "CarHall Jumper  ",      //035
                                      "036             ",      //036
                                      "037             ",      //037
                                      "038             ",      //038
                                      "039             ",      //039
                                      "040             ",      //040
                                      "041             ",      //041
                                      "042             ",      //042
                                      "FHM_Error       ",      //043
                                      "No Hall Door Sw ",      //044
                                      "No Close End    ",      //045
                                      "Over Load Stop  ",      //046
                                      "Parking Mode    ",      //047
                                      "   Fire         ",      //048
                                      "Water_Sensing   ",      //049
                                      "No Open End     ",      //050
                                      "VIP Drive       ",      //051
                                      "FHM_Mode        ",      //052
                                      "Manual Up       ",      //053
                                      "Manual Dn       ",      //054
                                      "Manual Mode     ",      //055
                                      "Safety Open     ",      //056
                                      "Open Door       ",      //057
                                      "Close Door      ",      //058
                                      "Auto Up         ",      //059
                                      "Auto Dn         ",      //060
                                      "Slow Speed      ",      //061
                                      "Wait LuLd       ",      //062
                                      "Auto Stop       ",      //063
                                      "Wait... Home !  ",      //064
                                      "Auto Ready      ",      //065
                                    };



////////temp routine
unsigned int   __attribute__((section(".usercode")))  ReadEncoder(void)
{
	return(0);
}

////////temp routine


/*******************************************************
   manual play and record
*******************************************************/


//    unsigned long xxx;

void  __attribute__((section(".usercode"))) flash_write(unsigned int pt)
{

    unsigned int write_verify,y;
    unsigned long src_data,target_data;

	if((SaveVerify == 0x55) &&  (bMoveCar==0)){

	    asm("CLRWDT");

	    DISABLE_INTERRUPTS;

	    do{
	        asm("CLRWDT");

	        write_verify=0;

	    	// Erase the 32 location row of flash
	    	erase_flash_row((unsigned int)&FlashDspCharBuf[pt]);
	    	// Write the 32 parameters back
	    	program_flash((unsigned int)&FlashDspCharBuf[pt],(unsigned int)&parameter_mirror[0]);

	        for(y=0;y<16;y++){
	            target_data=FlashDspCharBuf[pt+y].long_data;
	            src_data=parameter_mirror[y];

	            if(src_data != target_data){
	                write_verify=1;
	                break;
	            }
	        }
	    }while(write_verify);


	    ENABLE_INTERRUPTS;
	}
    asm("CLRWDT");
}





void  __attribute__((section(".usercode"))) SaveFloorCnt(void)
{
    unsigned int i;

    flash_write(FLR_01_PULSE);
    for(i=0;i<16;i++)    parameter_mirror[i] = parameter_mirror[i+16];
    flash_write(FLR_17_PULSE);
}








void  __attribute__((section(".usercode"))) flash_write_DspChar(unsigned int pt)
{
    unsigned int i;

    i=((pt/64) * 16);

    flash_write(i);
}







void  __attribute__((section(".usercode"))) F_SetupData_ReSet(unsigned int pt,unsigned int DtNm)
{
    LocalType  i,j,k;

    k=(pt % 64);

    j=((pt/64) * 16);

//    if(pt<64)   j=0;
//    else        j=16;

    for(i=0;i<16;i++){
        parameter_mirror[i]=FlashDspCharBuf[j+i].long_data;
    }

    for(i=0;i<DtNm;i++){
        b_LdTmpBufRam((unsigned long)(k+i))     = (UserDataType)(LadderBuf[C_SAVE_DATA+i]);
    }

    if((j==0) && (k==1)){
        if(cF_FLRDSPCH(0) == 'B'){
            if((LadderBuf[C_SAVE_DATA+0] == '7'))      i=0;
            else if((LadderBuf[C_SAVE_DATA+0] == '6')) i=2;
            else if((LadderBuf[C_SAVE_DATA+0] == '5')) i=4;
            else if((LadderBuf[C_SAVE_DATA+0] == '4')) i=6;
            else if((LadderBuf[C_SAVE_DATA+0] == '3')) i=8;
            else if((LadderBuf[C_SAVE_DATA+0] == '2')) i=10;
            else if((LadderBuf[C_SAVE_DATA+0] == '1')) i=12;
            else                                       j=1;
        }
        if(cF_FLRDSPCH(0) == '0'){
            if((LadderBuf[C_SAVE_DATA+0] == '1'))       i=14;
            else                                        j=1;
        }

        if(j==0){
            for(k=0;k<64;k++,i++){
                b_LdTmpBufRam(k)  = DftFlrName[i];
            }
        }
    }

    flash_write_DspChar(pt);
}






UserDataType  __attribute__((section(".usercode"))) VersionCheck(void)
{
    UserDataType    i;

    i=0;
    do{
        i++;
        if(i>10)    return(i);
    }while((cF_VER0 != VERSION_0) || (cF_VER1 != VERSION_1) || (cF_VER2 != VERSION_2));

    return(0);
}






//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
UserDataType  __attribute__((section(".usercode"))) EEP_Clear_All(void)
{
	unsigned int	i;
	for(i=0;i<16;i++)	parameter_mirror[i]=0;

	flash_write_DspChar(F_BLOCK_X0+0);
	flash_write_DspChar(F_BLOCK_X0+64);
	flash_write_DspChar(F_BLOCK_X0+128);
	flash_write_DspChar(F_BLOCK_X0+192);
	flash_write_DspChar(F_BLOCK_X0+256);

	return(0);
}



/*
UserDataType  __attribute__((section(".usercode"))) EEP_Save_All(void)
{

	unsigned int	i,j,k;
	long_field	m_data;

	parameter_mirror[0]=CurPulse;
	parameter_mirror[1]=MoveCounterx;

	m_data.intger[0]=PowerOnTime;
	m_data.byte[2]=sRamDArry[mcurfloor];
	m_data.byte[3]=CurDoorSelect;
	parameter_mirror[2]=m_data.long_data;


	m_data.byte[0]=NotRecoveryData;
	parameter_mirror[3]=m_data.long_data;

	flash_write_DspChar(F_BLOCK_X0);


	if(sRamDArry[mBefErrCntPt] != eDArry[eErrCntPt]){
		j=0;
		for(k=0;k<4;k++){
			for(i=0;i<16;i++){
				m_data.byte[0]=eDArry[j+0];
				m_data.byte[1]=eDArry[j+1];
				m_data.byte[2]=eDArry[j+2];
				m_data.byte[3]=eDArry[j+3];

				parameter_mirror[i]=m_data.long_data;
				j=(j+4);
			}
			flash_write_DspChar(F_BLOCK_X1 + (k * 64));
		}
	}


	return(0);
}



UserDataType  __attribute__((section(".usercode"))) EEP_Load_All(void)
{
	unsigned int i;

    for(i=0;i<E_END;i++){
    	eDArry[eSysErCnt+i]=cF_FLRDSPCH((unsigned long)(F_BLOCK_X1 + i));
	}

    eDArry[eErrCntPt]=cF_FLRDSPCH((unsigned long)(F_eErrCntPt));
    sRamDArry[mcurfloor]=cF_FLRDSPCH((unsigned long)(F_CurFloor));
    CurDoorSelect=cF_FLRDSPCH((unsigned long)(F_CurDoorSelect));
    NotRecoveryData=cF_FLRDSPCH((unsigned long)(F_GBR));

	CurPulse=GET_LONG_BYTE((unsigned long)(F_CurPulse0));
	MoveCounterx=GET_LONG_BYTE((unsigned long)(F_MoveCount0));
	PowerOnTime=iF_FLRDSPCH((unsigned long)(F_PowerOnTime0));

	sRamDArry[mBefErrCntPt]=eDArry[eErrCntPt];

	return(0);
}

*/
//////////////////////////////////////////////////////////////////


unsigned int  __attribute__((section(".usercode"))) PcCmdSaveChk(void)
{
	unsigned char i,j;

/*
	if(bMoveCar)	return(0);


	if((unsigned char)CmdFixFlrTime != cF_FIXFLOORTIME){
		SaveVerify=0x55;

        GetFlashDataToBufRam(F_BLOCK1);
        b_LdTmpBufRam(F_FixFloorTime)   = (unsigned char)(CmdFixFlrTime);
        flash_write_DspChar(F_BLOCK1);

		SaveVerify=0;
	}


	if( (cF_NONSERVICE0 != sRamDArry[ExtCmdFlrONOFF0]) || (cF_NONSERVICE1 != sRamDArry[ExtCmdFlrONOFF1]) || (cF_NONSERVICE2 != sRamDArry[ExtCmdFlrONOFF2]) || (cF_NONSERVICE3 != sRamDArry[ExtCmdFlrONOFF3])){
		SaveVerify=0x55;

        GetFlashDataToBufRam(F_BLOCK1);

        b_LdTmpBufRam(F_NonService0)    = sRamDArry[ExtCmdFlrONOFF0];
        b_LdTmpBufRam(F_NonService1)    = sRamDArry[ExtCmdFlrONOFF1];
        b_LdTmpBufRam(F_NonService2)    = sRamDArry[ExtCmdFlrONOFF2];
        b_LdTmpBufRam(F_NonService3)    = sRamDArry[ExtCmdFlrONOFF3];

        flash_write_DspChar(F_BLOCK1);

		SaveVerify=0;
	}


	i=0;
	j=0;
	if(bPC_FAMILY)		i=1;
	if(FamilyService)	j=1;

	if(i != j){
		SaveVerify=0x55;
        GetFlashDataToBufRam(F_BLOCK1);
		if(bPC_FAMILY)	bit_LdTmpBufRamSet(F_OnOff2,bFamilyService    	% 8);
		else			bit_LdTmpBufRamReset(F_OnOff2,bFamilyService    % 8);
        flash_write_DspChar(F_BLOCK1);
		SaveVerify=0;
	}
*/

	return(0);
}




UserDataType  __attribute__((section(".usercode"))) ReadInitSetupData(void)
{

    LocalType  i,j;

    if(VersionCheck()){
		SaveVerify = 0x55;

		EEP_Clear_All();

/*
        b_LdTmpBufRam(F_NonService0)    = Init_ALL_FF;
        b_LdTmpBufRam(F_NonService1)    = Init_ALL_FF;
        b_LdTmpBufRam(F_NonService2)    = Init_ALL_FF;
        b_LdTmpBufRam(F_NonService3)    = Init_ALL_FF;

        b_LdTmpBufRam(F_TopFlr)         = Init_TopFlr;
        b_LdTmpBufRam(F_PrkFlr)         = Init_PrkFlr;
        b_LdTmpBufRam(F_OpWtTm)         = Init_OpWtTm;
        b_LdTmpBufRam(F_OpTtTm)         = Init_OpTtTm;
        b_LdTmpBufRam(F_DirTime)        = Init_DirTime;
        b_LdTmpBufRam(F_VoiceTime)      = Init_VoiceTime;
        b_LdTmpBufRam(F_BrkMgtTime)     = Init_BrkMgtTime;


        b_LdTmpBufRam(F_FixFloor)       = Init_FixFloor;
        b_LdTmpBufRam(F_OneStopFlr)     = Init_OneStopFlr;

        b_LdTmpBufRam(F_FireSafeFlr)    = Init_FireSafeFlr;
        b_LdTmpBufRam(F_X7SafeFlr)      = Init_X7SafeFlr;
        b_LdTmpBufRam(F_WaterSafeFlr)   = Init_WaterSafeFlr;

        b_LdTmpBufRam(F_KidsKeyCnt)     = Init_KidsKeyCnt;
        b_LdTmpBufRam(F_X6SafeFlr)      = Init_X6SafeFlr;
        b_LdTmpBufRam(F_X5SafeFlr)      = Init_X5SafeFlr;


                                                 //     7 ,      6,      5,        4,         3,        2,       1,     0
        b_LdTmpBufRam(F_NcNoBit0)       = Init_ALL_FF;  //                                                                     emg
        b_LdTmpBufRam(F_NcNoBit1)       = Init_ALL_FF;  //                                                                     su1
        b_LdTmpBufRam(F_NcNoBit2)       = Init_ALL_FF;  //                                                                     rg
        b_LdTmpBufRam(F_NcNoBit3)       = Init_ALL_FF;  //                                                                     fr1
        b_LdTmpBufRam(F_NcNoBit4)       = Init_ALL_FF;  //                                                                     x0
        b_LdTmpBufRam(F_NcNoBit5)       = Init_ALL_FF;  //                                                                     fs0
        b_LdTmpBufRam(F_NcNoBit6)       = Init_ALL_FF;  //                                                                     op
        b_LdTmpBufRam(F_NcNoBit7)       = Init_ALL_FF;  //                                                                     fan
        b_LdTmpBufRam(F_NcNoBit8)       = Init_ALL_FF;  //                                                                     up

        b_LdTmpBufRam(F_StartFloor)     = START_FLOOR;


        bit_LdTmpBufRamSet  (F_OnOff0,bFhmOnOff             % 8);
        bit_LdTmpBufRamSet  (F_OnOff0,bDoorJumperOff        % 8);
        bit_LdTmpBufRamReset(F_OnOff0,bEncoderPhase         % 8);
        bit_LdTmpBufRamSet  (F_OnOff0,bWardPriority         % 8);
        bit_LdTmpBufRamReset(F_OnOff0,bNonStop              % 8);
        bit_LdTmpBufRamSet  (F_OnOff0,bOpenTotalOnOff       % 8);
        bit_LdTmpBufRamSet  (F_OnOff0,bCloseTotalOnOff      % 8);
        bit_LdTmpBufRamSet  (F_OnOff0,bOpenWaitToTalOnOff   % 8);

        bit_LdTmpBufRamSet  (F_OnOff1,bCarDoor1Use      	% 8);
        bit_LdTmpBufRamReset(F_OnOff1,bCarDoor2Use      	% 8);
        bit_LdTmpBufRamSet  (F_OnOff1,bHoleDoor1Use     	% 8);
        bit_LdTmpBufRamReset(F_OnOff1,bHoleDoor2Use     	% 8);
        bit_LdTmpBufRamReset(F_OnOff1,bSubSafety        	% 8);
        bit_LdTmpBufRamReset(F_OnOff1,bBrkMgtUse        	% 8);
        bit_LdTmpBufRamSet  (F_OnOff1,bEncoderUse       	% 8);
        bit_LdTmpBufRamReset(F_OnOff1,bRelevelOnOff     	% 8);

        bit_LdTmpBufRamSet  (F_OnOff2,bOilLopeTypeChk   	% 8);
        bit_LdTmpBufRamReset(F_OnOff2,bOnlyOneCall      	% 8);
        bit_LdTmpBufRamReset(F_OnOff2,bManualSusChk     	% 8);
        bit_LdTmpBufRamReset(F_OnOff2,bBrkOpenUse       	% 8);
        bit_LdTmpBufRamReset(F_OnOff2,bKidsKeyChkUse    	% 8);
        bit_LdTmpBufRamReset(F_OnOff2,bCarKeyFirstService  	% 8);
        bit_LdTmpBufRamReset(F_OnOff2,bFamilyService    	% 8);
        bit_LdTmpBufRamReset(F_OnOff2,bOpenOutSet    		% 8);


        bit_LdTmpBufRamReset(F_OnOff3,bSafetyValidLR    	% 8);
        bit_LdTmpBufRamReset(F_OnOff3,bFloorSel             % 8);
        bit_LdTmpBufRamReset(F_OnOff3,bRunOpenSetClear      % 8);
        bit_LdTmpBufRamSet(F_OnOff3,bManWorkingChk      	% 8);

        SWPAB   = 0;    //****


        b_LdTmpBufRam(F_FanTime)        = Init_FanTime;        //3 sec
        b_LdTmpBufRam(F_LightTime)      = Init_LightTime;       //99 min
        b_LdTmpBufRam(F_FixFloorTime)   = Init_FixFloorTime;
        b_LdTmpBufRam(F_NextFlrTime)    = Init_NextFlrTime;
        b_LdTmpBufRam(F_LuLdOffTime)    = Init_LuLdOffTime;
        b_LdTmpBufRam(F_ReOpTime)       = Init_ReOpTime;
        b_LdTmpBufRam(F_OpClWaitTime)   = Init_OpClWTime;
        b_LdTmpBufRam(F_DrJmpChkTime)   = Init_DrJmpChkTime;

        b_LdTmpBufRam(F_StTm1)          = Init_StTm1;
        b_LdTmpBufRam(F_StTm2)          = Init_StTm2;
        b_LdTmpBufRam(F_StTm3)          = Init_StTm3;
        b_LdTmpBufRam(F_StTm4)          = Init_StTm4;
        b_LdTmpBufRam(F_StTm5)          = Init_StTm5;



        b_LdTmpBufRam(F_RunOffTime)      = Init_RunOff_Time;
        b_LdTmpBufRam(F_Bk1OffTime)      = Init_Bk1Off_Time;
        b_LdTmpBufRam(F_Bk2OffTime)      = Init_Bk2Off_Time;
        b_LdTmpBufRam(F_UDOffTime)       = Init_UDOff_Time;
        b_LdTmpBufRam(F_P4OffTime)       = Init_P4Off_Time;


        b_LdTmpBufRam(F_SolOnTime)       = Init_SolOn_Time;
        b_LdTmpBufRam(F_DoorWaitTime)    = Init_DoorWait_Time;

#ifdef  TEST_SIMULATION
        b_LdTmpBufRam(F_TopFlr)         = 31;

        bit_LdTmpBufRamReset  (F_OnOff0,bDoorJumperOff        % 8);
        bit_LdTmpBufRamReset(F_OnOff3,bManWorkingChk      % 8);
#endif

        flash_write_DspChar(F_TopFlr);


/////////////////////////////////////////////////////////
		for(i=0;i<32;i++){
        	b_LdTmpBufRam(F_0506+i)           = DISABLE_FLR;
		}

        flash_write_DspChar(F_BLOCK3);
//////////////////////////////////////////////////////////////

*/



        b_LdTmpBufRam(EMG_PORT)         =SILK_EMG   | BIT_NORMAL_OPEN;          //1     64
        b_LdTmpBufRam(PRK_PORT)         =SILK_PRK   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(AUTO_PORT)        =SILK_AUTO  | BIT_NORMAL_OPEN;         //1
        b_LdTmpBufRam(U_B_PORT)         =SILK_U_B   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(D_B_PORT)         =SILK_D_B   | BIT_NORMAL_OPEN;          //1     65
        b_LdTmpBufRam(OLS_PORT)         =SILK_OLS   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(GS_PORT)          =SILK_GS    | BIT_NORMAL_OPEN;           //1
        b_LdTmpBufRam(DS_PORT)          =SILK_DS    | BIT_NORMAL_OPEN;           //1

        b_LdTmpBufRam(SU1_PORT)         =SILK_SU1   | BIT_NORMAL_OPEN;          //1     66
        b_LdTmpBufRam(SD1_PORT)         =SILK_SD1   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(SFT_PORT)         =SILK_SFT   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(OVL_PORT)         =SILK_OVL   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(ULS_PORT)         =SILK_ULS   | BIT_NORMAL_OPEN;          //1     67
        b_LdTmpBufRam(DLS_PORT)         =SILK_DLS   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(LU_PORT)          =SILK_LU    | BIT_NORMAL_OPEN;           //1
        b_LdTmpBufRam(LD_PORT)          =SILK_LD    | BIT_NORMAL_OPEN;           //1

        b_LdTmpBufRam(RG_PORT)          =SILK_GR    | BIT_NORMAL_OPEN;           //1     68
        b_LdTmpBufRam(BAT_PORT)         =SILK_BAT   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(PASS_PORT)        =SILK_PASS  | BIT_NORMAL_OPEN;         //1
        b_LdTmpBufRam(FIRE_PORT)        =SILK_FIRE  | BIT_NORMAL_OPEN;         //1
        b_LdTmpBufRam(WATER_PORT)       =SILK_WATER | BIT_NORMAL_OPEN;        //1     69
        b_LdTmpBufRam(FULL_PORT)        =SILK_FULL  | BIT_NORMAL_OPEN;         //1
        b_LdTmpBufRam(MM_PORT)          =SILK_MM    | BIT_NORMAL_OPEN;         //1
        b_LdTmpBufRam(FHM_PORT)         =SILK_FHM   | BIT_NORMAL_OPEN;          //1

        b_LdTmpBufRam(FR1_PORT)         =SILK_FR1   | BIT_NORMAL_OPEN;          //1     70
        b_LdTmpBufRam(FR2_PORT)         =SILK_FR2   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(GS2_PORT)         =SILK_GS2   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(DS2_PORT)         =SILK_DS2   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(SU2_PORT)         =SILK_SU2   | BIT_NORMAL_OPEN;          //1     71
        b_LdTmpBufRam(SD2_PORT)         =SILK_SD2   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(BM_PORT)          =SILK_BM    | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(INV_PORT)         =SILK_INV   | BIT_NORMAL_OPEN;          //1

        b_LdTmpBufRam(X0_PORT)          =SILK_X0    | BIT_NORMAL_OPEN;           //1     72
        b_LdTmpBufRam(X1_PORT)          =SILK_X1    | BIT_NORMAL_OPEN;           //1
        b_LdTmpBufRam(X2_PORT)          =SILK_X2    | BIT_NORMAL_OPEN;           //1
        b_LdTmpBufRam(X3_PORT)          =SILK_X3    | BIT_NORMAL_OPEN;           //1
        b_LdTmpBufRam(X4_PORT)          =SILK_X4    | BIT_NORMAL_OPEN;           //1     73
        b_LdTmpBufRam(X5_PORT)          =SILK_X5    | BIT_NORMAL_OPEN;           //1
        b_LdTmpBufRam(X6_PORT)          =SILK_X6    | BIT_NORMAL_OPEN;           //1
        b_LdTmpBufRam(X7_PORT)          =SILK_X7    | BIT_NORMAL_OPEN;           //1

        b_LdTmpBufRam(FS0_PORT)         =SILK_FS0   | BIT_NORMAL_OPEN;          //1     74
        b_LdTmpBufRam(FS1_PORT)         =SILK_FS1   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(FS2_PORT)         =SILK_FS2   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(FS3_PORT)         =SILK_FS3   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(FS4_PORT)         =SILK_FS4   | BIT_NORMAL_OPEN;          //1     75
        b_LdTmpBufRam(DER_PORT)         =SILK_DER   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(FID_PORT)         =SILK_FID   | BIT_NORMAL_OPEN;          //1
        b_LdTmpBufRam(UND_PORT)         =SILK_UND   | BIT_NORMAL_OPEN;          //1


        b_LdTmpBufRam(F_SubDoorFlr1)    = Init_SubDoorFlr1;
        b_LdTmpBufRam(F_SubDoorFlr2)    = Init_SubDoorFlr1;
        b_LdTmpBufRam(F_SubDoorFlr3)    = Init_SubDoorFlr1;
        b_LdTmpBufRam(F_SubDoorFlr4)    = Init_SubDoorFlr1;
        b_LdTmpBufRam(F_SubDoorFlr5)    = Init_SubDoorFlr1;
        b_LdTmpBufRam(F_SubDoorFlr6)    = Init_SubDoorFlr1;
        b_LdTmpBufRam(F_SubDoorFlr7)    = Init_SubDoorFlr1;
        b_LdTmpBufRam(F_SubDoorFlr8)    = Init_SubDoorFlr1;



        flash_write_DspChar(EMG_PORT);

/////////////////////////////////////////////////////////

        b_LdTmpBufRam(FAN_PORT)         =SILK_FAN   | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(LIT_PORT)         =SILK_LIGHT | BIT_NORMAL_OPEN;  //
        b_LdTmpBufRam(BUZ_PORT)         =SILK_BUZ   | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(BELL_PORT)        =SILK_BELL  | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(RST_PORT)         =SILK_RST   | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(ERR_PORT)         =SILK_ERR   | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(HOP_PORT)         =SILK_HOP   | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(HCL_PORT)         =SILK_HCL   | BIT_NORMAL_OPEN;

        b_LdTmpBufRam(OP_PORT)          =SILK_OP    | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(CL_PORT)          =SILK_CL    | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(S_OP_PORT)        =SILK_S_OP  | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(S_CL_PORT)        =SILK_S_CL  | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(BK2_PORT)         =SILK_BK2   | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(D_S_PORT)         =SILK_D_S   | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(GBR_PORT)         =SILK_GBR   | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(BK1_PORT)         =SILK_BK1   | BIT_NORMAL_OPEN;

        b_LdTmpBufRam(UP_PORT)          =SILK_UP    | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(DN_PORT)          =SILK_DN    | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(P1_PORT)          =SILK_P1    | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(P2_PORT)          =SILK_P2    | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(P3_PORT)          =SILK_P3    | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(P4_PORT)          =SILK_P4    | BIT_NORMAL_OPEN;
        b_LdTmpBufRam(DAC_PORT)         =SILK_DAC   | BIT_NORMAL_OPEN;

        flash_write_DspChar(FAN_PORT);
////////////////////////////////////////////////


///////////////////////////////////////////////
        b_LdTmpBufRam(F_Ver0)           = VERSION_0;
        b_LdTmpBufRam(F_Ver1)           = VERSION_1;
        b_LdTmpBufRam(F_Ver2)           = VERSION_2;


        b_LdTmpBufRam(F_Bd_Id)          = (IO | MAN_USE);



        b_LdTmpBufRam(F_Passward0)      = '0';
        b_LdTmpBufRam(F_Passward1)      = '0';
        b_LdTmpBufRam(F_Passward2)      = '0';
        b_LdTmpBufRam(F_Passward3)      = '0';

        i_LdTmpBufRam(F_SerialNm10)     = 0;

        b_LdTmpBufRam(F_ErrStopCnt)     = 0xff;
        b_LdTmpBufRam(F_NormalDsp)      = 0x0;
        b_LdTmpBufRam(F_UserLamp1)      = 8;
        b_LdTmpBufRam(F_UserLamp2)      = 1;
        b_LdTmpBufRam(F_UserLamp3)      = 6;
        b_LdTmpBufRam(F_UserLamp4)      = 3;
        b_LdTmpBufRam(F_GroupNm)        = 0;
        b_LdTmpBufRam(F_LocalNm)        = 0;
        b_LdTmpBufRam(F_PcbType)        = 0;

//        b_LdTmpBufRam(F_EncoderRate)    = 1;
        i_LdTmpBufRam(F_StopPulse0)     = 1000;

        b_LdTmpBufRam(F_year)           = 0;
        b_LdTmpBufRam(F_month)          = 0;
        b_LdTmpBufRam(F_day)            = 0;

        b_LdTmpBufRam(F_Company)        = COMPANY;
        b_LdTmpBufRam(F_Airconadr)      = 0xc0;

        flash_write_DspChar(F_BLOCK0);











///////////////////////////////////////////////
        //  P1  P2  P3  P4
        //  0   0   0   0           :NONE_USE_SPD
        //  1   0   0   0           :P1
        //  0   1   0   0           :P2
        //  1   1   0   0           :P1P2
        //  0   0   1   0           :P3
        //  1   0   1   0           :P1P3
        //  0   1   1   0           :P2P3
        //  1   1   1   0           :P1P2P3
        //  0   0   0   1           :P4
        //  1   0   0   1           :P1P4
        //  0   1   0   1           :P2P4
        //  1   1   0   1           :P1P2P4
        //  0   0   1   1           :P3P4
        //  1   0   1   1           :P1P3P4
        //  0   1   1   1           :P2P3P4
        //  1   1   1   1           :P1P2P3P4

///////////////////////////////////////////////

/*
        b_LdTmpBufRam(F_ManualSpeed)    = P1P2_SPD;
        b_LdTmpBufRam(F_BatterySpeed)   = NONE_USE_SPD;
        b_LdTmpBufRam(F_DecreaseSpeed)  = P2_SPD;
        b_LdTmpBufRam(F_FhmSpeed)       = NONE_USE_SPD;
        b_LdTmpBufRam(F_Etc1Speed)      = NONE_USE_SPD;
        b_LdTmpBufRam(F_Etc2Speed)      = NONE_USE_SPD;
        b_LdTmpBufRam(F_Speed30)        = NONE_USE_SPD;
        b_LdTmpBufRam(F_Speed45)        = P3_SPD;
        b_LdTmpBufRam(F_Speed60)        = P1_SPD;
        b_LdTmpBufRam(F_Speed90)        = NONE_USE_SPD;
        b_LdTmpBufRam(F_Speed105)       = NONE_USE_SPD;
        b_LdTmpBufRam(F_Speed120)       = NONE_USE_SPD;
        b_LdTmpBufRam(F_Speed150)       = NONE_USE_SPD;
        b_LdTmpBufRam(F_Speed180)       = NONE_USE_SPD;
        b_LdTmpBufRam(F_Speed210)       = NONE_USE_SPD;
        b_LdTmpBufRam(F_ElevSpeed)      = SPEED_60;
        b_LdTmpBufRam(F_LimitSpeed)     = SPEED_210;
        b_LdTmpBufRam(F_Su1Sd1_Velocity)= 90;      //mpm
        b_LdTmpBufRam(F_Su2Sd2_Velocity)= 90;      //mpm
        b_LdTmpBufRam(F_X0X1_Velocity)  = 210;     //mpm

        flash_write_DspChar(F_ManualSpeed);

*/

       	b_LdTmpBufRam(F_MainVolt) 		= 0x00;
       	b_LdTmpBufRam(F_MainCurrent)    = 0x00;
       	b_LdTmpBufRam(F_TrVolt)    		= 0x00;
       	b_LdTmpBufRam(F_TrCurrent)    	= 0x00;

       	b_LdTmpBufRam(F_SystemMode) = 0x00;	// CDS Mode
       	b_LdTmpBufRam(F_On_Percent) = 50;	// 50   %
       	b_LdTmpBufRam(F_On_Time)    = 2;	// 1 	sec
       	b_LdTmpBufRam(F_Day_S_H)    = 0x06;	// 06  hour
       	b_LdTmpBufRam(F_Day_S_M)    = 0x00;	// 00  min
       	b_LdTmpBufRam(F_Day_E_H)    = 0x18;	// 18  hour
       	b_LdTmpBufRam(F_Day_E_M)    = 0x00;	// 00  min
		b_LdTmpBufRam(F_MasterCDS)  = 0x00;

       	b_LdTmpBufRam(F_0Count)     = 0;  //0;	//  4
       	b_LdTmpBufRam(F_0Amp)     	= 0;	//  5
       	b_LdTmpBufRam(F_0Mode)     	= 0x0;	//  blinck , night
       	b_LdTmpBufRam(F_0Extra)     = 0x0;	//  0

       	b_LdTmpBufRam(F_1Count)     = 0;	//  4
       	b_LdTmpBufRam(F_1Amp)     	= 0;	//  5
       	b_LdTmpBufRam(F_1Mode)     	= 0x0;	//  blinck , night
       	b_LdTmpBufRam(F_1Extra)     = 0x0;	//  0

       	b_LdTmpBufRam(F_2Count)     = 0;	//  4
       	b_LdTmpBufRam(F_2Amp)     	= 0;	//  5
       	b_LdTmpBufRam(F_2Mode)     	= 0x0;	//  blinck , night
       	b_LdTmpBufRam(F_2Extra)     = 0x0;	//  0

       	b_LdTmpBufRam(F_3Count)     = 0;	//  4
       	b_LdTmpBufRam(F_3Amp)     	= 0;	//  5
       	b_LdTmpBufRam(F_3Mode)     	= 0x0;	//  blinck , night
       	b_LdTmpBufRam(F_3Extra)     = 0x0;	//  0


       	b_LdTmpBufRam(F_4Count)     = 0;	//  4
       	b_LdTmpBufRam(F_4Amp)     	= 0;	//  5
       	b_LdTmpBufRam(F_4Mode)     	= 0x0;	//  blinck , night
       	b_LdTmpBufRam(F_4Extra)     = 0x0;	//  0

       	b_LdTmpBufRam(F_5Count)     = 0;	//  4
       	b_LdTmpBufRam(F_5Amp)     	= 0;	//  5
       	b_LdTmpBufRam(F_5Mode)     	= 0x0;	//  blinck , night
       	b_LdTmpBufRam(F_5Extra)     = 0x0;	//  0

       	b_LdTmpBufRam(F_6Count)     = 0;	//  4
       	b_LdTmpBufRam(F_6Amp)     	= 0;	//  5
       	b_LdTmpBufRam(F_6Mode)     	= 0x0;	//  blinck , night
       	b_LdTmpBufRam(F_6Extra)     = 0x0;	//  0

       	b_LdTmpBufRam(F_7Count)     = 0;	//  4
       	b_LdTmpBufRam(F_7Amp)     	= 0;	//  5
       	b_LdTmpBufRam(F_7Mode)     	= 0x0;	//  blinck , night
       	b_LdTmpBufRam(F_7Extra)     = 0x0;	//  0

       	b_LdTmpBufRam(F_8Count)     = 0;	//  4
       	b_LdTmpBufRam(F_8Amp)     	= 0;	//  5
       	b_LdTmpBufRam(F_8Mode)     	= 0x0;	//  blinck , night
       	b_LdTmpBufRam(F_8Extra)     = 0x0;	//  0

       	b_LdTmpBufRam(F_9Count)     = 0;	//  4
       	b_LdTmpBufRam(F_9Amp)     	= 0;	//  5
       	b_LdTmpBufRam(F_9Mode)     	= 0x0;	//  blinck , night
       	b_LdTmpBufRam(F_9Extra)     = 0x0;	//  0



        flash_write_DspChar(F_BLOCK1);

	}


//	EEP_Load_All();

	SaveVerify = 0x0;

   	return(0);
}




void    __attribute__((section(".usercode"))) HextoASCIIByte(void)
{

    LocalType   i;

    i=(src & 0xf0) >> 4;
    if(i<0x0a)   i= i + '0';
    else         i= i + '7';
    firstdata=i;

    i=(src & 0x0f);
    if(i<0x0a)   i= i + '0';
    else         i= i + '7';
    seconddata=i;
}


/*

void  __attribute__((section(".usercode"))) WhyStop(void)
{
	if(bInspect){
        sRamDArry[mSysStatus]=sINSPECT;
	}

	else if(bsInvertErr){
		if(sRamDArry[mSysStatus] >= sInvertErr)
		sRamDArry[mSysStatus]=sInvertErr;
   	}

	else if(bsInvCurNext){
		if(sRamDArry[mSysStatus] >= sI_C_N_FLR)
		sRamDArry[mSysStatus]=sI_C_N_FLR;
   	}

	else if(bsEmergency){
		if(sRamDArry[mSysStatus] >= sEMERGENCY)
		sRamDArry[mSysStatus]=sEMERGENCY;
	}

	else if(bsLope){
		if(sRamDArry[mSysStatus] >= sLOPE_BRK)
		sRamDArry[mSysStatus]=sLOPE_BRK;
	}

	else if(bsSlip){
		if(sRamDArry[mSysStatus] >= sSLIP)
		sRamDArry[mSysStatus]=sSLIP;
	}

	else if(bsUls){
		if(sRamDArry[mSysStatus] >= sULS)
		sRamDArry[mSysStatus]=sULS;
	}

	else if(bsDls){
		if(sRamDArry[mSysStatus] >= sDLS)
		sRamDArry[mSysStatus]=sDLS;
	}


	else if(bRelevelErr){
		if(sRamDArry[mSysStatus] >= sRelevelError)
		sRamDArry[mSysStatus]=sRelevelError;
	}

	else if(bsHdsRunOff){
		if(sRamDArry[mSysStatus] >= sHDS_RUN_OFF)
		sRamDArry[mSysStatus]=sHDS_RUN_OFF;
	}

	else if(bsCleRunOff){
		if(sRamDArry[mSysStatus] >= sCLE_RUN_OFF)
		sRamDArry[mSysStatus]=sCLE_RUN_OFF;
	}

	else if(bsHdsNoOn){
		if(sRamDArry[mSysStatus] > sHDS_NO_ON)
		sRamDArry[mSysStatus]=sHDS_NO_ON;
	}

	else if(bsCleNoOn){
		if(sRamDArry[mSysStatus] > sCLE_NO_ON)
		sRamDArry[mSysStatus]=sCLE_NO_ON;
	}

	else if(bsNextFloor){
		if(sRamDArry[mSysStatus] > sNEXT_FLOOR)
		sRamDArry[mSysStatus] = sNEXT_FLOOR;
	}

	else if(bsLuLdNoOff){
		if(sRamDArry[mSysStatus] > sLULD_NO_OFF)
		sRamDArry[mSysStatus] = sLULD_NO_OFF;
	}


	else if(bMotor_Overheat){
		if(sRamDArry[mSysStatus] > sMOTOR_OVERHEAT)
		sRamDArry[mSysStatus]=sMOTOR_OVERHEAT;
	}


	else if(bDoorJumper){
		if(sRamDArry[mSysStatus] >= sCarDoor_Jumper)
		sRamDArry[mSysStatus]=(sCarDoor_Jumper + DoorJumperNm);
	}

	else if(bEncoderErr){
		if(sRamDArry[mSysStatus] >= sEncoderErr)
		sRamDArry[mSysStatus]=sEncoderErr;
	}

	else if(bEncoderABErr){
		if(sRamDArry[mSysStatus] >= sEncoderABErr)
		sRamDArry[mSysStatus]=sEncoderABErr;
	}

	else if(bEqualFloorError){
		if(sRamDArry[mSysStatus] >= sEqualFloorError)
		sRamDArry[mSysStatus]=sEqualFloorError;
	}

	else if(bsBreakMgtOpen){
		if(sRamDArry[mSysStatus] >= sBreakMgtOpen)
		sRamDArry[mSysStatus] = sBreakMgtOpen;
	}

	else if(bsBreakOpen){
		if(sRamDArry[mSysStatus] >= sBreakOpen)
		sRamDArry[mSysStatus] = sBreakOpen;
	}

	else if(bsSusErr){
		if(sRamDArry[mSysStatus] >= sSusErr)
		sRamDArry[mSysStatus] = sSusErr;
	}

	else if(bsSdsErr){
		if(sRamDArry[mSysStatus] >= sSdsErr)
		sRamDArry[mSysStatus] = sSdsErr;
	}


	else if(bsLuOrLdErr && (LuLdErrNm > 0)){
		if(sRamDArry[mSysStatus] >= (sLuOrLdErr0 + LuLdErrNm-1))
		sRamDArry[mSysStatus]=(sLuOrLdErr0 + LuLdErrNm-1);
	}


	else if(NoStart){
		switch(NoStart){
            case    1:
			    if(sRamDArry[mSysStatus] > sSpeedSetError)
				sRamDArry[mSysStatus]=sSpeedSetError;
                break;
            case    2:
			    if(sRamDArry[mSysStatus] > sSpeedPortError)
				sRamDArry[mSysStatus]=sSpeedPortError;
                break;
            case    3:
			    if(sRamDArry[mSysStatus] > sNoUseSpeed)
				sRamDArry[mSysStatus]=sNoUseSpeed;
                break;
            case    4:
			    if(sRamDArry[mSysStatus] > sMinLengthErr)
				sRamDArry[mSysStatus]=sMinLengthErr;
                break;
            default:
			    if(sRamDArry[mSysStatus] > sSystemErr)
				sRamDArry[mSysStatus]=sSystemErr;
                break;
		}
	}

    SegError=sRamDArry[mSysStatus];
}




UserDataType	__attribute__((section(".usercode"))) SaveEebuf(UserDataType  ErrName)
{
	LocalType i;

	i=(eDArry[eErrCntPt] % (ERR_SAVE_CNT));

	i=(i * ERR_SAVE_WIDE);

	eDArry[eSysErCnt + i + 0] = sRamDArry[Month];
    eDArry[eSysErCnt + i + 1] = sRamDArry[Day];
    eDArry[eSysErCnt + i + 2] = sRamDArry[mcurfloor];
    eDArry[eSysErCnt + i + 3] = ErrName;
    eDArry[eSysErCnt + i + 4] = sRamDArry[Hour];
    eDArry[eSysErCnt + i + 5] = sRamDArry[Minuate];

	eDArry[eErrCntPt]++;
	if(eDArry[eErrCntPt] > 0xff){
     	eDArry[eErrCntPt]=0;
	}

	return(0);
}



UserDataType  __attribute__((section(".usercode"))) ErrSave(void)
{
    if(sRamDArry[mDoorSeq] == READY_ELEVATOR)   return(1);


   	if(bsCleNoOn){								//44
      	if(!bBefbsCleNoOn){
   			SaveEebuf(sCLE_NO_ON);
            bBefbsCleNoOn=bsCleNoOn;
      	}
   	}

   	if(bsHdsNoOn){								//43
      	if(!bBefbsHdsNoOn){
   			SaveEebuf(sHDS_NO_ON);
        	bBefbsHdsNoOn=bsHdsNoOn;
      	}
   	}

   	if(bsSdsErr){								//26
      	if(!bBefbsSdsErr){
			SaveEebuf(sSdsErr);
        	bBefbsSdsErr=bsSdsErr;
      	}
   	}

   	if(bsSusErr){								//25
      	if(!bBefbsSusErr){
			SaveEebuf(sSusErr);
        	bBefbsSusErr=bsSusErr;
      	}
   	}

   	if(bsBreakOpen){							//24
      	if(!bBefbsBreakOpen){
			SaveEebuf(sBreakOpen);
        	bBefbsBreakOpen=bsBreakOpen;
      	}
   	}

   	if(bsBreakMgtOpen){							//23
      	if(!bBefbsBreakMgtOpen){
			SaveEebuf(sBreakMgtOpen);
        	bBefbsBreakMgtOpen=bsBreakMgtOpen;
      	}
   	}


   	if(bEncoderABErr){							//16
      	if(!bBefbEncoderABErr){
			SaveEebuf(sEncoderABErr);
        	bBefbEncoderABErr=bEncoderABErr;
      	}
   	}


   	if(bEncoderErr){							//15
      	if(!bBefbEncoderErr){
			SaveEebuf(sEncoderErr);
        	bBefbEncoderErr=bEncoderErr;
      	}
   	}


   	if(bMotor_Overheat){
      	if(!bBefbMotor_Overheat){
   			SaveEebuf(sMOTOR_OVERHEAT);
        	bBefbMotor_Overheat=bMotor_Overheat;
      	}
   	}


	if(bRelevelErr){							//13
		if(!bBefbRelevelErr){
			SaveEebuf(sRelevelError);
        	bBefbRelevelErr=bRelevelErr;
      	}
   	}


	if(bsCleRunOff){							//13
		if(!bBefbsCleRunOff){
			SaveEebuf(sCLE_RUN_OFF);
        	bBefbsCleRunOff=bsCleRunOff;
      	}
   	}


   	if(bsHdsRunOff){							//12
      	if(!bBefbsHdsRunOff){
			SaveEebuf(sHDS_RUN_OFF);
        	bBefbsHdsRunOff=bsHdsRunOff;
      	}
   	}


   	if(bsNextFloor){							//10
      	if(!bBefbsNextFloor){
			SaveEebuf(sNEXT_FLOOR);
        	bBefbsNextFloor=bsNextFloor;
      	}
   	}


   	if(bsLuLdNoOff){							//9
      	if(!bBefbsLuLdNoOff){
   			SaveEebuf(sLULD_NO_OFF);
        	bBefbsLuLdNoOff=bsLuLdNoOff;
      	}
   	}


   	if(bsDls){									//8
      	if(!bBefbsDls){
			SaveEebuf(sDLS);
        	bBefbsDls=bsDls;
      	}
   	}

   	if(bsUls){									//7
      	if(!bBefbsUls){
			SaveEebuf(sULS);
        	bBefbsUls=bsUls;
      	}
   	}


   	if(bsSlip){									//6
      	if(!bBefbsSlip){
      		SaveEebuf(sSLIP);
        	bBefbsSlip=bsSlip;
      	}
   	}


   	if(bsLope){									//4
      	if(!bBefbsLope){
      		SaveEebuf(sLOPE_BRK);
        	bBefbsLope=bsLope;
      	}
   	}

   	if(bsEmergency){							//3
      	if(!bBefbsEmergency){
			SaveEebuf(sEMERGENCY);
        	bBefbsEmergency=bsEmergency;
      	}
   	}

   	if(bsInvCurNext){							//2
      	if(!bBefbsInvCurNext){
			SaveEebuf(sI_C_N_FLR);
        	bBefbsInvCurNext=bsInvCurNext;
      	}
   	}

   	if(bsInvertErr){							//1
      	if(!bBefbsInvertErr){
			SaveEebuf(sInvertErr);
        	bBefbsInvertErr=bsInvertErr;
      	}
   	}


   	if(bsLuOrLdErr && (LuLdErrNm > 0)){							  //
      	if(!bBefsLuOrLdErr){
			SaveEebuf(sLuOrLdErr0 + LuLdErrNm-1);
        	bBefsLuOrLdErr=bsLuOrLdErr;
      	}
   	}



   	if(bDoorJumper){							  //
      	if(!bBefDoorJumper){
			SaveEebuf(sCarDoor_Jumper + DoorJumperNm);
        	bBefDoorJumper=bDoorJumper;
      	}
   	}


    return(0);
}


void  __attribute__((section(".usercode"))) ErrFlagClr(void)
{
	if(bErrClearOnce){
		bBefbsSlip=0;
		bBefbsCleRunOff=0;
		bBefbsHdsRunOff=0;
		bBefbsEmergency=0;
		bBefbsNextFloor=0;
		bBefbsHdsNoOn=0;
		bBefbsCleNoOn=0;
		bBefbsLuLdNoOff=0;
	    bBefbsLope=0;
	    bBefbsInvCurNext=0;
		bBefbsDls=0;
		bBefbsUls=0;
	    bBefbEncoderErr=0;
	    bBefbEncoderABErr=0;
	    bBefbsInvertErr=0;
	    bBefbsSdsErr=0;
	    bBefbsSusErr=0;
		bBefDoorJumper=0;
		bBefbMotor_Overheat=0;

		bErrClearOnce=0;
	}
}




void  __attribute__((section(".usercode"))) SystemErrSave(void)
{
	unsigned char kk;

	if(bSaveErrCnt){
		ErrSave();
		bErrClearOnce=1;
	}
    else    ErrFlagClr();
}


*/

















void  __attribute__((section(".usercode"))) Serial2Check(void)
{
    if(RxStatus2==RX_GOOD){
		if(bExportData || bImportData){
			NewLoaderChk();
		}
        LadderBuf[C_SAVE_DATA+0]=RxBuffer2[2];

		if(RxBuffer2[1] == '0'){
            bPasswardOk=1;
            bAllMenu=1;
			bCompanyCtl=1;
            bUserOn=1;
			bLoaderActive=1;
		}

		if(RxBuffer2[1] == cF_COMPANY)	bLoaderActive=1;


        NewFlashData();
        RxStatus2=STX_CHK;
    }

    if((RxStatus2 != TX_SET) && (SerialTime2 > 10)){
        if(LadderTime>100){
            LadderBuf[C_SAVE_DATA+0]='N';
            NewFlashData();
            LadderTime=0;
        }
        LdDataTx();
        SerialTime2=0;
    }
}



/*
void  __attribute__((section(".usercode"))) ClrUpDnWard(void)
{
    S2_DN1=0;
    S1_UP1=0;                                                     //up
    S3_SHIFT1=0;

    bUpWard=0;
    bDnWard=0;

	sRamDArry[DEST_FLR] = (sRamDArry[DEST_FLR] & ONLY_FLR);
}


void  __attribute__((section(".usercode"))) UpWard(void)
{
   S2_DN1=0;
   S1_UP1=1;
   bUpWard=1;
   bDnWard=0;

//	sRamDArry[DEST_FLR]= (sRamDArry[DEST_FLR] & ~DN_READY);
//	sRamDArry[DEST_FLR]= (sRamDArry[DEST_FLR] |  UP_READY);

   if(bMoveCar && !bUnd)    S3_SHIFT1=1; //shift
   else                     S3_SHIFT1=0; //shift
}


void  __attribute__((section(".usercode"))) DnWard(void)
{
   S2_DN1=1;
   S1_UP1=0;
   bUpWard=0;
   bDnWard=1;

//	sRamDArry[DEST_FLR]= (sRamDArry[DEST_FLR] & ~UP_READY);
//	sRamDArry[DEST_FLR]= (sRamDArry[DEST_FLR] |  DN_READY);

   if(bMoveCar && !bUnd)    S3_SHIFT1=1; //shift
   else                     S3_SHIFT1=0; //shift
}



UserDataType  __attribute__((section(".usercode"))) ReadyAndNoKeyCheck(void)
{
	if((sRamDArry[mDoorSeq] == READY_ELEVATOR) &&
        (sRamDArry[mHighFloor] == 0) &&
        (sRamDArry[mLowFloor] == 0)){
     	return(0);
	}
	else	return(1);
}



void  __attribute__((section(".usercode"))) Virtual_Floor_Set(UserDataType vFloor)
{

	if(vFloor <= cF_TOPFLR){
		if(sRamDArry[mcurfloor] > (unsigned char)(vFloor)){
			sRamDArry[mLowFloor] = (unsigned char)(vFloor | CAR_READY);
		}
		else if(sRamDArry[mcurfloor] < (unsigned char)(vFloor)){
			sRamDArry[mHighFloor] = (unsigned char)(vFloor | CAR_READY);
		}
	}
}



unsigned int  __attribute__((section(".usercode"))) AutoRunReady(void)
{
    if(!bAuto  || S3_PARKING1   || S2_FIRE1 || bWaterSen || S1_POWER_FAIL)      return(0);
    else if(bCarErr || bCarStopNoRun || bCarStop || bManualStop)                return(0);
    else                                                                        return(1);
}



unsigned int  __attribute__((section(".usercode"))) OneStopKey(void)
{
    LocalType     cTime;
    LocalType     xx0,xx1,xx2;

    unsigned 	char  TmpOneStopFlr;


	if( !(cF_ONESTOP_FLOOR & UPDN_READY))									return(0);


	TmpOneStopFlr = (cF_ONESTOP_FLOOR & (~(UPDN_READY)) );

  	if(S3_VIP1)																return(0);
    if(TmpOneStopFlr >= cF_TOPFLR)                                          return(0);
    if(!AutoRunReady())                                                     return(0);
    if(bTestKey)                                                            return(0);
	if( !bUpWard && !bDnWard)												return(0);
	if( (sRamDArry[mAckStopFloor] & ONLY_FLR) == (TmpOneStopFlr))			return(0);
    if((sRamDArry[mcurfloor] == TmpOneStopFlr))                           	return(0);

	if(bUpWard){
        if( !(cF_ONESTOP_FLOOR & UP_READY))           						return(0);

        if( !(sRamDArry[mHighFloor] & UPDN_CAR_READY))            			return(0);

        if((sRamDArry[mHighFloor] & ONLY_FLR) <= (TmpOneStopFlr))           return(0);

        if((sRamDArry[mcurfloor] > TmpOneStopFlr))                          return(0);
        if((sRamDArry[mcurfloor] >= cF_TOPFLR))                             return(0);
    	Virtual_Floor_Set(TmpOneStopFlr);

    }
    else if(bDnWard){
        if( !(cF_ONESTOP_FLOOR & DN_READY))           						return(0);

        if( !(sRamDArry[mLowFloor] & UPDN_CAR_READY))            			return(0);

        if((sRamDArry[mLowFloor] & ONLY_FLR) >= (TmpOneStopFlr))            return(0);
        if((sRamDArry[mcurfloor] < TmpOneStopFlr))                          return(0);
        if((sRamDArry[mcurfloor] <  1))                                 	return(0);
	    Virtual_Floor_Set(TmpOneStopFlr);
    }

    return(0);
}






void  __attribute__((section(".usercode"))) CarKeyClr(void)
{

	sRamDArry[mEqualFloor]=(sRamDArry[mEqualFloor] & ~CAR_READY);
	sRamDArry[mHighFloor]=(sRamDArry[mHighFloor] & ~CAR_READY);
	sRamDArry[mLowFloor]=(sRamDArry[mLowFloor] & ~CAR_READY);

    sRamDArry[mCarKey1] =0x0;
    sRamDArry[mCarKey9] =0x0;
    sRamDArry[mCarKey17]=0x0;
    sRamDArry[mCarKey25]=0x0;

}


void  __attribute__((section(".usercode"))) KeyClr(void)
{
	sRamDArry[mEqualFloor]=0x0;
	sRamDArry[mHighFloor]=0x0;
	sRamDArry[mLowFloor]=0x0;
    sRamDArry[mAckStopFloor] = 0x0;
    sRamDArry[mNewAckStopFloor]=0x0;

    sRamDArry[mCarKey1] =0x0;
    sRamDArry[mCarKey9] =0x0;
    sRamDArry[mCarKey17]=0x0;
    sRamDArry[mCarKey25]=0x0;
}
*/



void   __attribute__((section(".usercode"))) Init485_Bd(void)
{
/*
	for(LogAddressLoop=0;LogAddressLoop<ABS_ADDRESS;LogAddressLoop++){
		BDid[sRamDArry[mDoorSeq]]=0;
		BdErrCntSeq[sRamDArry[mDoorSeq]]=0;
	}

	for(LogAddressLoop=0;LogAddressLoop < 12;LogAddressLoop++){
		errbd[LogAddressLoop]=0x0;
		Olderrbd[LogAddressLoop]=0x0;
	}

	LogAddressLoop=0;
	LogAddress=0;
	TimeOutBd=0;
	BefRcvAdr=0;
	AbsAddress=0;
	SerialTime=0;
	bFindBoard=0;
	errpt=0;
	ErrBdNamePt=0;
	BefErrBdNamePt=0;
*/

}


/*
void  __attribute__((section(".usercode"))) OutData(void)
{

    unsigned int tmpio;
    unsigned int tmpiobuf;

    TRISB=0x00ff;


    OUT_ACT1=0;
    OUT_ACT2=0;

    OUT_0=0;
    OUT_1=0;
    OUT_2=0;


    RD_WR=1;
    RD_WR=1;


    tmpio=(LATB & 0x00ff);
    tmpiobuf=(O_U_W_bit << 8);
    tmpio=(tmpiobuf | tmpio);
    LATB=tmpio;

    OUT_0=1;
    OUT_0=1;
    OUT_0=1;
    OUT_0=1;

    OUT_0=0;
    OUT_0=0;
    OUT_0=0;
    OUT_0=0;



    tmpio=(LATB & 0x00ff);
    tmpiobuf=(O_OP_bit << 8);
    tmpio=(tmpiobuf | tmpio);

    LATB=tmpio;

    OUT_1=1;
    OUT_1=1;
    OUT_1=1;
    OUT_1=1;

    OUT_1=0;
    OUT_1=0;
    OUT_1=0;
    OUT_1=0;


    tmpio=(LATB & 0x00ff);
    tmpiobuf=(O_Y_0_bit << 8);

    tmpio=(tmpiobuf | tmpio);

    LATB=tmpio;

    OUT_2=1;
    OUT_2=1;
    OUT_2=1;
    OUT_2=1;

    OUT_2=0;
    OUT_2=0;
    OUT_2=0;
    OUT_2=0;

/////////////
    OUT_SEG_SEL0=1;
    OUT_SEG_SEL1=1;

    SegSel=(SegSel+1) % 3;

    switch(SegSel){
        case    0:
            tmpio=(LATB & 0x00ff);
            tmpiobuf=(SegData0 << 8);
            tmpio=(tmpiobuf | tmpio);
            LATB=tmpio;
            OUT_SEG_SEL0=0;
            OUT_SEG_SEL1=0;
            break;
        case    1:
            tmpio=(LATB & 0x00ff);
            tmpiobuf=(SegData1 << 8);
            tmpio=(tmpiobuf | tmpio);
            LATB=tmpio;
            OUT_SEG_SEL0=1;
            OUT_SEG_SEL1=0;
            break;
        case    2:
            tmpio=(LATB & 0x00ff);
            tmpiobuf=(SegData2 << 8);
            tmpio=(tmpiobuf | tmpio);
            LATB=tmpio;
            OUT_SEG_SEL0=0;
            OUT_SEG_SEL1=1;
            break;
        default:
            break;
    }

    OUT_SEG=1;
    OUT_SEG=1;
    OUT_SEG=1;
    OUT_SEG=1;

    OUT_SEG=0;
    OUT_SEG=0;
    OUT_SEG=0;
    OUT_SEG=0;
//////////////////


    OUT_ACT1=0;
    OUT_ACT2=0;

	RD_WR=0;

    TRISB=0xffff;

    sRamDArry[O_OP]  = O_OP_bit;
    sRamDArry[O_U_W] = O_U_W_bit;
    sRamDArry[O_Y_0] = O_Y_0_bit;
}




void __attribute__((section(".usercode")))  ButtonClrCheck(void)
{
    S2_EXT_BUT_CLR1=0;
    S2_CAR_BUT_CLR1=0;


    if(S1_POWER_FAIL || bDoorJumper){
        ClrUpDnWard();
       	bExtButClr=1;
        bCarButClr=1;
    }

   	if(bExtButClr){
        S2_EXT_BUT_CLR1=1;
   	}

   	if(bCarButClr){
		CarKeyClr();
        S2_CAR_BUT_CLR1=1;
   	}

   	if(bCarUpDnChButClr){    // && (sRamDArry[mDoorSeq] == STOP_ELEVATOR)){
		CarKeyClr();
        S2_CAR_BUT_CLR1=1;
   	}

   	if(bExtButClr && bCarButClr){
		KeyClr();
   	}
}


void  __attribute__((section(".usercode")))   UpDnStopCmd(void)
{
    OUT_U_W(0);
    OUT_D_W(0);
    OUT_P1(0);
    OUT_P2(0);
    OUT_P3(0);
    OUT_P4(0);
    OUT_BK2(0);
    OUT_BRK(0);

    sRamDArry[mCarMoveState]=0;


}


void   __attribute__((section(".usercode"))) CarAllStopCmd(void)
{


	UpDnStopCmd();

    OUT_ERR(1);


    OUT_DAC(0);
    bDac=0;


	bManualStop=1;

    ManualStopTime=0;


    S2_CAR_MOVE1=0;

    S3_UPDN_VO1=0;

    ClrUpDnWard();

    bMoveCar=0;
    bCarUpMove=0;
    bCarDnMove=0;
   	ElevMoveTime=0;

    bDoorOpenCmd=0;
    bDoorCloseCmd=0;

    sRamDArry[mReqStopFloor]=0;
    sRamDArry[mCarMoveState]=0;


   	bExtButClr=1;
    bCarButClr=1;
    KeyClr();

   	OUT_ON=0;
   	OUT_ON=0;
    OutData();
    OutData();
}




void  __attribute__((section(".usercode")))   CarManualStopCmd(void)
{
    LocalType tmp;
    unsigned int	tmp_time;

    tmp_time = (ManualStopTime * 100);

    tmp=0;

    bInvOutAllClr=0;

    CounterTime=0;
    ZeroSpeedCmd_IO();


    if(tmp_time>iF_Bk1OffTime){
        OUT_BRK(0);
        tmp=(tmp | 0x01);;
    }

    if(tmp_time>iF_Bk2OffTime){
        OUT_BK2(0);
        tmp=(tmp | 0x02);;
    }

    if(tmp_time>iF_UDOffTime){
        OUT_U_W(0);
        OUT_D_W(0);
        tmp=(tmp | 0x04);;
    }

    if(tmp_time>iF_P4OffTime){
        OUT_P4(0);
        tmp=(tmp | 0x08);;
    }


    if(tmp == 0x0f){
        bInvOutAllClr=1;
    }
    else{
        bInvOutAllClr=0;
    }
}



void   __attribute__((section(".usercode"))) CarAllStopCmdTest(void)
{
    unsigned long i,j;

    if(!bManualStop){
        ManualStopTime=0;
        bInvOutAllClr=0;
    }

   	bExtButClr=1;
    bCarButClr=1;
    ButtonClrCheck();

    bManualStop=1;
	ElevMoveTime=0;


    OUT_DAC(0);
    bDac=0;

    if(!OilLopeTypeChk || bCarErr || bCarStopNoRun){

		UpDnStopCmd();
        OUT_ERR(1);

        bDoorOpenCmd=0;
        bDoorCloseCmd=0;

        ClrUpDnWard();

        if(ManualStopTime > 15){
            bInvOutAllClr=1;
        }
    }
    else    CarManualStopCmd();


    if(bInvOutAllClr){
		bManualStop=0;

		UpDnStopCmd();

      	if(IN_DLS)		            bHomeUpDn = 1;
      	else if(IN_ULS)             bHomeUpDn = 0;
      	else if(!IN_BAT){												//3.4d
    		bHomeUpDn = 1;												//3.4d
			if(!IN_X4 && (USE_CHECK == MAN_USE))	bHomeUpDn = 0;		//3.4d
		}																//3.4d
      	else if(!IN_LU && IN_LD)    bHomeUpDn = 1;
      	else if(IN_LU && !IN_LD)    bHomeUpDn = 0;

        else if(!IN_SD1)          bHomeUpDn=0;
        else if(!IN_SU1)  		  bHomeUpDn=1;
        else if((sRamDArry[mcurfloor] == 0))          bHomeUpDn=0;
        else if((sRamDArry[mcurfloor] == cF_TOPFLR))  bHomeUpDn=1;

//        else if(!IN_SD1 || (sRamDArry[mcurfloor] == 0))          bHomeUpDn=0;
//        else if(!IN_SU1 || (sRamDArry[mcurfloor] == cF_TOPFLR))  bHomeUpDn=1;

      	else if(CurPulse <= FLOOR_COUNT(0)){
            bHomeUpDn = 1;
        }
      	else if(CurPulse >= FLOOR_COUNT(cF_TOPFLR)){
            bHomeUpDn = 0;
        }
      	else if(CurPulse <= FLOOR_COUNT(sRamDArry[mcurfloor])){
            if(sRamDArry[mcurfloor]>0){
                i=FLOOR_COUNT(sRamDArry[mcurfloor]);
                j=CurPulse;
                TmpFhmCounter1=(unsigned long)(i-j);

                i=FLOOR_COUNT(sRamDArry[mcurfloor]-1);
                j=CurPulse;
                TmpFhmCounter2=(unsigned long)(j-i);

                if(TmpFhmCounter1 >= TmpFhmCounter2)    bHomeUpDn = 0;
                else                                    bHomeUpDn = 1;
            }
            else    bHomeUpDn = 1;
        }
      	else if(CurPulse >= FLOOR_COUNT(sRamDArry[mcurfloor])){
            if(sRamDArry[mcurfloor] < cF_TOPFLR){
                i=FLOOR_COUNT(sRamDArry[mcurfloor]);
                j=CurPulse;
                TmpFhmCounter1=(unsigned long)(j-i);

                i=FLOOR_COUNT(sRamDArry[mcurfloor]+1);
                j=CurPulse;
                TmpFhmCounter2=(unsigned long)(i-j);

                if(TmpFhmCounter1 >= TmpFhmCounter2)    bHomeUpDn = 1;
                else                                    bHomeUpDn = 0;

            }
            else    bHomeUpDn = 0;
        }
      	else	bHomeUpDn = 0;


        bFR2Start1=0;

        S2_CAR_MOVE1=0;

        S3_UPDN_VO1=0;

      	ClrUpDnWard();

      	bMoveCar=0;
        bCarUpMove=0;
        bCarDnMove=0;

      	sRamDArry[mReqStopFloor]=0;
      	sRamDArry[mCarMoveState]=0;
        KeyClr();
	}
}



void   __attribute__((section(".usercode")))  MotorStop(void)
{
    if(!bMoveCar){
    	OUT_DAC(0);
        if(!bCarOnceStop){
            CarOnceStopTime=0;
            bCarOnceStop=1;
        }
    }
    else{
        bCarOnceStop=0;
        bSlipCheckStart=0;
    }
}


UserDataType	__attribute__((section(".usercode"))) NextUpDnKeyChk(unsigned int UpDn)
{
	UserDataType	i,j,k,bitvar,Fstartflr,Flastflr,ret;
	UserDataType	tmpCarkey[8];

	ret=1;
	CarBdButtonFlr=0;


	k=(UserDataType)(sRamDArry[mcurfloor]);
    k=(k & 0x00ff);

//	if(k < 1)					return(1);
//	if(k >= cF_TOPFLR)			return(1);


	tmpCarkey[0]=(sRamDArry[mCarKey1]  & sRamDArry[FLR_ON_OFF0]);
	tmpCarkey[1]=(sRamDArry[mCarKey9]  & sRamDArry[FLR_ON_OFF1]);
	tmpCarkey[2]=(sRamDArry[mCarKey17] & sRamDArry[FLR_ON_OFF2]);
	tmpCarkey[3]=(sRamDArry[mCarKey25] & sRamDArry[FLR_ON_OFF3]);


	j=0;


    bitvar=0x01;

   	if(UpDn){
		if(k >= cF_TOPFLR)			return(1);
		else{
			Fstartflr = (k+1);
			Flastflr  = cF_TOPFLR;

			i = (Fstartflr / 8);
			j = (j + i);

			i = (Fstartflr % 8);
	   	 	bitvar=(bitvar << i);
		}
	}
	else{
		if(k < 1)		return(1);
		else{
			Fstartflr=0;
			Flastflr=(k-1);
		}
	}


    for( ; Fstartflr <= Flastflr;Fstartflr++){
		if(tmpCarkey[j] & bitvar){
//////////////////			CarBdButtonFlr=(Fstartflr | CAR_READY);
			ret=0;
   			if(UpDn==0)	return(ret);
        }
        bitvar=(bitvar << 1);
        bitvar=(bitvar & 0x00ff);

        if(bitvar==0x0){
            bitvar=0x01;
            j++;
        }
    }

    return(ret);
}




UserDataType  __attribute__((section(".usercode"))) TopDirectionSet(void)
{
	bCarUpDnChButClr=1;
	DnWard();
	if( !(sRamDArry[mAckStopFloor] & DN_READY))	sRamDArry[DEST_FLR] = (sRamDArry[DEST_FLR]	& ~DN_READY);
	sRamDArry[mAckStopFloor]=(sRamDArry[mcurfloor] | DN_READY);
}


UserDataType  __attribute__((section(".usercode"))) LowDirectionSet(void)
{
	bCarUpDnChButClr=1;
	UpWard();
	if( !(sRamDArry[mAckStopFloor] & UP_READY))	sRamDArry[DEST_FLR] = (sRamDArry[DEST_FLR]	& ~UP_READY);
	sRamDArry[mAckStopFloor]=(sRamDArry[mcurfloor] | UP_READY);
}





unsigned int  __attribute__((section(".usercode"))) CounterCheck(void)
{
//    if((INVERTER_CHECK == IO) && (bitChk_FLRDSPCH(F_OnOff1,(bEncoderUse % 8))) ){
//    if((INVERTER_CHECK == IO) && (EncoderUseChk) ){
    if(INVERTER_CHECK == IO){

        if(bFhmRun)             CounterChkTime=30;
        else if(!bManualAuto)   CounterChkTime=30;
        else                    CounterChkTime=10;


        if(!bMoveCar){
            CounterTime=0;
        }

        if((bAuto || bFhmRun) && bMoveCar && (CounterTime > CounterChkTime)){
   		    bMoveOk=0;
		    bMoveUpOk=0;
		    bMoveDnOk=0;
            bCarStopNoRun=1;
            bEncoderErr=1;
        }

        if((bAuto || bFhmRun) && bMoveCar && bCarUpMove && (CountDn> 30)){
   		    bMoveOk=0;
		    bMoveUpOk=0;
		    bMoveDnOk=0;
            bCarStopNoRun=1;
            bEncoderABErr=1;
        }

        if((bAuto || bFhmRun) && bMoveCar && bCarDnMove && (CountUp> 30)){
   		    bMoveOk=0;
		    bMoveUpOk=0;
		    bMoveDnOk=0;
            bCarStopNoRun=1;
            bEncoderABErr=1;
        }

        if(bEncoderErr){
            if(sRamDArry[mSysStatus] >= sEncoderErr)   sRamDArry[mSysStatus]=sEncoderErr;
            return(0);
        }

        if(bEncoderABErr){
            if(sRamDArry[mSysStatus] >= sEncoderABErr)   sRamDArry[mSysStatus]=sEncoderABErr;
            return(0);
        }
    }
    return(1);
}









void  __attribute__((section(".usercode"))) LightOnOff(void)
{
    OUT_LIT(1);
    if(S3_PARKING1 && (FanTime > 2))                OUT_LIT(0);
    if(S1_POWER_FAIL && bDoorCloseOk)               OUT_LIT(0);
    if(bWaterSen && bDoorOpenEnd)   	            OUT_LIT(0);
    if((LightTime >= cF_LIGHTTIME))                 OUT_LIT(0);
}


void  __attribute__((section(".usercode"))) EmergencyCheck(void)
{
	unsigned int i;

	if(bsEmergency || bsLope || bsSlip){
        OUT_U_W(0);
        OUT_D_W(0);
        OUT_P1(0);
        OUT_P2(0);
        OUT_P3(0);
        OUT_P4(0);
        OUT_DAC(0);
        OUT_OP(0);
        OUT_CL(0);
        OUT_OP_S(0);
        OUT_CL_S(0);
        OUT_BK2(0);
        OUT_D_S(0);
        OUT_GBR(0);
        OUT_BRK(0);
        OUT_FAN(1);
        OUT_LIT(1);
        OUT_BUZ(0);
        OUT_BELL(0);
        OUT_RST(0);
        OUT_ERR(0);
        OUT_HOP(0);
        OUT_HCL(0);


// 		O_U_W_bit=0;
//   	O_OP_bit=0;
//   	O_Y_0_bit=0;

        bCarErr=1;

//        if(INVERTER_CHECK == LG)    OUT_P4(0);
//        else                        OUT_P4(0);


   		ElevMoveTime=0;

		ClrUpDnWard();

//		OUT_FAN(1);
//        OUT_LIT(1);
//        OUT_ERR(1);
	}

    if(bsSlip)			OUT_GBR(1);
    else if(bsLope)		OUT_GBR(1);
    else                OUT_GBR(0);


    if(!IN_FR1 || !IN_FR2){
        OUT_FAN(0);
	}

}





unsigned int __attribute__((section(".usercode")))  OneDoorSenserNoCloseChk(void)
{

    if(!bHoleDoorCloseEnd)         return(1);
    else if(!bCarDoorCloseEnd)     return(1);
    else                           return(0);

}


unsigned int __attribute__((section(".usercode")))  OneDoorSenserCloseChk(void)
{

	DoorJumperNm=0;

    if(CarDoor1UseChk){
        if(CarDoorCloseEndCheckForDoorjmp())        DoorJumperNm= 1;
//return(1);
    }


    if(HoleDoor1UseChk){
        if(HoleDoorCloseEndCheckForDoorjmp())      DoorJumperNm=(DoorJumperNm | 2);
//return(1);
    }

    return(DoorJumperNm);
}


unsigned int __attribute__((section(".usercode")))  OnlyOneCallChk(void)
{
	UserDataType	i,j,k,ext_clr_chk;

    BaggageFlrOnOff[0]     = 0xff;
    BaggageFlrOnOff[1]     = 0xff;
    BaggageFlrOnOff[2]     = 0xff;
    BaggageFlrOnOff[3]     = 0xff;

    ext_clr_chk=0;


    if(AutoRunReady()){
        if(OnlyOneCallOnOffChk){
            if((sRamDArry[mDoorSeq] != READY_ELEVATOR) || (sRamDArry[mHighFloor] > 0) || (sRamDArry[mLowFloor] > 0)){
                ext_clr_chk=1;
            }

            if(sRamDArry[mDoorSeq] > READY_ELEVATOR){
                BaggageFlrOnOff[0]     = 0x0;
                BaggageFlrOnOff[1]     = 0x0;
                BaggageFlrOnOff[2]     = 0x0;
                BaggageFlrOnOff[3]     = 0x0;


                i=(sRamDArry[DEST_FLR] & ONLY_FLR);
                j=(i / 8);
                k=(i % 8);
                i=(0x01 << k);

				if(j < 4){
            		BaggageFlrOnOff[j] = i;
				}
            }
        }
    }

    if(ext_clr_chk) bHibCallClear=1;
    else            bHibCallClear=0;
}



unsigned int __attribute__((section(".usercode")))  SafetyChk(void)
{

	unsigned int    i,j;

    i=0;

	bSafety_scrSft=0;

	if(IN_AUTO)				return(0);

    if(!IN_FR1 || !IN_FR2)  return(0);


    if(S3_PARKING1)  		return(0);


	if(IN_SFT)	bSafety_scrSft=1;


    if(INVERTER_CHECK != LG){
		if(!S2_FIRE1){
    		if(IN_FS1)	bSafety_scrSft=1;
		}
	}



    if(SubSafetyUseChk){
        if(USE_CHECK == CARLIFT_USE){
            if(bSafety_scrSft)                                      i=1;
            else if(IN_FULL)                                i=1;
        }
        else{
            if(!bMoveCar){
    	        j=MainSubDoorFlrChk(sRamDArry[mcurfloor]);
                if(j==0)                                    i=0;
                else if(j==1){
                    if(bSafety_scrSft)                              i=1;
                }
                else if(j==2){
                    if(IN_FULL)                             i=1;
                }
                else{
                    if(OneDoorSenserNoCloseChk()){
                        if(CurDoorSelect == MAIN_DOOR){

                            if(bSafety_scrSft || (SafetyValidLR && IN_FULL)){
                            	sRamDArry[mDoor] = (sRamDArry[mDoor] | MAIN_OPEN_KEY);
                      			i=1;
							}
                        }
                        else if(CurDoorSelect == SUB_DOOR){
                            if(IN_FULL || (SafetyValidLR && bSafety_scrSft)){
                            	sRamDArry[mDoor] = (sRamDArry[mDoor] | SUB_OPEN_KEY);
                      			i=1;
							}
                        }
                    }
                    else{
                        if(bSafety_scrSft){
                            sRamDArry[mDoor] = (sRamDArry[mDoor] | MAIN_OPEN_KEY);
                            i=1;
                        }
                        else if(IN_FULL){
                            sRamDArry[mDoor] = (sRamDArry[mDoor] | SUB_OPEN_KEY);
                            i=1;
                        }
                    }
                }
            }
        }
    }
    else{
        if(bSafety_scrSft)                             						i=1;
    }

    return(i);
}





unsigned int  __attribute__((section(".usercode")))  SaftyStopChk(void)
{
	UserDataType	safty_stop_chk;

    safty_stop_chk=0;

	if(bAuto){
//	    if(!CarDoor1UseChk && !CarDoor2UseChk)  safty_stop_chk=1;

	    if(!CarDoor1UseChk )  					safty_stop_chk=1;
	    if(USE_CHECK == CARLIFT_USE)            safty_stop_chk=1;
	}

    if(safty_stop_chk){
        if(SafetyChk() && !bLevelFind){
       		bMoveOk=0;
       		bMoveUpOk=0;
       		bMoveDnOk=0;

            if(bMoveCar){
        		bCarErr=1;
            }
        }
    }
    return(0);
}



void  __attribute__((section(".usercode"))) EmgDlsUlsCheck(void)
{
    bExtButClr=0;
	bCarButClr=0;

	if(IN_EMG || (!bAuto && IN_GR)){
		bExtButClr=1;
		bCarButClr=1;
		bMoveOk=0;
		bMoveUpOk=0;
		bMoveDnOk=0;
		S1_EMG1=1;
		bsEmergency=1;
		bCarErr=1;
	}
	else{
		bMoveOk=1;
		bMoveUpOk=1;
		bMoveDnOk=1;
		bsEmergency=0;
        S1_EMG1=0;
	}


	if(IN_ULS){
		if(bCarUpMove || bUpWard){
			bCarErr=1;
		    bExtButClr=1;
		    bCarButClr=1;
        	bsUls=1;
    		bMoveUpOk=0;
        }
    	else{
            if(bsUls){
                if(!bManualStop)   bsUls=0;
            }
            else                   bsUls=0;
        }
	}
	else    bsUls=0;


	if(IN_DLS){
		if(bCarDnMove || bDnWard){
			bCarErr=1;
    		bExtButClr=1;
	    	bCarButClr=1;
            bsDls=1;
    		bMoveDnOk=0;
        }
    	else{
            if(bsDls){
                if(!bManualStop)   bsDls=0;
            }
            else                   bsDls=0;
        }
	}
	else  bsDls=0;


	if(IN_GR){
		bExtButClr=1;
		bCarButClr=1;
		bsLope=1;
		bMoveOk=0;
		bMoveUpOk=0;
		bMoveDnOk=0;
		bCarErr=1;
	}
	else{
		bsLope=0;
	}


	if(!bMoveUpOk && bUpWard){
		bCarErr=1;
	}

	if( !bMoveDnOk && bCarDnMove){
         bCarErr=1;
   	}
}






unsigned int __attribute__((section(".usercode")))  ThisFloorDoorCheck(void)
{
	OneDoorSelOpenKeyCheck();
	if(CurDoorSelect == SUB_DOOR)	OUT_D_S(1);
	else							OUT_D_S(0);
	return(0);
}


unsigned int __attribute__((section(".usercode")))  RunningOpenCheck(void)
{

#ifdef  TEST_SIMULATION
	return(0);
#else
	if( !RunningOpenOnOff)																	return(0);
	if((USE_CHECK != MAN_USE) )																return(0);
 	if(!bAuto || !bDac || !bUnd || !bOnLuLd)												return(0);
	if((sRamDArry[mcurfloor] & ONLY_FLR) != (sRamDArry[mAckStopFloor] & ONLY_FLR))			return(0);
	if(iF_SolOnTime || iF_DoorWaitTime )													return(0);
	if( !NoUseNoOpen())																		return(0);

	if( !bRunningOpenOn){
	    if(S2_FIRE1)																		return(0);
	    if(S1_POWER_FAIL)																	return(0);
	}

	return(1);
#endif
}



unsigned int __attribute__((section(".usercode")))  RunningOpenAction(void)
{
	if(RunningOpenCheck()){
		ThisFloorDoorCheck();
		SelectDoorOpen_you();
		bRunningOpenOn=1;
		return(1);
	}
	else{
		bRunningOpenOn=0;
		return(0);
	}
}



void __attribute__((section(".usercode")))  DoorCloseOkCheck(void)
{
	DoorCloseEndCheck((UserDataType)bMoveCar);

   	if(!bDoorCloseOk){
		if( !bRunningOpenOn){
	      	bMoveOk=0;
	      	bMoveUpOk=0;
	      	bMoveDnOk=0;
	      	if(bMoveCar){
	        	bCarErr=1;
	      	}
		}
		if(!bsEmergency)    S1_EMG1=0;
   	}
}




void __attribute__((section(".usercode")))  LuLdErrSave(unsigned int updn)
{
	UserDataType	i,j,err;

	err=0;

	bsLuOrLdErr=1;
	LuLdErrNm=6;


	j=LuLdSeq[3];
	j= (j +1) % 4;
	if(LuLdSeq[2] == j){
		j= (j +1) % 4;
		if(LuLdSeq[1] == j){
			j= (j +1) % 4;
			if(LuLdSeq[0] == j){
				LuLdErrNm=1;
				err=1;
			}
		}
	}


	if(err==0){
		if(updn){
			if((LuLdSeq[0] == LuLdSeq[2]) && (LuLdSeq[1] == LuLdSeq[3])){
				if( (LuLdSeq[0] + LuLdSeq[1]) == 1)					LuLdErrNm=2;		//LD ON
				if( (LuLdSeq[0] + LuLdSeq[1]) == 5)					LuLdErrNm=3;		//LD OFF
				if( (LuLdSeq[0] + LuLdSeq[1]) == 3){
					if( (LuLdSeq[0] == 0) || (LuLdSeq[0] == 3))		LuLdErrNm=4;		//LU ON
					if( (LuLdSeq[0] == 1) || (LuLdSeq[0] == 2))		LuLdErrNm=5;		//LU OFF
				}
			}
		}
		else{
			if((LuLdSeq[0] == LuLdSeq[2]) && (LuLdSeq[1] == LuLdSeq[3])){
				if( (LuLdSeq[0] + LuLdSeq[1]) == 1)					LuLdErrNm=4;		//LU ON
				if( (LuLdSeq[0] + LuLdSeq[1]) == 5)					LuLdErrNm=5;		//LU OFF
				if( (LuLdSeq[0] + LuLdSeq[1]) == 3){
					if( (LuLdSeq[0] == 0) || (LuLdSeq[0] == 3))		LuLdErrNm=2;		//LD ON
					if( (LuLdSeq[0] == 1) || (LuLdSeq[0] == 2))		LuLdErrNm=3;		//LD OFF
				}
			}
		}
	}

   	bMoveOk=0;
   	bMoveUpOk=0;
   	bMoveDnOk=0;

   	bCarStop=1;
}






void __attribute__((section(".usercode")))  OverLoadCheck(void)
{
    if(IN_OVL && !bMoveCar && (!IN_LU || !IN_LD) && !bDoorCloseOk && bAuto){
        S1_OVERLOAD1=1;
        	if(IN_FR1){

            bMoveOk=0;
            bMoveUpOk=0;
            bMoveDnOk=0;

			if(sRamDArry[mDoorSeq] >= DOOR_OPEN_WAIT){
	            sRamDArry[mDoorSeq]=DOOR_OPEN_START;
	            DoorOpenTime=0;
			}

//////////// sRamDArry[mDoorSeq]=DOOR_OPEN_START;
//            DoorOpenTime=0;

            if(sRamDArry[mSysStatus] >= sOVL)   sRamDArry[mSysStatus]=sOVL;
        }
    }
    else{
        S1_OVERLOAD1=0;
    }
}




unsigned int    __attribute__((section(".usercode"))) ParkingValidCheck(void)
{

	if(IN_PRK || bPC_PRK || bSlavePrk){

		return(1);
    }
    else    return(0);
}



void  __attribute__((section(".usercode"))) ParkingCheck(void)
{
	S4_PARKING_READY1=0;

	if(S3_PARKING1 && (ParkingValidCheck()) && !S2_FIRE1 && (!IN_LU || !IN_LD) ){

///////////////////////	if(S3_PARKING1 && (ParkingValidCheck() || bPC_PRK) && !S2_FIRE1 && (!IN_LU || !IN_LD) ){

		if(sRamDArry[mSysStatus] >= sPARKING)   sRamDArry[mSysStatus]=sPARKING;

        if(!bDoorCloseOk){
        	bMoveOk=0;
        	bMoveUpOk=0;
        	bMoveDnOk=0;
        }

    	bExtButClr=1;
    	bCarButClr=1;
    	bTestKey=0;


   		sRamDArry[mEqualFloor] = 0;
   		sRamDArry[mHighFloor] = 0;
   		sRamDArry[mLowFloor] = 0;



   		if(!OpenKeyCheck()){
       		sRamDArry[mDoorSeq] = DOOR_OPEN_START;
            S3_PARKING1=0;
   		}

  	}
  	else if((ParkingValidCheck() || bPC_PRK) && bAuto && !S2_FIRE1 && (!IN_LU || !IN_LD) ){
   		if(sRamDArry[mcurfloor]== cF_PRKFLR){
       		if((sRamDArry[mDoorSeq] >= DOOR_CLOSE_END) && bDoorCloseOk && (sRamDArry[mDoorSeq] <= READY_ELEVATOR)){
                S3_PARKING1=1;
	    		OpenKeyCheck();
       		}
	   		else if( (sRamDArry[mDoorSeq] >= DOOR_OPEN_WAIT) && !bMoveCar){
				S4_PARKING_READY1=1;
			}
   		}
   		else if(!ReadyAndNoKeyCheck()){
   			Virtual_Floor_Set(cF_PRKFLR);
		}
  	}
  	else{
   		if(S3_PARKING1){
       		sRamDArry[mDoorSeq] = DOOR_OPEN_START;
   		}
   		S3_PARKING1=0;
  	}
}



void  __attribute__((section(".usercode"))) MyFamilyServiceCheck(void)
{

	S4_FAMILY_SERVICE1=0;
	if(bPC_FAMILY){
		S4_FAMILY_SERVICE1=1;
	}

	S4_WAIT_FLR_SERVICE1=0;
	if(CmdFixFlrTime > 0){
    	if(cF_FIXFLOOR <= cF_TOPFLR){
			S4_WAIT_FLR_SERVICE1=1;
		}
	}
}




void __attribute__((section(".usercode")))  VipCheck(void)
{

    if( (!IN_PASS || bOnceVip) && AutoRunReady()){
        bExtButClr=1;
        S3_VIP1=1;

        if(sRamDArry[mSysStatus] >= sVIP)   sRamDArry[mSysStatus]=sVIP;

        if((sRamDArry[mDoorSeq] == READY_ELEVATOR) && (sRamDArry[mNewAckStopFloor]==0) ){
            sRamDArry[mDoorSeq] = DOOR_OPEN_START;
        }
    }
    else{
        S3_VIP1=0;

        bOnceVip=0;
        Vip_Floor=0;
    }

    if(bOnceVip){
        if(((Vip_Floor & ONLY_FLR) != sRamDArry[mcurfloor]) &&  !bMoveCar){
            bOnceVip=0;
            Vip_Floor=0;
        }
    }

}



void __attribute__((section(".usercode")))  FullCheck(void)
{

    if(!SubSafetyUseChk){
		if(!AutoRunReady())	S3_FULL1=0;
		else{
			if(!bDoorCloseOk || !bMoveCar){
		        if(!IN_FULL){
		            S3_FULL1=1;
		            if( !(sRamDArry[mHighFloor] & CAR_READY))	sRamDArry[mHighFloor]=0;
		            if( !(sRamDArry[mLowFloor]  & CAR_READY))	sRamDArry[mLowFloor]=0;
		        }
		        else{
		            S3_FULL1=0;
		        }
			}
		}
    }
    else	S3_FULL1=0;

}




UserDataType __attribute__((section(".usercode")))  SubFireCheck(void)
{
    if(USE_CHECK == BAGGAGE_USE)    return(0);
    if(USE_CHECK == CARLIFT_USE)   	return(0);
    if(INVERTER_CHECK == D_F)		return(0);

    if(!IN_X7 && (cF_X7SAFEFLR <= cF_TOPFLR)){
		FireBaseFloor=cF_X7SAFEFLR;
    	return(1);
	}
    else if(!IN_X6 && (cF_X6SAFEFLR <= cF_TOPFLR)){
		FireBaseFloor=cF_X6SAFEFLR;
    	return(1);
	}
    else if(!IN_X5 && (cF_X5SAFEFLR <= cF_TOPFLR)){
		FireBaseFloor=cF_X5SAFEFLR;
    	return(1);
	}
    else                          	return(0);
}

UserDataType __attribute__((section(".usercode")))  UpLimitSpeedCheck(void)
{
    if(USE_CHECK == BAGGAGE_USE)    return(0);
    if(USE_CHECK == CARLIFT_USE)   	return(0);
    if(INVERTER_CHECK == D_F)		return(0);

    if(!IN_X1)                     	return(1);
    else                           	return(0);
}


UserDataType __attribute__((section(".usercode")))  DnLimitSpeedCheck(void)
{
    if(USE_CHECK == BAGGAGE_USE)    return(0);
    if(USE_CHECK == CARLIFT_USE)   	return(0);
    if(INVERTER_CHECK == D_F)		return(0);

    if(!IN_X0)                     	return(1);
    else                           	return(0);
}



UserDataType __attribute__((section(".usercode")))  KidsKeyCheck(void)
{
	UserDataType	i,j,k,l;

    if(USE_CHECK == BAGGAGE_USE)	return(0);
    if(USE_CHECK == CARLIFT_USE)   	return(0);
    if(INVERTER_CHECK == D_F)		return(0);
	if(cF_KIDS_KEY_CNT <= 1)		return(0);
    if(!IN_X2 || !KidsKeyChkUse)   	return(0);

	j=mCarKey1-1;
	k=0;
	for(i=0;i <= cF_TOPFLR;i++){
		if( !(i % 8)){
			l=0x01;
			j++;
		}

		if(sRamDArry[j] & l){
			k++;
		}
		l=(l << 1);
	}

	if(k > cF_KIDS_KEY_CNT)	bCarUpDnChButClr=1;

	return(0);
}



UserDataType __attribute__((section(".usercode")))  CarKeyMostServiceCheck(void)
{

	if(bMoveCar)	return(0);

	S4_CAR_MOST_SERVICE1=0;

    if(!CarKeyFirstService)   return(0);

	if( !(sRamDArry[mCarKey1] | sRamDArry[mCarKey9] | sRamDArry[mCarKey17] | sRamDArry[mCarKey25]))	return(0);

	if(!bUpWard && !bDnWard)																		return(0);




	if( !(sRamDArry[mHighFloor] & CAR_READY))	sRamDArry[mHighFloor]=0x0;
	if( !(sRamDArry[mLowFloor]  & CAR_READY))	sRamDArry[mLowFloor] =0x0;

	if(bDoorCloseOk){
		if((sRamDArry[mHighFloor] & UPDN_CAR_READY) ||  (sRamDArry[mLowFloor]  & UPDN_CAR_READY)){
			S4_CAR_MOST_SERVICE1=1;
		}
	}

	return(0);
}




void __attribute__((section(".usercode")))  BatCheck(void)
{
    if(!IN_BAT && bAuto){
        if(PowerFailTime > 20){
            PowerFailTime=21;

			if((USE_CHECK == MAN_USE) && bMoveCar){
				if(!IN_X4 && bHomeUpDn)			PowerFailTime=0;
				else if(IN_X4 && !bHomeUpDn)	PowerFailTime=0;
			}


            if(bOnLuLd && bDoorOpenEnd && !bMoveCar){
              	ClrUpDnWard();
                OUT_FAN(0);
            }
        }

        if(PowerFailTime < 20){
            bCarErr=1;
        }

        S1_POWER_FAIL=1;
        NextFloorTime=0;
    }
    else{
        if(S1_POWER_FAIL){
            if(PowerFailTime > 40){
                PowerFailTime=41;
                S1_POWER_FAIL=0;
                if(INVERTER_CHECK == IO){
                    OUT_P4(0);
                }
            }
            else{
                if(INVERTER_CHECK == IO){
                    OUT_P4(0);
                }
                bCarErr=1;
            }
        }
        else{
            PowerFailTime=0;
        }
    }
}






void __attribute__((section(".usercode")))  InputCheck(void)
{
    EmgDlsUlsCheck();
	ReLevelCheck();
	DoorCloseOkCheck();
    OverLoadCheck();
    ParkingCheck();
	MyFamilyServiceCheck();
    VipCheck();
    FullCheck();
///////////////    LuLdOnCheck();
    BatCheck();

}








void __attribute__((section(".usercode")))  AutoManualCheck(void)
{

    if(IN_AUTO || (WarmingUpTime<100) || (sRamDArry[LD_INSP] == 0x55) || ManWorkingChk){
        if(S1_AUTO1){
            if(bMoveCar)    bCarErr=1;
            else            bCarStop=1;
        }
        S1_AUTO1=0;
       	bExtButClr=1;
        bCarButClr=1;
        bAuto=0;

		sRamDArry[mSysStatus]=sREADY;
    }
    else{
	    if(bFhmRun && (sRamDArry[FHM_SEQ]!= FHM_SEQ_5)){
			bCarStopNoRun=1;
			if(sRamDArry[mSysStatus] >= sFhmError)   sRamDArry[mSysStatus]=sFhmError;
		}
		else{
	        S1_AUTO1=1;
	        bAuto=1;
		}
    }


    if(FhmOnOffChk && (INVERTER_CHECK == IO)){
	    S1_AUTO1=0;
	    bExtButClr=1;
	    bAuto=0;
	    bExtButClr=1;
	    bCarButClr=1;
    }
}



void __attribute__((section(".usercode")))  AutoUpDownStart(void)
{
    if(sRamDArry[mAckStopFloor]){
        if((sRamDArry[mAckStopFloor] & ONLY_FLR) > sRamDArry[mcurfloor]){
            if(sRamDArry[mCarMoveState]==0){
                if(bMoveUpOk)   MoveCounterx;
            }
            CarUpStartCmd();
        }
        else if((sRamDArry[mAckStopFloor] & ONLY_FLR) < sRamDArry[mcurfloor]){
            if(sRamDArry[mCarMoveState]==0){
                if(bMoveDnOk)   MoveCounterx;
            }
            CarDnStartCmd();
        }
    }
}



unsigned int	__attribute__((section(".usercode"))) LoadCmdStartCheck(void)
{

	if(!AutoRunReady())								return(0);


	if((sRamDArry[mVFlr1] != sRamDArry[mVFlr2]) && (sRamDArry[mVFlrCnt] > 0)){
		if((cF_FLRDSPCH(0) == 'Z') && (cF_FLRDSPCH(1) == 'Z')){
			AccTimer=0;
			sRamDArry[ExtCmdFlrONOFF0]=0x0;
			sRamDArry[ExtCmdFlrONOFF1]=0x0;
			sRamDArry[ExtCmdFlrONOFF2]=0x0;
			sRamDArry[ExtCmdFlrONOFF3]=0x0;

		}
	}


   	if(bCarUpMove || bCarDnMove)					return(0);

#ifdef  KTL
     sRamDArry[mVFlrCnt]= 254;
     sRamDArry[mVFlr1] 	= cF_TOPFLR+1;
     sRamDArry[mVFlr2] 	= 1;
#else
	if(sRamDArry[mVFlrCnt] == 0)					return(0);
	if(sRamDArry[mVFlrCnt] > 254)					return(0);
	if( !(sRamDArry[mVFlr1] | sRamDArry[mVFlr2]))	return(0);

	if(sRamDArry[mVFlrCnt] > 250)	sRamDArry[mVFlrCnt] = 254;
#endif


	if(sRamDArry[mLowFloor]  & UPDN_CAR_READY)		return(0);
	if(sRamDArry[mHighFloor] & UPDN_CAR_READY)		return(0);


		if((sRamDArry[mVFlr1] != sRamDArry[S0_FLOOR]) && (sRamDArry[mVFlr1] > 0) && (sRamDArry[mVFlr1] <= (cF_TOPFLR+1))){
			Virtual_Floor_Set(sRamDArry[mVFlr1] -1);
		}
		if((sRamDArry[mVFlr2] != sRamDArry[S0_FLOOR]) && (sRamDArry[mVFlr2] > 0) && (sRamDArry[mVFlr2] <= (cF_TOPFLR+1))){
			Virtual_Floor_Set((sRamDArry[mVFlr2] -1));
		}

	return(0);
}



unsigned int __attribute__((section(".usercode"))) TimeCmpare(unsigned int i_basetime,unsigned int i)
{
    LocalType     Tmp_timeH,Tmp_timeM,Tmp_Stime,Tmp_Etime;
    LocalType     i_localtime;

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


	if(Tmp_Stime == Tmp_Etime)	return(0);
	if(Tmp_Stime == 0)			return(0);
	if(Tmp_Etime == 0)			return(0);


	if( (i_basetime >= Tmp_Stime) && (i_basetime < Tmp_Etime)){
		return(1);
	}

	return(0);
}



unsigned int	__attribute__((section(".usercode"))) FixFloorStartTimeCheck(void)
{
    LocalType     cTime,cMin;

	cTime=0;

	if(CmdFixFlrTime > 0){
	    cTime=(((sRamDArry[Hour] & 0xf0) >> 4) * 10);
	    cTime=((sRamDArry[Hour] & 0x0f) + cTime);
	    cMin=(((sRamDArry[Minuate] & 0xf0) >> 4) * 10);
	    cMin=((sRamDArry[Minuate] & 0x0f) + cMin);
		if((cTime < 24) && (cMin < 60)){
			cTime=((cTime << 8) | cMin);

		}
	}
	return(cTime);
}


unsigned int	__attribute__((section(".usercode"))) FixFloorStartCheck(void)
{
    LocalType     TmpFlrBuf;
    LocalType     cTime;


	if(S3_PARKING1)					return(0);
   	if(bCarUpMove || bCarDnMove)	return(0);


	TmpFlrBuf=0xff;
	cTime=FixFloorStartTimeCheck();

	if(cTime > 0){
		if(TimeCmpare(cTime,0)){
			TmpFlrBuf=cF_FLRDSPCH((unsigned long)(F_0506 +  4));
		}
		else if(TimeCmpare(cTime,5)){
			TmpFlrBuf=cF_FLRDSPCH((unsigned long)(F_0506 +  9));
		}
		else if(TimeCmpare(cTime,10)){
			TmpFlrBuf=cF_FLRDSPCH((unsigned long)(F_0506 +  14));
		}


		if((TmpFlrBuf <= cF_TOPFLR) && (FixFlrTimeCnt >= CmdFixFlrTime) && (TmpFlrBuf != sRamDArry[mcurfloor])){
			Virtual_Floor_Set(TmpFlrBuf);
			return(0);
		}
		else	TmpFlrBuf=0xff;
	}


    if(TmpFlrBuf==0xff){
    	if((CmdFixFlrTime > 0) && (cF_FIXFLOOR <= cF_TOPFLR) && (cF_FIXFLOOR != sRamDArry[mcurfloor])){
			if(FixFlrTimeCnt >= CmdFixFlrTime){
    			Virtual_Floor_Set(cF_FIXFLOOR);
			}
    	}
    }
	return(0);
}







unsigned int  __attribute__((section(".usercode"))) 	UpDnRstCheck(void)
{
	if(bUpWard){
		if(!(sRamDArry[mHighFloor] & UPDN_CAR_READY)){
            ClrUpDnWard();
            sRamDArry[mHighFloor]=0;
            sRamDArry[mNewAckStopFloor] = 0x0;
        }
	}
   	else if(bDnWard){
       	if(!(sRamDArry[mLowFloor] & UPDN_CAR_READY)){
            ClrUpDnWard();
            sRamDArry[mNewAckStopFloor] = 0x0;
            sRamDArry[mLowFloor]=0;
       	}
	}
	return(0);

}


LocalType  __attribute__((section(".usercode"))) UpMoveCheck(void)
{
    LocalType   i;

    i=0;
    if(bsUls)                                                       i=1;

    if( (sRamDArry[mHighFloor] & ONLY_FLR) <= sRamDArry[mcurfloor]) i=1;
    if(!(sRamDArry[mHighFloor] & UPDN_CAR_READY))                   i=1;
    return(i);
}



LocalType  __attribute__((section(".usercode"))) DnMoveCheck(void)
{
    LocalType   i;

    i=0;

    if(bsDls)                                                       	i=1;

    if( (sRamDArry[mLowFloor] & ONLY_FLR) >= sRamDArry[mcurfloor])  	i=1;
    if(!(sRamDArry[mLowFloor] & UPDN_CAR_READY))                    	i=1;
    return(i);
}



void  __attribute__((section(".usercode"))) AutoDoorSystem(void)
{

	if(SafetyChk() || 	(!OpenKeyCheck())){

        S3_PARKING1=0;
        sRamDArry[mDoor]= (sRamDArry[mDoor] & MAIN_SUB_CLOSE_KEY_CLEAR);

     	if(sRamDArry[mDoorSeq] >= DOOR_OPEN_WAIT){
            if(USE_CHECK == MAN_USE){
                if(sRamDArry[mDoorSeq] >= DOOR_REOPEN_CHECK){
                   	sRamDArry[mDoorSeq]=DOOR_OPEN_START;
                    DoorOpenTime=0;
		            bReOpen=1;


					if(OpenOutSet){
		            	bReOpen=0;
					}

	         			if(!bUpWard && !bDnWard){
	         				if(sRamDArry[mEqualFloor] & UP_READY)		bReOpen=0;
	         	      		else if(sRamDArry[mEqualFloor] & DN_READY)	bReOpen=0;
	         			}
	               		else if((bUpWard) && (sRamDArry[mEqualFloor] & UP_READY))	bReOpen=0;
	               		else if((bDnWard) && (sRamDArry[mEqualFloor] & DN_READY))	bReOpen=0;
                }
            }
            else{
               	sRamDArry[mDoorSeq]=DOOR_OPEN_START;
                DoorOpenTime=0;
                bReOpen=1;
            }


            if(SafetyChk()){
                if(sRamDArry[mSysStatus] > sSFT)   sRamDArry[mSysStatus]=sSFT;
            }

      	}
	}
	else if(!CloseKeyCheck()){
         if(sRamDArry[mDoorSeq] < DOOR_CLOSE_START){
	         if(bOpenDoorOk || (sRamDArry[mDoorSeq] >= DOOR_OPEN_WAIT) ){
	         	if(!bDoorJumper && !bDoorCloseOk && !bOutDate && !bMotor_Overheat){
	                DoorOpenTime = cF_OPWTTM+cF_REOPTM+1;
		         	sRamDArry[mDoorSeq]=DOOR_CLOSE_START;
					DoorCloseTime=0;
				}
	         }
		 }
         sRamDArry[mDoor]= (sRamDArry[mDoor] & MAIN_SUB_CLOSE_KEY_CLEAR);
	}



//	AutoUpDnDirectionSet();


    if(sRamDArry[mDoorSeq] == READY_ELEVATOR){
        if(bUpWard){
            if(!UpMoveCheck()){
				OneStopKey();
                sRamDArry[mNewAckStopFloor] = sRamDArry[mHighFloor];
            }
            else{
                ClrUpDnWard();
                sRamDArry[mEqualFloor]=0;
                sRamDArry[mHighFloor]=0;
                sRamDArry[mNewAckStopFloor] = 0x0;
            }
        }
        else if(bDnWard){
            if(!DnMoveCheck()){
				OneStopKey();
               	sRamDArry[mNewAckStopFloor] = sRamDArry[mLowFloor];
            }
            else{
                ClrUpDnWard();
                sRamDArry[mNewAckStopFloor] = 0x0;
                sRamDArry[mEqualFloor]=0;
                sRamDArry[mLowFloor]=0;
            }
        }
    }
}





//20051008insert
void __attribute__((section(".usercode"))) 	NoButtonAndDefaultLoad(void)
{
	if(sRamDArry[mReqStopFloor] & CAR_READY){
		sRamDArry[DEST_FLR]=sRamDArry[mAckStopFloor] = sRamDArry[mReqStopFloor];
	}
}

*/








unsigned int   __attribute__((section(".usercode")))  FhmSuccessCheck(void)
{
/*
	if(sRamDArry[FHM_RUN] == FHM_RUN_CMD){
		if(IN_LU && !IN_LD && !IN_SU1){
			if(FhmToggle && bFhmRun){
				if(sRamDArry[FHM_SEQ]== FHM_SEQ_5){
					return(0);
				}
			}
		}
	}
*/
	return(1);
}



/*
void __attribute__((section(".usercode")))  FhmRunCheck(void)
{

    if( !bAuto && IN_AUTO && CounterCheck() && !ManWorkingChk){
        if( ( (sRamDArry[FHM_RUN] == FHM_RUN_CMD) && (FhmLoaderTime < 10) ) ){
            bFhmRunReset=1;
            bFhmRun=1;
            if(sRamDArry[FHM_SEQ]==0){
                sRamDArry[FHM_SEQ]=FHM_SEQ_0;
            }
        }
        else{
 			bFhmRun=0;
		}
    }
    else{
 		bFhmRun=0;
	}


    if((bAuto) || ((sRamDArry[FHM_RUN] == 0) ) ){
        bFhmRunReset=0;
        bFhmRun=0;
    }

}




void __attribute__((section(".usercode")))  ManualUpDnKeyCheck(void)
{
	LocalType KeyTime;
	unsigned int  x,y;
	unsigned long i,j;


	if(bAuto)      KeyTime=30;
	else           KeyTime=5;

	if( (!IN_U_B && IN_D_B) && (!bCarDnMove)){
		if(UpBttonTime >= KeyTime){
			UpBttonTime = KeyTime;
			bManualUpKey=1;
		}
		DnBttonTime=0;
	}
	else if( (!IN_D_B && IN_U_B) && (!bCarUpMove)){
		if(DnBttonTime >= KeyTime){
			DnBttonTime=KeyTime;
			bManualDnKey=1;
		}
		UpBttonTime=0;
	}
	else{
		DnBttonTime=0;
		UpBttonTime=0;
		bManualUpKey=0;
		bManualDnKey=0;
	}


    FhmRunCheck();


	if(bFhmRun){


		ReadEncoder();

		if(sRamDArry[mSysStatus] >= sFHM_MODE)   sRamDArry[mSysStatus]=sFHM_MODE;

		switch(sRamDArry[FHM_SEQ]){
			case  FHM_SEQ_0:
				if(!IN_DLS){
					if(!IN_LU && IN_LD && !IN_SD1){
						FhmTime=0;
	                    sRamDArry[FHM_SEQ]=FHM_SEQ_3;
					}
					else{
	                    sRamDArry[FHM_SEQ]=FHM_SEQ_2;
					}
				}
				else{
                    sRamDArry[FHM_SEQ]=FHM_SEQ_1;
				}
                if(INVERTER_CHECK == LG) OUT_P4(0);

				break;

			case  FHM_SEQ_1:
				if(IN_DLS){
		            if(sRamDArry[mSysStatus] >= sDLS)   sRamDArry[mSysStatus]=sDLS;
				}
				else if(IN_ULS){
		            if(sRamDArry[mSysStatus] >= sULS)   sRamDArry[mSysStatus]=sULS;
				}
				else{
		    		if(sRamDArry[mSysStatus] >= sFhmError)   sRamDArry[mSysStatus]=sFhmError;
				}
	            if(INVERTER_CHECK == LG) OUT_P4(0);
				break;
			case  FHM_SEQ_2:
				if(!IN_DLS){
					if(!IN_LU && IN_LD && !IN_SD1){
						FhmTime=0;
	                    sRamDArry[FHM_SEQ]=FHM_SEQ_3;
					}
					else{
						bManualDnKey=1;
					}
				}
				else{
	            	sRamDArry[FHM_SEQ]=FHM_SEQ_1;
				}
                if(INVERTER_CHECK == LG) OUT_P4(0);
				break;
			case  FHM_SEQ_3:
				if(!IN_LU && IN_LD && !IN_SD1){
					if(FhmTime>20){
                        sRamDArry[FHM_SEQ]=FHM_SEQ_4;
                        if(INVERTER_CHECK == LG) OUT_P4(1);
                        else if(INVERTER_CHECK == IO){
                            for(FhmToggle=0;FhmToggle<32;FhmToggle++){
                                parameter_mirror[FhmToggle]= 0x0fffffff;
                            }
                            FhmToggle=0;
                            sRamDArry[mcurfloor]=0;
                        }
					}
					else if(FhmTime>10){
                        if(INVERTER_CHECK == LG) OUT_P4(1);
					}
            	}
				else{
                    sRamDArry[FHM_SEQ]=FHM_SEQ_0;
				}
				break;
			case  FHM_SEQ_4:
				if(IN_ULS)	sRamDArry[FHM_SEQ]=FHM_SEQ_1;
				else{

					if(IN_LU && !IN_LD && !IN_SU1){
						if(bFhmEncoder){
							bFhmEncoder=0;
	                        LevelEncoderPulse2=CurPulse;
						}

	                	OUT_P3(0);
	                	OUT_P1(0);
	                	OUT_P2(0);

	                    if(!bMoveCar){
	                        if(INVERTER_CHECK == LG) OUT_P4(0);

	                        if(INVERTER_CHECK == IO){
	                        	LevelEncoderPulse2=(CurPulse - LevelEncoderPulse2);

    								for(i=0;i<32;i++)    parameter_mirror[i] = (parameter_mirror[i] + LevelEncoderPulse1);

								SaveVerify=0x55;
	                            SaveFloorCnt();

	                        }

	    		    		FhmTime=0;
	                        GetFlashDataToBufRam(F_BLOCK1);
	                        b_LdTmpBufRam(F_TopFlr)    = sRamDArry[mcurfloor]; // top floor
	                        bit_LdTmpBufRamReset(F_OnOff0,bFhmOnOff);


							SaveVerify=0x55;
	                        flash_write_DspChar(F_BLOCK1);
							SaveVerify=0;


	                        GetFlashDataToBufRam(F_BLOCK2);
        					i_LdTmpBufRam(F_StopPulse0)     = (unsigned int)LevelEncoderPulse1;
							SaveVerify=0x55;
	                        flash_write_DspChar(F_BLOCK2);
							SaveVerify=0;

	                        sRamDArry[FHM_SEQ]=FHM_SEQ_5;
	                    }
					}
					else{
	                    if(INVERTER_CHECK == LG) OUT_P4(1);

						bManualUpKey=1;

	                    if(INVERTER_CHECK == IO){
	                        if(!IN_LU && !IN_LD && !FhmToggle){
	                            FhmToggle=1;

	                            if(!IN_SD1){					//modify ?
	                                sRamDArry[mcurfloor]=0;
									bFhmEncoder=0;
		                            POSCNT=0;
									BefCurEncoderPulse=0;
		                            CurPulse=BASE_PULSE;
	                            }
	                            else{
	                                sRamDArry[mcurfloor]++;
	                            }

	                            parameter_mirror[sRamDArry[mcurfloor]] =CurPulse;
	                        }

	                        if(IN_LU && !IN_LD && FhmToggle){
	                            FhmToggle=0;
	                            LevelEncoderPulse2 = CurPulse;

	                            if(parameter_mirror[0] != BASE_PULSE){
									bManualUpKey=0;
	                        		sRamDArry[FHM_SEQ]=FHM_SEQ_2;
								}
	                        }

	                        if(IN_LU && IN_LD && !bFhmEncoder && !IN_SD1){
								bFhmEncoder=1;
	                            LevelEncoderPulse1=(CurPulse - parameter_mirror[0]);
	                            LevelEncoderPulse2=(CurPulse - LevelEncoderPulse2);
	                            LevelEncoderPulse1=(LevelEncoderPulse1 / 2);
//								LevelEncoderPulse3=LevelEncoderPulse1;
	                            LevelEncoderPulse2=(LevelEncoderPulse2 / 2);
	                            LevelEncoderPulse1=(LevelEncoderPulse1 - LevelEncoderPulse2);
	                        }
	                    }
					}
				}
	            DspFloorSet();
				break;
			case  FHM_SEQ_5:
				if(FhmTime>10){
	                if(FhmToggle == 0){
	                    FhmToggle=1;
	                    FhmTime=20;
	                    FhmTime=20;
	                    FhmTime=20;
	                }
				}
				else if(FhmTime>5){
	                if(INVERTER_CHECK == LG) OUT_P4(0);
	                FhmToggle=0;
				}
				break;

			default:
				sRamDArry[FHM_RUN]=0;
                sRamDArry[FHM_SEQ]=0;
                if(INVERTER_CHECK == LG) OUT_P4(0);
				break;
		}

	}
	else{
		sRamDArry[FHM_RUN]=0;
        sRamDArry[FHM_SEQ]=0;

        if(INVERTER_CHECK == LG) OUT_P4(0);
	}


}



void __attribute__((section(".usercode")))  ManualUpDown(void)
{
    unsigned int i;
    i=1;

    if(SafetyChk()){
        i=0;
        if(sRamDArry[mSysStatus] > sSFT)   sRamDArry[mSysStatus]=sSFT;
    }

	if(bManualUpKey){
		if(!bDoorCloseOk){
            if(i==1){
            	SelectDoorClose_you();
				if(bAuto)	OUT_ERR(1);
			}
            else if(!IN_LU || !IN_LD)   SelectDoorOpen_you();
		}
        else if(!bManualStop){
    		CarUpStartCmd();
    		if(!bMoveUpOk){
    			bCarErr=1;
    		}
        }
        else{
            bCarErr=1;
        }
	}
   	else if(bManualDnKey){
		if(!bDoorCloseOk){
            if(i==1){
            	SelectDoorClose_you();
				if(bAuto)	OUT_ERR(1);
			}
            else if(!IN_LU || !IN_LD)   SelectDoorOpen_you();
        }
        else if(!bManualStop){
            CarDnStartCmd();
            if(!bMoveDnOk){
                bCarErr=1;
            }
        }
        else{
            bCarErr=1;
        }
    }
    else{
        bCarStop=1;
    }
}



unsigned int	__attribute__((section(".usercode")))  OutDateCheck(void)
{

	bOutDate=0;

	if( !AutoRunReady())			return(1);

	if(cF_year	== 0)				return(1);
	if(cF_month == 0)				return(1);
	if(cF_day 	== 0)				return(1);
	if(cF_month > 0x12)				return(1);
	if(cF_day 	> 0x31)				return(1);

	if(sRamDArry[Year]	== 0)		return(1);
	if(sRamDArry[Month] == 0)		return(1);
	if(sRamDArry[Day] 	== 0)		return(1);
	if(sRamDArry[Month] > 0x12)		return(1);
	if(sRamDArry[Day] 	> 0x31)		return(1);


	if(sRamDArry[Year]	< cF_year)	return(1);
	if(sRamDArry[Month]	< cF_month)	return(1);
	if(sRamDArry[Day]	< cF_day)	return(1);


	bOutDate=1;

	return(0);
}


unsigned int __attribute__((section(".usercode")))  CheckFloorSensor(void)
{


#ifdef	TEST_SIMULATION
    bEqualFloorError=0;
	return(0);
#else

	LocalType FloorPort;

    bEqualFloorError=0;

    if(((USE_CHECK == BAGGAGE_USE) ||  (USE_CHECK == CARLIFT_USE))){

		if((sRamDArry[mcurfloor] == 0) && IN_SD1){
            bCarErr=1;
            bEqualFloorError=1;
		}

		if((sRamDArry[mcurfloor] == cF_TOPFLR) && IN_SU1){
            bCarErr=1;
            bEqualFloorError=1;
		}

        if(!IN_SD1 || !IN_SU1){
	        if(!IN_X0 || !IN_X1 || !IN_X2 || !IN_X3 || !IN_X4 || !IN_X5 || !IN_X6){
	            bCarErr=1;
	            bEqualFloorError=1;
	        }

        	if(!IN_SD1 && !IN_SU1){
	            bCarErr=1;
	            bEqualFloorError=1;
			}
			return(0);
		}

		if(!FloorSelMethod){
	        switch(sRamDArry[mcurfloor]){
	            case    1:
	                if(IN_X0 || !IN_X1 || !IN_X2 || !IN_X3 || !IN_X4 || !IN_X5 || !IN_X6){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;
	            case    2:
	                if(IN_X1 || !IN_X0 || !IN_X2 || !IN_X3 || !IN_X4 || !IN_X5 || !IN_X6){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;
	            case    3:
	                if(IN_X2 || !IN_X0 || !IN_X1 || !IN_X3 || !IN_X4 || !IN_X5 || !IN_X6){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;
	            case    4:
	                if(IN_X3 || !IN_X0 || !IN_X1 || !IN_X2 || !IN_X4 || !IN_X5 || !IN_X6){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;
	            case    5:
	                if(IN_X4 || !IN_X0 || !IN_X1 || !IN_X2 || !IN_X3 || !IN_X5 || !IN_X6){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;
	            case    6:
	                if(IN_X5 || !IN_X0 || !IN_X1 || !IN_X2 || !IN_X3 || !IN_X4 || !IN_X6){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;
	            case    7:
	                if((IN_X6 || IN_X0) || !IN_X1 || !IN_X2 || !IN_X3 || !IN_X4 || !IN_X5){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;
	            case    8:
	                if((IN_X6 || IN_X1) || !IN_X0 || !IN_X2 || !IN_X3 || !IN_X4 || !IN_X5){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;
	            case    9:
	                if((IN_X6 || IN_X2) || !IN_X0 || !IN_X1 || !IN_X3 || !IN_X4 || !IN_X5){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;
	            case    10:
	                if((IN_X6 || IN_X3) || !IN_X0 || !IN_X1 || !IN_X2 || !IN_X4 || !IN_X5){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;
	            case    11:
	                if((IN_X6 || IN_X4) || !IN_X0 || !IN_X1 || !IN_X2 || !IN_X3 || !IN_X5){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;
	            case    12:
	                if((IN_X6 || IN_X5) || !IN_X0 || !IN_X1 || !IN_X2 || !IN_X3 || !IN_X4){
	                    bCarErr=1;
	                    bEqualFloorError=1;
	                }
	                break;

	            default:
	                bCarErr=1;
	                bEqualFloorError=1;
	                break;
	        }
		}
		else{
			FloorPort=0;
			if( !IN_X0)	FloorPort=(FloorPort | 0x01);
			if( !IN_X1)	FloorPort=(FloorPort | 0x02);
			if( !IN_X2)	FloorPort=(FloorPort | 0x04);
			if( !IN_X3)	FloorPort=(FloorPort | 0x08);
			if( !IN_X4)	FloorPort=(FloorPort | 0x10);
			if( !IN_X5)	FloorPort=(FloorPort | 0x20);

	        if(sRamDArry[mcurfloor] != FloorPort){
	            bCarErr=1;
	            bEqualFloorError=1;
			}
		}
    }

#endif

	return(0);
}


//////


void __attribute__((section(".usercode")))  PositionOkChk(void)
{
#ifdef	TEST_SIMULATION
        bsSdsErr=0;
        bsSusErr=0;
#else
    if( !IN_SU1 || (sRamDArry[mcurfloor] >= cF_TOPFLR)){
        if(!IN_SU1 && (sRamDArry[mcurfloor] == cF_TOPFLR)){
            bsSusErr=0;
        }
        else{
            bsSusErr=1;
            bCarErr=1;
        }

        if(!IN_SU1 && !IN_EMG) sRamDArry[mcurfloor]=cF_TOPFLR;        //modify ?
    }
    else if(!IN_SD1 || (sRamDArry[mcurfloor] == 0)){
        if(!IN_SD1 && (sRamDArry[mcurfloor] == 0)){
            bsSdsErr=0;
        }
        else{
            bsSdsErr=1;
            bCarErr=1;
        }

        if(!IN_SD1 && !IN_EMG) sRamDArry[mcurfloor]=0;                 //modify ?
    }
    else{
        bsSdsErr=0;
        bsSusErr=0;
    }
#endif

}

//////


void __attribute__((section(".usercode")))  DoorOpenCloseSeq(void)
{
	LocalType TmpTime;


    if((sRamDArry[mDoorSeq] >= DOOR_CLOSE_START) && (sRamDArry[mDoorSeq] < READY_ELEVATOR) && !S2_FIRE1){
        CloseOutOffTime=0;
        DoorOpenTime=0;
    }

    if((sRamDArry[mDoorSeq] >= DOOR_OPEN_START) && (sRamDArry[mDoorSeq] < DOOR_OPEN_WAIT) && !S2_FIRE1){
        DoorCloseTime=0;
    }

    if((sRamDArry[mDoorSeq] >= DOOR_OPEN_START) && (sRamDArry[mDoorSeq] < READY_ELEVATOR)){
    	CheckFloorSensor();
    }




    if((sRamDArry[mDoorSeq] >= DOOR_OPEN_START) && (sRamDArry[mDoorSeq] <= READY_ELEVATOR)){
		bMotor_Overheat=0;
		if(!S2_FIRE1 && !IN_FHM){
			bMotor_Overheat=1;

			if(sRamDArry[mDoorSeq] > DOOR_REOPEN_CHECK){
		  		sRamDArry[mDoorSeq]=DOOR_OPEN_START;
			}
		}
    }




    switch(sRamDArry[mDoorSeq]){
        case    DOOR_OPEN_START:
            sRamDArry[mDoor]= (sRamDArry[mDoor] & MAIN_SUB_CLOSE_KEY_CLEAR);

	        sRamDArry[mDoor] =(sRamDArry[mDoor] & ~UPDN_CAR_READY);

            PositionOkChk();

            if(INVERTER_CHECK == IO){
				CurPulse=FLOOR_COUNT(sRamDArry[mcurfloor]);
            }

            sRamDArry[mEqualFloor] = 0x0;

            S3_UPDN_VO1=0;
			bDoorOpenEndFind=0;
            DoorCloseTime=0;


			ThisFloorDoorCheck();

			if(bRunningOpenOn){
                SelectDoorOpen_you();
                sRamDArry[mDoorSeq] = DOOR_OPEN_CHECK;
                DoorOpenTime=0;
				bRunningOpenOn=0;
			}
			else if(S2_FIRE1 && (sRamDArry[mcurfloor] != FireBaseFloor) && (sRamDArry[mFireSeq] == FIRE_START) && bDoorCloseOk){
                sRamDArry[mDoor]=0;
                sRamDArry[mDoorSeq] = READY_ELEVATOR;
            }

			else if(NoUseNoOpen() || S1_POWER_FAIL){
                if((CloseOutOffTime >= cF_OPCLWTM) && (SolTimer > iF_DoorWaitTime)){
                    if(DoorOpenTime >= 5){
    	                SelectDoorOpen_you();
	   	                sRamDArry[mDoorSeq] = DOOR_OPEN_CHECK;
    	                DoorOpenTime=0;
                    }
                }
            	if(sRamDArry[mSysStatus] > sOPEN)   sRamDArry[mSysStatus]=sOPEN;

            }
            else{
                bSlipCheckStart=1;
                DoorOpenTime=0;
                bReOpen=0;
                bCarUpDnChButClr=0;
                sRamDArry[mDoorSeq] = DOOR_CLOSE_START;
            }
            break;
        case	DOOR_OPEN_CHECK:
            bSlipCheckStart=1;
            SelectDoorOpen_you();
			OpenKeyClear();

			if(IN_FR1){
                sRamDArry[mDoor]= (sRamDArry[mDoor] & MAIN_SUB_CLOSE_KEY_CLEAR);
            }


            if(bOpenDoorOk){
				bDoorOpenEndFind= 1;
                DoorOpenTime	= 0;

                if(USE_CHECK == MAN_USE){
	                if(bReOpen){
	                    DoorOpenTime = cF_OPWTTM;    //car
	                }
	                else{
	                    DoorOpenTime = cF_REOPTM;    //hib
	                }
				}
                else{
                    DoorOpenTime = cF_REOPTM;    //hib
                }

                bReOpen=0;
                sRamDArry[mDoorSeq] = DOOR_OPEN_WAIT;
            }
            else if(DoorOpenTime > cF_OPTTTM){
                if(OpenTotalUseChk){
                    sRamDArry[mDoorSeq] = DOOR_OPEN_WAIT;
                    DoorOpenTime = cF_REOPTM;
                }
            }

            if((DoorOpenTime > 60) && (bDoorCloseOk)){
                sRamDArry[mDoorSeq] = DOOR_OPEN_WAIT;
   	            DoorOpenTime 	= cF_OPWTTM + cF_REOPTM;
            }

            if(DoorOpenTime > 10){
                bCarUpDnChButClr=0;
            }

            break;
		case	DOOR_OPEN_WAIT:
            bDoorOpenEnd=1;
            bCarUpDnChButClr=0;
            DoorCloseTime=0;
            OpenKeyClear();

			if(OpenOutSet){
	            SelectDoorOpen_you();
			}

            if(sRamDArry[mSysStatus] > sOPEN)   sRamDArry[mSysStatus]=sOPEN;


			OutDateCheck();


			if((OneDoorSenserCloseChk()) && !(!IN_FR1 && !IN_FR2) && DoorJumperChkOnOffChk){
			  	sRamDArry[mDoorSeq]=DOOR_REOPEN_CHECK;
				bDoorJumper=1;
				if(DoorJumperNm > 0)	DoorJumperNm=(DoorJumperNm-1);
			}

			if(bDoorJumper==0){
				if((DoorOpenTime > (cF_OPWTTM + cF_REOPTM))){
	                if(OpenWaitUseChk){
	    				sRamDArry[mDoorSeq] = DOOR_REOPEN_CHECK;
						DoorOpenTime=0;   ///
	                }
				}
			}
            break;
        case	DOOR_REOPEN_CHECK:
            if(bDoorJumper || bOutDate || bMotor_Overheat){
                SelectDoorOpen_you();
                sRamDArry[mDoorSeq]=DOOR_REOPEN_CHECK;
            }
            else{
                DoorCloseTime=0;

                if(S3_VIP1){
		            SelectDoorOpen_you();
					DoorOpenTime=cF_OPWTTM+1;
                    UpDnRstCheck();
                }
	            else{
                    if(DoorOpenTime > cF_REOPTM){             ////
        		        sRamDArry[mDoorSeq] = DOOR_CLOSE_START;
                    }
                }
                if(sRamDArry[mSysStatus] > sOPEN)   sRamDArry[mSysStatus]=sOPEN;
            }
            break;
        case  DOOR_CLOSE_START:

            bDoorOpenEnd=0;

            if(sRamDArry[mHighFloor] | sRamDArry[mLowFloor])	S3_UPDN_VO1=1;

            if(sRamDArry[mSysStatus] > sCLOSE)   sRamDArry[mSysStatus]=sCLOSE;

			if(DoorCloseTime >= 5){
				sRamDArry[mDoorSeq]=DOOR_CLOSE_CHECK;
				DoorCloseTime=0;
            }

            break;
        case  DOOR_CLOSE_CHECK:
			if(bDoorJumper == 0){

                SelectDoorClose_you();                      //4

    	        if(!CloseTotalUseChk)   DoorCloseTime=0;


                if(bDoorCloseOk){
                    sRamDArry[mDoorSeq]=DOOR_CLOSE_END;
                    DoorCloseTime=0;
                    bsHdsNoOn=0;
                    bsCleNoOn=0;
                    UpDnRstCheck();

					#ifdef	TEST_SIMULATION
                        if(!IN_X5)	sRamDArry[mDoorSeq]=DOOR_OPEN_START;
					#endif

                }

                else if(DoorCloseTime > cF_OPTTTM){
                    if((USE_CHECK == BAGGAGE_USE) || (USE_CHECK == CARLIFT_USE)){
                        bsCleNoOn=1;
           	            bCarErr=1;
                    }
                    else{
                        sRamDArry[mDoorSeq]=DOOR_OPEN_START;
                        bReOpen=1;
                    }
                }

            }
			else{
	    		sRamDArry[mDoorSeq] = DOOR_REOPEN_CHECK;
			}
            break;
        case  DOOR_CLOSE_END:
            SelectDoorClose_you();   //5
            if(bDoorCloseOk){
                if(DoorCloseTime > 5){
                    sRamDArry[mDoorSeq]=READY_ELEVATOR;
                }
            }
            else{
                sRamDArry[mDoorSeq]=DOOR_CLOSE_CHECK;
                DoorCloseTime=0;
            }
            break;
    }
}








unsigned int  __attribute__((section(".usercode"))) TestUpDn(void)
{
	ManualUpDnKeyCheck();


   	if(bManualUpKey){
		if(sRamDArry[mcurfloor] < cF_TOPFLR){
			if(sRamDArry[mHighFloor] == 0x0){
				sRamDArry[mHighFloor] = (cF_TOPFLR | CAR_READY);
     	    	bTestKey=1;
			}
		}
   	}
    else if(bManualDnKey){
        if(sRamDArry[mcurfloor] > 0){
			if(sRamDArry[mLowFloor] == 0x0){
				sRamDArry[mLowFloor] = CAR_READY;
     	    	bTestKey=1;
			}
		}
   	}
   	else{
        if(bTestKey){
            sRamDArry[mLowFloor]  = 0x0;
            sRamDArry[mHighFloor] = 0x0;
            sRamDArry[mEqualFloor]= 0x0;
            bTestKey=0;
		}
   	}
	return(0);
}







void  __attribute__((section(".usercode"))) SlipCheck(void)
{
	if( bSlipCheckStart && (IN_LU && IN_LD) && (OneDoorSenserNoCloseChk()) ){
        bMoveOk=0;
        bMoveUpOk=0;
        bMoveDnOk=0;
        bsSlip=1;
        bCarErr=1;
        bCarStopNoRun=1;

	}
}




void  __attribute__((section(".usercode"))) OilTypeStopChk(void)
{

	if(!OilLopeTypeChk){
		if( bsNextFloor || bsLuLdNoOff){
	        bMoveOk=0;
	        bMoveUpOk=0;
	        bMoveDnOk=0;
	        bCarErr=1;
	        bCarStopNoRun=1;
		}
	}

}


void __attribute__((section(".usercode")))  NextFloorCheck(void)
{
   if(sRamDArry[mBefcurfloor] != sRamDArry[mcurfloor]){
      NextFloorTime=0;
      sRamDArry[mBefcurfloor] = sRamDArry[mcurfloor];

      	if(OilLopeTypeChk){
	    	bsNextFloor=0;
	      	ErrFlagClr();
		}
   }

   if(!bMoveCar){
      NextFloorTime=0;
   }
   else if(NextFloorTime> (BASE_NEXT_FLR_TIME + (cF_NEXTFLRTIME * 10))){
        bsNextFloor=1;
		bCarErr=1;
   }
}



*/







void  __attribute__((section(".usercode")))   AddressRead(void)
{

    MyGroupAddr=cF_GroupNm;
    MyLocalAddr=cF_LocalNm;
}









/*

void __attribute__((section(".usercode")))  InPutRead(void)
{
    unsigned int i;

    LocalType tmp;


    OUT_ON=0;

    PORTD=0xffff;
    i=PORTD;

    tmp=(UserDataType)(i & 0x00ff);
    EmgByte=tmp;

	i=(i >> 8);
    tmp=(UserDataType)(i & 0x00ff);
    Su1Byte=tmp;


    tmp=0;
    if(PORTBbits.RB2)    tmp = tmp | 0x01;  //IN_RG
    if(PORTBbits.RB6)    tmp = tmp | 0x02;  //IN_BAT
    if(PORTBbits.RB7)    tmp = tmp | 0x04;  //IN_PASS
    if(PORTAbits.RA9)    tmp = tmp | 0x08;  //IN_FIRE
    if(PORTAbits.RA10)   tmp = tmp | 0x10;  //IN_WATER
    if(PORTAbits.RA14)   tmp = tmp | 0x20;  //IN_EX0
    if(PORTAbits.RA15)   tmp = tmp | 0x40;  //IN_EX1
    if(PORTFbits.RF6)    tmp = tmp | 0x80;  //IN_EX2

    GrByte=tmp;


	RD_WR=0;

    IN_3=0;
    IN_3=0;
    IN_3=0;
    IN_3=0;



    i=PORTB;
    tmp=(UserDataType)(i>>8);
    FireByte=tmp;

    IN_3=1;


    IN_0=0;
    IN_0=0;
    IN_0=0;
    IN_0=0;

    i=PORTB;
    tmp=(UserDataType)(i>>8);

    if(INVERTER_CHECK == LG){
	    if(Bef_I_FS0 == tmp){
	        if(I_FS0_Cht >= 10){
	                I_FS0_bit = ~tmp;
	                Bef_I_FS0 = tmp;
	                I_FS0_Cht = 0;
	        }
	    }
	    else{
	        I_FS0_Cht=0;
	        Bef_I_FS0=tmp;
	    }
	}
	else{
		I_FS0_bit = tmp;
	}



    IN_0=1;
    IN_0=1;


    IN_1=0;
    IN_1=0;
    IN_1=0;
    IN_1=0;

    i=PORTB;
    tmp=(UserDataType)(i>>8);
    X0Byte=tmp;

    IN_1=1;



    if(INVERTER_CHECK != LG){

	    if(BEF_IN_FS0_IN == IN_FS0_IN){
	        if(Fs0Time>20){
	            IN_FS0_IN=BEF_IN_FS0_IN;
	            Fs0Time=0;
	        }
	    }
	    else{
	        BEF_IN_FS0_IN=IN_FS0_IN;
	        Fs0Time=0;
	    }


	    if(BEF_IN_FS1_IN == IN_FS1_IN){
	        if(Fs1Time>20){
	            IN_FS1_IN=BEF_IN_FS1_IN;
	            Fs1Time=0;
	        }
	    }
	    else{
	        BEF_IN_FS1_IN=IN_FS1_IN;
	        Fs1Time=0;
	    }

	    if(BEF_IN_FS2_IN == IN_FS2_IN){
	        if(Fs2Time>20){
	            IN_FS2_IN=BEF_IN_FS2_IN;
	            Fs2Time=0;
	        }
	    }
	    else{
	        BEF_IN_FS2_IN=IN_FS2_IN;
	        Fs2Time=0;
	    }

	    if(BEF_IN_FS3_IN == IN_FS3_IN){
	        if(Fs3Time>20){
	            IN_FS3_IN=BEF_IN_FS3_IN;
	            Fs3Time=0;
	        }
	    }
	    else{
	        BEF_IN_FS3_IN=IN_FS3_IN;
	        Fs3Time=0;
	    }

	    if(BEF_IN_FS4_IN == IN_FS4_IN){
	        if(Fs4Time>20){
	            IN_FS4_IN=BEF_IN_FS4_IN;
	            Fs4Time=0;
	        }
	    }
	    else{
	        BEF_IN_FS4_IN=IN_FS4_IN;
	        Fs4Time=0;
	    }


	    if(BEF_IN_DER_IN == IN_DER_IN){
	        if(DerTime>20){
	            IN_DER_IN=BEF_IN_DER_IN;
	            DerTime=0;
	        }
	    }
	    else{
	        BEF_IN_DER_IN=IN_DER_IN;
	        DerTime=0;
	    }


	    if(BEF_IN_FID_IN == IN_FID_IN){
	        if(FidTime>20){
	            IN_FID_IN=BEF_IN_FID_IN;
	            FidTime=0;
	        }
	    }
	    else{
	        BEF_IN_FID_IN=IN_FID_IN;
	        FidTime=0;
	    }

	    if(BEF_IN_UND1_IN == IN_UND1_IN){
	        if(UndTime>20){
	            IN_UND1_IN=BEF_IN_UND1_IN;
	            UndTime=0;
	        }
	    }
	    else{
	        BEF_IN_UND1_IN=IN_UND1_IN;
	        UndTime=0;
	    }
	}

//////////////////////
    if(BEF_IN_EMG_IN == IN_EMG_IN){
        if(EmgTime>20){
            IN_EMG_PORT=BEF_IN_EMG_IN;
            EmgTime=0;
        }
    }
    else{
        BEF_IN_EMG_IN=IN_EMG_IN;
        EmgTime=0;
    }


    if(BEF_IN_PRK_IN == IN_PRK_IN){
        if(PrkTime>20){
            IN_PRK_PORT=BEF_IN_PRK_IN;
            PrkTime=0;
        }
    }
    else{
        BEF_IN_PRK_IN=IN_PRK_IN;
        PrkTime=0;
    }


    if(BEF_IN_AUTO_IN == IN_AUTO_IN){
        if(AtTime>20){
            IN_AUTO_PORT=BEF_IN_AUTO_IN;
            AtTime=0;
        }
    }
    else{
        BEF_IN_AUTO_IN=IN_AUTO_IN;
        AtTime=0;
    }


    if(BEF_IN_U_B_IN == IN_U_B_IN){
        if(UbTime>10){
            IN_U_B_PORT=BEF_IN_U_B_IN;
            UbTime=0;
        }
    }
    else{
        BEF_IN_U_B_IN=IN_U_B_IN;
        UbTime=0;
    }


    if(BEF_IN_D_B_IN == IN_D_B_IN){
        if(DbTime>10){
            IN_D_B_PORT=BEF_IN_D_B_IN;
            DbTime=0;
        }
    }
    else{
        BEF_IN_D_B_IN=IN_D_B_IN;
        DbTime=0;
    }



    if(BEF_IN_OP_E_IN == IN_OP_E_IN){
        if(OlsTime>10){
            IN_OP_E_PORT=BEF_IN_OP_E_IN;
            OlsTime=0;
        }
    }
    else{
        BEF_IN_OP_E_IN=IN_OP_E_IN;
        OlsTime=0;
    }


    if(BEF_IN_CL_E_IN == IN_CL_E_IN){
        if(GsTime>10){
            IN_CL_E_PORT=BEF_IN_CL_E_IN;
            GsTime=0;
        }
    }
    else{
        BEF_IN_CL_E_IN=IN_CL_E_IN;
        GsTime=0;
    }


    if(BEF_IN_HL_D_IN == IN_HL_D_IN){
        if(DsTime>10){
            IN_HL_D_PORT=BEF_IN_HL_D_IN;
            DsTime=0;
        }
    }
    else{
        BEF_IN_HL_D_IN=IN_HL_D_IN;
        DsTime=0;
    }




//////////////////////

    if(BEF_IN_SU1_IN == IN_SU1_IN){
        if(Su1Time>10){
            IN_SU1_PORT=BEF_IN_SU1_IN;
            Su1Time=0;
        }
    }
    else{
        BEF_IN_SU1_IN=IN_SU1_IN;
        Su1Time=0;
    }


    if(BEF_IN_SD1_IN == IN_SD1_IN){
        if(Sd1Time>10){
            IN_SD1_PORT=BEF_IN_SD1_IN;
            Sd1Time=0;
        }
    }
    else{
        BEF_IN_SD1_IN=IN_SD1_IN;
        Sd1Time=0;
    }




    if(BEF_IN_SFT_IN == IN_SFT_IN){
        if(SftTime>10){
            IN_SFT_PORT=BEF_IN_SFT_IN;
            SftTime=0;
        }
    }
    else{
        BEF_IN_SFT_IN=IN_SFT_IN;
        SftTime=0;
    }




    if(BEF_IN_OVL_IN == IN_OVL_IN){
        if(OvlTime>20){
            IN_OVL_PORT=BEF_IN_OVL_IN;
            OvlTime=0;
        }
    }
    else{
        BEF_IN_OVL_IN=IN_OVL_IN;
        OvlTime=0;
    }



    if(BEF_IN_ULS_IN == IN_ULS_IN){
        if(UlsTime>10){
            IN_ULS_PORT=BEF_IN_ULS_IN;
            UlsTime=0;
        }
    }
    else{
        BEF_IN_ULS_IN=IN_ULS_IN;
        UlsTime=0;
    }


    if(BEF_IN_DLS_IN == IN_DLS_IN){
        if(DlsTime>10){
            IN_DLS_PORT=BEF_IN_DLS_IN;
            DlsTime=0;
        }
    }
    else{
        BEF_IN_DLS_IN=IN_DLS_IN;
        DlsTime=0;
    }


    if(BEF_IN_LU_IN == IN_LU_IN){
        if(LuTime>=10){
            IN_LU_PORT=BEF_IN_LU_IN;
            LuTime=0;
        }
    }
    else{
        BEF_IN_LU_IN=IN_LU_IN;
        LuTime=0;
    }


    if(BEF_IN_LD_IN == IN_LD_IN){
        if(LdTime>=10){
            IN_LD_PORT=BEF_IN_LD_IN;
            LdTime=0;
        }
    }
    else{
        BEF_IN_LD_IN=IN_LD_IN;
        LdTime=0;
    }

//////////////////////


    if(BEF_IN_GR_IN == IN_GR_IN){
        if(RgTime>20){
            IN_GR_PORT=BEF_IN_GR_IN;
            RgTime=0;
        }
    }
    else{
        BEF_IN_GR_IN=IN_GR_IN;
        RgTime=0;
    }



    if(BEF_IN_BAT_IN == IN_BAT_IN){
        if(BatTime>10){
            IN_BAT_PORT=BEF_IN_BAT_IN;
            BatTime=0;
        }
    }
    else{
        BEF_IN_BAT_IN=IN_BAT_IN;
        BatTime=0;
    }



    if(BEF_IN_PASS_IN == IN_PASS_IN){
        if(PassTime>20){
            IN_PASS_PORT=BEF_IN_PASS_IN;
            PassTime=0;
        }
    }
    else{
        BEF_IN_PASS_IN=IN_PASS_IN;
        PassTime=0;
    }


    if(BEF_IN_FIRE_IN == IN_FIRE_IN){
        if(FireTime>20){
            IN_FIRE_PORT=BEF_IN_FIRE_IN;
            FireTime=0;
        }
    }
    else{
        BEF_IN_FIRE_IN=IN_FIRE_IN;
        FireTime=0;
    }


    if(BEF_IN_WATER_IN == IN_WATER_IN){
        if(WaterTime>20){
            IN_WATER_PORT=BEF_IN_WATER_IN;
            WaterTime=0;
        }
    }
    else{
        BEF_IN_WATER_IN=IN_WATER_IN;
        WaterTime=0;
    }



    if(BEF_IN_FULL_IN == IN_FULL_IN){
        if(FullTime>20){
            IN_FULL_PORT=BEF_IN_FULL_IN;
            FullTime=0;
        }
    }
    else{
        BEF_IN_FULL_IN=IN_FULL_IN;
        FullTime=0;
    }


    if(BEF_IN_MM_IN == IN_MM_IN){
        if(MmTime>10){
            IN_MM_PORT=BEF_IN_MM_IN;
            MmTime=0;
        }
    }
    else{
        BEF_IN_MM_IN=IN_MM_IN;
        MmTime=0;
    }



    if(BEF_IN_FHM_IN == IN_FHM_IN){
        if(FhmInTime>20){
            IN_FHM_PORT=BEF_IN_FHM_IN;
            FhmInTime=0;
        }
    }
    else{
        BEF_IN_FHM_IN=IN_FHM_IN;
        FhmInTime=0;
    }

//////////////////////


    if(BEF_IN_FR1_IN == IN_FR1_IN){
        if(Fr1Time>20){
            IN_FR1_PORT=BEF_IN_FR1_IN;
            Fr1Time=0;
        }
    }
    else{
        BEF_IN_FR1_IN=IN_FR1_IN;
        Fr1Time=0;
    }

    if(BEF_IN_FR2_IN == IN_FR2_IN){
        if(Fr2Time>20){
            IN_FR2_PORT=BEF_IN_FR2_IN;
            Fr2Time=0;
        }
    }
    else{
        BEF_IN_FR2_IN=IN_FR2_IN;
        Fr2Time=0;
    }



    if(BEF_IN_GS2_IN == IN_GS2_IN){
        if(Gs2Time>20){
            IN_GS2_PORT=BEF_IN_GS2_IN;
            Gs2Time=0;
        }
    }
    else{
        BEF_IN_GS2_IN=IN_GS2_IN;
        Gs2Time=0;
    }


    if(BEF_IN_DS2_IN == IN_DS2_IN){
        if(Ds2Time>20){
            IN_DS2_PORT=BEF_IN_DS2_IN;
            Ds2Time=0;
        }
    }
    else{
        BEF_IN_DS2_IN=IN_DS2_IN;
        Ds2Time=0;
    }



    if(BEF_IN_SU2_IN == IN_SU2_IN){
        if(Su2Time>10){
            IN_SU2_PORT=BEF_IN_SU2_IN;
            Su2Time=0;
        }
    }
    else{
        BEF_IN_SU2_IN=IN_SU2_IN;
        Su2Time=0;
    }

    if(BEF_IN_SD2_IN == IN_SD2_IN){
        if(Sd2Time>10){
            IN_SD2_PORT=BEF_IN_SD2_IN;
            Sd2Time=0;
        }
    }
    else{
        BEF_IN_SD2_IN=IN_SD2_IN;
        Sd2Time=0;
    }



    if(BEF_IN_BM_IN == IN_BM_IN){
        if(BmTime>10){
            IN_BM_PORT=BEF_IN_BM_IN;
            BmTime=0;
        }
    }
    else{
        BEF_IN_BM_IN=IN_BM_IN;
        BmTime=0;
    }


    if(BEF_IN_INV_IN == IN_INV_IN){
        if(InvTime>10){
            IN_INV_PORT=BEF_IN_INV_IN;
            InvTime=0;
        }
    }
    else{
        BEF_IN_INV_IN=IN_INV_IN;
        InvTime=0;
    }


    if(cF_PcbType == 1){
        IN_SU2_PORT =1;
        IN_SD2_PORT =1;
        IN_BM_PORT  =1;
        IN_INV_PORT =1;
    }


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



	if(ExIOTimer > 30)	sRamDArry[mExtIN0]= 0xff;

    OldFireBuf = 0x00;

    if((I_GR_bit   & 0x01))  OldFireBuf=(OldFireBuf | 0x01);
    if((I_FIRE_bit & 0x01))  OldFireBuf=(OldFireBuf | 0x02);
    if((I_FIRE_bit & 0x02))  OldFireBuf=(OldFireBuf | 0x20);
    if((I_FIRE_bit & 0x04))  OldFireBuf=(OldFireBuf | 0x40);
    if((I_FIRE_bit & 0x08))  OldFireBuf=(OldFireBuf | 0x80);

    OldFireBuf=(OldFireBuf | 0x04);

    sRamDArry[I_EMG]  = I_EMG_bit;
    sRamDArry[I_SU1]  = I_SU1_bit;
    sRamDArry[I_GR]   = I_GR_bit;
    sRamDArry[I_FIRE] = I_FIRE_bit;
    sRamDArry[I_X_0]  = I_X0_bit;
    sRamDArry[I_FS0]  = I_FS0_bit;
}




unsigned int  __attribute__((section(".usercode")))  UserLampSetCheck(unsigned int val)
{
	unsigned int i;

	i=0;

   	switch(val){
      	case  0:
         		if(bAuto)           i=1;
         		break;
      	case  1:
         		if(!bAuto)          i=1;
         		break;
      	case  2:
         		if(S1_OVERLOAD1)    i=1;
         		break;
      	case  3:
         		if(S2_FIRE1)        i=1;
         		break;
      	case  4:
         		if(S1_EMG1)         i=1;
         		break;
      	case  5:
         		if(S3_PARKING1)     i=1;
         		break;
      	case  6:
         		if(S3_VIP1)         i=1;
         		break;
      	case  7:
                if( AutoRunReady() && (sRamDArry[mDoorSeq] != READY_ELEVATOR))  i=1;
         		break;
      	case  8:
         		if(S3_FULL1)     	i=1;
         		break;
      	default:
         		i=0;
         		break;
   	}

	return(i);
}


void  __attribute__((section(".usercode")))  	UserLampSet(void)
{
	unsigned int i;

 	i=UserLampSetCheck((unsigned int)(cF_USERLAMP1));
	if(i)	S2_LAMP_USER1=1;
	else	S2_LAMP_USER1=0;

 	i=UserLampSetCheck((unsigned int)(cF_USERLAMP2));
	if(i)	S1_MANUAL1=1;
	else	S1_MANUAL1=0;

 	i=UserLampSetCheck((unsigned int)(cF_USERLAMP3));
	if(i)	S4_USERLAMP3=1;
	else	S4_USERLAMP3=0;

	i=UserLampSetCheck((unsigned int)(cF_USERLAMP4));
	if(i)	S4_USERLAMP4=1;
	else	S4_USERLAMP4=0;
}




void  __attribute__((section(".usercode")))   VoiceStart(void)
{
    LocalType voice_start,bell_start;

    voice_start=1;
	bell_start=1;

    if(bAuto && bUnd){
        if((sRamDArry[mDoorSeq] >= WAIT_NEXT_LULD)){
            if((sRamDArry[DEST_FLR] & ONLY_FLR) == (sRamDArry[mcurfloor] & ONLY_FLR)){
                if((cF_VOICETIME > 0) && (VoiceTime < (cF_VOICETIME + 10)) && (VoiceTime > cF_VOICETIME)){
                    voice_start=0;
                }

                if(BellTime < 10){
                    bell_start=0;
                }
            }
			else{
				BellTime=0;
				VoiceTime=0;
			}
        }
    }



	if((iF_SolOnTime > 3) && (iF_DoorWaitTime > 3)){
        if(bAuto && !bManualStop){
			if((sRamDArry[mDoorSeq] >= DOOR_CLOSE_START) && (sRamDArry[mDoorSeq] <= DOOR_CLOSE_END)){
				if(!IN_DS2 || bHoleDoorCloseEnd || bDoorCloseOk){      			//close end
					if(SolWaitTimer > iF_DoorWaitTime){
						OUT_BELL(0);
						SolTimer = iF_SolOnTime;
					}
				}
				else if(!IN_GS2){   		// decrease
					if(SolTimer < iF_SolOnTime){
						SolWaitTimer=0;
						OUT_BELL(1);
					}
					else		OUT_BELL(0);
				}
				else{
					OUT_BELL(0);
					SolTimer=0;
				}
				bSolControl=0;
			}
			else if(sRamDArry[mDoorSeq] < DOOR_CLOSE_START){
				if(!IN_GS2){   		// decrease
					if( !bSolControl){
						bSolControl=1;
						SolTimer=0;
					}
				}
				else{
					bSolControl=0;
					SolTimer=100;
				}

				if(SolTimer < iF_SolOnTime){
					if(SolTimer > 3){
						OUT_BELL(1);
					}
				}
				else{
					OUT_BELL(0);
				}
			}
			else{
				OUT_BELL(0);
				bSolControl=0;
				SolTimer=0;
			}
		}
		else{
			OUT_BELL(0);
			bSolControl=0;
			SolTimer=0;
			if(bDoorCloseOk)	sRamDArry[mDoorSeq] = DOOR_CLOSE_END;
		}
	}
	else{
		bSolControl=0;
		SolTimer=100;
	}



    if(!voice_start){
        S2_FLOW1=1;
    }
    else{
        S2_FLOW1=0;
    }

    if(!bell_start){
		if((iF_SolOnTime == 0) || (iF_DoorWaitTime == 0))	OUT_BELL(1);
    }
    else{
		if((iF_SolOnTime == 0) || (iF_DoorWaitTime == 0))	OUT_BELL(0);
    }

}




void  __attribute__((section(".usercode"))) 	ServiceCheck(void)
{

	sRamDArry[FLR_ON_OFF0] =(sRamDArry[ExtCmdFlrONOFF0] & BaggageFlrOnOff[0]);
	sRamDArry[FLR_ON_OFF1] =(sRamDArry[ExtCmdFlrONOFF1] & BaggageFlrOnOff[1]);
	sRamDArry[FLR_ON_OFF2] =(sRamDArry[ExtCmdFlrONOFF2] & BaggageFlrOnOff[2]);
	sRamDArry[FLR_ON_OFF3] =(sRamDArry[ExtCmdFlrONOFF3] & BaggageFlrOnOff[3]);



	if((sRamDArry[mVFlr1] != sRamDArry[mVFlr2]) && (sRamDArry[mVFlrCnt] > 0)){
		if((sRamDArry[mVFlr1] > 0) && (sRamDArry[mVFlr2] > 0)){
			if((cF_FLRDSPCH(0) == 'Z') && (cF_FLRDSPCH(1) == 'Z')){
				AccTimer=0;
				sRamDArry[FLR_ON_OFF0]=0x0;
				sRamDArry[FLR_ON_OFF1]=0x0;
				sRamDArry[FLR_ON_OFF2]=0x0;
				sRamDArry[FLR_ON_OFF3]=0x0;
			}
		}
	}


	if(S2_FIRE1){
		sRamDArry[FLR_ON_OFF0] = 0xff;
		sRamDArry[FLR_ON_OFF1] = 0xff;
		sRamDArry[FLR_ON_OFF2] = 0xff;
		sRamDArry[FLR_ON_OFF3] = 0xff;
	}
}





void    __attribute__((section(".usercode"))) WaterCheck(void)
{

    unsigned char watersafefloor,tmpwatersafefloor;

    watersafefloor=cF_WATERSAFEFLR;

    if(cF_WATERSAFEFLR > cF_TOPFLR){
        watersafefloor=cF_TOPFLR;
    }

   	if(!IN_WATER && bAuto && !S3_PARKING1){
      	if((sRamDArry[mcurfloor] == watersafefloor) && !bMoveCar){
                if(bOnLuLd && bDoorOpenEnd){
                  	ClrUpDnWard();
                   	bExtButClr=1;
                    bCarButClr=1;
                    bWaterSen=1;
             		if(sRamDArry[mDoorSeq] >= DOOR_REOPEN_CHECK){
                		sRamDArry[mDoorSeq]=DOOR_OPEN_START;
             		}
                }
                else if(bOnLuLd && (sRamDArry[mDoorSeq] > DOOR_OPEN_WAIT)){
                    sRamDArry[mDoorSeq] = DOOR_OPEN_START;
                }

            	if(sRamDArry[mSysStatus] > sWaterSense)   sRamDArry[mSysStatus]=sWaterSense;
      	}
      	else{
	            if(sRamDArry[mcurfloor] < watersafefloor){
	                sRamDArry[mHighFloor]=(watersafefloor | CAR_READY);
	            }
	            else if(sRamDArry[mcurfloor] > watersafefloor){
	                sRamDArry[mLowFloor]=(watersafefloor | CAR_READY);
	            }

        }
	}
    else{
        bWaterSen=0;
    }
}



void  __attribute__((section(".usercode")))  InitialInPortChk(void)
{

    if(WarmingUpTime < 100){
        if(!IN_U_B || !IN_D_B ){
            if(WarmingUpTime >= 80) WarmingUpTime=80;

            if(sRamDArry[mSysStatus] > sInPortError)   sRamDArry[mSysStatus]=sInPortError;
            bCarErr=1;
        }
    }

}


void  __attribute__((section(".usercode")))  AllReadyCheck(void)
{
    LocalType readygood;

    readygood=1;

    if(!AutoRunReady())                         readygood=0;
    if(!bDoorCloseOk)                           readygood=0;
    if(sRamDArry[mDoorSeq] != READY_ELEVATOR)   readygood=0;


    if(readygood==0){
        MinuteTime=0;
        FixFlrTimeCnt=0;
        LightTime=0;
    }

    if(ElevStopTime>10){
        bCarUpDnChButClr=0;
    }

}


void  __attribute__((section(".usercode")))  FanOnOff(void)
{
    if(bCarUpMove || bCarDnMove || !bDoorCloseOk){
        OUT_FAN(1);
        FanTime=0;
    }
    else if(FanTime > cF_FANTIME){
        FanTime = cF_FANTIME +1;
        OUT_FAN(0);
    }
}


void  __attribute__((section(".usercode")))  BuzOnOff(void)
{
    if(IN_FR1 || IN_FR2){

        if(USE_CHECK == BAGGAGE_USE){
            if(S1_OVERLOAD1){
                OUT_BUZ(1);
            }
            else if(bAuto && bOnLuLd && !bMoveCar && SafetyChk()){
                OUT_BUZ(1);
            }


            else if(bAuto && bOnLuLd && !bMoveCar && (TmpBuzor < 10)){
                OUT_BUZ(1);
            }


            else	OUT_BUZ(0);
        }
        else if(USE_CHECK == CARLIFT_USE){
            if(S1_OVERLOAD1){
                OUT_BUZ(1);
            }
            else if(bAuto && SafetyChk()){
                OUT_BUZ(1);
            }
            else	OUT_BUZ(0);
        }
        else{
            if(S1_OVERLOAD1)    OUT_BUZ(1);
            else                OUT_BUZ(0);
        }
    }

    if(!bAuto){
        OUT_BUZ(0);
    }
}




void  __attribute__((section(".usercode")))  EtcStopSet(void)
{

    S3_STOP1=0;

    if( bAuto && bOneLuOrLd && !bMoveCar){
		if(!IN_WATER || S1_POWER_FAIL || (sRamDArry[mDoor] & 0x10)){
        	S3_STOP1=1;
		}

		if( !(sRamDArry[mExtIN0] & 0x80)){
        	S3_STOP1=1;
			if(sRamDArry[mDoorSeq] > DOOR_REOPEN_CHECK){
		  		sRamDArry[mDoorSeq]=DOOR_OPEN_START;
			}
		}
    }

    if(sRamDArry[mDoorSeq] >= READY_ELEVATOR)	sRamDArry[mDoor]= (sRamDArry[mDoor] & 0xef);
    if( !AutoRunReady())    					sRamDArry[mDoor]= (sRamDArry[mDoor] & 0xef);
}



void  __attribute__((section(".usercode")))  WaitAndNotMoveChk(void)
{
    if(WarmingUpTime < 100){
        bCarErr=1;
        CurFTime=0;
        NextFTime=0;
    }

    if((cF_ERRSTOPCNT < eDArry[eErrCntPt]) && (cF_ERRSTOPCNT != 0xff)){
        bCarStopNoRun=1;
        bInspect=1;
    }
}





unsigned int  __attribute__((section(".usercode")))  BreakMgtOpenCheck(void)
{
	#ifdef	TEST_SIMULATION
		return(0);
	#else

		if(CurSelOutPortChk(cF_BRK)){
			if(bBK1==0){
				BreakTime=0;
				bBK1=1;
			}
		}
		else{
			if(bBK1==1){
				BreakTime=0;
				bBK1=0;
			}
		}


			if(!BrkMgtUseChk )	return(0);

			if(!bAuto)		return(0);


		if(bBK1==1){
    		if( (BreakTime > cF_BRK_MGT_TIME) && IN_MM){
	            bsBreakMgtOpen=1;
			 }
		}

		else{
    		if( (BreakTime > cF_BRK_MGT_TIME) && !IN_MM){
	            bsBreakMgtOpen=1;
			 }
		}

		if(bsBreakMgtOpen)	bCarStopNoRun=1;

	#endif

}


unsigned int    __attribute__((section(".usercode")))  BreakOpenCheck(void)
{

	#ifdef	TEST_SIMULATION
		return(0);
	#else

			if(!BrkOpenUseChk )	return(0);

			    if(!bAuto)		return(0);


			if(bBK1==1){
	    		if( (BreakTime > cF_BRK_MGT_TIME) && IN_BM){
		            bsBreakOpen=1;
				 }
			}

			else{
	    		if( (BreakTime > cF_BRK_MGT_TIME) && !IN_BM){
		            bsBreakOpen=1;
				 }
			}


		if(bsBreakOpen)	bCarStopNoRun=1;

	#endif

}






void  __attribute__((section(".usercode")))   InvErrChk(void)
{

    LocalType rst;

    if(OilLopeTypeChk){
        if(!IN_INV){
			bCarErr=1;
			bsInvertErr=1;
		}
		else{
			bsInvertErr=0;
		}

    }
    else{
        bsInvertErr=0;
        bInvErrCnt=0;
        InvErrTime=0;
        RstCnt=0;
    }
}
*/


unsigned int	__attribute__((section(".usercode")))	PcCmdchk(void)
{
/*

	if( !(CrtMoveFlr & CAR_READY))	return(0);

	if(sRamDArry[mcurfloor] == (CrtMoveFlr & ONLY_FLR)){
    	if( (sRamDArry[mDoorSeq] >= DOOR_OPEN_START) && (sRamDArry[mDoorSeq] <= DOOR_OPEN_WAIT)){
			CrtMoveFlr=0;
		}
	}

	if(bExtButClr)	CrtMoveFlr=0;
	if( (CrtMoveFlr & ONLY_FLR) > cF_TOPFLR)	CrtMoveFlr=0;

	if( !(CrtMoveFlr & CAR_READY))	return(0);



	if( !bCarUpMove && !bCarDnMove){
		Virtual_Floor_Set((CrtMoveFlr & ONLY_FLR));
	}
*/
}


////////hib_crt







void  Timer1Init(void)
{

	T1CON = 0 ;//Turn off Timer1 by clearing control register
    IPC0bits.T1IP=1;
	TMR1 = 0 ;  //Start Timer1 at zero
    PR1=16000;
    _T1ON = 1;
    _T1IE = 1;
}



void  LvdInit(void)
{
    LVDIE=0;


    RCON=RCON & 0xc0ff;
    RCON=RCON | 0x1c00;    //4.1v


    LVDIF=0;
    LVDIE=1;
}


void  Uart1Init(void)
{
    U1BRG=51;
    U1STA=0x0;

    _U1RXIP=4;
    _U1TXIP=4;

    _U1PDSEL=0;

    _U1STSEL  = 0;

    _U1ABAUD  = 0;
    _U1LPBACK = 0;
    _U1WAKE   = 0;
    _U1ALTIO  = 0;
    _U1USIDL  = 0;
    _U1UARTEN = 1;

    _U1TXEN=1;
    _U1TXISEL=1;

    _U1TXIF=0;
    _U1RXIF=0;

    _U1RXIE=1;
    _U1TXIE=1;
}


void  Uart2Init(void)
{
    U2BRG=51;
    U2STA=0x0;

    _U2RXIP=4;
    _U2TXIP=4;

    _U2PDSEL=0;

    _U2STSEL  = 0;

    _U2ABAUD  = 0;
    _U2LPBACK = 0;
    _U2WAKE   = 0;
    _U2USIDL  = 0;
    _U2UARTEN = 1;


    _U2TXEN=1;
    _U2TXISEL=1;

    _U2TXIF=0;
    _U2RXIF=0;

    _U2RXIE=1;
    _U2TXIE=1;
}



/*
void  __attribute__((section(".usercode")))  FindCurFloor(void)
{
	unsigned long backup_pulse,save_pulse;

    LocalType i;


    i=sRamDArry[mcurfloor];


	backup_pulse=CurPulse;


    if(sRamDArry[mcurfloor] == 0){
		save_pulse=GET_LONG(FLR_02_PULSE);
        if(backup_pulse >= save_pulse){
            sRamDArry[mcurfloor] = 0;
			CurPulse=0;
        }
    }
    else if(sRamDArry[mcurfloor] > cF_TOPFLR){
        sRamDArry[mcurfloor] = 0;
		CurPulse=0;
    }

    else if(sRamDArry[mcurfloor] == cF_TOPFLR){
		save_pulse=GET_LONG(FLR_01_PULSE + cF_TOPFLR - 1 );
        if(backup_pulse <= save_pulse){
            sRamDArry[mcurfloor] = 0;
			CurPulse=0;
        }
    }
	else{
		save_pulse=GET_LONG(FLR_01_PULSE + (i+1));
        if(backup_pulse >= save_pulse){
	        sRamDArry[mcurfloor]=0;
			CurPulse=0;
		}

		save_pulse=GET_LONG(FLR_01_PULSE + (i-1));
        if(backup_pulse < save_pulse){
	        sRamDArry[mcurfloor]=0;
			CurPulse=0;
		}

	}

    DspFloorSet();
}
*/



int   __attribute__((section(".usercode"))) main(void)
{
    unsigned int i;


///////////////////////////////
    PORTA=0xffff;
    PORTB=0xffff;
    PORTC=0xffff;
    PORTD=0xffff;
    PORTE=0xffff;
    PORTF=0xffff;
    PORTG=0xffff;


    TRISA=0xffff;
    TRISB=0xffff;
    TRISC=0xffff;
    TRISD=0xff00;

    TRISE=0xffff;
    TRISF=0xffff;  //    01010101;
    TRISG=0xffff;  //          01


    TRISE=0xffdf;


	TRISF=(TRISF & 0xff55);
	TRISG=(TRISG & 0xfff9);
    TRISC=(TRISC & 0x9fff);


    SPLIM=SPLIM;

    ADPCFG=0xffff;  //portb digital i/o

   	TXEN=1;
    C2IE       = 0x0;


  	I_X0_bit=0xff;
   	O_Y_0_bit=0x0;
	O_OP_bit=0x0;

//    sRamDArry[O_Y_0] = O_Y_0_bit;
//    sRamDArry[O_OP]  = O_OP_bit;



	OutAllClear();


	for(LogAddressLoop=0;LogAddressLoop < MAX_LONG_BUF;LogAddressLoop++){
        parameter_mirror[LogAddressLoop]=0x0;
	}

    AddressRead();



/*
	for(LogAddressLoop=0;LogAddressLoop <= sec;LogAddressLoop++){
		sRamDArry[sRamDArry[mDoorSeq]]=0x0;
	}
*/


/*
    sRamDArry[S1_STATE]=S1_STATE_bit=0;
    sRamDArry[S2_STATE]=S2_STATE_bit=0;
    sRamDArry[S3_STATE]=S3_STATE_bit=0;
    sRamDArry[S4_STATE]=S4_STATE_bit=0;

*/

	Init485_Bd();


    INTCON1bits.NSTDIS=1;


    asm("CLRWDT");
    ReadInitSetupData();
    asm("CLRWDT");

    LvdInit();
    Timer1Init();
    Uart1Init();
    Uart2Init();
    Can1Init();
    Can2Init();
	ADInit();

    INTCON1bits.NSTDIS=1;


/*
	WarmingUpTime=0;
    while(WarmingUpTime < 5){
        asm("CLRWDT");
    }
*/


    if(!SWR){
        asm("RESET");
    }
    SWR=0;

    Ladder485Init();

	WarmingUpTime=0;
    while(WarmingUpTime < 5){
        InPutRead();
        asm("CLRWDT");
    }




    C1Loop=0;
    C2Loop=0;

/*
   	bSlavePrk=0;
    bPC_PRK=0;
    bPC_FIRE=0;
	bPC_FAMILY=0;
*/

    disp_clk_regs();
    UseBaseTime = sRamDArry[mHour];

/*
    sRamDArry[FHM_RUN]=0;
    sRamDArry[FHM_SEQ]=0;



	sRamDArry[ExtCmdFlrONOFF0]=cF_NONSERVICE0;
	sRamDArry[ExtCmdFlrONOFF1]=cF_NONSERVICE1;
	sRamDArry[ExtCmdFlrONOFF2]=cF_NONSERVICE2;
	sRamDArry[ExtCmdFlrONOFF3]=cF_NONSERVICE3;


	if(FamilyService)	bPC_FAMILY=1;
	else				bPC_FAMILY=0;

*/

  	I_X0_bit=0xff;

   	O_Y_0_bit=0x0;
	O_OP_bit=0x0;
//    sRamDArry[O_Y_0] = O_Y_0_bit;
//    sRamDArry[O_OP]  = O_OP_bit;

	OutAllClear();





////////////////////
/*

    PORTA=0xffff;
    PORTB=0xffff;
    PORTC=0xffff;
    PORTD=0xffff;
    PORTE=0xffff;
    PORTF=0xffff;
    PORTG=0xffff;


    TRISA=0xffff;
    TRISB=0xffff;
    TRISC=0x0000;
    TRISD=0xffff;
    TRISE=0x0000;

    TRISF=0x0055;  //    01010101;

    TRISG=0x0001;  //          01

    SPLIM=SPLIM;

    ADPCFG=0xffff;  //portb digital i/o

   	TXEN=1;
    C2IE       = 0x0;


    bSaveFlash=0;

    OUT_ACT1=1;
    OUT_ACT2=1;

    IN_0=1;
    IN_1=1;
    IN_2=1;
    IN_3=1;
    IN_4=1;

    OUT_0=0;
    OUT_1=0;
    OUT_2=0;


    RD_WR=0;

   	OUT_ON=0;
   	OUT_ON=0;


    I_SU1_bit=0xff;
    I_EMG_bit=0xff;
  	I_GR_bit=0xff;
  	I_FIRE_bit=0xff;
 	I_FS0_bit=0xff;
  	I_X0_bit=0xff;

   	O_OP_bit=0x0;
  	O_U_W_bit=0x80;
   	O_Y_0_bit=0x0;


    sRamDArry[I_EMG]  = I_EMG_bit;
    sRamDArry[I_SU1]  = I_SU1_bit;
    sRamDArry[I_GR]   = I_GR_bit;
    sRamDArry[I_FIRE] = I_FIRE_bit;
    sRamDArry[I_X_0]  = I_X0_bit;
    sRamDArry[I_FS0]  = I_FS0_bit;

    sRamDArry[O_OP]  = O_OP_bit;
    sRamDArry[O_U_W] = O_U_W_bit;
    sRamDArry[O_Y_0] = O_Y_0_bit;


	CarAllStopCmd();


	for(LogAddressLoop=0;LogAddressLoop < MAX_LONG_BUF;LogAddressLoop++){
        parameter_mirror[LogAddressLoop]=0x0;
	}

	bRelevelErr=0;
	bBefbRelevelErr=0;

	bCarErr=0;
   	bCarStopNoRun=0;
    bManualAuto=0;
	bsLope=0;
   	bsSlip=0;
    bSlipCheckStart=0;
	bsNextFloor=0;
	bsLuLdNoOff=0;
	bExportData=0;
	bImportData=0;
	bPowerChkStart=0;

    bBefbsBreakMgtOpen=0;
    bBefbsBreakOpen=0;
	bsBreakMgtOpen=0;
	bsBreakOpen=0;
	bRunningOpenOn=0;

    AddressRead();

	for(LogAddressLoop=0;LogAddressLoop <= sec;LogAddressLoop++){
		sRamDArry[sRamDArry[mDoorSeq]]=0x0;
	}


	sRamDArry[mDoorSeq]=DOOR_CLOSE_START;
	DoorCloseTime=0;


    sRamDArry[S1_STATE]=S1_STATE_bit=0;
    sRamDArry[S2_STATE]=S2_STATE_bit=0;
    sRamDArry[S3_STATE]=S3_STATE_bit=0;
    sRamDArry[S4_STATE]=S4_STATE_bit=0;
    sRamDArry[SelDoorStatus]=DoorStatus_bit=0;

	Init485_Bd();


	bManualAuto=0;
	bHomeUpDn=0;


    INTCON1bits.NSTDIS=1;



    LvdInit();
    Timer1Init();
    Uart1Init();
    Uart2Init();
    Can1Init();
    Can2Init();


    INTCON1bits.NSTDIS=1;



    asm("CLRWDT");
    ReadInitSetupData();
    asm("CLRWDT");


    if(!SWR){
        asm("RESET");
    }
    SWR=0;

    Ladder485Init();

#ifndef		RS485_FOR_CRT
	Init485_Ladder();
   	RxStatus=STX_CHK;
   	MyDataTx();
#endif


   	sRamDArry[LD_INSP] = 0x0;


   	NoCloseCnt=0;
   	WarmingUpTime=0;
   	CurFTime=0;
   	NextFTime=0;
	SaveVerify=0;


	sRamDArry[mMostLongDst]= 0x00;
	sRamDArry[mStartFloor] = 0;   //delete value




    while(WarmingUpTime < 50){
        InPutRead();
        asm("CLRWDT");
    }



    if(INVERTER_CHECK == IO){
        FindCurFloor();
    }

    C1Loop=0;
    C2Loop=0;

   	bSlavePrk=0;
    bPC_PRK=0;
    bPC_FIRE=0;
	bPC_FAMILY=0;

    disp_clk_regs();
    UseBaseTime = sRamDArry[Hour];
    sRamDArry[FHM_RUN]=0;
    sRamDArry[FHM_SEQ]=0;


    bCarService=0;





#ifdef  TEST_SIMULATION
	NotRecoveryData=0;
	bsSlip=0;
#endif




	LevelEncoderPulse2=0;

	sRamDArry[ExtCmdFlrONOFF0]=cF_NONSERVICE0;
	sRamDArry[ExtCmdFlrONOFF1]=cF_NONSERVICE1;
	sRamDArry[ExtCmdFlrONOFF2]=cF_NONSERVICE2;
	sRamDArry[ExtCmdFlrONOFF3]=cF_NONSERVICE3;

	if(FamilyService)	bPC_FAMILY=1;
	else				bPC_FAMILY=0;

	CmdFixFlrTime=cF_FIXFLOORTIME;
////////////////////////////////////
	bBK1=0;
	BreakTime=0;


	ThisFloorDoorCheck();


*/

	MasterHostLiveTime=100;

    do{
        AddressRead();
		Ad_Check();
        new_IO_Check();
        SerialCheck();
        Serial2Check();

        asm("CLRWDT");

        Can1Check();
        Can2Check();

        disp_clk_regs();

        asm("CLRWDT");
   	}while(1);
}




void _ISR_X _T1Interrupt(void)
{

    if( _T1IF ){

        _T1IF = 0 ;
        TMR1=0;


        if(EepTime<50)    EepTime++;


        EmgTime++;
        PrkTime++;
        AtTime++;
        UbTime++;
        DbTime++;
        OlsTime++;
        GsTime++;
        DsTime++;

        Su1Time++;
        Sd1Time++;
        SftTime++;
        OvlTime++;
        UlsTime++;
        DlsTime++;
        LuTime++;
        LdTime++;

        RgTime++;
        BatTime++;
        PassTime++;
        FireTime++;
        WaterTime++;
        FullTime++;
        MmTime++;
        FhmInTime++;

        Fr1Time++;
        Fr2Time++;
        Gs2Time++;
        Ds2Time++;
        Su2Time++;
        Sd2Time++;
        BmTime++;
        InvTime++;

		Fs0Time++;
		Fs1Time++;
		Fs2Time++;
		Fs3Time++;
		Fs4Time++;
		DerTime++;
		FidTime++;
		UndTime++;


        X0Time++;
        X1Time++;
        X2Time++;
        X3Time++;
        X4Time++;
        X5Time++;
        X6Time++;
        X7Time++;


        I_EMG_Cht++;
        I_FS0_Cht++;
        I_GR_Cht++;
        I_SU1_Cht++;
        I_X_0_Cht++;
        I_FIRE_Cht++;


        C1Time++;
        C2Time++;
        SerialTime2++;



#ifndef		RS485_FOR_CRT
        SerialTime++;
        if(SerialTime>10){
            TimeOutBd++;
            SerialTime=0;
        }
#else
        if(SerialTime < 100)	SerialTime++;
#endif


        if(bOpenDoorOk && (DoorOpenOnTime<250)){
            DoorOpenOnTime++;
        }




		if(cF_On_Time > 0)		FrequenceMainTime=(unsigned int)((unsigned int)60000/(unsigned int)cF_On_Time);
		else					FrequenceMainTime=60000;

//		FrequenceMainTime=(unsigned int)(cF_On_Time * 1000);
		tmplong=(unsigned long)((unsigned long)cF_On_Percent * (unsigned long)FrequenceMainTime);
		FrequenceTime=(unsigned int)(tmplong/100);

		LuLdTime++;
		if(LuLdTime >= FrequenceMainTime){
			LuLdTime=0;
		}

		if(LuLdTime >= FrequenceTime){
			bBlinck=1;  // off
		}
		else{
			bBlinck=0; // on
		}

		if(LuLdTime < 250)	sRamDArry[mMasterSync] = LuLdTime;
		else				sRamDArry[mMasterSync] = 0xff;





        if(LadderTime  < 20002)  LadderTime++;
		if(EncoderReloadTime < 1000)	EncoderReloadTime++;

        msec10++;
        if(msec10 > 100){
            msec10=0;

			OUT_RUN = !OUT_RUN;

			if(MasterHostLiveTime < 100)	MasterHostLiveTime++;

			if(TmpBuzor < 100)	TmpBuzor++;

            if(RstTime < 65530) RstTime++;

            C2Loop++;


            if(CurPulse>BefCurPulse){
                PulsePer100Msec=CurPulse-BefCurPulse;
                CountDn=0;
                CountUp++;
            }
            else if(CurPulse<BefCurPulse){
                PulsePer100Msec=BefCurPulse-CurPulse;
                CountUp=0;
                CountDn++;
            }
            else{
                PulsePer100Msec=BefCurPulse-CurPulse;
                CountUp=0;
                CountDn=0;
            }
            BefCurPulse=CurPulse;

			if(EncoderReloadTime > 100){
	            TmpMmPerMin=GET_LONG(MPM_VARIABLE);
	            CurMeterPerMin=(PulsePer100Msec * TmpMmPerMin);
	            CurMeterPerMin=(CurMeterPerMin/(unsigned long)1000000);

			}
			else	EncoderReloadTime = 101;


            if(CarOnceStopTime < 250)   CarOnceStopTime++;
            if(PowerFailTime<250)       PowerFailTime++;
            if(LoopTime<100)            LoopTime++;


            if(!bMoveCar){
                CounterTime=0;
                ElevMoveTime=0;
                if(ElevStopTime < 250)    ElevStopTime++;
            }
            else{
                ElevStopTime=0;
                if(CounterTime < 250)   	CounterTime++;
                if(ElevMoveTime < 50000)  	ElevMoveTime++;
            }


/*
            if(bLevelFind){
                if(RelevelTime < 250)  RelevelTime++;

                if(RelevelTime > 240){
        	        CarAllStopCmd();
                }
            }
            else    RelevelTime=0;
*/

            FhmLoaderTime++;
            CurFTime++;
            NextFTime++;
            errtime++;
            DoorCloseTime++;
            DoorOpenTime++;
            NextFloorTime++;
            UpBttonTime++;
            DnBttonTime++;
            SerialLoopMaxTime++;

			if(BreakTime  < 1000)		BreakTime++;

			if(ReLevelAnounceTime  < 100)		ReLevelAnounceTime++;

			if(SolTimer < 100)			SolTimer++;
			if(SolWaitTimer < 100)		SolWaitTimer++;
            if(FhmTime<100)             FhmTime++;
            if(SelMainCarTime < 100)    SelMainCarTime++;
            if(SelSubCarTime < 100)     SelSubCarTime++;
            if(LadderKeyTime < 36500)   LadderKeyTime++;
            if(WarmingUpTime < 210)     WarmingUpTime++;
            if(CloseOutOffTime < 250)   CloseOutOffTime++;
            if(ExIOTimer < 250)   		ExIOTimer++;

            if(bUnd){
                if(VoiceTime < 250) VoiceTime++;
                if(BellTime < 250) BellTime++;
            }
            else{
		        VoiceTime=0;
				BellTime=0;
			}

            if( bDoorCloseOkSensor && (DoorCloseOnTime<250)){
                DoorCloseOnTime++;
            }


            sec1++;
            if(sec1 >= 10){
                sec1=0;
                bDspSeq=!bDspSeq;
                FanTime++;
				if(AccTimer < 254)			AccTimer++;
                if(FixFlrTimeCnt <= 254)	FixFlrTimeCnt++;
            }

            MinuteTime++;
            if(MinuteTime>=600){
                MinuteTime=0;
                LightTime++;

				UseBaseTime++;
				if(UseBaseTime >= 60){
					UseBaseTime=0;
        			if(PowerOnTime < 65530) PowerOnTime++;
					else					PowerOnTime=0;
				}

                if(InvErrTime < 600)    InvErrTime++;
            }

            if(bManualStop){
                if(ManualStopTime < 100)  ManualStopTime++;
            }
            else	ManualStopTime=0;

/*
            if(bFR2Start1 && (sRamDArry[mFireSeq]==FIRE_THREE)){
                if(RestartTime<60)  RestartTime++;
            }
            else{
                RestartTime=0;
            }
*/

#ifdef  TEST_SIMULATION
            if(bMoveCar){
                if(MotorMoveTime<250)	MotorMoveTime++;
            }
            else{
        		MotorMoveTime=0;
            }
#endif

        }
    }
}




void _ISR_X _OscillatorFail(void)
{

    INTCON1bits.OSCFAIL=0;
    asm("RESET");
}


void _ISR_X _AddressError(void)
{
    INTCON1bits.ADDRERR=0;
    asm("RESET");
}


void _ISR_X _StackError(void)
{
    INTCON1bits.STKERR=0;
    asm("RESET");
}




void _ISR_X _MathError(void)
{
    INTCON1bits.MATHERR=0;
    asm("RESET");
}



void	SaveEEpromPulse(void)
{
/*
    unsigned int i;


// apl 수정	CarAllStopCmd();
// apl 수정	CarAllStopCmd();


    if(bSaveFlash){
		SaveVerify = 0x55;
		EEP_Save_All();
        bSaveFlash=0;
		SaveVerify = 0x0;
    }
*/
}


void _ISR_X _LVDInterrupt(void)
{

	SaveEEpromPulse();
    LVDIF=0;
    asm("RESET");
}




void _ISR_X _DefaultInterrupt(void)
{
	SaveEEpromPulse();
    asm("RESET");
}





