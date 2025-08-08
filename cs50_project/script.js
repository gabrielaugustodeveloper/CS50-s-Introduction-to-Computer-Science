// Elementos DOM
// Obtém referências para os elementos da página pelo ID
const btnAnalisar = document.getElementById('btn-analisar');
const redacaoTextarea = document.getElementById('redacao');
const resultContainer = document.getElementById('result-container');
const loadingElement = document.getElementById('loading');
const errorElement = document.getElementById('error');
const errorMessage = document.getElementById('error-message');
const themeToggle = document.getElementById('theme-toggle');
const primaryColorPicker = document.getElementById('primary-color');
const totalScoreElement = document.getElementById('total-score');
const strengthsElement = document.getElementById('strengths');
const improvementsElement = document.getElementById('improvements');
const suggestionsElement = document.getElementById('suggestions');

// Configuração de cores
function updateColors() {
    // Pega a cor escolhida pelo usuário
    const primaryColor = primaryColorPicker.value;
    // Define a cor primária na raiz do CSS
    document.documentElement.style.setProperty('--primary', primaryColor);
    
    // Gerar cor secundária baseada na primária, escurecendo 20%
    const secondaryColor = shadeColor(primaryColor, -20);
    document.documentElement.style.setProperty('--secondary', secondaryColor);
}

// Função para escurecer/clarear cores
function shadeColor(color, percent) {
    // Converte o código hexadecimal da cor para valores RGB
    let R = parseInt(color.substring(1, 3), 16);
    let G = parseInt(color.substring(3, 5), 16);
    let B = parseInt(color.substring(5, 7), 16);
    
    // Altera o brilho dos canais RGB conforme o percentual
    R = Math.min(255, Math.max(0, R + R * percent / 100));
    G = Math.min(255, Math.max(0, G + G * percent / 100));
    B = Math.min(255, Math.max(0, B + B * percent / 100));
    
    // Retorna a nova cor em formato hexadecimal
    return `#${((1 << 24) + (R << 16) + (G << 8) + B).toString(16).slice(1)}`;
}

// Event Listeners
// Atualiza as cores sempre que o seletor de cor for alterado
primaryColorPicker.addEventListener('input', updateColors);

// Alterna o tema claro/escuro ao clicar no botão
themeToggle.addEventListener('click', () => {
    document.body.classList.toggle('light-theme');
    const isDark = !document.body.classList.contains('light-theme');
    themeToggle.innerHTML = isDark ? 
        '<i class="fas fa-moon"></i> Modo Escuro' : 
        '<i class="fas fa-sun"></i> Modo Claro';
});


// Ação ao clicar no botão de "Analisar"
btnAnalisar.addEventListener('click', () => {
    // Pega o texto digitado na redação
    const redacao = redacaoTextarea.value.trim();
    
    // Verifica se tem pelo menos 100 caracteres
    if (redacao.length < 100) {
        showError('Sua redação precisa ter pelo menos 100 caracteres');
        return;
    }
    
    // Chama a função de análise com a API
    analyzeWithGemini(redacao);
});

// Mostra uma mensagem de erro por 5 segundos
function showError(message) {
    errorMessage.textContent = message;
    errorElement.style.display = 'block';
    
    setTimeout(() => {
        errorElement.style.display = 'none';
    }, 5000);
}

// Função para analisar com a API Gemini
async function analyzeWithGemini(redacao) {
    // Mostrar o indicador de carregamento
    loadingElement.style.display = 'block';
    errorElement.style.display = 'none';
    resultContainer.style.display = 'none';
    
    try {
        // Substitua pela sua chave API
        const API_KEY = "AIzaSyBfD5Lr3eSMONwbXauCg3Ai-VP3AAwiqa8";
        
        // URL atual da API Gemini 1.5
        const API_URL = `https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=${API_KEY}`;
        
        // Prompt com instruções para correção da redação
        const prompt = `Atue como um corretor especializado em redações do ENEM. Siga rigorosamente:

        1. Atribua uma nota final de 0 a 1000
        2. Avalie separadamente cada uma das 5 competências do ENEM:
           - Competência 1: Domínio da norma padrão (0-200)
           - Competência 2: Compreensão do tema (0-200)
           - Competência 3: Argumentação (0-200)
           - Competência 4: Coesão textual (0-200)
           - Competência 5: Proposta de intervenção (0-200)
        3. Destaque 3 pontos fortes
        4. Aponte 3 pontos a melhorar
        5. Sugira melhorias concretas
        6. Sugira lugares de estudo para melhorar a redação
        7. Formate a resposta em markdown

        **Texto para análise:**
        ${redacao}
        `;

        // Faz a requisição POST para a API
        const response = await fetch(API_URL, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                contents: [{
                    role: "user",
                    parts: [{
                        text: prompt
                    }]
                }],
                generationConfig: {
                    maxOutputTokens: 2048,
                    temperature: 0.7
                }
            })
        });
        
        const data = await response.json();
        
        // Se houver erro na resposta
        if (!response.ok) {
            const errorMsg = data.error?.message || "Erro na API";
            throw new Error(`${errorMsg} (Status: ${response.status})`);
        }
        
        // Pega o texto retornado da resposta
        const responseText = data.candidates[0].content.parts[0].text;
        
        // Processa a resposta (mostra na tela)
        processResponse(responseText);
        
    } catch (error) {
        // Mostra erro amigável ao usuário
        showError(`Erro na análise: ${error.message || "Erro desconhecido"}`);
        console.error("Detalhes do erro:", error);
    } finally {
        // Oculta o carregamento
        loadingElement.style.display = 'none';
    }
}

// Mostra a resposta gerada na área de resultados
function processResponse(responseText) {
    const respostaTextarea = document.getElementById("resposta-bruta");
    respostaTextarea.value = responseText;

    resultContainer.style.display = 'block';
    resultContainer.scrollIntoView({ behavior: 'smooth' });
}

// Função para formatar texto (converter markdown para HTML)
function formatText(text) {
    if (!text) return '';
    
    // Quebras de parágrafo
    let formatted = text
        .replace(/\n\n/g, '</p><p>')
        .replace(/\n/g, '<br>');
    
    // Listas numeradas
    formatted = formatted.replace(/(\d+\.\s+)([^\n]+)/g, '<p>$1$2</p>');
    
    // Negrito em markdown para HTML
    formatted = formatted.replace(/\*\*([^*]+)\*\*/g, '<strong>$1</strong>');
    
    return `<p>${formatted}</p>`;
}

// Inicializar as cores ao carregar a página
updateColors();
