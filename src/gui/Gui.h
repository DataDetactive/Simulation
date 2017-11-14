#ifndef GUI_H
#define GUI_H

#include <string.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <iostream>
#include <iostream>
#include <fstream>

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifndef PLUGIN_DATA_DIR
#define PLUGIN_DATA_DIR_ ""
#else
#define PLUGIN_DATA_DIR_ QUOTE(PLUGIN_DATA_DIR)
#endif

class Gui {
public :

    virtual void init(int argc, char ** argv) = 0;

    virtual int run() = 0;

    /// Get the full path of the current process. The given filename should be the value of argv[0].
    std::string getProcessFullPath(const char* filename)
    {
        if (!filename || filename[0]!='/')
        {
            char path[1024];
            memset(path,0,sizeof(path));
            if (readlink("/proc/self/exe",path,sizeof(path)-1) == -1)
              std::cerr <<"Error: can't read the contents of the link." << std::endl;
            if (path[0])
                return path;
            else
                std::cout << "ERROR: can't get current process path..." << std::endl;
        }

        return filename;
    }

    std::string getParentDir(std::string path)
    {
        std::string::size_type pos = path.find_last_of("/\\");
        if (pos == std::string::npos)
            return ""; // no directory
        else {
            std::string res = path.substr(0,pos);
            if (!res.empty()) res += '/';
            return res;
        }
    }

    std::string getFullPath(const char * fn) {
        {
            std::string filename = fn;
            std::ifstream f(filename.c_str());
            if (f.good()) return filename;
        }

        {
            std::string filename = parentSceneDir + fn;
            std::ifstream f(filename.c_str());
            if (f.good()) return filename;
        }

        {
            std::string filename = parentProcessDir + fn;
            std::ifstream f(filename.c_str());
            if (f.good()) return filename;
        }

        {
            std::string filename = std::string(PLUGIN_DATA_DIR_) + fn;
            std::ifstream f(filename.c_str());
            if (f.good()) return filename;
        }

        return std::string(fn);
    }


    static void cpuid(unsigned int a, unsigned int b[4])
    {
        asm volatile("xchgl %%ebx, %1\n"
                     "cpuid\n"
                     "xchgl %%ebx, %1\n"
                     :"=a"(*b),"=r"(*(b+1)),
                     "=c"(*(b+2)),"=d"(*(b+3)):"0"(a));
    }

    std::string cpu_name()
    {
        unsigned int b[13] = {0};
        cpuid(0x80000000,b);
        unsigned int max = b[0];
        if (max < 0x80000004) return std::string();
        cpuid(0x80000002,b);
        cpuid(0x80000003,b+4);
        cpuid(0x80000004,b+8);
        std::string s;
        b[12] = 0;
        const char* p = (const char*)b;
        char last = '\0';
        while (*p)
        {
            char c = *p; ++p;
            if (c == ' ' && last == ' ') continue;
            if (c == '(')
            {
                while (*p && c != ')') c = *p++;
                continue;
            }
            s += c; last = c;
        }
        return s;
    }

protected :
    std::string parentProcessDir;
    std::string parentSceneDir;
};

#endif
