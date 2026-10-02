# Templates de aula (transcritos do material do Prof. Vladimir)

Índice:
1. Fila
2. Pilha
3. Grafo (matriz de adjacências + buscas)
4. Tabela Hash – encadeamento externo
5. Tabela Hash – endereçamento aberto
6. Função hash com string (soma ASCII) e fator de carga
7. Utilitários do main (aleatório, menu)

Adapte tipos e campos, mantenha nomes e forma.

---

## 1. Fila (circular)

```cpp
class Fila {
  private:
    int vetor[MAX_VERTICES];
    int inicio = -1, fim = -1, tamanho = 0;

  public:
    bool vazia() { 
        return tamanho == 0; 
    }

    bool cheia() { 
        return tamanho == MAX_VERTICES; 
    }

    void enfila(int valor) {
        if (!cheia()) {
            fim = (fim + 1) % MAX_VERTICES;
            vetor[fim] = valor;
            tamanho++;
            if (inicio == -1) {
                inicio = 0;
            }
        }
    }

    int desenfila() {
        if (!vazia()) {
            int valor = vetor[inicio];
            inicio = (inicio + 1) % MAX_VERTICES;
            tamanho--;
            return valor;
        }
        return -1;
    }
};
```

## 2. Pilha

```cpp
class Pilha {
  private:
    int vetor[MAX_VERTICES];
    int topo = -1;

  public:
    bool vazia() { 
        return topo == -1; 
    }

    bool cheia() { 
        return topo == MAX_VERTICES - 1; 
    }

    void push(int valor) {
        if (!cheia()) {
            topo++;
            vetor[topo] = valor;
        }
    }

    int pop() {
        int dado = -1;
        if (!vazia()) {
            dado = vetor[topo];
            topo--;
        }
        return dado;
    }
};
```

## 3. Grafo

```cpp
class Grafo {
  private:
    int numVertices;                          
    int matrizAdj[MAX_VERTICES][MAX_VERTICES]; 
    string vertices[MAX_VERTICES];               

  public:
    Grafo(int n, string nomes[]) {
        if (n > MAX_VERTICES) {
            numVertices = MAX_VERTICES;
        } else {
            numVertices = n;
        }

        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                matrizAdj[i][j] = 0;
            }
        }

        for (int i = 0; i < numVertices; i++) {
            vertices[i] = nomes[i];
        }
    }

    void adicionarAresta(int v1, int v2, int peso) {
        if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices && peso > 0) {
            matrizAdj[v1][v2] = peso;
            matrizAdj[v2][v1] = peso; //para grafo direcionado, remover esta linha
        }
    }

    void mostrarMatriz() {
        cout << "\nMatriz de Adjacências:" << endl;

        cout << "     ";
        for (int j = 0; j < numVertices; j++) {
            cout << vertices[j] << "   ";
        }
        cout << endl;

        for (int i = 0; i < numVertices; i++) {
            cout << vertices[i] << "  ";
            for (int j = 0; j < numVertices; j++) {
                cout << "  " << matrizAdj[i][j] << " ";
            }
            cout << endl;
        }
    }

    void mostrarVertices() {
        cout << "\nVértices:" << endl;
        for (int i = 0; i < numVertices; i++) {
            cout << i << " -> " << vertices[i] << endl;
        }
    }

    int pesoAresta(int v1, int v2) {
        if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices) {
            return matrizAdj[v1][v2];
        }
        return -1; //não encontrado
    }

    int indiceVertice(string nome) {
        for (int i = 0; i < numVertices; i++) {
            if (vertices[i] == nome) {
                return i;
            }
        }
        return -1; //não encontrado
    }

    int grauVertice(int indice) {
        if (indice < 0 || indice >= numVertices) {
            return -1;
        }

        int grau = 0;
        for (int j = 0; j < numVertices; j++) {
            if (matrizAdj[indice][j] > 0) {
                grau++;
            }
        }
        return grau;
    }

    void buscaEmLarguraCaminho(int inicio, int destino) {
        bool visitado[MAX_VERTICES] = {false};
        int anterior[MAX_VERTICES];
        for (int i = 0; i < numVertices; i++) {
            anterior[i] = -1;
        }
    
        Fila fila;
        visitado[inicio] = true;
        fila.enfila(inicio);
    
        while (!fila.vazia()) {
            int v = fila.desenfila();
    
            if (v == destino) { //Chegou no final
                cout << "\nCaminho encontrado: ";
                int atual = destino;
                string caminho = "Fim";
                while (atual != -1) {
                    caminho = vertices[atual] + " -> " + caminho;
                    atual = anterior[atual];
                }
                cout << caminho << endl;
                return;
            }
    
            for (int i = 0; i < numVertices; i++) {
                if (matrizAdj[v][i] > 0 && !visitado[i]) {
                    visitado[i] = true;
                    anterior[i] = v; //guarda de onde veio
                    fila.enfila(i);
                }
            }
        }
        cout << "\nDestino não encontrado!" << endl;
    }

    void buscaEmProfundidadeCaminho(int inicio, int destino) {
        bool visitado[MAX_VERTICES] = {false};
        int anterior[MAX_VERTICES];
        for (int i = 0; i < numVertices; i++) {
            anterior[i] = -1;
        }
    
        Pilha pilha;
        pilha.push(inicio);
    
        while (!pilha.vazia()) {
            int v = pilha.pop();
    
            if (!visitado[v]) {
                visitado[v] = true;
    
                if (v == destino) { //Chegou no final
                    cout << "\nCaminho encontrado: ";
                    int atual = destino;
                    string caminho = "Fim";
                    while (atual != -1) {
                        caminho = vertices[atual] + " -> " + caminho;
                        atual = anterior[atual];
                    }
                    cout << caminho << endl;
                    return;
                }
    
                for (int i = numVertices - 1; i >= 0; i--) {
                    if (matrizAdj[v][i] > 0 && !visitado[i]) {
                        anterior[i] = v; // guarda de onde veio
                        pilha.push(i);
                    }
                }
            }
        }
        cout << "\nDestino não encontrado!" << endl;
    }
};
```

