int man(int argc, char *argv[])
{
  FILE *fp;
  char ch;
  printf("\033[2J");
  printf("\033[0;0H");
  int found=0;   
  if ((strcmp(argv[1],"ls"))==0)
   {  fp=fopen("ls.txt","r");
      found=1; }
  else if ((strcmp(argv[1],"ps"))==0)
   {  fp=fopen("ps.txt","r");
      found=1;
   }
  else if ((strcmp(argv[1],"cp"))==0)
   {  fp=fopen("cp.txt","r");
      found=1;
   }
  else if ((strcmp(argv[1],"mv"))==0)
    { fp=fopen("mv.txt","r");
      found=1;
    }
  else if ((strcmp(argv[1],"grep"))==0)
    { fp=fopen("grep.txt","r");
      found=1;
    }
  if (found == 1)
  {
  while((ch=fgetc(fp))!=EOF)
     printf("%c",ch);
  printf("\n:");
  while(1)
   {
     ch=getchar();
     if (ch == 'q')
        break;
   }
 }
 else
   printf("\n Invalid command for man");  
  return 0;
}

