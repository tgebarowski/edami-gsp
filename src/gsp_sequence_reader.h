/**
 * @file  gsp_sequence_reader.h
 *
 * @brief Abstract interface for a data source
 *
 * @author: Adam Szczepankiewicz <aszczepa@cern.ch>
 * @date: Fri Jun 8 15:14:38 2012
 */

#ifndef GSP_SEQUENCE_READER_H_
#define GSP_SEQUENCE_READER_H_

class GspSequence;

/**
 * @brief Abstract interface for sequence reader
 */

class GspSequenceReader
{
  public:
    /**
     * @brief Destructor
     */
    virtual ~GspSequenceReader() {};
  

    /**
     * @brief Is stream valid, could be opened ?
     *
     * @return Defines if stream could be opened
     */
    virtual bool IsValid() = 0;

    /**
     * @brief Rewind stream to begining
     *
     * @return True is success, otherwise false
     */
    virtual bool RewindStream() = 0;

    /**
     * @brief Get next @ref GspSequence from stream
     *
     * @return Newly allocated @ref GspSequence object, to be freed by method caller
     */
    virtual GspSequence *GetNextSequence() = 0;
};

#endif /* GSP_SEQUENCE_READER_H_ */
