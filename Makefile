SRC_DIR = ./src
INC_DIR = ./src
OBJ_DIR = ./obj
EXE_DIR = ./bin

VENDOR_INCLUDE_DIR = ./vendor/include

CXX ?= g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++17 -O -I$(INC_DIR) -isystem $(VENDOR_INCLUDE_DIR)
LDFLAGS = -Wall -Wextra -Werror -pedantic

EXES = portfolio_optimizer
LIST = $(addprefix $(EXE_DIR)/, $(EXES))

all: $(LIST)

PORTFOLIO_OPTIMIZER_DEPS = main.o optimizer.o methods/crank_nicolson.o methods/explicit_euler.o methods/implicit_euler.o output/matlab_m_output.o output/stdout_output.o
$(EXE_DIR)/portfolio_optimizer: $(addprefix $(OBJ_DIR)/, $(PORTFOLIO_OPTIMIZER_DEPS))
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp init dep
	$(CXX) -o $@ -c $< $(CXXFLAGS)

# dependencies
dep: $(VENDOR_INCLUDE_DIR)/Eigen $(VENDOR_INCLUDE_DIR)/tclap

$(VENDOR_INCLUDE_DIR)/Eigen:
	@mkdir -p $(VENDOR_INCLUDE_DIR) && ./scripts/install_eigen.sh $(VENDOR_INCLUDE_DIR)

$(VENDOR_INCLUDE_DIR)/tclap:
	@mkdir -p $(VENDOR_INCLUDE_DIR) && ./scripts/install_tclap.sh $(VENDOR_INCLUDE_DIR)

init:
	@mkdir -p $(EXE_DIR) $(OBJ_DIR)
	cd $(SRC_DIR) && find . -type d -exec mkdir -p ../obj/{} \;

.PHONY: clean

clean:
	rm -rf $(EXE_DIR) $(OBJ_DIR)