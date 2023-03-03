# Basics

Docker is all about creating the minimum required environment for our application. Here, we demonstrate a small test project call `MyCoolApp`. It's a super cool application that you developed and, while its not finished yet, you'd like to test in in Docker.

## Dockerfile

The first step with Docker is a `Dockerfile`. Note that there is no extension on the file and that Docker is uppercase. This is a requirement for a Dockerfile to be valid.

We'll break down the Dockerfile line by line but here it is first:

```Dockerfile
FROM ubuntu:20.04

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
            git \
            build-essential

WORKDIR /MyContainer

COPY . .

ENTRYPOINT [ "ping" "www.google.com" "-i"  "5"]
```

Each line of a Dockerfile follows the format of `COMMAND ARGUMENT(S)`. And each line creates a new layer in your image. Essentially, you can think of layers as git commits. they describe what changed after each command was run. This format lets Docker cache layers and share them between image.

```Dockerfile
FROM ubuntu:20.04
```

The first command of a Dockerfile is always `FROM`, it tells Docker the starting point. While it's possible to build from scratch, more often we'll start from an existing operating system. These are well built and regularly tested.

In this example we start with [the official Ubuntu image](https://hub.docker.com/_/ubuntu) for 20.04

```Dockerfile
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
            git \
            build-essential \
            iputils-ping
```

The thing with these base images is they tend to be bare bones. So the first thing we want to do in this case is update our packages and then install some things we'll need in our container. `RUN` tells Docker to execute the commands that follow.

Since each creates a new layer and, generally, we don't want too many layers in our image, we execute a few commands on one line of text. The `&&` symbol lets us run commands back to back and the `\` character lets us split one command across multiple lines.

You might notice the package `iputils-ping`. That's right. We have to install ping if we want that command. You might be surprised what you take for granted in a full install. We practice and patience you'll figure out what basic tolls you need and how to install them.

If you aren't familiar with the `apt-get install` options, I'll explain them briefly:

- The `-y` flag automatically say yes to any prompts that `apt-get` might ask us (we can't interact with the container as it is built, so this ie pretty important);

- The `--no-install-recommends` flag tells the program that we *only* want [`git`](https://packages.ubuntu.com/focal/git), [`build-essential`](https://packages.ubuntu.com/focal/build-essential), [iputils-ping](https://packages.ubuntu.com/bionic/iputils-ping), and whatever packages they depend upon. This helps keep the size of our image relatively small. This isn't alway needed, but useful to know. You can follow the links to see what packages [`git`](https://packages.ubuntu.com/focal/git), [`build-essential`](https://packages.ubuntu.com/focal/build-essential), and [iputils-ping](https://packages.ubuntu.com/bionic/iputils-ping) list as requirements and suggestions.

```Dockerfile
WORKDIR /MyContainer
```

This line creates a new folder (if it doesn't exist) and sets it as out working directory. Any changes we make will be relative to this. For example...

```Dockerfile
COPY . .
```

The `COPY` command does exactly what you might think. It looks at the files in the directory the `Dockerfile` is in and copies them into the image.

Its worth point out another file now, the `.dockerignore` which lists what directories to skip in this step. While you're ready to test MyCoolApp, the code might change often. We don't want to copy that. However, the code libraries in the `Libs/` directory won't change much if at all. It's okay to copy those.

There are plenty more `Dockerfile` commands. You can view the documentation [here](https://docs.docker.com/engine/reference/builder/).

## Building our Image

A `Dockerfile` is just a set of instructions. To interact with a container we must ask Docker to build it. We do this with the build command:

```bash
docker build -t my-cool-app:0.1 .
```

The core of the command is `docker build`. This tells Docker what we'd like it to do. The flag `-t` means we want to give this image a name and a tag. A image is labeled with `NAME:TAG`. The tags help us differentiate between versions (e.g., `ubuntu:18.04` and `ubuntu:20.04`).

We need to give the image a name and tag that are lowercase. As a default the tag will be `latest`. But better practice is to give a meaningful tag, something like `alpha` might work since we are still testing our application but for fun we'll use label this vesion `0.1`. Ouf full name and tag is `my-cool-app:0.1`.

Finally, we need to tell Docker where the file is. I'm assuming you are in this cd into this directory so `.` indicates that the Dockerfile is right here. If you're in the root of this repo it we would tell it to search for the Dockerfile in `Basics` instead. Point is, Docker will find the Dockerfile.

After running the command you should see something:

```bash
[+] Building 0.7s (9/9) FINISHED
 => [internal] load build definition from Dockerfile                                          0.1s
 => => transferring dockerfile: 32B                                                           0.0s
 => [internal] load .dockerignore                                                             0.0s
 => => transferring context: 34B                                                              0.0s
 => [internal] load metadata for docker.io/library/ubuntu:20.04                               0.0s
 => [1/4] FROM docker.io/library/ubuntu:20.04                                                 0.0s
 => [internal] load build context                                                             0.1s
 => => transferring context: 8.08kB                                                           0.0s
 => CACHED [2/4] RUN apt-get update &&     apt-get install -y         git         build-esse  0.0s
 => CACHED [3/4] WORKDIR /MyContainer                                                         0.0s
 => [4/4] COPY . .                                                                            0.1s
 => exporting to image                                                                        0.1s
 => => exporting layers                                                                       0.1s
 => => writing image sha256:358c3f11dcffc2a4f2f70dc921b5af21ac8a6ccb75241cdafd49919e9e717ccd  0.0s
 => => naming to docker.io/library/my-cool-app:0.1                                            0.0s
 ```

There are some steps `[X/4]` showing each layer. You might notice the word `CACHED` a few times too. Again, this helps speed up building the image. You can see the time each step took on the right hand column. Your first build might take a minute, but this is the output building a second time, it was super fast because of all the caching.

You can view your new image (and any installed images) with:

```bash
docker image ls

REPOSITORY         TAG                        IMAGE ID       CREATED          SIZE
my-cool-app        0.1                        358c3f11dcff   4 minutes ago    413MB
```

Notice that our image has an image id. This is a SHA256 signature unique to our image. The name and tag are really just an alias for this long number. Using `my-cool-app:0.1` is much easier if you ask me.

## Run a Container

This is what we've been waiting for! It's time to run our container!

This most basic way to do this is:

```bash
docker container run my-cool-app:0.1
```

Amazing! it runs. But you might be disappointed because nothing seems to be happening. Fair enough. But trust me, something is happening.

```bash
docker container ls -a

CONTAINER ID   IMAGE              COMMAND       CREATED              STATUS                          PORTS     NAMES
af937d43f680   my-cool-app:0.1    "/bin/bash"   About a minute ago   Exited (0) About a minute ago             peaceful_greider
```

Okay! We see a few things here. There is a container from the `my-cool-app:01` image and it exited. We also notice this name `peaceful_greider` and a container ID `af937d43f680`. Just like our image name and tag alias a unique SHA256 key, the name is a easy way to reference the specific container. This name was randomly generated by Docker because we did not specify one.

Lets start an *interesting* container now.

```bash
docker container run --name CoolApp -it my-cool-app:0.1 /bin/bash
```

This time, we give the container a name `--name CoolApp`, request an interactive terminal `-it`, and ask it to run bash by providing the executable's location `/bin/bash`.

If successful you should find yourself in remote connection to the container! You can run some commands and see the outputs:

```bash
root@f266b8a53ef8:/MyContainer# ls
Dockerfile  Libs  README.md
root@f266b8a53ef8:/MyContainer# ls Libs
README.md  lib1.txt
root@f266b8a53ef8:/MyContainer#
root@f266b8a53ef8:/MyContainer# ps
  PID TTY          TIME CMD
    1 pts/0    00:00:00 bash
   15 pts/0    00:00:00 ps
root@f266b8a53ef8:/MyContainer# cd .. && ls
MyContainer  bin  boot  dev  etc  home  lib  lib32  lib64  libx32  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var
```

We notice first, we are in the `MyContainer` directory that we setup way back in the `Dockerfile`. We can list files with `ls` to view the files we copied (and verify that the `MyCoolApp` directory was not copied). We can print the current processes with `ps`. Notice how we only see `bash`, the process we are using, and the process `ps` itself. If you ran this on a full desktop installation of Ubuntu there'd be tens if not hundreds of processes running. When we talk about Docker being minimal, executing only what processes you specify is part of that. Finally, we can move up a directory and see the regular Linux root file system (and our working directory).

Its important to know that you are automatically the super user in Docker. (Notice that the Dockerfile didn't use `sudo` in its commands).

If you wanted to create a new container from this image you'd simply run the command above. Just give it a new name. The containers will not share files or processes. You can use `ps` or create a file in one with `touch file.txt` and verify that the processes and file systems are completely independent.

To leave the interactive terminal simply press `CTRL`+`P`+`Q`. The container will keep running. But you will be back in your terminal. If you leave you can verify that it is running with:

```bash
docker container ls
CONTAINER ID   IMAGE             COMMAND       CREATED          STATUS          PORTS     NAMES
f266b8a53ef8   my-cool-app:0.1   "/bin/bash"   12 minutes ago   Up 12 minutes             CoolApp
```

To reconnect to a terminal run

```bash
docker container exec -it CoolApp /bin/bash
```

You'll find yourself back in the  `MyContainer` directory.

## Stopping, Restarting and Removing

So that's pretty neat! Lets talk about a container lifetime. Right now you may have one or more containers running right now. Try the following commands and see if you get a similar output:

```bash
docker container stop CoolApp
CoolApp

docker container ls
CONTAINER ID   IMAGE     COMMAND   CREATED   STATUS    PORTS     NAMES

docker container ls
CONTAINER ID   IMAGE              COMMAND       CREATED          STATUS                        PORTS     NAMES
f266b8a53ef8   my-cool-app:0.1    "/bin/bash"   16 minutes ago   Exited (127) 10 seconds ago             CoolApp

docker container start CoolApp
CoolApp

docker container ls
CONTAINER ID   IMAGE             COMMAND       CREATED          STATUS         PORTS     NAMES
f266b8a53ef8   my-cool-app:0.1   "/bin/bash"   16 minutes ago   Up 5 seconds             CoolApp
```

The first command, `docker container stop [CONTAINER NAME]` stops the container. Multiple names can be provided at once and Docker echos the name(s) of the container(s) that we stopped. We can verify this by listing the containers with `docker container ls`. By default only running containers are shown. Since we just stopped our only container, Docker rightly shows nothing.

We can see that the container is stopped with `docker container ls -a`. The `-a` flag asks Docker for ALL containers on our system. So now we see that the `CoolApp` container was started 16 minutes ago but stopped just a few seconds ago.

A stopped container remembers its state. Any environment variables or local files are stashed away. Any running processes get a `SIGTERM` command from the operating system and have 10 seconds to shut down.

We can restart a container with `docker container start [CONTAINER NAME]`. Once again, we see the name echoed as confirmation. And `docker container ls` will once again show our container. Its worth mentioning that while data is saved, running applications might not be restarted. You can read more about automatic start policies, and specifically re-start policies in Dockers documentation [here](https://docs.docker.com/config/containers/start-containers-automatically/).

But what is we are done with a container. Like never want to start it again. What do we do? We remove it.

Removing deletes the container and any changes local to that container. A container must be stopped to be removed. But the remove command follows the same basic pattern as the `stop` and `start` ones. To see it in action we can run:

```bash
docker container stop CoolApp
CoolApp

docker container rm CoolApp
CoolApp

docker container ls -a
CONTAINER ID   IMAGE              COMMAND       CREATED          STATUS                      PORTS     NAMES
```

Now, even listing all of our containers does not show the `CoolApp` container. Although local changes are lost, the image is not affected and we can launch any new containers from this. This means if we mess something up inside our container, we can easily wipe everything, start from the image and try again.

## Volumes

It might be scary that removing a container deletes all changes made since the container was launched. But most of the time the image is made up of things that wont be changing. Our installed packages and files in the `Libs` directory aren't supposed to change much. So any data lost shouldn't be important.

But what if we do want some of that data? Maybe there are important log files for our app. Or perhaps we need to connect to a database. Or wait... we have our Cool App in the `MyCoolApp` and our `my-cool-app` containers don't have access to it!

Volumes some this data persistence problem. The let up mount folders on our host machine into any number of containers. Let's try it!

```bash
docker run --name CoolAppDev -v [EDIT ME: ABSOLUTE PATH ON YOUR MACHINE]:/MyCoolAppVolume -it my-cool-app:0.1

root@e85a2bfee219:/MyContainer# ls
Dockerfile  Libs  README.md
root@e85a2bfee219:/MyContainer# cd ..
root@e85a2bfee219:/# ls
MyContainer  MyCoolAppVolume  bin  boot  dev  etc  home  lib  lib32  lib64  libx32  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var
root@e85a2bfee219:/# ls MyCoolAppVolume/
MyCoolApp.txt
root@e85a2bfee219:/# more MyCoolAppVolume/MyCoolApp.txt
I promise this app will be awesome! Just you wait and see.
root@e85a2bfee219:/# echo " New updates are coming all the time" >> MyCoolAppVolume/MyCoolApp.txt
root@e85a2bfee219:/# more MyCoolAppVolume/MyCoolApp.txt
I promise this app will be awesome! Just you wait and see. New updates are coming all the time
```

This time we name the container `CoolAppDev` in case any of our past containers are running or stopped and to indicate that this one has a volume attached. In the command we use the `-v` flag to indicate that Docker should create a volume for this container. After the flag we must provide information about what host directory we want and where it should go in the container this is:`[ABSOLUTE PATH IN HOST]:[ABSOLUTE PATH IN GUEST]`.

We can move around in the container and see the new `MyCoolAppVolume/` with the `MyCoolApp.txt` inside of it. We can display the contents of this file with the `more` command. And the edit it with `echo` (the `>>` operator direct the output to the end of the file). Using `more` again we can confirm this change in the container. On your machine you can confirm that the changes are indeed made to the file on the host too.

Volumes are powerful. This is just the surface of a complex topic. Be aware that while containers can share volumes, there is nothing built into Docker or its volumes to prevent [race conditions](https://en.wikipedia.org/wiki/Race_condition#In_software). It is up to us to make sure our programs and containers share access to volumes responsibly and don't try to edit the same files at the same time.

## Wrap up

This whirlwind tour of Docker is just the beginning. There is so much more for each of these topics, Dockerfiles, images, containers, and volumes are as complex as they are powerful. Please visit the [Docker documentation website](https://docs.docker.com/) to continue learning.

One last topic to mention is docker-compose. You might have noticed the commands to start a container get a bit unwieldy with names, options, executables, and volumes. The docker-compose tool allows us to specify these things in a YAML file called `docker-compose.yml`. This lets us launch one, or many, containers with simpler commands. Learn more [here](https://docs.docker.com/compose/gettingstarted/).
