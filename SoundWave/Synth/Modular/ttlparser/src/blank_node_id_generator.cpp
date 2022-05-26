#include "blank_node_id_generator.h"


BlankNodeIdGenerator::BlankNodeIdGenerator() : seq_no(0) {
}
BlankNodeIdGenerator::BlankNodeIdGenerator(int init_no) : seq_no(init_no) {
}

const std::string BlankNodeIdGenerator::DEFAULT_PREFIX = "_:";

std::string BlankNodeIdGenerator::generate() {
  std::stringstream ss;
  ss << DEFAULT_PREFIX << seq_no;
  seq_no++;
  return ss.str();
}
