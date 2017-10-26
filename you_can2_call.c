

#include <p30fxxxx.h>
#include "you_can_lib.h" 
#include "you_can2.h" 

           
#include  "30f6010_io.h"            
#include  "host_io.h"            
#include  "iodef.h"
#include  "door.h"
//#include  "eeprom.h"
#include  "com.h"
#include  "counter.h"


UserDataType	Can2RcvBuf[100];


UserDataType   C2DataSeqStart=0;

UserDataType   C2Time=0;
UserDataType   C2DataSeq=0;
unsigned int   C2Company=0;
//unsigned int   C2ReceiveGroupNumber=0;
//unsigned int   C2ReceiveMaterAdr=0;
unsigned int   C2ReceiveSlaveAdr=0;
unsigned int   C2ReceiveAdrStatus=0;

unsigned int   C2TmpSid=0;
unsigned int   C2TmdEidL=0;
unsigned int   C2TmdEidH=0;

unsigned int   C2Loop=0;

/*
#define  H2_INTV_TIME1  100   //msec
#define  H2_INTV_TIME2  105   //msec
#define  H2_INTV_TIME3  110   //msec
#define  H2_INTV_TIME4  115   //msec
#define  H2_INTV_TIME5  120   //msec
*/

#define  H2_INTV_TIME1  100   //msec
#define  H2_INTV_TIME2  100   //msec
#define  H2_INTV_TIME3  100   //msec
#define  H2_INTV_TIME4  100   //msec
#define  H2_INTV_TIME5  100   //msec


#define  I_AM_MASTER    0x2000





unsigned    int     CAN2bit;

unsigned    char    Can2RxLiveCnt;
unsigned    char    Can2Tx1ConfirmCnt;
unsigned    char    Can2Tx0ConfirmCnt;


unsigned    int     Can2Block_Nm=0;
unsigned    int     Can2DataSeq=0;
unsigned 	int		Can2TxDataCnt=0;
unsigned 	int   	Can2DataCnt=0;
unsigned    char    Can2DataFrameNm=0;
unsigned    char	Can2RxDataTotalCnt=0;
unsigned    char    Can2RxDataTxPointer=0;
unsigned    char	Can2TxDataTotalCnt=0;
unsigned    char    Can2TxDataTxPointer=0;
unsigned	char	Can2PollingTimer=0;
unsigned	char	Can2RxTimer=0;
unsigned    int    	Can2ContCrc;

unsigned    char    Can2MySrcAddr=0;
unsigned    char    Can2MyDestAddr=0;
unsigned    char    Can2MyProductId=0;
unsigned    char    Can2MyGroupAddr=0;
unsigned    char    Can2MyLocalAddr=0;

unsigned    char    Can2RxSrcAddr=0;
unsigned    char    Can2RxDestAddr=0;
unsigned    char    Can2RxProductId=0;
unsigned    char    Can2RxGroupAddr=0;
unsigned    char    Can2RxLocalAddr=0;


unsigned	char	Can2RxCnt=0;
unsigned	char	Can2RxThisPt=0;
unsigned	char	Can2TxCnt=0;
unsigned	char	Can2TxThisPt=0;

unsigned    long    Can2RxEidFilter;
unsigned    int     Can2RxSidFilter;
unsigned    long    Can2RxEidMask;
unsigned    int     Can2RxSidMask;
unsigned    long    Can2TxEid;
unsigned    int     Can2TxSid;
unsigned    char    Can2TxDlc;
unsigned    long    Can2RxEid;
unsigned    int     Can2RxSid;
unsigned    char    Can2RxDlc;

unsigned    long    BefCan2RxEid;
unsigned    int     BefCan2RxSid;


unsigned 	int   	Can2ThisRxBuf[8];
unsigned 	int   	Can2ThisTxBuf[8];
unsigned    int     Can2RxGood=0;

UserDataType	TmpCan2RcvBuf[100];


unsigned    int     *TxBufPt=0;


