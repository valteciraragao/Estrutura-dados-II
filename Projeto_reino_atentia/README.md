# Reino de Atentia

Jogo de perguntas em console, feito em C++, em que o jogador atravessa o reino
respondendo desafios de Matemática, Português e Lógica para juntar cristais.

- **Disciplina:** Estruturas de Dados II (CEFET/RJ)
- **Professor:** Vladimir Erthal
- **Dupla:** Valtecir Aragão e Mariana Eva

## Como compilar e rodar

```bash
g++ -std=c++11 reino_de_atentia.cpp -o reino
./reino
```

## Estruturas de dados usadas

- **Grafo com matriz de adjacências** (`Grafo`): o mapa do reino; cada vértice é
  um local com um desafio sorteado.
- **Busca em largura com fila** (`Fila`): menor caminho até a vitória, de quem
  acerta tudo.
- **Busca em profundidade com pilha** (`Pilha`): um caminho completo, o mais
  longo até a vitória.
- **Tabelas hash com encadeamento externo**: `TabelaDesafios` (banco de
  perguntas), `TabelaPoderes` (poderes comprados com cristais) e
  `TabelaJogadores` (estatísticas de cada jogador).
- **Tabela hash com endereçamento aberto** (`TabelaLogin`): logins da área do
  administrador.
