//+------------------------------------------------------------------+
//|                                                   Trabalho Final |
//|              Alunos: Leonardo Barros Bilhalva  e Santiago Gonzaga|
//|                        Disciplina: Linguagens Formais e Automatos|
//+------------------------------------------------------------------+

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> // count - sort
#include <sstream>   // istringstream
#include <cstdlib>   // clear

using namespace std;

void reseta_alfabeto(vector<string> &estados_novos, vector<string>alfabeto)//
{
    estados_novos.clear();
    estados_novos = alfabeto;

    for(int a =0; a < estados_novos.size(); a++)
    {
        estados_novos[a] = estados_novos[a] + ":";

    }
}

 string acha_caminho(char letra_recebida, string estado_inicial, vector<vector<string>> vetor_afd, vector<string> vetor_de_finais_afd)
    {
        string estado_destino;
        string reserva;
        bool comeca_leitura = false;
        for(int a = 0; a<vetor_afd.size(); a++)                     //percorre a matriz
        {
            if(vetor_afd[a][0] == estado_inicial)                   //se encontrou o estado inicial que foi fornecido, trabalha em cima das transicoes dele
            {                
                for(int b=0; b<vetor_afd[a].size(); b++)            //percorre as transicoes da matriz
                {
                 if(count(vetor_afd[a][b].begin(), vetor_afd[a][b].end(), letra_recebida) > 0)          // confere se a letra recebida esta na transicao atual
                 {
                   reserva.clear();
                   reserva = vetor_afd[a][b].substr(vetor_afd[a][b].find(":") + 1);                     // tira ate o :, pois a transicao esta armazenada como "a:q0q1" e o estado seria somente "q0q1"
                   estado_destino.insert(estado_destino.end(), reserva.begin(), reserva.end());
                   cout << "estado: " << estado_inicial << "----------" << letra_recebida << "---------->" << estado_destino << endl;

                   return estado_destino;                   
                 }                               
                }
                break;
            }
        } 

        if(letra_recebida == ' ')                                                                                       // se recebeu um espaco, chegou ao fim da palavra
        {            
            for(int a = 0; a < vetor_de_finais_afd.size(); a++)
            {
                if(estado_inicial == vetor_de_finais_afd[a])                                                            //se o estado atual for um final, significa que validou a palavra
                {                
                cout << "Nao foram encontrada mais transicoes e o estado atual e final, portanto a palavra foi aceita.";
                
                return "";
                }
            }            
        cout << "Nao foram encontrada mais transicoes e o estado atual nao e final, portanto a palavra nao foi aceita.";    // caso contrario nao validou a palavra
          
        return "";
        }
        else
        {
        cout << "A transicao recebida nao foi aceita por funcao indefinida, portando a palavra nao foi aceita.";        // se nao recebeu um espaco, significa que tem alguma transicao indefinida
        
        return "";
        }

    }