void   __attribute__((section(".usercode"))) CAN2SendMessageyou(char MsgFlag,unsigned int * data, unsigned int  datalen) 
{
    int i;

    for(i = 0;i < datalen;i++)
    {
        switch(MsgFlag)
        {
        case 0: 
            *((unsigned char *)&C2TX0B1+i)= data[i];
            break;
        case 1: *((unsigned char *)&C2TX1B1+i)= data[i];
            break;
        case 2: *((unsigned char *)&C2TX2B1+i)= data[i];
            break;
        default:*((unsigned char *)&C2TX0B1+i)= data[i];
            break;
        }
    }

    
    
    switch(MsgFlag)
    {
    case 0:
        C2TX0DLCbits.DLC = datalen;
        C2TX0CON=7;         
        C2TX0CONbits.TXREQ = 1;
        break;
    case 1:
        C2TX1DLCbits.DLC = datalen;
        C2TX1CON=7; 
        C2TX1CONbits.TXREQ = 1;
        break;
    case 2:
        C2TX2DLCbits.DLC = datalen;
        C2TX2CON=7; 
        C2TX2CONbits.TXREQ = 1;
        break;
    default:
        C2TX0DLCbits.DLC = datalen;
        C2TX0CON=7; 
        C2TX0CONbits.TXREQ = 1;
        break;
    }
}



void  __attribute__((section(".usercode"))) SetSidEid(void)
{

    C2TmpSid=(unsigned int)cF_COMPANY;
    C2TmpSid=(C2TmpSid | (MyGroupAddr << 7));

    C2TmdEidH=0;
	C2TmdEidL=0;

#ifndef	MAX_EIGHT_CAR
	if(MyGroupAddr & 0x10)	C2TmdEidH=(C2TmdEidH | 0x0001);	
	if(MyGroupAddr & 0x20)	C2TmdEidH=(C2TmdEidH | 0x0002);


    if(MyLocalAddr & 0x01)   C2TmdEidL=(C2TmdEidL | 0x4000);
    if(MyLocalAddr & 0x02)   C2TmdEidL=(C2TmdEidL | 0x8000);
#else
	if(MyGroupAddr & 0x10)	C2TmdEidH=(C2TmdEidH | 0x0002);
	
    if(MyLocalAddr & 0x01)   C2TmdEidL=(C2TmdEidL | 0x4000);
    if(MyLocalAddr & 0x02)   C2TmdEidL=(C2TmdEidL | 0x8000);
    if(MyLocalAddr & 0x04)   C2TmdEidH=(C2TmdEidH | 0x0001);
#endif

	C2TmdEidL = (C2TmdEidL | I_AM_MASTER); //master set
	
}



