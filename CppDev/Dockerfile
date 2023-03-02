FROM gcc:12.2.0-bullseye

# Our demo requires eigen. Note that we do need to apt-get update
# before we can install Eigen3
RUN apt-get update && \
    apt-get install -y libeigen3-dev

# While we could copy our files, it'll be easier to edit if we use volumes
# COPY . . 