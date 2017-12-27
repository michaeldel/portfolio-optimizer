VENDOR_INCLUDE_DIR = ./vendor/include

dep:
	@mkdir -p $(VENDOR_INCLUDE_DIR) && ./scripts/install_eigen.sh $(VENDOR_INCLUDE_DIR)