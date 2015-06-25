/* This is the supportive file for myls command. It used by main.c file as supportive file.
*  Author : Rakesh Kumar Swarankar
*  Date   : 06/24/2015
*  Work   : CS590 Midterm Project
*  FileNm : myls.c
*/

#include "../include/myls.h"

char *populateDirectoryName(int *index, char *argv[])
{
    char dirPath[128];
    int i= *index;
    
    while(i>0)
    {
	memset(dirPath,0,sizeof(dirPath));
	if(argv[i][0] != '-')
	{
	
	    sprintf(dirPath,"%s",argv[i]);
	    
	    *index=i-1;
	    return argv[i];
	}
	else
	{
	    *index=0;
	    i--;
	    return NULL;
	} 
    }//end while
    return dirPath;
}
void optionHelp()
{
   printf("Format: myls [OPTION] [DirName]..\n This command list content of the specified directory and avaialble with mutiple option with specific objectives.\n");
   printf("OPTION: Option are optional for this command. if any option is not specified then default option listing will take place.\n");
   printf("-l:  Long listing. \n");
   printf("-s:  listing with block size. \n");
   printf("-R:  Recursive listing. \n");
   printf("-Rl: Recusive long listing. \n");
   printf("-S:  Listing in DESC sort order. \n");
   printf("-lS: Long Listing in DESC sort order. \n");
   printf("-t:  Listing in sort order based on modification time. \n");
   printf("-lt: Long Listing in sort order based on modification time. \n");
   printf("-tc: Listing in sort order based on modification time. \n");
   printf("-tu: Listing in sort order based on access time. \n");
   printf("DirName: Specify the directory name for listing. Default is current directory.\n");
}
void getFilePermission(struct stat fileStat,char permission[])
{
    
    permission[0]=fileStat.st_mode & S_IRUSR?'r':'-';
    permission[1]=fileStat.st_mode & S_IWUSR?'w':'-';
    permission[2]=fileStat.st_mode & S_IXUSR?'x':'-';
    permission[3]=fileStat.st_mode & S_IRGRP?'r':'-';
    permission[4]=fileStat.st_mode & S_IWGRP?'w':'-';
    permission[5]=fileStat.st_mode & S_IXGRP?'x':'-';
    permission[6]=fileStat.st_mode & S_IROTH?'r':'-';
    permission[7]=fileStat.st_mode & S_IWOTH?'w':'-';
    permission[8]=fileStat.st_mode & S_IXOTH?'x':'-';
    permission[9]='\0';
}
char getFileType(struct stat fileStat)
{
    char fileType;
    fileType=(fileStat.st_mode & S_IFREG?'-':(fileStat.st_mode & S_IFDIR?'d':(fileStat.st_mode & S_IFBLK?'b':(fileStat.st_mode & S_IFCHR?'c':(fileStat.st_mode & S_IFIFO?'f':(fileStat.st_mode & S_IFLNK?'l':(fileStat.st_mode & S_IFSOCK?'S':'-')))))));
return fileType;
}
void optionLong(char *dirName,int dirFlag)
{
    DIR *dir;
    struct dirent *ent;
    struct stat fileStat;
    char buffer[256];
    uid_t uid;
    gid_t gid;
    struct passwd *mypasswd;
    struct group *mygroup;
    uid=getuid();
    gid=getgid();
    mypasswd = getpwuid(uid);
    mygroup = getgrgid(gid);

    if (dirFlag ==1 && (dir = opendir (dirName)) != NULL) {
    /* print all the files and directories within directory */
   	printf("%s:\n",dirName);
	if(stat(dirName,&fileStat) != 0)
	{
	    perror("File/Directory does not exit!");
	    return;
	}
	printf("total %u\n",(unsigned int)fileStat.st_blocks);
   while ((ent = readdir(dir)) != NULL) {
        if(ent->d_name[0]!= '.')
        {
	    memset(buffer,0,sizeof(buffer));
	    memset(&fileStat,0,sizeof(fileStat));
	    sprintf(buffer,"%s/%s",dirName,ent->d_name);
	    if(stat(buffer,&fileStat) != 0)
	    {
		perror("File/Directory does not exit!");
		return;
	    }
	    memset(buffer,0,sizeof(buffer));
	// FileType
	    char fileType= getFileType(fileStat);
	    sprintf (buffer,"%c",fileType);
	// Permission
	    char permission[10];
		memset(permission,0,sizeof(permission));
		getFilePermission(fileStat,permission);
	    sprintf (&(buffer[strlen(buffer)]),"%s ",permission);
	// number of links
            sprintf (&(buffer[strlen(buffer)]),"%3lu ",(unsigned long )fileStat.st_nlink);
	// user id and group id
	     sprintf (&(buffer[strlen(buffer)]),"%10s %10s ",mypasswd->pw_name, mygroup->gr_name);

        // size, date and file name
	    struct tm * time_info;
	    char timeString[13];

	    time_info = localtime(&fileStat.st_mtime);
	    strftime(timeString, 13, "%b %e %H:%M",time_info);
             sprintf (&(buffer[strlen(buffer)]),"%-10lld %-15s %-s",(unsigned long long)fileStat.st_size,timeString,ent->d_name);
	printf("%s\n",buffer);
    
	}
    }
        printf("\n");
    closedir (dir);
   }
   else // other than directory
  {
	memset(buffer,0,sizeof(buffer));
	if(stat(dirName,&fileStat) != 0)
	{
	    perror("File/Directory does not exit!");
	    return;
	}
	// FileType
	char fileType= getFileType(fileStat);
	sprintf (buffer,"%c",fileType);
	// Permission
	char permission[10];
	memset(permission,0,sizeof(permission));
	getFilePermission(fileStat,permission);
	sprintf (&(buffer[strlen(buffer)]),"%s ",permission);
	// number of links
        sprintf (&(buffer[strlen(buffer)]),"%3lu ",(unsigned long )fileStat.st_nlink);
	// user id and group id
	sprintf (&(buffer[strlen(buffer)]),"%10s %10s ",mypasswd->pw_name, mygroup->gr_name);
        // size, date and file name
	struct tm * time_info;
	char timeString[15];

	time_info = localtime(&fileStat.st_mtime);
	strftime(timeString, 13, "%b %e %H:%M",time_info);

        sprintf (&(buffer[strlen(buffer)]),"%-10lld %-15s",(unsigned long long)fileStat.st_size,timeString);

	if(strrchr(dirName,'/') != NULL)
            sprintf (&(buffer[strlen(buffer)]),"%-s",&(strrchr(dirName,'/')[1]));
        else
	    sprintf (&(buffer[strlen(buffer)]),"%-s",dirName);
	printf("%s\n",buffer);
  }

}
void optionDefault(char *dirName, int dirFlag)
{
    DIR *dir;
    struct dirent *ent;
    if(dirFlag ==1){	
    	if ((dir = opendir (dirName)) != NULL) {
    	/* print all the files and directories within directory */
   	printf("Directory Content for %s\n",dirName);	
   	while ((ent = readdir(dir)) != NULL) {
	    if(ent->d_name[0]!= '.')
        	printf ("%s\t",ent->d_name);
    	}
	printf("\n");
    	closedir (dir); 

   	}
   }
   else
   {
	if(strrchr(dirName,'/') != NULL)
            printf("%s\t",&(strrchr(dirName,'/')[1]));
	else
	    printf ("%s\t",dirName);
   }
}
void optionTime(char *dirName)
{
    DIR *dir;
    struct dirent *ent;
    struct stat fileStat;
    char buffer[256];
    int index=0;

    
        if ((dir = opendir (dirName)) != NULL) {
        /* print all the files and directories within directory */
      //printf("Directory Content for %s\n",dirName);   
        while ((ent = readdir(dir)) != NULL) {
            if(ent->d_name[0]!= '.')
            {
            	memset(buffer,0,sizeof(buffer));
            	memset(&fileStat,0,sizeof(fileStat));
            	sprintf(buffer,"%s/%s",dirName,ent->d_name);
		
                if(stat(buffer,&fileStat) != 0)
                {
                    perror("File/Directory does not exit!");
                    return;
                }

                sprintf(fileList[index].fileName,"%s",buffer);
		if(optionFlag.optionC)
		    fileList[index].time=fileStat.st_ctime;
		else if(optionFlag.optionU)
		    fileList[index].time=fileStat.st_atime;
		else
                    fileList[index].time=fileStat.st_mtime;
                index++;
            }
                //printf ("%s\t",ent->d_name);
        }
        // sorting based on size order is DESC 
        int i,j;
        struct FileList temp;
	double diff=0;
        for(i=0;i<index;i++)
        {   for(j=i+1;j<index;j++)
            {
                diff=difftime(fileList[i].time,fileList[j].time);
		if(diff<0)
                {
                    memset(&temp,0,sizeof(temp));
                    strcpy(temp.fileName,fileList[i].fileName);
                    temp.time=fileList[i].time;
                    
		    strcpy(fileList[i].fileName,fileList[j].fileName);
                    fileList[i].time=fileList[j].time;
                    
		    strcpy(fileList[j].fileName,temp.fileName);
                    fileList[j].time=temp.time;
                }
            }
        }
        for(i=0;i<index;i++)
        {
	    if(optionFlag.optionL)
		optionLong(fileList[i].fileName,0);
	    else
	    	optionDefault(fileList[i].fileName,0);
	}
        printf("\n");
        closedir (dir);
    }
}
void optionRecursive(char *dirName)
{
    DIR *dir;
    struct dirent *ent;
    struct stat fileStat;
    char buffer[256];
    // Get Destination file information
    if(stat(dirName,&fileStat) != 0 )
    {
	perror("Directory does not exit!");
	return;
    }
    // 1. If destination is directory
    if((fileStat.st_mode & S_IFDIR))
    {
	// Check for write permission for user
	if(!(fileStat.st_mode & S_IRUSR))
	{
		perror("Read permission is denied for Target Directory!");
		return;
	}
	if ((dir = opendir (dirName)) != NULL) {
    	/* print all the files and directories within directory */
   	printf("%s:\n",dirName);
	
	if(optionFlag.optionL)
	    printf("total %ld\n",(unsigned long)fileStat.st_blocks);

   	while ((ent = readdir(dir)) != NULL) {
        if(ent->d_name[0]!= '.')
        {
	    memset(buffer,0,sizeof(buffer));
	    memset(&fileStat,0,sizeof(fileStat));
	    sprintf(buffer,"%s/%s",dirName,ent->d_name);
	    if(stat(buffer,&fileStat) != 0)
	    {
		perror("File/Directory does not exit!");
		return;
	    }
		
	    if(optionFlag.optionL == 1)
		optionLong(buffer,0);
	    else
		optionDefault(buffer,0);
	    
	}
        }// while end
	rewinddir(dir);
	while((ent= readdir(dir))!=NULL) 
	{
	    if(ent->d_name[0]!= '.')
            {
            	memset(buffer,0,sizeof(buffer));
	        memset(&fileStat,0,sizeof(fileStat));
        	sprintf(buffer,"%s/%s",dirName,ent->d_name);
                if(stat(buffer,&fileStat) != 0)
                {
                    perror("File/Directory does not exit!");
                    return;
                }   
    	       //Recursive onlt for directory
	       if((fileStat.st_mode & S_IFDIR))
	       {
	   	   printf("\n\n");
		   optionRecursive(buffer);
	       }
	    }
        }
      }  
    }
}

