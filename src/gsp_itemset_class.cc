/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_itemset_class.cc
 *
 * @brief Itemset datatype class definition
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:46:11 2012
 */

#include "gsp_itemset_class.h"
#include <sstream>

/* Documented in header */
GspItemset::GspItemset(//string id,
                       int timestamp)
  : //id_(id),
    timestamp_(timestamp), itemset_(), iter_(itemset_.begin())
{
}

/* Documented in header */
GspItemset::GspItemset(const GspItemset &src)
  : timestamp_(src.timestamp_), itemset_(src.itemset_), iter_(itemset_.begin())
{

}


/* Documented in header */
GspItemset::~GspItemset()
{
}

/* Documented in header */

std::string GspItemset::ToString() const
{
  std::stringstream strStream;
  strStream<<"{ ";
  for(std::set<int>::const_iterator it = itemset_.begin(); it != itemset_.end(); ++it)
  {
    strStream<<*it<<" ";
  }
  strStream<<"}";

  return strStream.str();
}

/* Documented in header */
bool GspItemset::operator==(const GspItemset &right) const
{
  if (this->itemset_.size() != right.itemset_.size())
    return false;

  IterType
//  std::set<std::string>::const_iterator
    leftIt = this->itemset_.begin(),
    rightIt = right.itemset_.begin();

  while (leftIt != this->itemset_.end())
  {
    if (*leftIt != *rightIt)
      return false;
    ++leftIt;
    ++rightIt;
  }

  return true;
}

//A string-like comparison
bool GspItemset::operator<(const GspItemset &right) const
{
  IterType leftStart = itemset_.begin();
  IterType rightStart = right.itemset_.begin();

  while(1)
  {
    if(*leftStart < *rightStart)
      return true;
    if(*leftStart > *rightStart)
      return false;

    ++leftStart;
    ++rightStart;

    if(leftStart == itemset_.end())
    {
      //both finished and equal
      if(rightStart == right.itemset_.end())
        return false;
	  else
		return true;
    }

    if(rightStart == right.itemset_.end())
    {
      return false;
    }
  }

  return false;
}

bool GspItemset::CompareNoLast(GspItemset &right)
{
  int leftSize = item_count();
  int rightSize = right.item_count();

  if (leftSize == rightSize - 1)
  {
    IterType leftIt = begin();
    IterType rightIt = right.begin();

    while(leftIt != end())
    {
      if (*leftIt != *rightIt)
        return false;

      ++leftIt;
      ++rightIt;
    }
  }
  else
    return false;

  return true;
}
