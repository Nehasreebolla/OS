#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include<dirent.h>
#include <fcntl.h>
int cp(int argc, char *argv[]) 
{   
    char path1[100];
    char path2[100];

    int i,opt,vflag,iflag,lflag,fflag;
    vflag=iflag=lflag=fflag=0;
    optind=1;
    while((opt=getopt(argc,argv,"vilf"))!=-1)
 {
  switch(opt)
  {
    case 'v':
       vflag=1;
       break;
    case 'i':
      iflag=1;
      break;
    case 'l':
       lflag=1;
        break;
    case 'f':
      fflag=1;
      break;
    default:
      printf("\n Invalid option");
  }
 }

  char src_fle[100], dst_fle[100];

  if (!vflag && !lflag && !iflag && !fflag)
   {
  strcpy(src_fle,argv[1]);
  strcpy(dst_fle,argv[2]); 
 FILE *src, *dst;
        char ch;


        src = fopen(src_fle, "r");
        if (src == NULL)
        {
         printf("Error opening src file.\n");
                return -1;
        }

        dst = fopen(dst_fle, "w");
        if (dst == NULL)
        {
         printf("Error opening dst file.\n");
                return -1;
        }
          while ((ch = fgetc(src)) != EOF)
        {
         fputc(ch, dst);
        }

        printf("File copied successfully");
        fclose(src);
        fclose(dst);
   }
else
 {
  strcpy(src_fle,argv[2]);
  strcpy(dst_fle,argv[3]);
} 
 if(lflag)
 {
        char ch;

        
	 int fd1, fd2;

	 fd2 = link(src_fle, dst_fle);
         if (fd2 < 0) 
	 {
             printf("Error creating hard link\n");
             return -1;
         }


         printf("Hard link created successfully\n");

}
  else if(vflag)
	    {
        FILE *src, *dst;
        char ch;


        src = fopen(src_fle, "r");
        if (src == NULL)
        {
         printf("Error opening src file.\n");
                return -1;
        }

        dst = fopen(dst_fle, "w");
        if (dst == NULL)
        {
         printf("Error opening dst file.\n");
                return -1;
        }

        while ((ch = fgetc(src)) != EOF)
        {
         fputc(ch, dst);
        }

        printf("%s is copied to %s.\n",src_fle,dst_fle);
        fclose(src);
        fclose(dst);


         }
  else if(fflag)
       {               
        FILE *src, *dst;
        char ch;


        src = fopen(src_fle, "r");
        if (src == NULL) 
	{
         printf("Error opening src file.\n");
		return -1;
        }

        dst = fopen(dst_fle, "w");
        if (dst == NULL) 
	{
         printf("Error opening dst file.\n");
		return -1;
        }

        while ((ch = fgetc(src)) != EOF) 
	{
         fputc(ch, dst);
        }

        printf("File copied successfully.\n");
        fclose(src);
        fclose(dst);

         }
  else if(iflag)
	    {   
	      FILE *src, *dst;
              char ch;


              src = fopen(src_fle, "r");
              if (src == NULL) 
	      {
              printf("Error opening src file.\n");
		return -1;
              }

	      dst = fopen(dst_fle, "r");
              if(dst != NULL)
              {
                printf("Error: The file already exists .\n");
                fclose(dst);
	      }
              dst = fopen(dst_fle, "w");
              if(dst == NULL)
              {
                printf("Error: Unable to create file in the destination directory.\n");
                fclose(src);
              }
	      printf("Do you want to overwrite Y/n: ");
              ch=getchar();
              if(ch=='Y')
             {
              while ((ch = fgetc(src)) != EOF)
              {
                fputc(ch, dst);
              }

              fclose(src);
              fclose(dst);

              printf("File copied successfully.\n");
            }
            else
           {
            return -1;
           }


          }
	
} 
