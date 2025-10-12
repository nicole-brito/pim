from flask import Flask, session, jsonify, request, send_from_directory
from flask_cors import CORS
from routes import init_routes

app = Flask(__name__)
app.secret_key = 'chave_secreta_mude_para_producao'
CORS(app)

init_routes(app)

# Servir front-end
@app.route('/')
def index():
    return send_from_directory('../frontend', 'index.html')

@app.route('/dashboard.html')
def dashboard():
    return send_from_directory('../frontend', 'dashboard.html')

@app.route('/<path:path>')
def static_files(path):
    return send_from_directory('../frontend', path)

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)