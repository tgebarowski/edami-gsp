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
    k_(0),
    joinTree_(NULL)
{
}

/* Documented in header */
GspSequencePool::GspSequencePool(GspSequenceReader *reader,
                                 GspAlgorithm *parent)
  : parent_(parent),
    k_(1),
    joinTree_(NULL)
{
  GspSequence *seq;
  std::map<int, unsigned> cand1;
  std::set<int> cand1Sequence;

  //create 1-length candidates,
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

    for (std::set<int>::const_iterator seqIt = cand1Sequence.begin();
         seqIt!= cand1Sequence.end(); ++seqIt)
    {
      std::map<int, unsigned>::iterator mapIter = cand1.find(*seqIt);
      if(mapIter != cand1.end())
        ++(mapIter->second);
      else
        cand1[*seqIt] = 1;
    }
//    std::cout<<seq->ToString()<<"\n";
    delete seq;
  }

  for(std::map<int, unsigned>::iterator mapIter = cand1.begin();
      mapIter != cand1.end(); ++mapIter)
  {
    if (mapIter->second > parent_->min_support())
    {
      seq = new GspSequence(-1);
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
      int itemLeft = tempItemsetL->current_item();
      newSeq = new GspSequence(-1);
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
        int itemRight = tempItemsetR->current_item();
        //(L)(R)
        newSeq = new GspSequence(-1);
        newItemset = new GspItemset(0);
        newItemset->add_item(itemLeft);
        newSeq->add_itemset(newItemset);
        newItemset = new GspItemset(0);
        newItemset->add_item(itemRight);
        newSeq->add_itemset(newItemset);
        ret->sequences_.push_back(newSeq);

        //(R)(L)
        newSeq = new GspSequence(-1);
        newItemset = new GspItemset(0);
        newItemset->add_item(itemRight);
        newSeq->add_itemset(newItemset);
        newItemset = new GspItemset(0);
        newItemset->add_item(itemLeft);
        newSeq->add_itemset(newItemset);
        ret->sequences_.push_back(newSeq);

        //(LR) ordered
        newSeq = new GspSequence(-1);
        newItemset = new GspItemset(0);
        newItemset->add_item(itemRight);
        newItemset->add_item(itemLeft);
        newSeq->add_itemset(newItemset);
        ret->sequences_.push_back(newSeq);
      }
      it = ++jt;
	  if(it != sequences_.end())
		jt = it++;
    }
  }
  else
  {
    joinTree_ = new GspJoinTree();

    for (std::list<GspSequence *>::iterator it = sequences_.begin(); it != sequences_.end(); ++it)
    {
      joinTree_->AddSequence(*it);
    }

    std::list<GspSequence *> res;
    for (std::list<GspSequence *>::iterator it = sequences_.begin(); it != sequences_.end(); ++it)
    {
      res.clear();
      GspSequence *copy = new GspSequence(**it);
      copy->DropFirstItem();
//      std::cout<<"Checking: "<<(*it)->ToString()<<" | "<<copy->ToString()<<std::endl;
      joinTree_->FindJoinable(copy, res);
      if (res.size() > 0)
      {
//        std::cout<<"FOUND!! "<<(*it)->ToString()<<std::endl;
        for(std::list<GspSequence *>::iterator jt = res.begin(); jt != res.end(); ++jt)
        {
//          std::cout<<(*jt)->ToString()<<std::endl;
          GspSequence *toAdd = new GspSequence(**it);
          toAdd->AppendSequence(*jt);
          ret->sequences_.push_back(toAdd);

        }
      }

      delete copy;
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
    std::cout<<(*it)->ToString()<<" "<<(*it)->get_support()<<"\n";
  }
}

void GspSequencePool::PrintResult(std::ostream &str)
{
  for(std::list<GspSequence *>::const_iterator it = sequences_.begin();
      it != sequences_.end();
      ++it)
  {
    str<<((*it)->ToString())<<" support: "<<(*it)->get_support()<<"\n";
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
    (*it)->ToString();
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

  delete joinTree_;
}

/*
std::set<std::string> *GspSequencePool::ToStringSet()
{
  std::set<std::string> *ret = new std::set<std::string>();

  for(std::list<GspSequence *>::iterator it = sequences_.begin(); it != sequences_.end(); ++it)
  {
    ret->insert((*it)->ToString());
  }

  return ret;
}
*/

void GspSequencePool::DropNonContiguous(GspJoinTree * joinTree)
{
  for(std::list<GspSequence *>::iterator it = sequences_.begin(); it != sequences_.end();)
  {
    if((!(*it)->ContiguousCheck(joinTree)))
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

