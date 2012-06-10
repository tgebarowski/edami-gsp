/*
 * gsp_array_reader.h
 *
 *  Created on: Jun 9, 2012
 *      Author: adas
 */

#ifndef GSP_ARRAY_READER_H_
#define GSP_ARRAY_READER_H_

#include "gsp_sequence_reader.h"
#include "gsp_sequence_class.h"
#include "gsp_itemset_class.h"

//class GspSequence;

#include <vector>

class GspArrayReader : public GspSequenceReader
{
  private:
    std::vector<GspSequence *> sequences;
    std::vector<GspSequence *>::const_iterator iter;
  public:
    GspArrayReader(const std::vector<GspSequence *> & seqs) :
      sequences(seqs), iter(sequences.begin())
    {
    }

    ~GspArrayReader()
    {
      for (std::vector<GspSequence *>::iterator jt = sequences.begin(); jt != sequences.end(); ++jt)
        delete *jt;
    }

    bool rewindStream()
    {
      iter = sequences.begin();

      return true;
    }

    GspSequence *getNextSequence()
    {
      if (iter == sequences.end())
        return NULL;

      return new GspSequence(**iter++);
    }
};

#endif /* GSP_ARRAY_READER_H_ */
