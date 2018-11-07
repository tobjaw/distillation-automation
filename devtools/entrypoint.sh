#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

CLI_CMD=${1:-}


info_display() {
	echo "avr-devtools"
  echo "common utilities to work with the distillation-automation software."
}

avr_gcc_invocation() {
    avr-gcc \
        -pass-exit-codes \
        -Wall \
        -Werror \
        -mmcu=atmega2560 \
        -DF_CPU=16000000 \
        -DBAUD=9600 \
        $@
}

artifacts_delete() {
    echo "+++ Delete old build artifacts..."
    rm -f "${DIR_BUILD_DOCKER}/*"

    mkdir -p "${DIR_BUILD_DOCKER}"
}

build() {
    artifacts_delete;

    echo "+++ Building binary from ${DIR_SRC_DOCKER}/${FILE_SRC_IN}..."

    gcc \
        -pass-exit-codes \
        -Wall \
        -Werror \
        -o ${DIR_BUILD_DOCKER}/${FILE_BUILD_OUT} \
        ${DIR_SRC_DOCKER}/*.c

    echo "Success!"
}

cmake_cmds() {
    echo "+++ Run cmake"
    rm -rf ${DIR_BUILD_DOCKER}/*

    cd ${DIR_BUILD_DOCKER} && \
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
    
    cp compile_commands.json ..
}

run() {
    echo "+++ Run ${DIR_BUILD_DOCKER}/${FILE_BUILD_OUT}..."
    ${DIR_BUILD_DOCKER}/${FILE_BUILD_OUT}
}

build_avr() {
    artifacts_delete;

    echo "+++ Build object files..."
    for FILE_PATH in ${DIR_SRC_DOCKER}/*.c; do
        [ -f "${FILE_PATH}" ] || break
        FILE_NAME=$(basename -- "${FILE_PATH}")
        FILE_NAME_NO_EXT="${FILE_NAME%.*}"
        avr_gcc_invocation \
            -Os \
            -c "${FILE_PATH}" \
            -o "${DIR_BUILD_DOCKER}/${FILE_NAME_NO_EXT}.o"

    done

    echo "+++ Link object files into .elf..."
    avr_gcc_invocation \
        -Wl,-u,vfprintf \
        -lprintf_flt \
        "${DIR_BUILD_DOCKER}/*.o" \
        -o "${DIR_BUILD_DOCKER}/${FILE_BUILD_OUT}.elf"

    echo "+++ Copy to .hex..."
    avr-objcopy \
        -v \
        -O ihex \
        "${DIR_BUILD_DOCKER}/${FILE_BUILD_OUT}.elf" \
        "${DIR_BUILD_DOCKER}/${FILE_BUILD_OUT}.hex"

    echo "Success!"
}

flash_avr() {
    echo "Flashing currently does not work from inside the container."
    echo "Make sure avrdude is installed, the JTAG debugger is attached and run:"
    echo -e "\tavrdude -v -c jtag3 -p m2560 -U flash:w:${DIR_BUILD_HOST}/${FILE_BUILD_OUT}.hex"
}

debug_display() {
    echo "contents of ${DIR_BUILD_DOCKER}:"
    ls -al ${DIR_BUILD_DOCKER};

    echo "contents of ${DIR_SRC_DOCKER}:"
    ls -al ${DIR_SRC_DOCKER};

    echo "macros defined by gcc:"
    echo | gcc -E -dM -

    echo "macros defined by avr-gcc:"
    echo | avr_gcc_invocation -E -dM -
}

clang_format() {
    echo "--- formatting src"
    clang-format -i ${DIR_SRC_DOCKER}/*.[ch]
}

console_attach() {
    echo "Connecting to the console currently does not work from inside the container."
    echo "Use a command similar to:"
    echo -e "\tsudo cu -l /dev/cu.usbserial-AK05A8A6 -s 9600"
}

help_display() {
    echo -e "Usage:\n"
    echo -e "help\t display usage information"
    echo -e "debug\t display debug information"
    echo -e "build\t build binary for container"
    echo -e "build-avr\t build binary for ATmega2560"
    echo -e "run\t run container binary"
    echo -e "flash\t print avr flashing invocation"
    echo -e "format\t check code for formatting"
    echo -e "cmake\t build compile_commands.json"
    echo -e "console\t print console connection info"
}

if [ "${CLI_CMD}" = 'help' ]; then
    info_display;
    help_display;
elif [ "${CLI_CMD}" = 'debug' ]; then
    debug_display;
elif [ "${CLI_CMD}" = 'run' ]; then
    run;
elif [ "${CLI_CMD}" = 'build' ]; then
    build;
elif [ "${CLI_CMD}" = 'build-avr' ]; then
    build_avr;
elif [ "${CLI_CMD}" = 'flash' ]; then
    flash_avr;
elif [ "${CLI_CMD}" = 'cmake' ]; then
    cmake_cmds;
elif [ "${CLI_CMD}" = 'format' ]; then
    clang_format;
elif [ "${CLI_CMD}" = 'console' ]; then
    console_attach;
else
    echo "Unknown command \"${CLI_CMD}\"."
    help_display;
fi
