/*
    Reino de Atentia
    Disciplina: Estruturas de Dados II
    Professor: Vladimir Erthal
    Dupla: Valtecir Aragão e Mariana Eva
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const int MAX_VERTICES = 10;

struct Desafio {
    string chave;
    string categoria;
    string pergunta;
    string resposta; //alternativas aceitas separadas por |
    string dica;
    int nivel;
    int tempo; //segundos disponíveis para responder
    int cristais; //cristais ganhos ao acertar
    int acertos;
    int erros;
};

struct Poder {
    string chave;
    string efeito;
    int custo;
};

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

class Grafo {
  private:
    int numVertices;
    int matrizAdj[MAX_VERTICES][MAX_VERTICES];
    string vertices[MAX_VERTICES];
    string chaveDesafio[MAX_VERTICES]; //chave do desafio sorteado para cada local

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
            chaveDesafio[i] = ""; //ninguém tem desafio sorteado ainda
        }
    }

    void adicionarAresta(int v1, int v2, int peso) {
        if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices && peso > 0) {
            matrizAdj[v1][v2] = peso;
            //grafo direcionado: removida a linha de volta
        }
    }

    void mostrarMatriz() {
        cout << "\n\tMatriz de Adjacências:\n";

        cout << "\t     ";
        for (int j = 0; j < numVertices; j++) {
            cout << j << "   ";
        }
        cout << "\n";

        for (int i = 0; i < numVertices; i++) {
            cout << "\t" << i << "  ";
            for (int j = 0; j < numVertices; j++) {
                cout << "  " << matrizAdj[i][j] << " ";
            }
            cout << "\n";
        }
    }

    void mostrarVertices() {
        cout << "\n\tLocais do reino:\n";
        for (int i = 0; i < numVertices; i++) {
            cout << "\t" << i << " -> " << vertices[i] << "\n";
        }
    }

    string nomeVertice(int indice) {
        if (indice >= 0 && indice < numVertices) {
            return vertices[indice];
        }
        return "Não encontrado";
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

    void definirChave(int v, string chave) {
        if (v >= 0 && v < numVertices) {
            chaveDesafio[v] = chave;
        }
    }

    string obterChave(int v) {
        if (v >= 0 && v < numVertices) {
            return chaveDesafio[v];
        }
        return "";
    }

    int proximoVertice(int atual, int peso) {
        if (atual >= 0 && atual < numVertices) {
            for (int j = 0; j < numVertices; j++) {
                if (matrizAdj[atual][j] == peso) {
                    return j;
                }
            }
        }
        return -1; //não encontrado
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

            if (v == destino) { //chegou no final
                cout << "\n\tCaminho encontrado: ";
                int atual = destino;
                string caminho = "Fim";
                while (atual != -1) {
                    caminho = vertices[atual] + " -> " + caminho;
                    atual = anterior[atual];
                }
                cout << caminho << "\n";
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
        cout << "\n\tDestino não encontrado!\n";
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

                if (v == destino) { //chegou no final
                    cout << "\n\tCaminho encontrado: ";
                    int atual = destino;
                    string caminho = "Fim";
                    while (atual != -1) {
                        caminho = vertices[atual] + " -> " + caminho;
                        atual = anterior[atual];
                    }
                    cout << caminho << "\n";
                    return;
                }

                for (int i = numVertices - 1; i >= 0; i--) {
                    if (matrizAdj[v][i] > 0 && !visitado[i]) {
                        anterior[i] = v; //guarda de onde veio
                        pilha.push(i);
                    }
                }
            }
        }
        cout << "\n\tDestino não encontrado!\n";
    }
};

class TabelaDesafios {
  private:
    static const int MAX = 11;

    struct No {
        Desafio item;
        No* prox;
    };

    No* tabelaHash[MAX];
    int quantidade;

    int funcaoHash(string chave) {
        int soma = 0;
        for (int i = 0; i < (int)chave.length(); i++) {
            soma = soma + (int)chave[i];
        }
        return soma % MAX;
    }

  public:
    TabelaDesafios() {
        for (int i = 0; i < MAX; i++) {
            tabelaHash[i] = NULL;
        }
        quantidade = 0;
    }

    ~TabelaDesafios() {
        for (int i = 0; i < MAX; i++) {
            No* temp = tabelaHash[i];
            while (temp != NULL) {
                No* proximo = temp->prox;
                delete temp;
                temp = proximo;
            }
        }
    }

    void inserir(Desafio d) {
        int posicao = funcaoHash(d.chave);
        No* temp = tabelaHash[posicao];
        while (temp != NULL) {
            if (temp->item.chave == d.chave) {
                temp->item = d;
                return;
            }
            temp = temp->prox;
        }

        No* novo = new No;
        novo->item = d;
        novo->prox = tabelaHash[posicao]; //entra na frente da lista
        tabelaHash[posicao] = novo;
        quantidade++;
    }

    Desafio* buscar(string chave) {
        int posicao = funcaoHash(chave);
        No* temp = tabelaHash[posicao];
        while (temp != NULL) {
            if (temp->item.chave == chave) {
                return &(temp->item);
            }
            temp = temp->prox;
        }
        return NULL; //não encontrado
    }

    void registrarResultado(string chave, bool acertou) {
        Desafio* d = buscar(chave);
        if (d != NULL) {
            if (acertou) {
                d->acertos++;
            } else {
                d->erros++;
            }
        }
    }

    void mostrarDesempenho() {
        string categorias[3] = {"Matemática", "Português", "Lógica"};
        int acertos[3] = {0, 0, 0};
        int erros[3] = {0, 0, 0};

        for (int i = 0; i < MAX; i++) {
            No* temp = tabelaHash[i];
            while (temp != NULL) {
                for (int c = 0; c < 3; c++) {
                    if (temp->item.categoria == categorias[c]) {
                        acertos[c] = acertos[c] + temp->item.acertos;
                        erros[c] = erros[c] + temp->item.erros;
                    }
                }
                temp = temp->prox;
            }
        }

        cout << "\n\tDesempenho por matéria:\n";

        int totalRespondidas = 0;
        for (int c = 0; c < 3; c++) {
            totalRespondidas = totalRespondidas + acertos[c] + erros[c];
        }
        if (totalRespondidas == 0) {
            cout << "\tAinda não há partidas.\n";
            return;
        }

        int pior = -1; //matéria com menor % de acerto
        int piorPercentual = 0;
        for (int c = 0; c < 3; c++) {
            int respondidas = acertos[c] + erros[c];
            cout << "\t" << categorias[c] << "\t";
            if (c == 2) {
                cout << "\t"; //alinha a coluna de Lógica
            }
            cout << "Respondidas: " << respondidas << "\tAcertos: " << acertos[c];
            if (respondidas > 0) {
                int percentual = acertos[c] * 100 / respondidas;
                cout << "\tAcerto: " << percentual << "%\n";
                if (pior == -1 || percentual < piorPercentual) {
                    pior = c;
                    piorPercentual = percentual;
                }
            } else {
                cout << "\tAcerto: -\n";
            }
        }
        cout << "\n\t>> Matéria com mais dificuldade: " << categorias[pior] << "\n";
    }

    void mostrarTabela() {
        cout << "\n\tTabela hash dos desafios:\n";
        for (int i = 0; i < MAX; i++) {
            cout << "\t" << i << " --> ";
            No* temp = tabelaHash[i];
            while (temp != NULL) {
                cout << temp->item.chave << " ";
                temp = temp->prox;
            }
            cout << "\n";
        }
    }

    float fatorCarga() {
        return (float)quantidade / MAX;
    }
};

class TabelaPoderes {
  private:
    static const int MAX = 5;

    struct No {
        Poder item;
        No* prox;
    };

    No* tabelaHash[MAX];

    int funcaoHash(string chave) {
        int soma = 0;
        for (int i = 0; i < (int)chave.length(); i++) {
            soma = soma + (int)chave[i];
        }
        return soma % MAX;
    }

  public:
    TabelaPoderes() {
        for (int i = 0; i < MAX; i++) {
            tabelaHash[i] = NULL;
        }
    }

    ~TabelaPoderes() {
        for (int i = 0; i < MAX; i++) {
            No* temp = tabelaHash[i];
            while (temp != NULL) {
                No* proximo = temp->prox;
                delete temp;
                temp = proximo;
            }
        }
    }

    void inserir(Poder p) {
        int posicao = funcaoHash(p.chave);
        No* temp = tabelaHash[posicao];
        while (temp != NULL) {
            if (temp->item.chave == p.chave) {
                temp->item = p;
                return;
            }
            temp = temp->prox;
        }

        No* novo = new No;
        novo->item = p;
        novo->prox = tabelaHash[posicao]; //entra na frente da lista
        tabelaHash[posicao] = novo;
    }

    Poder* buscar(string chave) {
        int posicao = funcaoHash(chave);
        No* temp = tabelaHash[posicao];
        while (temp != NULL) {
            if (temp->item.chave == chave) {
                return &(temp->item);
            }
            temp = temp->prox;
        }
        return NULL; //não encontrado
    }

    void mostrarTabela() {
        cout << "\n\tPODERES DO REINO\n";
        cout << "\t(Durante uma pergunta, digite P para usar)\n\n";
        for (int i = 0; i < MAX; i++) {
            No* temp = tabelaHash[i];
            while (temp != NULL) {
                cout << "\t" << temp->item.chave << " - " << temp->item.efeito;
                if (temp->item.custo == 1) {
                    cout << " (custa 1 cristal)\n";
                } else {
                    cout << " (custa " << temp->item.custo << " cristais)\n";
                }
                temp = temp->prox;
            }
        }
    }
};

int aleatorio(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

void limparTela() {
    for (int i = 0; i < 12; i++) {
        cout << "\n";
    }
}

//passa as letras de A a Z para minúsculas
string minusculo(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
    }
    return s;
}

//a resposta esperada pode ter várias alternativas separadas por |
bool respostaCorreta(string digitada, string esperada) {
    string digitadaMinuscula = minusculo(digitada);

    string alternativas[10];
    int totalAlternativas = 0;
    string atual = "";

    for (int i = 0; i < (int)esperada.length(); i++) {
        if (esperada[i] == '|') {
            alternativas[totalAlternativas] = atual;
            totalAlternativas++;
            atual = "";
        } else {
            atual = atual + esperada[i];
        }
    }
    alternativas[totalAlternativas] = atual;
    totalAlternativas++;

    for (int i = 0; i < totalAlternativas; i++) {
        if (digitadaMinuscula == alternativas[i]) {
            return true;
        }
    }
    return false;
}

//mostra só a primeira alternativa da resposta (antes do |)
string primeiraAlternativa(string esperada) {
    string texto = "";
    int i = 0;
    while (i < (int)esperada.length() && esperada[i] != '|') {
        texto = texto + esperada[i];
        i++;
    }
    return texto;
}

//sorteia uma posição do pool que ainda não foi usada nesta partida
string sortearSemRepetir(string pool[], bool usados[], int tamanho) {
    int indice = aleatorio(0, tamanho - 1);
    while (usados[indice]) {
        indice = aleatorio(0, tamanho - 1);
    }
    usados[indice] = true;
    return pool[indice];
}

void carregarBanco(TabelaDesafios &tabela) {
    Desafio banco[24] = {
        {"MAT-01", "Matemática", "Quanto é 7 + 8?", "15", "Conte a partir do 8.", 1, 30, 2, 0, 0},
        {"MAT-02", "Matemática", "Quanto é 9 x 6?", "54", "Pense em 9 grupos de 6.", 1, 30, 2, 0, 0},
        {"MAT-03", "Matemática", "Maria tinha 23 figurinhas e ganhou mais 17. Com quantas ficou?", "40", "Some as dezenas e depois as unidades.", 1, 40, 2, 0, 0},
        {"MAT-04", "Matemática", "Qual é a metade de 48?", "24", "Divida por 2.", 1, 25, 2, 0, 0},
        {"MAT-05", "Matemática", "Quanto é 100 - 37?", "63", "Pense em quanto falta de 37 até 100.", 1, 30, 2, 0, 0},
        {"MAT-06", "Matemática", "Um pacote tem 6 balas. Quantas balas há em 4 pacotes?", "24", "Some 6 quatro vezes.", 1, 30, 2, 0, 0},
        {"MAT-07", "Matemática", "Quantos lados tem um triângulo?", "3|três|tres", "Pense no nome da figura.", 1, 20, 2, 0, 0},
        {"MAT-08", "Matemática", "Um lápis custa 5 reais. Quanto custam 3 lápis?", "15|15 reais", "Some 5 três vezes.", 1, 30, 2, 0, 0},

        {"POR-01", "Português", "Qual é o antônimo de \"alegre\"?", "triste", "Pense no sentimento oposto.", 2, 25, 3, 0, 0},
        {"POR-02", "Português", "Qual é o sinônimo de \"veloz\"?", "rápido|rapido|ligeiro", "Pense em algo que anda depressa.", 2, 25, 3, 0, 0},
        {"POR-03", "Português", "Na frase \"O gato subiu no telhado\", qual é o verbo?", "subiu", "É a ação que o gato fez.", 2, 30, 3, 0, 0},
        {"POR-04", "Português", "Complete: \"Ele foi ___ escola de bicicleta.\" Digite 1 para \"a\", 2 para \"há\" ou 3 para \"à\".", "3", "Pense em ir PARA algum lugar.", 2, 30, 3, 0, 0},
        {"POR-05", "Português", "Qual destas é um substantivo: correr, bonito, cadeira, rapidamente?", "cadeira", "Substantivo nomeia um objeto ou ser.", 2, 30, 3, 0, 0},
        {"POR-06", "Português", "Como se escreve certo: \"excessão\" ou \"exceção\"?", "exceção|excecao", "A palavra correta tem c e ç.", 2, 30, 3, 0, 0},
        {"POR-07", "Português", "Qual é o plural de \"papel\"?", "papéis|papeis", "Troque o \"l\" final pelo som \"is\".", 2, 25, 3, 0, 0},
        {"POR-08", "Português", "Qual é o sujeito de \"As crianças brincam no parque\"?", "as crianças|crianças|as criancas|criancas", "É quem faz a ação de brincar.", 2, 30, 3, 0, 0},

        {"LOG-01", "Lógica", "Complete a sequência: 2, 4, 6, 8, ___", "10", "Cada número aumenta de 2 em 2.", 3, 35, 4, 0, 0},
        {"LOG-02", "Lógica", "Se hoje é quarta-feira, que dia será depois de amanhã?", "sexta-feira|sexta|sexta feira", "Conte dois dias a partir de hoje.", 3, 35, 4, 0, 0},
        {"LOG-03", "Lógica", "Complete a sequência: 3, 6, 12, 24, ___", "48", "Cada número é o dobro do anterior.", 3, 40, 4, 0, 0},
        {"LOG-04", "Lógica", "João é mais alto que Pedro, e Pedro é mais alto que Ana. Quem é o mais baixo?", "ana", "Compare os três em ordem.", 3, 40, 4, 0, 0},
        {"LOG-05", "Lógica", "Qual não pertence ao grupo: quadrado, círculo, triângulo, cachorro?", "cachorro", "Três delas são formas geométricas.", 3, 30, 4, 0, 0},
        {"LOG-06", "Lógica", "Se A=1, B=2, C=3, quanto vale a soma das letras da palavra AC?", "4|quatro", "Some o valor de cada letra.", 3, 40, 4, 0, 0},
        {"LOG-07", "Lógica", "Todos os gatos são animais. Frajola é um gato. O que é Frajola?", "um animal|animal", "Aplique a regra geral ao caso específico.", 3, 35, 4, 0, 0},
        {"LOG-08", "Lógica", "Se hoje é dia 10, que dia será depois de amanhã?", "12|dia 12", "Some dois dias à data de hoje.", 3, 30, 4, 0, 0}
    };

    for (int i = 0; i < 24; i++) {
        tabela.inserir(banco[i]);
    }
}

void carregarPoderes(TabelaPoderes &tabela) {
    Poder poderes[3] = {
        {"DICA", "Revela a dica do desafio atual", 3},
        {"TEMPO", "Adiciona 15 segundos ao tempo do desafio", 1},
        {"PULAR", "Troca por outra pergunta do mesmo nível", 2}
    };

    for (int i = 0; i < 3; i++) {
        tabela.inserir(poderes[i]);
    }
}

void montarMapa(Grafo &grafo) {
    grafo.adicionarAresta(0, 3, 1);
    grafo.adicionarAresta(0, 1, 2);
    grafo.adicionarAresta(1, 3, 1);
    grafo.adicionarAresta(1, 2, 2);
    grafo.adicionarAresta(2, 3, 1);
    grafo.adicionarAresta(2, 9, 2);
    grafo.adicionarAresta(3, 6, 1);
    grafo.adicionarAresta(3, 4, 2);
    grafo.adicionarAresta(4, 6, 1);
    grafo.adicionarAresta(4, 5, 2);
    grafo.adicionarAresta(5, 6, 1);
    grafo.adicionarAresta(5, 9, 2);
    grafo.adicionarAresta(6, 8, 1);
    grafo.adicionarAresta(6, 7, 2);
    grafo.adicionarAresta(7, 8, 1);
    grafo.adicionarAresta(7, 9, 2);
}

void mostrarComoJogar() {
    cout << "\n\tCOMO JOGAR\n";
    cout << "\n\tVocê vai viajar pelo Reino de Atentia até chegar ao Castelo.\n";
    cout << "\tEm cada lugar, responda uma pergunta de Matemática, Português ou Lógica.\n";
    cout << "\tDigite a sua resposta e aperte ENTER.\n";
    cout << "\tSe acertar, você avança para um lugar mais perto do castelo.\n";
    cout << "\tSe errar, você vai para outro lugar do mesmo nível e pode tentar de novo.\n";
    cout << "\tCuidado: se errar em todos os lugares de um nível, a aventura acaba!\n";
    cout << "\n\tCada acerto dá cristais.\n";
    cout << "\tSe responder dentro do tempo, você ganha todos os cristais.\n";
    cout << "\tSe passar do tempo, você avança do mesmo jeito, mas ganha só a metade.\n";
    cout << "\n\tPara usar um poder, digite P no lugar da resposta:\n";
    cout << "\t  DICA  - mostra uma dica da pergunta (3 cristais)\n";
    cout << "\t  TEMPO - dá mais 15 segundos (1 cristal)\n";
    cout << "\t  PULAR - troca por outra pergunta do mesmo nível (2 cristais)\n";
    cout << "\n\tBoa sorte, aventureiro!\n";
}

//monta a trilha percorrida no formato "Local -> Local -> ... -> Fim"
string montarTrilhaTexto(Grafo &grafo, int trilha[], int totalTrilha) {
    string texto = "Fim";
    for (int i = totalTrilha - 1; i >= 0; i--) {
        texto = grafo.nomeVertice(trilha[i]) + " -> " + texto;
    }
    return texto;
}

void jogar(Grafo &grafo, TabelaDesafios &tabelaDesafios, TabelaPoderes &tabelaPoderes) {
    string poolMat[8] = {"MAT-01", "MAT-02", "MAT-03", "MAT-04", "MAT-05", "MAT-06", "MAT-07", "MAT-08"};
    string poolPor[8] = {"POR-01", "POR-02", "POR-03", "POR-04", "POR-05", "POR-06", "POR-07", "POR-08"};
    string poolLog[8] = {"LOG-01", "LOG-02", "LOG-03", "LOG-04", "LOG-05", "LOG-06", "LOG-07", "LOG-08"};

    bool usadosMat[8];
    bool usadosPor[8];
    bool usadosLog[8];
    for (int i = 0; i < 8; i++) {
        usadosMat[i] = false;
        usadosPor[i] = false;
        usadosLog[i] = false;
    }

    for (int v = 0; v <= 2; v++) {
        grafo.definirChave(v, sortearSemRepetir(poolMat, usadosMat, 8));
    }
    for (int v = 3; v <= 5; v++) {
        grafo.definirChave(v, sortearSemRepetir(poolPor, usadosPor, 8));
    }
    for (int v = 6; v <= 7; v++) {
        grafo.definirChave(v, sortearSemRepetir(poolLog, usadosLog, 8));
    }

    int cristais = 2;
    int trilha[MAX_VERTICES];
    int totalTrilha = 0;
    int atual = 0;
    trilha[totalTrilha] = atual;
    totalTrilha++;
    string pausa;

    while (atual != 8 && atual != 9) {
        limparTela();

        Desafio* desafio = tabelaDesafios.buscar(grafo.obterChave(atual));
        if (desafio == NULL) {
            cout << "\n\tOps! Esse local perdeu sua pergunta. Você foi levado para a Floresta do Esquecimento.\n";
            atual = 9;
            trilha[totalTrilha] = atual;
            totalTrilha++;
            break;
        }

        cout << "\n\tVocê está em: " << grafo.nomeVertice(atual) << "\n";
        cout << "\tNível " << desafio->nivel << " - " << desafio->categoria << "\n";
        cout << "\tCristais: " << cristais << "\n";

        time_t inicio = time(0); //conta a partir daqui, incluindo o tempo gasto no menu de poderes
        int bonusTempo = 0;
        string resposta = "";
        while (resposta == "") {
            cout << "\n\t" << desafio->pergunta << "\n";
            int restante = desafio->tempo + bonusTempo - (int)(time(0) - inicio);
            if (restante > 0) {
                cout << "\t(Você tem " << restante << " segundos para ganhar todos os cristais)\n";
            } else {
                cout << "\t(O tempo do bônus acabou, mas você ainda pode responder!)\n";
            }
            cout << "\n\tDigite a sua RESPOSTA e aperte ENTER.\n\t(Para usar um poder, digite P)\n\t> ";
            string entrada;
            getline(cin, entrada);

            if (entrada == "P" || entrada == "p") {
                cout << "\n\tSELECIONE O PODER:\n";
                cout << "\t  1 - DICA\n";
                cout << "\t  2 - TEMPO\n";
                cout << "\t  3 - PULAR\n";
                cout << "\t  0 - Voltar\n";
                cout << "\t> ";
                string opcao;
                getline(cin, opcao);

                string nomePoder = "";
                if (opcao == "1") {
                    nomePoder = "DICA";
                } else if (opcao == "2") {
                    nomePoder = "TEMPO";
                } else if (opcao == "3") {
                    nomePoder = "PULAR";
                } else if (opcao != "0") {
                    cout << "\n\tOpção inválida.\n";
                }

                if (nomePoder != "") {
                    Poder* poder = tabelaPoderes.buscar(nomePoder);
                    if (cristais < poder->custo) {
                        cout << "\n\tVocê não tem cristais suficientes para usar " << nomePoder << ".\n";
                    } else if (nomePoder == "DICA") {
                        cristais = cristais - poder->custo;
                        cout << "\n\tDica: " << desafio->dica << "\n";
                        cout << "\tCristais restantes: " << cristais << "\n";
                    } else if (nomePoder == "TEMPO") {
                        cristais = cristais - poder->custo;
                        bonusTempo = bonusTempo + 15;
                        cout << "\n\tVocê ganhou mais 15 segundos!\n";
                        cout << "\tCristais restantes: " << cristais << "\n";
                    } else if (nomePoder == "PULAR") {
                        int livres = 0;
                        for (int i = 0; i < 8; i++) {
                            if (desafio->nivel == 1 && !usadosMat[i]) {
                                livres++;
                            }
                            if (desafio->nivel == 2 && !usadosPor[i]) {
                                livres++;
                            }
                            if (desafio->nivel == 3 && !usadosLog[i]) {
                                livres++;
                            }
                        }

                        if (livres == 0) {
                            cout << "\n\tNão há outra pergunta deste nível. Seus cristais não foram gastos.\n";
                        } else {
                            string novaChave;
                            if (desafio->nivel == 1) {
                                novaChave = sortearSemRepetir(poolMat, usadosMat, 8);
                            } else if (desafio->nivel == 2) {
                                novaChave = sortearSemRepetir(poolPor, usadosPor, 8);
                            } else {
                                novaChave = sortearSemRepetir(poolLog, usadosLog, 8);
                            }
                            cristais = cristais - poder->custo;
                            grafo.definirChave(atual, novaChave);
                            desafio = tabelaDesafios.buscar(novaChave);
                            inicio = time(0);
                            cout << "\n\tA pergunta foi trocada!\n";
                            cout << "\tCristais restantes: " << cristais << "\n";
                        }
                    }
                }
            } else if (entrada == "") {
                cout << "\n\tVocê não digitou nada. Escreva a sua resposta.\n";
            } else {
                resposta = entrada;
            }
        }
        int tempoGasto = (int)(time(0) - inicio);

        bool acertou = respostaCorreta(resposta, desafio->resposta);
        tabelaDesafios.registrarResultado(desafio->chave, acertou);

        int peso;
        cout << "\n";
        if (acertou) {
            if (tempoGasto <= desafio->tempo + bonusTempo) {
                cristais = cristais + desafio->cristais;
                cout << "\tParabéns! Resposta certa! Você ganhou " << desafio->cristais << " cristais.\n";
            } else {
                int premio = desafio->cristais / 2;
                if (premio < 1) {
                    premio = 1;
                }
                cristais = cristais + premio;
                cout << "\tResposta certa! Mas o tempo passou.\n";
                if (premio == 1) {
                    cout << "\tVocê ganhou 1 cristal.\n";
                } else {
                    cout << "\tVocê ganhou " << premio << " cristais.\n";
                }
            }
            peso = 1;
        } else {
            cout << "\tNão foi dessa vez. A resposta certa era: " << primeiraAlternativa(desafio->resposta) << "\n";
            if (cristais > 0) {
                cristais--;
            }
            peso = 2;
        }

        int proximo = grafo.proximoVertice(atual, peso);
        if (proximo == -1) {
            proximo = 9;
        }
        cout << "\tVocê seguiu para: " << grafo.nomeVertice(proximo) << "\n";
        cout << "\n\tAperte ENTER para continuar...";
        getline(cin, pausa);

        atual = proximo;
        trilha[totalTrilha] = atual;
        totalTrilha++;
    }

    limparTela();
    if (atual == 8) {
        cout << "\n\t*** VITÓRIA! ***\n";
        cout << "\tVocê chegou ao Castelo de Atentia! O reino está salvo!\n";
    } else {
        cout << "\n\t*** Fim de jogo ***\n";
        cout << "\tVocê se perdeu na Floresta do Esquecimento. Tente novamente!\n";
    }

    cout << "\tCristais finais: " << cristais << "\n";
    cout << "\tSua trilha: " << montarTrilhaTexto(grafo, trilha, totalTrilha) << "\n";

    cout << "\n\tCaminho perfeito (o mais curto possível):\n";
    grafo.buscaEmLarguraCaminho(0, 8);

    cout << "\n\tAperte ENTER para continuar...";
    getline(cin, pausa);
}

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    srand(time(0));

    string nomesVertices[MAX_VERTICES] = {
        "Vale dos Números", "Ponte da Soma", "Moinho da Tabuada",
        "Biblioteca Encantada", "Jardim das Palavras", "Torre dos Verbos",
        "Labirinto dos Enigmas", "Portal do Dragão Sábio",
        "Castelo de Atentia", "Floresta do Esquecimento"
    };
    Grafo grafo(MAX_VERTICES, nomesVertices);
    montarMapa(grafo);

    TabelaDesafios tabelaDesafios;
    carregarBanco(tabelaDesafios);

    TabelaPoderes tabelaPoderes;
    carregarPoderes(tabelaPoderes);

    int selecao;

    do {
        cout << "\n\tREINO DE ATENTIA\n";
        cout << "\n\tSELECIONE A OPÇÃO DESEJADA:\n";
        cout << "\t  1 - Jogar\n";
        cout << "\t  2 - Ver o mapa do reino\n";
        cout << "\t  3 - Como jogar\n";
        cout << "\t  4 - Ver os poderes\n";
        cout << "\t  5 - Área do administrador\n";
        cout << "\t  0 - Sair\n";
        cout << "\t> ";
        cin >> selecao;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            selecao = -1; //cai no default: "Opção inválida."
        }

        switch (selecao) {
            case 1:
                cin.ignore();
                jogar(grafo, tabelaDesafios, tabelaPoderes);
                break;

            case 2:
                grafo.mostrarVertices();
                cout << "\n\tQuantidade de caminhos que saem de cada local:\n";
                for (int i = 0; i < MAX_VERTICES; i++) {
                    cout << "\t" << grafo.nomeVertice(i) << ": " << grafo.grauVertice(i) << "\n";
                }
                break;

            case 3:
                mostrarComoJogar();
                break;

            case 4:
                tabelaPoderes.mostrarTabela();
                break;

            case 5:
                cout << "\n\tÁREA DO ADMINISTRADOR\n";
                tabelaDesafios.mostrarDesempenho();
                grafo.mostrarMatriz();
                tabelaDesafios.mostrarTabela();
                cout << "\n\tFator de carga da tabela hash de desafios: " << tabelaDesafios.fatorCarga() << "\n";
                cout << "\n\tBusca em largura (BFS) de 0 até 8:";
                grafo.buscaEmLarguraCaminho(0, 8);
                cout << "\n\tBusca em profundidade (DFS) de 0 até 8:";
                grafo.buscaEmProfundidadeCaminho(0, 8);
                break;

            case 0:
                cout << "\n\tAté a próxima aventura!\n";
                break;

            default:
                cout << "\n\tOpção inválida.\n";
                break;
        }
        cout << "\n";
    } while (selecao != 0);

    return 0;
}
