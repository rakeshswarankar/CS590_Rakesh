/* This is the main file for myls command. It use supportive file myls.c.
*  Author : Rakesh Kumar Swarankar
*  Date   : 06/24/2015
*  Work   : CS590 Midterm Project
*  FileNm : main.c
*/
#include<stdio.h>
#include"../include/myls.h"

int main(int argc,char **argv)
{
    int index=argc-1;
    char *dir=".";    
    short dirFlag=0;    
    
    if(populateFlag(argc,argv)==0){
    	while(dir!=NULL && index > 0)   
   	{  
   	    dir=populateDirectoryName(&index,argv);
   	    if(dir != NULL){
	    	dirFlag=1;	  
	    //printf("Directory %s\n",dir);
	    listContent(dir);
	    }
	}
        if(dirFlag==0)
    	{
    	    dir=".";
	    //printf("Directory %s\n",dir);
	    listContent(dir);
    	}
    }
    else
    {
    	perror("No command Found!");
	return(-1);
    }
   return(0);
}     

