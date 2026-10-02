---
name: ed2-cefet-estilo
description: Estilo de código C++ da disciplina Estruturas de Dados II (CEFET/RJ, Prof. Vladimir Erthal). Use SEMPRE que for escrever, revisar ou corrigir código C++ desta disciplina ou do projeto "Reino de Atentia" — tabela hash (endereçamento aberto ou encadeamento externo), grafo com matriz de adjacências, busca em largura/profundidade, classes Fila/Pilha, menus de console. Também use quando o usuário mencionar "estilo do professor", "ED2", "trabalho da faculdade", "projeto em dupla" ou pedir que o código "pareça feito em aula", mesmo que não cite a skill.
---

# Estilo ED2 – CEFET/RJ

O código entregue é avaliado por um professor que ensinou padrões bem específicos. O objetivo é que o código pareça **escrito por um aluno que domina o que foi dado em aula**: correto, legível, sem recursos que não foram ensinados. Recursos "avançados" não ganham ponto; podem até levantar suspeita de que o código não foi feito pelo aluno.

Antes de escrever qualquer estrutura, abra `references/templates.md` e parta dos modelos de lá — eles são transcrições do material de aula. Adapte (tipos, campos, nomes), mas não reinvente a estrutura.

## Regras obrigatórias

**Linguagem e bibliotecas**
- C++ (compatível com `g++ -std=c++11`). Arquivo único `.cpp`, salvo pedido contrário.
- Includes permitidos: `<iostream>`, `<string>`, `<cstdlib>`, `<ctime>`. Evite qualquer outro; se for imprescindível, justifique em comentário.
- `using namespace std;` no topo.
- **Proibido**: `vector`, `map`, `unordered_map`, `set`, `queue`, `stack`, `list`, `pair`, `auto`, lambdas, templates, `nullptr` (use `NULL`), smart pointers, exceções, `std::sort`/`<algorithm>`, herança/polimorfismo, sobrecarga de operadores, ranged-for.
- `std::string` é permitido e é o tipo padrão para textos.

**Estruturas**
- Toda estrutura é uma `class` com `private:` (dados e funções auxiliares, como `funcaoHash`) e `public:` (métodos). Registros internos são `struct` dentro da classe (`struct Linha`, `struct No`) ou `struct` global simples para dados do jogo.
- Tamanhos fixos com constantes: `const int MAX_VERTICES = N;` global para grafo/fila/pilha e `static const int MAX = N;` dentro da classe de hash.
- Vetores estáticos (`int vetor[MAX]`, `string vertices[MAX_VERTICES]`). Memória dinâmica **apenas** nos nós do encadeamento externo (`new No` / `delete`).
- Valores sentinela: `-1` para índice/inteiro não encontrado, `"Não encontrado"` para string, `NULL` para ponteiro, `""` para posição vazia em vetor/matriz de strings.

**Tabela Hash**
- Função hash dentro do `private`, chamada `funcaoHash`. Para chaves string, a versão ensinada é soma dos códigos ASCII (`(int)chave[i]`) `% MAX`.
- Encadeamento externo: `No* tabelaHash[MAX];`, construtor inicializa tudo com `NULL`, inserção **no início** da lista, busca percorrendo `temp = temp->prox`.
- Endereçamento aberto: `struct Linha` com `ocupado`/`removido`, sondagem linear `(posicao + i) % MAX`.
- Métodos com os nomes de aula: `inserir`, `buscar`, `remover`, `mostrarTabela`; `fatorCarga` quando fizer sentido (n/m, retorna `float`).

**Grafo**
- Matriz de adjacências `int matrizAdj[MAX_VERTICES][MAX_VERTICES]` + `string vertices[MAX_VERTICES]` + `int numVertices`.
- Construtor `Grafo(int n, string nomes[])` que limita `n` a `MAX_VERTICES` e zera a matriz.
- `adicionarAresta(v1, v2, peso)` só aceita `peso > 0`. Grafo direcionado = remover a linha espelhada (`matrizAdj[v2][v1] = peso;`), deixando o comentário do professor explicando isso.
- Métodos padrão mantidos com os mesmos nomes: `mostrarMatriz`, `mostrarVertices`, `pesoAresta`, `indiceVertice`, `grauVertice`.
- Buscas: `buscaEmLargura` / `buscaEmLarguraCaminho` com a classe `Fila`; `buscaEmProfundidade` / `buscaEmProfundidadeCaminho` com a classe `Pilha`; caminho reconstruído com vetor `anterior[]` iniciado em `-1`, montando a string de trás pra frente a partir de `"Fim"`.

