import unittest

from flask import json

from openapi_server.models.generate_path_body import GeneratePathBody  # noqa: E501
from openapi_server.models.path import Path  # noqa: E501
from openapi_server.test import BaseTestCase


class TestDefaultController(BaseTestCase):
    """DefaultController integration test stubs"""

    def test_generate_path_post(self):
        """Test case for generate_path_post

        Generate a path based on field data
        """
        generate_path_body = openapi_server.GeneratePathBody()
        headers = { 
            'Accept': 'application/json',
            'Content-Type': 'application/json',
        }
        response = self.client.open(
            '/generatePath',
            method='POST',
            headers=headers,
            data=json.dumps(generate_path_body),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))


if __name__ == '__main__':
    unittest.main()