Variações vistas em aula:
- Grafo direcionado: apagar `matrizAdj[v2][v1] = peso;`.
- Matriz de `string` (placares): vazio = `""`, grau conta `!= ""`.
- `buscaPorNivel`: BFS com `nivel[i] = nivel[v] + 1`, retorna `static string amigos[MAX_VERTICES]` via `string*`.

## 4. Tabela Hash – encadeamento externo

```cpp
class TabelaHash {
  private:
    static const int MAX = 10;

    struct No {
        int chave;
        string valor;
        No* prox;
    };

    No* tabelaHash[MAX];

    int funcaoHash(int chave) {
        return chave % MAX;
    }

  public:
    TabelaHash() {
        for (int i = 0; i < MAX; i++) {
            tabelaHash[i] = NULL;
        }
    }

    void inserir(int chave, string valor) {
        int posicao = funcaoHash(chave);
        No* temp = tabelaHash[posicao];
        while (temp != NULL) {
            if (temp->chave == chave) {
                temp->valor = valor;
                return;
            }
            temp = temp->prox;
        }
        No* novo = new No;
        novo->chave = chave;
        novo->valor = valor;
        novo->prox = tabelaHash[posicao];
        tabelaHash[posicao] = novo;
    }

    string buscar(int chave) {
        int posicao = funcaoHash(chave);
        No* temp = tabelaHash[posicao];
        while (temp != NULL) {
            if (temp->chave == chave) {
                return temp->valor;
            }
            temp = temp->prox;
        }
        return "Não encontrado";
    }

    void remover(int chave) {
        int posicao = funcaoHash(chave);
        No* temp = tabelaHash[posicao];
        No* ant = NULL;
        while (temp != NULL) {
            if (temp->chave == chave) {
                if (ant == NULL) {
                    tabelaHash[posicao] = temp->prox;
                } else {
                    ant->prox = temp->prox;
                }
                delete temp;
                return;
            }
            ant = temp;
            temp = temp->prox;
        }
    }

    void mostrarTabela() {
        for (int i = 0; i < MAX; i++) {
            cout << i << " --> ";
            No* temp = tabelaHash[i];
            while (temp != NULL) {
                cout << "(" << temp->chave << ", " << temp->valor << ") ";
                temp = temp->prox;
            }
            cout << endl;
        }
    }
};
```

Quando o valor for um registro (struct), `buscar` pode retornar um ponteiro para o registro (`Desafio*`) e `NULL` quando não encontrar — é a adaptação natural do `"Não encontrado"`.

## 5. Tabela Hash – endereçamento aberto

