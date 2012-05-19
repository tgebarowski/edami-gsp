/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_itemset_class.h
 *
 * @brief Itemset datatype class declaration
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:45:45 2012
 */

#ifndef __GSP_ITEMSET_H__
#define __GSP_ITEMSET_H__

#include <list>
#include <string>

using namespace std;

/**
 * @class GspItemset
 *
 * @brief Class representing set of items aka itemset
 */
class GspItemset
{
  public:

    /**
     * @brief Constructs Itemset object
     */
    GspItemset();

    /**
     * @brief Destroys Itemset object
     */
    ~GspItemset();

    /**
     * @brief Get string representation of itemset
     */
    string ToString() const;

    /**
     * @brief Add item by appending it to the end of itemset
     *
     * @param[in] item String representing appended item
     */
    inline void add_item(string item) 
    {
      itemset_.push_back(item);
    }

  private:
    list<string> itemset_; /**< List of string itemset */
};


#endif /* __GSP_ITEMSET_H__ */
