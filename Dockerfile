FROM --platform=$BUILDPLATFORM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN rm -rf build/CMakeCache.txt build/CMakeFiles
RUN cmake -S . -B build && cmake --build build

FROM alpine:latest AS runner
WORKDIR /app
COPY --from=builder /app/build/bin/sandpile-model ./sandpile-model

ENTRYPOINT ["./sandpile-model"]