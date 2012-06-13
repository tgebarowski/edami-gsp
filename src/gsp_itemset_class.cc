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
  unsigned size = itemset_.size();
  std::stringstream strStream;
  for(std::set<std::string>::const_iterator it = itemset_.begin(); it != itemset_.end(); ++it)
  {
    strStream<<*it;
    if (size-- > 1)
      strStream<<",";
  }

  return strStream.str();
}

/* Documented in header */
bool GspItemset::operator==(const GspItemset &right) const
{
  if (this->itemset_.size() != right.itemset_.size())
    return false;

  std::set<std::string>::const_iterator
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
