Web API
=======

Overview
--------

The web API is a RESTful API that allows users to interact with the Fields2Cover library. The API is built using the OpenAPI specification and is generated using the openapi-generator-cli. 

Methods
-------

At the moment two methods are available:
- generate route: This method generates a route given a set of swaths/working lanes, and a set of transport lanes
- generate path: This method generates a path given the same data

Other methods for other use cases can be included easily by extending the webserver

See the supplied postman collection or the open api UI at <server>:<port>/ui/#/default/generate_path_post


Extending the webserver
-----------------------

1. Edit the `src/fields2cover-api/openapi_server/openapi/openapi.yaml` file to update the API definition.
2. Regenerate the server code with the following command:

    .. code-block:: bash

        java -jar openapi-generator-cli.jar generate -i openapi_server/openapi/openapi.yaml -g python-flask

3. Note that each method is associated with a controller. The `default_controller.py` file has been excluded in the `.openapi-generator-ignore` which means it will not be overwritten, but also that new methods will not be added to it. If you want to add a new method, you will need to add it to the `default_controller.py` file manually or overwrite and integrate the changes.

Requirements
------------

Python 3.5.2+

Usage
-----

To run the server, please execute the following from the root directory:

.. code-block:: bash

    pip3 install -r requirements.txt
    python3 -m openapi_server

and open your browser to here:

