FROM buildpack-deps:stretch AS build-env

RUN set -ex; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
            tofrodos zip mingw-w64;            \
    rm -rf /var/lib/apt/lists/*

COPY src /root/

RUN set -ex; \
    cd /root; \
    make zip


FROM busybox

COPY --from=build-env /root/wsl-toyc.zip /
