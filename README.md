# bluepill-examples
Example code to use libopencm3 library with STM32F103 board

# Usage
## 1 clone this repo
```commandline
git clone git@github.com:themightyteam/bluepill-examples.git
cd bluepill-examples
```
## 2 Initialise submodules
In this case, it is just [libopencm3](https://github.com/libopencm3/libopencm3)
```commandline
git submodule init
git submodule update
```
## 3 build an example
```commandline
make build_image # Builds the docker image that contains the building software, do this just once
make examples/bluepill_test
```
Note that we are using a Docker image to contain the compiling/flashing stuff. The image is created using ./Dockerfile.

At any time, you can run an image with a terminal with `make console`.

## 4 flash an example


## How To: Messing with with libopencm3
```commandline
make build_image # Builds the docker image that contains the building software, do this just once
make console
mightyteam@c2ab9dbb25bc:/home/src$ cd libopencm3/
mightyteam@c2ab9dbb25bc:/home/src/libopencm3$ make clean
[...]
```
The next time you compile an example, libopencm3 will be compilled again.

# Related Projects
* our line follower firmware with extra instructions for compiling and flashing a bluepill board https://github.com/hmightypirate/AllMyCircuits
* for our line follower, we are using the chassis from https://gitlab.com/punkto/line_follower_3d_designs
* stm32loader.py: https://github.com/jsnyder/stm32loader
* libopencm3: https://github.com/libopencm3/libopencm3