void    __attribute__((section(".usercode"))) Can2Init(void)
{
    unsigned int sid;
    unsigned int eidh,eidl;


    C2CTRLbits.REQOP    = 0x4;      // Entry Configration Mode
//    C2CTRLbits.CANCAP   = 0x1;      // enable CAN Capture mode
    C2CTRLbits.CANCAP   = 0x0;      // disable CAN Capture mode
    C2CTRLbits.CSIDL    = 0x1;      // when idle mode,stop can
    C2CTRLbits.ABAT     = 0x1;      // aboart all pending transmit
    C2CTRLbits.CANCKS   = 0x1;      // fcan=fcy

    C2CFG2bits.SAM      = 0x1 ;     // Bus Line is sampled three times prior to the sample point
    C2CFG2bits.WAKFIL   = 0x0 ; 	// CAN bus line filter is not used for wake-up
    C2CFG2bits.SEG2PHTS = 0x1 ;     // Freely programmable;

//    C1CFG1bits.BRP      = 0x9;      // tq=0.00000125
    C2CFG1bits.BRP      = 19;      // tq=0.0000025
  
    C2CFG1bits.SJW      = 0x0;      // Synchronized Jump Width bits 1xTq
    C2CFG2bits.SEG1PH   = 0x1 ;     // 2 Tq
    C2CFG2bits.PRSEG    = 0x2 ;     // 3 Tq
    C2CFG2bits.SEG2PH   = 0x1 ; 	// 2 Tq

    C2TX0CON            = 0x3;
    C2TX1CON            = 0x3;

    C2TX0SIDbits.TXIDE  = 0x1;
    C2TX0SIDbits.SRR    = 0x0;
    C2TX0DLCbits.TXRB0  = 0x0;
    C2TX0DLCbits.TXRB1  = 0x0;
    C2TX0DLCbits.TXRTR  = 0x0;

    C2TX1SIDbits.TXIDE  = 0x1;
    C2TX1SIDbits.SRR    = 0x0;
    C2TX1DLCbits.TXRB0  = 0x0;
    C2TX1DLCbits.TXRB1  = 0x0;
    C2TX1DLCbits.TXRTR  = 0x0;

    C2TX2SIDbits.TXIDE  = 0x1;
    C2TX2SIDbits.SRR    = 0x1;
    C2TX2DLCbits.TXRB0  = 0x0;
    C2TX2DLCbits.TXRB1  = 0x0;
    C2TX2DLCbits.TXRTR  = 0x0;


    C2RXM0SID           = 0x0;
    C2RXM0EIDH          = 0x0;
    C2RXM0EIDL          = 0x0;
    C2RXM0SIDbits.MIDE  = 0x0;

    C2RXM1SID           = 0x0;
    C2RXM1EIDH          = 0x0;
    C2RXM1EIDL          = 0x0;
    C2RXM1SIDbits.MIDE  = 0x0;

    C2RXF0SID           = 0x0;
    C2RXF0EIDH          = 0x0;
    C2RXF0EIDL          = 0x0;
    C2RXF0SIDbits.EXIDE = 0x0;

    C2RXF1SID           = 0x0;
    C2RXF1EIDH          = 0x0;
    C2RXF1EIDL          = 0x0;
    C2RXF1SIDbits.EXIDE = 0x0;

    C2RXF2SID           = 0x0;
    C2RXF2EIDH          = 0x0;
    C2RXF2EIDL          = 0x0;
    C2RXF2SIDbits.EXIDE = 0x0;

    C2RXF3SID           = 0x0;
    C2RXF3EIDH          = 0x0;
    C2RXF3EIDL          = 0x0;
    C2RXF3SIDbits.EXIDE = 0x0;

    C2RXF4SID           = 0x0;
    C2RXF4EIDH          = 0x0;
    C2RXF4EIDL          = 0x0;
    C2RXF4SIDbits.EXIDE = 0x0;

    C2RXF5SID           = 0x0;
    C2RXF5EIDH          = 0x0;
    C2RXF5EIDL          = 0x0;
    C2RXF5SIDbits.EXIDE = 0x0;

    C2RX0CONbits.RXFUL  = 0x0;
//    C1RX0CONbits.DBEN   = 0x1;
    C2RX0CONbits.DBEN   = 0x0;
    C2RX0CONbits.JTOFF  = 0x0;
    C2RX0CONbits.FILHIT0= 0x0;

    C2RX1CONbits.RXFUL  = 0x0;


    C2INTE              = 0x0;
    C2INTEbits.ERRIE    = 0x1;
    C2INTEbits.RX0IE    = 0x1;
    C2INTEbits.RX1IE    = 0x1;

    C2IE       = 0x1;



	SetSidEid();

/*
    sid=(unsigned int)cF_COMPANY;
    sid=(sid | (MyGroupAddr << 7));

    eidh=0;
    eidl=0;


    if(MyLocalAddr & 0x02)   eidl=(eidl | 0x8000);
    if(MyLocalAddr & 0x01)   eidl=(eidl | 0x4000);

    if(MyGroupAddr & 0x20)   eidh=(eidh | 0x0002);
    if(MyGroupAddr & 0x10)   eidh=(eidh | 0x0001);
*/


    youCAN2SetFilter(0,C2TmpSid,C2TmdEidH,C2TmdEidL);
    youCAN2SetFilter(1,C2TmpSid,C2TmdEidH,0x2000);
    youCAN2SetFilter(2,0x0,0x0,0x0);
    youCAN2SetFilter(3,0x0,0x0,0x0);
    youCAN2SetFilter(4,0x0,0x0,0x0);
    youCAN2SetFilter(5,0x0,0x0,0x0);
 
    youCAN2SetMask(5,0xffff,0xffff,0xffff);
    youCAN2SetMask(4,0xffff,0xffff,0xffff);
    youCAN2SetMask(3,0xffff,0xffff,0xffff);
    youCAN2SetMask(2,0xffff,0xffff,0xffff);
    youCAN2SetMask(1,0x07ff,0x0003,0xc000);
    youCAN2SetMask(0,0x07ff,0x0003,0xc000);

 
    IPC9bits.C2IP=4;


/*
    C2Time=0;
    while(C2Time<10);

    C2CTRLbits.REQOP    = 0x0;

    C2Time=0;
    while(C2Time<10);
*/
    
    C2CTRLbits.REQOP    = 0x0;

                
    C2DataSeq=0;                
    C2Time=0;
    C2EC=0;
}







