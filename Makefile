BIN_PATH=./bin

CSRC = $(wildcard *.c)
OBJ_NO_DIR = $(CSRC:%.c=%.o)
OBJ = $(patsubst %.c,$(BIN_PATH)/%.o,$(CSRC))

DOCKER_IMAGE_NAME = mighty-arm-development
DOCKER_TAG = 2023_11
DOCKER_IMAGE_NAME_WITH_TAG = $(DOCKER_IMAGE_NAME):$(DOCKER_TAG)

PORT = /dev/ttyUSB0
EXAMPLE = bluepill_test

UID = $(shell id -u)
GID = $(shell id -g)

build_image:
	sudo docker build --build-arg UID=${UID} \
		--build-arg GID=${GID} \
		-t $(DOCKER_IMAGE_NAME_WITH_TAG) .

rm_image:
	sudo docker rmi -f $(DOCKER_IMAGE_NAME_WITH_TAG)

console:
	sudo docker run -v $(shell pwd):/home/src/ \
		-ti \
		--rm \
<<<<<<< HEAD
		--device=$(PORT) \
		$(DOCKER_IMAGE_NAME_WITH_TAG)
=======
		$(DOCKER_IMAGE_NAME)
>>>>>>> master

EXAMPLES := $(sort $(wildcard examples/*))
FLASH := $(addsuffix _flash, $(EXAMPLES))

$(EXAMPLES): BINARY = $(subst examples/,,$@)
$(EXAMPLES):
	sudo docker run -v $(shell pwd):/home/src/ \
		-ti \
		--rm \
		$(DOCKER_IMAGE_NAME_WITH_TAG) \
		make -C $@ clean all BIN=$(BINARY)

$(FLASH): FOLDER = $(subst _flash,,$@)
$(FLASH): BINARY = $(subst examples/,,$(FOLDER))
$(FLASH):
	sudo docker run -v $(shell pwd):/home/src/ \
		-ti \
		--rm \
		--device=$(PORT) \
		$(DOCKER_IMAGE_NAME_WITH_TAG) \
		stm32loader -p $(PORT) -e -w -V -g 0x08000000 -v $(FOLDER)/$(BINARY).bin

.PHONY: $(EXAMPLES) $(FLASH)

help:
	@printf '\033[33mBase targets:\033[0m\n'
	@printf '\033[36m    %-25s\033[0m %s\n' 'build_image' 'Generate Docker development image. Includes ARM GCC toolchain and stm32loader.'
	@printf '\033[36m    %-25s\033[0m %s\n' 'rm_image' 'Delete Docker development image.'
	@printf '\033[36m    %-25s\033[0m %s\n' 'console' 'Run Docker container with an interactive bash terminal'
	@printf '\n'
	@printf '\033[33mBluepill Examples:\033[0m'
	@printf '\033[0m	  Targets to build each example. Add \033[36m"_flash"\033[0m suffix to load the corresponding example to the board connected to PORT (default: /dev/ttyUSB0)\n' 
	@for v in $(EXAMPLES) ; do \
		printf '\033[36m    %s\033[0m\n' $$v; \
    done

.DEFAULT_GOAL := help
