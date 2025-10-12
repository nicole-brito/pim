from flask import Blueprint, jsonify, request, send_from_directory, session
from data_handler import autenticar_user, ler_dados_txt, ler_alunos_from_c
from busca_ordenacao import buscar_aluno, ordenar_notas
from relatorios import gerar_relatorio_notas
import os

bp = Blueprint('api', __name__)

@bp.route('/login', methods=['POST'])
def login():
    data = request.json
    usuario = data.get('usuario')
    senha = data.get('senha')
    eh_prof, sucesso = autenticar_user(usuario, senha)
    if sucesso:
        session['usuario'] = usuario
        session['eh_prof'] = eh_prof
        return jsonify({'sucesso': True, 'tipo': 'professor' if eh_prof else 'aluno'})
    return jsonify({'sucesso': False, 'erro': 'Credenciais inválidas'}), 401

@bp.route('/logout', methods=['POST'])
def logout():
    session.clear()
    return jsonify({'sucesso': True})

@bp.route('/notas', methods=['GET'])
def get_notas():
    if 'usuario' not in session:
        return jsonify({'erro': 'Não autenticado'}), 401
    aluno_id = request.args.get('aluno_id', type=int)
    if not session.get('eh_prof', False) and aluno_id != 1:  # Aluno vê só si (ID fixo exemplo)
        return jsonify({'erro': 'Acesso negado'}), 403
    notas = ler_dados_txt('notas.txt')
    notas_filtradas = [n for n in notas if n.get('aluno_id') == aluno_id]
    return jsonify(notas_filtradas)

@bp.route('/busca', methods=['GET'])
def busca():
    nome = request.args.get('nome', '')
    resultados = buscar_aluno(nome)
    return jsonify(resultados)

@bp.route('/relatorio', methods=['POST'])
def relatorio():
    if not session.get('eh_prof', False):
        return jsonify({'erro': 'Apenas professores'}), 403
    data = request.json
    aluno_id = data.get('aluno_id')
    filename = f"relatorio_{aluno_id}.pdf"
    gerar_relatorio_notas(aluno_id, f"../data/{filename}")
    return jsonify({'sucesso': True, 'arquivo': filename})

def init_routes(app):
    app.register_blueprint(bp, url_prefix='/api')

# Adicione esta rota após init_routes(app)
@bp.route('/data/<path:filename>')
def serve_pdf(filename):
    return send_from_directory('../data', filename)