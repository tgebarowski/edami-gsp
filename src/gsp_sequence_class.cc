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
#include <sstream>

/* Documented in header */
GspSequence::GspSequence(const string &id_) : id(id_), support(0)
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
/*
 * TODO needed?
 */
//string GspSequence::GetItemByIndex(int n)
//{
//  int max_current_count = 0; /**< Maximum count achievable while
//                                  processing current itemset */
//  int max_next_count = 0; /**< Maximum count achievable while
//                               processing next itemset */
//  for (list<GspItemset *>::const_iterator it = itemsets_.begin();
//       it != itemsets_.end();
//       ++it)
//  {
//    max_next_count = max_current_count + (*it)->item_count();
//    if (n >= max_current_count && n < max_next_count)
//    {
//      return (*it)->item_by_index(abs(max_current_count - n));
//    }
//    max_current_count = max_next_count;
//  }
//  return EMPTY_SET;
//}


/* Documented in header */
/*
 * TODO needed?
 */
//bool GspSequence::CompareWithSubsequence(GspSequence &s)
//{
//  GspSequence s1(*this);
//  GspSequence s2(s);
//
//  GspItemset *s1_itemset_first = s1.get_first_itemset();
//  GspItemset *s2_itemset_last = s2.get_last_itemset();
//
//  /* Remove first item from first sequence */
//  if (s1_itemset_first != NULL)
//  {
//    s1_itemset_first->remove_first_item();
//  }
//  /* Remove last item from last sequence */
//  if (s2_itemset_last != NULL)
//  {
//    s2_itemset_last->remove_last_item();
//  }
//  return (s1 == s2);
//}

/* Documented in header */
/*
 * TODO needed?
 */
//void GspSequence::AppendItemset(GspItemset *itemset)
//{
//  if (itemset != NULL)
//  {
//    /** Append to existing last itemset */
//    if (itemset->item_count() == 1)
//    {
//      GspItemset *last_itemset = get_last_itemset();
//      if (last_itemset != NULL)
//      {
//        last_itemset->add_item(itemset->item_by_index(0));
//      }
//    }
//    /** Append as separate itemset */
//    else if (itemset->item_count() > 1)
//    {
//      itemsets_.push_back(itemset);
//    }
//  }
//}

GspSequence *GspSequence::joinSequences(GspSequence *right)
{

  GspSequence *leftCopy = new GspSequence(*this);
  GspSequence *rightCopy = new GspSequence(*right);
  GspSequence *ret = NULL;

  leftCopy->dropFirstItem();
  rightCopy->dropLastItem();

  if (*leftCopy == *rightCopy)
  {
    GspSequence *ret = new GspSequence(*this);
    ret->appendSequence(right);
  }

  delete leftCopy;
  delete rightCopy;

  return ret;
}

void GspSequence::dropFirstItem()
{
  if (itemsets_.size() > 0)
  {
    GspItemset *firstItemSet = *itemsets_.begin();
    firstItemSet->remove_first_item();
  }
}

void GspSequence::dropLastItem()
{
  if (itemsets_.size() > 0)
  {
    GspItemset *lastItemSet = *(--itemsets_.end());
    lastItemSet->remove_last_item();
  }
}

/* Documented in header */
string GspSequence::ToString() const
{
  std::stringstream strStream;
  unsigned size = itemsets_.size();

  for(list<GspItemset *>::const_iterator it = itemsets_.begin(); it != itemsets_.end(); ++it)
  {
    strStream<<"(";
    strStream<<(*it)->ToString();
    strStream<<")";
    if(size-- > 1)
      strStream<<",";
  }

  return strStream.str();
}

/* Operators */

/* Documented in header */
bool GspSequence::operator==(const GspSequence &other) const
{
  if (this->itemsets_.size() != other.itemsets_.size())
    return false;

  std::list<GspItemset *>::const_iterator
    leftIt = this->itemsets_.begin(),
    rightIt = other.itemsets_.begin();

  while (leftIt != this->itemsets_.end())
  {
    if (!(**leftIt == **rightIt))
      return false;
    ++leftIt;
    ++rightIt;
  }

  return true;
}

void GspSequence::appendSequence(GspSequence *right)
{
  GspItemset *lastRight = *(--right->itemsets_.end());

  if(lastRight->item_count() == 1)
  {
    this->add_itemset(new GspItemset(*lastRight));
  }
  else
  {
    GspItemset *lastLeft = *(--this->itemsets_.end());
    lastRight->rewind();
    std::string insertItem = lastRight->next();
    lastLeft->add_item(insertItem);
  }
}
