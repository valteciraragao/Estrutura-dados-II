#include <iostream>
#include <string>
using namespace std;

class Estante{
    private:
        const int MAX = 10;
        string livros[MAX];
    public:
        void cadastrarLivro(string titulo){
            bool cadastrou = false;
            for(int i = 0; i < MAX; i++){
                if(livros[i] == ""){
                    livros[i] = titulo;
                    cadastrou = true;
                    break;
                }
            }

            if(!cadastrou){
                cout << "\n\tEstante encontra-se cheia!\n";
            }
        }

        
};
