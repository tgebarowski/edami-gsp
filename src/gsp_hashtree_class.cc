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

/* Documented in header */
GspHashTree::GspHashTree(int max_level)
  : max_level_(max_level)
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
  /** If HashTree is empty */
  if (root_ == NULL)
  {
    root_ = new Node();
  }
  root_->SetSequence(sequence);
  
  return GSP_OK;
}

/* Documented in header */
const GspSequence * 
GspHashTree::FindSequence(GspSequence &sequence)
{
  return NULL;
}