void  __attribute__((section(".usercode"))) NormalDataReturn(void)
{
    LocalType sel,xx;

    sel=0;

    if(!C2TX0CONbits.TXREQ && (C2DataSeqStart==1)){

        if(C2Time > H2_INTV_TIME5){
			xx=0;
			if(C2DataSeq == 4){
				C2DataSeq=8;
				xx=4;
			}
            youCAN2TxSidSet(sel,C2TmpSid);
            youCAN2TxEidSet(sel,C2TmdEidH,C2TmdEidL+C2DataSeq);
            LoadCanBuffer(C2DataSeq-xx);

			if(C2DataSeq >= 8){
				CAN_Buf[7] = CAN2_Buf[2];
			}

            CAN2SendMessageyou(sel,&CAN_Buf[0], 8);        
            C2DataSeq++;       			

			if(C2DataSeq > 8){
	            C2Time=0;
	            C2DataSeq=0;       
	            C2DataSeqStart=0;
			}
		}

	}
 
}






/*
void  __attribute__((section(".usercode"))) DataReturn(unsigned int par,unsigned int nmb)
{
    LocalType sel;
    LocalType cmdgood;

    cmdgood=0;
    sel=0;


    if(par==0x01){  //prk
        CAN_Buf[0]=0x0;
//        if(IN_PRK || bPC_PRK)   CAN_Buf[0]=0x01;
        if(IN_PRK || bPC_PRK || bSlavePrk)   CAN_Buf[0]=0x01;
        CAN_Buf[1]=0x00;
        cmdgood=1;
    }
    else if(par==0x02){ //odd/even
        CAN_Buf[0]=cF_NONSERVICE0;
        CAN_Buf[1]=cF_NONSERVICE1;
        CAN_Buf[2]=cF_NONSERVICE2;
        CAN_Buf[3]=cF_NONSERVICE3;
        cmdgood=1;
    }
    else if(par==0x03){  //rush hour
        CAN_Buf[0]=0x0;
        CAN_Buf[0]=cF_FIXFLOORTIME;
        CAN_Buf[1]=0x00;
        cmdgood=1;
    }
    else if(par==0x04){  //fire
        CAN_Buf[0]=0x0;
        if(!IN_FIRE || bPC_FIRE)   CAN_Buf[0]=0x01;
        CAN_Buf[1]=0x00;
        cmdgood=1;
    }



    if(!C2TX0CONbits.TXREQ && (C2DataSeqStart==1) && (cmdgood)){
        youCAN2TxSidSet(sel,C2TmpSid);
        youCAN2TxEidSet(sel,C2TmdEidH,C2TmdEidL+0);         
        CAN2SendMessageyou(sel,&CAN_Buf[0], nmb);            
        C2Time=0;
        C2DataSeq=0;       
        C2DataSeqStart=0;               
    }
}
*/


LocalType __attribute__((section(".usercode"))) HostReadMyData(void)
{
    if((CAN2_Buf[2] == 0x00) && (CAN2_Buf[3] == 0x00)){
        NormalDataReturn();
    }
/*
    else if((CAN2_Buf[2] == 0x01) && (CAN2_Buf[3] == 0)){
        DataReturn(CAN2_Buf[2],2);
    }
    else if((CAN2_Buf[2] == 0x02) && (CAN2_Buf[3] == 0)){
        DataReturn(CAN2_Buf[2],4);
    }
    else if((CAN2_Buf[2] == 0x03) && (CAN2_Buf[3] == 0)){
        DataReturn(CAN2_Buf[2],2);
    }
    else if((CAN2_Buf[2] == 0x04) && (CAN2_Buf[3] == 0)){
        DataReturn(CAN2_Buf[2],2);
    }
*/
}





