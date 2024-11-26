from waitress import serve
from openapi_server import app
import logging

logging.basicConfig(level=logging.INFO, format='%(asctime)s %(message)s')
serve(app.app, host='0.0.0.0', port=8080)

