#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>
#include<unistd.h>
#include"tokenizer.h"
#include"ls.h"
#include"ps.h"
#include"cp.h"
#include"mv.h"
#include"man.h"
#include"grep.h"
int main()
{  char *cmd_line,*cmd, *token, sh_prompt[150];
   int cmd_line_len,argc=0,retval;
  int lflag=0,iflag=0,oflag=0,gflag=0,pflag=0,option;    
   int i,j,k=0;

 sprintf(sh_prompt,"%s@%s:~%s ",getenv("USER"),getenv("HOSTNAME"),getenv("PWD"));
 printf("\033[2J");
 printf("\033[0;0H");

  while(1)
   { 
   cmd_line=readline(sh_prompt);
   cmd_line_len=strlen(cmd_line);
   if (strcmp(cmd_line,"exit")!=0)
   {
   cmd=(char*)malloc(cmd_line_len * sizeof(char));
   token=(char*)malloc(cmd_line_len * sizeof(char));
   char **argv=malloc(sizeof(char*) * 150 );
   for(i=0;i<150;i++)
    argv[i]=(char*)malloc(150);

   argc=tokenize_cmd(argv,cmd_line);

    if (strcmp(argv[0],"ls")==0)
     { 
        retval=process_ls_options(argc,argv);
     }
    else if (strcmp(argv[0],"ps")==0)
     { 
        retval=process_ps_cmd(argc,argv);
        }
    else if (strcmp(argv[0],"cp")==0)
     {
       retval=cp(argc,argv);
     }
    else if (strcmp(argv[0],"mv")==0)
     {
       retval=mv(argc,argv);
     }
    else if (strcmp(argv[0],"grep")==0)
    { 
       retval=grep(argc,argv);
    }
    else if (strcmp(argv[0],"man")==0)
     { 
         retval=man(argc,argv);
         printf("\033[2J");
         printf("\033[0;0H");
     }   
    else if (strcmp(argv[0],"help")==0)
       { if (strcmp(argv[1],"ls")==0)
           printf("\nls: ls [-a][-l][-i][-s] [file] ...");
         else if (strcmp(argv[1],"ps")==0)
           printf("\nps: ps [-a][-l][-x][-e]");
         else if (strcmp(argv[1],"cp")==0)
           printf("\ncp: cp [-v][-i][-l][-f]");
         else if (strcmp(argv[1],"mv")==0)
           printf("\nmv: mv [-v][-i][-l][-f]");
         else if (strcmp(argv[1],"grep")==0)
           printf("\ngrep: grep [-i][-c][-n][-v]");
         else
           printf("\nunknown command");
       }
    else
      printf("\nhelp command not found");
  if (strlen(cmd_line)>0)
     add_history(cmd_line);

  free(token);
  free(cmd_line);
  for(i=0;i<argc;i++)
     free(argv[i]);
  argc=0;
  printf("\n");
}
else
{
  printf("\nGOOD BYE");
  break;
}
}

   printf("\n");
   return 0;
}
