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
GspSequence::GspSequence(const GspSequence &src)
{
  for (list<GspItemset *>::const_iterator it = src.itemsets_.begin();
       it != src.itemsets_.end();
       ++it)
  {
    itemsets_.push_back(new GspItemset(**it));
  }
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
bool GspSequence::CompareWithSubsequence(GspSequence &s)
{
  GspSequence s1(*this);
  GspSequence s2(s);

  GspItemset *s1_itemset_first = s1.get_first_itemset();
  GspItemset *s2_itemset_last = s2.get_last_itemset();
  
  /* Remove first item from first sequence */
  if (s1_itemset_first != NULL)
  {
    s1_itemset_first->remove_first_item();
  }
  /* Remove last item from last sequence */
  if (s2_itemset_last != NULL)
  {
    s2_itemset_last->remove_last_item();
  }
  return (s1 == s2);
}

/* Documented in header */
void GspSequence::AppendItemset(GspItemset *itemset)
{
  if (itemset != NULL)
  {
    /** Append to existing last itemset */
    if (itemset->item_count() == 1)
    {
      GspItemset *last_itemset = get_last_itemset();
      if (last_itemset != NULL)
      {
        last_itemset->add_item(itemset->item_by_index(0));
      }
    }
    /** Append as separate itemset */
    else if (itemset->item_count() > 1)
    {
      itemsets_.push_back(itemset);
    }
  }
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

/* Operators */

/* Documented in header */
bool GspSequence::operator==(const GspSequence &other) const
{
  return (this->ToString() == other.ToString());
}
