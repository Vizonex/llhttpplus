/* C++ Wrapper for llhttp with minimal code */

#ifndef llHTTPCPP_H
#define llHTTPCPP_H


#include <string>
#include <cstdint>

#include "llhttp.h"



namespace llhttp { 

/* string_view makes this sort of stuff go by a bit 
 * quicker... However I wanted to make as many C++ versions 
 * as possible compatable with this library at the same time */

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
    /* This is simply std::string_view */
    typedef std::string_view string;
#else
    /* This is simply std::string */
    typedef std::string string;
#endif





/* Little Shortcut incase or whenever llhttp is updated or 
 * whenever methods end up changing ever so slighly */
#define MAP_METHODS(NUM, NAME, FUNC)  NAME = NUM, 


/* This makes it really easy to tweak something without needing to go anywhere else */

#define CALLBACK_METHOD_MAP(XX) \
    XX(1, Message_begin, on_message_begin) \
    XX(2, Headers_complete, on_headers_complete) \
    XX(4, Message_complete, on_message_complete) \
    XX(8, URL_complete, on_url_complete) \
    XX(16, Status_complete, on_status_complete) \
    XX(32, Method_complete, on_method_complete) \
    XX(64, Version_complete, on_version_complete) \
    XX(128, Header_field_complete, on_header_field_complete) \
    XX(256, Header_value_complete, on_header_value_complete) \
    XX(512, Chunk_extension_name_complete, on_chunk_extension_name_complete) \
    XX(1024, Chunk_extension_value_complete, on_chunk_extension_value_complete) \
    XX(2048, Chunk_header, on_chunk_header) \
    XX(4096, Chunk_complete, on_chunk_complete) \
    XX(8192, Reset, on_reset)


#define DATACALLBACK_METHOD_MAP(XX) \
    XX(1, Body, on_body) \
    XX(2, Url, on_url) \
    XX(4, Status, on_status) \
    XX(5, Method, on_method) \
    XX(16, Version, on_version) \
    XX(32, Header_field, on_header_field) \
    XX(64, Header_value, on_header_value) \
    XX(128, Chunk_extension_name, on_chunk_extension_name) \
    XX(256, Chunk_extension_value, on_chunk_extension_value)




/* ===== Flags for implementing callbacks ======== */

/* Meant to define llhttptype with better meaning */
enum class ParserType {
    BOTH = 0,
    REQUEST = 1,
    RESPONSE = 2
};

enum class CallbackSettings {
    NONE = 0,
    CALLBACK_METHOD_MAP(MAP_METHODS)
    /* Enable All Callbacks */
    ALL = 16383
};


enum class DataCallbackSettings {
    NONE = 0,
    DATACALLBACK_METHOD_MAP(MAP_METHODS)
    /* Enable All Data Callbacks */
    ALL = 511
};

#ifdef __cpp_constexpr
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif


/* Flag Operators incase needed or required */

CONSTEXPR inline DataCallbackSettings operator&(DataCallbackSettings a , DataCallbackSettings b){
    return static_cast<DataCallbackSettings>(static_cast<int>(a) & static_cast<int>(b));
}

CONSTEXPR inline CallbackSettings operator&(CallbackSettings a, CallbackSettings b){
    return static_cast<CallbackSettings>(static_cast<int>(a) & static_cast<int>(b));
}

CONSTEXPR inline DataCallbackSettings operator|(DataCallbackSettings a, DataCallbackSettings b){
    return static_cast<DataCallbackSettings>(static_cast<int>(a) | static_cast<int>(b));
}

CONSTEXPR inline CallbackSettings operator|(CallbackSettings a, CallbackSettings b){
    return static_cast<CallbackSettings>(static_cast<int>(a) | static_cast<int>(b));
}

CONSTEXPR inline DataCallbackSettings operator|=(DataCallbackSettings a, DataCallbackSettings b){
    return a = a | b; 
} 

CONSTEXPR inline CallbackSettings operator|=(CallbackSettings a, CallbackSettings b){
    return a = a | b; 
}


/* Used to help with defining http methods over 
* from llhttp to here for the sake of readability 
* using an enumclass */


enum class Method {
    HTTP_METHOD_MAP(MAP_METHODS)
};

enum class Status {
    HTTP_STATUS_MAP(MAP_METHODS)
};

#undef MAP_METHODS



/* used write your own http parser with */
class Parser {

protected:

    /* Left these as protected incase nessesary or when lower-level 
     * stuff with llhttp by any developer is required */

