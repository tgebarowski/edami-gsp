/*
 * gsp_sequence_pool.h
 *
 *  Created on: Jun 9, 2012
 *      Author: adas
 */

#ifndef GSP_SEQUENCE_POOL_H_
#define GSP_SEQUENCE_POOL_H_

#include <list>

#include "gsp_itemset_class.h"
#include "gsp_sequence_class.h"

//#include "gsp_sequence_class.h"
//#include "gsp_sequence_reader.h"
class GspSequenceReader;


class GspSequencePool
{
  private:
    std::list<GspSequence *> sequences;
    unsigned k;
    unsigned minSupport;
    int windowSize;
    int minGap;
    int maxGap;
  public:
    GspSequencePool(unsigned minSupport_, int windowSize, int minGap, int maxGap);
    GspSequencePool(GspSequenceReader *reader_, unsigned minSupport_, int windowSize, int minGap, int maxGap);
    ~GspSequencePool();
    unsigned getK()
    {
      return k;
    }
    unsigned getSequenceCnt()
    {
      return sequences.size();
    }
    GspSequencePool *join();
    void countSupport(GspSequenceReader *reader);
    void printSequences();
};

#endif /* GSP_SEQUENCE_POOL_H_ */
