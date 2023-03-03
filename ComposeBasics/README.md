# Compose Basics

Inside this tutorial director we revisit the generic `MyCoolApp` that was used to demonstrate the fundamentals of Docker inside the [Basics directory](../Basics). We now uses this same project to explore the essentials of docker-compose. Here, this tool is used to build our Dockerfiles, launch multiple containers, create and attach volumes with one command that operates from a file called `docker-compose.yaml`

As we scale up our applications we may find it easier to break a complex service into many micro-services. Or we may find that the docker command to launch a single development container becomes cumbersome to type and prone to misspellings or simple syntactical mistakes.

Docker Compose is a separate from the Docker client and engine that we have been using. At its core it is a Python based tool which reads our `docker-compose.yaml` file and then runs the appropriate Docker commands for us.

While it is an independent tool, it is integrated into docker is is likely installed already if you have Docker. Try `docker-compose --version` to check if it is installed. Note that the `-` in `docker-compose` is optional. Just leaving a space (e.g., `docker compose` then your command) is perfectly valid too. It is just an alias within Docker for the `docker-compose` tool.

## Docker Compose YAML

The `docker-compose.yaml` is a declarative configuration file. It outlines, among many other things, what containers we would like, what volumes should be connected, and how networks should be set up.

Our `docker-compose.yaml` is extremely simple. It creates one container and establishes one volume:

```yaml
version: '3.8'

services:
  cool-app:
    build: .
    command: ping localhost -i 5
    volumes:
      - .\MyCoolApp:/MyCoolApp
```

Let's break it down.

Docker Compose requires `version` as the first top-level key. This ensures that Docker Compose will parse things correctly. Any current version above 3 should be fine for most of our projects.

The next top level key is `services`. The noun here, service, is closely related to a container: Every container runs a service but we can also have multiple containers running the same service. You can read some more [here](https://stackoverflow.com/questions/35565770/).

In our case, we only want one service which will be named `cool-app`. Underneath this declaration, we describe that we want to with the following keys:

- `build` will tell docker an image from the Dockerfile at `.`. the local directory. This file is slightly modified from the one shown in `Basic` but is still relatively simple. This new container automatically named based on the directory the `docker-compose.yaml` file is in and the name of the service. In this case it is called `composebasics-cool-app`.
- `command` describes what the service should do. In this case the container(s) spawned by this service will ping the local hose (`127.0.0.1`) every 5 seconds. Note that this may be redundant as we can use `CMD` and `ENTRYPOINT` inside our Dockerfile to do this too.
- `volumes` establishes a simple relative volume that is exclusive to this service's container. There are many ways to set up a volume in a with Docker Compose, this is just the simplest. For example, a under a new top level key called `volumes` we could describe volumes that services could share.

We could of course do this with the command `docker run --name cool_app -v $(pwd)/MyCoolApp:/MyCoolApp my-cool-app:0.1`. But, with this file we only need to run:

```bash
docker-compose up
```

This will launch the services we requested in the `docker-compose.yaml` file. The first time this is run you should see output dialog from Docker building a new image. Then the terminal will start printing the results of our `ping`. Pressing `CTRL` + `C` will tell Docker Compose to shut everything down.

If we wanted to retain control of out terminal we can add a flag `-d` to the end of this command to return control of the terminal to us. Try it out: `docker-compose up -d`.

Docker Compose may not notice some of the changes in the directory after the image is built for the first time. If you add new files to the `Libs` directory or modify the `Dockerfile` or `.dockerignore` the image may not be rebuilt. To force this to happen you can use the flag `--build` and even `--force-recreate` if it is needed.

We can use the Docker Compose commands `stop` or `start` to pause and resume our composed collection of services as needed. This is conceptually the same as these commands for Docker containers. We can even use `rm` to remove all composed containers at once. Or the command `down` will pause and then remove for us.

While trying these commands you should see something like:

```bash
docker-compose up -d --build --force-recreate
[+] Building 0.5s (9/9) FINISHED
 => [internal] load build definition from Dockerfile                                         0.0s
 => => transferring dockerfile: 32B                                                          0.0s
 => [internal] load .dockerignore                                                            0.0s
 => => transferring context: 35B                                                             0.0s
 => [internal] load metadata for docker.io/library/ubuntu:20.04                              0.4s
 => [1/4] FROM docker.io/library/ubuntu:20.04@sha256:9fa30fcef427e5e88c76bc41ad37b7cc573e1d  0.0s
 => [internal] load build context                                                            0.0s
 => => transferring context: 90B                                                             0.0s
 => CACHED [2/4] RUN apt-get update &&     apt-get install -y         git         build-ess  0.0s
 => CACHED [3/4] WORKDIR /MyCoolLibs                                                         0.0s
 => CACHED [4/4] COPY . .                                                                    0.0s
 => exporting to image                                                                       0.0s
 => => exporting layers                                                                      0.0s
 => => writing image sha256:09901613330f28a7dfd4391dae197d08b40c8b6e7f337d186ce271a6677bb2f  0.0s
 => => naming to docker.io/library/composebasics-cool-app                                    0.0s

Use 'docker scan' to run Snyk tests against images to find vulnerabilities and learn how to fix them
[+] Running 2/2
 - Network composebasics_default       Created                                               0.1s
 - Container composebasics-cool-app-1  Started                                               0.5s

docker-compose down
[+] Running 2/2
 - Container composebasics-cool-app-1  Removed                                              10.3s
 - Network composebasics_default       Removed                                               0.1s
```

## Closing Thoughts

There are a lot of complex things that may be achieved with Docker Compose. It's a great tool to help manage complex and interconnected processes and is itself a complex tool. The demonstration here is oversimplified but should help you get started with the key commands of Docker Compose and the format of the `docker-compose.yaml` file.

To continue learning checkout the [Docker Compose Docs](https://docs.docker.com/compose/features-uses/) or the [`docker-compose.yaml` Reference Specifications](https://docs.docker.com/compose/compose-file/).

Docker Compose launches containers is great for starting up a number of complex containers on *one host*. But it is a jumping off point for multi-host orchestration tools. Docker Swarm and Kubernetes are two competing tools that distribute, scale, and coordinate how containers and services operate across many computers. These tools, and others, are the foundation for much of what people refer to as "cloud computing".