    llhttp_t _handle;
    llhttp_settings_t _settings;

public:
    static constexpr auto SUCCESS = 0;
    static constexpr auto FAIL = -1;

    Parser(){};

    /// @brief initalizes `llhttp_settings_t` and `llhttp_t` 
    /// @param cb all the callback settings that do not carry data
    /// @param data_cb all the callbacks settings that do carry data
    /// @param type Client (response), Server (request) or Both (default)
    void setup(CallbackSettings cb, DataCallbackSettings data_cb, ParserType type = ParserType::BOTH);


    /* ====== CALLBACKS ===================================================== */

    int on_message_begin(){return SUCCESS;}
    int on_headers_complete(){return SUCCESS;}
    int on_message_complete(){return SUCCESS;}
    int on_url_complete(){return SUCCESS;}
    int on_status_complete(){return SUCCESS;}
    int on_method_complete(){return SUCCESS;}
    int on_version_complete(){return SUCCESS;}
    int on_header_field_complete(){return SUCCESS;}
    int on_header_value_complete(){return SUCCESS;}
    int on_chunk_extension_name_complete(){return SUCCESS;}
    int on_chunk_extension_value_complete(){return SUCCESS;}
    int on_chunk_header(){return SUCCESS;}
    int on_chunk_complete(){return SUCCESS;}
    int on_reset(){return SUCCESS;}


    /* ====== DATA CALLBACKS ================================================ */
  
    int on_body(string str){return SUCCESS;}
    int on_url(string str){return SUCCESS;}
    int on_status(string str){return SUCCESS;}
    int on_method(string str){return SUCCESS;}
    int on_version(string str){return SUCCESS;}
    int on_header_field(string str){return SUCCESS;}
    int on_header_value(string str){return SUCCESS;}
    int on_chunk_extension_name(string str){return SUCCESS;}
    int on_chunk_extension_value(string str){return SUCCESS;}


    /* ========= Response Reading ============================================= */

    llhttp_errno execute(const char* data, size_t data_len){
        return llhttp_execute(&_handle, data, data_len);
    }

    llhttp_errno execute(string data, size_t data_len){
        return llhttp_execute(&_handle, data.data(), data_len);
    }

    llhttp_errno execute(const char* data) {
        return llhttp_execute(&_handle, data, strlen(data));
    }
    
    llhttp_errno execute(string data){
        return llhttp_execute(&_handle, data.data(), data.length());
    }

    /* TODO: Maybe Copy the original llhttp documentation and put it over here? */

    void resume_after_upgrade(){
        llhttp_resume_after_upgrade(&_handle);
    }

    void resume(){
        llhttp_resume(&_handle);
    }

    void pause(){
        llhttp_pause(&_handle);
    }

    bool should_keep_alive(){
        return llhttp_should_keep_alive(&_handle) ? true : false;
    }

    uint8_t get_type(){
        return llhttp_get_type(&_handle);
    }

    uint8_t get_http_minor(){
        return llhttp_get_http_minor(&_handle);
    }

    uint8_t get_http_major(){
        return llhttp_get_http_major(&_handle);
    }

    const char* get_error_name(llhttp_errno err){
        return llhttp_errno_name(err);
    }

    void set_error_reason(const char* reason){
        return llhttp_set_error_reason(&_handle, reason);
    }

    const char* get_error_reason(){
        return llhttp_get_error_reason(&_handle);
    }

    const char* get_error_pos(){
        return llhttp_get_error_pos(&_handle);
    };

    const char* status_name(llhttp_status_t status){
        return llhttp_status_name(status);
    }

    const char* status_name(Status status){
        return llhttp_status_name(static_cast<llhttp_status>(status));
    }

    /* I didn't feel like spending my time making these so this was a reasonable solution to me */

    #define SET_THING(NAME) void set_##NAME(bool enable) {llhttp_set_##NAME(&_handle, enable ? 1 : 0);}
    
    SET_THING(lenient_headers)
    SET_THING(lenient_chunked_length)
    SET_THING(lenient_keep_alive)
    SET_THING(lenient_transfer_encoding)
    SET_THING(lenient_version)
    SET_THING(lenient_data_after_close)
    SET_THING(lenient_optional_lf_after_cr)
    SET_THING(lenient_optional_cr_before_lf)
    SET_THING(lenient_optional_crlf_after_chunk)
    SET_THING(lenient_spaces_after_chunk_size)
    
    #undef SET_THING



};


} /* namespace llhttp */





#endif // llHTTPCPP_H