LocalType __attribute__((section(".usercode"))) HostCommandAct(void)
{

/*
    if((CAN2_Buf[2]    ==   (PC_COMMAND | CMD_PARKING)) && (CAN2_Buf[3] == 0x00)){
        if(CAN2_Buf[4] == 0x01) bPC_PRK = 1;
        else                    bPC_PRK = 0;
		NormalDataReturn();    
    }    
    else if((CAN2_Buf[2]    ==   (PC_COMMAND | CMD_FIRE)) && (CAN2_Buf[3] == 0x00)){
        if(CAN2_Buf[4] == 0x01) bPC_FIRE = 1;
        else                    bPC_FIRE = 0;
		NormalDataReturn();    
    }
    else if((CAN2_Buf[2]    ==   (PC_COMMAND | CMD_FAMILY_SEV)) && (CAN2_Buf[3] == 0x00)){
        if(CAN2_Buf[4] == 0x01) bPC_FAMILY = 1;
        else                    bPC_FAMILY = 0;
		NormalDataReturn();    
    }

    else if((CAN2_Buf[2]    ==   (PC_COMMAND | CMD_CALL_SEV)) && (CAN2_Buf[3] == 0x00)){
		CrtMoveFlr=(unsigned char)(CAN2_Buf[4] | CAR_READY);
		NormalDataReturn();    
    }

    else if((CAN2_Buf[2]    ==   (PC_COMMAND | CMD_EXT_CALL_SEV)) && (CAN2_Buf[3] == 0x00)){
		sRamDArry[mCrtExtMoveFlr]=(unsigned char)(CAN2_Buf[4]);
		ExtKeyCnt=3;
		CAN2_Buf[0]=0;
		NormalDataReturn();    
    }
*/
}



LocalType __attribute__((section(".usercode"))) HostWriteMyData(void)
{

	    if((CAN2_Buf[2] == (CMD_EVEN_ODD)) && (CAN2_Buf[3] == 0x00)){
	        Pc_Command();
			NormalDataReturn();    
	    }    
	    else if((CAN2_Buf[2] == (CMD_MORNING)) && (CAN2_Buf[3] == 0x00)){
	        Pc_Command();
			NormalDataReturn();    
	    }    
}




/*
void  __attribute__((section(".usercode"))) Can2TxData(void)
{
    LocalType sel,xx;

    sel=0;

    if(!C2TX0CONbits.TXREQ && (C2DataSeqStart==1)){

        if(C2Time > H2_INTV_TIME5){
			xx=0;
			if(C2DataSeq == 4){
				C2DataSeq=8;
				xx=4;
			}
            youCAN2TxSidSet(sel,C2TmpSid);
            youCAN2TxEidSet(sel,C2TmdEidH,C2TmdEidL+C2DataSeq);
            LoadCanBuffer(C2DataSeq-xx);

			if(C2DataSeq >= 8){
				CAN_Buf[7] = CAN2_Buf[2];
			}

            CAN2SendMessageyou(sel,&CAN_Buf[0], 8);        
            C2DataSeq++;       			

			if(C2DataSeq > 8){
	            C2Time=0;
	            C2DataSeq=0;       
	            C2DataSeqStart=0;
			}
		}

	}
 
}
*/


LocalType __attribute__((section(".usercode"))) Can2Check(void)
{    
	unsigned int i,datalen,seqnm,lastdata;
	unsigned int tmpeidl;
    
	lastdata=0;
	datalen=0;
	seqnm=0;

	tmpeidl=(unsigned int)(Can2RxEid & 0x00ff);
	C2TmdEidL=(tmpeidl | C2TmdEidL);

	if(!C2TX0CONbits.TXREQ && Can2RxGood){
		for(i=0;i<8;i++){
			datalen++;
	    	CAN_Buf[i]= Can2RcvBuf[Can2TxThisPt];
			Can2TxThisPt++;
			if( Can2TxThisPt >= Can2TxCnt){
				lastdata=DATA_BLOCK_END;
				Can2RxGood=0;
				break;
			}
		}
	
		tmpeidl=C2TmdEidL;

		seqnm=((Can2TxThisPt -1)/8);

		seqnm=(seqnm << 8);
		tmpeidl=(tmpeidl | seqnm | lastdata);

        youCAN2TxSidSet(0,C2TmpSid);
        youCAN2TxEidSet(0,C2TmdEidH,tmpeidl);
        CAN2SendMessageyou(0,&CAN_Buf[0], datalen); 
		
	}
    return(0);
}





