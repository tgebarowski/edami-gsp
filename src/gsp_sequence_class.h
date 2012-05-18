/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_sequence_class.h
 *
 * @brief Sequence datatype class declaration
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:45:45 2012
 */

#ifndef __GSP_SEQUENCE_H__
#define __GSP_SEQUENCE_H__

#include <list>

using namespace std;

/* Class predefinitions */
class GspItemset;

/**
 * @class GspSequence
 *
 * @brief Class defining a sequence of itemsets
 */
class GspSequence
{
  public:

    /**
     * @brief Constructs Sequence object
     */
    GspSequence();

    /**
     * @brief Destroys Sequence object
     */
    ~GspSequence();

    /**
     * @brief Add itemset to the list of itemsets_
     *
     * @param[in] itemset Added itemset, to which the method
     *            owns the pointer
     */
    inline void add_itemset(GspItemset *itemset)
    {
      itemsets_.push_back(itemset);
    }
    

  private:

    list<GspItemset *> itemsets_; /**< List of itemsets */
};


#endif /* __GSP_SEQUENCE_H__ */
