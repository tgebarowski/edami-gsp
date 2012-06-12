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

/**
 * @brief Implementation of GspSequenceReader interface
 *        realizing in-memory storage of sequences_
 */
class GspArrayReader : public GspSequenceReader
{
  public:

    GspArrayReader(const std::vector<GspSequence *> & seqs) :
      sequences_(seqs),
      iter_(sequences_.begin())
    {
    }

    ~GspArrayReader()
    {
      for (std::vector<GspSequence *>::iterator jt = sequences_.begin();
           jt != sequences_.end();
           ++jt)
      {
        delete *jt;
      }
    }

    /**
     * @brief Is stream valid, could be opened ?
     *
     * @return Defines if stream could be opened
     */
    bool IsValid()
    {
      if (sequences_.size() > 0)
      {
        return true;
      }
      return false;
    }
      
   /**
    * @brief Rewind stream to begining
    *
    * @return True is success, otherwise false
    */
    bool RewindStream()
    {
      iter_ = sequences_.begin();
      return true;
    }

    /**
     * @brief Get next @ref GspSequence from stream
     *
     * @return Newly allocated @ref GspSequence object, to be freed by method caller
     */
    GspSequence *GetNextSequence()
    {
      if (iter_ == sequences_.end())
        return NULL;

      return new GspSequence(**iter_++);
    }

  private:
    std::vector<GspSequence *> sequences_;
    std::vector<GspSequence *>::const_iterator iter_;
};

#endif /* GSP_ARRAY_READER_H_ */
