

#include <p30fxxxx.h>

        
#include  "30f6010_io.h"            
#include  "host_io.h"            
#include  "iodef.h"
#include  "door.h"
//#include  "eeprom.h"
#include  "com.h"
#include  "counter.h"



unsigned int   CAN_Buf[8];
unsigned int   CAN2_Buf[8];




void LoadCanBuffer(unsigned int i)
{
    unsigned int   j; 
    unsigned int   this_pt; 

	for(j=0;j<8;j++){
    	CAN_Buf[j]= sRamDArry[mCurrentState1 + j + i];
	}


/*
    switch(i){
        case    0:
            CAN_Buf[0]= sRamDArry[mCurrentState1];
            CAN_Buf[1]= sRamDArry[mCurrentState1];
            CAN_Buf[2]= sRamDArry[mCurrentState1];
            CAN_Buf[3]= sRamDArry[mMainErrStatus];
            CAN_Buf[4]= sRamDArry[mMainWarningStatus];
            CAN_Buf[5]= sRamDArry[mOut1ErrStatus];
            CAN_Buf[6]= sRamDArry[mOut2ErrStatus];
            CAN_Buf[7]= sRamDArry[mOut1WarningStatus];
            break;
        case    1:
            CAN_Buf[0]= sRamDArry[mOut2WarningStatus];
            CAN_Buf[1]= sRamDArry[mInStatus];
            CAN_Buf[2]= sRamDArry[mOut1Status];
            CAN_Buf[3]= sRamDArry[mOut1Status];
            CAN_Buf[4]= sRamDArry[mExtra1];
            CAN_Buf[5]= sRamDArry[mExtra2];
            CAN_Buf[6]= sRamDArry[Hour];
            CAN_Buf[7]= sRamDArry[Minuate];
            break;
        case    2:
            CAN_Buf[0]= sRamDArry[sec];
            CAN_Buf[1]= sRamDArry[mMainVoltage];
            CAN_Buf[2]= sRamDArry[mMainCurrent];
            CAN_Buf[3]= sRamDArry[mTrVoltage];     
            CAN_Buf[4]= sRamDArry[mTrCurrent];
            CAN_Buf[5]= sRamDArry[mOut0Current];
            CAN_Buf[6]= sRamDArry[mOut1Current];
            CAN_Buf[7]= sRamDArry[mOut2Current];
            break;
        case    3:            
            CAN_Buf[0]= sRamDArry[mOut3Current];
            CAN_Buf[1]= sRamDArry[mOut4Current];
            CAN_Buf[2]= sRamDArry[mOut5Current];
            CAN_Buf[3]= sRamDArry[mOut6Current];     
            CAN_Buf[4]= sRamDArry[mOut7Current];
            CAN_Buf[5]= sRamDArry[mOut8Current];
            CAN_Buf[6]= sRamDArry[mOut9Current];
            CAN_Buf[7]= sRamDArry[mExtra3];
            break;
        case    4:            
            CAN_Buf[0]= sRamDArry[mExtra4];
            CAN_Buf[1]= sRamDArry[mExtra4];
            CAN_Buf[2]= sRamDArry[mExtra4];
            CAN_Buf[3]= sRamDArry[Year];     
            CAN_Buf[4]= sRamDArry[Month];
            CAN_Buf[5]= sRamDArry[Day];
            CAN_Buf[6]= sRamDArry[];
            CAN_Buf[7]= sRamDArry[];
            break;
        case    5:            
            CAN_Buf[0]= sRamDArry[mCarKey33];
            CAN_Buf[1]= sRamDArry[mCarKey41];
            CAN_Buf[2]= sRamDArry[mCarKey49];
            CAN_Buf[3]= sRamDArry[mCarKey57];     
            CAN_Buf[4]= sRamDArry[FLR_ON_OFF4];
            CAN_Buf[5]= sRamDArry[FLR_ON_OFF4];
            CAN_Buf[6]= sRamDArry[FLR_ON_OFF4];
            CAN_Buf[7]= sRamDArry[FLR_ON_OFF4];
            break;
        case    6:            
        case    7:            
        case    8:            
        case    9:            
        case    10:            
            CAN_Buf[0]= 0;
            CAN_Buf[1]= 0;
            CAN_Buf[2]= 0;
            CAN_Buf[3]= 0;     
            CAN_Buf[4]= 0;
            CAN_Buf[5]= 0;
            CAN_Buf[6]= 0;
            CAN_Buf[7]= 0;
            break;
    }
*/
} 


