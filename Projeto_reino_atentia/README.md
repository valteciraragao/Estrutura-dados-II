# Reino de Atentia

Jogo de perguntas em console, feito em C++, para crianças do 4º ano do ensino fundamental. O jogador atravessa o Reino de Atentia rumo ao Castelo respondendo desafios de **Matemática**, **Português** e **Lógica**. Acertos dão cristais, que podem ser trocados por poderes.

Projeto Pessoal em Dupla (Parte 1) da disciplina **Estruturas de Dados II** (CEFET/RJ), professor Vladimir Erthal.

**Dupla:** Valtecir Aragão e Mariana Eva

## Como compilar e jogar

```bash
g++ -std=c++11 reino_de_atentia.cpp -o reino
./reino
```

No Windows, o programa ajusta o console para UTF-8 sozinho, para os acentos aparecerem corretamente.

## Como jogar

1. Em cada local do mapa aparece uma pergunta. Digite a **resposta** e aperte ENTER.
2. **Acertou?** Você avança para o próximo nível.
3. **Errou?** Você vai para outro local do mesmo nível e tenta de novo. Se errar em todos os locais de um nível, a aventura acaba.
4. Chegue ao **Castelo de Atentia** para vencer.

As respostas aceitam maiúsculas e minúsculas. As variações aceitas de cada resposta (com e sem acento, por exemplo) estão listadas no banco de perguntas.

### Tempo do bônus

Cada pergunta tem um tempo para o bônus. Responder certo dentro dele dá todos os cristais da pergunta. Responder certo depois dele **também faz avançar**, mas dá só metade dos cristais. O tempo nunca faz o jogador perder.

### Poderes

Durante uma pergunta, digite **P** para abrir o menu de poderes:

| Poder | Custo | Efeito |
|---|---|---|
| DICA | 3 cristais | Mostra uma dica da pergunta |
| TEMPO | 1 cristal | Mais 15 segundos para o bônus |
| PULAR | 2 cristais | Troca por outra pergunta do mesmo nível |

## O mapa

| Nível | Matéria | Locais |
|---|---|---|
| 1 | Matemática | Vale dos Números, Ponte da Soma, Moinho da Tabuada |
| 2 | Português | Biblioteca Encantada, Jardim das Palavras, Torre dos Verbos |
| 3 | Lógica | Labirinto dos Enigmas, Portal do Dragão Sábio |
| Fim | – | Castelo de Atentia (vitória) ou Floresta do Esquecimento (derrota) |

O nível 3 tem só duas chances, para a dificuldade aumentar no final.

## Área do administrador

Pelo menu principal (opção 5) é possível ver:
- o desempenho por matéria (respondidas, acertos e % de acerto), com destaque para a matéria com mais dificuldade;
- a matriz de adjacências do mapa;
- a tabela hash dos desafios e o seu fator de carga;
- os caminhos da busca em largura e da busca em profundidade do início até o castelo.

Os dados valem enquanto o programa está aberto.

## Estruturas de dados usadas

| Estrutura | Onde é usada |
|---|---|
| **Grafo direcionado** (matriz de adjacências) | O mapa do reino. Cada local é um vértice. A aresta de **peso 1** leva quem acerta e a de **peso 2** leva quem erra. Cada vértice guarda só a **chave** da pergunta sorteada. |
| **Tabela hash: desafios** (encadeamento externo) | Banco com as 24 perguntas, buscadas pela chave (ex.: `MAT-01`). Função hash: soma dos códigos ASCII da chave, módulo 11. |
| **Tabela hash: poderes** (encadeamento externo) | Custo e efeito de cada poder, buscados pelo nome. |
| **Fila** | Busca em largura (BFS): mostra o caminho perfeito, o menor até o castelo. |
| **Pilha** | Busca em profundidade (DFS): mostra o caminho mais longo possível até o castelo. |

### Decisões de projeto

- **O grafo guarda só a chave, e a pergunta fica na hash.** As perguntas são sorteadas sem repetição a cada partida, então o mesmo mapa gera partidas diferentes sem duplicar dados.
- **O fator de carga da tabela de desafios é cerca de 2,18** (24 perguntas em 11 posições). Isso mostra que, no encadeamento externo, ele pode passar de 1.
- **O desempenho por matéria vem de uma varredura completa da tabela hash de desafios.** Cada pergunta guarda os seus acertos e erros, e a soma é feita por matéria.
- **A jogabilidade foi ajustada depois de um teste com uma criança.** A resposta virou o padrão da tela e os poderes ficaram atrás da letra P. O tempo passou a afetar só o prêmio, e o jogo pede ENTER antes de limpar a tela.
