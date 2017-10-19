
#include  "iodef.h"


unsigned int    __attribute__((section(".usercode"))) CurSelPortIn(unsigned int port_sel)
{
	unsigned int retval=1;
	unsigned int NcNo=1;

    if(port_sel & 0x80)     NcNo=1;      //N_Open
    else                    NcNo=0;      //N_Close

    port_sel=(port_sel & 0x7f);
	
	switch(port_sel){

		case	SILK_X0:
				if(IN_X0_PORT)	    retval=1;		
				else		        retval=0;
                if(!NcNo)           retval=!retval;               
				break;
		case	SILK_X1:
				if(IN_X1_PORT)	    retval=1;		
				else		        retval=0;
                if(!NcNo)           retval=!retval;               
				break;
		case	SILK_X2:
				if(IN_X2_PORT)	    retval=1;		
				else		        retval=0;
                if(!NcNo)           retval=!retval;               
				break;
		case	SILK_X3:
				if(IN_X3_PORT)	    retval=1;		
				else		        retval=0;
                if(!NcNo)           retval=!retval;               
				break;
		case	SILK_X4:
				if(IN_X4_PORT)	    retval=1;		
				else		        retval=0;
                if(!NcNo)           retval=!retval;               
				break;
		case	SILK_X5:
				if(IN_X5_PORT)	    retval=1;		
				else		        retval=0;
                if(!NcNo)           retval=!retval;               
				break;
		case	SILK_X6:
				if(IN_X6_PORT)	    retval=1;		
				else		        retval=0;
                if(!NcNo)           retval=!retval;               
				break;
		case	SILK_X7:
				if(IN_X7_PORT)	    retval=1;		
				else		        retval=0;
                if(!NcNo)           retval=!retval;               
				break;

		default:
				retval=1;
				break;
    }

    return(retval);
}





unsigned int       __attribute__((section(".usercode"))) CurSelOutPort(unsigned int port_sel,unsigned int port_val)
{
	unsigned int retval=1;
	unsigned int NcNo=1;

    if(port_sel & 0x80)     NcNo=1;      //N_Open
    else                    NcNo=0;      //N_Close

    port_sel=(port_sel & 0x7f);

    switch(port_sel){
        case    SILK_OP:
            if(!NcNo)  port_val=!port_val;               
			OUT_OP_PORT=port_val;
            retval=0;
            break;
        case    SILK_CL:
            if(!NcNo)  port_val=!port_val;               
			OUT_CL_PORT=port_val;
            retval=0;
            break;
        case    SILK_S_OP:
            if(!NcNo)  port_val=!port_val;               
			OUT_OP_S_PORT=port_val;
            retval=0;
            break;
        case    SILK_S_CL:
            if(!NcNo)  port_val=!port_val;               
			OUT_CL_S_PORT=port_val;
            retval=0;
            break;
        case    SILK_BK2:
            if(!NcNo)  port_val=!port_val;               
			OUT_BK2_PORT=port_val;
            retval=0;
            break;
        case    SILK_D_S:
            if(!NcNo)  port_val=!port_val;               
			OUT_D_S_PORT=port_val;
            retval=0;
            break;
        case    SILK_GBR:
            if(!NcNo)  port_val=!port_val;               
			OUT_GBR_PORT=port_val;
            retval=0;
            break;
        case    SILK_BK1:
            if(!NcNo)  port_val=!port_val;               
			OUT_BRK_PORT=port_val;
            retval=0;
            break;
        case    SILK_FAN:
            if(!NcNo)  port_val=!port_val;               
			OUT_FAN_PORT=port_val;
            retval=0;
            break;
        case    SILK_LIGHT:
            if(!NcNo)  port_val=!port_val;               
			OUT_LIT_PORT=port_val;
            retval=0;
            break;
        case    SILK_BUZ:
            if(!NcNo)  port_val=!port_val;               
			OUT_BUZ_PORT=port_val;
            retval=0;
            break;
        case    SILK_BELL:
            if(!NcNo)  port_val=!port_val;               
			OUT_BELL_PORT=port_val;
            retval=0;
            break;
        case    SILK_RST:
            if(!NcNo)  port_val=!port_val;               
			OUT_RST_PORT=port_val;
            retval=0;
            break;
        case    SILK_ERR:
            if(!NcNo)  port_val=!port_val;               
			OUT_ERR_PORT=port_val;
            retval=0;
            break;
        case    SILK_HOP:
            if(!NcNo)  port_val=!port_val;               
			OUT_HOP_PORT=port_val;
            retval=0;
            break;
        case    SILK_HCL:
            if(!NcNo)  port_val=!port_val;               
			OUT_HCL_PORT=port_val;
            retval=0;
            break;
        case    SILK_UP:
            if(!NcNo)  port_val=!port_val;               
			OUT_U_W_PORT=port_val;		
            break;
        case    SILK_DN:
            if(!NcNo)  port_val=!port_val;               
			OUT_D_W_PORT=port_val;		
            break;
        case    SILK_P1:
            if(!NcNo)  port_val=!port_val;               
			OUT_P1_PORT=port_val;		
            break;
        case    SILK_P2:
            if(!NcNo)  port_val=!port_val;               
			OUT_P2_PORT=port_val;		
            break;
        case    SILK_P3:
            if(!NcNo)  port_val=!port_val;               
			OUT_P3_PORT=port_val;		
            break;
        case    SILK_P4:
            if(!NcNo)  port_val=!port_val;               
			OUT_P4_PORT=port_val;		
            break;
        case    SILK_DAC:
            if(!NcNo)  port_val=!port_val;               
//            if(!bitChk_FLRDSPCH(F_NcNoBit8,7))  port_val=!port_val;               
			OUT_DAC_PORT=port_val;		
            break;
        default:
            retval=1;
            break;
    }

    return(retval);
}




unsigned int       __attribute__((section(".usercode"))) CurSelOutPortChk(unsigned int port_sel)
{
    unsigned int retval=1;
	unsigned int NcNo=1;

    if(port_sel & 0x80)     NcNo=1;      //N_Open
    else                    NcNo=0;      //N_Close

    port_sel=(port_sel & 0x7f);

    switch(port_sel){
        case    SILK_FAN:
			if(OUT_FAN_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_LIGHT:
			if(OUT_LIT_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_BUZ:
			if(OUT_BUZ_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_BELL:
			if(OUT_BELL_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_RST:
			if(OUT_RST_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_ERR:
			if(OUT_ERR_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_HOP:
			if(OUT_HOP_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_HCL:
			if(OUT_HCL_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_OP:
			if(OUT_OP_PORT)	    retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_CL:
			if(OUT_CL_PORT)	    retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_S_OP:
			if(OUT_OP_S_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_S_CL:
			if(OUT_CL_S_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_BK2:
			if(OUT_BK2_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_D_S:
			if(OUT_D_S_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_GBR:
			if(OUT_GBR_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_BK1:
			if(OUT_BRK_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_UP:
			if(OUT_U_W_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_DN:
			if(OUT_D_W_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_P1:
			if(OUT_P1_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_P2:
			if(OUT_P2_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_P3:
			if(OUT_P3_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_P4:
			if(OUT_P4_PORT)	    retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        case    SILK_DAC:
			if(OUT_DAC_PORT)	retval=1;		
			else		        retval=0;
            if(!NcNo)           retval=!retval;               
            break;
        default:
           retval=1;
           break;
    }
    return(retval);
}