void youCAN1TxSidSet(int MsgFlag,unsigned int sid)
{
    unsigned int  uppereid;

    uppereid=((sid<<5) & 0xf800);
    sid=((sid<<2) & 0x00fc);
    sid=(sid | uppereid);

    switch(MsgFlag)
    {
    case 0:
        C1TX0SID  = (C1TX0SID & 0x0003); 
        C1TX0SID  = (C1TX0SID | sid);
        C1TX0SID  = (C1TX0SID | 0x01);
        break;
    case 1:
        C1TX1SID  = (C1TX1SID & 0x0003); 
        C1TX1SID  = (C1TX1SID | sid);
        C1TX1SID  = (C1TX1SID | 0x01);
        break;
    case 2:
        C1TX2SID  = (C1TX2SID & 0x0003); 
        C1TX2SID  = (C1TX2SID | sid);
        C1TX2SID  = (C1TX2SID | 0x01);
	    break;
    default:
        C1TX0SID  = (C1TX0SID & 0x0003); 
        C1TX0SID  = (C1TX0SID | sid);
        C1TX0SID  = (C1TX0SID | 0x01);
        break;
    }
}


void youCAN1TxEidSet(int MsgFlag,unsigned int eidh, unsigned int eidl)
{
    unsigned int  x1,x2;

    x1=(eidh << 14) & 0xc000;

    x2=(eidl >> 2)  & 0x3000;
    x1=x1 | x2;

    x2=(eidl >> 6)  & 0x00ff;
    x1=x1 | x2;

    x2=(eidl << 10)  & 0xfc00;


    switch(MsgFlag)
    {
    case 0:
        C1TX0EID = x1;
        C1TX0DLC = (C1TX0DLC & 0x03ff); 
        C1TX0DLC = (C1TX0DLC | x2); 
        break;
    case 1:
        C1TX1EID = x1;
        C1TX1DLC = (C1TX1DLC & 0x03ff); 
        C1TX1DLC = (C1TX1DLC | x2); 
        break;
    case 2:
        C1TX2EID = x1;
        C1TX2DLC = (C1TX2DLC & 0x03ff); 
        C1TX2DLC = (C1TX2DLC | x2); 
	    break;
    default:
        C1TX0EID = x1;
        C1TX0DLC = (C1TX0DLC & 0x03ff); 
        C1TX0DLC = (C1TX0DLC | x2); 
        break;
    }
}



/*
void youCAN1TxSidEidSet(int MsgFlag,unsigned int sid, unsigned long eid)
{

    unsigned long  x1,x2;
    unsigned int  uppereid;

    sid=(sid<<2);
    uppereid=sid;
    uppereid=(uppereid<<3) & 0xf800;
    sid=(sid & 0x00ff);
    sid=(sid | uppereid);

    x1=(eid << 10) & (unsigned long)0x00fffc00;
    x2=(eid << 16) & (unsigned long)0xf0000000;

    x1=(x1 | x2);
    uppereid = (x1 >> 16); 

    switch(MsgFlag)
    {
    case 0:
        C1TX0SID  = (C1TX0SID & 0x0003); 
        C1TX0SID  = (C1TX0SID | sid);

        C1TX0EID = uppereid;
        uppereid = (unsigned int)x1;
        C1TX0DLC = (C1TX0DLC & 0x03ff); 
        C1TX0DLC = (C1TX0DLC | uppereid); 
        break;
    case 1:
        C1TX1SID  = (C1TX1SID & 0x0003); 
        C1TX1SID  = (C1TX1SID | sid);

        C1TX1EID = uppereid;
        uppereid = (unsigned int)x1;
        C1TX1DLC = (C1TX1DLC & 0x03ff); 
        C1TX1DLC = (C1TX1DLC | uppereid);

        break;
    case 2:
        C1TX2SID  = (C1TX2SID & 0x0003); 
        C1TX2SID  = (C1TX2SID | sid);

        C1TX2EID = uppereid;
        uppereid = (unsigned int)x1;
        C1TX2DLC = (C1TX2DLC & 0x03ff); 
        C1TX2DLC = (C1TX2DLC | uppereid);
	    break;
    default:
        C1TX0SID  = (C1TX0SID & 0x0003); 
        C1TX0SID  = (C1TX0SID | sid);

        C1TX0EID = uppereid;
        uppereid = (unsigned int)x1;
        C1TX0DLC = (C1TX0DLC & 0x03ff); 
        C1TX0DLC = (C1TX0DLC | uppereid);
        break;
    }

}
*/

