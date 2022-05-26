#Makefile

PARSER_DIR = .

CPPFLAGS += -isystem $(PARSER_DIR)/include -isystem $(GTEST_DIR)/include

CXXFLAGS += -pthread -std=c++11 -O2 -fPIC -march=native -mavx2

MPICXX = mpicxx


INCLUDE_DIR = $(PARSER_DIR)/include
SRC_DIR = $(PARSER_DIR)/src
TEST_DIR = $(PARSER_DIR)/test


OBJ_DIR = $(PARSER_DIR)/obj
LIB_DIR = $(PARSER_DIR)/lib
BIN_DIR = $(PARSER_DIR)/bin

HEADER_PATH = /usr/local/include/ttlparser
LIB_PATH = /usr/local/lib


ALL_OBJS = $(OBJ_DIR)/blank_node_id_generator.o $(OBJ_DIR)/rdf_vocabulary.o $(OBJ_DIR)/xsd_vocabulary.o \
           $(OBJ_DIR)/turtle_lexer.o $(OBJ_DIR)/turtle_parser.o

TEST_OBJS = $(OBJ_DIR)/test_main.o $(OBJ_DIR)/turtle_parser_test.o


TARGET = $(LIB_DIR)/libttlparser.a


all: build


build: dirs $(TARGET)
	@ echo "Build finished."


test: dirs $(BIN_DIR)/ttltest
	@ echo "Tests starting..."
	@ $(BIN_DIR)/ttltest


install:
	sudo cp $(PARSER_DIR)/include/*.h $(HEADER_PATH)


clean:
	rm -f $(OBJ_DIR)/*.o $(LIB_DIR)/*.a $(LIB_DIR)/*.so $(BIN_DIR)/*


dirs:
	mkdir -p $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)



#Turtle Parser Library
$(LIB_DIR)/libttlparser.a: $(ALL_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/blank_node_id_generator.o: $(SRC_DIR)/blank_node_id_generator.h $(SRC_DIR)/blank_node_id_generator.cpp
	$(CXX) $(CPPFLAGS) -I$(SRC_DIR) $(CXXFLAGS) -c -o $@ $(SRC_DIR)/blank_node_id_generator.cpp

$(OBJ_DIR)/rdf_vocabulary.o: $(SRC_DIR)/rdf_vocabulary.h $(SRC_DIR)/rdf_vocabulary.cpp
	$(CXX) $(CPPFLAGS) -I$(SRC_DIR) $(CXXFLAGS) -c -o $@ $(SRC_DIR)/rdf_vocabulary.cpp

$(OBJ_DIR)/xsd_vocabulary.o: $(SRC_DIR)/xsd_vocabulary.h $(SRC_DIR)/xsd_vocabulary.cpp
	$(CXX) $(CPPFLAGS) -I$(SRC_DIR) $(CXXFLAGS) -c -o $@ $(SRC_DIR)/xsd_vocabulary.cpp

$(OBJ_DIR)/turtle_lexer.o: $(SRC_DIR)/turtle_lexer.h $(SRC_DIR)/turtle_lexer.cpp
	$(CXX) $(CPPFLAGS) -I$(SRC_DIR) $(CXXFLAGS) -c -o $@ $(SRC_DIR)/turtle_lexer.cpp

$(OBJ_DIR)/turtle_parser.o: $(INCLUDE_DIR)/turtle_parser.h $(SRC_DIR)/turtle_parser.cpp
	$(CXX) $(CPPFLAGS) -I$(SRC_DIR) $(CXXFLAGS) -c -o $@ $(SRC_DIR)/turtle_parser.cpp



#Turtle Parser Test
$(BIN_DIR)/ttltest: $(OBJ_DIR)/gtest-all.o $(TEST_OBJS) $(LIB_DIR)/libttlparser.a
	$(CXX) $(CPPFLAGS) -I$(SRC_DIR) $(CXXFLAGS) -lpthread -o $@ $^


$(OBJ_DIR)/test_main.o: $(TEST_DIR)/test_main.cpp
	$(CXX) $(CPPFLAGS) -I$(SRC_DIR) $(CXXFLAGS) -c -o $@ $(TEST_DIR)/test_main.cpp

$(OBJ_DIR)/turtle_parser_test.o: $(TEST_DIR)/turtle_parser_test.cpp
	$(CXX) $(CPPFLAGS) -I$(SRC_DIR) $(CXXFLAGS) -c -o $@ $(TEST_DIR)/turtle_parser_test.cpp



#Gtest

GTEST_DIR = $(PARSER_DIR)/third_party/googletest

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

GTEST_SRCS = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

$(OBJ_DIR)/gtest-all.o: $(GTEST_SRCS)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c -o $@ $(GTEST_DIR)/src/gtest-all.cc
