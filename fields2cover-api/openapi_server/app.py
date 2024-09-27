import connexion
from openapi_server import encoder

# ==============================================================================
#    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                     Author: Tom Schut
#                        BSD-3 License
# ==============================================================================

app = connexion.App(__name__, specification_dir='./openapi/')
app.app.json_encoder = encoder.JSONEncoder
app.add_api('openapi.yaml',
            arguments={'title': 'Path Generation API'},
            pythonic_params=True)

