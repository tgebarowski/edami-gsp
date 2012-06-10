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
  public:
    GspSequencePool(unsigned minSupport_);
    GspSequencePool(GspSequenceReader *reader_, unsigned minSupport_);
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
    void printSequences();
};

#endif /* GSP_SEQUENCE_POOL_H_ */