void funcao_um_cria_afn (vector<vector <string>> &vetor_afn, vector<string> &vetor_de_iniciais_afn,  vector<string> &vetor_de_finais_afn, string nome_arquivo, vector<string> &alfabeto)
{
    vetor_afn.clear();
    vetor_de_iniciais_afn.clear();
    vetor_de_finais_afn.clear();

    ifstream leitura_nomes;

    string line;

    string word;

    int contagem_de_iniciais = 0;
    int contagem_troca = false;
    int contagem_de_finais = 0;

    int aux = 0;

    leitura_nomes.open(nome_arquivo);  

    getline(leitura_nomes, line);

    for(int a = 0; a < line.size(); a++)
    {
        if(line[a] == ',' && !contagem_troca)  //comeca contagem troca com false, para comecar lendo quantas virgulas tem os inciais, ou seja, quantos estados iniciais temos
        contagem_de_iniciais ++;

        if(line[a] == '{') // ao encontrar o {, comecaremos a ler os estados finais. entao troca 'contagem_troca' fica true para adicionarmos oas finais
        contagem_troca = true;

        if((line[a] == ',' || line[a] == '}') && contagem_troca) // conta a quantidade de estados finais
        contagem_de_finais ++;
    }

    istringstream var(line);

    getline(var, line, '(');

    aux = contagem_de_iniciais;
    while( aux > 0)
    {
    getline(var, line, ',');
    vetor_de_iniciais_afn.push_back(line);
    aux --;
    }

    getline(var, line, '{');


    aux = contagem_de_finais-1;           // -1, pois o ultimo nao tem virgula no final
    while( aux > 0)
    {
    getline(var, line, ',');
    vetor_de_finais_afn.push_back(line);
    aux --;
    }    

    getline(var, line, '}');   //hardcoded por necessidade
    vetor_de_finais_afn.push_back(line);

    aux = -1;
    vector <string> vetor_auxiliar;    
    while(getline(leitura_nomes, line))
    {        
      if(count(line.begin(), line.end(), ':') > 0)
      {
      vetor_auxiliar.clear();
      vetor_auxiliar.push_back(line);
      
      alfabeto.push_back(line.substr(0, line.find(":"))); // pega até a :, ou seja, so pega a letra da transicao

      vetor_afn[aux].push_back(line);
      continue;
      }
    vetor_auxiliar.clear();
    vetor_auxiliar.push_back(line);

    vetor_afn.push_back(vetor_auxiliar);
    aux ++;
    }	

    leitura_nomes.close();

    sort(alfabeto.begin(), alfabeto.end());
    alfabeto.erase(unique(alfabeto.begin(), alfabeto.end()), alfabeto.end()); // deixa o vetor com só um simbolo para cada transicao

    if(vetor_de_iniciais_afn[0].empty())
    {
    cout << "O afn inicializado nao possui estado inicial. Confira a inicializacao do arquivo se isso nao era o objetivo do afn." << endl << "Lembrando que a inicializacao segue o padrao:" << endl << "AUTOMATO=(q0,{q2})"  << endl <<  "Onde q0 e o unico estado inicial e q2 e o unico estado final. TODOS os caracteres especiais devem ser respeitados!" << endl;
    cout << "Insira qualquer tecla para voltar para o menu";
    fflush(stdin);
    getchar();
    }
    if(vetor_de_finais_afn[0].empty())
    {
    cout << "O afn inicializado nao possui estados finais. Confira a inicializacao do arquivo se isso nao era o objetivo do afn." << endl << "Lembrando que a inicializacao segue o padrao:" << endl << "AUTOMATO=(q0,{q2})"  << endl <<  "Onde q0 e o unico estado inicial e q2 e o unico estado final. TODOS os caracteres especiais devem ser respeitados!" << endl;
    cout << "Insira qualquer tecla para voltar para o menu";
    fflush(stdin);
    getchar();
    }
    if(vetor_de_iniciais_afn.size() > 1)
    {
    cout << "O afn inicializado possui mais de um estado inicial. Isso e invalido de acordo com o conteudo apresentado durante a cadeira. Mude os estados iniciais para que o automato seja aceito." << endl << "Lembrando que a inicializacao segue o padrao:" << endl << "AUTOMATO=(q0,{q2})"  << endl <<  "Onde q0 e o unico estado inicial e q2 e o unico estado final. TODOS os caracteres especiais devem ser respeitados!" << endl;
    cout << "Insira qualquer tecla para voltar para o menu";
    fflush(stdin);
    getchar();
    }
}

