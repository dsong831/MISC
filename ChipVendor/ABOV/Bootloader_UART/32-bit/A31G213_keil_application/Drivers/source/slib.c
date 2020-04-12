#include <stdio.h>
#include "A31G21x.h"
#include "debug_frmwrk.h"
#include "slib.h"


//char	InData[80];
//int		InFlag;
//int		InCount;

int	getstring(void)
{
	int	ch;
	
	InFlag=0; InCount=0;
	
	while(1){
		ch=_DG;
		if(ch>0) {
			if(InCount<80) {
				if(InCount==0&&ch<0x20) {
					InData[0]=0;
					return ch;
				}
				_DBC(ch);
				if(ch==8) {
					_DBC(' ');
					_DBC(8);
					InCount--;
					continue;
				}

				InData[InCount++]=ch;
			}
			if(ch==TERMINATE_CHAR) {
				InData[InCount]=0;
				InFlag=1;
				return 0;
			}
		}
	}
}


char	*scani(char *s, unsigned int *result)
{
	uint32_t n=0;
	int i;

	*result=0;
	while(*s!=0&&*s==' ') s++;
	if(*s==0)
		return NULL;
	else
	while((i=*s)!=0) {
		s++;
		if(i==',')
			break;

		if(i>='0'&&i<='9')
			i-='0';
		else
		if(i>='A'&&i<='F')
			i-='A'-10;
		else
		if(i>='a'&&i<='f')
			i-='a'-10;
		else
			break;
		n=(n<<4)+i;
	}
	*result=n;
	return s;
}

char	*scans(char *s,char *result)
{
	int i;
	while(*s!=0&&*s==' ') s++;
	while((i=*s)!=0) {
		s++;
		if(/*i==' '||*/i==',')
			break;
		*result++=i;
	}
	*result=0;
	return s;
}

