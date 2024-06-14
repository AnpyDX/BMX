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
* -> Header of c++ binding for BMX
* - Project: https://github.com/AnpyDX/BMX
* - Version: 2.0 (for C++)
*
* @copyright Copyright (c) 2022-2023 AnpyD
* Distributed under MIT license, or public domain if desired and recognized in your jurisdiction.
*/

#pragma once
#include <string>
#include <fstream>
#include <stdexcept>
#include <map>


namespace BMX {
    
    /** @section Types of BMX **/
    /**
    * @brief base exception type of all BMX exception.
    */
    class BaseException : public std::exception {
    public:
        BaseException() = default;
        ~BaseException() = default;

        virtual const char* what() const noexcept;

    protected:
        std::string m_exception_info;
    };


    /**
    * @brief common exception type of BMX.
    * @note this exception includes following situations
    *       1. Failed to open file while calling func < BMX::loads >
    *       
    */
    class Exception : private BaseException {
    public:
        Exception() = delete;
        Exception(const std::string& info);
        ~Exception() = default;

        virtual const char* what() const noexcept;
    };

    /**
    * @brief syntax exception type that BMX-Parser throws while parsing.
    */
    class SyntaxException : private BaseException {
    public:
        SyntaxException() = delete;
        SyntaxException(const std::string& msg, const std::string& line, int line_number, int error_pointer);
        ~SyntaxException() = default;

        virtual const char* what() const noexcept;

        /**
        * @brief get error line's detail
        */
        virtual void get_detail(std::string& error_line, int& line_number, int& error_pointer) const;

    private:
        std::string m_error_line;
        int m_line_number;
        int m_error_pointer;
    };

    /**
    * @brief BMX data type, which stores the data read from bmx file
    */
    class Data {
    public:
        /** BMX Text Blocks  */
        std::map<std::string, std::string> texts;

        /** BMX Attribute Blocks */
        std::map<std::string, std::map<std::string, std::string>> attributes;
    };

    
    /** @section Functions of BMX **/

    /**
    * @brief load BMX data from std::string
    */
    BMX::Data load(const std::string& str);

    /**
    * @brief load BMX data from std::fstream
    */
    BMX::Data loads(std::fstream& file);

    /**
    * @brief convert BMX data to std::string, which can be used for writting into a file
    */
    std::string dumps(const BMX::Data& data);
}