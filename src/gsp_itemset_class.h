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

#include <vector>
#include <string>
#include "gsp_common.h"

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
     * @brief Copy constructor
     *
     * @param[in] src Source object
     */
    GspItemset(const GspItemset &src);

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
    
    /**
     * @brief Remove first item in this itemset 
     */
    inline void remove_first_item()
    {
      itemset_.erase(itemset_.begin(), itemset_.begin()+1);
    }
    
    /**
     * @brief Remove last item in this itemset 
     */
    inline void remove_last_item()
    {
      itemset_.pop_back();
    }

    /**
     * @brief Get item count
     *
     */
    inline int item_count()
    {
      return itemset_.size();
    }

    /**
     * @brief Get Nth item from the itemset
     *
     * @param[in] n Index
     */
    string item_by_index(size_t n)
    {
      if (n < itemset_.size())
        return itemset_[n];
      return EMPTY_SET;
    }
    

  private:
    vector<string> itemset_; /**< List of string itemset */
};


#endif /* __GSP_ITEMSET_H__ */