void funcao_um_afn_para_afd(vector<vector <string>> vetor_afn, vector<string> vetor_de_iniciais_afn,  vector<string> vetor_de_finais_afn, vector<vector <string>> &vetor_afd, vector<string> &vetor_de_iniciais_afd,  vector<string> &vetor_de_finais_afd, vector<string> alfabeto)
{
    //
    vector<string> estados_novos;
    vector<string> vetor_auxiliar;
    string auxiliar;
    string sem_espacos;
    string sem_espacos_2;
    string sem_espacos_3;
    bool efetua_troca;
    vetor_de_iniciais_afd = vetor_de_iniciais_afn;          // estados iniciais são os mesmos dos finais
    vetor_afd[0] = vetor_de_iniciais_afd;                   //inicializa o primeiro estado do afd
        

    for(int a =0; a < vetor_afd.size(); a++)
    {
        //cout << vetor_afd[a][0] << endl;
        istringstream var(vetor_afd[a][0]); // vai cortando os espacos para saber cada estado -> um exemplo de estado nesse vetor seria "q0 q1 q2 qf", onde os istringstream cortariam os espacos
        reseta_alfabeto(estados_novos, alfabeto);


        while(var >> auxiliar)  // cortando as os espaços dos estados
        {
            for(int b =0; b< vetor_afn.size(); b++) // percorrendo todos estados do afn
            {
                if(vetor_afn[b][0] == auxiliar) // se estou no estado atual
                {      
                    for(int c = 1; c< vetor_afn[b].size(); c++)
                    {
                        for(int d =0; d<alfabeto.size(); d++)
                        {            
                            if(vetor_afn[b][c].find(alfabeto[d]))
                            {        
                              for(int e = 0; e<estados_novos.size(); e++)
                              {
                                  if(estados_novos[e].find(alfabeto[d]))
                                  {
                                    estados_novos[e] = estados_novos[e] + " " + vetor_afn[b][c].substr(vetor_afn[b][c].find(":") + 1);
                                  }
                              }                              
                            break;  // uma transicao nao pode ser igual a mais de uma letra do alfabeto, por isso eu quebro esse for
                            }
                        }

                    }
                 break; // só vai ser igual a um estado por vez
                }
            }           
        }


        for(int b = 0; b<estados_novos.size(); b++)
        {
            vetor_auxiliar.clear();                                                                                        //limpa vetor auxiliar          
       
            vetor_auxiliar.push_back(estados_novos[b].substr(estados_novos[b].find(":") + 1));                             //aloca substring que contem só os estados. Exemplo: a: q0 q1 -> "q0 q1" é a string alocada 
            vetor_afd.insert(vetor_afd.end(), vetor_auxiliar);                                                              //essa substring será o próximo estado a percorrer 

            estados_novos[b].erase(remove(estados_novos[b].begin(), estados_novos[b].end(), ' '), estados_novos[b].end()); // limpa espaço em branco nas strings

            if(estados_novos[b].substr(estados_novos[b].find(":") + 1).empty())                                            // se não tivermos algo depois dos ':', não precisa adicionar a transicao
            continue;

            vetor_afd[a].insert(vetor_afd[a].end(), estados_novos[b]);                                                     //aloca a transicao no local correto ao longo do vetor atual
        }

        for(int b = 0; b < vetor_afd.size(); b++)                                                                          //limpeza caso: adicionar um estado a matriz que ja foi percorrido. temos que excluir este estado
        {
            for(int c =b+1; c < vetor_afd.size(); c++)
            {
                sem_espacos = vetor_afd[c][0]; 
                sem_espacos_2 = vetor_afd[b][0];
                sem_espacos_3 = vetor_afd[c][0];
                sem_espacos.erase(remove(sem_espacos.begin(), sem_espacos.end(), ' '), sem_espacos.end());
                if(sem_espacos == sem_espacos_2 || sem_espacos_2 == sem_espacos_3)
                {
                vetor_afd.erase(vetor_afd.begin() + c, vetor_afd.begin() + c + 1); 
                }
            }
        }

    }


    for(int a = 0; a < vetor_afd.size() && !vetor_de_finais_afn[0].empty(); a++)    //loop quando tem estados finais                                   
    {
        vetor_afd[a][0].erase(remove(vetor_afd[a][0].begin(), vetor_afd[a][0].end(), ' '), vetor_afd[a][0].end());            // remove os espacos em brancos dos estados iniciais de cada transicao. Lembrando que esses espacos foram adicionados e utilizados no tratamento da criacao do afd.

        for(int b = 0; b< vetor_de_finais_afn.size(); b++)                          //criação dos estados finais do afd
        {
            if(vetor_afd[a][0].find(vetor_de_finais_afn[b]) != string::npos)        //se achou um estado final do afn dentro dos novos estados do afd, esse estado é final para afd
            vetor_de_finais_afd.push_back(vetor_afd[a][0]);
        }
    }

    for(int a = 0; a < vetor_afd.size() && vetor_de_finais_afn[0].empty(); a++)     // loop quando nao tem estados finais                                   
    {
        vetor_afd[a][0].erase(remove(vetor_afd[a][0].begin(), vetor_afd[a][0].end(), ' '), vetor_afd[a][0].end());            // remove os espacos em brancos dos estados iniciais de cada transicao. Lembrando que esses espacos foram adicionados e utilizados no tratamento da criacao do afd.
    }

    sort(vetor_de_finais_afd.begin(), vetor_de_finais_afd.end());
    vetor_de_finais_afd.erase(unique(vetor_de_finais_afd.begin(), vetor_de_finais_afd.end()), vetor_de_finais_afd.end()); // deixa o vetor com um so estado final para cada duplicada 

    //cout << "Insira qualquer tecla para voltar para o menu";
    //fflush(stdin);
    //getchar();
}

