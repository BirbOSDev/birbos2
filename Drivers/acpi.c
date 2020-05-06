#include <stddef.h>

typedef unsigned int dword;
typedef unsigned short word;
typedef unsigned char byte;

dword *SMI_CMD;
byte ACPI_ENABLE;
byte ACPI_DISABLE;
dword *PM1a_CNT;
dword *PM1b_CNT;
word SLP_TYPa;
word SLP_TYPb;
word SLP_EN;
word SCI_EN;
byte PM1_CNT_LEN;



struct RSDPtr
{
   byte Signature[8];
   byte CheckSum;
   byte OemID[6];
   byte Revision;
   dword *RsdtAddress;
};



struct FACP
{
   byte Signature[4];
   dword Length;
   byte unneded1[40 - 8];
   dword *DSDT;
   byte unneded2[48 - 44];
   dword *SMI_CMD;
   byte ACPI_ENABLE;
   byte ACPI_DISABLE;
   byte unneded3[64 - 54];
   dword *PM1a_CNT_BLK;
   dword *PM1b_CNT_BLK;
   byte unneded4[89 - 72];
   byte PM1_CNT_LEN;
};



unsigned int *acpiCheckRSDPtr(unsigned int *ptr)
{
   char *sig = "RSD PTR ";
   struct RSDPtr *rsdp = (struct RSDPtr *) ptr;
   byte *bptr;
   byte check = 0;
   int i;

   if (memcmp(sig, rsdp, 8) == 0)
   {
      bptr = (byte *) ptr;
      for (i=0; i<sizeof(struct RSDPtr); i++)
      {
         check += *bptr;
         bptr++;
      }

      // found valid rsdpd   
      if (check == 0) {

         return (unsigned int *) rsdp->RsdtAddress;
      }
   }

   return NULL;
}



unsigned int *acpiGetRSDPtr(void)
{
   unsigned int *addr;
   unsigned int *rsdp;

   for (addr = (unsigned int *) 0x000E0000; (int) addr<0x00100000; addr += 0x10/sizeof(addr))
   {
      rsdp = acpiCheckRSDPtr(addr);
      if (rsdp != NULL)
         return rsdp;
   }

   int ebda = *((short *) 0x40E);
   ebda = ebda*0x10 &0x000FFFFF;

   for (addr = (unsigned int *) ebda; (int) addr<ebda+1024; addr+= 0x10/sizeof(addr))
   {
      rsdp = acpiCheckRSDPtr(addr);
      if (rsdp != NULL)
         return rsdp;
   }

   return NULL;
}



int acpiCheckHeader(unsigned int *ptr, char *sig)
{
   if (memcmp(ptr, sig, 4) == 0)
   {
      char *checkPtr = (char *) ptr;
      int len = *(ptr + 1);
      char check = 0;
      while (0<len--)
      {
         check += *checkPtr;
         checkPtr++;
      }
      if (check == 0)
         return 0;
   }
   return -1;
}



int acpiEnable(void)
{

   if ( (inportw((unsigned int) PM1a_CNT) &SCI_EN) == 0 )
   {
  
      if (SMI_CMD != 0 && ACPI_ENABLE != 0)
      {
         outportb((unsigned int) SMI_CMD, ACPI_ENABLE);
         int i;
         for (i=0; i<300; i++ )
         {
            if ( (inportw((unsigned int) PM1a_CNT) &SCI_EN) == 1 )
               break;
            sleep(10);
         }
         if (PM1b_CNT != 0)
            for (; i<300; i++ )
            {
               if ( (inportw((unsigned int) PM1b_CNT) &SCI_EN) == 1 )
                  break;
               sleep(10);
            }
         if (i<300) {
            return 0;
         } else {
            return -1;
         }
      } else {
         return -1;
      }
   } else {
      return 0;
   }
}

int initAcpi(void)
{
   unsigned int *ptr = acpiGetRSDPtr();

   if (ptr != NULL && acpiCheckHeader(ptr, "RSDT") == 0)
   {
      int entrys = *(ptr + 1);
      entrys = (entrys-36) /4;
      ptr += 36/4;

      while (0<entrys--)
      {
         if (acpiCheckHeader((unsigned int *) *ptr, "FACP") == 0)
         {
            entrys = -2;
            struct FACP *facp = (struct FACP *) *ptr;
            if (acpiCheckHeader((unsigned int *) facp->DSDT, "DSDT") == 0)
            {
               char *S5Addr = (char *) facp->DSDT +36;
               int dsdtLength = *(facp->DSDT+1) -36;
               while (0 < dsdtLength--)
               {
                  if ( memcmp(S5Addr, "_S5_", 4) == 0)
                     break;
                  S5Addr++;
               }
               if (dsdtLength > 0)
               {
                  if ( ( *(S5Addr-1) == 0x08 || ( *(S5Addr-2) == 0x08 && *(S5Addr-1) == '\\') ) && *(S5Addr+4) == 0x12 )
                  {
                     S5Addr += 5;
                     S5Addr += ((*S5Addr &0xC0)>>6) +2;

                     if (*S5Addr == 0x0A)
                        S5Addr++;  
                     SLP_TYPa = *(S5Addr)<<10;
                     S5Addr++;

                     if (*S5Addr == 0x0A)
                        S5Addr++;  
                     SLP_TYPb = *(S5Addr)<<10;

                     SMI_CMD = facp->SMI_CMD;

                     ACPI_ENABLE = facp->ACPI_ENABLE;
                     ACPI_DISABLE = facp->ACPI_DISABLE;

                     PM1a_CNT = facp->PM1a_CNT_BLK;
                     PM1b_CNT = facp->PM1b_CNT_BLK;
                     
                     PM1_CNT_LEN = facp->PM1_CNT_LEN;

                     SLP_EN = 1<<13;
                     SCI_EN = 1;

                     return 0;
                  } else {
                  }
               } else {
               }
            } else {
            }
         }
         ptr++;
      }
   } else {
   }

   return -1;
}



void acpiPowerOff(void)
{
   if (SCI_EN == 0){
       print("\nacpi: shutdown failed.\n\n");
      return;
   }

   acpiEnable();
   outportw((unsigned int) PM1a_CNT, SLP_TYPa | SLP_EN );
   if ( PM1b_CNT != 0 )
      outportw((unsigned int) PM1b_CNT, SLP_TYPb | SLP_EN );

}

int shutdownSupported(){
   if(SCI_EN==0){
      return 0;
   }
   return 1;
}