```cpp
class TabelaHash {
  private:
    static const int MAX = 10;

    struct Linha {
        int chave;
        string valor;
        bool ocupado;
        bool removido;
    };

    Linha tabela[MAX];

    int funcaoHash(int chave) {
        return chave % MAX;
    }

  public:
    TabelaHash() {
        for (int i = 0; i < MAX; i++) {
            tabela[i].ocupado = false;
            tabela[i].removido = false;
        }
    }

    void inserir(int chave, string valor) {
        int posicao = funcaoHash(chave);
        for (int i = 0; i < MAX; i++) {
            int indice = (posicao + i) % MAX;
            if (tabela[indice].ocupado == false || tabela[indice].chave == chave) {
                tabela[indice].chave = chave;
                tabela[indice].valor = valor;
                tabela[indice].ocupado = true;
                tabela[indice].removido = false;
                return;
            }
        }
        cout << "Erro: tabela cheia, não foi possível inserir " << valor << endl;
    }

    string buscar(int chave) {
        int posicao = funcaoHash(chave);
        for (int i = 0; i < MAX; i++) {
            int indice = (posicao + i) % MAX;
            if (tabela[indice].ocupado == true && tabela[indice].chave == chave) {
                return tabela[indice].valor;
            }
            if (tabela[indice].ocupado == false && tabela[indice].removido == false) {
                return "Não encontrado";
            }
        }
        return "Não encontrado";
    }

    void remover(int chave) {
        int posicao = funcaoHash(chave);
        for (int i = 0; i < MAX; i++) {
            int indice = (posicao + i) % MAX;
            if (tabela[indice].ocupado == true && tabela[indice].chave == chave) {
                tabela[indice].ocupado = false;
                tabela[indice].removido = true;
                return;
            }
            if (tabela[indice].ocupado == false && tabela[indice].removido == false) {
                break;
            }
        }
        cout << "Erro: chave não encontrada para remoção." << endl;
    }

    void mostrarTabela() {
        for (int i = 0; i < MAX; i++) {
            if (tabela[i].ocupado == true) {
                cout << i << " --> (" << tabela[i].chave << ", " << tabela[i].valor << ")" << endl;
            } else if (tabela[i].removido == true) {
                cout << i << " --> [removido]" << endl;
            } else {
                cout << i << " --> [vazio]" << endl;
            }
        }
    }
};
```

### 5b. Variação Login com tentativas (A03 – Login)

Mesma estrutura do endereçamento aberto, com chave = login, valor = senha e um contador de tentativas erradas:

```cpp
    struct Linha {
        string login;
        string senha;
        int tentativas; //senhas erradas seguidas para esse login
        bool ocupado;
        bool removido;
    };

    string buscar(string login, string senha) {
        int posicao = funcaoHash(login);
        for (int i = 0; i < MAX; i++) {
            int indice = (posicao + i) % MAX;
            if (tabela[indice].ocupado == true && tabela[indice].login == login) {
                if (tabela[indice].tentativas >= 3) {
                    return "Número de tentativas excedidas para esse login.";
                }
                if (tabela[indice].senha == senha) {
                    tabela[indice].tentativas = 0;
                    return "Login e senha corretos";
                }
                tabela[indice].tentativas++;
                if (tabela[indice].tentativas >= 3) {
                    return "Número de tentativas excedidas para esse login.";
                }
                return "Senha inválida";
            }
            if (tabela[indice].ocupado == false && tabela[indice].removido == false) {
                return "Login não existe";
            }
        }
        return "Login não existe";
    }

    bool existeLogin(string login) {
        int posicao = funcaoHash(login);
        for (int i = 0; i < MAX; i++) {
            int indice = (posicao + i) % MAX;
            if (tabela[indice].ocupado == true && tabela[indice].login == login) {
                return true;
            }
            if (tabela[indice].ocupado == false && tabela[indice].removido == false) {
                return false;
            }
        }
        return false;
    }
```

Varredura completa da tabela (padrão `mostrarMeusPosts`, A04): percorrer todas as posições e todas as listas, filtrando pelo critério desejado. É assim que se calculam estatísticas sobre todos os itens de uma hash.

## 6. Hash com string e fator de carga

Soma ASCII (ensinada na A03 – Login):

```cpp
    int funcaoHash(string chave) {
        int soma = 0;
        for (int i = 0; i < chave.length(); i++) {
            soma = soma + (int)chave[i];
        }
        return soma % MAX;
    }
```

(Use `int i` e, para evitar warning de comparação signed/unsigned, `for (int i = 0; i < (int)chave.length(); i++)`.)

Fator de carga (A02): quantidade de elementos armazenados / tamanho da tabela.

```cpp
    float fatorCarga() {
        return (float)quantidade / MAX;
    }
```

Outras funções de espalhamento citadas em aula: parte da chave, dobradura, metade do quadrado, resto da divisão.

## 7. Utilitários do main

```cpp
int aleatorio(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

int main() {
    srand(time(0));
    int selecao;
    string texto;

    do {
        cout << "\nSELECIONE A OPÇÃO DESEJADA:\n";
        cout << "  0 - Sair\n";
        cout << "  1 - ...\n";
        cin >> selecao;

        switch (selecao) {
            case 1:
                cout << "Digite ...: ";
                cin.ignore();
                getline(cin, texto);
                break;
        }
        cout << "\n";
    } while (selecao != 0);
}
```
