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
#include <array>
#include <functional>


/* @section Internal Members */
namespace {
    struct BMX_SIGN_MAP {
        char BLOCK_BEGIN_SIGN = '[';
        char BLOCK_END_SIGN = ']';
        char ATTR_BEGIN_SIGN = '@';
        char ATTR_END_SIGN = ':';
        char ATTR_ED_SIGN = '!';
        char COMMENT_BEGIN_SIGN = '#';
    };

    static constexpr BMX_SIGN_MAP BMX_SMAP {};

    enum BMX_BLOCK_TYPE {
        BMX_TEXT_BLOCK,
        BMX_ATTRIBUTE_BLOCK,
        BMX_COMMENT_BLOCK
    };


    class BMXParser final {
    public:
        BMXParser() = delete;

        BMXParser(const std::function<bool(std::string& line)>& geter)
        : m_geter(geter) {}


        BMX::Data parse() {
            BMX::Data data {};

            std::string L; // current line string
            int N = 1; // current line number

            /* Parser Context */
            std::string ctx_block_name = "__global__"; // current block's name
            BMX_BLOCK_TYPE ctx_block_type = BMX_ATTRIBUTE_BLOCK; // current block's type

            /* -------------- */

            while (m_geter(L)) {
                
                /* if read BLOCK_BEGIN_SIGN, considered beginning a new block */
                if (L[0] == BMX_SMAP.BLOCK_BEGIN_SIGN) {
                    ctx_block_name = blockNameReader(L, N, ctx_block_type);
                    if (data.texts.find(ctx_block_name) != data.texts.end()) {
                        syntaxMessage(L, L.size() - 1, "Block \"" + ctx_block_name + "\" is existed!");
                    }

                    // TODO 
                    //  1. Block name 的重定义问题
                    //  2. Attribute Block 中 key 的重定义问题
                    //  3. Line Number
                    //  4. 分离 Common 和 Syntax Exception; Syntax Exception 加入行号和位置
                }

                /* if block is BMX_COMMENT_BLOCK type, just skip current line */
                else if (ctx_block_type == BMX_COMMENT_BLOCK) {
                    // SKIP, DO NOTHING
                }

                /* if block is BMX_TEXT_BLOCK type, record this line and append */
                else if (ctx_block_type == BMX_TEXT_BLOCK) {
                    data.texts[ctx_block_name].append(L + "\n");
                }
                
                /* if block is BMX_ATTRIBUTE_BLOCK type, parse line line and append */
                else if (ctx_block_type == BMX_ATTRIBUTE_BLOCK) {
                    // if current line does start with ATTR_BEGIN_SIGN, then parse
                    if (L[0] == BMX_SMAP.ATTR_BEGIN_SIGN) {
                        auto attr = attributeReader(L, N);
                        if (data.attributes[ctx_block_name].find(attr[0]) != data.attributes[ctx_block_name].end()) {
                            syntaxMessage(L, L.size() - 1, "Attribute \"" + attr[0] + "\" is existed!");
                        }

                        data.attributes[ctx_block_name][attr[0]] = attr[1];
                    }

                    // if current line doesn't start with ATTR_BEGIN_SIGN, just ignore
                }
                

                N++;
            }

            return data;
        }

    private:
        /** @section Utility Functions */

        /**
        * @brief remove space from two side of input string
        */
        std::string getCleanString(const std::string& str) {
            if (str.empty()) return str;

            int begin_index = -1;
            int end_index = -1;
            for (int i = 0; i < str.size(); i++) {
                if (str[i] != ' ') {
                    begin_index = i;
                    break;
                }
            }
            if (begin_index == -1) { return ""; }

            for (int i = str.size() - 1; i != -1; i--) {
                if (str[i] != ' ') {
                    end_index = i;
                    break;
                }
            }

            return str.substr(begin_index, end_index - begin_index + 1);
        }

