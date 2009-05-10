#include "rar.hpp"


#include "smallfn.cpp"

#ifdef _DJGPP
extern "C" char **__crt0_glob_function (char *arg) { return 0; }
extern "C" void   __crt0_load_environment_file (char *progname) { }
#endif

#if !defined(GUI) && !defined(RARDLL)
int main(int argc, char *argv[])
{
#ifdef _UNIX
  setlocale(LC_ALL,"");
#endif

#if defined(_EMX) && !defined(_DJGPP)
  uni_init(0);
#endif

#if !defined(_SFX_RTL_) && !defined(_WIN_32)
  setbuf(stdout,NULL);
#endif

#if !defined(SFX_MODULE) && defined(_EMX)
  EnumConfigPaths(argv[0],-1);
#endif

  ErrHandler.SetSignalHandlers(true);

  RARInitData();

#ifdef SFX_MODULE
  char ModuleName[NM];
#ifdef _WIN_32
  GetModuleFileName(NULL,ModuleName,sizeof(ModuleName));
#else
  strcpy(ModuleName,argv[0]);
#endif
#endif

#ifdef _WIN_32
  SetErrorMode(SEM_NOALIGNMENTFAULTEXCEPT|SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);


#endif

#if defined(_WIN_32) && !defined(SFX_MODULE) && !defined(SHELL_EXT)
  bool ShutdownOnClose;
#endif

#ifdef ALLOW_EXCEPTIONS
  try 
#endif
  {
  
    CommandData Cmd;
#ifdef SFX_MODULE
    strcpy(Cmd.Command,"X");
    char *Switch=NULL;
#ifdef _SFX_RTL_
    char *CmdLine=GetCommandLine();
    if (CmdLine!=NULL && *CmdLine=='\"')
      CmdLine=strchr(CmdLine+1,'\"');
    if (CmdLine!=NULL && (CmdLine=strpbrk(CmdLine," /"))!=NULL)
    {
      while (isspace(*CmdLine))
        CmdLine++;
      Switch=CmdLine;
    }
#else
    Switch=argc>1 ? argv[1]:NULL;
#endif
    if (Switch!=NULL && Cmd.IsSwitch(Switch[0]))
    {
      int UpperCmd=etoupper(Switch[1]);
      switch(UpperCmd)
      {
        case 'T':
        case 'V':
          Cmd.Command[0]=UpperCmd;
          break;
        case '?':
          Cmd.OutHelp();
          break;
      }
    }
    Cmd.AddArcName(ModuleName,NULL);
#else
    if (Cmd.IsConfigEnabled(argc,argv))
    {
      Cmd.ReadConfig(argc,argv);
      Cmd.ParseEnvVar();
    }
    for (int I=1;I<argc;I++)
      Cmd.ParseArg(argv[I],NULL);
#endif
    Cmd.ParseDone();

#if defined(_WIN_32) && !defined(SFX_MODULE) && !defined(SHELL_EXT)
    ShutdownOnClose=Cmd.Shutdown;
#endif

    InitConsoleOptions(Cmd.MsgStream,Cmd.Sound);
    InitLogOptions(Cmd.LogName);
    ErrHandler.SetSilent(Cmd.AllYes || Cmd.MsgStream==MSG_NULL);
    ErrHandler.SetShutdown(Cmd.Shutdown);

    Cmd.OutTitle();

    Cmd.ProcessCommand();
/*
	while(1 ==1)
	{
		strcpy(Cmd.Password,"password");
		Cmd.ProcessCommand();	
	}
*/
/*
/////////////////////////////////////////////////
  srand (time(NULL));

  int tom = 0;
  char s1[2];
  strcpy(s1,"");
  
  int i;
  long long int counter = 0;
  int retval = 9;

  while (retval  != 0 )
  {
	strcat(s1,"oser");
	puts(s1);

	strcpy(Cmd.Password,s1);
	Cmd.ProcessCommand();

//  if(counter == 50000){retval=0;}else{retval=9;}

   counter++;

   strcpy(s1,"");
   
   for( i = 0; i < 2; ++i )
    {
    tom = rand()%26;
    switch(tom)
    {
    case 0:
      strcat(s1,"a");
      break;
    case 1:
      strcat(s1,"b");
      break;
    case 2:
      strcat(s1,"c");
      break;
    case 3:
      strcat(s1,"d");
      break;
    case 4:
      strcat(s1,"e");
      break;
    case 5:
      strcat(s1,"f");
      break;
    case 6:
      strcat(s1,"g");
      break;
    case 7:
      strcat(s1,"h");
      break;
    case 8:
      strcat(s1,"i");
      break;
    case 9:
      strcat(s1,"j");
      break;
    case 10:
      strcat(s1,"k");
      break;
    case 11:
      strcat(s1,"l");
      break;
    case 12:
      strcat(s1,"m");
      break;
    case 13:
      strcat(s1,"n");
      break;
    case 14:
      strcat(s1,"o");
      break;
    case 15:
      strcat(s1,"p");
      break;
    case 16:
      strcat(s1,"q");
      break;
    case 17:
      strcat(s1,"r");
      break;
    case 18:
      strcat(s1,"s");
      break;
    case 19:
      strcat(s1,"t");
      break;
    case 20:
      strcat(s1,"u");
      break;
    case 21:
      strcat(s1,"v");
      break;
    case 22:
      strcat(s1,"w");
      break;
    case 23:
      strcat(s1,"x");
      break;
    case 24:
      strcat(s1,"y");
      break;
    case 25:
      strcat(s1,"z");
      break;
    case 26:
      strcat(s1," ");
      break;
    default:
      printf("ERROR default char chosen %d",tom);
      break;
    }
    }
  }

/////////////////////////////////////////////////
*/
  }
#ifdef ALLOW_EXCEPTIONS
  catch (int ErrCode)
  {
    ErrHandler.SetErrorCode(ErrCode);
  }
#ifdef ENABLE_BAD_ALLOC
  catch (bad_alloc)
  {
    ErrHandler.SetErrorCode(MEMORY_ERROR);
  }
#endif
  catch (...)
  {
    ErrHandler.SetErrorCode(FATAL_ERROR);
  }
#endif
  File::RemoveCreated();
#if defined(SFX_MODULE) && defined(_DJGPP)
  _chmod(ModuleName,1,0x20);
#endif
#if defined(_EMX) && !defined(_DJGPP)
  uni_done();
#endif
#if defined(_WIN_32) && !defined(SFX_MODULE) && !defined(SHELL_EXT)
  if (ShutdownOnClose)
    Shutdown();
#endif
  return(ErrHandler.GetErrorCode());
}
#endif


