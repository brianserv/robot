include ../../fastframe/Makefile_public

ROBOT_DIR = $(BASE_DIR)/robot

ROBOT_INCLUDE_DIR = $(ROBOT_DIR)

ROBOT_OBJ_DIR = $(ROBOT_DIR)/.objs

SRC = $(wildcard *.cpp)
OBJS = $(addprefix $(ROBOT_OBJ_DIR)/, $(subst .cpp,.o,$(SRC)))

INC = -I$(FASTFRAME_INCLUDE_DIR) -I$(ROBOT_INCLUDE_DIR) -I$(MYSQL_INCLUDE_DIR)

all : $(OBJS)

$(ROBOT_OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS)
