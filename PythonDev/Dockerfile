FROM python:3.11.2-alpine3.17

LABEL name       Python 3.11 with Numpy
LABEL maintainer Andrew Schellenberg

# As a defauly, Apline only comes with shell (`/bin/sh`). To use bash we must
# install it first. As a relatively non-project-specific change, we do this first so that 
# this layer this is cached for other python alpine images may use it.
RUN apk add --no-cache bash

# Required for compiling Numpy from source. Could remove after numpy install. See:
#   https://stackoverflow.com/questions/33421965/ 
RUN apk --no-cache add musl-dev linux-headers g++

# While we will mount the volume later, copying the requirements now is needed for installation.
COPY requirements.txt requirements.txt
RUN  pip install --upgrade pip && \
     pip install --no-cache-dir -r requirements.txt && \
     rm requirements.txt
