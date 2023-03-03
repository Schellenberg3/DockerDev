# PythonDev

This is a super small demonstration of how to use Docker for C++ development. The directory `project` contains a small Python module that prints what operating system it runs on and does some simple math with the NumPy library.

This container uses the [official Python contain on Alpine linux](https://hub.docker.com/_/python). And shows how sometimes aiming for small size may make setting up our environment difficult, but possible.

```Dockerfile
FROM python:3.11.2-alpine3.17

LABEL name       Python 3.11 with Numpy
LABEL maintainer Andrew Schellenberg

RUN apk add --no-cache bash

# Required for compiling Numpy from source. Could remove after numpy install. See:
#   https://stackoverflow.com/questions/33421965/ 
RUN apk --no-cache add musl-dev linux-headers g++

COPY requirements.txt requirements.txt
RUN  pip install --upgrade pip && \
     pip install --no-cache-dir -r requirements.txt && \
     rm requirements.txt
```

We start, as always, with out `FROM` command. This time we use the Python image for Python 3.11 installed on a base of Alpine 3.17. Alpine is a linux distribution focused on being super lightweight. Compared to the ubuntu image, a respectably compact 80 MB, the Alpine image is a tiny 7 MB. It's amazing an operating system can be so small!

Alpine achieves this by shedding anything that isn't absolutely necessary. In fact, we have to install bash with `RUN apk add --no-cache bash`.

Now, intuition says that for the Python alpine image we would just skip straight to the lines to copy `requirements.txt` and run `pip install -r requirements.txt`. But here's the thing, the install will fail with a crazy command like:

```shell
error: Command "gcc -fno-strict-aliasing -Os -fomit-frame-pointer -DNDEBUG -Os -fomit-frame-pointer -fPIC -Inumpy/core/include -Ibuild/src.linux-x86_64-2.7/numpy/core/include/numpy -Inumpy/core/src/private -Inumpy/core/src -Inumpy/core -Inumpy/core/src/npymath -Inumpy/core/src/multiarray -Inumpy/core/src/umath -Inumpy/core/src/npysort -I/usr/include/python2.7 -Ibuild/src.linux-x86_64-2.7/numpy/core/src/private -Ibuild/src.linux-x86_64-2.7/numpy/core/src/private -Ibuild/src.linux-x86_64-2.7/numpy/core/src/private -c build/src.linux-x86_64-2.7/numpy/core/src/npymath/ieee754.c -o build/temp.linux-x86_64-2.7/build/src.linux-x86_64-2.7/numpy/core/src/npymath/ieee754.o" failed with exit status 1
```

Essentially, `pip` would like to build NumPy's C++ backend but doesn't have the tools to do so! So we install these with `apk --no-cache add musl-dev linux-headers g++`. See this [Stack Overflow](https://stackoverflow.com/questions/33421965/) question if you are curious.

Then we copy our `requirements.txt` and upgrade pip then install NumPy with `pip install --upgrade pip && pip install --no-cache-dir -r requirements.txt && rm requirements.txt`. Note that because the image is built, we don't really need to `requirements.txt`, so I opted to remove it.

## Image Size

We have a few options for what base image we use for Python. But we opted to use the small Alpine base. However, our project requires NumPy. Its a great library. But it is huge.

The base Alpine library is just 7 MB. The python:alpine image we stated from in our Dockerfile increases this to 43 MB with everything it installs. But with our NumPy installation this balloons to a whopping 455 MB.

This is okay. There's nothing wrong with the size of this image. But don't be fooled that starting from Alpine will keep things small or that one library couldn't require that much space.

There are likely way to install the NumPy build dependencies so they take less space. Or remove them after we install NumPy. Or perhaps we could try the Alpine community NumPy Package [here](https://pkgs.alpinelinux.org/packages?name=*numpy).

What I did was simply the first and easiest solution that I could find. Perhaps you could try making this image smaller!

## Build, Run, Execute

If you've followed these in order you should be capable of building and running containers now. Click the spoilers if you need a hit

Build:
>! docker build -t python-dev:0.1 .

Run:
>! docker run --name PythonDev -it -v $(pwd):/PythonDev python-dev:0.1 /bin/bash

Execute:
>!python /project/main.py
