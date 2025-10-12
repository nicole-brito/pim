document.addEventListener('DOMContentLoaded', function() {
    const ehProf = localStorage.getItem('eh_prof') === 'true';
    const usuario = localStorage.getItem('usuario');

    if (!usuario) {
        alert('Sessão expirada. Redirecionando para login.');
        window.location.href = 'index.html';
        return;
    }

    // Mostra dashboard
    document.getElementById('dashboard').style.display = 'block';

    // Mostra botão de relatório só para professor
    if (ehProf) {
        document.getElementById('btnRelatorio').style.display = 'inline-block';
    }

    // Logout
    document.getElementById('logout').addEventListener('click', async () => {
        try {
            await fetch('http://localhost:5000/api/logout', { method: 'POST' });
        } catch (error) {
            console.error('Erro no logout:', error);
        }
        localStorage.clear();
        window.location.href = 'index.html';
    });
});

// Função para carregar notas
async function carregarNotas() {
    const alunoId = prompt('Digite o ID do Aluno (para professores; seu ID é 1 para aluno):') || 1;
    const ehProf = localStorage.getItem('eh_prof') === 'true';

    if (!ehProf && parseInt(alunoId) !== 1) {
        alert('Alunos só podem ver suas próprias notas!');
        return;
    }

    try {
        const response = await fetch(`http://localhost:5000/api/notas?aluno_id=${alunoId}`);
        if (!response.ok) {
            throw new Error('Erro na API');
        }
        const notas = await response.json();

        if (notas.erro) {
            alert(notas.erro);
            return;
        }

        exibirTabela('conteudo', ['ID', 'Nota', 'Data'], notas.map(n => [n.id, n.nota, n.data]));
    } catch (error) {
        alert('Erro ao carregar notas: ' + error.message);
        console.error(error);
    }
}

// Função para buscar aluno
async function buscarAluno() {
    const nome = prompt('Digite o nome para buscar:');
    if (!nome) return;

    try {
        const response = await fetch(`http://localhost:5000/api/busca?nome=${encodeURIComponent(nome)}`);
        const resultados = await response.json();

        exibirTabela('conteudo', ['ID', 'Nome', 'Matrícula'], resultados.map(r => [r.id, r.nome, r.matricula]));
    } catch (error) {
        alert('Erro na busca: ' + error.message);
        console.error(error);
    }
}

// Função para gerar relatório (só prof)
async function gerarRelatorio() {
    const alunoId = prompt('Digite o ID do Aluno para relatório:');
    if (!alunoId) return;

    try {
        const response = await fetch('http://localhost:5000/api/relatorio', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ aluno_id: parseInt(alunoId) })
        });

        const data = await response.json();
        if (data.sucesso) {
            // Download do PDF (ajuste URL se necessário)
            const link = document.createElement('a');
            link.href = `http://localhost:5000/data/${data.arquivo}`;
            link.download = data.arquivo;
            link.click();
            alert('Relatório gerado e baixado!');
        } else {
            alert(data.erro || 'Erro no relatório');
        }
    } catch (error) {
        alert('Erro ao gerar relatório: ' + error.message);
        console.error(error);
    }
}

// Função auxiliar para exibir tabela
function exibirTabela(elementId, headers, rows) {
    const conteudo = document.getElementById(elementId);
    let html = '<table class="tabela"><thead><tr>';
    headers.forEach(h => html += `<th>${h}</th>`);
    html += '</tr></thead><tbody>';
    rows.forEach(row => {
        html += '<tr>';
        row.forEach(cell => html += `<td>${cell}</td>`);
        html += '</tr>';
    });
    html += '</tbody></table>';
    conteudo.innerHTML = html;
}