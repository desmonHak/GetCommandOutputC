#include "cmd_rsp.h" 

// https://codereview.stackexchange.com/questions/162546/send-command-and-get-response-from-windows-cmd-prompt-silently


/* child process's STDIN is the user input or data that you enter into the child process - READ */
static void * g_hChildStd_IN_Rd = NULL;
/* child process's STDIN is the user input or data that you enter into the child process - WRITE */
static void * g_hChildStd_IN_Wr = NULL;
/* child process's STDOUT is the program output or data that child process returns - READ */
static void * g_hChildStd_OUT_Rd = NULL;
/* child process's STDOUT is the program output or data that child process returns - WRITE */
static void * g_hChildStd_OUT_Wr = NULL;

// Private prototypes
static int CreateChildProcess(const char *cmd);
static int ReadFromPipe(char **rsp, unsigned int chunk_size);
static char * ReSizeBuffer(char **str, unsigned int chunk_size);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Prototype:      int cmd_rsp(char *command, char **chunk, size_t chunk_size)  
//
//  Description:    Executes any command that can be executed in a Windows cmd prompt and returns
//                  the response via auto-resizing buffer.
//
//  Inputs:         const char *command - string containing complete command to be sent
//                  char **chunk - initialized pointer to char array to return results
//                  size_t chunk_size - Initial memory chunk_size in bytes of char **chunk.
//
//  Return:         0 for success
//                 -1 for failure
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int __declspec(dllexport) cmd_rsp(const char *command, char **chunk, unsigned int chunk_size)
{
    SECURITY_ATTRIBUTES saAttr;
    
    /// All commands that enter here must contain (and start with) the substring: "cmd.exe /c 
    /// /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// char cmd[] = ("cmd.exe /c \"dir /s\"");  /// KEEP this comment until format used for things like
                                                 /// directory command (i.e. two parts of syntax) is captured
    /// /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    const char rqdStr[] = {"cmd.exe /c "};
    int len = (int)strlen(command);
    char *Command = NULL;
//  if(!strstr(command, rqdStr))
//  {
        Command = calloc(len + sizeof(rqdStr), 1);
        strcat(Command, rqdStr);
        strcat(Command, command);
//  }


    // Set the bInheritHandle flag so pipe handles are inherited. 
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    //child process's STDOUT is the program output or data that child process returns
    // Create a pipe for the child process's STDOUT. 
    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
    {
        return -1;
    }

    // Ensure the read handle to the pipe for STDOUT is not inherited.
    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
    {
        return -1;
    }

    //child process's STDIN is the user input or data that you enter into the child process
    // Create a pipe for the child process's STDIN. 
    if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
    {
        return -1;
    }

    // Ensure the write handle to the pipe for STDIN is not inherited. 
    if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
    {
        return -1;
    }
    
    //eg: CreateChildProcess("adb");
    if(CreateChildProcess(Command) == 0)
    {
        ReadFromPipe(chunk, chunk_size); 
    }
    free(Command);

    // Read from pipe that is the standard output for child process. 
    
    return 0;
}

// Create a child process that uses the previously created pipes for STDIN and STDOUT.
static int CreateChildProcess(const char *cmd)
{
    //TCHAR szCmdline[] = TEXT("cmd.exe /c \"C:\\path\\to\\exe\\program.exe -parameter C:\\path\\to\\file\\file.txt\"");
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    // Set up members of the PROCESS_INFORMATION structure. 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput = g_hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process. 

    bSuccess = CreateProcess(
        NULL,
        (LPSTR)cmd,                     // command line 
        (LPSECURITY_ATTRIBUTES)NULL,    // process security attributes 
        NULL,                           // primary thread security attributes 
        TRUE,                           // handles are inherited 
        CREATE_NO_WINDOW,               // creation flags 
        NULL,                           // use parent's environment 
        (LPCSTR)NULL,                   // use parent's current directory 
        &siStartInfo,                   // STARTUPINFO pointer 
        &piProcInfo);                   // receives PROCESS_INFORMATION 

    // If an error occurs, exit the application. 
    if (!bSuccess)
    {
        return -1;
    }
    else
    {
        // Close handles to the child process and its primary thread.
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
        CloseHandle(g_hChildStd_OUT_Wr);
    }
    return 0;
}

// Read output from the child process's pipe for STDOUT
// Grow the buffer as needed
// Stop when there is no more data. 
static int ReadFromPipe(char **rsp, unsigned int chunk_size)
{
    COMMTIMEOUTS ct;
    int size_recv = 0;
    unsigned int total_size = 0;
    unsigned long dwRead;
    BOOL bSuccess = TRUE;
    char *accum;
    char *tmp1 = NULL;
    char *tmp2 = NULL;
    
    
    //Set timeouts for stream
    ct.ReadIntervalTimeout = 0;
    ct.ReadTotalTimeoutMultiplier = 0;
    ct.ReadTotalTimeoutConstant = 10;
    ct.WriteTotalTimeoutConstant = 0;
    ct.WriteTotalTimeoutMultiplier = 0;
    SetCommTimeouts(g_hChildStd_OUT_Rd, &ct);

                                                          
    //This accumulates each read into one buffer, 
    //and copies back into rsp before leaving
    accum = (char *)calloc(1, sizeof(char)); //grow buf as needed
    if(!accum) return -1;
    memset(*rsp, 0, chunk_size);
    
    do
    {
        //Reads stream from child stdout 
        bSuccess = ReadFile(g_hChildStd_OUT_Rd, *rsp, chunk_size-1, &dwRead, NULL);
        if (!bSuccess || dwRead == 0) 
        {
            free(accum);
            return 0;//successful - reading is done
        }
        
        (*rsp)[dwRead] = 0;
        size_recv = (int)strlen(*rsp);


        if(size_recv == 0)
        {
            //should not get here for streaming
            (*rsp)[total_size]=0;
            return total_size;
        }
        else
        {
            //New Chunk:
            (*rsp)[size_recv]=0;
            //capture increased byte count
            total_size += size_recv+1;
            //increase chunk_size of accumulator
            tmp1 = ReSizeBuffer(&accum, total_size);
            if(!tmp1)
            {
                free(accum);
                strcpy(*rsp, "");
                return -1;
            }
            accum = tmp1;
            strcat(accum, *rsp);
            if(total_size > (chunk_size - 1))
            {   //need to grow buffer
                tmp2 = ReSizeBuffer(&(*rsp), total_size+1);
                if(!tmp2)
                {
                    free(*rsp);
                    return -1;
                }
                *rsp = tmp2;
            }
            strcpy(*rsp, accum);//refresh rsp
        }
        
        
    }while(1);
}

// return '*str' after number of bytes realloc'ed to 'chunk_size'
static char * ReSizeBuffer(char **str, unsigned int chunk_size)
{
    char *tmp=NULL;

    if(!(*str)) return NULL;

    if(chunk_size == 0)
    {
        free(*str);
        return NULL;
    }

    tmp = (char *)realloc((char *)(*str), chunk_size);
    if(!tmp)
    {
        free(*str);
        return NULL;
    }
    *str = tmp;

    return *str;
}