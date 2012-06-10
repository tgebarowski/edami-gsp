/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_hashtree_class.cc
 *
 * @brief Hash Tree datatype class definition
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:14:38 2012
 */
#include <stdio.h>

#include "gsp_hashtree_class.h"
#include "gsp_sequence_class.h"
#include "gsp_common.h"

/* Documented in header */
GspHashTree::GspHashTree(int max_level,
                         int max_leaf)
  : root_(NULL),
    max_level_(max_level),
    max_leaf_(max_leaf)
{
}

/* Documented in header */
GspHashTree::~GspHashTree()
{
}

/* Documented in header */
gsp_status_t 
GspHashTree::AddSequence(GspSequence *sequence)
{
    cout << "Add: " + sequence->ToString() << endl;

  /** If HashTree is empty */
  if (root_ == NULL)
  {
    root_ = new Node(max_leaf_, max_level_);
  }
  root_->SetSequence(sequence);
  
  return GSP_OK;
}

/* Documented in header */
const GspSequence * 
GspHashTree::FindSequence(GspSequence &sequence)
{
  if (root_ != NULL)
  {
    return root_->Find(root_, &sequence);
  }
  return NULL;
}

/* Documented in header */
int GspHashTree::Hash(GspSequence *sequence, int n, int upper_bound)
{
  //  string item = sequence->GetItemByIndex(n);
  int hash = 0;

  /*
  if (item != EMPTY_SET)
  {
    hash = item[0] % upper_bound;
  }
  */
  return hash;
}



