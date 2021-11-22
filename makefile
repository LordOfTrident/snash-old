# Names
N_APP = snash

# Files
F_SRC = \
	src/main.c\
	src/utils.c\
	src/snash.c

F_HEADER = \
	src/snash.h\
	src/utils.h\
	src/types.h\
	src/components.h

F_ALL = ${F_SRC} ${F_HEADER}

# Compiler related
C = gcc
C_VER = c99
C_FLAGS = \
	-O3\
	-std=${C_VER}\
	-I./src\
	-I./\
	-o ./bin/app\
	-lreadline\
	-g

compile: ${F_ALL}
	@mkdir -p ./bin
	@echo Created ./bin/

	@echo Compiling...
	@${C} ${F_SRC} ${C_FLAGS}
	@echo Compiled successfully

install:
	@echo Installing...
	@cp ./bin/app /usr/bin/${N_APP}
	@echo Installed successfully

clean:
	@echo Cleaning...
	@rm ./bin/app
	@echo Cleaned successfully

all:
	@echo compile - Compiles the source
	@echo install - Copies the binary in /usr/bin
	@echo clean - Removes built files