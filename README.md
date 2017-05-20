# Flows

[![Build status](https://travis-ci.org/Rarioty/FlowS.svg?branch=master)](https://travis-ci.org/Rarioty/FlowS)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/2391d8a93bc6413f9658c1a804b62ea6)](https://www.codacy.com/app/Rarioty/FlowS?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Rarioty/FlowS&amp;utm_campaign=Badge_Grade)
[![SensioLabsInsight](https://insight.sensiolabs.com/projects/c8c0ee78-1a66-4f42-9df0-ed5ab94e05ce/big.png)](https://insight.sensiolabs.com/projects/c8c0ee78-1a66-4f42-9df0-ed5ab94e05ce)
[![GitHub Issues](https://img.shields.io/github/issues/Rarioty/FlowS.svg)](https://github.com/Rarioty/FlowS/issues)
[![License](https://img.shields.io/badge/license-MIT%20License-brightgreen.svg)](https://opensource.org/licenses/MIT)

FlowS is a small operating system developped for fun. It is coded in C and assembly without any library not even stdlib. This project has no purpose at all to become a big thing.

## Getting started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

In order to compile this project you will need many tools:
* a cross compiler (you can find a very good tutorial here: [Tutorial](http://wiki.osdev.org/GCC_Cross-Compiler))
* nasm
* bochs for testing
* doxygen for documentation
* a good IDE

### Installing

```
$ git clone git@github.com:rarioty/flows.git
$ cd flows/
```
From here you'll need to edit the makefile.inc.default file to set those two variables:
* CROSS_COMPILER_BINARIES_FOLDER: Folder where you have build your cross compiler (example: /usr/local/cross/bin)
* CROSS_COMPILER_TARGET: Target architecture of your cross compiler (example: i686-elf)

Then you have to save the new file with the name makefile.inc
```
$ make && make docs
```
This command will compile the project and generate the documentation in the doc folder. You can generate the iso image with:
```
$ make os.iso
```
and launch bochs (if you have set the configuration in extern/bochs_configuration in the root folder) with:
```
$ make run
```

## Deployment

This project don't support deployment yet.

## Authors

* **CHEVALIER Arthur** - *Creator* - <arthur.chevalier@inria.fr>

See also the list of [contributors](https://github.com/Rarioty/FlowS/contributors) who participated in this project.

## License

This project is  licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