/*********************************************************************
* Function Name     : CAN1SetFilter
* Description       : This function sets the acceptance filter values 
*                     (SID and EID) for the specified filter
* Parameters        : char: filter_no
*                     unsigned int: sid register value  
*                     unsigned long: eid registers value
* Return Value      : None 
*********************************************************************/


void youCAN1SetFilter(int filter_no, unsigned int sid, unsigned int eidh,unsigned int eidl)
{      
    unsigned int uppereid;
    unsigned int uppereid1,uppereid2;

    sid=(sid << 2) & 0x1ffc;

    uppereid  =(eidl << 10) & 0xfc00;

    uppereid1 =(eidl >> 6)  & 0x03ff; 
    uppereid2 =(eidh << 10) & 0x0c00; 
    uppereid1 = (uppereid1 | uppereid2);

    switch(filter_no)
    {
    case 0:
      C1RXF0SID  = (C1RXF0SID | sid);
      C1RXF0SID  = (C1RXF0SID | 0x0001); 

      C1RXF0EIDH = uppereid1;   
      C1RXF0EIDL = uppereid;        
      break;
    case 1:
      C1RXF1SID  = (C1RXF1SID | sid);
      C1RXF1SID  = (C1RXF1SID | 0x0001); 

      C1RXF1EIDH = uppereid1;   
      C1RXF1EIDL = uppereid;        
      break;
    case 2:
      C1RXF2SID  = (C1RXF2SID | sid);
      C1RXF2SID  = (C1RXF2SID | 0x0001); 
      C1RXF2EIDH = uppereid1;   
      C1RXF2EIDL = uppereid;        
      break;
    case 3:
      C1RXF3SID  = (C1RXF3SID | sid);
      C1RXF3SID  = (C1RXF3SID | 0x0001); 
      C1RXF3EIDH = uppereid1;  
      C1RXF3EIDL = uppereid;        
      break;
    case 4:
      C1RXF4SID  = (C1RXF4SID | sid);
      C1RXF4SID  = (C1RXF4SID | 0x0001); 
      C1RXF4EIDH = uppereid1;  
      C1RXF4EIDL = uppereid;       
      break;
    case 5:
      C1RXF5SID  = (C1RXF5SID | sid);
      C1RXF5SID  = (C1RXF5SID | 0x0001); 
      C1RXF5EIDH = uppereid1;  
      C1RXF5EIDL = uppereid;        
      break;
    default:
      C1RXF0SID  = (C1RXF0SID | sid);
      C1RXF0SID  = (C1RXF0SID | 0x0001); 
      C1RXF0EIDH = uppereid1;  
      C1RXF0EIDL = uppereid;        
      break;
    }
}


/*************************************************************************
* Function Name     : CAN1SetMask
* Description       : This function sets the values for the acceptance 
*                     filter mask registers (SID and EID)
* Parameters        : char: mask_no
*                     unsigned int: sid register value  
*                     unsigned long: eid registers value
* Return Value      : None 
**************************************************************************/


