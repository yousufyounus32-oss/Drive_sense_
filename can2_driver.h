/*can2_driver.c*/
#include <LPC21xx.H>
#include "header.h"

void can2_init(void)
{
	PINSEL1|=0x00014000;//P0.23-->TD2 & P0.24-->RD2
	VPBDIV=1; //PCLK=60MHz

	C2MOD=0x1; //CAN1 into Reset  Mode 
	AFMR=0x2; //accept all receiving messages(data/remote)
	C2BTR=0x001C001D; //B125Kbps @ PLCK=60MHz
	//C2BTR=0x001C0011; //B100Kbps @ PLCK=60MHz
	C2MOD=0x0; //CAN1 into Normal Mode
}
//u32 *ptr=(u32*)0xE0038000;


/*void can2_init(void)
{
	PINSEL1|=0x00014000;//P0.23-->RD2 & P0.24-->TD2
	VPBDIV=1; //PCLK=60MHz
  C2MOD=0x1; //CAN2 into Reset  Mode 
	C2BTR=0x001C001D; //B125Kbps @ PLCK=60MHz
	ptr[0]=0x20012003;//accept id 1 & 3
  ptr[1]=0x20052007;//accept id 5 & 7
  ptr[2]=0x2009200B;//accept id 9 & 11
	SFF_sa=0x0;	//offset of start addr of individual addr table
  SFF_GRP_sa=0xC;
	EFF_sa=0xC;
  EFF_GRP_sa=0xC;
	ENDofTable=0xC;	
	AFMR=0x4;//Enable Acceptance Filter
  C2MOD=0x0; //CAN2 into Normal Mode
}*/






void can2_tx(CAN2_MSG m1){
  C2TID1=m1.id;
	C2TFI1=(m1.dlc<<16);	
	if(m1.rtr==0) //if data frame
	{
		C2TFI1&=~(1<<30);//RTR=0
		C2TDA1=m1.byteA; //lower 4bytes of data
		C2TDB1=m1.byteB; //upper 4bytes of data
	}
	else{
		C2TFI1|=(1<<30); //RTR=1
	}
	C2CMR=(1<<0)|(1<<5);//Start Xmission & select Tx Buf1
	while((C2GSR&(1<<3))==0);//wait for data Xmission.
}


void can2_rx(CAN2_MSG *m1)
{
	while((C2GSR&0x1)==0);//waiting untill message received
	m1->id=C2RID;
	m1->dlc=(C2RFS>>16)&0xF;
	m1->rtr=(C2RFS>>30)&0x1;
	if(m1->rtr==0){ //if data frame
		m1->byteA=C2RDA;
		m1->byteB=C2RDB;
	}
	C2CMR=(1<<2);//free receiver buffer(imp)
}




















