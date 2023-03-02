# Start from a "Docker Official" base image.
# If it is not installed locally, this will find ubuntu and identify the image
# with the tag `20.04`.
FROM ubuntu:20.04

# Update packages and install git.
RUN apt-get update && \
    apt-get install -y \
        git \
        build-essential \
        iputils-ping

# Create a directory within the container
WORKDIR /MyContainer

# Embeds a copy of the local files, excluding those in our .dockerignore
# file. If we change these files we would need to re-build the image.  
COPY . .
