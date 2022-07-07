# Extends builder-full
FROM registry.gams.com/devel/machines/wei/builder-full:latest

SHELL ["powershell", "-executionpolicy", "bypass", "-command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue'; $verbosePreference='Continue'; Set-PSDebug -Trace 0;"]

WORKDIR C:/

### Make github-release tool available (https://github.com/github-release/github-release)
RUN Invoke-WebRequest https://cloud.gams.com/s/NJ26xfETjadAC23/download/github-release-binaries.zip -OutFile "github-release-binaries.zip" -UseBasicParsing ; \
  unzip github-release-binaries.zip ; \
  mv wei-github-release.exe C:/minigams/minigams-wei ; \
  rm deg-github-release ; \
  rm leg-github-release

VOLUME C:/home
WORKDIR C:/home/distrib
CMD ["env", "HOME=/home/distrib", "USERNAME=distrib", "bash"]
