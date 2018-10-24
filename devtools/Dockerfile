FROM alpine

RUN apk --no-cache add \
  bash \
  build-base \
  gcc-avr \
  avr-libc \
  cmake \
  clang \
  git

WORKDIR /workdir

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT [ "/entrypoint.sh" ]

CMD [ "help" ]
