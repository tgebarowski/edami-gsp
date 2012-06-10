/*
 * gsp_sequence_pool.h
 *
 *  Created on: Jun 9, 2012
 *      Author: adas
 */

#ifndef GSP_ALGORITHM_H_
#define GSP_ALGORITHM_H_SEQUENCE_POOL

#include <memory>

#include "gsp_sequence_reader.h"
#include "gsp_sequence_pool.h"

class GspAlgorithm
{
  private:
    std::auto_ptr<GspSequenceReader> reader;
    unsigned  k;
    unsigned minSupport;
    unsigned windowSize;
    unsigned minGap;
    unsigned maxGap;

    GspSequencePool *frequent;
    GspSequencePool *candidates;

    bool finished;

  public:
    GspAlgorithm(GspSequenceReader *inReader_, unsigned minSupport_, unsigned windowSize_, unsigned minGap_, unsigned maxGap_);
    ~GspAlgorithm();
    unsigned getK()
    {
      return k;
    }
    unsigned getFrequentCnt()
    {
      if(frequent)
        return frequent->getSequenceCnt();
      return 0;
    }
    unsigned getCandidateCnt()
    {
      if (candidates)
        return candidates->getSequenceCnt();
      return 0;
    }
    void printFrequentSeqs();
    void printCandidateSeqs();
    void runPass();
    bool isFinished()
    {
      return finished;
    }
};

#endif /* GSP_ALGORITHM_H_ */
