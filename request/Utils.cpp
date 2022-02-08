#include "Utils.hpp"

std::vector<std::string>    ft_split( std::string str, std::string delimiter ) {
    std::vector<std::string> parts;
    while (str.length() && str.find(delimiter) != std::string::npos) {
        std::string tmp = str.substr(0, str.find(delimiter));
        if (tmp.length() > 0) {
            parts.push_back(tmp);
        }
        str.erase(0, str.find(delimiter) + 1);
    }
    if (str.length() > 0) {
        parts.push_back(str);
    }
    return parts;
}
