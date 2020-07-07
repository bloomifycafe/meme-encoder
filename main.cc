////////////////////////////////////////////////////////////////////////
//                                                                    //
// phan mem b40 ch0t thoi dai 4.0                                     //
// written by angel from bloomify.me. Copyright (c) 2020 bloomify.me. //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
std::vector<int> encode(std::string orig, unsigned char offset, std::string key)
{
    std::vector<int> ret{};
    int v1 = 0;
    for (auto i = 0; i < orig.size(); ++i)
    {
        if (!key.empty())
            for (auto it = key.begin(); it != key.end(); ++it)
                orig[i] ^= *it;

        switch (i % 4)
        {
        case 0:
            if (i) ret.push_back(v1);
            v1 = 0;
            v1 = ((orig[i] + offset) << 24);
            break;
        case 1:
            v1 |= ((orig[i] + offset) << 16);
            break;
        case 2:
            v1 |= ((orig[i] + offset) << 8);
            break;
        case 3:
            v1 |= (orig[i] + offset);
            break;
        }
    }
    ret.push_back(v1);
    return ret;
}

int main()
{
    std::string str, key; int offset = -1, i = 0;
    std::cout << "bloomify.me's bao chot software by angel. copyright (c) 2020 bloomify.me" << std::endl;
    std::cout << "Type the string you want to encode: ";
    std::getline(std::cin, str);
    std::cout << "Key: ";
    std::getline(std::cin, key);
    std::cout << "Offset: ";
    std::cin >> offset;

    while (offset < 0 && offset > 255) {
        std::cout << "Offset: ";
        std::cin >> offset;
    }
    auto encoded_vec = encode(str, offset, key);
    auto encoded_key = encode(key, 0, {});
    std::cout << R"(
#include <iostream>
#include <vector>

int main(int, char**) {
    std::vector<unsigned> v1 { )";
    for (i = 0; i < encoded_vec.size(); ++i)
        std::cout << std::hex << "0x" << encoded_vec.at(i) << (i < encoded_vec.size() - 1 ? "u, " : "u ") << std::dec;
    std::cout << "}, v2 { ";
    for (i = 0; i < encoded_key.size(); ++i)
        std::cout << std::hex << "0x" << encoded_key.at(i) << (i < encoded_key.size() - 1 ? "u, " : "u ") << std::dec;
    std::cout << "};";

    std::cout << R"(
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
}
