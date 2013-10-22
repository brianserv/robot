include ../fastframe/Makefile_public

ROBOT_DIR = $(BASE_DIR)/robot

ROBOT_INCLUDE_DIR = $(ROBOT_DIR)

LOGICMOD_DIR = logic

ROBOT_OBJ_DIR = $(ROBOT_DIR)/.objs

ROBOT_LDFLAGS = $(LDFLAGS) -ldl -lpthread

TARGET	= $(DEBUG_TARGET)

DEBUG_TARGET = $(BIN_DIR)/robot$(BIN_SUFFIX)

ROBOT_SRC = $(wildcard *.cpp)
LOGICMOD_SRC = $(wildcard $(LOGICMOD_DIR)/*.cpp)

ROBOT_OBJS = $(addprefix $(ROBOT_OBJ_DIR)/, $(subst .cpp,.o,$(ROBOT_SRC)))

OBJS = $(wildcard $(ROBOT_OBJ_DIR)/*.o)

INC = -I$(FASTFRAME_INCLUDE_DIR) -I$(ROBOT_INCLUDE_DIR) -I$(MYSQL_INCLUDE_DIR)

all : $(TARGET)

$(TARGET) : $(ROBOT_OBJS) COMMON FASTFRAME LOGICMOD
	$(CXX)  -o $@ $(OBJS) $(ROBOT_LDFLAGS)
$(ROBOT_OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@
COMMON:
	cd $(COMMON_DIR); make
LOGICMOD:
	cd $(LOGICMOD_DIR); make
FASTFRAME:
	cd $(FASTFRAME_DIR); make

clean: 
	cd $(COMMON_DIR); make clean;
	cd $(LOGICMOD_DIR); make clean;
	cd $(FASTFRAME_DIR); make clean;
	rm -f $(OBJS) $(TARGET)


