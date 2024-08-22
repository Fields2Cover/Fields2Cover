# OpenAPI generated server

## Overview

This server exposes a (partial) f2c REST API. The server is generated from the OpenAPI definition in `openapi_server/openapi/openapi.yaml`. The server is a Flask server that uses Connexion to handle the OpenAPI definition.

## Extending the webserver

1. Edit the `src/fields2cover-api/openapi_server/openapi/openapi.yaml` file to update the API definition
2. regenerate the server code with the following command:

    ```bash
    java -jar openapi-generator-cli.jar generate -i openapi_server/openapi/openapi.yaml -g python-flask
    ```
3. Note that each method is associated with a controller. The `default_controller.py` file has been excluded in the `.openapi-generator-ignore` which means it will not be overwritten, but also that new methods will not be added to it. If you want to add a new method, you will need to add it to the `default_controller.py` file manually or overwrite and integrate the changes.

## Requirements
Python 3.5.2+

## Usage
To run the server, please execute the following from the root directory:

```
pip3 install -r requirements.txt
python3 -m openapi_server
```

and open your browser to here:

```
http://localhost:8080/ui/
```

Your OpenAPI definition lives here:

```
http://localhost:8080/openapi.json
```

To launch the integration tests, use tox:
```
sudo pip install tox
tox
```

## Running with Docker

To run the server on a Docker container, please execute the following from the root directory:

```bash
# building the image
docker build -t f2c-api --target api .

# starting up a container
docker run -p 8080:8080 f2c-api
```

