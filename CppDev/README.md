# CppDev

This is a super small demonstration of how to use Docker for C++ development. The file `build_me.cpp` contains a small function that returns what platform it was compiled on and does some simple math with the Eigen3 linear algebra library.

Our Dockerfile is super short:

```Dockerfile
FROM gcc:12.2.0-bullseye

RUN apt-get update && \
    apt-get install -y libeigen3-dev
```

We pull from the [GNU Compiler Collection (GCC) Docker image](https://hub.docker.com/_/gcc) for version 12 of GCC on top of Debian bullseye. This is similar to Ubuntu for those who are more familiar with that operating system.

We then update our repositories source list with `apt-get update`, without this we would not be able to find the library for Eigen3. We install this dependency with `apt-get install -y libeigen3-dev`.

And that's it! It's nice that GCC has almost all the tools we need. Things like git are installed (you can verify this with `which git` which will return a path to the git executable). But still some basic non-development tools, like ping are not available (try running it or running `which ping` to see that there is no executable). So even though some things are included we still need to be aware that everything might not be there.

## Building and Running

Let's build and run this!

```shell
docker build -t cpp-dev:0.1 .
```

When we rub this, lets skip straight into using a volume so we can edit the file and re-compile as we see fit. To help save some typing we can use the shortcuts `%cd%` in Windows or `$(pwd)` Linux to print our current directories. This makes declaring a volume with the `docker run` command much easier!

On Windows run the container with:

```cmd
docker run --name CppDev -it -v %cd%:/CppDevRemote cpp-dev:0.1
```

On Linux run the container with:

```cmd
docker run --name CppDev -it -v %cd%:/CppDevRemote cpp-dev:0.1
```

## Compiling

So lets compile this program! We will use `g++`, the GNU C++ compiler to put our program together. We need to include Eigen3. Since its a header only library we simply need to specify the include path with `-I /usr/include/eigen3`. We then state what file to compile, `build_me.cpp`, and specify the output file name with `-o linux_build`.

 We can immediately run another command on this line with `&&` and `./linux_build` will run our program!

In your container, build and run our executable with:

```shell
g++ -I /usr/include/eigen3 build_me.cpp -o linux_build && ./linux_build 
```

The output should look lile

```shell
Project compiled on a Linux operating system!
The result of multiplying      0    1.5 -9.456 by 2 is       0       3 -18.912
```

Neat! We see that the executable was compiled on Linux and we see the expected output. Its a bit messy. So, on out host, add some newline characters (`\n`) on line 34 the recompile and run to make the out look like:

```shell
The result of multiplying
     0    1.5 -9.456
 by 2 is
      0       3 -18.912
```

It shouldn't shock us to see these edits in our containers rebuilt executable. That's what volumes are for! We should see the `linux_build` file on our host too.

If you're on Windows with GCC installed and the Eigen3 library installed we can compile a Windows version too! I won't cover how to install either but the links on each should help get you started.

In the `CppDev` folder on our windows host, we can run nearly the same command to compile `build_me.cpp` for windows:

```cmd
g++ -I C:\src\vcpkg\installed\x86-windows\include build_me.cpp -o windows_build.exe && windows_build.exe```
```

Note, my Eigen3 is installed with [vcpkg](https://vcpkg.io/en/index.html) so what you put after the `-I` flag may differ.

What results should be:

```cmd
Project compiled on a Windows 32-bit operating system!
The result of multiplying
     0    1.5 -9.456
 by 2 is
      0       3 -18.912
```

The same output but now the first print statement says `Windows 32-bit`!

Before you finish, don't forget to `stop` you container so it does not continue using resources in the background. Feel free to remove it too. Becaus of the volume we haven't change anything important in our container. It's okay to lose that data and have a clean slate with a new container next time we work out our `build_me.cpp` code.

## VS Code

Here's where I plug the [Remote Development Pack for VS Code](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack). It's too much to explain here, but it helps automate a lot of this process within VS Code.

After this example you shouldn't *need* it. But it is undeniably useful. Check it out!
