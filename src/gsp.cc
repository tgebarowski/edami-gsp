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
  GspArrayReader *arrReader = createSampleArrayReader();
  GspAlgorithm gsp(arrReader, 1, 0, 0, numeric_limits<unsigned>::max());

  gsp.printFrequentSeqs();
  std::cout<<"\n\n";
  gsp.runPass();
  gsp.printCandidateSeqs();

  return 0;
}

GspArrayReader *createSampleArrayReader()
{
  std::vector<GspSequence *> sequences;

  GspSequence *seq = new GspSequence("C1");
  GspItemset *itSet = new GspItemset(1);
  itSet->add_item("Ringworld");
  seq->add_itemset(itSet);
  itSet = new GspItemset(2);
  itSet->add_item("Foundation");
  seq->add_itemset(itSet);
  itSet = new GspItemset(15);
  itSet->add_item("Ringworld Engineers");
  itSet->add_item("Second Foundation");
  seq->add_itemset(itSet);
  sequences.push_back(seq);

  seq = new GspSequence("C2");
  itSet = new GspItemset(1);
  itSet->add_item("Foundation");
  itSet->add_item("Ringworld");
  seq->add_itemset(itSet);
  itSet = new GspItemset(20);
  itSet->add_item("Foundation and Empire");
  seq->add_itemset(itSet);
  itSet = new GspItemset(50);
  itSet->add_item("Ringworld Engineers");
  seq->add_itemset(itSet);
  sequences.push_back(seq);

  return new GspArrayReader(sequences);
}