void funcao_dois (vector<vector <string>> vetor_afd, vector<string>vetor_de_iniciais_afd,  vector<string> vetor_de_finais_afd) // funcao que verifica a palavra
{
    string palavra;

    system("CLS"); // limpando a tela
    cout << "Entre a palavra escolhida para ser verificada: ";
    fflush(stdin);
    cin >> palavra;
    cout << endl << "verificacao para palavra: " << palavra << endl;
    palavra = palavra + " ";                                                //esse espaco em branco que garante o fim da palavra
    string estado_atual = vetor_de_iniciais_afd[0];                         //o primeiro estado atual e sempre o estado inicial do automato
    cout << "estado inicial: " << estado_atual << endl;

    for(int a = 0; a<palavra.size(); a++)
    {
       estado_atual = acha_caminho(palavra[a], estado_atual, vetor_afd, vetor_de_finais_afd);

       if(estado_atual == "")                                                               // caractere recebido que indica o fim da funcao
       break;
    }

    cout << endl << "Insira qualquer tecla para voltar para o menu";
    fflush(stdin);
    getchar();
}

void funcao_tres (vector<vector <string>> vetor_afn, vector<string> vetor_de_iniciais_afn,  vector<string> vetor_de_finais_afn, vector<vector <string>> vetor_afd, vector<string>vetor_de_iniciais_afd,  vector<string> vetor_de_finais_afd, vector<string> alfabeto) // funcao que verifica propriedades para a linguagem
{
    system("CLS"); // limpando a tela

    if(vetor_de_finais_afd.size() > 1)    // lembrando que a existencia de um estado final nesse vetor garante a acessibilidade a ele por meio de palavra, graças ao metodo utilizado para criar a matriz afd                                             
    {                                     // tem que ser maior que 1, pois eu sempre inicio o vetor de finais com um estado vazio no primeiro elemento. Essa inicializacao e fundamental para o funcionamento do codigo! Maior que 1 seria 2, ou seja, o vazio e mais um elemento(final).
        cout << "A linguagem nao e vazia, pois temos pelo menos um estado final acessavel por meio de palavras." << endl << endl;
    }
    else                                                                                                                     // se o vetor de finais esta vazio, significa que nao temos estados finais. sem estados finais => palavra vazia
    {
        cout << "A linguagem e vazia, pois nao temos pelo menos um estado final acessavel por meio de palavras." << endl << endl;
    }

    string auxiliar_estado_atual;                                                                                               //após isso, testaremos linguagem finita e infinita
    string auxiliar_transicao;
    string auxiliar_transicao_completo;
    bool loop = false;

    for(int a = 0; a < vetor_afd.size() && !loop; a++)                                             //percorre a matriz
    {
        auxiliar_estado_atual = vetor_afd[a][0];                                                   //anota o estado inicial
        for(int b = 1; b < vetor_afd[a].size() && !loop; b++ )                                      //percorre as transicoes
        {            
            for(int c = 0; c <= a && !loop; c++)                                                    //percorre a matriz ate o estado inicial atual
            {
            auxiliar_transicao = vetor_afd[c][b].substr(vetor_afd[c][b].find(":") + 1);             //anota a transicao atual apos o :, mesma logica ja utilizada anteriormente             
            if(auxiliar_estado_atual == auxiliar_transicao && !loop)                                //se a transicao atual apontar para o estado atual ou algum estado anterior, temos um loop
            {
                auxiliar_transicao_completo = vetor_afd[c][b];
                //encontramos um loop valido!
                //agora precisamos testar se o estado atual funciona como
                loop = true;
            }
                
            }
        }
    }

                if(loop && vetor_de_finais_afd.size() > 1)                                         //se tivermos um loop e o vetor de finais nao for vazio, significa que a linguagem e infinita
                {                  
                cout << "O estado " << auxiliar_estado_atual << " transiciona " << auxiliar_transicao_completo << ". Essa transicao da em um estado predecessor ao " << auxiliar_estado_atual << " formando um loop." << endl;
                cout << "O estado " << auxiliar_estado_atual << " consegue chegar a um estado final, o que e evidenciado dentro das transicoes da matriz seguir: " << endl;
                cout << vetor_de_finais_afd[0] << endl;

                for(int a =0; a< vetor_afd.size(); a++)
                {
                    for(int b =0; b< vetor_afd[a].size(); b++)
                    {
                        cout << vetor_afd[a][b] << " ";
                    }
                    cout << endl;
                }
                cout << "A linguagem e infinita, ou seja, possui um loop em uma das palavras que a completa. " << endl;
                }
                else
                {
                if(vetor_de_finais_afd.size() > 1)                                                                         
                cout << "A linguagem e finita, ou seja, nao possui um loop em uma das palavras que a completa."<< endl;            // aqui nao temos loop, mas temos palavra formada -> linguagem finita
                else
                {
                cout << "A linguagem e finita, ou seja, nao possui um loop em uma das palavras que a completa. A linguagem tambem nao possui estados finais, impossibilitando a existencia desse loop."<< endl; // aqui temos loop, mas nao temos palavra formada -> linguagem finita
                }
                }            



    cout << "Insira qualquer tecla para voltar para o menu";
    fflush(stdin);
    getchar();
}

