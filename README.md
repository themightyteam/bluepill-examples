# bluepill-examples
Example code to use libopencm3 library with STM32F103 board


# Compilation notes.
## "Unterminated quoted string" error.
Using the "new", bullseye based Dockerfile to compile the current version of libopencm3 gives an error like this:
```
  GENHDR  include/libopencm3/stm32/l0/irq.json
  GENHDR  include/libopencm3/stm32/l1/irq.json
  GENHDR  include/libopencm3/stm32/l4/irq.json
  BUILD   lib/stm32/f1
/bin/sh: 1: Syntax error: Unterminated quoted string
make[1]: *** [Makefile:68: lib/stm32/f1] Error 2
make[1]: Leaving directory '/home/src/libopencm3'
make: *** [../../rules.mk:43: libopencm3] Error 2
make: Leaving directory '/home/src/examples/buzzer_music'
make: *** [Makefile:37: examples/buzzer_music] Error 2
```

This can be fixed using the fix in https://github.com/nanovna-v2/NanoVNA2-firmware/issues/73#issue-808022410 , that is, open ./libopencm3/Makefile and change the line 28 from
```
SRCLIBDIR:= $(subst $(space),\$(space),$(realpath lib))
```
to
```
SRCLIBDIR:= $(subst $(space),$(space),$(realpath lib))
```

This does not happen with newer versions of libopencm3 that will be merged soon to main.