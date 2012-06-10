/*
 * gsp_abstract_sequence_reader.h
 *
 *  Created on: Jun 9, 2012
 *      Author: adas
 */

#ifndef GSP_SEQUENCE_READER_H_
#define GSP_SEQUENCE_READER_H_

class GspSequence;

class GspSequenceReader
{
  public:
    virtual ~GspSequenceReader() = 0;
    virtual bool RewindStream() = 0;
    virtual GspSequence *GetNextSequence() = 0;
};

inline GspSequenceReader::~GspSequenceReader()
{

}

#endif /* GSP_SEQUENCE_READER_H_ */
