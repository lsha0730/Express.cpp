FROM python:3.10-slim

# Create a non-root user and group
RUN addgroup --system conan && adduser --system --ingroup conan --home /home/conan conan

# Change ownership of Conan server data directory
RUN mkdir -p /home/conan/.conan_server && chown -R conan:conan /home/conan

# Set working directory and switch to non-root user
USER conan
WORKDIR /home/conan

RUN pip install conan-server

VOLUME /home/conan/.conan_server

EXPOSE 9300

ENTRYPOINT ["conan_server"]