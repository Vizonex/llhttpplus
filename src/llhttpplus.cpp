#include "llhttp.h"
#include "llhttpplus.hpp"


namespace llhttp {


/* =========== CALLBACK HANDLES ============================================================================= */

#define FORMAT_DATACALLBACK_NAME(FUNC) __data_cb__##FUNC
#define FORMAT_CALLBACK_NAME(FUNC) __cb__##FUNC

#define GET_XXHTTP_HANDLE(handle) reinterpret_cast<Parser*>(handle->data)

#define DATACALLBACK_FUNCTION(NUM, NAME, FUNC) \
    int FORMAT_DATACALLBACK_NAME(FUNC)(llhttp_t* handle, const char *at, size_t length){ \
        return GET_XXHTTP_HANDLE(handle)->##FUNC(string(at, length)); \
    }


#define CALLBACK_FUNCTION(NUM, NAME, FUNC) \
    int FORMAT_CALLBACK_NAME(FUNC)(llhttp_t* handle){ return GET_XXHTTP_HANDLE(handle)->##FUNC(); }

/* Write all Data Callbacks and Callbacks */
DATACALLBACK_METHOD_MAP(DATACALLBACK_FUNCTION)
CALLBACK_METHOD_MAP(CALLBACK_FUNCTION)



        
void Parser::setup(CallbackSettings cb, DataCallbackSettings data_cb, ParserType type){
    llhttp_settings_init(&_settings);

    #define CALLBACK_METHOD_FLAG_CHECK(NUM, NAME, FUNC) if (static_cast<bool>(cb & CallbackSettings::##NAME)){ _settings.##FUNC = FORMAT_CALLBACK_NAME(FUNC);}
    CALLBACK_METHOD_MAP(CALLBACK_METHOD_FLAG_CHECK)
    #undef CALLBACK_METHOD_FLAG_CHECK

    #define DATACALLBACK_METHOD_FLAG_CHECK(NUM, NAME, FUNC) if (static_cast<bool>(data_cb & DataCallbackSettings::##NAME)){ _settings.##FUNC = FORMAT_DATACALLBACK_NAME(FUNC);}
    DATACALLBACK_METHOD_MAP(DATACALLBACK_METHOD_FLAG_CHECK)
    #undef DATACALLBACK_METHOD_FLAG_CHECK

    llhttp_init(&_handle, static_cast<llhttp_type>(type), &_settings);
};

} // namespace xxhttp 