void youCAN1SetMask(int mask_no, unsigned int sid, unsigned int eidh,unsigned int eidl)
{ 
    unsigned int uppereid;
    unsigned int uppereid1,uppereid2;

    sid=(sid << 2) & 0x1ffc;

    uppereid  =(eidl << 10) & 0xfc00;

    uppereid1 =(eidl >> 6)  & 0x03ff; 
    uppereid2 =(eidh << 10) & 0x0c00; 
    uppereid1 = uppereid1 | uppereid2;

 
    switch(mask_no)
    {
    case 0:
      C1RXM0SID = (C1RXM0SID | sid);
      C1RXM0SID = (C1RXM0SID & 0x0001);

      C1RXM0EIDH = uppereid1;     
      C1RXM0EIDL = uppereid;          
      break;
    case 1:
      C1RXM1SID = (C1RXM1SID | sid);
      C1RXM1SID = (C1RXM1SID & 0x0001);

      C1RXM1EIDH = uppereid1;   
      C1RXM1EIDL = uppereid;        
      break;
    default:
      C1RXM0SID = (C1RXM0SID | sid);
      C1RXM0SID = (C1RXM0SID & 0x0001);
      C1RXM0EIDH = uppereid1;    
      C1RXM0EIDL = uppereid;         
      break;
    }
}





/////port2
void youCAN2SetFilter(int filter_no, unsigned int sid, unsigned int eidh,unsigned int eidl)
{      
    unsigned int uppereid;
    unsigned int uppereid1,uppereid2;

    sid=(sid << 2) & 0x1ffc;

    uppereid  =(eidl << 10) & 0xfc00;

    uppereid1 =(eidl >> 6)  & 0x03ff; 
    uppereid2 =(eidh << 10) & 0x0c00; 
    uppereid1 = uppereid1 | uppereid2;

    switch(filter_no)
    {
    case 0:
      C2RXF0SID  =0;  
      C2RXF0SID  = (C2RXF0SID | sid);
      C2RXF0SID  = (C2RXF0SID | 0x0001); 
      
	  C2RXF0EIDH = uppereid1;         
	  C2RXF0EIDL = uppereid;

//      C2RXF0EIDH = (MyLocalAddr << 8);   
//      C2RXF0EIDL = 0x0000;
        
      break;
    case 1:
      C2RXF1SID  =0;  
      C2RXF1SID  = (C2RXF1SID | sid);
      C2RXF1SID  = (C2RXF1SID | 0x0001); 

      C2RXF1EIDH = uppereid1;   
      C2RXF1EIDL = uppereid;        
      break;
    case 2:
      C2RXF2SID  =0;  
      C2RXF2SID  = (C2RXF2SID | sid);
      C2RXF2SID  = (C2RXF2SID | 0x0001); 
      C2RXF2EIDH = uppereid1;   
      C2RXF2EIDL = uppereid;        
      break;
    case 3:
      C2RXF3SID  =0;  
      C2RXF3SID  = (C2RXF3SID | sid);
      C2RXF3SID  = (C2RXF3SID | 0x0001); 
      C2RXF3EIDH = uppereid1;  
      C2RXF3EIDL = uppereid;        
      break;
    case 4:
      C2RXF4SID  =0;  
      C2RXF4SID  = (C2RXF4SID | sid);
      C2RXF4SID  = (C2RXF4SID | 0x0001); 
      C2RXF4EIDH = uppereid1;  
      C2RXF4EIDL = uppereid;       
      break;
    case 5:
      C2RXF5SID  =0;  
      C2RXF5SID  = (C2RXF5SID | sid);
      C2RXF5SID  = (C2RXF5SID | 0x0001); 
      C2RXF5EIDH = uppereid1;  
      C2RXF5EIDL = uppereid;        
      break;
    default:
      C2RXF0SID  =0;  
      C2RXF0SID  = (C2RXF0SID | sid);
      C2RXF0SID  = (C2RXF0SID | 0x0001); 
      C2RXF0EIDH = uppereid1;  
      C2RXF0EIDL = uppereid;        
      break;
    }
}


/*************************************************************************
* Function Name     : CAN1SetMask
* Description       : This function sets the values for the acceptance 
*                     filter mask registers (SID and EID)
* Parameters        : char: mask_no
*                     unsigned int: sid register value  
*                     unsigned long: eid registers value
* Return Value      : None 
**************************************************************************/