        /**
        * @brief parse block name
        */
        std::string blockNameReader(const std::string& line, int line_number, BMX_BLOCK_TYPE& type) {
            
            int end_index = -1; // the index of BLOCK_END_SIGN
            for (int i = line.size() - 1; i > -1; i--) {
                if (line[i] == BMX_SMAP.BLOCK_END_SIGN) {
                    end_index = i;
                    break;
                }
            }

            if (end_index == -1) {
                syntaxMessage(line, line.size() - 1, "Block decleration isn't closed!");
            }

            for (int i = 1; i < end_index; i++) {
                // FIXME cannot detect unexpected character after end_index
                if (line[i] == BMX_SMAP.BLOCK_BEGIN_SIGN || line[i] == BMX_SMAP.BLOCK_END_SIGN) {
                    syntaxMessage(line, i, "Unexpected bracket in block decleration!");
                }
            }

            std::string block_name = getCleanString(line.substr(1, end_index - 1));

            if (block_name.empty()) {
                syntaxMessage(line, line.size() - 1, "Block's name cannot be empty!");
            }

            switch (block_name[0]) {
            case BMX_SMAP.ATTR_BEGIN_SIGN:
                type = BMX_ATTRIBUTE_BLOCK;
                return block_name.substr(1, block_name.size() - 1);

            case BMX_SMAP.COMMENT_BEGIN_SIGN:
                type = BMX_COMMENT_BLOCK;
                return block_name.substr(1, block_name.size() - 1);

            default:
                type = BMX_TEXT_BLOCK;
                return block_name;
            }
        }

        /**
        * @brief parse attribute line
        * @return [ attr_key, attr_value ]
        */
        std::array<std::string, 2> attributeReader(const std::string& line, int line_number) {
            // if attr is an empty declaration(ED), just cut its key
            if (line[1] == BMX_SMAP.ATTR_ED_SIGN) {
                std::string attr_key = getCleanString(line.substr(2));
                if (attr_key.empty()) {
                    syntaxMessage(line, line.size() - 1, "Attribute key is empty!");
                }

                return { attr_key, std::string("") };
            }

            // if not DE, get its key
            int key_end_index = -1;
            for (int i = 1; i < line.size(); i++) {
                if (line[i] == BMX_SMAP.ATTR_END_SIGN) {
                    key_end_index = i;
                    break;
                }
            }

            if (key_end_index == -1) {
                syntaxMessage(line, line.size() - 1, "Failed to find \":\" after attribute's key!");
            }

            std::string attr_key = getCleanString(line.substr(1, key_end_index - 1));
            if (attr_key.empty()) {
                syntaxMessage(line, line.size() - 1, "Attribute's key is empty!");
            }

            // get attr value
            if (key_end_index == line.size() - 1) {
                syntaxMessage(line, line.size() - 1, "Attribute's value is empty!");
            }

            std::string attr_value = getCleanString(line.substr(key_end_index + 1));

            return { attr_key, attr_value };
        }

        /**
        * @brief syntax error message printer
        */
        void syntaxMessage(const std::string& line, 
                            int warn_pointer_index,
                            const std::string& msg) {

            std::string error_line_hint = line + "\n";
            for (int i = 0; i < line.size(); i++) {
                if (i == warn_pointer_index) {
                    error_line_hint.append("^");
                    break;
                }
                error_line_hint.append("~");
            }

            error_line_hint += "\n";

            throw BMX::SyntaxException("syntax error: " + msg + "\n" + error_line_hint);
        }

    private:
        const std::function<bool(std::string &line)> m_geter;
    };
}

/* @section BMX Types Impl */
BMX::Exception::Exception(const std::string& info) {
    this->m_exception_info = info;
}

BMX::Exception::~Exception() {}

const char* BMX::Exception::what() const {
    return this->m_exception_info.c_str();
}

BMX::SyntaxException::SyntaxException(const std::string& info) {
    this->m_exception_info = info;
}

BMX::SyntaxException::~SyntaxException() {}

const char* BMX::SyntaxException::what() const {
    return this->m_exception_info.c_str();
}

/* @section BMX Functions Impl */
BMX::Data BMX::load(const std::string& str) {
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

    int count = lines.size();
    BMXParser _Parser {
        [&lines, &count](std::string& L) -> bool {
            if (count == 0) { L = std::string(""); return false; }

            L = lines[count - 1];
            count--;
            return true;
        }
    };

    return _Parser.parse();
}

BMX::Data BMX::loads(std::fstream& file) {
    if (!file.is_open()) {
        throw BMX::Exception("Failed to open BMX file!");
    }

    BMXParser _Praser {
        [&file](std::string& L) -> bool {
            return getline(file, L) ? true : false;
        }
    };

    return _Praser.parse();
}

std::string BMX::dumps(const BMX::Data& data) {
    return "";
}