#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <signal.h>  //for kill
#include <fcntl.h>
#include <errno.h>
     
void errorMessage(void);
int checkBG(char *input);
int checkRed (char *input);
void redirection (char **args, char *fileName, int counter);
void pythonMode(char **argv);
void buildInAndFormArgs (char *input);
void execute(char **args, char *fileName , int bd, int red, int py);
int main(int argc, char **argv);
     
     
     
void errorMessage(void)
{
  char error_message[30]= "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}
     
     
int checkBG (char *input)
{
  char *ptr;
  ptr = strstr(input, "&");
  if(ptr != NULL){
    *ptr = '\0';
    return 1;
  }
  return 0;
}
     
     
int checkRed (char *input)
{
  int counter = 0; // to indicate how many ">" inpput have
  //checking for number of ">"
  char *beg = input;
  while(*beg)
    {
      if(strchr(">", *beg))
	{
	  counter++;
	}
      beg++;
    }

  return counter;
}
     
     
void redirection (char **args, char *fileName, int counter)
{
  char *temp;
  if(counter >= 2)
    {
      errorMessage();
      exit(0);
    }
           
  else if (counter == 1)
    {
      fileName = strtok(fileName, " ");
      temp = strtok(NULL, " ");
      if(temp)
	{
	  errorMessage();
	  exit(0);
	}
                   
      else {
	close(STDOUT_FILENO);
	int fd = open(fileName, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
	if (fd<0) {
	  errorMessage();
	  exit(0);
	}
	int wrong = 
	  execvp(args[0], args);
	if(wrong)
	  {
	    errorMessage();
	    exit(0);
	  }
      }    
    }
}
     
     
     
void pythonMode(char **argv)
{
  int i;
  int arg_count = 0;
  for (i = 0; argv[i];i++)
    {
      arg_count ++;
    }
           
  char *new_argv[arg_count + 2];
  new_argv[0]="/usr/bin/python";
  for (i=1; i<=arg_count;i++)
    {
      new_argv[i]=argv[i-1];
    }
  new_argv[arg_count+1]=0;
  int wrong =
    execvp(new_argv[0],new_argv);
  if(wrong)
    {
      errorMessage();
    }
}
     
     
     
void buildInAndFormArgs (char *input)
{
  int bg = checkBG(input);
  int counter = checkRed(input);
  int py = 0;
  char **args = (char**)calloc(512,sizeof(char*));
  char **next = args;
  char* temp;
           
  char *fileName = NULL;
           
  if(counter == 1)
    {
      char *rawArg = strtok(input, ">");
      char *fileName = strtok(NULL, ">");
                   
      // find the arguements
      temp = strtok(rawArg, " ");
      while(temp != NULL)
	{
	  *next++ = temp;
	  temp = strtok(NULL, " ");
	}      
      *next = NULL;
      execute(args,fileName ,bg,counter,py);
      return;
    }
           
           
  //argument does not need redirection
  temp = strtok(input, " ");
  while(temp != NULL)
    {
     
      *next++ = temp;
      temp = strtok(NULL, " ");  
    }      
  *next = NULL;
  next=args;
           
           
  if (strcmp(args[0], "exit")==0)
    {
      if(args[1])
	{
	  errorMessage();
	  return;
	}
      exit(0);
      return;
    }  //is our first argument "exit" command?
           
  else if (strcmp(args[0], "cd")==0)  //is our first argument "cd" command?
    {
      if(args[2])
	{
	  errorMessage();
	  return;
	}
      else
	{
	  if ((!args[1])) //is the second argument for "cd" command NULL or ~?
	    {
	      int badcd =
		chdir(getenv("HOME")); //change the current directory to home
	      if(badcd !=0)
		{
		  errorMessage();
		  return;
		}
	      return;
                                   
	    }
	  else
	    {
	      int badcd =
		chdir(args[1]);  //otherwise change the current directory to the given directory
	      if(badcd !=0)
		{
		  errorMessage();
		  return;
		}
	      return;
                                   
	    }
	}
    }
  else if (strcmp(args[0],"wait")==0)
    {
      if(args[1])
	{
	  errorMessage();
	  return;
	}
      
      
      while(waitpid(-1,NULL,0))
	{
	  if(errno == ECHILD)
	    {
	      break;
	    }
	}

    }
           
  else if(strcmp(args[0],"pwd")==0)
    {
      if(args[1])
	{
	  errorMessage();
	  return;
	}
      char *cwd;
      cwd = getcwd(NULL,0);
      write(STDOUT_FILENO,cwd,strlen(cwd));
      write(STDOUT_FILENO,"\n",strlen("\n"));
      return;
    }  
           
  else if (strstr(args[0],".py") !=NULL){ //is our first argument a python program?
    py=1; //if yes set the py_flag
    execute(args,fileName ,bg,counter,py);
  }            
  else
    execute(args,fileName ,bg,counter,py);
}
     
     
void execute(char **args, char *fileName , int bg, int red, int py)
{
  int pid = fork();
  if (pid  <0)  //fork error has happened
    {
      errorMessage();
      exit(0);
    }
  if (pid==0)   //It's the child process and can run his task
    {
      if(red)
	{
	  redirection(args,fileName,red);
	}
      if (bg)
	{
	  int wrong =
	    execvp(args[0],args);
	  if(wrong)
	    {
	      errorMessage();
	      exit(0);
	    }
	}
      if(py)
	{
	  pythonMode(args);
	}
      else
	{
	  int wrong =
	    execvp(args[0],args);
	  if(wrong)
	    {
	      errorMessage();
	    }
	  exit(0);

	}
    }
  else  //pid>0 it's the parent and should wait for the child
    {
     
      if (bg != 1) 
	{
	  (void)waitpid(pid,NULL,0);
	}
           
    }
}
     
     
int main(int argc, char **argv)
{
  FILE *bfd=NULL;       //batch file descriptor
           
  char cwd[100];        //current working directory
           
  int batch_mode=0;     //indicator for batch mode
           
           
  char input[514]= {0};      //get the input
           
  getcwd(cwd,sizeof(cwd));
           
  if (argc > 2) {
    errorMessage();
    exit(1);
  }
     
  //check batch mode
  if (argv[1]!=0)
    {
      batch_mode=1;
      if((bfd = fopen(argv[1], "r")) == NULL)
	{
	  errorMessage();
	  exit(1);
	}
    }  
           
  //get input
           
  while(1)
    {
      if(batch_mode)
	{	      	  


	  while( fgets(input, 514, bfd) != NULL )
	    {
	      
	      char *newLine = (char*)(input + strlen(input) - 1);
	      write(STDOUT_FILENO,input, strlen(input));
	      int wrong = 0;
	      
	      while(!strstr("\n", newLine))
		{
		  wrong = 1;
		  fgets(input, 514, bfd);
		  write(STDOUT_FILENO,input, strlen(input));   
		  newLine = (char*)(input + strlen(input) - 1);
		}
	    
	      if(wrong)
		{
		  errorMessage();
		}

	     
	      if(input == NULL)
		{	
		  exit(0);
		}
	      if (strcmp(input, "\n") == 0)
		{
		  continue;
		}
	      	 
	      
	      if(strlen(input) > 513)
		{
		  continue;
		}
	      
	                        
	      input[strlen(input) - 1] = 0;                  
	      buildInAndFormArgs(input);
	    }
	  exit(0);
	}
     
       
             
             
      if(!batch_mode)
	{
	  
	  write(STDOUT_FILENO,"mysh> ",strlen("mysh> "));
	  char *id = fgets(input, 514,stdin);
	  

	 
	  
	  if(id == NULL)
	    exit(0);
	 
	  if(input == NULL)
	    {	
	      exit(0);
	    }

	  
	  if (strcmp(input, "\n") == 0)
	    {
	      continue;
	    }

	  char *newLine = (char*)(input + strlen(input) - 1);
	  int wrong = 0;
	      
	  while(!strstr("\n", newLine))
	    {
	      wrong = 1;
	      fgets(input, 514, stdin);
	      newLine = (char*)(input + strlen(input) - 1);
	    }

	  
	  input[strlen(input) - 1] = 0;
	  buildInAndFormArgs(input);
  
	}
    }  
  return 0;
}