void funcao_quatro(vector<vector <string>> vetor_afn, vector<string>vetor_de_iniciais_afn,  vector<string> vetor_de_finais_afn, vector<vector <string>> vetor_afd, vector<string>vetor_de_iniciais_afd,  vector<string> vetor_de_finais_afd)
{

    system("CLS");
    cout << "Matriz do afn:" << endl << "A sua leitura se da por estados na primeira coluna e respectivas derivacoes nas suas linhas. Exemplo: q0 a:q1 b:q2 e o estado q0 que deriva com a para q1 e com b para q2" << endl;

    for (int i = 0; i < vetor_afn.size(); i++) {                           //Deixei aqui essa funcao para conferir a leitura
        for (int j = 0; j < vetor_afn[i].size(); j++)
            cout << vetor_afn[i][j] << " ";
        cout << endl;
    }

    cout << "Estados iniciais do afn:" << endl;
    for(int i = 0; i < vetor_de_iniciais_afn.size(); i++)
    {
        cout << vetor_de_iniciais_afn[i] << " ";
    }
    cout << endl;    

    cout << "Estados finais do afn:" << endl;
    for(int i = 0; i < vetor_de_finais_afn.size(); i++)
    {
        cout << vetor_de_finais_afn[i] << " ";
    }
    cout << endl << endl;

    cout << "Matriz do afd:" << endl;

    for (int i = 0; i < vetor_afd.size(); i++) {                           //Deixei aqui essa funcao para conferir a leitura
        for (int j = 0; j < vetor_afd[i].size(); j++)
            cout << vetor_afd[i][j] << " ";
        cout << endl;
    }

    cout << "Estados iniciais do afd:" << endl;
    for(int i = 0; i < vetor_de_iniciais_afd.size(); i++)
    {
        cout << vetor_de_iniciais_afd[i] << " ";
    }
    cout << endl;    

    cout << "Estados finais do afd:" << endl;
    for(int i = 0; i < vetor_de_finais_afd.size(); i++)
    {
        cout << vetor_de_finais_afd[i] << " ";
    }
    cout << endl << endl;

    cout << "Insira qualquer tecla para voltar para o menu";
    fflush(stdin);
    getchar();
}

