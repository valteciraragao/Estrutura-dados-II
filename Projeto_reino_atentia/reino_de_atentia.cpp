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
const string MATERIAS[3] = {"Matemática", "Português", "Lógica"}; //índice = nível - 1

struct Desafio {
    string chave;
    string categoria;
    string pergunta;
    string resposta; //alternativas aceitas separadas por |
    string dica;
    int nivel;
    int tempo; //segundos para ganhar o prêmio cheio
    int cristais;
    int acertos;
    int erros;
};

struct Poder {
    string chave;
    string efeito;
    int custo;
};

struct Jogador {
    string nome;
    string chave; //nome normalizado, assim "Ana" e "ana" são a mesma pessoa
    int partidas;
    int vitorias;
    int melhorCristais;
    int acertos[3];
    int erros[3];
};

string removerAcentos(string s) {
    string comAcento[24] = {"á", "à", "â", "ã", "é", "ê", "í", "ó", "ô", "õ", "ú", "ç",
                             "Á", "À", "Â", "Ã", "É", "Ê", "Í", "Ó", "Ô", "Õ", "Ú", "Ç"};
    string semAcento[24] = {"a", "a", "a", "a", "e", "e", "i", "o", "o", "o", "u", "c",
                             "a", "a", "a", "a", "e", "e", "i", "o", "o", "o", "u", "c"};

    for (int i = 0; i < 24; i++) {
        size_t posicao = s.find(comAcento[i]);
        while (posicao != string::npos) {
            s.replace(posicao, comAcento[i].length(), semAcento[i]);
            posicao = s.find(comAcento[i], posicao + semAcento[i].length());
        }
    }
    return s;
}

