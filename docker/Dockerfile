FROM debian

RUN apt-get update && apt-get install

RUN apt-get install -y \
    git \
    vim \
    build-essential

WORKDIR /root

COPY pic32-64.tgz .
# Extract lib to /opt
RUN tar xzvf pic32-64.tgz -C /opt
RUN rm pic32-64.tgz

# Add pic32mx/bin to PATH
RUN printf \
"if [ -d /opt/pic32mx/bin ] ; then\n\
    export PATH=$PATH:/opt/pic32mx/bin\n\
fi" >> .bashrc