SRC_DIR = ./src
INC_DIR = ./src
OBJ_DIR = ./obj
EXE_DIR = ./bin

VENDOR_DIR = ./vendor
VENDOR_INCLUDE_DIR = $(addsuffix /include, $(VENDOR_DIR))

CXX ?= g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++17 -O -I$(INC_DIR) -isystem $(VENDOR_INCLUDE_DIR)
LDFLAGS = -Wall -Wextra -Werror -pedantic

EXES = portfolio_optimizer
LIST = $(addprefix $(EXE_DIR)/, $(EXES))

all: $(LIST)

PORTFOLIO_OPTIMIZER_DEPS = main.o optimizer.o methods/crank_nicolson.o methods/explicit_euler.o methods/implicit_euler.o output/matlab_m_output.o output/stdout_output.o
$(EXE_DIR)/portfolio_optimizer: $(addprefix $(OBJ_DIR)/, $(PORTFOLIO_OPTIMIZER_DEPS))
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp .initialized $(VENDOR_DIR)/.sentinel
	$(CXX) -o $@ -c $< $(CXXFLAGS)

# dependencies
INCLUDE_DEPS = Eigen tclap
dep:  $(VENDOR_DIR)/.sentinel

$(VENDOR_DIR)/.sentinel: $(addsuffix /.sentinel, $(addprefix $(VENDOR_INCLUDE_DIR)/, $(INCLUDE_DEPS)))
	touch $@

$(VENDOR_INCLUDE_DIR)/%: $(VENDOR_INCLUDE_DIR)/%/.sentinel

$(VENDOR_INCLUDE_DIR)/%/.sentinel:
	$(eval LOWERCASE_NAME := $(shell echo $(shell basename $(@D)) | tr '[:upper:]' '[:lower:]'))
	@mkdir -p $(VENDOR_INCLUDE_DIR)
	./scripts/install_$(LOWERCASE_NAME).sh $(VENDOR_INCLUDE_DIR)
	touch $@

init: .initialized

.initialized:
	@mkdir -p $(EXE_DIR) $(OBJ_DIR)
	cd $(SRC_DIR) && find . -type d -exec mkdir -p ../obj/{} \;
	touch .initialized

.PHONY: clean

clean:
	rm -rf $(EXE_DIR) $(OBJ_DIR) .initialized