#ifndef _ERROR_H_
#define _ERROR_H_

#include <string>

#define ERR_INVALID_IP -1
#define ERR_CONNECTION -2
#define ERR_UNKNOWN_IP -3
#define ERR_DUPLICATE_BAN -4
#define ERR_NO_BAN -5
#define ERR_NOT_LOGGED_IN -6
#define ERR_INVALID_ARGS -7

std::string err_to_str(int errcode) {
    switch (errcode) {
    case ERR_INVALID_IP:
        return "InvalidIPError";
    case ERR_CONNECTION:
        return "ConnectionError";
    case ERR_DUPLICATE_BAN:
        return "UnknownIPError";
    case ERR_NO_BAN:
        return "NoBanError";
    case ERR_INVALID_ARGS:
        return "InvalidArgumentsError";
    default:
        return "UnknownError";
    }
}

bool is_err(int errcode) { return errcode < 0 ? true : false; }

#endif