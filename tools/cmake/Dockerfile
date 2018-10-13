FROM debian:sid-slim

RUN apt-get update && \
    apt-get install -y build-essential && \
    apt-get install -y cmake

# USER 1000:100

# ENTRYPOINT ["cmake"]
# CMD ["-DCMAKE_EXPORT_COMPILE_COMMANDS=ON", ".."]

ENTRYPOINT ["/bin/sh"]
