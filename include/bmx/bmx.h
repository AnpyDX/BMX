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
* - Project: https://github.com/AnpyDX/bmx-lib
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
    * @brief common exception type of BMX.
    */
    class Exception : public std::exception {
    public:
        Exception(const std::string& info);

        virtual ~Exception();

        virtual const char* what() const;

    private:
        std::string m_exception_info;
    };

    /**
    * @brief syntax exception type that BMX-Parser throws while parsing.
    */
    class SyntaxException : public std::exception {
    public:
        SyntaxException(const std::string& info);

        virtual ~SyntaxException();

        virtual const char* what() const;

    private:
        std::string m_exception_info;
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