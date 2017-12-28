SRC_DIR = ./src
INC_DIR = ./src
OBJ_DIR = ./obj
EXE_DIR = ./bin

VENDOR_INCLUDE_DIR = ./vendor/include

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -O -I$(INC_DIR) -I$(VENDOR_INCLUDE_DIR)
LDFLAGS = -Wall -Wextra -Werror -pedantic

EXES = portfolio_optimizer
LIST = $(addprefix $(EXE_DIR)/, $(EXES))

all: $(LIST)

PORTFOLIO_OPTIMIZER_DEPS = main.o methods.o
$(EXE_DIR)/portfolio_optimizer: $(addprefix $(OBJ_DIR)/, $(PORTFOLIO_OPTIMIZER_DEPS))
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp init dep
	$(CXX) -o $@ -c $< $(CXXFLAGS)

# dependencies
dep: $(VENDOR_INCLUDE_DIR)/Eigen

$(VENDOR_INCLUDE_DIR)/Eigen:
	@mkdir -p $(VENDOR_INCLUDE_DIR) && ./scripts/install_eigen.sh $(VENDOR_INCLUDE_DIR)

init:
	@mkdir -p $(EXE_DIR) $(OBJ_DIR)

.PHONY: clean

clean:
	rm -rf $(EXE_DIR) $(OBJ_DIR)