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

#include <set>
#include <string>

#include "gsp_common.h"

/**
 * @class GspItemset
 *
 * @brief Class representing set of items aka itemset. The itemset is identified by
 * its timestamp. It provides an internal iteration mechanism as well as methods
 * that return iterators to the first and last element of the set.
 */
class GspItemset
{
  public:
    /**
     * @class IterType
     *
     * @brief typedef for the iterator type used for iteration over items
     */
    typedef std::set<std::string>::iterator IterType;

    /**
     * @brief Constructs Itemset object
     *
     * @param[in] timestamp Timestamp
     *
     */
    GspItemset(int timestamp = 0);

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
    std::string ToString() const;

    /**
     * @brief Get string representation of itemset without brackets
     */
    std::string ToStringNoBrac();

    /**
      * @brief Get string representation of itemset without brackets and last item
      */
    std::string ToStringNoBracNoLast();

    /**
     * @brief Add item by appending it to the end of itemset
     *
     * @param[in] item String representing appended item
     */
    inline void add_item(std::string item)
    {
      itemset_.insert(item);
    }
    
    /**
     * @brief Remove first item in this itemset 
     */
    inline void remove_first_item()
    {
      itemset_.erase(itemset_.begin());
    }
    
    /**
     * @brief Remove last item in this itemset 
     */
    inline void remove_last_item()
    {
      itemset_.erase(--itemset_.end());
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
     * @brief Get timestamp
     */
    inline int get_timestamp()
    {
      return timestamp_;
    }
    
    /**
     * @brief Set the internal iterator to the first element
     */
    void rewind()
    {
      iter_ = itemset_.begin();
    }

    /**
     * @brief Move the internal iterator to the next element
     *
     * @return Boolean value telling if the next item existed
     */
    bool next_item()
    {
      ++iter_;
      if (iter_ == itemset_.end())
        return false;

      return true;
    }

    /**
     * @brief Get an item pointed by the internal iterator
     *
     * @return Element pointed by internal iterator
     */
    std::string current_item()
    {
      if (iter_ == itemset_.end())
        return "";

      return *iter_;
    }

    /**
     * @brief Get the iterator to the first item
     *
     * @return Iterator to the first item
     */
    IterType begin()
    {
      return itemset_.begin();
    }

    /**
     * @brief Get the iterator to the last item
     *
     * @return Iterator to the last item
     */
    IterType end()
    {
      return itemset_.end();
    }

    /**
     * @brief Compare two itemsets
     *
     * @param[in] right the itemset to compare with.
     *
     * @return Iterator to the last item
     */
    bool operator==(const GspItemset &right) const;

  private:
    int timestamp_; /**< Timestamp */
    std::set<std::string> itemset_;
    std::set<std::string>::const_iterator iter_;
};


#endif /* __GSP_ITEMSET_H__ */