void optionSort(char *dirName)
{
    DIR *dir;
    struct dirent *ent;
    struct stat fileStat;
    char buffer[256];
    int index=0;
    	/* print all the files and directories within directory */
    	if(stat(dirName,&fileStat) != 0 )
    	{
	    perror("Directory does not exit!");
	    return;
        }
    // 1. If destination is directory
    if((fileStat.st_mode & S_IFDIR))
    {
	// Check for write permission for user
	if(!(fileStat.st_mode & S_IRUSR))
	{
		perror("Read permission is denied for Target Directory!");
		return;
	}
	if ((dir = opendir (dirName)) != NULL) 
	{
    	    /* print all the files and directories within directory */
   	    printf("%s:\n",dirName);
	    if(optionFlag.optionL)
	    	printf("total %ld\n",(unsigned long)fileStat.st_blocks);

   	    //	printf("Directory Content for %s\n",dirName);	
   	    while ((ent = readdir(dir)) != NULL) {
	    	if(ent->d_name[0]!= '.')
            	{
	
		    memset(buffer,0,sizeof(buffer));
		    memset(&fileStat,0,sizeof(fileStat));
	    	    sprintf(buffer,"%s/%s",dirName,ent->d_name);
	    	    if(stat(buffer,&fileStat) != 0)
		    {
		    	perror("File/Directory does not exit!");
	 	    	return;
	    	    }
		    sprintf(fileList[index].fileName,"%s",buffer);
		    fileList[index].size=(unsigned int)fileStat.st_size;
		    index++;
	    	}
		//printf ("%s\t",ent->d_name);
    	    }
	// sorting based on size order is DESC 
	int i,j;
	struct FileList temp;
	for(i=0;i<index;i++)
	{   for(j=i+1;j<index;j++)
	    {
		if(fileList[i].size<fileList[j].size)
		{
		    memset(&temp,0,sizeof(temp));
		    strcpy(temp.fileName,fileList[i].fileName);
		    temp.size=fileList[i].size;
		    strcpy(fileList[i].fileName,fileList[j].fileName);
		    fileList[i].size=fileList[j].size;
		    strcpy(fileList[j].fileName,temp.fileName);
		    fileList[j].size=temp.size;
		}
	    }
	}
	for(i=0;i<index;i++)
        {    
            if(optionFlag.optionL)
                optionLong(fileList[i].fileName,0);
            else
                optionDefault(fileList[i].fileName,0);
        }
	printf("\n");
    	closedir (dir); 
     }
   }
}
void optionSize(char *dirName)
{
    DIR *dir;
    struct dirent *ent;
    struct stat fileStat;
    int fileSizeInBlock=0;
    char buffer[256];
    if ((dir = opendir (dirName)) != NULL) {
    /* print all the files and directories within directory */
   while ((ent = readdir(dir)) != NULL) {
        if(ent->d_name[0]!= '.')
        {
	    memset(buffer,0,sizeof(buffer));
	    memset(&fileStat,0,sizeof(fileStat));
	    sprintf(buffer,"%s/%s",dirName,ent->d_name);
	    if(stat(buffer,&fileStat) != 0)
	    {
		perror("File/Directory does not exit!");
		return;
	    }	    
	    printf ("%u %s\t",fileStat.st_blocks,ent->d_name);
	}
    }
        printf("\n");
    closedir (dir);
   }
}

