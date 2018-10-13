FROM debian:sid-slim

RUN apt-get update && \
    apt-get install -y clang-format

ENTRYPOINT ["clang-format"]
CMD ["--help"]
