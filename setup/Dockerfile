FROM alpine:latest

COPY init_vm.sh /init.sh

RUN apk add --no-cache \
    openrc util-linux build-base busybox-openrc \
    && chmod +x /init.sh