LocalType __attribute__((section(".usercode"))) Can2CmdSort(void)
{    

	unsigned int i,base_mem,data_nm,tmp_flash_adr,FlashSaveOk;
	unsigned char tmpbit,tmpnm;

	base_mem=(Can2RcvBuf[2]);
	data_nm = Can2RcvBuf[3];

	if(Can2RcvBuf[0] & BIT_DEG){
		data_nm =(Can2RcvBuf[4]);

		if((Can2RcvBuf[3] == 0) || (Can2RcvBuf[3] > 8))	return(0);

		tmpbit =0x01;
		tmpnm=Can2RcvBuf[3];
		tmpbit = (tmpbit << (tmpnm-1));

	}
  
    switch(Can2RcvBuf[0]){
        case    SRAM_BYTE_READ:
			for(i=0;i<data_nm;i++){
				Can2RcvBuf[4+i]=sRamDArry[((base_mem+M_BASE) + i)];
			}

			Can2RcvBuf[1]= Can2RcvBuf[3]+2;
			Can2TxThisPt=0;
			Can2TxCnt=Can2RcvBuf[1]+2;
			Can2RxGood=1;
            break;
		case	FLASH_BYTE_READ:	
			for(i=0;i<data_nm;i++){
				Can2RcvBuf[4+i]=cF_FLRDSPCH( (unsigned long)(F_BLOCK1 + base_mem + i));
			}

			Can2RcvBuf[1]= Can2RcvBuf[3]+2;
			Can2TxThisPt=0;
			Can2TxCnt=Can2RcvBuf[1]+2;
			Can2RxGood=1;
            break;
        case    SRAM_BYTE_RWITE:
			for(i=0;i<data_nm;i++){
				sRamDArry[((VIRTUAL_RAM+base_mem) + i)]=Can2RcvBuf[4+i];
			}

			if((base_mem+M_BASE) == mYear){
				for(i=0;i<data_nm;i++){
					sRamDArry[((M_BASE+base_mem) + i)]=Can2RcvBuf[4+i];
				}
                Initialize_DS1302();
			}

			Can2TxThisPt=0;
			Can2TxCnt=Can2RcvBuf[1]+2;
			Can2RxGood=1;
            break;
        case    FLASH_BYTE_WRITE:
			SaveVerify = 0x55;
			GetFlashDataToBufRam(F_BLOCK1);				
			for(i=0;i<data_nm;i++){
    			b_LdTmpBufRam(i)=Can2RcvBuf[4+i];
			}				
   			flash_write_DspChar(F_BLOCK1);
			SaveVerify = 0x0;

			for(i=0;i<data_nm;i++){
				Can2RcvBuf[4+i]=cF_FLRDSPCH( (unsigned long)(F_BLOCK1 + base_mem + i));
			}

			Can2TxThisPt=0;
			Can2TxCnt=Can2RcvBuf[1]+2;
			Can2RxGood=1;
            break;
        case    SRAM_BIT_WRITE:
			for(i=0;i<data_nm;i++){
				if(Can2RcvBuf[5+i]==1){
					sRamDArry[(base_mem+M_BASE)]=(sRamDArry[(base_mem+M_BASE)] | tmpbit);
				}
				else if(Can2RcvBuf[5+i]==0){
					sRamDArry[(base_mem+M_BASE)]=(sRamDArry[(base_mem+M_BASE)] & (~tmpbit));
				}
				else{
					if(sRamDArry[(base_mem+M_BASE)] & tmpbit)	Can2RcvBuf[5+i]=1;
					else										Can2RcvBuf[5+i]=0;	
				}
			
				if(tmpbit >= 0x80){
					tmpbit=0x01;
					base_mem++;
				}				
				else	tmpbit=(tmpbit << 1);
			}
			Can2TxThisPt=0;
			Can2TxCnt=Can2RcvBuf[1]+2;
			Can2RxGood=1;
            break;
        case    FLASH_BIT_WRITE:
			FlashSaveOk=0;	
			tmp_flash_adr=(base_mem % 64);
			GetFlashDataToBufRam(F_BLOCK1);		
			for(i=0;i<data_nm;i++){
				if(Can2RcvBuf[5+i]==1){
					if( !bitChk_FLRDSPCH((tmp_flash_adr),tmpnm)){
		        		bit_LdTmpBufRamSet((tmp_flash_adr),tmpnm);
						FlashSaveOk=1;	
					}
				}
				else if(Can2RcvBuf[5+i]==0){
					if(bitChk_FLRDSPCH((tmp_flash_adr),tmpnm)){
			        	bit_LdTmpBufRamReset((tmp_flash_adr),tmpnm);
						FlashSaveOk=1;	
					}
				}
				else{
					if(bitChk_FLRDSPCH((tmp_flash_adr),tmpnm))	Can2RcvBuf[5+i]=1;	
					else										Can2RcvBuf[5+i]=0;
				}


				if(tmpnm >= 0x8){
					tmpnm=0x01;
					tmp_flash_adr++;
				}				
				else	tmpnm++;	
			}			
			
			if(FlashSaveOk){	
				SaveVerify = 0x55;
	   			flash_write_DspChar(F_BLOCK1);
				SaveVerify = 0x0;
			}

			Can2TxThisPt=0;
			Can2TxCnt=Can2RcvBuf[1]+2;
			Can2RxGood=1;
            break;
        default:
			Can2RxGood=0;
			break;
    }        

	SetSidEid();

	Can2RcvBuf[0]= (Can2RcvBuf[0] | REQ_ACK);

    return(0);
}




