#ifndef BLANK_NODE_ID_GENERATOR
#define BLANK_NODE_ID_GENERATOR

#include <string>
#include <sstream>


class BlankNodeIdGenerator {
public:
  BlankNodeIdGenerator();
  BlankNodeIdGenerator(int init_no);

  std::string generate();

private:
  const static std::string DEFAULT_PREFIX;
  int seq_no;
};


#endif
