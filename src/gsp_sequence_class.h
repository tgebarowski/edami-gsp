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
#include <string>

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
    GspSequence(const string &id_);

    /**
     * @brief Copy constructor
     *
     * @param[in] src source object
     */
    GspSequence(const GspSequence &src);

    /**
     * @brief Destroys Sequence object
     */
    ~GspSequence();

    /**
     * @brief Get N-th item from the sequence
     *
     * @param[in] n Index representing N-th item from sequence
     *
     * @return string representing N-th item content
     */
    string GetItemByIndex(int n);

    /**
     * @brief Compare this GspSequence (s1) with another provided as argument (s2)
     *        If a subsequence s1' obtained from s1 by droping first item
     *        is equal to a subsequence s2' obtained from s2 by droping last item
     *        then the method return TRUE.
     *        Otherwise it returns always false
     *
     * @param[in] s2 Second subsequence used for comparision
     *
     * @return True if above condition is satisfied, otherwise false
     */
    bool CompareWithSubsequence(GspSequence &s2);

    /**
     * @brief Join GspSequence represented by this object (s1)
     *        with an GspItemset provided in the argument
     *        If GspItemset is a separate element then this element
     *        will constitute to the last itemset of s1.
     *        Otherwise it will be appended as a separate itemset to s1
     *
     * @param[in] itemset GspItemset to be appended, owned by GspSequence
     */
    void AppendItemset(GspItemset *itemset);

    /**
     * @brief Get string representation of sequence
     */
    string ToString() const;


    /* Operators */

    /**
     * @brief Equal operator
     */
    bool operator==(const GspSequence &other) const;

    /* Inline members */

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



    /**
     * @brief Get last Itemset from GspSequence
     */
    inline GspItemset * get_first_itemset()
    {
      return itemsets_.front();
    }

    /**
     * @brief Get last Itemset from GspSequence
     */
    inline GspItemset * get_last_itemset()
    {
      return itemsets_.back();
    }

    void rewind()
    {
      iter = itemsets_.begin();
    }

    GspItemset *nextItemset()
    {
      if (iter == itemsets_.end())
         return NULL;

      return *iter++;
    }

    void increaseSupport()
    {
      ++support;
    }

    unsigned getSupport()
    {
      return support;
    }

    void setSupport(unsigned support_)
    {
      support = support_;
    }

  private:

    list<GspItemset *> itemsets_; /**< List of itemsets */
    list<GspItemset *>::iterator iter;
    string id;
    unsigned support;

};


#endif /* __GSP_SEQUENCE_H__ */
