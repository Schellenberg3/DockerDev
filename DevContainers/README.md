# DevContainers

This directory concerns the [VS Code Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension. It's one of the four extensions in [VS Code Remote Development](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack) extension pack.

Instead of a Dockerfile inside this sub-directory, we will bring this entire workspace into a Docker container. You may have noticed the top level [`Dockerfile`](../Dockerfile) or the [`.devcontainer`](../.devcontainer) directory. With the Dockerfile we can create an environment where we can view this repository in an Ubuntu container with some helpful libraries installed. and the [`devcontainer.json`](../.devcontainer/devcontainer.json) file, inside the aforementioned directory, lets VS Code coordinate with Docker.

## Why a Dev Container?

While the other directories have demonstrated how to use Docker, they have required us to coordinate between the Docker GUI/CLI and our code editor. We have had to launch our containers and connect volumes.

By now all of this should be comfortable. But there is a convenience to letting this routine task be automated. And to remain within VS Code instead of shifting our attention between several different contexts.

It's still valuable to know how to manually do these things. Sometimes it will be easier. But now that we understand Docker's role it will be easier to discuss what Dev Containers add.

## The Dockerfile

In this directory we use the [`Dockerfile`](../Dockerfile) at the root of the workspace. Our goal is to view this entire workspace inside an Ubuntu container.

For this dev container we pull from the [VS Code Remote / GitHub Codespaces Development Container Images](https://hub.docker.com/_/microsoft-vscode-devcontainers). This container has extra tools installed to help with VS code's server functionality (more below).

<details>
    <summary>On Base Image Choice</summary>
The dev containers still seem to work even if you pull from from the the official [Ubuntu](https://hub.docker.com/_/ubuntu) images too. That's how I began developing this. Honestly, I don't know exactly how the dev containers manage what image it ends up pulling from but there is some behind the scenes stuff that I have not figured out here.

But the bottom line is even if any image works you may want to start with the recommended ones.
</details>

Inside this file we install the build tools and the Eigen3 library to compile the [C++ project in CppDev](../CppDev/) and run the [Python code in PythonDev](../PythonDev/). We also install git; this is a git project after all so we'd like to push any changes we make in the container.

The Dev Container extension will automatically set up a volume to connect the container to  the workspace's root folder.

## Working between Operating Systems

It's worth mentioning that line endings are different between operating systems. The details are not relevant here. Take a look at the [`.gitattributes`](../.gitattributes) file for more details. What we need to know is that we must consider this difference  when we work between a Windows or macOS host and a Linux container.  

Fortunately this is easy. We just need to remember to do it. This [`.gitattributes`](../.gitattributes) file informs git to handle these line ending differences. Without this you might see git incorrectly flagging files as being modified because it has confused the line endings.

## Dev Containers JSON

After installing the Remote Development Extension Pack (and assuming you have Docker installed) you should be ready to go with the extension. To use it, you need a `devcontainer.json` file somewhere in your project. Typically this goes in a folder called `.devcontainer`. But it could be in another location.

Sometimes this is tracked with the project. Some times it is not needed. This is a consideration that must be made per-project. Obviously, we would like to track it here.

The contents of the `devcontainer.json` file are:

```json
{
    "name": "DockerDev",
    "build" : {
        "dockerfile": "../Dockerfile"
    },   
    "customizations": {
        "vscode": {
            "extensions": [
                "ms-python.python",            // Python extension 
                "ms-vscode.cpptools",          // C/C++ extension
                "ms-azuretools.vscode-docker", // Docker extension
                "ms-iot.vscode-ros"            // ROS extension
            ]
        }
    }
}
```

The tag `name` lets us give the dev container a name of our choosing. **This is different than the name of the docker container which actually runs our dev container**.

We can specify an image (see [here](https://code.visualstudio.com/docs/devcontainers/containers#_prebuilding-dev-container-images)). But using the tag `"build"` to specify information about the Dockerfile is perhaps more useful for *development* than just an image. This way we may more easily ask the extension to rebuild our container as things change. We provide the location of our Dockerfile in the `"dockerfile"` tag.

The interesting part comes from the list under `"vscode": "extensions"`. We see this is under the `"customizations"` tag, where we define settings for tools like VS Code. (VS Code is generally the only tool that you will care about.) Inside this is a list of VS Code extensions which will run inside the container.

The extensions are listed in a format of `"[PUBLISHER].[EXTENSION NAME]`. What extensions these are ought to be self explanatory.

The more interesting question of "why are we listing these extension" is worth asking.

Essentially, VS Code operates on a client-server model. The user interface only displays code. Any extensions which affect the appearance (e.g., a fancy color theme) are installed on the client side. But any extensions that interact with the code (e.g., the Python or C++ extensions looking for function definitions) operate on the serve side.

<details>
    <summary>I'm still confused on the local/remote extensions</summary>
In my view there are a few kinds of CS Code extensions:

- **Themes and other GUI modifications**. These only modify the visual appearance of the VS Code window or the text that is displayed. Personally, I find the base VS code to work well without these. But they would be installed locally.
- **Spellcheckers and Linters**. These operate on your actual code. For linters they may use external programs they expect to be installed on the machine. For example, [fly-lint](https://marketplace.visualstudio.com/items?itemName=jbenden.c-cpp-flylint), an extension for C/C++ linting, can utilize the the clang linter but to do so this must be installed on the machine running the code. So if we want to develop inside a container this extension must be installed on the dev container AND the [clang](https://packages.ubuntu.com/focal/clang-10) installed inside the container.
- **Intellisense and debugging features**. These extensions are almost a requirement for using VS Code. They provide feedback by looking at our code and the external libraries our code uses. For example, the Microsoft Python extension can inform us about NumPy functions (or warn us if it cannot find NumPy!) because of these extensions. These also install the requisite programs for VS Code to debug our code. Because these access files (e.g. installed libraries or supporting programs) on the dev container, these language extensions need to be on the remote container.

Missing an extension won't break your dev container but adding one makes it more useful. Add extensions as you need them rather than all at once. Like the packages you install in a Docker container, the extensions you request for a VS Code dev container should self-document the minimal requirements to work with your code.
</details>

In a dev container, our host system continues to run the VS Code GUI while the server is moved to the container. So even if we have installed the Microsoft C/C++ extension on our host, the dev container needs to install this too.

The [Developing inside a Container](https://code.visualstudio.com/docs/devcontainers/containers) is a great reference which explains this (and much more!) in more detail.

To learn more about the tags inside a `devcontainer.json` file, check out the [Dev Container specifications page](https://containers.dev/implementors/json_reference/).

## Launching a Dev Container

There are two common ways to launch a dev container.

Post installation you should find a green icon with two angle brackets: `><`. Clicking this will open a dialog from the Remote Development extension pack. It has options for a few different remote development strategies but we are interested in the `Reopen in Container` option. Pressing this will launch the dev container and connect to it.

The green icon should expand now to show the status reading `Dev Container: [CONTAINER NAME]`. Pressing this now that we are inside a container will show different options including `Reopen Folder Locally`. This will stop the container return us to our host machine.

The other way to open the dev container is using the `CTRL`+`SHIFT`+`P` shortcut to show all commands. As we type out "Dev Container" we will see all commands for dev containers. This include more options like

- `Open Folder in Container` to open the folder/workspace you are currently working in as a dev container
- `Open Workspace in Container` to open a different folder/workspace in its own dev container
- `Create Dev Container` make a new `.devcontainer.json` file based on a template
- `Rebuild and Reopen in Container` and `Rebuild without Cache and Reopen in Container` to make sure the image will be rebuild to reflect changes to the Dockerfile before opening.

As a fun side note, this shortcut, `CTRL`+`SHIFT`+`P`, is a great way to see how to interact with other extensions!

## Closing Thoughts

Dev containers offer a nice way to stay within VS Code for you development needs. They work well for sharing the project with others who also use VS Code. In addition to wrapping Docker with a VS Code interface, it adds some extra functionality by installing the VS Code server and any desired extensions inside the container (including the ability to debug your code inside the container).

However, as I put this guide together I found the documentation a bit lacking lacking. Microsoft has a written guides on how to [Create a Dev Container](https://code.visualstudio.com/docs/devcontainers/create-dev-container) and [Develop Inside a Container](https://code.visualstudio.com/docs/devcontainers/containers). While these offer lots of breadth, it can be difficult to find specifics on any one topic.

Maybe there are more place to look (Microsoft also has open specifications for [Development Containers](https://containers.dev/)). Or understanding just comes with use. But it took a lot of trial and error to set it up. And I am not confident that what I have shown is robust.

For example, running either `Rebuild and Reopen in Container` or `Rebuild without Cache and Reopen in Container` would often result in a dialog window that the container could not be opened with the options to edit the `devcontainer.json` or `retry`. Waiting a few seconds and pressing `retry` would then successfully start the container. I'm no expert but this does not feel like proper behavior.

I think the best way to get use to dev containers is to keep modifying, building, launching and breaking the demo that this repo provides.

However, I was able to get this functioning with trial and error. So if this exact code does not work when you run it or adapt it for another project then keep trying and see what changes might make it work for you. If you learn more, please feel free to submit a pull request here.
