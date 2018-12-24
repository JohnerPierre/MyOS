#include "library.h"

/*
* Copie value in add pointed by dst
*void* dst destination
*int value value to copie
*uint count number of data copie
*/
void *memset(void *dst, int value, uint count)
{
	uint i;
	char *d = dst;
	for(i=0;i<count;i++){
		d[i] = value;
	}
	return 0;
}

/*
* Copies data
* void* dst destination of the copie
* void* src source from the copie
* uint count number of data copie
*/
void *memcpy(void *dst, void *src, uint count)
{
	uint i;
    char *csrc = (char *)src;
    char *cdst = (char *)dst;
	for(i=0;i<count;i++){
	    cdst[i] = csrc[i];
	}
    return 0;
}

/*
* Compare two char*
* Char* p 
* Char* q  
* uint n number of char to compare
*/
int strncmp(const char *p, const char *q, uint n){
	for(uint i=0;i<n;i++){
	    if(p[i]>q[i])
	        return 1;
	        
	    if(p[i]<q[i])
	        return -1;
	
	}
    return 0;
}

/*
* Lenght of char*
* Char* testStr 
* int n number of char
*/
int strlen( char *testStr )
{
    int rtnCount;
    for(rtnCount = 0; testStr[rtnCount]; rtnCount++ ) 
    { }
    return rtnCount;
} 


/*
* Convert Int to String
* Integer value number to convert
* Char* result  contain the result
* Integer b base of convert
*/
void itos(int value, char* result, int b){
    char digit[] = "0123456789ABCDEF";
    int count = 0;
    int d = value;
    int i;
    while(d != 0){
        d = d / b;
        count++;
    }
	if(count == 0)
		count++;
    result[count] = '\0';
    d = value;
    for(i=count-1;i >= 0; i--){
        result[i] = digit[d % b];
        d = d / b;
    }
    return;
}

/*
* Print in screen 
* char* s string to display
* ... number of parameter variable accept %s,%x,%c,%d
* % alone is not suported & and number arguments must be equals to % in string
*/
void print(char*s, ...){
    int *ap;
    int i = 0;
    char data[10];
    
    ap = (int*)&s;

    while(s[i] != '\0'){
        if(s[i] == '%'){
            //Next Argument
            ap++;
            if(s[i+1] == 'd'){
                itos((int)*ap, data, 10);
                displayStringCursor(data);
            }
			else if(s[i+1] == 'x'){
                itos((int)*ap, data, 16);
                displayStringCursor(data);
            }
            else if(s[i+1] == 'c'){
                displayCharCursor((char)*ap);
            }
			else if(s[i+1] == 's'){ 
                displayStringCursor((char*)*ap);
            }
			i++;
        }else if (s[i] == '\n'){
			uint8_t x,y;
			getCursorPos(&x, &y);
			setCursorPos(0,y+1);
		}else{
			displayCharCursor(s[i]);
		}
		i++;
    }
	
    return;
}

