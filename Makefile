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

PORTFOLIO_OPTIMIZER_DEPS = main.o
$(EXE_DIR)/portfolio_optimizer: $(addprefix $(OBJ_DIR)/, $(PORTFOLIO_OPTIMIZER_DEPS))
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp init
	$(CXX) -o $@ -c $< $(CXXFLAGS)

init:
	@mkdir -p $(EXE_DIR) $(OBJ_DIR)

dep:
	@mkdir -p $(VENDOR_INCLUDE_DIR) && ./scripts/install_eigen.sh $(VENDOR_INCLUDE_DIR)

.PHONY: clean

clean:
	rm -rf $(EXE_DIR) $(OBJ_DIR)