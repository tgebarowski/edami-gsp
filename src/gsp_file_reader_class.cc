/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_file_reader_class.cc
 *
 * @brief File Reader representation
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Thu Jun 07 18:08:22 2012
 */

#include "gsp_file_reader_class.h"
#include "gsp_itemset_class.h"
#include "gsp_sequence_class.h"
#include "gsp_common.h"

/* Documented in header */
GspFileReader::GspFileReader(string file_path)
  : path_(file_path), input_file_(path_.c_str())
{
  begin_iterator_ = input_file_;
  line_iterator_ = begin_iterator_;
}

/* Documented in header */
GspFileReader::~GspFileReader()
{
}

/* Documented in header */
bool GspFileReader::RewindStream()
{
  input_file_.clear();
  input_file_.seekg(0, ios::beg);

  begin_iterator_ = input_file_;
  line_iterator_ = begin_iterator_;

  return IsValid();
}

/* Documented in header */
GspSequence *GspFileReader::GetNextSequence()
{
  string id = "", old_id = "";
  GspSequence *sequence = NULL;
  GspItemset * itemset = GetNextItemset(id);

  if (itemset != NULL)
  {
    sequence = new GspSequence(ToInt(id));
  
    do
    {
      old_id = id;
      sequence->add_itemset(itemset);
      itemset = GetNextItemset(id);
    }
    while (id == old_id &&
           itemset != NULL);
  }
  return sequence;
}

/* Documented in header */
bool GspFileReader::IsValid()
{
  return input_file_.is_open();
}

/* Documented in header */
GspItemset * GspFileReader::GetNextItemset(string &p_id)
{
  GspItemset *itemset = NULL;
  istream_iterator<string> eol;
  string id = "";
  
  if (line_iterator_ !=  eol)
  {
    vector<string> *tokens = Tokenize(*line_iterator_, '#');

    if (tokens != NULL &&
        tokens->size() == 3)
    {
      id = (*tokens)[0];

      string timestamp = (*tokens)[1];
      
      vector<string> * seq_tokens = Tokenize((*tokens)[2], ',');

      itemset = new GspItemset(ToInt(timestamp));

      if (seq_tokens != NULL)
      {
        for (unsigned int i = 0; i < seq_tokens->size(); i++)
        {
          itemset->add_item(ToInt((*seq_tokens)[i]));
        }
        delete seq_tokens;
      }
      delete tokens;
    }

    if (id == p_id ||
        p_id == "")
    {
      ++line_iterator_;
    }
    p_id = id;
  }
  return itemset;
}

