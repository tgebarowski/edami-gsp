/*
 * gsp_sequence_pool.cpp
 *
 *  Created on: Jun 9, 2012
 *      Author: adas
 */

#include <map>
#include <set>
#include <string>

#include "gsp_sequence_pool.h"
#include "gsp_sequence_reader.h"
#include "gsp_hashtree_class.h"

GspSequencePool::GspSequencePool(unsigned minSupport_, int windowSize_, int minGap_, int maxGap_)
  : k(0), minSupport(minSupport_), windowSize(windowSize_), minGap(minGap_), maxGap(maxGap_)
{

}

GspSequencePool::GspSequencePool(GspSequenceReader *reader_, unsigned minSupport_, int windowSize_, int minGap_, int maxGap_)
  : k(1), minSupport(minSupport_), windowSize(windowSize_), minGap(minGap_), maxGap(maxGap_)
{
  GspSequence *seq;
  std::map<std::string, unsigned> cand1;
  std::set<std::string> cand1Sequence;

  while((seq = reader_->GetNextSequence()))
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

    for (std::set<std::string>::const_iterator seqIt = cand1Sequence.begin(); seqIt!= cand1Sequence.end(); ++seqIt)
    {
      std::map<std::string, unsigned>::iterator mapIter = cand1.find(*seqIt);
      if(mapIter != cand1.end())
        ++(mapIter->second);
      else
        cand1[*seqIt] = 1;
    }

    delete seq;
  }

  for(std::map<std::string, unsigned>::iterator mapIter = cand1.begin(); mapIter != cand1.end(); ++mapIter)
  {
    if (mapIter->second > minSupport)
    {
      seq = new GspSequence("");
      GspItemset *itemset = new GspItemset();
      itemset->add_item(mapIter->first);
      seq->add_itemset(itemset);
      seq->setSupport(mapIter->second);
      sequences.push_back(seq);
    }
  }
}

GspSequencePool *GspSequencePool::join()
{
  GspSequencePool *ret = new GspSequencePool(minSupport, windowSize, minGap, maxGap);
  GspSequence *newSeq;
  GspItemset *newItemset;
  GspSequence *tempSeqL, *tempSeqR;
  GspItemset *tempItemsetL, *tempItemsetR;
  ret->k = k+1;

  if (k == 1)
  {
    std::list<GspSequence *>::const_iterator it = sequences.begin();
    std::list<GspSequence *>::const_iterator jt = it++;
    for (; jt != sequences.end();)
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
      ret->sequences.push_back(newSeq);

      for (; it != sequences.end(); ++it)
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
        ret->sequences.push_back(newSeq);

        //(R)(L)
        newSeq = new GspSequence("");
        newItemset = new GspItemset(0);
        newItemset->add_item(itemRight);
        newSeq->add_itemset(newItemset);
        newItemset = new GspItemset(0);
        newItemset->add_item(itemLeft);
        newSeq->add_itemset(newItemset);
        ret->sequences.push_back(newSeq);

        //(LR) ordered
        newSeq = new GspSequence("");
        newItemset = new GspItemset(0);
        newItemset->add_item(itemRight);
        newItemset->add_item(itemLeft);
        newSeq->add_itemset(newItemset);
        ret->sequences.push_back(newSeq);
      }
      it = ++jt;
      jt = it++;
    }
  }
  else
  {
    GspSequence *candidate;
    for(std::list<GspSequence *>::const_iterator it = sequences.begin(); it != sequences.end(); ++it)
    {
      for(std::list<GspSequence *>::const_iterator jt = sequences.begin(); jt != sequences.end(); ++jt)
      {
        candidate = (*it)->joinSequences(*jt);
        if (candidate)
          ret->sequences.push_back(candidate);
      }
    }
  }

  return ret;
}

void GspSequencePool::printSequences()
{
  for(std::list<GspSequence *>::const_iterator it = sequences.begin(); it != sequences.end(); ++it)
  {
    std::cout<<(*it)->ToString()<<" "<<(*it)->getSupport()<<"\n";
  }
}

void GspSequencePool::countSupport(GspSequenceReader *reader)
{
  //TODO cos madrzejszego niz 4:)
  GspHashTree tree(k, 2);

  for (std::list<GspSequence *>::const_iterator it = sequences.begin(); it != sequences.end(); ++it)
  {
    tree.AddSequence(*it);
  }
  std::cout<<std::endl;

  GspSequence *seqTemp;

  tree.printTree();

  //TODO sprawdzac returna
  reader->RewindStream();
  while((seqTemp = reader->GetNextSequence()))
  {
    tree.checkClientSequence(seqTemp, windowSize, minGap, maxGap);
    std::cout<<"Client sequence: "<<seqTemp->ToString()<<" :"<<std::endl;
    seqTemp->printCandidates();
    //TODO add calculating support for candidates
    delete seqTemp;
  }
}

GspSequencePool::~GspSequencePool()
{
  for(std::list<GspSequence *>::iterator it = sequences.begin(); it != sequences.end(); ++it)
  {
    delete *it;
  }
}

