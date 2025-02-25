# Submit improvements

Your contributions to the GAMS C++ API project are highly appreciated! Depending on
your type of improvement you may want to [create an issue](https://help.github.com/en/articles/creating-an-issue)
or [fork gams-cpp](https://guides.github.com/activities/forking/), to make a pull
request when your changes are ready.

## Requirements

To get started with the development you need a system which provides the following.

* A C++17-standard-compliant compiler
* The latest [GAMS](https://www.gams.com/download/) release

# Quality Gates

## Virtual Python Environment

Setup a virtual Python environment. Please check on your own, which one to use and how to setup the environment. Below `venv`is used on a Debian 12 machine.

```
sudo apt install python3-venv
```

Create a virtual environment.

```
python3 -m venv .venv
```

Ativate the virtual environment.

```
source .venv/bin/activate
```

## Install pre-commit

Please check the information about the pre-commit tool and follow the install steps given on the [project page](https://pre-commit.com/). Also check the `.pre-commit-config.yaml` to see which checks are currently used.

## Install detect-secrets

Please check the information about the detect-secrets tool and follow the install steps given on the [project page](hhttps://github.com/Yelp/detect-secrets).
