#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <fstream>

#include <sys/types.h>
#include <dirent.h>

#include "config/ruleset.h"
#include "handler/interfaces.h"
#include "handler/webget.h"
#include "handler/settings.h"
#include "script/cron.h"
#include "server/socket.h"
#include "server/webserver.h"
#include "utils/base64/base64.h"
#include "utils/defer.h"
#include "utils/file_extra.h"
#include "utils/logger.h"
#include "utils/network.h"
#include "utils/rapidjson_extra.h"
#include "utils/system.h"
#include "utils/urlencode.h"
#include "version.h"
#include "generator/config/subexport.h"
#include "./parser/subparser.h"

//#include "vfs.h"

WebServer webServer;

#ifndef _WIN32
void SetConsoleTitle(const std::string &title)
{
    system(std::string("echo \"\\033]0;" + title + "\\007\\c\"").data());
}
#endif // _WIN32

void setcd(std::string &file)
{
    char szTemp[1024] = {}, filename[256] = {};
    std::string path;
#ifdef _WIN32
    char *pname = NULL;
    DWORD retVal = GetFullPathName(file.data(), 1023, szTemp, &pname);
    if(!retVal)
        return;
    strcpy(filename, pname);
    strrchr(szTemp, '\\')[1] = '\0';
#else
    char *ret = realpath(file.data(), szTemp);
    if(ret == NULL)
        return;
    ret = strcpy(filename, strrchr(szTemp, '/') + 1);
    if(ret == NULL)
        return;
    strrchr(szTemp, '/')[1] = '\0';
#endif // _WIN32
    file.assign(filename);
    path.assign(szTemp);
    chdir(path.data());
}

void chkArg(int argc, char *argv[])
{
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-cfw") == 0)
        {
            global.CFWChildProcess = true;
            global.updateRulesetOnRequest = true;
        }
        else if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0)
        {
            if(i < argc - 1)
                global.prefPath.assign(argv[++i]);
        }
        else if(strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--gen") == 0)
        {
            global.generatorMode = true;
        }
        else if(strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
        {
            std::cerr<<"subconverter " VERSION " backend\n";
            return;
        }
        else if(strcmp(argv[i], "--artifact") == 0)
        {
            if(i < argc - 1)
                global.generateProfiles.assign(argv[++i]);
        }
        else if(strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--log") == 0)
        {
            if(i < argc - 1)
                if(freopen(argv[++i], "a", stderr) == NULL)
                    std::cerr<<"Error redirecting output to file.\n";
        }
    }
}

void signal_handler(int sig)
{
    //std::cerr<<"Interrupt signal "<<sig<<" received. Exiting gracefully...\n";
    writeLog(0, "Interrupt signal " + std::to_string(sig) + " received. Exiting gracefully...", LOG_LEVEL_FATAL);
    switch(sig)
    {
#ifndef _WIN32
    case SIGHUP:
    case SIGQUIT:
#endif // _WIN32
    case SIGTERM:
    case SIGINT:
        webServer.stop_web_server();
        break;
    }
}

void cron_tick_caller()
{
    if(global.enableCron)
        cron_tick();
}

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        std::string input; 
        if (strcmp(argv[1], "-f") == 0)
        {
            if(argc <= 2)
            {
                std::cerr << "Error: arg length in file mode must greater than 1." << std::endl;
                return 1;
            }

            std::ifstream infile(argv[2]);
            if(!infile)
            {
                std::cerr << "Error: could not open file " << argv[2] << std::endl;
                return 1; 
            }

            input.assign((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
            infile.close();

            std::remove(argv[2]);
        }
        else
        {
            std::string i = argv[1];
            input = base64Decode(i);
        }

        std::vector<Proxy> Nodes;
        explodeSub(input, Nodes);
        std::string ret = proxyToClashProxy(Nodes);
        std::cout << base64Encode(ret) << std::endl;
        return 0;
    }
    return 0;
}