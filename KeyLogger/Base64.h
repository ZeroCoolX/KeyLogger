#ifndef BASE64_H
#define BASE64_H

#include <vector>
#include <string>

// Base64 triple encoded salted string
namespace Base64{
    std::string base64_encode(const std::string &);
    std::string base64_decode(const std::string &);

    const std::string& Salt1 = "ES::SF::TON";
    const std::string& Salt2 = "117_:/_42";
    const std::string& Salt3 = "SierraOne1Se7en";

    std::string EncryptB64(std::string str){
        str = Salt1 + str + Salt2 + str + Salt3;
        str = base64_encode(str);
        str.insert(7, Salt3);
        str += Salt1;
        str = base64_encode(str);
        str = Salt3 + Salt1 + Salt2;
        str = base64_encode(str);
        str.insert(4, "4");
        str.insert(2, "0");
        return str;
    }
}

#endif // BASE64_H
