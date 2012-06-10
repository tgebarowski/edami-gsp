/*
 * gsp_sequence_pool.cpp
 *
 *  Created on: Jun 9, 2012
 *      Author: adas
 */

#include <cstdlib>

#include "gsp_algorithm.h"
#include "gsp_sequence_reader.h"
#include "gsp_sequence_pool.h"

GspAlgorithm::GspAlgorithm(GspSequenceReader *inReader_, unsigned minSupport_, int windowSize_, int minGap_, int maxGap_)
  : reader(inReader_), k(1), minSupport(minSupport_), windowSize(windowSize_), minGap(minGap_), maxGap(maxGap_), frequent(NULL), candidates(NULL), finished(false)
{
  frequent = new GspSequencePool(reader.get(), minSupport, windowSize, minGap, maxGap);
  if (frequent->getSequenceCnt() == 0)
    finished = true;
}

GspAlgorithm::~GspAlgorithm()
{
  delete frequent;
  delete candidates;
}

void GspAlgorithm::printFrequentSeqs()
{
  if(frequent)
    frequent->printSequences();
}

void GspAlgorithm::printCandidateSeqs()
{
  if(candidates)
    candidates->printSequences();
}

void GspAlgorithm::runPass()
{
  if (finished)
    return;

  candidates = frequent->join();
  if (candidates->getSequenceCnt() == 0)
  {
    finished = true;

    return;
  }

  candidates->printSequences();
  std::cout<<std::flush;

  std::cout<<std::endl;
  candidates->countSupport(reader.get());


}

