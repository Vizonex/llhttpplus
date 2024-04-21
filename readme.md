# llhttpplus

A C++ Wrapper around [llhttp](https://llhttp.org) to make client and server protocols easier to handle...
This library has very minimal C++ code making it a good choice for developers who 
are looking to write an http requests library.


```CPP

#include "llhttpplus.hpp"
#include <unordered_map>
#include <stdio.h>


/* NOTE: llhttp::string can be either std::string or std::string_view depending on your C++ version 
 * This is why llhttp::string is used instead of std::string or std::string_view. At least later versions 
 * of C++ have a performance increase because of std::string_view */


class ClientParser : public llhttp::Parser {
public: 

    /* There's also a built-in static constant expression known as SUCCESS = 0 AND FAIL = -1 */

    /* Using these enums such as staus is entirely optional as the llhttp C library has it's own */
    llhttp::Status status;
    std::string header_name;
    std::string header_value;
    std::string body;

    std::unordered_map<std::string, std::string>headers;


    int on_status_complete(){
        status = static_cast<llhttp::Status>(_handle.status_code);
        switch(status){
            case llhttp::Status::TOO_MANY_REQUESTS: {
                printf("[FAILED] Sever kicked you off for sending out Too Many Requests :(\n");
                return FAIL;
            }
            case llhttp::Status::OK: {
                printf("[SUCCESS] The Server Approved of your Request :)\n")
                return SUCCESS;
            }
    
            /* .... */
 
            default: {
                return SUCCESS;
            }
        }
    }

    int on_header_field(llhttp::string str){
        header_name += str;
        return SUCCESS;
    };
    
    int on_header_value(llhttp::string str){
        header_value += str;
        return SUCCESS;
    };

    int on_header_value_complete(){
        headers.insert(header_name, header_value);
        header_name.clear();
        header_value.clear();
        return SUCCESS;
    }

    int on_body(llhttp::string str){
        body += str;
        return SUCCESS;
    }

    ClientParser(){

        /* The way I was able to get around settings class 
         * callbacks was to use flags for setting 
         * the parser up , Alternatively you could leave the 
         * setup handling to the other developers if your 
         * making an http library of your own with this 
         * library, Alternatively you can pass along 
         * "::ALL" for both DataCallbackSettings and 
         * CallbackSettings */

        setup(
            llhttp::CallbackSettings::Header_value_complete |  llhttp::CallbackSettings::Status_complete, 
            llhttp::DataCallbackSettings::Header_field | llhttp::DataCallbackSettings::Header_value,
            llhttp::ParserType::RESPONSE 
        );
    }
};


int parse_response(ClientParser* myClient, const char* response, size_t response_size){
    /* std::string or std::string_view will also work... */
    return static_cast<int>(myClient->execute(response, response_size));
}

```



## TODOS

- CMAKELISTS.TXT
- Tutorial
- making sure llhttp compiles with this library 
- Testing this library