void listContent(char* dirName)
{
    struct stat fileStat;
    // Get Destination file information
    
    if(stat(dirName,&fileStat) != 0 )
    {
	perror("Directory does not exit!");
	return;
    }
    // 1. If destination is directory
    if((fileStat.st_mode & S_IFDIR))
    {
	// Check for write permission for user
	if(!(fileStat.st_mode & S_IRUSR))
	{
		perror("Read permission is denied for Target Directory!");
		return;
	}
	//printf("OPTION \n");
	if(optionFlag.optionH)
	{
	    optionHelp();
	}
	else if(optionFlag.optionR )
        {
	    optionRecursive(dirName);
	}	
	else if(optionFlag.optionSort)
        {
	    optionSort(dirName);
	}
	else if(optionFlag.optionT)
        {
	    optionTime(dirName);
	}
	else if(optionFlag.optionL)
        {
            optionLong(dirName,1);
	}    
	else if(optionFlag.optionS)
        {
	    optionSize(dirName);
	}
	else
	{
	    optionDefault(dirName,1);
	}
    
    }// Directory case end
    else
    {
    	printf("It is regular file not a directory!\n");
	return;
    }
return;
}


int populateFlag(int argc, char **argv)
{
   int option;
   while((option=getopt(argc,argv,"hlsRtucS")) != -1)
   {
	//printf("Choose Option = %c\n",option);
   	switch(option)
	{
	    case 'h':
		optionFlag.optionH=1;
		break;
	    case 'l':
                optionFlag.optionL=1;
		break;
            case 's':
		optionFlag.optionS=1;
                break;
            case 'R':
                optionFlag.optionR=1;
		break;
            case 't':
                optionFlag.optionT=1;
		break;
            case 'c':
                optionFlag.optionC=1;
		break;
            case 'u':
                optionFlag.optionU=1;
		break;
            case 'S':
                optionFlag.optionSort=1;
		break;
            default:
                printf("Choose wrong option\n");
		return(-1);
	}
	//printf("OPTION Flag : %d\n",optionFlag);
   }
return 0;
}

//int executeOption(int argc, int argv**,
