int tokenize_cmd(char **argv,char *cmd_line)
{ int argc=0,i,j,k,cmd_line_len;
   cmd_line_len=strlen(cmd_line);
   char *token=(char*)malloc(150 * sizeof(char));
  for(i=0;cmd_line[i]!='\0'&& (i<cmd_line_len);i++)
   { k=0;
     for(j=i;cmd_line[j]!='\0';j++)
     {   if ( cmd_line[j]==' ')
         { // printf("%d ",i);
            break;
          }
        else
         { token[k++]=cmd_line[j];
           i++;
         }
    }
     token[k]='\0';
     //printf("\n token is  = %s",token);
     strcpy(argv[argc++],token);
   }

  return argc;
}