//    unsigned int ll2;
                                      
void youCAN2SetMask(int mask_no, unsigned int sid, unsigned int eidh,unsigned int eidl)
{ 
    unsigned int uppereid;
    unsigned int uppereid1,uppereid2;

//3 =eidh
//c0=eidl

    sid=(sid << 2) & 0x1ffc;


    uppereid  =(eidl << 10) & 0xfc00;
    uppereid1 =(eidl >> 6)  & 0x03ff; 
    uppereid2 =(eidh << 10) & 0x0c00; 
    uppereid1 = uppereid1 | uppereid2;

 
    switch(mask_no)
    {
    case 0:
      C2RXM0SID = 0;
      C2RXM0SID = (C2RXM0SID | sid);
      C2RXM0SID = (C2RXM0SID | 0x0001);


      C2RXM0EIDH = uppereid1;     
      C2RXM0EIDL = uppereid;


//      C2RXM0EIDH = (MyLocalAddr << 8);     
//      C2RXM0EIDL = 0;     
          
      break;
    case 1:
      C2RXM1SID = 0;
      C2RXM1SID = (C2RXM1SID | sid);
      C2RXM1SID = (C2RXM1SID | 0x0001);

      C2RXM1EIDH = uppereid1;   
      C2RXM1EIDL = uppereid;        
      break;
    default:
      C2RXM0SID = 0;
      C2RXM0SID = (C2RXM0SID | sid);
      C2RXM0SID = (C2RXM0SID | 0x0001);
      C2RXM0EIDH = uppereid1;    
      C2RXM0EIDL = uppereid;         
      break;
    }
}

void youCAN2TxSidSet(int MsgFlag,unsigned int sid)
{
    unsigned int  uppereid;

    uppereid=((sid<<5) & 0xf800);
    sid=((sid<<2) & 0x00fc);
    sid=(sid | uppereid);

    switch(MsgFlag)
    {
    case 0:
        C2TX0SID  = (C2TX0SID & 0x0003); 
        C2TX0SID  = (C2TX0SID | sid);
        C2TX0SID  = (C2TX0SID | 0x01);
        break;
    case 1:
        C2TX1SID  = (C2TX1SID & 0x0003); 
        C2TX1SID  = (C2TX1SID | sid);
        C2TX1SID  = (C2TX1SID | 0x01);
        break;
    case 2:
        C2TX2SID  = (C2TX2SID & 0x0003); 
        C2TX2SID  = (C2TX2SID | sid);
        C2TX2SID  = (C2TX2SID | 0x01);
	    break;
    default:
        C2TX0SID  = (C2TX0SID & 0x0003); 
        C2TX0SID  = (C2TX0SID | sid);
        C2TX0SID  = (C2TX0SID | 0x01);
        break;
    }
}



void youCAN2TxEidSet(int MsgFlag,unsigned int eidh, unsigned int eidl)
{
    unsigned int  x1,x2;

    x1=(eidh << 14) & 0xc000;
    x2=(eidl >> 2)  & 0x3000;
    x1=x1 | x2;
    x2=(eidl >> 6)  & 0x00ff;
    x1=x1 | x2;

    x2=(eidl << 10)  & 0xfc00;


    switch(MsgFlag)
    {
    case 0:
        C2TX0EID = x1;
        C2TX0DLC = (C2TX0DLC & 0x03ff); 
        C2TX0DLC = (C2TX0DLC | x2); 
        break;
    case 1:
        C2TX1EID = x1;
        C2TX1DLC = (C2TX1DLC & 0x03ff); 
        C2TX1DLC = (C2TX1DLC | x2); 
        break;
    case 2:
        C2TX2EID = x1;
        C2TX2DLC = (C2TX2DLC & 0x03ff); 
        C2TX2DLC = (C2TX2DLC | x2); 
	    break;
    default:
        C2TX0EID = x1;
        C2TX0DLC = (C2TX0DLC & 0x03ff); 
        C2TX0DLC = (C2TX0DLC | x2); 
        break;
    }
}

