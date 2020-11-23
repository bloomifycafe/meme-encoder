#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

/*
 * Function: encode
 * Arguments: string original, u8 offset, string xor_key
 * Returns: vector<int> of padded encoded string
 */
std::vector<uint32_t> encode(std::string orig, uint8_t offset, std::string key)
{
    std::vector<uint32_t> ret{};
    uint32_t v1 = 0;
    for (auto i = 0; i < orig.size(); ++i)
    {
        if (!key.empty())
            for (auto it = key.begin(); it != key.end(); ++it)
                orig[i] ^= *it;

        if (!(i % 4)) {
            if (i) ret.push_back(v1);
            v1 = ((orig[i] + offset) << 24);
        } else {
            v1 |= ((orig[i] + offset) << (24 - 8 * i));
        }

    }
    ret.push_back(v1);
    return ret;
}

int main(int, char**)
{
    std::string str, key; int offset = -1, i = 0;

    std::cout << "Type the string you want to encode: ";
    std::getline(std::cin, str);
    std::cout << "Key: ";
    std::getline(std::cin, key);
    std::cout << "Offset: ";
    std::cin >> offset;

    while (offset < 0 || offset > 255) {
        std::cout << "Offset: ";
        std::cin >> offset;
    }
    auto encoded_vec = encode(str, offset, key);
    auto encoded_key = encode(key, 0, {});

    std::stringstream ss;
    ss << R"(#include <iostream>
#include <vector>
int main(int, char**) {
    std::vector<uint32_t> v1 { )";
    for (i = 0; i < encoded_vec.size(); ++i)
        ss << std::hex << "0x" << encoded_vec.at(i) << (i < encoded_vec.size() - 1 ? "u, " : "u ") << std::dec;
    ss << "}, v2 { ";
    for (i = 0; i < encoded_key.size(); ++i)
        ss << std::hex << "0x" << encoded_key.at(i) << (i < encoded_key.size() - 1 ? "u, " : "u ") << std::dec;
    ss << "};";

    ss << R"(
    char c; int i, j;
    for (auto it = v1.begin(); it != v1.end(); ++it) {
        for (i = 0; i < 4; ++i) {
            c = char((*it >> (24 - 8 * i)) - )" << std::hex << "0x" << std::setfill('0') << std::setw(2) << offset << ");" <<
        R"(
            for (auto it2 = v2.rbegin(); it2 != v2.rend(); ++it2) {
                for (j = 0; j < 4; ++j)
                    c ^= char(*it2 >> (8 * j));
            }
            if ((unsigned char)c < 0x80)
                std::cout << c;
        }
    }
}
)";

    auto s = ss.str();
    std::cout << "---------------------------------------------------------------------------------------------------------" << std::endl << std::endl;
    std::cout << s << std::endl;
    std::cout << "---------------------------------------------------------------------------------------------------------" << std::endl;

    std::getchar();
}
