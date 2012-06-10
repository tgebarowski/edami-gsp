/**
 * @file  tortures.cc
 *
 * @brief Unit tests
 * 
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Sat May 19 18:19:22 2012
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN


#include <boost/test/unit_test.hpp>
#include <stdio.h>

using boost::unit_test::test_suite;

#include "gsp_hashtree_class.h"
#include "gsp_sequence_class.h"
#include "gsp_itemset_class.h"
#include "gsp_datastore_class.h"

/**
 * @brief Test Case for GspSequence primitives
 */
BOOST_AUTO_TEST_CASE(tc_sequence_primitives)
{
  GspSequence *seq = new GspSequence("s1");
  GspItemset *is1 = new GspItemset();
  GspItemset *is2 = new GspItemset();
  GspItemset *is3 = new GspItemset();
  is1->add_item("a");
  is1->add_item("b");
  is2->add_item("c");
  is2->add_item("d");
  is2->add_item("e");
  is2->add_item("f");
  is3->add_item("g");
  
  seq->add_itemset(is1);
  seq->add_itemset(is2);
  seq->add_itemset(is3);

  cout << "Input sequence: " + seq->ToString() << endl;

  const string sequence_str = "abcdefgh";

#if 0  
  for (size_t pos = 0; pos < sequence_str.length() - 1; pos++)
  {
    string found_item = seq->GetItemByIndex(pos);
    string correct_item = ToString(sequence_str[pos]);
    cout << "Find element at index: "\
         << pos << "=>" << found_item  << endl;
    BOOST_CHECK(correct_item == found_item);
  }
#endif
}

/**
 * @brief Test Case for Sequence->AppendItemset
 */
#if 0
BOOST_AUTO_TEST_CASE(tc_sequence_append_itemset)
{
  GspSequence *seq = new GspSequence("s1");
  GspItemset *is1 = new GspItemset();
  GspItemset *is2 = new GspItemset();
  GspItemset *appended_1 = new GspItemset();
  GspItemset *appended_2 = new GspItemset();
  is1->add_item("a");
  is1->add_item("b");
  is2->add_item("c");
  is2->add_item("d");

  seq->add_itemset(is1);
  seq->add_itemset(is2);

  appended_1->add_item("e");
  appended_2->add_item("f");
  appended_2->add_item("g");

  cout << "Input sequence: " << seq->ToString() << endl;
  seq->AppendItemset(appended_1);

  cout << "After appending " << appended_1->ToString()\
       << ": " << seq->ToString() << endl;

  /* Remove previously appended item */
  is2->remove_last_item();

  cout << "Input sequence: " << seq->ToString() << endl;
  seq->AppendItemset(appended_2);

  cout << "After appending " << appended_2->ToString()\
       << ": " << seq->ToString() << endl;
}
#endif

/**
 * @brief Test Case for Sequence->CompareWithSubsequence
 */
#if 0
BOOST_AUTO_TEST_CASE(tc_sequence_compare)
{
  GspHashTree ht(2,2);
  GspSequence *seq = new GspSequence("s1");
  GspSequence *seq2= new GspSequence("s2");
  GspItemset *is1 = new GspItemset();
  GspItemset *is2 = new GspItemset();
  GspItemset *is3 = new GspItemset();
  GspItemset *is4 = new GspItemset();
  is1->add_item("a");
  is1->add_item("b");
  is2->add_item("c");
  is2->add_item("d");

  is3->add_item("b");
  is4->add_item("c");
  is4->add_item("d");
  is4->add_item("e");
  
  seq->add_itemset(is1);
  seq->add_itemset(is2);

  seq2->add_itemset(is3);
  seq2->add_itemset(is4);

  cout << "Input sequence (s1): " + seq->ToString() << endl;
  cout << "Input sequence (s2): " + seq2->ToString() << endl;

  BOOST_CHECK_EQUAL(true, seq->CompareWithSubsequence(*seq2));
}
#endif


/**
 * @brief Test Case for HashTree->AddSequence
 */
#if 0
BOOST_AUTO_TEST_CASE(tc_hashtree_add_sequence)
{
  GspHashTree ht(2, /* max  level */
                 2 /* max leaf */);
  GspSequence *seq = new GspSequence("s1");
  GspSequence *seq2 = new GspSequence("s2");
  GspSequence *seq3 = new GspSequence("s3");
  GspSequence *seq4 = new GspSequence("s4");
  GspItemset *is1 = new GspItemset();
  GspItemset *is2 = new GspItemset();
  GspItemset *is3 = new GspItemset();
  is1->add_item("a");
  is1->add_item("b");
  is2->add_item("c");
  is3->add_item("c");
  is3->add_item("d");
  is3->add_item("e");
  
  seq->add_itemset(is1);
  seq->add_itemset(is2);

  seq2->add_itemset(is1);

  seq3->add_itemset(is2);

  seq4->add_itemset(is2);
  seq4->add_itemset(is3);

  cout << "Input sequence: " + seq->ToString() << endl;

  BOOST_CHECK_EQUAL(GSP_OK, ht.AddSequence(seq));
  BOOST_CHECK_EQUAL(GSP_OK, ht.AddSequence(seq2));
  BOOST_CHECK_EQUAL(GSP_OK, ht.AddSequence(seq3));
  BOOST_CHECK_EQUAL(GSP_OK, ht.AddSequence(seq4));
}
#endif

/**
 * @brief Test Case for HashTree->FindSequence
 */
#if 0
BOOST_AUTO_TEST_CASE(tc_hashtree_find_sequence)
{
  GspHashTree ht(2, /* max  level */
                 2 /* max leaf */);
  GspSequence *seq = new GspSequence("s1");
  GspSequence *seq2 = new GspSequence("s2");
  GspSequence *seq3 = new GspSequence("s3");
  GspSequence *seq4 = new GspSequence("s4");
  GspItemset *is1 = new GspItemset();
  GspItemset *is2 = new GspItemset();
  GspItemset *is3 = new GspItemset();
  is1->add_item("a");
  is1->add_item("b");
  is2->add_item("c");
  is3->add_item("c");
  is3->add_item("d");
  is3->add_item("e");
  
  seq->add_itemset(is1);
  seq->add_itemset(is2);

  seq2->add_itemset(is1);

  seq3->add_itemset(is2);

  seq4->add_itemset(is2);
  seq4->add_itemset(is3);

  BOOST_CHECK_EQUAL(GSP_OK, ht.AddSequence(seq));
  BOOST_CHECK_EQUAL(GSP_OK, ht.AddSequence(seq2));
  BOOST_CHECK_EQUAL(GSP_OK, ht.AddSequence(seq3));
  BOOST_CHECK_EQUAL(GSP_OK, ht.AddSequence(seq4));

  cout << "Looking for: " << seq2->ToString() << endl;

  const GspSequence *found = ht.FindSequence(*seq2);
  
  if (found != NULL)
  {
    cout << "Found: " << found->ToString() << endl;
  }
}
#endif

/**
 * @brief Test Case for DataStore
 */
BOOST_AUTO_TEST_CASE(tc_datastore)
{
  GspDataStore ds("test.dat");
  
  GspSequence *sequence = NULL;

  cout << "Testing Data Store..." << endl;
  
  while ((sequence = ds.GetNextSequence()) != NULL)
  {
    cout << "ID:" << sequence->getId() << " "\
         << sequence->ToString() << endl;
  }
  cout << "End of Data Store testing..." << endl;
}



