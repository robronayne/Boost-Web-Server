### Build/test coverage report container ###
# Define builder stage
FROM ngineers:base as builder

# Share work directory
COPY . /usr/src/project
WORKDIR /usr/src/project/build

# Build targets/binaries
RUN cmake ..
RUN make

WORKDIR /usr/src/project/build_coverage

# Build and generate coverage report
RUN cmake -DCMAKE_BUILD_TYPE=Coverage ..
RUN make coverage
