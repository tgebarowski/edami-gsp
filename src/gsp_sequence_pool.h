/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_sequence_pool.h
 *
 * @brief GSP Sequence Pool
 *
 * @author: Adam Szczepankiewicz <adam.szczepankiewicz@cern.ch>
 * @date: Sat Jun 9 15:30:39 2012
 */

#ifndef GSP_SEQUENCE_POOL_H_
#define GSP_SEQUENCE_POOL_H_

#include <list>
#include <fstream>

#include "gsp_itemset_class.h"
#include "gsp_sequence_class.h"

/* Predefinitions */
class GspSequenceReader;
class GspAlgorithm;

class GspSequencePool
{
  public:
 
    GspSequencePool(GspAlgorithm *parent);

    GspSequencePool(GspSequenceReader *reader,
                    GspAlgorithm *parent);

    ~GspSequencePool();

    unsigned int GetK()
    {
      return k_;
    }

    unsigned int GetSequenceCount()
    {
      return sequences_.size();
    }

    GspSequencePool *Join();

    void CountSupport(GspSequenceReader *reader);

    void DropSequences();

    void PrintSequences();

    void PrintResult(std::ostream &str);

  private:

    GspAlgorithm *parent_;
    std::list<GspSequence *> sequences_;
    unsigned int k_;
};

#endif /* GSP_SEQUENCE_POOL_H_ */
