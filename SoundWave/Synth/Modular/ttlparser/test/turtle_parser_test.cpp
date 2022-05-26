#include <stdio.h>
#include <string>
#include <gtest/gtest.h>
#include "turtle_parser.h"


class TurtleParserTest : public testing::Test {
protected:
};

TEST_F(TurtleParserTest, parse1) {
  std::string input = "@prefix foaf: <http://xmlns.com/foaf/0.1/> . \n";
  input += "[ foaf:name \"Alice\" ] foaf:knows [ \n";
  input += "foaf:name \"Bob\" ; \n";
  input += "foaf:knows [ \n";
  input += "foaf:name \"Eve\" ] ; \n";
  input += "foaf:mbox <bob@example.com> ] .";
  TurtleParser parser(input);
  std::string subject;
  std::string predicate;
  std::string object;
  int count = 0;
  while(parser.parse(subject, predicate, object)){
    count++;
  }
  EXPECT_EQ(6, count);
}

TEST_F(TurtleParserTest, parse2) {
  std::string input = "PREFIX : <http://example.org/stuff/1.0/>";
  input += ":a :b ( \"apple\" \"banana\" ) .";
  TurtleParser parser(input);
  std::string subject;
  std::string predicate;
  std::string object;
  int count = 0;
  while(parser.parse(subject, predicate, object)){
    count++;
  }
  EXPECT_EQ(5, count);
}

TEST_F(TurtleParserTest, parse3) {
  TurtleFileParser parser("test/data/ttl_test2.ttl");
  std::string subject;
  std::string predicate;
  std::string object;
  int count = 0;
  while(parser.parse(subject, predicate, object)){
    count++;
  }
  EXPECT_EQ(5, count);
}

/*
TEST_F(TurtleParserTest, parse4) {
  TurtleFileParser parser("test/data/ttl_test3.nt");
  std::string subject;
  std::string predicate;
  std::string object;
  int count = 0;
  while(parser.parse(subject, predicate, object)){
    count++;
  }
  EXPECT_EQ(10916457, count);
}
*/
