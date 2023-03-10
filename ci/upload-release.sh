#!/bin/bash -xe
# Prerequisite: https://github.com/github-release/github-release in PATH
# Call like this GITHUB_TOKEN=xyz sh upload-release.sh

## get version information for artifact, tag name and description
GAMSCPP_MAJOR_VERSION=$(grep ^GAMSCPP_MAJOR_VERSION version | cut -f2 -d"=")
GAMSCPP_MINOR_VERSION=$(grep ^GAMSCPP_MINOR_VERSION version | cut -f2 -d"=")
GAMSCPP_PATCH_LEVEL=$(grep ^GAMSCPP_PATCH_LEVEL version | cut -f2 -d"=")

export GAMSCPP_VERSION=$GAMSCPP_MAJOR_VERSION.$GAMSCPP_MINOR_VERSION.$GAMSCPP_PATCH_LEVEL
export GAMS_DISTRIB_MAJOR_VERSION=$(grep ^GAMS_DISTRIB_MAJOR version | cut -f2 -d"=")

CHANGELOG=""
foundFirst="false"
while IFS="" read -r line
do
    if [[ $line =~ ^Version.* ]] && [[ $foundFirst == "true" ]]; then
        break
    elif [[ $line =~ ^Version.* ]]; then
        foundFirst="true"
    elif [[ $line =~ ^=.* ]]; then
    	 continue
    else
        CHANGELOG+=$line$'\n'
    fi
done < CHANGELOG

## GitHub parameters
GITHUB_ORGA=GAMS-dev
GITHUB_REPO=gams-cpp

## delete old release if needed
RELEASE_DELETED=0
github-release delete --user $GITHUB_ORGA --repo $GITHUB_REPO --tag ${TAG_NAME} && RELEASE_DELETED=0 || RELEASE_DELETED=1
if [[ $RELEASE_DELETED -eq 0 ]]; then
  echo "Release deleted"
else
  echo "No release to delete"
fi

## create a GitHub release
github-release release --user $GITHUB_ORGA --repo $GITHUB_REPO --tag ${TAG_NAME} --pre-release \
--description "${CHANGELOG}

This version of the GAMS C++ API requires GAMS ${GAMS_DISTRIB_MAJOR_VERSION} or any newer GAMS version that gets released before there is a new release of the GAMS C++ API. To download GAMS, please visit https://www.gams.com/latest/. To learn more about the GAMS C++ API, please visit https://www.gams.com/latest/docs/API_CPP_OVERVIEW.html"
