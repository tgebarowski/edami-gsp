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

//#include <vector>
#include <set>
#include <string>
#include "gsp_common.h"

//using namespace std;

/**
 * @class GspItemset
 *
 * @brief Class representing set of items aka itemset
 */
class GspItemset
{
  public:
    typedef std::set<std::string>::iterator IterType;

    /**
     * @brief Constructs Itemset object
     *
     * @param[in] id String representing ID of user generating this itemset
     * @param[in] timestamp Timestamp
     *
     */
    GspItemset(//string id = "",
               int timestamp = 0);

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
     * @brief Get Nth item from the itemset
     *
     * @param[in] n Index
     */
    /*
    string item_by_index(size_t n)
    {
      if (n < itemset_.size())
        return itemset_[n];
      return EMPTY_SET;
    }
    */

    /**
     * @brief Get ID
     */
    /*
    inline string get_id()
    {
      return id_;
    }
    */

    /**
     * @brief Get timestamp
     */
    inline int get_timestamp()
    {
      return timestamp_;
    }
    
    void rewind()
    {
      iter_ = itemset_.begin();
    }

    bool next_item()
    {
      ++iter_;
      if (iter_ == itemset_.end())
        return false;

      return true;
    }

    std::string current_item()
    {
      if (iter_ == itemset_.end())
        return "";

      return *iter_;
    }

    IterType begin()
    {
      return itemset_.begin();
    }

    IterType end()
    {
      return itemset_.end();
    }

    bool operator==(const GspItemset &right) const;

  private:
//    string id_; /**< Identifier of User generating this sequence  */
    int timestamp_; /**< Timestamp */
    //vector<string> itemset_; /**< List of string itemset */
    std::set<std::string> itemset_;
    std::set<std::string>::const_iterator iter_;
};


#endif /* __GSP_ITEMSET_H__ */
