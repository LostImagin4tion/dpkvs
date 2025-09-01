FROM alpine:3.22.1 AS builder

# ====== System dependencies ======
RUN apk add --no-cache \
    build-base \
    linux-headers \
    cmake \
    git \
    pkgconfig

# ====== Copy source code ======
WORKDIR /app

COPY . .

# ====== Build the project ======
RUN --mount=type=cache,target=/app/build/_deps \
    --mount=type=cache,target=/root/.cache \
    mkdir -p build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release \
             -DCMAKE_CXX_STANDARD=23 \
             -DSPDLOG_BUILD_EXAMPLES=OFF \
             -DSPDLOG_BUILD_TESTS=OFF \
             -DSPDLOG_BUILD_INSTALL=ON

RUN --mount=type=cache,target=/app/build/_deps \
    --mount=type=cache,target=/root/.cache \
    cmake --build build --target dpkvs_server -j $(nproc)

# ====== Runtime stage ======
FROM alpine:3.22.1 AS runtime

# ====== Runtime dependencies ======
RUN apk add --no-cache \
    libstdc++ \
    libgcc

# ===== Copy built binary ======
WORKDIR /app

COPY --from=builder /app/build/dpkvs_server /app/
COPY --from=builder /app/configs/features /app/configs/features

CMD ["./dpkvs_server"]