//tira espaços, acentos e maiúsculas para comparar respostas e nomes
string normalizarTexto(string s) {
    while (s.length() > 0 && s[0] == ' ') {
        s.erase(0, 1);
    }
    while (s.length() > 0 && s[s.length() - 1] == ' ') {
        s.erase(s.length() - 1, 1);
    }

    s = removerAcentos(s);

    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
    }
    return s;
}

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
            chaveDesafio[i] = "";
        }
    }

    void adicionarAresta(int v1, int v2, int peso) {
        if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices && peso > 0) {
            matrizAdj[v1][v2] = peso;
            //grafo direcionado: removida a linha de volta
        }
    }

    void mostrarMatriz() {
        cout << "\n\tMatriz de Adjacências:\n\t";
        for (int j = 0; j < numVertices; j++) {
            cout << "\t" << j;
        }
        cout << "\n";

        for (int i = 0; i < numVertices; i++) {
            cout << "\t" << i;
            for (int j = 0; j < numVertices; j++) {
                cout << "\t" << matrizAdj[i][j];
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
        return NULL;
    }

    //varredura: soma acertos e erros de uma matéria
    void somarCategoria(string categoria, int &acertos, int &erros) {
        acertos = 0;
        erros = 0;
        for (int i = 0; i < MAX; i++) {
            No* temp = tabelaHash[i];
            while (temp != NULL) {
                if (temp->item.categoria == categoria) {
                    acertos = acertos + temp->item.acertos;
                    erros = erros + temp->item.erros;
                }
                temp = temp->prox;
            }
        }
    }

    //varredura: guarda ponteiros dos desafios já respondidos
    int copiarRespondidos(Desafio* lista[]) {
        int total = 0;
        for (int i = 0; i < MAX; i++) {
            No* temp = tabelaHash[i];
            while (temp != NULL) {
                if (temp->item.acertos + temp->item.erros > 0) {
                    lista[total] = &(temp->item);
                    total++;
                }
                temp = temp->prox;
            }
        }
        return total;
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
        return NULL;
    }

    void mostrarTabela() {
        cout << "\n\tTabela hash dos poderes:\n";
        for (int i = 0; i < MAX; i++) {
            cout << "\t" << i << " --> ";
            No* temp = tabelaHash[i];
            while (temp != NULL) {
                cout << temp->item.chave << " (custo " << temp->item.custo << ") ";
                temp = temp->prox;
            }
            cout << "\n";
        }
    }
};

class TabelaJogadores {
  private:
    static const int MAX = 7;

    struct No {
        Jogador item;
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
    TabelaJogadores() {
        for (int i = 0; i < MAX; i++) {
            tabelaHash[i] = NULL;
        }
        quantidade = 0;
    }

    ~TabelaJogadores() {
        for (int i = 0; i < MAX; i++) {
            No* temp = tabelaHash[i];
            while (temp != NULL) {
                No* proximo = temp->prox;
                delete temp;
                temp = proximo;
            }
        }
    }

    Jogador* buscar(string chave) {
        int posicao = funcaoHash(chave);
        No* temp = tabelaHash[posicao];
        while (temp != NULL) {
            if (temp->item.chave == chave) {
                return &(temp->item);
            }
            temp = temp->prox;
        }
        return NULL;
    }

    //se ainda não existe, cria o jogador zerado
    Jogador* buscarOuCriar(string nome, string chave) {
        Jogador* j = buscar(chave);
        if (j != NULL) {
            return j;
        }
        No* novo = new No;
        novo->item.nome = nome;
        novo->item.chave = chave;
        novo->item.partidas = 0;
        novo->item.vitorias = 0;
        novo->item.melhorCristais = 0;
        for (int i = 0; i < 3; i++) {
            novo->item.acertos[i] = 0;
            novo->item.erros[i] = 0;
        }
        int posicao = funcaoHash(chave);
        novo->prox = tabelaHash[posicao]; //entra na frente da lista
        tabelaHash[posicao] = novo;
        quantidade++;
        return &(novo->item);
    }

    //varredura: soma partidas e vitórias de todos
    void somarPartidas(int &partidas, int &vitorias) {
        partidas = 0;
        vitorias = 0;
        for (int i = 0; i < MAX; i++) {
            No* temp = tabelaHash[i];
            while (temp != NULL) {
                partidas = partidas + temp->item.partidas;
                vitorias = vitorias + temp->item.vitorias;
                temp = temp->prox;
            }
        }
    }

    void mostrarTabela() {
        cout << "\n\tTabela hash dos jogadores:\n";
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

class TabelaLogin {
  private:
    static const int MAX = 5;

    struct Linha {
        string login;
        string senha;
        int tentativas; //senhas erradas seguidas para esse login
        bool ocupado;
        bool removido;
    };

    Linha tabela[MAX];

    int funcaoHash(string chave) {
        int soma = 0;
        for (int i = 0; i < (int)chave.length(); i++) {
            soma = soma + (int)chave[i];
        }
        return soma % MAX;
    }

  public:
    TabelaLogin() {
        for (int i = 0; i < MAX; i++) {
            tabela[i].ocupado = false;
            tabela[i].removido = false;
            tabela[i].tentativas = 0;
        }
    }

    void inserir(string login, string senha) {
        int posicao = funcaoHash(login);
        for (int i = 0; i < MAX; i++) {
            int indice = (posicao + i) % MAX;
            if (tabela[indice].ocupado == false || tabela[indice].login == login) {
                tabela[indice].login = login;
                tabela[indice].senha = senha;
                tabela[indice].tentativas = 0;
                tabela[indice].ocupado = true;
                tabela[indice].removido = false;
                return;
            }
        }
        cout << "\tErro: tabela cheia, não foi possível inserir " << login << "\n";
    }

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

    void mostrarTabela() {
        cout << "\n\tTabela hash dos logins:\n";
        for (int i = 0; i < MAX; i++) {
            if (tabela[i].ocupado == true) {
                cout << "\t" << i << " --> " << tabela[i].login << "\n"; //a senha não aparece
            } else if (tabela[i].removido == true) {
                cout << "\t" << i << " --> [removido]\n";
            } else {
                cout << "\t" << i << " --> [vazio]\n";
            }
        }
    }
};

int aleatorio(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

//a resposta esperada pode ter várias alternativas separadas por |
bool respostaCorreta(string digitada, string esperada) {
    string alternativa = "";
    for (int i = 0; i <= (int)esperada.length(); i++) {
        if (i == (int)esperada.length() || esperada[i] == '|') {
            if (normalizarTexto(digitada) == normalizarTexto(alternativa)) {
                return true;
            }
            alternativa = "";
        } else {
            alternativa = alternativa + esperada[i];
        }
    }
    return false;
}

string primeiraAlternativa(string esperada) {
    size_t posicao = esperada.find('|');
    if (posicao == string::npos) {
        return esperada;
    }
    return esperada.substr(0, posicao);
}

string sortearSemRepetir(string pool[], bool usados[], int tamanho) {
    int indice = aleatorio(0, tamanho - 1);
    while (usados[indice]) {
        indice = aleatorio(0, tamanho - 1);
    }
    usados[indice] = true;
    return pool[indice];
}

bool temChaveLivre(bool usados[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (!usados[i]) {
            return true;
        }
    }
    return false;
}

void carregarBanco(TabelaDesafios &tabela) {
    //os dois zeros do fim são os acertos e erros da pergunta
    Desafio banco[24] = {
        {"MAT-01", "Matemática", "Quanto é 7 + 8?", "15", "Conte a partir do 8.", 1, 30, 2, 0, 0},
        {"MAT-02", "Matemática", "Quanto é 9 x 6?", "54", "Pense em 9 grupos de 6.", 1, 30, 2, 0, 0},
        {"MAT-03", "Matemática", "Maria tinha 23 figurinhas e ganhou mais 17. Com quantas ficou?", "40", "Some as dezenas e depois as unidades.", 1, 40, 2, 0, 0},
        {"MAT-04", "Matemática", "Qual é a metade de 48?", "24", "Divida por 2.", 1, 25, 2, 0, 0},
        {"MAT-05", "Matemática", "Quanto é 100 - 37?", "63", "Pense em quanto falta de 37 até 100.", 1, 30, 2, 0, 0},
        {"MAT-06", "Matemática", "Um pacote tem 6 balas. Quantas balas há em 4 pacotes?", "24", "Some 6 quatro vezes.", 1, 30, 2, 0, 0},
        {"MAT-07", "Matemática", "Quantos lados tem um triângulo?", "3|três", "Pense no nome da figura.", 1, 20, 2, 0, 0},
        {"MAT-08", "Matemática", "Um lápis custa 5 reais. Quanto custam 3 lápis?", "15|15 reais", "Some 5 três vezes.", 1, 30, 2, 0, 0},

        {"POR-01", "Português", "Qual é o antônimo de \"alegre\"?", "triste", "Pense no sentimento oposto.", 2, 25, 3, 0, 0},
        {"POR-02", "Português", "Qual é o sinônimo de \"veloz\"?", "rápido|ligeiro", "Pense em algo que anda depressa.", 2, 25, 3, 0, 0},
        {"POR-03", "Português", "Na frase \"O gato subiu no telhado\", qual é o verbo?", "subiu", "É a ação que o gato fez.", 2, 30, 3, 0, 0},
        {"POR-04", "Português", "Complete: \"Ele foi ___ escola de bicicleta.\" Digite 1 para \"a\", 2 para \"há\" ou 3 para \"à\".", "3", "Pense em ir PARA algum lugar.", 2, 30, 3, 0, 0},
        {"POR-05", "Português", "Qual destas é um substantivo: correr, bonito, cadeira, rapidamente?", "cadeira", "Substantivo nomeia um objeto ou ser.", 2, 30, 3, 0, 0},
        {"POR-06", "Português", "Como se escreve certo: \"excessão\" ou \"exceção\"?", "exceção", "A palavra correta tem c e ç.", 2, 30, 3, 0, 0},
        {"POR-07", "Português", "Qual é o plural de \"papel\"?", "papéis", "Troque o \"l\" final pelo som \"is\".", 2, 25, 3, 0, 0},
        {"POR-08", "Português", "Qual é o sujeito de \"As crianças brincam no parque\"?", "as crianças|crianças", "É quem faz a ação de brincar.", 2, 30, 3, 0, 0},

        {"LOG-01", "Lógica", "Complete a sequência: 2, 4, 6, 8, ___", "10", "Cada número aumenta de 2 em 2.", 3, 35, 4, 0, 0},
        {"LOG-02", "Lógica", "Se hoje é quarta-feira, que dia será depois de amanhã?", "sexta-feira|sexta|sexta feira", "Conte dois dias a partir de hoje.", 3, 35, 4, 0, 0},
        {"LOG-03", "Lógica", "Complete a sequência: 3, 6, 12, 24, ___", "48", "Cada número é o dobro do anterior.", 3, 40, 4, 0, 0},
        {"LOG-04", "Lógica", "João é mais alto que Pedro, e Pedro é mais alto que Ana. Quem é o mais baixo?", "Ana", "Compare os três em ordem.", 3, 40, 4, 0, 0},
        {"LOG-05", "Lógica", "Qual não pertence ao grupo: quadrado, círculo, triângulo, cachorro?", "cachorro", "Três delas são formas geométricas.", 3, 30, 4, 0, 0},
        {"LOG-06", "Lógica", "Se A=1, B=2, C=3, quanto vale a soma das letras da palavra AC?", "4|quatro", "Some o valor de cada letra.", 3, 40, 4, 0, 0},
        {"LOG-07", "Lógica", "Todos os gatos são animais. Frajola é um gato. O que é Frajola?", "um animal|animal", "Aplique a regra geral ao caso específico.", 3, 35, 4, 0, 0},
        {"LOG-08", "Lógica", "Se hoje é dia 10, que dia será depois de amanhã?", "12|dia 12", "Some dois dias à data de hoje.", 3, 30, 4, 0, 0}
    };

    for (int i = 0; i < 24; i++) {
        tabela.inserir(banco[i]);
    }
}

void montarMapa(Grafo &grafo) {
    grafo.adicionarAresta(0, 3, 1); //peso 1 = caminho de quem acerta
    grafo.adicionarAresta(0, 1, 2); //peso 2 = caminho de quem erra
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

void mostrarDesempenho(TabelaDesafios &tabelaDesafios, TabelaJogadores &tabelaJogadores) {
    int partidas, vitorias;
    tabelaJogadores.somarPartidas(partidas, vitorias);
    if (partidas == 0) {
        cout << "\n\tAinda não há partidas.\n";
        return;
    }
    cout << "\n\tPartidas: " << partidas << "\tVitórias: " << vitorias << "\n";
    cout << "\n\tMatéria\t\t\tRespondidas\tAcertos\t% acerto\n";

    int pior = -1;
    int piorPorcento = 101;
    for (int i = 0; i < 3; i++) {
        int acertos, erros;
        tabelaDesafios.somarCategoria(MATERIAS[i], acertos, erros);
        cout << "\tNível " << i + 1 << " - " << MATERIAS[i] << "\t";
        if (acertos + erros == 0) {
            cout << "sem dados\n";
        } else {
            int porcento = acertos * 100 / (acertos + erros);
            cout << acertos + erros << "\t\t" << acertos << "\t" << porcento << "%\t";
            for (int j = 0; j < porcento / 10; j++) {
                cout << "#"; //um # a cada 10%
            }
            cout << "\n";
            if (porcento < piorPorcento) {
                piorPorcento = porcento;
                pior = i;
            }
        }
    }
    cout << "\n\t>> Matéria com mais dificuldade: " << MATERIAS[pior] << " (" << piorPorcento << "% de acerto)\n";
}

void consultarJogador(TabelaJogadores &tabelaJogadores) {
    string nome;
    cout << "\n\tNome do jogador: ";
    getline(cin, nome);
    Jogador* j = tabelaJogadores.buscar(normalizarTexto(nome));
    if (j == NULL) {
        cout << "\tJogador não encontrado.\n";
        return;
    }

    cout << "\n\tJogador: " << j->nome << "\n";
    cout << "\tPartidas: " << j->partidas << "\tVitórias: " << j->vitorias << "\n";
    cout << "\tMelhor pontuação: " << j->melhorCristais << " cristais\n";
    int pior = -1;
    int piorPorcento = 101;
    for (int i = 0; i < 3; i++) {
        int total = j->acertos[i] + j->erros[i];
        cout << "\t" << MATERIAS[i] << ": " << j->acertos[i] << " acertos, " << j->erros[i] << " erros";
        if (total == 0) {
            cout << " (sem respostas)\n";
        } else {
            int porcento = j->acertos[i] * 100 / total;
            cout << " (" << porcento << "% de acerto)\n";
            if (porcento < piorPorcento) {
                piorPorcento = porcento;
                pior = i;
            }
        }
    }
    if (pior != -1) {
        cout << "\t>> Matéria para reforçar: " << MATERIAS[pior] << "\n";
    }
}

void mostrarMaisDificeis(TabelaDesafios &tabelaDesafios) {
    Desafio* lista[24];
    int total = tabelaDesafios.copiarRespondidos(lista);
    if (total == 0) {
        cout << "\n\tAinda não há partidas.\n";
        return;
    }

    //seleção: a pergunta com maior % de erro vai para a frente
    for (int i = 0; i < total - 1; i++) {
        int maior = i;
        for (int j = i + 1; j < total; j++) {
            int erroJ = lista[j]->erros * 100 / (lista[j]->acertos + lista[j]->erros);
            int erroMaior = lista[maior]->erros * 100 / (lista[maior]->acertos + lista[maior]->erros);
            if (erroJ > erroMaior) {
                maior = j;
            }
        }
        Desafio* aux = lista[i];
        lista[i] = lista[maior];
        lista[maior] = aux;
    }

    cout << "\n\tChave\tVezes\t% erro\tPergunta\n";
    for (int i = 0; i < total && i < 5; i++) {
        int vezes = lista[i]->acertos + lista[i]->erros;
        cout << "\t" << lista[i]->chave << "\t" << vezes << "\t" << lista[i]->erros * 100 / vezes << "%\t" << lista[i]->pergunta << "\n";
    }
}

void areaAdministrador(Grafo &grafo, TabelaDesafios &tabelaDesafios, TabelaPoderes &tabelaPoderes,
                       TabelaJogadores &tabelaJogadores, TabelaLogin &tabelaLogin) {
    int opcao;
    string login, senha;

    do {
        cout << "\n\t*** ÁREA DO ADMINISTRADOR ***\n";
        cout << "\tDados desde que o jogo foi aberto.\n";
        cout << "\n\tSELECIONE A OPÇÃO DESEJADA:\n";
        cout << "\t  1 - Desempenho por matéria\n";
        cout << "\t  2 - Consultar um jogador\n";
        cout << "\t  3 - Perguntas mais difíceis\n";
        cout << "\t  4 - Estruturas de dados\n";
        cout << "\t  5 - Cadastrar novo administrador\n";
        cout << "\t  0 - Voltar\n";
        cin >> opcao;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                mostrarDesempenho(tabelaDesafios, tabelaJogadores);
                break;
            case 2:
                cin.ignore();
                consultarJogador(tabelaJogadores);
                break;
            case 3:
                mostrarMaisDificeis(tabelaDesafios);
                break;
            case 4:
                grafo.mostrarMatriz();
                tabelaDesafios.mostrarTabela();
                cout << "\tFator de carga: " << tabelaDesafios.fatorCarga() << "\n";
                tabelaPoderes.mostrarTabela();
                tabelaJogadores.mostrarTabela();
                cout << "\tFator de carga: " << tabelaJogadores.fatorCarga() << "\n";
                tabelaLogin.mostrarTabela();
                cout << "\n\tBFS (Fila): menor caminho, de quem acerta tudo";
                grafo.buscaEmLarguraCaminho(0, 8);
                cout << "\n\tDFS (Pilha): um caminho completo, o mais longo até a vitória";
                grafo.buscaEmProfundidadeCaminho(0, 8);
                break;
            case 5:
                cin.ignore();
                cout << "\n\tNovo login: ";
                getline(cin, login);
                if (tabelaLogin.existeLogin(login)) {
                    cout << "\tEsse login já existe.\n";
                } else {
                    cout << "\tSenha: ";
                    getline(cin, senha);
                    tabelaLogin.inserir(login, senha);
                    cout << "\tAdministrador cadastrado!\n";
                }
                break;
            case 0:
                break;
            default:
                cout << "\tOpção inválida.\n";
                break;
        }
    } while (opcao != 0);
}

//devolve o poder usado ou "" se voltou
string usarPoder(TabelaPoderes &tabelaPoderes, int &cristais, bool podePular) {
    string chavesPoderes[3] = {"DICA", "TEMPO", "PULAR"};
    string entrada;

    while (true) {
        cout << "\n\tPODERES (você tem " << cristais << " cristais)\n";
        for (int i = 0; i < 3; i++) {
            Poder* poder = tabelaPoderes.buscar(chavesPoderes[i]);
            cout << "\t  " << i + 1 << " - " << poder->chave << "\t(" << poder->custo;
            if (poder->custo == 1) {
                cout << " cristal)";
            } else {
                cout << " cristais)";
            }
            cout << "\t" << poder->efeito << "\n";
        }
        cout << "\t  0 - Voltar para a pergunta\n\tEscolha: ";
        getline(cin, entrada);

        if (entrada == "0") {
            return "";
        }
        if (entrada != "1" && entrada != "2" && entrada != "3") {
            cout << "\tDigite só o número do poder (0 a 3).\n";
        } else {
            Poder* poder = tabelaPoderes.buscar(chavesPoderes[entrada[0] - '1']); //"1" vira a posição 0
            if (poder->chave == "PULAR" && !podePular) {
                cout << "\tNão há outra pergunta para trocar.\n";
            } else if (cristais < poder->custo) {
                cout << "\tVocê precisa de " << poder->custo << " cristais e tem " << cristais << ".\n";
            } else {
                cristais = cristais - poder->custo;
                cout << "\tVocê usou o poder " << poder->chave << "!\n";
                return poder->chave;
            }
        }
    }
}

void jogar(Grafo &grafo, TabelaDesafios &tabelaDesafios, TabelaPoderes &tabelaPoderes, TabelaJogadores &tabelaJogadores) {
    string nome = "";
    while (normalizarTexto(nome) == "") {
        cout << "\n\tQual é o seu nome, aventureiro? ";
        getline(cin, nome);
    }
    cout << "\tBoa sorte, " << nome << "!\n";

    string pools[3][8] = {
        {"MAT-01", "MAT-02", "MAT-03", "MAT-04", "MAT-05", "MAT-06", "MAT-07", "MAT-08"},
        {"POR-01", "POR-02", "POR-03", "POR-04", "POR-05", "POR-06", "POR-07", "POR-08"},
        {"LOG-01", "LOG-02", "LOG-03", "LOG-04", "LOG-05", "LOG-06", "LOG-07", "LOG-08"}
    };
    bool usados[3][8] = {{false}};
    for (int v = 0; v <= 7; v++) {
        int n = v / 3; //0-2 Matemática, 3-5 Português, 6-7 Lógica
        grafo.definirChave(v, sortearSemRepetir(pools[n], usados[n], 8));
    }

    int cristais = 2;
    int acertos[3] = {0, 0, 0};
    int erros[3] = {0, 0, 0};
    int atual = 0;
    int trilha[MAX_VERTICES] = {0}; //começa no Vale dos Números
    int totalTrilha = 1;
    string entrada;

    while (atual != 8 && atual != 9) {
        Desafio* desafio = tabelaDesafios.buscar(grafo.obterChave(atual));
        int n = desafio->nivel - 1;
        time_t inicio = time(0); //o relógio começa quando a pergunta aparece
        int bonusTempo = 0;
        bool mostrarDica = false;
        bool mostrarPergunta = true;
        string resposta = "";

        while (resposta == "") {
            if (mostrarPergunta) {
                int restante = desafio->tempo + bonusTempo - (int)(time(0) - inicio);
                cout << "\n\tVocê está em: " << grafo.nomeVertice(atual) << "\n";
                cout << "\tNível " << desafio->nivel << " - " << desafio->categoria << "\t\tCristais: " << cristais << "\n";
                if (restante > 0) {
                    cout << "\tTempo para o bônus: " << restante << " segundos\n";
                } else {
                    cout << "\tO tempo do bônus já acabou, mas você ainda pode responder!\n";
                }
                cout << "\n\tPERGUNTA: " << desafio->pergunta << "\n";
                if (mostrarDica) {
                    cout << "\tDICA: " << desafio->dica << "\n";
                }
                cout << "\n\tDigite a sua RESPOSTA e aperte ENTER.\n\t(Precisa de ajuda? Digite P para usar um poder.)\n";
            }
            cout << "\t> ";
            getline(cin, entrada);

            mostrarPergunta = false;
            if (normalizarTexto(entrada) == "") {
                cout << "\tDigite a sua resposta ou P para os poderes.\n"; //não conta como erro
            } else if (normalizarTexto(entrada) == "p") {
                string poder = usarPoder(tabelaPoderes, cristais, temChaveLivre(usados[n], 8));
                if (poder == "DICA") {
                    mostrarDica = true;
                } else if (poder == "TEMPO") {
                    bonusTempo = bonusTempo + 15;
                } else if (poder == "PULAR") {
                    string novaChave = sortearSemRepetir(pools[n], usados[n], 8);
                    grafo.definirChave(atual, novaChave);
                    desafio = tabelaDesafios.buscar(novaChave);
                    inicio = time(0); //pergunta nova, relógio novo
                    mostrarDica = false;
                }
                mostrarPergunta = true;
            } else {
                resposta = entrada;
            }
        }

        int tempoGasto = (int)(time(0) - inicio);
        int peso;
        if (respostaCorreta(resposta, desafio->resposta)) {
            desafio->acertos++;
            acertos[n]++;
            peso = 1;
            //fora do tempo ainda avança, só ganha menos
            if (tempoGasto <= desafio->tempo + bonusTempo) {
                cristais = cristais + desafio->cristais;
                cout << "\n\tResposta certa! Você levou " << tempoGasto;
                if (tempoGasto == 1) {
                    cout << " segundo";
                } else {
                    cout << " segundos";
                }
                cout << " e ganhou " << desafio->cristais << " cristais.\n";
            } else {
                int premio = desafio->cristais / 2;
                if (premio < 1) {
                    premio = 1;
                }
                cristais = cristais + premio;
                cout << "\n\tResposta certa! Passou do tempo do bônus, então você ganhou " << premio;
                if (premio == 1) {
                    cout << " cristal.\n";
                } else {
                    cout << " cristais.\n";
                }
                cout << "\tDa próxima vez, use o poder TEMPO!\n";
            }
        } else {
            desafio->erros++;
            erros[n]++;
            peso = 2;
            cout << "\n\tNão foi dessa vez.\n\tA resposta certa era: " << primeiraAlternativa(desafio->resposta) << ".\n";
            cout << "\tDica para a próxima: " << desafio->dica << "\n";
            if (cristais > 0) {
                cristais--;
            }
        }

        int proximo = grafo.proximoVertice(atual, peso);
        if (proximo == -1) {
            proximo = 9;
        }
        cout << "\n\tVocê seguiu para: " << grafo.nomeVertice(proximo) << "\n";
        cout << "\tAperte ENTER para continuar...";
        getline(cin, entrada);
        cout << "\n\n\n\n\n\n\n\n\n\n"; //limpa a tela

        atual = proximo;
        trilha[totalTrilha] = atual;
        totalTrilha++;
    }

    if (atual == 8) {
        cout << "\n\t*** VITÓRIA! ***\n\tParabéns, " << nome << "! Você chegou ao Castelo de Atentia!\n";
    } else {
        cout << "\n\t*** FIM DE JOGO ***\n\t" << nome << ", você se perdeu na Floresta do Esquecimento. Tente novamente!\n";
    }
    cout << "\tCristais finais: " << cristais << "\n";

    string caminho = "Fim";
    for (int i = totalTrilha - 1; i >= 0; i--) {
        caminho = grafo.nomeVertice(trilha[i]) + " -> " + caminho;
    }
    cout << "\tSua trilha: " << caminho << "\n";

    cout << "\n\tResumo da partida:\n";
    for (int i = 0; i < 3; i++) {
        cout << "\t  " << MATERIAS[i] << ": " << acertos[i] << " acertos, " << erros[i] << " erros\n";
    }
    cout << "\n\tCaminho perfeito (o mais curto possível):";
    grafo.buscaEmLarguraCaminho(0, 8);
    cout << "\n\tAperte ENTER para continuar...";
    getline(cin, entrada);

    Jogador* jogador = tabelaJogadores.buscarOuCriar(nome, normalizarTexto(nome));
    jogador->partidas++;
    if (atual == 8) {
        jogador->vitorias++;
    }
    if (cristais > jogador->melhorCristais) {
        jogador->melhorCristais = cristais;
    }
    for (int i = 0; i < 3; i++) {
        jogador->acertos[i] = jogador->acertos[i] + acertos[i];
        jogador->erros[i] = jogador->erros[i] + erros[i];
    }
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
    Poder poderes[3] = {
        {"DICA", "Mostra uma dica da pergunta", 3},
        {"TEMPO", "Mais 15 segundos para o bônus", 1},
        {"PULAR", "Troca por outra pergunta do mesmo nível", 2}
    };
    for (int i = 0; i < 3; i++) {
        tabelaPoderes.inserir(poderes[i]);
    }

    TabelaJogadores tabelaJogadores;
    TabelaLogin tabelaLogin;
    tabelaLogin.inserir("admin", "atentia");

    int selecao;
    string login, senha, resultado;

    do {
        cout << "\n\t*** REINO DE ATENTIA ***\n";
        cout << "\n\tSELECIONE A OPÇÃO DESEJADA:\n";
        cout << "\t  0 - Sair\n";
        cout << "\t  1 - Jogar\n";
        cout << "\t  2 - Ver o mapa do reino\n";
        cout << "\t  3 - Como jogar\n";
        cout << "\t  4 - Área do administrador\n";
        cin >> selecao;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            selecao = -1;
        }

        switch (selecao) {
            case 1:
                cin.ignore();
                jogar(grafo, tabelaDesafios, tabelaPoderes, tabelaJogadores);
                break;
            case 2:
                grafo.mostrarVertices();
                cout << "\n\tCaminhos que saem de cada local:\n";
                for (int i = 0; i < MAX_VERTICES; i++) {
                    cout << "\t" << grafo.nomeVertice(i) << ": " << grafo.grauVertice(i) << "\n";
                }
                break;
            case 3:
                cout << "\n\t*** COMO JOGAR ***\n\tResponda perguntas de Matemática, Português e Lógica até chegar ao Castelo.\n";
                cout << "\tAcertou: avança. Errou: tenta outro lugar do mesmo nível. Errou o nível todo: a aventura acaba.\n";
                cout << "\tAcertou dentro do tempo do bônus: ganha todos os cristais. Depois do tempo: ganha metade.\n";
                cout << "\tDigite P no lugar da resposta para usar um poder (DICA, TEMPO ou PULAR).\n";
                break;
            case 4:
                cin.ignore();
                cout << "\n\tLogin: ";
                getline(cin, login);
                cout << "\tSenha: ";
                getline(cin, senha);
                resultado = tabelaLogin.buscar(login, senha);
                cout << "\t" << resultado << "\n";
                if (resultado == "Login e senha corretos") {
                    areaAdministrador(grafo, tabelaDesafios, tabelaPoderes, tabelaJogadores, tabelaLogin);
                }
                break;
            case 0:
                cout << "\tAté a próxima aventura!\n";
                break;
            default:
                cout << "\tOpção inválida.\n";
                break;
        }
    } while (selecao != 0);

    return 0;
}
