all:
	@echo Parsing DDL
	@echo ******************************************
	@(cd gpio_config; make all)
	@(cd gpio_config; ./configure)
	@echo ******************************************
	@echo Building XINU
	@(cd compile; make clean)
	@(cd compile; make rebuild)
	@(cd compile; make)
	@echo Build Completed
	@echo ******************************************