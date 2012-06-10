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
    timestamp_(timestamp), itemset_(), iter(itemset_.begin())
{
}

/* Documented in header */
GspItemset::GspItemset(const GspItemset &src)
  : timestamp_(src.timestamp_), itemset_(src.itemset_), iter(itemset_.begin())
{

}


/* Documented in header */
GspItemset::~GspItemset()
{
}

//TODO stringstream
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
