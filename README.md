# Estruturas de Dados II

Códigos e projetos desenvolvidos na disciplina **Estruturas de Dados II** do curso de Bacharelado em Sistemas de Informação do **CEFET/RJ – UnED Maria da Graça**, com o professor **Vladimir Erthal**.

Todos os programas são escritos em **C++**, seguindo os padrões apresentados em aula: classes com parte privada e pública, vetores de tamanho fixo e estruturas implementadas do zero (sem bibliotecas prontas de contêineres).

## Conteúdo

| Pasta | O que tem |
|---|---|
| [`Projeto_reino_atentia/`](Projeto_reino_atentia/) | **Reino de Atentia**: jogo de perguntas para crianças, feito como Projeto Pessoal em Dupla (Parte 1). Usa tabelas hash e grafo. |
| [`Aulas/`](Aulas/) | Exercícios feitos durante as aulas. |

## Conteúdos da disciplina praticados aqui

- Classes em C++
- Tabela hash: função de espalhamento, fator de carga, tratamento de colisões por **encadeamento externo** e por **endereçamento aberto**
- Grafos com **matriz de adjacências** (no jogo, direcionado e com pesos)
- **Busca em largura** (com Fila) e **busca em profundidade** (com Pilha)

## Como compilar

Os programas que têm função `main` compilam com o g++:

```bash
g++ -std=c++11 arquivo.cpp -o programa
./programa
```

## Autores

- Valtecir Aragão
- Mariana Eva

## Licença

Distribuído sob a licença Apache 2.0. Veja o arquivo [LICENSE](LICENSE).
