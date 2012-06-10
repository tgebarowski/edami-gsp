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

GspAlgorithm::GspAlgorithm(GspSequenceReader *inReader_, unsigned minSupport_, unsigned windowSize_, unsigned minGap_, unsigned maxGap_)
  : reader(inReader_), k(1), minSupport(minSupport_), windowSize(windowSize_), minGap(minGap_), maxGap(maxGap_), frequent(NULL), candidates(NULL), finished(false)
{
  frequent = new GspSequencePool(reader.get(), minSupport);
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


}

