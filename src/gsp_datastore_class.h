/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_datastore_class.h
 *
 * @brief DataStore representation
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Thu Jun 07 18:08:22 2012
 */

#ifndef __GSP_DATASTORE_H__
#define __GSP_DATASTORE_H__

#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include "gsp_common.h"

using namespace std;

/* Predefined classes */
class GspItemset;

/**
 * @class GspDataStore
 *
 * @brief Class Data Store object used for iterating through data file
 *
 * Note: Input file should be formatted according to the following pattern
 *
 * ID # Timestamp # Itemset
 *
 * 1  # 123456789 # item1,item2
 * 2  # 123456789 # item3,item2
 * 3  # 123456789 # item1
 * 4  # 123456789 # item3
 * 5  # 123456789 # item2
 * 6  # 123456789 # item1
 *
 * Fields are separated by '#' symbol, while corresponding items with coma ','
 */
class GspDataStore
{
  public:

    /**
     * @brief Constructs DataStore
     *
     * @param[in] file_path Pointer to file with data
     */
    GspDataStore(string file_path);

    /**
     * @brief Destroys Itemset object
     */
    ~GspDataStore();

    /**
     * @brief Get next Itemset from DataStore
     *
     * @return Newly allocated @ref GspItemset to be freed by method caller
     *         NULL is returned when no more itemsets
     */
    GspItemset * GetNextItemset();
    

  private:
    ifstream input_file_; /**< Object representing opened file */
    istream_iterator<string> line_iterator_; /**< Current line iterator */
};


#endif /* __GSP_DATASTORE_H__ */
