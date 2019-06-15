#ifndef BASE64_H
#define BASE64_H

#include <vector>
#include <string>

// Base64 triple encoded salted string
namespace Base64{
    const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string base64_encode(const std::string &);

    const std::string &Salt1 = "ES::SF::TON";
    const std::string &Salt2 = "117_:/_42";
    const std::string &Salt3 = "SierraOne1Se7en";

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

    std::string base64_encode(const std::string &str){
        // maps input to base64 (0-63) table
        auto val {0};
        // number of bits in the subgroup
        auto bits {-6};
        // max hex value (63)
        const unsigned int b63 {0X3F};

        std::string encoded;
         for(const auto &c : str){
            // left bitwise shift val = val * 2^8 + ascii(c)
            val = (val << 8) + c;
            // b64 works in octets when extracting the string
            bits += 8;

            while(bits >= 0){
                encoded.push_back(BASE64_CODES[(val >> bits) & b63]);
                // subtract bits in subgroup
                bits -=6;
            }
        }

        if(bits > -6){
            encoded.push_back(BASE64_CODES[((val << 8) >> (bits + 8)) & b63]);
        }

        // Pad is needed
        while(encoded.size() % 4){
            encoded.push_back('=');
        }
        return encoded;
    }
}

#endif // BASE64_H
