/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_common.h
 *
 * @brief Common type definitions used within project
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:13:25 2012
 */

#ifndef __GSP_COMMON_H__
#define __GSP_COMMON_H__

#include <string>
#include <sstream>
#include <iostream>
using namespace std;

/**
 * @brief Useful macro defining empty set
 */
#define EMPTY_SET ""

/*
 * @brief Enumeration representing possible values returned by functions
 */
typedef enum 
{
  GSP_OK,        /**< OK */
  GSP_EINVAL,    /**< Invalid value/pointer */
  GSP_EEXIST,    /**< Value already exists */
  GSP_ENOTFOUND, /**< Value not found */
  GSP_ERROR      /**< Generic error */
} gsp_status_t;

/**
 * @brief Conversion to string
 */
template < class T >
inline string ToString(const T &arg)
{
    ostringstream	out(std::ios_base::out);
    out << arg;
    return(out.str());
}

/**
 * @brief Conversion from string to integer
 */
inline int ToInt(const string &arg)
{
    int i = 0;
    stringstream iss(arg);
    iss >> i;
    return i;
}

/**
 * @brief Conversion from string to integer
 */
inline bool ToBool(const string &arg)
{
    if (arg == "yes" || arg == "YES" || arg == "On" || arg == "on")
        return true;
    return false;
}

/**
 * @brief Conversion from string to double
 */
inline double ToDouble(const string &arg)
{
    double d = 0;
    stringstream iss(arg);
    iss >> d;
    return d;
}

/**
 * @brief Conversion from int to string (hex)
 */
inline string ToHex(const unsigned int value)
{
    ostringstream out;

    out << std::hex << value;
    return (out.str());

}

#endif /* __GSP_COMMON_H__ */
