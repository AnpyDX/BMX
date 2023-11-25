/**
*                    ___           ___
*     _____         /__/\         /__/|
*    /  /::\       |  |::\       |  |:|
*   /  /:/\:\      |  |:|:\      |  |:|
*  /  /:/~/::\   __|__|:|\:\   __|__|:|
* /__/:/ /:/\:| /__/::::| \:\ /__/::::\____
* \  \:\/:/~/:/ \  \:\~~\__\/    ~\~~\::::/
*  \  \::/ /:/   \  \:\           |~~|:|~~
*   \  \:\/:/     \  \:\          |  |:|
*    \  \::/       \  \:\         |  |:|
*     \__\/         \__\/         |__|/
*
* Block Mixture Language Binding for C++
* -> Implementation of c++ binding for BMX
* - Project: https://github.com/AnpyDX/bmx-lib
* - Version: 2.0 (for C++)
*
* @copyright Copyright (c) 2022-2023 AnpyD
* Distributed under MIT license, or public domain if desired and recognized in your jurisdiction.
*/

#include "bmx/bmx.h"
#include <vector>


/* @section Internal Members */
namespace {
    enum BMX_KEYWORD: int {
        ATTR_BEGIN_SIGN = 0,
        ATTR_END_SIGN,
        BLOCK_BEGIN_SIGN,
        BLOCK_END_SIGN
    };

    std::map<int, char> BMX_SIGN_MAP = {
        { ATTR_BEGIN_SIGN, '@' }, 
        { ATTR_END_SIGN, ':' },
        { BLOCK_BEGIN_SIGN, '[' },
        { BLOCK_END_SIGN, ']' },
    };

    class BMXPraser final {
    public:
        BMXPraser() = default;

        BMX::Data praser(const std::vector<std::string>& lines_buffer) {
            BMX::Data data;

            for (int i = 0; i < lines_buffer.size(); i++) {
                /* Is Attribute? */
                if (lines_buffer[i][0] == BMX_SIGN_MAP[ATTR_BEGIN_SIGN]) {
                    
                }

                /* Is Block? */
                else if (lines_buffer[i][0] == BMX_SIGN_MAP[BLOCK_BEGIN_SIGN]) {
                    
                }
            }
        }

    private:
        /**
        * @brief remove space from two side of input string
        */
        std::string getCleanString(const std::string& str) {
            if (str.empty()) return str;

            int begin_index, end_index;
            for (int i = 0; i < str.size(); i++) {
                if (str[i] != ' ') {
                    begin_index = i;
                    break;
                }
            }
            for (int i = str.size() - 1; i != -1; i--) {
                if (str[i] != ' ') {
                    end_index = i;
                    break;
                }
            }

            return str.substr(begin_index, end_index - begin_index + 1);
        }
    } _Praser;
}

/* @section BMX Types Impl */
BMX::Exception::Exception(const std::string& info) {
    this->m_exception_info = info;
}

BMX::Exception::~Exception() {}

const char* BMX::Exception::what() const {
    return this->m_exception_info.c_str();
}

/* @section BMX Functions Impl */
BMX::Data load(const std::string& str) {
    if (str.empty()) {
        throw BMX::Exception("fatal error: Failed to load BMX Data from an empty string!");
    }

    std::vector<std::string> lines;
    std::string current_line = "";

    for (auto i = str.begin(); i != str.end(); i++) {
        if (*i == '\n') {
            lines.push_back(current_line);
            current_line = "";
            continue;
        }
        
        current_line.append(std::to_string(*i));
    }

    if (!current_line.empty()) {
        lines.push_back(current_line);
    }

    return _Praser.praser(lines);
}