**Fila e Pilha**
- Copiar as classes `Fila` (circular, `enfila`/`desenfila`/`vazia`/`cheia`) e `Pilha` (`push`/`pop`/`vazia`/`cheia`) exatamente como no template.

**main e interação**
- Menu em `do { ... switch (selecao) { case ...: break; } } while (selecao != 0);`.
- Leitura de número com `cin >> x;`; leitura de texto com `cin.ignore();` seguido de `getline(cin, texto);` (cuidado: só um `cin.ignore()` depois de um `cin >>`).
- Aleatoriedade: `srand(time(0));` no início do `main` e a função `int aleatorio(int minimo, int maximo)` do material.

**Interação com o jogador (console, público infantil)**
- Nunca misturar no mesmo prompt "resposta" e "comando". O prompt principal de uma pergunta é sempre a resposta; comandos entram por uma letra reservada e explícita (ex.: `P` abre os poderes) e menus internos são numerados com `0 - Voltar`.
- Toda mensagem de resultado (acerto, erro, fim de nível) termina com `Aperte ENTER para continuar...` antes de "limpar a tela" com linhas em branco — senão o feedback some antes de ser lido.
- Entrada vazia ou inválida nunca conta como erro do jogador: explique o que digitar e pergunte de novo.
- Texto curto, positivo, uma informação por linha; plural com um if simples onde o número pode ser 1; não criar função de plural.
- Telas de jogador não mostram estruturas cruas (posições vazias da hash, matriz). Isso fica na área do administrador.

**Structs com contadores**
- Não usar inicializador dentro da struct (`int acertos = 0;`) em structs que são preenchidas com chaves `{...}` — em C++11 isso deixa de ser agregado e quebra a inicialização. Zere os contadores explicitamente na carga (ou deixe-os de fora das chaves, que viram 0).

**Formatação de saída (marca do estilo de aula)**
- Quebras e recuos vão **dentro das strings**: `cout << "\n\tPERGUNTA: " << pergunta << "\n";`. Prefira `"\n"` a `endl`.
- Menus como no material: `cout << "\n\tSELECIONE A OPÇÃO DESEJADA:\n";` seguido de uma linha por opção (`"\t  1 - Jogar\n"`).
- Colunas de tabela com `\t`, nunca com funções de alinhamento, `setw` ou `to_string`.

**Parecer código de aluno (anti-padrões de IA a evitar)**
- Sem blocos separadores de seção (`//-------`, `// ===== X =====`).
- Comentários poucos e curtos, só onde há uma ideia (`//guarda de onde veio`).
- Sem tratamento defensivo além do que o professor faz (nada de `cin.eof()` em cada `getline`). Proteção aceita: `cin.fail()` nos menus.
- Sem funções minúsculas só para "organizar". Uma função longa e clara (até ~120 linhas) é mais natural que dez funções de 8 linhas.
- Sem funcionalidades além do pedido: cada função extra é algo que a dupla precisa saber explicar.

**Estilo de escrita**
- Identificadores e mensagens **em português**, camelCase (`buscarDesafio`, `numVertices`, `cristais`).
- Indentação de 4 espaços, chaves na mesma linha.
- Comentários curtos em português explicando o *porquê* (ex.: `//guarda de onde veio`, `//entra na frente da lista`). Sem blocos gigantes de documentação.
- `for` com índice `int i`.

## Checklist antes de entregar
1. Compila sem warnings com `g++ -std=c++11 -Wall -Wextra arquivo.cpp`.
2. Nenhum item da lista "Proibido" aparece no código (faça um `grep` por `vector`, `map`, `auto`, `nullptr`, `#include <algorithm>`).
3. Todo `new` tem um `delete` correspondente (destrutor da tabela hash liberando as listas).
4. Rodou o programa de ponta a ponta com entrada simulada (`printf "..." | ./programa`), incluindo caminhos de vitória e derrota.
5. Os nomes dos métodos das estruturas batem com os de aula.
6. Nenhum `to_string`/`setw`/`//---`, `endl` raro e tamanho compatível com o conteúdo (sem código morto ou duplicado).
