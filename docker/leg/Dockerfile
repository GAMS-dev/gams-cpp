# Extends builder-bare from machines
FROM registry.gams.com/devel/machines/leg/builder-bare:latest

### More recent cmake to get junit XML output as option for ctest
RUN curl -OL https://cloud.gams.com/s/E4Bkqdcq56rb8Zn/download/cmake-3.23.1-linux-x86_64.tar.gz && \
  tar -xzvf cmake-3.23.1-linux-x86_64.tar.gz && \
  rm cmake-3.23.1-linux-x86_64.tar.gz && \
  mkdir -p /opt/cmake && \
  mv cmake-3.23.1-linux-x86_64/* /opt/cmake

### Make github-release tool available (https://github.com/github-release/github-release)
RUN curl -OL https://cloud.gams.com/s/NJ26xfETjadAC23/download/github-release-binaries.zip && \
  unzip github-release-binaries.zip && \
  chmod u+x leg-github-release && \
  mv leg-github-release /usr/local/bin/github-release && \
  rm deg-github-release && \
  rm wei-github-release.exe
