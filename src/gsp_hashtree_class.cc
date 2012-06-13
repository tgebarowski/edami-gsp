/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_hashtree_class.cc
 *
 * @brief Hash Tree datatype class definition
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:14:38 2012
 */
//#include <stdio.h>

#include "gsp_hashtree_class.h"
#include "gsp_algorithm.h"


enum GSP_STATUS GspHashTree::AddSequence(GspSequence *sequence)
{
//  std::cout << "Add To Tree: " + sequence->ToString() << std::endl;

  sequence->rewind_all();
  root_->SetSequence(sequence);

  return GSP_OK;
}

/* Documented in header */
GspHashTree::GspHashTree(int max_level,
                         int max_leaf)
  : root_(NULL),
    max_level_(max_level),
    max_leaf_(max_leaf)
{
  root_ = new Node(max_leaf_, max_level_);
}

/* Documented in header */
GspHashTree::~GspHashTree()
{
  delete root_;
}

/* Documented in header */
int GspHashTree::Hash(const std::string &item, int upper_bound)
{
  int stringSum = 0;
  int stringSize = item.size();

  for(int it = 0; it < stringSize; ++it)
    stringSum += item[it];

  return stringSum % upper_bound;
}

/* Documented in header */
void GspHashTree::CheckClientSequence(GspSequence *seq,
                                      GspAlgorithm *parent)
{
  GspSequence::IterType itemSetIt = seq->begin();
  GspItemset::IterType itemIt = (*itemSetIt)->begin();
  root_->CheckClientSequence(seq,
                             *itemSetIt,
                             itemSetIt,
                             itemIt,
                             parent);
}

/* Documented in header */
void GspHashTree::Node::CheckClientSequence(GspSequence *seq,
                                            GspItemset *itemSet,
                                            GspSequence::IterType itemSetIter,
                                            GspItemset::IterType itemIter,
                                            GspAlgorithm *parent)
{
  if (type_ == NODE_LEAF)
  {
    for(std::list<GspSequence *>::iterator it = sequences_.begin();
        it != sequences_.end();
        ++it)
    {
      seq->add_candidate_sequence(*it);
    }
  }
  else
  {
    if(level_ == 0)
    {
      for (itemSetIter = seq->begin();
           itemSetIter != seq->end();
           ++itemSetIter)
      {
        itemSet = *itemSetIter;
        for (itemIter = itemSet->begin();
             itemIter != itemSet->end();
             ++itemIter)
        {
          nodes_[Hash(*itemIter, MAX_NODES)]->CheckClientSequence(seq,
                                                                  itemSet,
                                                                  itemSetIter,
                                                                  itemIter,
                                                                  parent);
        }
      }
    }
    else
    {
      bool maxGapPresent = (parent->max_gap() != 0);
      int minTime = itemSet->get_timestamp() - parent->window_size();
      int maxTime;
      if (maxGapPresent)
      {
        maxTime = itemSet->get_timestamp() +\
          (parent->max_gap() > parent->window_size() ? parent->max_gap() : parent->window_size());
      }

      //TODO check?!
      GspItemset::IterType backItemIterCopy(itemIter);
      GspSequence::IterType backItemsetIterCopy(itemSetIter);
      GspItemset *backItemsetCopy = itemSet;
      do
      {
        for(GspItemset::IterType it = backItemsetCopy->begin();
            it != backItemIterCopy; ++it)
        {
          nodes_[Hash(*it, MAX_NODES)]->CheckClientSequence(seq,
                                                            backItemsetCopy,
                                                            backItemsetIterCopy,
                                                            it, parent);
        }
        if(backItemsetIterCopy-- == seq->begin())
          break;
        backItemsetCopy = *backItemsetIterCopy;
        if (backItemsetCopy->get_timestamp() < minTime)
          break;
        backItemIterCopy = backItemsetCopy->end();
      } while(1);

      //TODO check?!
      GspItemset::IterType forwardItemIterCopy(itemIter);
      GspSequence::IterType forwardItemsetIterCopy(itemSetIter);
      ++forwardItemIterCopy;
      GspItemset *forwardItemsetCopy = itemSet;
      GspSequence::IterType it = forwardItemsetIterCopy;
      while(1)
      {
        for(; forwardItemIterCopy != forwardItemsetCopy->end(); ++forwardItemIterCopy)
        {
          nodes_[Hash(*forwardItemIterCopy,
                      MAX_NODES)]->CheckClientSequence(seq,
                                                       forwardItemsetCopy,
                                                       it, forwardItemIterCopy,
                                                       parent);
        }

        if (++it == seq->end())
          break;

        forwardItemsetCopy = *it;

        if (maxGapPresent && forwardItemsetCopy->get_timestamp() > maxTime)
        {
          break;
        }
        forwardItemIterCopy = forwardItemsetCopy->begin();
      }
    }
  }
}