int main(){
    cout << "Laboratorio comecou" << endl;
      
    vector<vector<string>> vetor_afn;
    vector<string> vetor_de_iniciais_afn;
    vector<string> vetor_de_finais_afn;  

    vector<vector<string>> vetor_afd(1);
    vector<string> vetor_de_iniciais_afd(1);
    vector<string> vetor_de_finais_afd(1);

    vector<string> alfabeto;
    
    string nome_arquivo_usuario;
    int selecao_menu = 0;
    bool booleano_nome_arquivo = false;
    bool booleano_afd_pronto = false;


    while (selecao_menu != 5)
    {
        system("CLS");
        cout << "Menu principal do programa" << endl;
        cout << "1. Entra nome do arquivo afn que sera transformado em afd" << endl;
        cout << "2. Entra palavra para verificar a validade de acordo com afd" << endl;
        cout << "3. Verificar se propriedades para a linguagem afd" << endl;
        cout << "4. Imprimir o afn fornecido pelo usuario e o afd produzido para consulta visual" << endl;
        cout << "5. Finalizar o programa" << endl;

        while (selecao_menu < 1 || selecao_menu > 5)
        {
            selecao_menu = 0;
            cout << endl << "Insira o numero da funcao escolhida : ";
            fflush(stdin);
            cin >> selecao_menu;

            if (selecao_menu < 1 || selecao_menu > 5)
            cout << "O numero da funcao inserido foi invalido." << endl;

            if(cin.fail())                                                  //corrige se enviou algo que nao um int, como uma string, por exemplo
            cin.clear();
        }

        if (selecao_menu == 1)
        {
            system("CLS");
            cout <<"Entre o nome do arquivo a ser processado. O nome do arquivo deve ser inserido sem a extensao dele(exemplo: .txt)" << endl;
            fflush(stdin);
            cin >> nome_arquivo_usuario;
            cout << "O nome selecionado foi : " << nome_arquivo_usuario << endl;
            cout << "Se esse nao for o nome desejado, tente inserir outro nome voltando a essa funcao pelo menu" << endl << endl;
            nome_arquivo_usuario = nome_arquivo_usuario + ".txt";

            booleano_nome_arquivo = true;

            funcao_um_cria_afn(vetor_afn, vetor_de_iniciais_afn, vetor_de_finais_afn, nome_arquivo_usuario, alfabeto);
            if(vetor_de_iniciais_afn.size() < 2)
            {
            funcao_um_afn_para_afd(vetor_afn, vetor_de_iniciais_afn, vetor_de_finais_afn, vetor_afd, vetor_de_iniciais_afd, vetor_de_finais_afd, alfabeto); // funcao 1 -> afn para afd
            booleano_afd_pronto = true;
            }
            fflush(stdin);
            selecao_menu = 0;
        }

        if (selecao_menu == 2)
        {
            if (!booleano_nome_arquivo || !booleano_afd_pronto)
            {      
            if(!booleano_nome_arquivo)     
            cout << "Nome do arquivo afn nao iniciado. Execute a funcao 1 primeiro para o programa executar corretamente." << endl;
            if(!booleano_afd_pronto)
            cout << "AFD nao foi transformado. Execute a funcao 1 primeiro para o programa executar corretamente." << endl;
            cout << "Insira qualquer tecla para voltar para o menu";
            fflush(stdin);
            getchar();
            selecao_menu = 0;
            }
            else
            {

            funcao_dois(vetor_afd, vetor_de_iniciais_afd, vetor_de_finais_afd); // funcao 2 -> verifica a palavra
            fflush(stdin);
            selecao_menu = 0;
            }
        }

        if (selecao_menu == 3)
        {
            if (!booleano_nome_arquivo || !booleano_afd_pronto)
            {           
            if(!booleano_nome_arquivo)     
            cout << "Nome do arquivo afn nao iniciado. Execute a funcao 1 primeiro para o programa executar corretamente." << endl;
            if(!booleano_afd_pronto)
            cout << "AFD nao foi transformado. Execute a funcao 2 primeiro para o programa executar corretamente." << endl;
            cout << "Insira qualquer tecla para voltar para o menu";
            fflush(stdin);
            getchar();
            selecao_menu = 0;
            }
            else
            {
            funcao_tres(vetor_afn, vetor_de_iniciais_afn, vetor_de_finais_afn, vetor_afd, vetor_de_iniciais_afd, vetor_de_finais_afd, alfabeto); // funcao 3 - verifica propriedades da linguagem
            fflush(stdin);
            selecao_menu = 0;
            }
        }

        if (selecao_menu == 4)
        {
            if (!booleano_nome_arquivo || !booleano_afd_pronto)
            {                
            cout << "Nome do arquivo afn nao iniciado. Execute a funcao 1 primeiro para o programa executar corretamente." << endl;
            cout << "Insira qualquer tecla para voltar para o menu";
            fflush(stdin);
            getchar();
            selecao_menu = 0;
            }
            else
            {
            funcao_quatro(vetor_afn, vetor_de_iniciais_afn, vetor_de_finais_afn, vetor_afd, vetor_de_iniciais_afd, vetor_de_finais_afd);
            fflush(stdin);
            selecao_menu = 0;
            }
        }

    } // while

    cout << endl;
    cout << "Laboratorio final terminou :)";
    return 0;
}