LocalType __attribute__((section(".usercode"))) Can2DataCheck(void)
{    
	if(Can2RcvBuf[3] == 0){	
		// if bit==0  or  r/w nm=0  return(0);
		Can2RxThisPt=0;
		Can2RxCnt=0;
		return(0);
	}


	if(Can2RcvBuf[0] & BIT_DEG){
		if(Can2RcvBuf[4] == 0){
			// bit nm == 0 return(0);
			Can2RxThisPt=0;
			Can2RxCnt=0;
			return(0);
		}
	}
    

/*
	if( (CommonRxCnt-2) == CommonRcvBuf[1]){
		if(CommonRcvBuf[0] & WRITE_CMD){
			if(CommonRcvBuf[0] & BIT_DEG){
				if( (CommonRxCnt-4) == (CommonRcvBuf[4] + 1)){
					CommonCmdSort();
				}
			}
			else{
				if( (CommonRxCnt-4) == CommonRcvBuf[3]){
					CommonCmdSort();
				}
			}			
		}
		else{
			CommonCmdSort();
		}
	}	
	
	CommonRxThisPt=0;
	CommonRxCnt=0;

    return(0);
*/



	if( (Can2RxCnt-2) == Can2RcvBuf[1]){
		if(Can2RcvBuf[0] & WRITE_CMD){
			if(Can2RcvBuf[0] & BIT_DEG){
				if( (Can2RxCnt-4) == (Can2RcvBuf[4] + 1)){
					Can2CmdSort();
				}
			}
			else{
				if( (Can2RxCnt-4) == Can2RcvBuf[3]){
					Can2CmdSort();
				}	
			}		
		}
		else{
			Can2CmdSort();
		}
	}	
	
	Can2RxThisPt=0;
	Can2RxCnt=0;

    return(0);

}



