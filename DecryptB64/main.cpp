#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::string base64_decode(const std::string &);
std::string DecryptB64(std::string str);

const std::string &Salt1 = "ES::SF::TON";
const std::string &Salt2 = "117_:/_42";
const std::string &Salt3 = "SierraOne1Se7en";
const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


int main(int argc, char *argv[])
{
    if(argc != 3) return std::cout << "Invalid number of args. Require Input and Output file." << std::endl,  2;

    std::string in {argv[1]};
    std::string out {argv[2]};

    std::ifstream fin(in);
    if(!fin) return std::cout << "Unable to open input file '" << in << "'" << std::endl, 3;
    std::cout << "Decrypting file " << in << std::endl;

    std::string data;
    fin >> data;
    if(!fin) return std::cout << "Input file '" << in << "' corrupted." << std::endl, 4;

    data = DecryptB64(data);

    std::ofstream fout(out);
    if(!fout) return std::cout << "Unable to open output file '" << out << "'" << std::endl, 5;
    fout << data;

    std::cout << "File " << in << " successfully decrypted to " << out << std::endl;
    return 0;
}

std::string DecryptB64(std::string str){
    str = str.erase(2, 1);
    str = str.erase(4, 1);
    str = base64_decode(str);
    str = str.substr (Salt3.length() + Salt1.length());
    str = str.substr (0, str.length() - Salt2.length());
    str = base64_decode(str);
    str = str.substr (0, str.length() - Salt1.length());
    str = str.erase (7, Salt3.length());
    str = base64_decode(str);
    str = str.substr (Salt1.length());
    str = str.substr (0, str.length() - Salt2.length() - Salt3.length());
    return str;
}

std::string base64_decode(const std::string &str){
    std::vector<int> vec(256, -1);
    for(auto i = 0; i < 64; ++i){
        vec[BASE64_CODES[i]] = i;
    }
    auto val {0};
    auto bits {-8};
    std::string decoded;
    for(const auto &c : str){
        if(vec[c] == -1)break;
        val = (val << 6) + vec[c];
        bits += 6;

        if(bits >= 0){
            decoded.push_back(char((val >> bits) & 0xFF));
            bits -= 8;
        }
    }
    return decoded;
}
