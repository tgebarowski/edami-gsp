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
#include "gsp_join_tree.h"

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

  //create 1-length candidates,
//  std::cout<<"CANDIDATES:"<<std::endl;
  while((seq = reader->GetNextSequence()))
  {
//    std::cout<<seq->ToString()<<std::endl;
    cand1Sequence.clear();
    seq->rewind();
    GspItemset *itemset = seq->current_itemset();
    while(itemset)
    {
      cand1Sequence.insert(itemset->begin(), itemset->end());
      seq->next_itemset();
      itemset = seq->current_itemset();
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
//    std::cout<<seq->ToString()<<"\n";
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
      seq->set_support(mapIter->second);
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
      tempItemsetL = tempSeqL->current_itemset();
      tempItemsetL->rewind();
      std::string itemLeft = tempItemsetL->current_item();
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
        tempItemsetR = tempSeqR->current_itemset();
        tempItemsetR->rewind();
        std::string itemRight = tempItemsetR->current_item();
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
    GspJoinTree *joinTree = new GspJoinTree();

    for (std::list<GspSequence *>::iterator it = sequences_.begin(); it != sequences_.end(); ++it)
    {
      joinTree->AddSequence(*it);
    }

    std::list<GspSequence *> *res;
    for (std::list<GspSequence *>::iterator it = sequences_.begin(); it != sequences_.end(); ++it)
    {
      res = NULL;
      GspSequence *copy = new GspSequence(**it);
      copy->DropFirstItem();
//      std::cout<<"Checking: "<<(*it)->ToString()<<" | "<<copy->ToString()<<std::endl;
      joinTree->FindJoinable(copy, res);
      if (res && res->size() > 0)
      {
//        std::cout<<"SEQ: "<<(*it)->ToString()<<std::endl;
        for(std::list<GspSequence *>::iterator jt = res->begin(); jt != res->end(); ++jt)
        {
          GspSequence *toAdd = new GspSequence(**it);
          toAdd->AppendSequence(*jt);
          ret->sequences_.push_back(toAdd);
//          std::cout<<(*jt)->ToString()<<std::endl;
        }
      }

      delete copy;
    }

    delete joinTree;

    /*
    GspSequence *candidate;
    for(std::list<GspSequence *>::const_iterator it = sequences_.begin();
        it != sequences_.end();
        ++it)
    {
      for(std::list<GspSequence *>::const_iterator jt = sequences_.begin();
          jt != sequences_.end();
          ++jt)
      {
        candidate = (*it)->JoinSequences(*jt);
        if (candidate)
        {
          ret->sequences_.push_back(candidate);
        }
      }
    }
    */
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
    std::cout<<(*it)->ToString()<<" "<<(*it)->get_support()<<"\n";
  }
}

void GspSequencePool::PrintResult(std::ostream &str)
{
  for(std::list<GspSequence *>::const_iterator it = sequences_.begin();
      it != sequences_.end();
      ++it)
  {
    str<<((*it)->ToString())<<"\n";
  }
}

/* Documented in header */
void GspSequencePool::CountSupport(GspSequenceReader *reader)
{
  GspHashTree tree(k_, 15);

  for (std::list<GspSequence *>::const_iterator it = sequences_.begin();
       it != sequences_.end();
       ++it)
  {
    tree.AddSequence(*it);
  }

  GspSequence *seqTemp;

  reader->RewindStream();
  while((seqTemp = reader->GetNextSequence()))
  {
//    std::cout<<seqTemp->ToString();
    tree.CheckClientSequence(seqTemp,
                             parent_);

//    std::cout<<"\nClient sequence: "<<seqTemp->ToString()<<" :"<<std::endl;
//    seqTemp->print_candidates();
//    std::cout<<std::flush;
    seqTemp->CheckCandidates(parent_->window_size(), parent_->min_gap(), parent_->max_gap());
//    seqTemp->print_candidates();
    delete seqTemp;
  }
}

void GspSequencePool::DropSequences()
{
  for(std::list<GspSequence *>::iterator it = sequences_.begin(); it !=  sequences_.end();)
  {
    std::list<GspSequence *>::iterator prevIt = it++;
    if ((*prevIt)->get_support() <= parent_->min_support())
    {
      delete *prevIt;
      sequences_.erase(prevIt);
    }
  }
}

/* Documented in header */
GspSequencePool::~GspSequencePool()
{
  for(std::list<GspSequence *>::iterator it = sequences_.begin();
      it != sequences_.end(); ++it
      )
  {
    delete *it;
  }
}

std::set<std::string> *GspSequencePool::ToStringSet()
{
  std::set<std::string> *ret = new std::set<std::string>();

  for(std::list<GspSequence *>::iterator it = sequences_.begin(); it != sequences_.end(); ++it)
  {
    ret->insert((*it)->ToString());
  }

  return ret;
}

void GspSequencePool::DropNonContiguous(std::set<std::string> *stringSet)
{
  for(std::list<GspSequence *>::iterator it = sequences_.begin(); it != sequences_.end();)
  {
    if((!(*it)->ContiguousCheck(stringSet)))
    {
      delete *it;
      sequences_.erase(it++);
    }
    else
    {
      ++it;
    }
  }
}