void  __attribute__((section(".usercode")))     Can2GetRxSidEidDlc(void)
{
    unsigned long ltmp1,ltmp2;

	ltmp1=(unsigned long)C2RX0EID;
	ltmp1=(ltmp1 << 6);

	ltmp2=(unsigned long)C2RX0DLC;
	ltmp2=(ltmp2 >> 10);

	Can2RxEid=(ltmp2 | ltmp1);
	Can2RxSid=(C2RX0SID >> 2);
	Can2RxDlc=(C2RX0DLC & 0x0f);	

	
	Can2RxLocalAddr=0;
	if(Can2RxEid & 0x4000)	Can2RxLocalAddr=(Can2RxLocalAddr | 0x01);	
	if(Can2RxEid & 0x8000)	Can2RxLocalAddr=(Can2RxLocalAddr | 0x02);	
	if(Can2RxEid & 0x10000)	Can2RxLocalAddr=(Can2RxLocalAddr | 0x04);	

	Can2RxGroupAddr=0;
	if(Can2RxSid & 0x80)	Can2RxGroupAddr=(Can2RxGroupAddr | 0x01);	
	if(Can2RxSid & 0x100)	Can2RxGroupAddr=(Can2RxGroupAddr | 0x02);	
	if(Can2RxSid & 0x200)	Can2RxGroupAddr=(Can2RxGroupAddr | 0x03);	
	if(Can2RxSid & 0x400)	Can2RxGroupAddr=(Can2RxGroupAddr | 0x08);	
	if(Can2RxEid & 0x20000)	Can2RxGroupAddr=(Can2RxGroupAddr | 0x10);

}


void      __attribute__((section(".usercode"))) Can2SaveReceiveData(void)
{
	CAN2_Buf[0] =(unsigned char)(C2RX0B1 & 0x00ff);
	CAN2_Buf[1] =(unsigned char)((C2RX0B1 >> 8) & 0x00ff);     	
	CAN2_Buf[2] =(unsigned char)(C2RX0B2 & 0x00ff);
	CAN2_Buf[3] =(unsigned char)((C2RX0B2 >> 8) & 0x00ff);     
	CAN2_Buf[4] =(unsigned char)(C2RX0B3 & 0x00ff);
	CAN2_Buf[5] =(unsigned char)((C2RX0B3 >> 8) & 0x00ff);     
	CAN2_Buf[6] =(unsigned char)(C2RX0B4 & 0x00ff);
	CAN2_Buf[7] =(unsigned char)((C2RX0B4 >> 8) & 0x00ff);     
}




void      __attribute__((section(".usercode"))) Can2ReceiveDataSave(void)
{
    unsigned int  i=0;

	Can2SaveReceiveData();
	Can2RxThisPt=(unsigned int)((Can2RxEid>>8) & 0x0f);
	Can2RxThisPt=(Can2RxThisPt * 8);

	if(Can2RxThisPt==0)	Can2RxCnt=0;	

	for(i=0;i<Can2RxDlc;i++){
		Can2RcvBuf[Can2RxThisPt+i]=CAN2_Buf[i];
		Can2RxCnt++; 
	}

	if(DATA_BLOCK_END  & Can2RxEid){
		Can2DataCheck();
	}
}




/*
void      __attribute__((section(".usercode"))) Can2ReceiveSIDEIDArrary(void)
{

    unsigned int  tmpid1,tmpid2;

	
    if(C2RX0EID & 0x80){
        C2ReceiveAdrStatus=MASTER_TX_SLAVE;
    }
    else{
        C2ReceiveAdrStatus=SLAVE_TX_MASTER;
        tmpid1=(C2RX0DLC >> 10) & 0x0007;
        tmpid2=(C2RX0EID << 6)  & 0x00c0;
        C2ReceiveSlaveAdr=(tmpid1 | tmpid2);
    }
}
*/




void _ISR_X _C2Interrupt(void)
{

    if(C2INTFbits.RX0IF){        
		Can2GetRxSidEidDlc();
		if( (Can2RxLocalAddr == MyLocalAddr) && (Can2RxGroupAddr == MyGroupAddr)){ 
			Can2ReceiveDataSave();
//			OtherWork();
		}
      	else{
			OtherWork();
		}

        C2INTFbits.RX0IF=0;
        C2RX0CONbits.RXFUL=0;
    }    


    else{
        C2CTRLbits.ABAT=1;      // aboart all pending transmit
        C2INTFbits.RX1IF=0;
        C2RX1CONbits.RXFUL=0;
        C2INTFbits.WAKIF=0;
        C2INTF=0;
        C2INTFbits.ERRIF=0;        
    }

    C2IF=0;
    C2INTFbits.RX0IF=0;
    C2RX0CONbits.RXFUL=0;
}



