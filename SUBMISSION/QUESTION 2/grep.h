#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024
#define SIZE 1000

int grep(int argc, char *argv[]) {
    int iflag = 0;
    int cflag = 0;
    int nflag = 0;
    int vflag = 0;
    optind=1;
    int option;
    while ((option = getopt(argc, argv, "icnv")) != -1) 
    {
        switch (option) {
		 case 'i':
                       iflag=1;
                       break;
                 case 'c':
                       cflag=1;
                       break;
                 case 'n':
                       nflag=1;
                       break;
                 case 'v':
                       vflag=1;
                       break;
                 default:
                       printf("\n Invalid option");
                       }
 }
char search_term[150];
char filename[150];
strcpy(search_term,argv[2]);
strcpy(filename,argv[3]);
if(iflag)
  {
    char buffer[1000];
    FILE *fp;
    int line_num = 1;

    fp = fopen(filename, "r");
    if (fp == NULL) 
    {
        printf("Error: Could not open file.\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) 
    {
        char *p = buffer;
        while (*p) 
	{
            *p = tolower(*p);
            p++;
        }

        if (strstr(buffer, search_term) != NULL) 
	{
            printf("\n %s:%d:%s", filename, line_num, buffer);
        }

        line_num++;
    }

    fclose(fp);
}
else if(cflag)
   {
    char file[SIZE], line[SIZE], srch_strng[SIZE];
      strcpy(srch_strng,argv[2]);
    int count=0;
    FILE *fp;
    srch_strng[strcspn(srch_strng, "\n")] = '\0';
    strcpy(file,argv[3]);

    fp = fopen(file, "r");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        return -1;
    }

    while (fgets(line,SIZE, fp))
    {
        if (strstr(line, srch_strng) != NULL)
        {
            count++;
        }
    }
    printf("\n search string found %d times",count);

    fclose(fp);
}

else if(nflag)
   {	
    FILE *fp;
    char file[SIZE], line[SIZE], srch_strng[SIZE];
    strcpy(srch_strng,argv[2]);
    int line_no=0;
    srch_strng[strcspn(srch_strng, "\n")] = '\0';
    strcpy(file,argv[3]);


    fp = fopen(file, "r");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        return -1;
    }

    while (fgets(line,SIZE, fp))
    { line_no++;
        if (strstr(line, srch_strng) != NULL)
        {
            printf("\n search string occurred line : %d ", line_no);
        }
    }

    fclose(fp);
   }
else if(vflag)
    { 
      FILE *fp;
      char file[SIZE], line[SIZE], srch_strng[SIZE];
      strcpy(srch_strng,argv[2]);
      srch_strng[strcspn(srch_strng, "\n")] = '\0';
      strcpy(file,argv[3]);

      fp = fopen(file, "r");

      if (fp == NULL)
     {
        printf("Unable to open file.\n");
        return -1;
     }

      while (fgets(line,SIZE, fp))
     {
        if (strstr(line, srch_strng) == NULL)
        {
            printf("\n%s", line);
        }
     }

    fclose(fp);
   }
}
