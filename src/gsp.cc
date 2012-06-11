/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp.cc
 *
 * @brief Generalized Sequential Patterns (GSP) application
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:30:39 2012
 */

#include <iostream>
#include <vector>
#include <limits>

#include "gsp_array_reader.h"
#include "gsp_sequence_class.h"
#include "gsp_algorithm.h"

GspArrayReader *createSampleArrayReader();

int main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;
  GspArrayReader *arrReader = createSampleArrayReader();
  GspAlgorithm gsp(arrReader, 1, 0, 0, 30);

  gsp.printFrequentSeqs();
  std::cout<<"\n\n";
  gsp.printCandidateSeqs();
  std::cout<<"\n\n";
  gsp.runPass();


  std::cout<<std::endl<<std::endl<<"EXITING!!"<<std::endl;

  return 0;
}

GspArrayReader *createSampleArrayReader()
{
  std::vector<GspSequence *> sequences;

  GspSequence *seq = new GspSequence("C1");
  GspItemset *itSet = new GspItemset(1);
  itSet->add_item("R");
  seq->add_itemset(itSet);
  itSet = new GspItemset(2);
  itSet->add_item("F");
  seq->add_itemset(itSet);
  itSet = new GspItemset(15);
  itSet->add_item("S");
  itSet->add_item("T");
  seq->add_itemset(itSet);
  sequences.push_back(seq);

  seq = new GspSequence("C2");
  itSet = new GspItemset(1);
  itSet->add_item("F");
  itSet->add_item("R");
  seq->add_itemset(itSet);
  itSet = new GspItemset(20);
  itSet->add_item("G");
  seq->add_itemset(itSet);
  itSet = new GspItemset(50);
  itSet->add_item("S");
  seq->add_itemset(itSet);
  sequences.push_back(seq);

  return new GspArrayReader(sequences);
}

