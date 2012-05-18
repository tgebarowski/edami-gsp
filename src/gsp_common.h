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


#endif /* __GSP_COMMON_H__ */
