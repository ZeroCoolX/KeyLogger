#ifndef HEADER_H
#define HEADER_H

#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

namespace Helper{

    template<typename T>
    std::string ToString(const T&);

    struct DateTime{
        int DD, mm, YYYY, HH, MM, SS;

        DateTime(){
            time_t sec;
            time(&sec);
            struct tm* info = localtime(&sec);

            DD = info->tm_mday;
            mm = info->tm_mon + 1;
            YYYY = 1900 + info->tm_year;
            MM = info->tm_min;
            HH = info->tm_hour;
            SS = info->tm_sec;
        }
        DateTime(int DD, int mm, int YYYY, int HH, int MM, int SS)
        : DD(DD), mm(mm), YYYY(YYYY), HH(HH), MM(MM), SS(SS){}

        DateTime(int DD, int mm, int YYYY)
        : DD(DD), mm(mm), YYYY(YYYY), HH(0), MM(0), SS(0){}

        DateTime Now() const {
            return DateTime();
        }

        std::string GetDateString() const {
            return  std::string(DD < 10 ? "0" : "") + ToString(DD) +
                    std::string(mm < 10 ? ".0" : ".") + ToString(mm) +
                    "." + ToString(YYYY);
        }

        std::string GetTimeString(const std::string &delim = ":") const{
            return  std::string(HH < 10 ? "0" : "") + ToString(HH) + delim +
                    std::string(MM < 10 ? "0" : "") + ToString(MM) + delim +
                    std::string(SS < 10 ? "0" : "") + ToString(SS);
        }

        std::string GetDateTimeString(const std::string &delim = ":") const{
            return GetDateString() + " " + GetTimeString(delim);
        }
    };

    template <typename T>
    std::string ToString(const T &elem){
        std::ostringstream oss;
        oss << elem;
        return oss.str();
    }

    void WriteAppLog(const std::string &str){
        std::ofstream fout("AppLog.txt", std::ios::app);
        fout << "[" << Helper::DateTime().GetDateTimeString() << "]" <<
        "\n" << str << std::endl << "\n";
        fout.close();
    }

}

#endif // HEADER_H
