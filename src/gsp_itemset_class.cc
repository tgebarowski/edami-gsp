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

/* Documented in header */
GspItemset::GspItemset()
{
}

/* Documented in header */
GspItemset::~GspItemset()
{
}

/* Documented in header */
string GspItemset::ToString() const
{
  string out = "";
  for (vector<string>::const_iterator it = itemset_.begin();
       it != itemset_.end();
       ++it)
  {
    out += (string)*it + ((distance(it, itemset_.end()) != 1) ? "," : "");
  }
  return out;  
}
