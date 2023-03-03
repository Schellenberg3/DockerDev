# 🐋 DockerDev 🖥️

This repo is designed to help people get started with docker as a tool for developing code using docker. The directories in this repo cover the basics of how to use docker and show some simple environments for developing C++, Python, and ROS applications.

The contents in this repo do not cover how to install docker. There a plenty of tutorials to show that process (see docker's official guides for: [Windows](https://docs.docker.com/desktop/install/windows-install/), [Linux](https://docs.docker.com/desktop/install/linux-install/), [Mac](https://docs.docker.com/desktop/install/mac-install/)). Instead, it is assumed that Docker GUI is installed, though most explanations use the command line interface this is installed alongside the GUI.

The docker commands shown as an example will use syntax for an Linux system. The docker interfaces is the same across systems, but specific shell/terminal commands or how a path is formatted may vary depending on your system. Be cognizant of this as you follow along.

## [Basics](Basics)

This sub-directory demonstrates a simple linux container. We explore a simple docker file, build our first container, and test how volumes transfer data between our host and our container.

## [CppDev](CppDev)

This sub-directory demonstrates a simple C++ application on a linux container. It touches on what dependencies are needed when starting from a base Ubuntu container.

## [PythonDev](PythonDev)

This sub-directory demonstrates the Python official container and how to get started with containers that provide drop-in tools.

## [Compose Basics](ComposeBasics)

This sub-directory demonstrates the Docker Compose tool. It provides a minimal example of the key commands and how to write a Docker Compose file. 

## [ROSDev](ROSDev)

This sub-directory demonstrates the ROS 2 official container. We show how we can run ROS 2 on Ubuntu 20.04 amd ROS on Ubuntu 18.04 on the same machine.
