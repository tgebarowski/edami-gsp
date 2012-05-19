/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_sequence_class.cc
 *
 * @brief Sequence datatype class definition
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:46:11 2012
 */

#include "gsp_sequence_class.h"
#include "gsp_itemset_class.h"
#include <cmath>

/* Documented in header */
GspSequence::GspSequence()
{
}

/* Documented in header */
GspSequence::~GspSequence()
{
  /* Remove all itemsets added to this sequence */
  for (list<GspItemset *>::iterator it = itemsets_.begin();
       it != itemsets_.end();
       ++it)
  {
    delete *it;
  }
}

/* Documented in header */
string GspSequence::GetItemByIndex(int n)
{
  int max_current_count = 0; /**< Maximum count achievable while
                                  processing current itemset */
  int max_next_count = 0; /**< Maximum count achievable while
                               processing next itemset */
  for (list<GspItemset *>::const_iterator it = itemsets_.begin();
       it != itemsets_.end();
       ++it)
  {
    max_next_count = max_current_count + (*it)->item_count();
    if (n >= max_current_count && n < max_next_count)
    {
      return (*it)->item_by_index(abs(max_current_count - n));
    }
    max_current_count = max_next_count;
  }
  return EMPTY_SET;
}


/* Documented in header */
string GspSequence::ToString() const
{
  string out = "";
  for (list<GspItemset *>::const_iterator it = itemsets_.begin();
       it != itemsets_.end();
       ++it)
  {
    out += "(" + (*it)->ToString() + ")" +\
           ((std::distance(it,itemsets_.end()) != 1) ? "," : "");
  }
  return out;
}

