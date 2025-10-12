document.addEventListener('DOMContentLoaded', function() {
    const form = document.getElementById('loginForm');
    const mensagem = document.getElementById('mensagem');

    form.addEventListener('submit', async (e) => {
        e.preventDefault();
        const usuario = document.getElementById('usuario').value;
        const senha = document.getElementById('senha').value;

        mensagem.style.display = 'none';  // Limpa mensagens antigas

        try {
            const response = await fetch('http://localhost:5000/api/login', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ usuario, senha })
            });

            const data = await response.json();

            if (data.sucesso) {
                mensagem.textContent = `Login realizado com sucesso! Tipo: ${data.tipo}. Redirecionando...`;
                mensagem.className = 'sucesso';
                mensagem.style.display = 'block';

                // Armazena tipo de usuário no localStorage para dashboard
                localStorage.setItem('eh_prof', data.tipo === 'professor');
                localStorage.setItem('usuario', usuario);

                // Redireciona após 1s
                setTimeout(() => {
                    window.location.href = 'dashboard.html';
                }, 1000);
            } else {
                mensagem.textContent = data.erro || 'Erro no login';
                mensagem.className = 'erro';
                mensagem.style.display = 'block';
            }
        } catch (error) {
            mensagem.textContent = 'Erro de conexão com o servidor. Verifique se o backend está rodando.';
            mensagem.className = 'erro';
            mensagem.style.display = 'block';
            console.error('Erro:', error);
        }
    });
});