# Use an x86_64 base image
FROM debian:bullseye

# Update the package list and install necessary packages
RUN apt-get update && \
    apt-get install -y \
    g++ \
    valgrind \
    gcc \
    libc6-dev \
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Copy source files
COPY . /app
WORKDIR /app

# Build the application
RUN g++ -std=c++17 -Ofast -flto -march=native -fno-rtti -DNDEBUG prog1.cpp -o prog1

# Make the executable runnable
RUN chmod +x ./prog1

# Copy the script that waits for interaction
COPY startInternal.sh /startInternal.sh
RUN chmod +x /startInternal.sh

# Set the entrypoint to the script that will handle interaction
ENTRYPOINT ["/startInternal.sh"]
