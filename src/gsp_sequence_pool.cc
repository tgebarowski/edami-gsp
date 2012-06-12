/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_sequence_pool.cc
 *
 * @brief GSP Sequence Pool
 *
 * @author: Adam Szczepankiewicz <adam.szczepankiewicz@cern.ch>
 * @date: Sat Jun 9 15:30:39 2012
 */

#include <map>
#include <set>
#include <string>

#include "gsp_sequence_pool.h"
#include "gsp_sequence_reader.h"
#include "gsp_hashtree_class.h"
#include "gsp_algorithm.h"

/* Documented in header */
GspSequencePool::GspSequencePool(GspAlgorithm *parent)
  : parent_(parent),
    k_(0)
{
}

/* Documented in header */
GspSequencePool::GspSequencePool(GspSequenceReader *reader,
                                 GspAlgorithm *parent)
  : parent_(parent),
    k_(1)
{
  GspSequence *seq;
  std::map<std::string, unsigned> cand1;
  std::set<std::string> cand1Sequence;

  while((seq = reader->GetNextSequence()))
  {
    cand1Sequence.clear();
    seq->rewind();
    GspItemset *itemset = seq->currentItemset();
    while(itemset)
    {
      cand1Sequence.insert(itemset->begin(), itemset->end());
      seq->nextItemset();
      itemset = seq->currentItemset();
    }

    for (std::set<std::string>::const_iterator seqIt = cand1Sequence.begin();
         seqIt!= cand1Sequence.end(); ++seqIt)
    {
      std::map<std::string, unsigned>::iterator mapIter = cand1.find(*seqIt);
      if(mapIter != cand1.end())
        ++(mapIter->second);
      else
        cand1[*seqIt] = 1;
    }

    delete seq;
  }

  for(std::map<std::string, unsigned>::iterator mapIter = cand1.begin();
      mapIter != cand1.end(); ++mapIter)
  {
    if (mapIter->second > parent_->min_support())
    {
      seq = new GspSequence("");
      GspItemset *itemset = new GspItemset();
      itemset->add_item(mapIter->first);
      seq->add_itemset(itemset);
      seq->setSupport(mapIter->second);
      sequences_.push_back(seq);
    }
  }
}

/* Documented in header */
GspSequencePool *GspSequencePool::Join()
{
  GspSequencePool *ret = new GspSequencePool(parent_);
  GspSequence *newSeq;
  GspItemset *newItemset;
  GspSequence *tempSeqL, *tempSeqR;
  GspItemset *tempItemsetL, *tempItemsetR;
  ret->k_ = k_ + 1;

  if (k_ == 1)
  {
    std::list<GspSequence *>::const_iterator it = sequences_.begin();
    std::list<GspSequence *>::const_iterator jt = it++;
    for (; jt != sequences_.end();)
    {
      //(L)(L)
      tempSeqL = *jt;
      tempSeqL->rewind();
      tempItemsetL = tempSeqL->currentItemset();
      tempItemsetL->rewind();
      std::string itemLeft = tempItemsetL->currentItem();
      newSeq = new GspSequence("");
      newItemset = new GspItemset(0);
      newItemset->add_item(itemLeft);
      newSeq->add_itemset(newItemset);
      newItemset = new GspItemset(0);
      newItemset->add_item(itemLeft);
      newSeq->add_itemset(newItemset);
      ret->sequences_.push_back(newSeq);

      for (; it != sequences_.end(); ++it)
      {
        tempSeqR = *it;
        tempSeqR->rewind();
        tempItemsetR = tempSeqR->currentItemset();
        tempItemsetR->rewind();
        std::string itemRight = tempItemsetR->currentItem();
        //(L)(R)
        newSeq = new GspSequence("");
        newItemset = new GspItemset(0);
        newItemset->add_item(itemLeft);
        newSeq->add_itemset(newItemset);
        newItemset = new GspItemset(0);
        newItemset->add_item(itemRight);
        newSeq->add_itemset(newItemset);
        ret->sequences_.push_back(newSeq);

        //(R)(L)
        newSeq = new GspSequence("");
        newItemset = new GspItemset(0);
        newItemset->add_item(itemRight);
        newSeq->add_itemset(newItemset);
        newItemset = new GspItemset(0);
        newItemset->add_item(itemLeft);
        newSeq->add_itemset(newItemset);
        ret->sequences_.push_back(newSeq);

        //(LR) ordered
        newSeq = new GspSequence("");
        newItemset = new GspItemset(0);
        newItemset->add_item(itemRight);
        newItemset->add_item(itemLeft);
        newSeq->add_itemset(newItemset);
        ret->sequences_.push_back(newSeq);
      }
      it = ++jt;
      jt = it++;
    }
  }
  else
  {
    GspSequence *candidate;
    for(std::list<GspSequence *>::const_iterator it = sequences_.begin();
        it != sequences_.end();
        ++it)
    {
      for(std::list<GspSequence *>::const_iterator jt = sequences_.begin();
          jt != sequences_.end();
          ++jt)
      {
        candidate = (*it)->joinSequences(*jt);
        if (candidate)
          ret->sequences_.push_back(candidate);
      }
    }
  }

  return ret;
}

/* Documented in header */
void GspSequencePool::PrintSequences()
{
  for(std::list<GspSequence *>::const_iterator it = sequences_.begin();
      it != sequences_.end();
      ++it)
  {
    std::cout<<(*it)->ToString()<<" "<<(*it)->getSupport()<<"\n";
  }
}

/* Documented in header */
void GspSequencePool::CountSupport(GspSequenceReader *reader)
{
  //TODO cos madrzejszego niz 4:)
  GspHashTree tree(k_, 2);

  for (std::list<GspSequence *>::const_iterator it = sequences_.begin();
       it != sequences_.end();
       ++it)
  {
    tree.AddSequence(*it);
  }
  std::cout<<std::endl;

  GspSequence *seqTemp;

  tree.PrintTree();

  //TODO sprawdzac returna
  reader->RewindStream();
  while((seqTemp = reader->GetNextSequence()))
  {
    tree.CheckClientSequence(seqTemp,
                             parent_);

    std::cout<<"Client sequence: "<<seqTemp->ToString()<<" :"<<std::endl;
    seqTemp->printCandidates();
    //TODO add calculating support for candidates
    delete seqTemp;
  }
}

/* Documented in header */
GspSequencePool::~GspSequencePool()
{
  for(std::list<GspSequence *>::iterator it = sequences_.begin();
      it != sequences_.end();
      ++it)
  {
    delete *it;
  }
}

