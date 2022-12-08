#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <locale.h>

#include "Occupation.h"

#define Limpar() system("cls")


int cod_AtualUsuario, cod_UltimoUsuario, num_cadastros;
char informacoes[10][100];


//PARA CONGELAR A TELA
void Wait(int Seconds)///congelar a tela
{
    clock_t endwait;///congelar a tela
    endwait = clock() + Seconds* CLK_TCK;///congelar a tela
    while(clock() < endwait){}///congelar a tela
}

//ESTRUTURAS
typedef struct
{
    char Nome[30];
    char CPF[15];
    char Contato[30];
    char Endereco[50];
    char marca[30];
    char Modelo[30];
    char placa[15];
}Cadastro;

typedef struct
{
  char Nome[30];
  char tipo[20];
  char CNPJ[30];
  char Contato[30];
  char Email[50];
  char Endereco[30];
}Fornecedor;

typedef struct
{
  int Codigo;
  char Nome[20];
  char Categoria[15];
  float Valor_compra;
  float Valor_Venda;
  char Marca[20];
}produto;

typedef struct
{
  char placa[20];
  char modelo[20];
  char marca[20];

}veiculo;

typedef struct
{
    char nome[30];
    char endereco[30];
    char contato[30];
    char CNPJ[30];
}oficina;

typedef struct
{
  int Codigo;
  char Nome[150];
  char Marca[30];
  float Preco;
  char Cor[30];
  int Num_em_estoque;
}estoque;

typedef struct
{
  char usuario[20];
  char senha1[20];
}login;

typedef struct {
  int dia,mes,ano;
  char nome[30];
  char mecanico[20];
  char item[100];
  float valor;
}servico;


//ARQUIVOS
FILE *Arq_princ;
FILE *Aux;
FILE *arquivo;
FILE *arq_ordem;

///FUNÇÃO PARA ALTERA A COR DA TELA E DAS LETRAS
void textColor(int letras, int fundo){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),letras+fundo);
}

///CORES PARA AS LETRAS
enum
{
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROUN,
    LIGHTGRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE,
};

///CORES DE FUNDO DE TELA
enum
{
     _BLACK=0,
     _BLUE=16,
     _GREEN=32,
     _CYAN=48,
     _RED=64,
     _MAGENTA=80,
     _BROUN=96,
     _LIGHTGRAY=112,
     _DARKGRAY=126,
     _LIGHTBLUE=144,
     _LIGHTGREEN=160,
     _LIGHTCYAN=176,
     _LIGHTRED=192,
     _LIGHTMAGENTA=206,
     _YELLOW=224,
     _WHITE=240,
};


///CURSOR DE TELA
void gotoxy(int x , int y)
{
  HANDLE h;
  COORD dwpos;
  dwpos.X = x;
  dwpos.Y = y;
  h =
  GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(h,dwpos);
}

void Imprime_Login_Entrar()
{
    system("cls");
    printf( "+=================================================================+\n"
            "|                             Entrar                              |\n"
            "+=================================================================+\n");
}

void Imprime_Login_Cadastro()
{
    system("cls");
    printf( "+=================================================================+\n"
            "|                           Novo Usuário                          |\n"
            "+=================================================================+\n");
}

void Menu_Login()
{// Função para a impressão e seleção do menu de login.

    /// DECLARAÇÃO DE VARIAVEIS
    int opcao;

    /// IMPRESSÃO DO MENU
    system("cls");
    printf( "+=================================================================+\n"
            "|                              Login                              |\n"
            "+=================================================================+\n"
            " [1] - Entrar                                                      \n"
            " [2] - Novo Usuário                                                \n"
            " [3] - Sair                                                        \n"
            "+=================================================================+\n");

    printf("|--> ");
    setbuf(stdin, NULL);
    scanf("%d", &opcao);

    /// FILTRAR SELEÇÃO
    switch(opcao)
    {
        case 1:
            Login_Entrar();
            break;
        case 2:
            Login_Cadastrar();
            break;
        case 3:
            exit(0);
        default:
            Menu_Login();
    }
}

void Login_Entrar()
{// Função utilizada na realização de login do usuário.

    /// DECLARAÇÃO DE VARIAVEIS
    char Verificacao, usuario[30], senha[30];
    int acesso = 0;

    /// IMPRESSÃO DO MENU
    Imprime_Login_Entrar();

    /// ENTRADA DE DADOS
    printf(" Login: ");
    setbuf(stdin, NULL);
    scanf("%s", usuario);

    printf(" Senha: ");
    setbuf(stdin, NULL);
    scanf("%s", senha);

    /// CHECAGEM DOS DADOS
    acesso = Buscar_Usuario(3, usuario, senha);

    if(acesso == 1){
            Limpar();
       menuPrincipalScreen();
    }
    else
    {
        Imprime_Login_Entrar();

        if(Msg_Pergunta("Usuário ou Senha Incorretos!\n\nDeseja realizar o cadastro de um novo usuário?") == 1)
            Login_Cadastrar();
        else
            Login_Entrar();
    }
}

void Verificar_Cadastro(char txt[100])
{
    /// DECLARAÇÃO DE VARIAVEIS
    char verificacao;

    /// IMPRESSÃO DA MENSAGEM
    Imprime_Login_Cadastro();

    if(Msg_Pergunta(("%s\n\nDeseja cadastrar novamente?", txt)) == 1)
        Login_Cadastrar();
    else
        Menu_Login();
}

void Login_Cadastrar()
{// Função utilizada no cadastro de um novo usuário.

    // DECLARAÇÃO DE VARIAVEIS
    char usuario[50], senha1[30], senha2[30];
    char Verificacao;
    int comparacao, tamUsuario, tamSenha;

    /// IMPRESSÃO DO MENU
    Imprime_Login_Cadastro();

    /// ENTRADA DE DADOS
    printf(" Usuário: ");
    setbuf(stdin, NULL);
    scanf("%s", usuario);

    printf("\n Senha: ");
    setbuf(stdin, NULL);
    scanf("%s", senha1);

    printf(" Confirme a Senha: ");
    setbuf(stdin, NULL);
    scanf("%s", senha2);

    // COMPARAÇÃO DOS DADOS
    comparacao = strcmp(senha1, senha2);//COMPARANDO AS DUAS STRINGS
    tamSenha = strlen(senha1);
    tamUsuario = strlen(usuario);

    if(comparacao != 0) Verificar_Cadastro("As senhas não correspondem!");
    else if(tamSenha < 5) Verificar_Cadastro("Senha muito curta!");
    else if(tamSenha > 29) Verificar_Cadastro("Senha muito longa!");
    else if(tamUsuario > 49) Verificar_Cadastro("Usuário muito longo!");
    else
    {
        Imprime_Login_Cadastro();
        Adicionar_Usuario(usuario, senha1);
        Menu_Login();
    }
}

int Buscar_Usuario(int modo, char usuario[50], char senha[30])
{// Função para buscar o usuario informado no arquivo de texto.
    // Modo 1 - Retorna se o usuário está ou não cadastrado.
    // Modo 2 - Retorna se o usuário e senha estão cadastrados e se correspondem.
    // Modo 3 - Retorna se o usuário e senha estão cadastrados, se correspondem e informa seu código.

    /// DECLARAÇÃO DE VARIAVEIS

    char Verificacao, dados1[50], dados2[50], info[3][50], *resultado, *pedacos;
    int index, acesso = 0;

    /// ABERTURA DO ARQUIVO
    arquivo = fopen("./database/usuarios.bin", "a+b");

    /// VERIFICAÇÃO DA ABERTURA DO ARQUIVO
    if(arquivo == NULL)
    {
        printf("Não foi possivel encontrar o arquivo de usuários!\n\n");
        system("pause");
        Menu_Login();
    }

    /// PERCORRER CADA LINHA DO ARQUIVO
    while(!feof(arquivo))//INDICADOR DO FINAL DO ARQUIVO
    {
        // Retornar a string da linha atual
        resultado = fgets(dados1, 100, arquivo);

        // Copiar o valor de um vetor para o outro
        strcpy(dados2, dados1);

        /// VERIFICAR SE A LINHA RETORNOU ALGO
        index = 0;
        if(resultado)
        {
            /// REPARTIR O RESULTADO DA LINHA
            pedacos = strtok(dados2, "§");

            /// PERCORRER CADA REPARTIÇÃO
            while(pedacos)
            {
                // Atribui o valor da repartição atual para o vetor
                strcpy(info[index], pedacos);
                cod_UltimoUsuario = atoi(pedacos);
                pedacos = strtok(NULL, "§");

                index++;
            }
        }else
            cod_UltimoUsuario = 0;

        /// VERIFICAR E RETORNAR OS DADOS COM BASE NO MODO INFORMADO
        if(modo == 1){
            if(!strcmp(usuario, info[0])) acesso = 1;//COMPARANDO AS DUAS STRINGS
        }
        if(modo == 2){
            if((!strcmp(info[0], usuario))&(!strcmp(info[1], senha)))//COMPARANDO AS DUAS STRINGS
                acesso = 1;
        }
        if(modo == 3){
            if((!strcmp(usuario, info[0]))&&(!strcmp(senha, info[1]))){//COMPARANDO AS DUAS STRINGS
                acesso = 1;
                cod_AtualUsuario = atoi(info[2]);
            }
        }
    }
    /// FECHAMENTO DO ARQUIVO
    fclose(arquivo);

    /// RETORNA SE ENCONTROU OU NÃO O USUARIO
    // 0 - Não encontrado
    // 1 - Encotrado
    return acesso;
}

int Adicionar_Usuario(char usuario[50], char senha[30])
{// Função para adicionar um novo usuário ao arquivo de texto.

    /// DECLARAÇÃO DE VARIAVEIS

    char Verificacao;
    int acesso;

    /// VERIFICAR SE USUÁRIO JA EXISTE
    acesso = Buscar_Usuario(1, usuario, senha);
    if(acesso == 1)
    {
        if(Msg_Pergunta("Usuário ja existente!\n\nDeseja realizar o cadastro novamente?") == 1)
            Login_Cadastrar();
        else
            Menu_Login();
    }else
    {
        /// ABERTURA DO ARQUIVO
        arquivo = fopen("./database/usuarios.bin", "a+b");

        /// VERIFICAÇÃO DA ABERTURA DO ARQUIVO
        if(arquivo == NULL)
        {
            printf("Não foi possivel encontrar o arquivo de usuários!\n");
            system("pause");
            Menu_Login();
        }

        /// ESCREVER NO ARQUIVO TEXTO COM SEPARAÇÃO POR '§'
        fprintf(arquivo, "\n%s§%s§%d§", usuario, senha, (cod_UltimoUsuario + 1));
        printf("Usuário cadastrado com sucesso!\n\n");
        system("pause");

        /// FECHAMENTO DO ARQUIVO
        fclose(arquivo);
    }
    return 0;
}

int Msg_Pergunta(char txt[200])
{
    /// DECLARAÇÃO DE VARIAVEIS
    char pergunta;

    /// IMPRESSÃO DA MENSAGEM
    printf("%s\n[S,N]|--> ", txt);
    setbuf(stdin, NULL);
    scanf("%c", &pergunta);

    /// RETORNO DA RESPOSTA
    if((pergunta == 's')||(pergunta == 'S')) return 1;
    else return 0;
}

///OPÇÕES DO MENU PRINCIPAL
void menuPrincipalScreen(){

    int option;
    Limpar();
    printf("\n+==================== Bem-Vindo =============================+");
    printf("\n|                   MENU PRINCIPAL                           | ");
    printf("\n+============================================================+");
    printf("\n [1] - Cadastro                                               ");
    printf("\n [2] - Atendimento Ao Cliente                                 ");
    printf("\n [3] - Estoque                                                ");
    printf("\n [4] - Financeiro                                             ");
    printf("\n\n [0] - Sair                                                 ");
    printf("\n+============================================================+\n");
  do
  {
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d", &option);
    switch (option){
        case 1: clientesScreen();
                break;
        case 2: atendimentoScreen();
                break;
        case 3: Menu_de_Estoque();
                break;
        case 4: financeiroScreen();
                break;
        default:
            Menu_Login();
                break;
    }
  } while (option == 0);
}

///TELA DE CADASTRO
void clientesScreen(){

    int option;
    Limpar();
    printf("\n+=============================================================+");
    printf("\n|                   MENU DE CADASTRO                          |");
    printf("\n+=============================================================+");
    printf("\n [1] - Clientes                                                ");
    printf("\n [2] - Veiculos                                                ");
    printf("\n [3] - Fornecedores                                            ");
    printf("\n [4] - Usuários                                                ");
    printf("\n [5] - Oficinas                                                ");
    printf("\n [6] - Relatorio                                               ");
    printf("\n\n [0] - Menu Principal                                        ");
    printf("\n+=============================================================+\n");
  do
  {
      printf("Digite uma das opções\n");
      printf("|--> ");
      scanf("%d", &option);
    switch (option){
        case 1: cadastroScreen2();
                break;
        case 2: veiculosScreen();
                break;
        case 3: fornecedoresScreen();
                break;
        case 4: usuarioScreen();
                break;
        case 5: oficinasScreen();
                break;
        default: menuPrincipalScreen();
                break;
    }
  } while (option == 0);
}


///TELA DE CADASTRO DE CLIENTES
void cadastroScreen2(){

    int option;
    Limpar();
    printf("+============================================================+");
    printf("\n|                       CLIENTES                             |");
    printf("\n+============================================================+");
    printf("\n [1] - Novo Cadastro                                          ");
    printf("\n [2] - Alterar Cadastro                                       ");
    printf("\n [3] - Excluir Cadastro                                       ");
    printf("\n [4] - Lista de Clientes                                      ");
    printf("\n [5] - Voltar                                                 ");
    printf("\n\n [0] - Menu Principal                                       ");
    printf("\n+============================================================+\n");
    printf("Digite uma das opções\n");
  printf("|--> ");
  scanf("%d", &option);
  do
  {
    switch (option)
    {
    case 1: cadastro_de_cliente();
            break;
    case 2: alterar_cadastro();
            break;
    case 3: Excluirclientes();
            break;
    case 4: listarclientes();
            break;
    default: clientesScreen();
            break;
    }
  } while (option ==0);

}

void cadastro_de_cliente()
{
  Cadastro C;
  Arq_princ = fopen("./database/Cliente.bin","a+b"); ///ABERTURA DO ARQUIVO

  int option = 0;

    printf("\n");
    printf("+============================================================+");
    printf("\n [1] - Para Continuar                                        |");
    printf("\n [2] - Voltar                                                |\n");
    printf("+============================================================+\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: cadastroScreen2();
            break;
  }

  if(Arq_princ)
  {
      Limpar();
      printf("+==============================================================+\n");
      printf("|                 NOVO CADASTRO DE CLIENTE                     |\n");
      printf("+==============================================================+\n");
      fflush(stdin);
      printf("| Nome Completo: ");
      fgets(C.Nome,sizeof(C.Nome),stdin);
      printf("+==============================================================+\n");
      printf("| CPF:");
      fflush(stdin);
      gets(C.CPF);
      printf("+==============================================================+\n");
      printf("| Telefone para Contato:");
      fflush(stdin);
      fgets(C.Contato,sizeof(C.Contato),stdin);
      printf("+==============================================================+\n");
      printf("| Endereço:");
      fgets(C.Endereco,sizeof(C.Endereco),stdin);
      fflush(stdin);
      printf("+==============================================================+\n");
      printf("| Marca do Veiculo:");
      fflush(stdin);
      fgets(C.marca,sizeof(C.marca),stdin);
      printf("+==============================================================+\n");
      printf("| Modelo do Veiculo:");
      fflush(stdin);
      fgets(C.Modelo,sizeof(C.Modelo),stdin);
      printf("+==============================================================+\n");
      printf("| Placa do Veiculo:");
      fflush(stdin);
      fgets(C.placa,sizeof(C.placa),stdin);
      printf("+==============================================================+\n\n");
      fwrite(&C,sizeof(Cadastro),1,Arq_princ); // GRAVANDO NO ARQUIVO
      fclose(Arq_princ); //FECHANDO O ARQUIVO PRINCIPAL
      getch();
  }
  else
  {
    printf("\nErro ao abrir o Arquivo!"); // CASO O ARQUIVO NÃO SEJA ABERTO CORRETAMENTE
    Wait(1);///congelar a tela
    menuPrincipalScreen();
  }
    printf("\n");
    printf("+==============================================================+");
    printf("\n [1] - Para Continuar                                       ");
    printf("\n [2] - Voltar                                               ");
    printf("\n\n [0] - Menu Clientes                                      ");
     printf("\n+=============================================================+\n");
    printf("Digite uma das opções\n");
  printf("|--> ");
  scanf("%d",&option);
  switch (option)
  {
    case 1: Limpar();
            cadastro_de_cliente();
            break;
    case 2: cadastroScreen2();
            break;
    default: clientesScreen();
            break;
  }
}

void alterar_cadastro()
{
  Arq_princ = fopen("./database/Cliente.bin","a+b");//ABERTURA DO ARQUIVO
  Aux = fopen("./database/AUXILIAR.bin","wb");///ABERTURA DO ARQUIVO AUXILIAR

  Cadastro alt;
  char nome[20];
  int option,a;
    Limpar();
    printf("+=============================================================+");
    printf("\n|                     ALTERAR CADASTRO                        |");
    printf("\n+=============================================================+");
    printf("\n [1] - Alterar Nome                                            ");
    printf("\n [2] - Alterar CPF                                             ");
    printf("\n [3] - Alterar Contato                                         ");
    printf("\n [4] - Alterar Endereço                                        ");
    printf("\n [5] - Alterar Marca/Modelo do Veiculo                         ");
    printf("\n [6] - Alterar Placa Do Veiculo                                ");
    printf("\n [7] - Alterar Todas As Opções                                 ");
    printf("\n [8] - Voltar                                                  ");
    printf("\n\n [0] - Menu principal                                          \n");
    printf("+==============================================================+\n");

  printf("Digite uma das opções\n");
  printf("|--> ");
  scanf("%d", &option);

  switch (option){
    case 1:

      printf("Digite o nome do Cliente\n");
      printf("|--> ");
      fgets(nome,strlen(nome),stdin);

    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if(strcmp(alt.Nome, nome)== 0)//COMPARANDO AS DUAS STRINGS
      {
        a++;
        Limpar();
          printf("+===============================================================+\n");
          printf("| Alterar Nome :");
          fflush(stdin);
          fgets(alt.Nome,sizeof(alt.Nome),stdin);
          printf("+===============================================================+\n");
      }
        fwrite(&alt,sizeof(Cadastro),1,Aux);// GRAVANDO NO ARQUIVO
          fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(a == 0)printf("\nNome de cadastro não Encontrado!");//SE O CADASTRO NÃO FOR LOCALIZADO
      else printf("\nCadastro Alterado com Sucesso!\n");//SE A ALTERAÇÃO FOI FEITA COM SUCESSO
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("cliente.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","cliente.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();///ESPERANDO UM ENTER
    break;
    case 2:

      printf("Digite o nome do Cliente\n");
      printf("|--> ");
      fgets(nome,strlen(nome),stdin);

    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if(strcmp(alt.Nome,nome) == 0) //COMPARANDO AS DUAS STRINGS
      {
        a++;
        Limpar();
          printf("+===============================================================+\n");
          printf("| Alterar CPF :");
          fflush(stdin);
          fgets(alt.Nome,sizeof(alt.Nome),stdin);
          printf("+===============================================================+\n");
      }
        fwrite(&alt,sizeof(Cadastro),1,Aux);// GRAVANDO NO ARQUIVO
          fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(a == 0)printf("\nNome de cadastro não Encontrado!");
      else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("cliente.bin"); /// Excluindo o arquivo
      fclose(Aux); //FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","cliente.bin"); ///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
    case 3:

      printf("Digite o nome do Cliente\n");
      printf("|--> ");
      fgets(nome,strlen(nome),stdin);

    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while (!feof(Arq_princ)) //INDICADOR DO FINAL DO ARQUIVO
    {
      if(strcmp(alt.Nome,nome) == 0)//COMPARANDO AS DUAS STRINGS
      {
        a++;
        Limpar();
          printf("+===============================================================+\n");
          printf("| Alterar Contato :");
          fflush(stdin);
          fgets(alt.Nome,sizeof(alt.Nome),stdin);
          printf("+===============================================================+\n");
      }
        fwrite(&alt,sizeof(Cadastro),1,Aux);// GRAVANDO NO ARQUIVO
          fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(a == 0)printf("\nNome de cadastro não Encontrado!");
      else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("cliente.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","cliente.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
    case 4:

      printf("Digite o nome do Cliente\n");
      printf("|--> ");
      fgets(nome,strlen(nome),stdin);

    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if(strcmp(alt.Nome,nome) == 0)//COMPARANDO AS DUAS STRINGS
      {
        a++;
        Limpar();
          printf("+===============================================================+\n");
          printf("| Altera Endereço :");
          fflush(stdin);
          fgets(alt.Nome,sizeof(alt.Nome),stdin);
          printf("+===============================================================+\n");

      }
        fwrite(&alt,sizeof(Cadastro),1,Aux);// GRAVANDO NO ARQUIVO
          fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(a == 0)printf("\nNome de cadastro não Encontrado!");
      else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("cliente.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","cliente.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
    case 5:

      printf("Digite o nome do Cliente\n");
      printf("|-->\n");
      fgets(nome,strlen(nome),stdin);

    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if(strcmp(alt.Nome,nome) == 0)//COMPARANDO AS DUAS STRINGS
      {
        a++;
        Limpar();
          printf("+===============================================================+\n");
           printf("| Alterar Marca do Veiculo:");
          fflush(stdin);
          fgets(alt.marca,sizeof(alt.marca),stdin);
          printf("+===============================================================+\n");
          printf("| Altera Modelo do Veiculo:");
          fflush(stdin);
          fgets(alt.Nome,sizeof(alt.Nome),stdin);
          printf("+===============================================================+\n");

      }
        fwrite(&alt,sizeof(Cadastro),1,Aux);// GRAVANDO NO ARQUIVO
          fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(a == 0)printf("\nNome de cadastro não Encontrado!");
      else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("cliente.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","cliente.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
    case 6:

      printf("Digite o nome do Cliente\n");
      printf("|--> ");
      fgets(nome,strlen(nome),stdin);

    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if(strcmp(alt.Nome,nome) == 0)//COMPARANDO AS DUAS STRINGS
      {
        a++;
        Limpar();
          printf("+===============================================================+\n");
          printf("| Alterar placa:");
          fflush(stdin);
          fgets(alt.Nome,sizeof(alt.Nome),stdin);
          printf("+===============================================================+\n");

      }
        fwrite(&alt,sizeof(Cadastro),1,Aux);// GRAVANDO NO ARQUIVO
          fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(a == 0)printf("\nNome de cadastro não Encontrado!");
      else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("cliente.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","cliente.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
    case 7:

      printf("Digite o nome do Cliente\n");
      printf("|--> ");
      fgets(nome,strlen(nome),stdin);

    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if(strcmp(alt.Nome,nome)== 0)//COMPARANDO AS DUAS STRINGS
      {
        a++;
        Limpar();
          printf("+===============================================================+\n");
          fflush(stdin);
          printf("| Altera Nome: ");
          fgets(alt.Nome,sizeof(alt.Nome),stdin);
          printf("+===============================================================+\n");
          printf("| Altera CPF:");
          fflush(stdin);
          gets(alt.CPF);
          printf("+===============================================================+\n");
          printf("| Altera Telefone para Contato:");
          fflush(stdin);
          fgets(alt.Contato,sizeof(alt.Contato),stdin);
          printf("+===============================================================+\n");
          printf("| Altera Endereco:");
          fgets(alt.Endereco,sizeof(alt.Endereco),stdin);
          fflush(stdin);
          printf("+===============================================================+\n");
          printf("| Altera Modelo do Veiculo:");
          fflush(stdin);
          fgets(alt.Modelo,sizeof(alt.Modelo),stdin);
          printf("+===============================================================+\n");
          printf("| Alterar Marca do Veiculo:");
          fflush(stdin);
          fgets(alt.marca,sizeof(alt.marca),stdin);
          printf("+===============================================================+\n");
          printf("| Altera Placa do Veiculo:");
          fgets(alt.placa,sizeof(alt.placa),stdin);
          fflush(stdin);
          printf("+===============================================================+\n");

      }
        fwrite(&alt,sizeof(Cadastro),1,Aux);// GRAVANDO NO ARQUIVO
          fread(&alt,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(a==0)printf("\nNome de cadastro não Encontrado!");
      else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("cliente.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","cliente.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
    case 8: cadastroScreen2();
            break;
  default: clientesScreen();
            break;
  }
}

void Excluirclientes(){

  Cadastro Del;

  Aux = fopen("./database/AUXILIAR.bin","wb");//ABERTURA DE UM ARQUIVO AUXILIAR///ABERTURA DO ARQUIVO AUXILIAR
  Arq_princ = fopen("./database/cliente.bin","a+b");//ABERTURA DO ARQUIVO
  char nome[20];
  int x,option;

    printf("\n");
    printf("+============================================================+");
    printf("\n [1] - Para Continuar                                         ");
    printf("\n [2] - Voltar                                                 \n");
    printf("+============================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: cadastroScreen2();
            break;
  }
    Limpar();
    printf("+============================================================+\n");
    printf("|                       EXCLUIR CADASTRO                     |\n");
    printf("+============================================================+\n");
    printf("Digite o Nome do Cliente\n");
    fflush(stdin);
    printf("|--> ");
    fflush(stdin);
    fgets(nome,sizeof(nome),stdin);

  fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
  fread(&Del,sizeof(Cadastro),1,stdin);//LENDO OS BLOCOS DO ARQUIVO

  while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
  {
    if(strcmp(Del.Nome,nome)== 0) x++;//COMPARANDO AS DUAS STRINGS
      else fwrite(&Del,sizeof(Cadastro),1,Aux);// GRAVANDO NO ARQUIVO
      fread(&Del,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
  }
  if(x == 0)printf("\nNumero de cadastro Não Encotrado!");
  else printf("\nCadastro Excluido com Sucesso!\n");
    fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    remove("cliente.bin");/// Excluindo o arquivo
    fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
    rename("AUXILIAR.bin","cliente.bin");///Renomendo o arquivo auxiliar
    remove("AUXILIAR.bin");/// Excluindo o arquivo
  getch();
    printf("\n");
    printf("+============================================================+");
    printf("\n [1] - Para Continuar                                       ");
    printf("\n [2] - Voltar                                               ");
    printf("\n\n [0] - Menu Principal                                     ");
     printf("\n+=========================================================+\n");
    printf("Digite uma das opções\n");
  printf("|--> ");
  scanf("%d",&option);

  switch (option)
  {
    case 1: Excluirclientes();
            break;
    case 2: cadastroScreen2();
            break;
    default: clientesScreen();
            break;
  }
}

void listarclientes()
{
  Limpar();

  Cadastro Pesquisar;
  int a=1;

  Arq_princ = fopen("./database/cliente.bin","a+b");//ABERTURA DO ARQUIVO

  if (Arq_princ)
  {
    while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if (fread(&Pesquisar,sizeof(Cadastro),1,Arq_princ))//LENDO OS BLOCOS DO ARQUIVO
      {
          printf("+==============================================================+\n");
          printf("|                     LISTA DE CLIENTES (%d)                    |\n",a++);
          printf("+==============================================================+");
          printf("\n| Nome: %s",Pesquisar.Nome);
          printf("+==============================================================+");
          printf("\n| CPF: %s",Pesquisar.CPF);
          printf("\n+==============================================================+");
          printf("\n| Telefone: %s",Pesquisar.Contato);
          printf("+==============================================================+");
          printf("\n| Endereço: %s",Pesquisar.Endereco);
          printf("+==============================================================+");
          printf("\n| Veiculo: %s",Pesquisar.Modelo);
          printf("+==============================================================+");
          printf("\n| Marca : %s",Pesquisar.marca);
          printf("+==============================================================+");
          printf("\n| Placa: %s",Pesquisar.placa);
          printf("+==============================================================+\n\n");
          fflush(stdin);
      }
    }
    fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
  }
  else{
  printf("\tErro ao abrir Arquivo!");
  Wait(1); ///congelar a tela
    clientesScreen();
  }
  pesquisarclientes();
}

void pesquisarclientes(){

    Cadastro Pesquisar;
  char Nome_cliente[30];
    int option,a=0;

  Arq_princ = fopen("./database/cliente.bin","rb");///ABERTURA DO ARQUIVO

    printf("\n");
    printf("+==============================================================+");
    printf("\n [1] - Para Continuar                                         ");
    printf("\n [2] - Voltar                                                 \n");
    printf("+==============================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: cadastroScreen2();
            break;
  }
    printf("\n");
    printf("\n\n+==============================================================+\n");
    printf("|                    PESQUISA DE CLIENTES                       |\n");
    printf("+==============================================================+\n");
    printf("Digite o Nome do Cliente\n");
    fflush(stdin);
    printf("|--> ");
    fgets(Nome_cliente,sizeof(Nome_cliente),stdin);
    fflush(stdin);
    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
    fread(&Pesquisar,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
      {
        if(strcmp(Pesquisar.Nome ,Nome_cliente) == 0)//COMPARANDO AS DUAS STRINGS
        {a++;
            Limpar();
            printf("\n+==============================================================+");
            printf("\n| Nome: %s",Pesquisar.Nome);
            printf("+==============================================================+");
            printf("\n| CPF: %s",Pesquisar.CPF);
            printf("\n+==============================================================+");
            printf("\n| Telefone: %s",Pesquisar.Contato);
            printf("+==============================================================+");
            printf("\n| Enddereço: %s",Pesquisar.Endereco);
            printf("+==============================================================+");
            printf("\n| Veiculo: %s",Pesquisar.Modelo);
            printf("+==============================================================+");
            printf("\n| Marca: %s",Pesquisar.marca);
            printf("+==============================================================+\n");
            printf("\n| Placa: %s",Pesquisar.placa);
            printf("+==============================================================+\n");

         getch();
        }
        fread(&Pesquisar,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      }
      if(a == 0){ printf("\nCadastro Não Localizado!\n");
        Wait(2);///congelar a tela
      }
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    Limpar();
    printf("+==========================================================+");
    printf("\n [1] - Para Continuar                                       ");
    printf("\n [2] - Voltar                                               ");
    printf("\n\n [0] - Menu Principal                                     ");
     printf("\n+==========================================================+\n");
    printf("Digite uma das opções\n");
  printf("|--> ");
  scanf("%d",&option);
  switch (option)
  {
    case 1: Limpar();
            listarclientes();
            break;
    case 2: cadastroScreen2();
            break;
    default: clientesScreen();
            break;
  }
}

void veiculosScreen()
{
  int option;

        Limpar();
        printf("+============================================================+");
        printf("\n|                       VEICULOS                            |");
        printf("\n+============================================================+");
        printf("\n [1] - Novo Cadastro                                          ");
        printf("\n [2] - Alterar Cadastro                                       ");
        printf("\n [3] - Excluir Cadastro                                       ");
        printf("\n [4] - Lista De Veiculos                                      ");
        printf("\n [5] - Voltar                                                 ");
        printf("\n\n [0] - Menu Principal                                        ");
        printf("\n+============================================================+\n");

  printf("Digite uma das opções\n ");
  printf("|--> ");
  scanf("%d",&option);
  switch(option)
  {
    case 1:
      Limpar();
      cadastro_de_Veiculos();
      break;
    case 2:
      Limpar();
      altera_Veiculo();
      break;
    case 3:
      Limpar();
      Excluir_Veiculo();
      break;
    case 4: Limpar();
            Lista_Veiculos();
            break;
    default:
      Limpar();
      clientesScreen();
      break;
  }
}

void cadastro_de_Veiculos()
{
  veiculo V;
  Arq_princ = fopen("./database/veiculos.bin","a+b");///ABERTURA DO ARQUIVO
  int option;

    printf("+===========================================================+");
    printf("\n [1] - Para Continuar                                       ");
    printf("\n [2] - Voltar                                               \n");
    printf("+============================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: veiculosScreen();
            break;
  }


  if(Arq_princ)
  {
      Limpar();
      printf("+==============================================================+\n");
      printf("|                 NOVO CADASTRO DE VEICULO                    |\n");
      printf("+==============================================================+\n");
      fflush(stdin);
      printf("| Modelo do Veiculo:");
      fgets(V.modelo,sizeof(V.modelo),stdin);
      printf("+==============================================================+\n");
      printf("| Marca do Veiculo:");
      fflush(stdin);
      fgets(V.marca,sizeof(V.marca),stdin);
      printf("+==============================================================+\n");
      printf("| Placa do Veiculo:");
      fflush(stdin);
      fgets(V.placa,sizeof(V.placa),stdin);
      printf("+==============================================================+\n");

      fwrite(&V,sizeof(veiculo),1,Arq_princ);// GRAVANDO NO ARQUIVO
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
      getch();
  }
  else{
  printf("Erro ao abrir Arquivo!");
  Wait(1);///congelar a tela
    menuPrincipalScreen();
  }
    printf("\n");
    printf("\n+==============================================+");
    printf("\n [1] - Continuar                                ");
    printf("\n [2] - Voltar                                   ");
    printf("\n\n [0] - Menu principal                    ");
    printf("\n+==============================================+\n\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
    switch (option)
    {
    case 1:
      Limpar();
      cadastro_de_Veiculos();
      break;
      case 2: veiculosScreen();
              break;
    default:
      clientesScreen();
      break;
    }
}

void altera_Veiculo()
{
  Arq_princ = fopen("./database/veiculos.bin","a+b");///ABERTURA DO ARQUIVO
  Aux = fopen("./database/AUXILIAR.bin","wb");//ABERTURA DE UM ARQUIVO AUXILIAR///ABERTURA DO ARQUIVO AUXILIAR

  veiculo alt;
  char modelo[20];
  int option,a;

        Limpar();
        printf("+============================================================+");
        printf("\n|              ALTERAR CADASTRO DE VEICULO                  |");
        printf("\n+============================================================+");
        printf("\n [1] - Alterar Modelo/Marca Do Veiculo                        ");
        printf("\n [2] - Alterar Placa                                          ");
        printf("\n [3] - Alterar Todas As Opções                                ");
        printf("\n [4] - Voltar                                                 ");
        printf("\n\n [0] - Menu Principal                                           ");
        printf("\n+============================================================+\n");

    printf("Digite uma das opções \n");
    printf("|--> ");
    scanf("%d",&option);
    switch (option)
    {
    case 1:

        printf("\n+===================================================+\n");
        printf("digite o Modelo do Veiculo:");
        fflush(stdin);
        fgets(modelo,sizeof(modelo),stdin);
        printf("\n+===================================================+\n");


      fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(veiculo),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
      {
        if(strcmp(alt.modelo,modelo) == 0)//COMPARANDO AS DUAS STRINGS
        {a++;

              printf("+===================================================+\n");
              printf("| Alterar Modelo:");
              fflush(stdin);
              fgets(alt.modelo,sizeof(alt.modelo),stdin);
              printf("\n+===================================================+\n");
              printf("| Alterar Marca:");
              fflush(stdin);
              fgets(alt.marca,sizeof(alt.marca),stdin);
              printf("\n+===================================================+\n");
        }
        fwrite(&alt,sizeof(veiculo),1,Aux);// GRAVANDO NO ARQUIVO
        fread(&alt,sizeof(veiculo),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      }
       if(a == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("veiculos.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin","veiculos.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
      break;
    case 2:
            printf("\n+===================================================+\n");
            printf("digite o Modelo do Veiculo:");
            fflush(stdin);
            fgets(modelo,sizeof(modelo),stdin);
            printf("\n+==========================================================+\n");


        fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
        fread(&alt,sizeof(veiculo),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(strcmp(alt.modelo,modelo) == 0)//COMPARANDO AS DUAS STRINGS
            {a++;
                    printf("+====================================================+\n");
                    printf("| Alterar placa");
                    fflush(stdin);
                    fgets(alt.placa,sizeof(alt.placa),stdin);
                    printf("\n+====================================================+\n");

            }
            fwrite(&alt,sizeof(veiculo),1,Aux);// GRAVANDO NO ARQUIVO
            fread(&alt,sizeof(veiculo),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(a == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("veiculos.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin","veiculos.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
      break;
    case 3:

            printf("+================================================+\n");
            printf("digite o Modelo do Veiculo:");
            fflush(stdin);
            fgets(modelo,sizeof(modelo),stdin);
            printf("\n+================================================+\n");

        fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
        fread(&alt,sizeof(veiculo),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ)){//INDICADOR DO FINAL DO ARQUIVO
            if(strcmp(alt.modelo,modelo)==0){//COMPARANDO AS DUAS STRINGS
                a++;

                    printf("+================================================+\n");
                    printf("| Alterar Modelo:");
                    fgets(alt.modelo,sizeof(alt.modelo),stdin);
                    fflush(stdin);
                    printf("\n+================================================+\n");
                    printf("| Alterar Marca:");
                    fflush(stdin);
                    fgets(alt.marca,sizeof(alt.marca),stdin);
                    printf("\n+================================================+\n");
                    printf("| Alterar Placa:");
                    fgets(alt.placa,sizeof(alt.placa),stdin);
                    printf("\n+================================================+");

            }
            fwrite(&alt,sizeof(veiculo),1,Aux);// GRAVANDO NO ARQUIVO
            fread(&alt,sizeof(veiculo),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(a == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("veiculos.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin","veiculos.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
      break;
      case 4: veiculosScreen();
              break;
    default:
        Limpar();
        clientesScreen();
      break;
    }
    printf("\n");
    printf("\n+==============================================+");
    printf("\n [1] - Continuar                                ");
    printf("\n [2] - Voltar                                   ");
    printf("\n\n [0] - Menu Principal                         ");
    printf("\n+==============================================+\n\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
    switch (option)
    {
    case 1:
      Limpar();
      altera_Veiculo();
      break;
    case 2: veiculosScreen();
            break;
    default: clientesScreen();
            break;
    }
}

void Excluir_Veiculo()
{
    Arq_princ = fopen("./database/veiculos.bin","a+b");///ABERTURA DO ARQUIVO
    Aux = fopen("./database/AUXILIAR.bin","wb");//ABERTURA DE UM ARQUIVO AUXILIAR///ABERTURA DO ARQUIVO AUXILIAR

    veiculo del;
    char marca[20];
    int option,n;

    printf("+============================================================+");
    printf("\n [1] - Para Continuar                                       ");
    printf("\n [2] - Voltar                                               \n");
    printf("+============================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: veiculosScreen();
            break;
  }
      Limpar();
      printf("+============================================================+\n");
      printf("|                 EXCLUIR CADASTRO DE VEICULO                |\n");
      printf("+============================================================+\n");
      printf("| Digite o Modelo do Veiculo\n");
      fflush(stdin);
      printf("|--> ");
      fgets(marca,sizeof(marca),stdin);


    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
    fread(&del,sizeof(veiculo),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if(strcmp(del.modelo, marca)== 0)n++;//COMPARANDO AS DUAS STRINGS
      else fwrite(&del,sizeof(veiculo),1,Aux);// GRAVANDO NO ARQUIVO
      fread(&del,sizeof(veiculo),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(n == 0) printf("\nModelo de veiculo Cadastrado não encotrado!");
    else printf("\nCadastro Excluido com Sucesso!");
    fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("veiculos.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin","veiculos.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
    printf("\n");
    printf("\n+==============================================+");
    printf("\n [1] - Continuar                                ");
    printf("\n [2] - Voltar                                   ");
    printf("\n\n [0] - Menu Principal                         ");
    printf("\n+==============================================+\n\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
    switch (option)
    {
    case 1:
      Limpar();
      Excluir_Veiculo();
      break;
    case 2: veiculosScreen();
            break;
    default: clientesScreen();
            break;
    }
}

void Lista_Veiculos()
{
    veiculo Lista;
    int a = 1;
   Arq_princ = fopen("./database/veiculos.bin","a+b");///ABERTURA DO ARQUIVO

    if(Arq_princ)
    {
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(fread(&Lista,sizeof(veiculo),1,Arq_princ))//LENDO OS BLOCOS DO ARQUIVO
            {
                printf("+==============================================================+\n");
                printf("|                    Lista De Veiculos (%d)                     |\n",a++);
                printf("+==============================================================+\n");
                printf("| Modelo: %s",Lista.modelo);
                printf("+==============================================================+\n");
                printf("| Marca: %s",Lista.marca);
                printf("+==============================================================+\n");
                printf("| Placa: %s",Lista.placa);
                printf("+==============================================================+\n");
                printf("\n");
            }
        }
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    }
    else{
        printf("\tErro ao Abrir Arquivo!\n");
        Wait(1);///congelar a tela
    }
     pesquisa_Veiculo();
}


void pesquisa_Veiculo()
{
  veiculo P;

  Arq_princ = fopen("./database/veiculos.bin","a+b");///ABERTURA DO ARQUIVO

  char modelo[20];
  int x;
  int option;
    printf("\n");
    printf("+==============================================================+");
    printf("\n [1] - Para Continuar                                         ");
    printf("\n [2] - Voltar                                                 \n");
    printf("+==============================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: veiculosScreen();
            break;
  }
    printf("\n");
    printf("+==============================================================+\n");
    printf("|                     PESQUISA DE VEICULOS                     |\n");
    printf("+==============================================================+\n");
    fflush(stdin);
    printf("Digite o Modelo do Veiculo para Pesquisa\n");
    fflush(stdin);
    printf("|--> ");
    fgets(modelo,sizeof(modelo),stdin);

    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
    fread(&P,sizeof(veiculo),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
  {
    if(strcmp(P.modelo,modelo) == 0)//COMPARANDO AS DUAS STRINGS
    {x++;
        Limpar();
         printf("|                 RESULTADO DA PESQUISA                  |\n");
         printf("+=======================================================+\n");
         printf("| Modelo: %s",P.modelo);
         printf("+=======================================================+\n");
         printf("| Marca: %s",P.marca);
         printf("+=======================================================+\n");
         printf("| Placa: %s",P.placa);
         printf("+=======================================================+\n");
      getch();
    }
    fread(&P,sizeof(veiculo),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
  }
  if(x == 0){printf("\nVeiculo não encontrado!");
    Wait(2);///congelar a tela
  }
  fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    Limpar();
   printf("\n+==============================================+");
    printf("\n [1] - Continuar                                ");
    printf("\n [2] - Voltar                                   ");
    printf("\n\n [0] - Menu Principal                           ");
    printf("\n+==============================================+\n\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
    switch (option)
    {
    case 1: Lista_Veiculos();
            break;
    case 2: veiculosScreen();
            break;
    default:
      clientesScreen();
      break;
    }
}


void fornecedoresScreen(){

        int option;
    Limpar();
    printf("\n+===========================================================+");
    printf("\n|                      FORNECEDORES                         |");
    printf("\n+===========================================================+");
    printf("\n [1] - Novo Cadastro                                         ");
    printf("\n [2] - Alterar Cadastro                                      ");
    printf("\n [3] - Lista/Pesquisa de Fornecedores                        ");
    printf("\n [4] - Excluir Cadastro                                      ");
    printf("\n [5] - Voltar (Tela Anterior)                                ");
    printf("\n\n [0] - Menu Principal                                        ");
    printf("\n+===========================================================+\n");
    printf("Digite uma das opções\n");
  printf("|--> ");
  scanf("%d", &option);
  do{
    switch (option){

    case 1: cadastro_de_Fornecedores();
            break;
    case 2: alterar_Fornecedor();
            break;
    case 3: Limpar();
            listafornecedores();
             break;
    case 4: Excluir_Fornecedor();
            break;
    case 5: Limpar();
            clientesScreen();
             break;
    default: menuPrincipalScreen();
            break;
    }
  } while (option == 0);

}

void cadastro_de_Fornecedores()
{
  Fornecedor F;
  Arq_princ = fopen("./database/Fornecedores.bin","a+b");///ABERTURA DO ARQUIVO
  int option;

    printf("+============================================================+");
    printf("\n [1] - Para Continuar                                       ");
    printf("\n [2] - Voltar                                               \n");
    printf("+============================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: fornecedoresScreen();
            break;
  }

  if (Arq_princ)
  {     Limpar();
      printf("+==============================================================+\n");
      printf("|                 NOVO CADASTRO DE FORNECEDOR                  |\n");
      printf("+==============================================================+\n");
      fflush(stdin);
      printf("| Nome Completo:");
      fgets(F.Nome,sizeof(F.Nome),stdin);
      printf("+==============================================================+\n");
      printf("| TIPO PESSOA/JURIDICA:");
      fflush(stdin);
      fgets(F.tipo,sizeof(F.tipo),stdin);
      printf("+==============================================================+\n");
      printf("| CPF/CNPJ:");
      fflush(stdin);
      fgets(F.CNPJ,sizeof(F.CNPJ),stdin);
      printf("+==============================================================+\n");
      printf("| Telefone para Contato:");
      fflush(stdin);
      fgets(F.Contato,sizeof(F.Contato),stdin);
      printf("+==============================================================+\n");
      printf("| Email:");
      fflush(stdin);
      fgets(F.Email,sizeof(F.Email),stdin);
      printf("+==============================================================+\n");
      printf("| Endereço:");
      fflush(stdin);
      fgets(F.Endereco,sizeof(F.Endereco),stdin);
      printf("+==============================================================+\n");
      fwrite(&F,sizeof(Fornecedor),1,Arq_princ);// GRAVANDO NO ARQUIVO
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
      getch();
  }
  else
  {
    printf("\nERRO AO ABRIR AQUIVO!");
    Wait(1);///congelar a tela
    menuPrincipalScreen();
  }
    printf("\n");
    printf("\n+===================================================+");
    printf("\n [1] - Para Continuar                                ");
    printf("\n [2] - Para Voltar                                   ");
    printf("\n\n [0] - Menu Principal                                ");
    printf("\n+===================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
  case 1: Limpar();
          cadastro_de_Fornecedores();
          break;
  case 2: fornecedoresScreen();
          break;
  default: clientesScreen();
          break;
  }
}
///EXCLUIR CADASTRO
void Excluir_Fornecedor()
{
  Fornecedor Del;

  Aux = fopen("./database/AUXILIAR.bin","wb");//ABERTURA DE UM ARQUIVO AUXILIAR///ABERTURA DO ARQUIVO AUXILIAR
  Arq_princ = fopen("./database/Fornecedores.bin","a+b");///ABERTURA DO ARQUIVO
  char nome[20];
  int x;
  int option;

    printf("+==========================================================+");
    printf("\n [1] - Para Continuar                                       ");
    printf("\n [2] - Voltar                                               ");
    printf("+============================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: fornecedoresScreen();
            break;
  }
    Limpar();
    printf("+============================================================+");
    printf("|                    EXCLUIR CADASTRO                        |");
    printf("+============================================================+");
    printf("Nome do Fornecedor\n");
    fflush(stdin);
    printf("|--> ");
    gets(nome);

  fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
  fread(&Del,sizeof(Fornecedor),1,stdin);//LENDO OS BLOCOS DO ARQUIVO

  while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
  {
    if(strcmp(Del.Nome,nome)== 0) x++;//COMPARANDO AS DUAS STRINGS
      else fwrite(&Del,sizeof(Fornecedor),1,Aux);// GRAVANDO NO ARQUIVO
      fread(&Del,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
  }
  if(x == 0)printf("\nNumero de cadastro Não Encotrado!");
  else printf("\nCadastro Excluido com Sucesso!\n");
    fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    remove("Fornecedores.bin");/// Excluindo o arquivo
    fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
    rename("AUXILIAR.bin","Fornecedores.bin");///Renomendo o arquivo auxiliar
    remove("AUXILIAR.bin");/// Excluindo o arquivo
  getch();

    Limpar();
    printf("\n+===================================================+");
    printf("\n [1] - Para Continuar                                ");
    printf("\n [2] - Voltar                                        ");
    printf("\n\n [0] - Menu Principal                                ");
    printf("\n+===================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
  case 1: Limpar();
          Excluir_Fornecedor();
          break;
  case 2: fornecedoresScreen();
          break;
  default: clientesScreen();
          break;
  }
}
///ALTERAR CADASTRO
void alterar_Fornecedor()
{
  Aux = fopen("./database/AUXILIAR.bin","wb");//ABERTURA DE UM ARQUIVO AUXILIAR///ABERTURA DO ARQUIVO AUXILIAR
  Arq_princ = fopen("./database/Fornecedores.bin","a+b");///ABERTURA DO ARQUIVO

  Fornecedor alt;
  char nome[20];
  int option,a;

    Limpar();
    printf("+============================================================+");
    printf("\n|                  ALTERAR FORNECEDORES                    |\n");
    printf("+============================================================+");
    printf("\n [1] - Alterar Nome                                         ");
    printf("\n [2] - Alterar CPF/CNPJ                                     ");
    printf("\n [3] - Alterar Contato                                      ");
    printf("\n [4] - Alterar Email                                        ");
    printf("\n [5] - Alterar Endereço                                     ");
    printf("\n [6] - Alterar Todas As Opções                              ");
    printf("\n [7] - Voltar                                               ");
    printf("\n\n [0] - Menu Principal                                   \n");
    printf("+============================================================+\n");
  printf("Digite uma das opções \n");
  printf("|--> ");
  scanf("%d", &option);

  switch (option)
  {
  case 1:

      printf("Nome do Fornecedor\n");
      printf("|--> ");
      fgets(nome,sizeof(nome),stdin);

      fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
      {
        if(strcmp(alt.Nome,nome) == 0)//COMPARANDO AS DUAS STRINGS
        {
          a++;
            printf("+==================================================+\n");
            printf("| Alterar Nome:");
            fflush(stdin);
            fgets(alt.Nome,sizeof(alt.Nome),stdin);
            printf("+==================================================+\n");
        }
        fwrite(&alt,sizeof(Fornecedor),1,Aux);// GRAVANDO NO ARQUIVO
        fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      }
      if(a == 0)printf("\nNome de cadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("Fornecedores.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","Fornecedores.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
  case 2:

    printf("Nome do Fornecedor\n");
    printf("|--> ");
    fgets(nome,sizeof(nome),stdin);


    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
    fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
        if(strcmp(alt.Nome,nome) == 0)//COMPARANDO AS DUAS STRINGS
        {
            a++;
                printf("+==================================================+\n");
                printf("| Alterar CNPJ: ");
                fflush(stdin);
                fgets(alt.CNPJ,sizeof(alt.CNPJ),stdin);
                printf("+==================================================+\n");
        }
        fwrite(&alt,sizeof(Fornecedor),1,Aux);// GRAVANDO NO ARQUIVO
        fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if (a == 0)printf("\nNome de Fornecedor Não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!\n");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
          remove("Fornecedores.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
          rename("AUXILIAR.bin","Fornecedores.bin");///Renomendo o arquivo auxiliar
          remove("AUXILIAR.bin");/// Excluindo o arquivo
          getch();
          break;
  case 3:

    printf("Nome do Fornecedor\n");
    printf("|--> ");
    fgets(nome,sizeof(nome),stdin);


    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
    fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ)){//INDICADOR DO FINAL DO ARQUIVO
            if(strcmp(alt.Nome,nome) == 0)//COMPARANDO AS DUAS STRINGS
            {
              a++;
                    printf("+==================================================+\n");
                    printf("| Altera Telefone para contato");
                    fflush(stdin);
                    fgets(alt.Contato,sizeof(alt.Contato),stdin);
                    printf("+==================================================+\n");

            }
            fwrite(&alt,sizeof(Fornecedor),1,Aux);// GRAVANDO NO ARQUIVO
            fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(a == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("Fornecedores.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin","Fornecedores.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;
  case 4:

      printf("Nome do Fornecedor\n");
      fflush(stdin);
      printf("|--> ");
      fgets(nome,sizeof(nome),stdin);

        fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
        fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if(strcmp(alt.Nome,nome) == 0)//COMPARANDO AS DUAS STRINGS
      {a++;

          printf("+==================================================+\n");
          printf("| Alterar E-mail ");
          fflush(stdin);
          fgets(alt.Email,sizeof(alt.Email),stdin);
          printf("+==================================================+\n");

      }
      fwrite(&alt,sizeof(Fornecedor),1,Aux);// GRAVANDO NO ARQUIVO
      fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
     if(a == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("Fornecedores.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin","Fornecedores.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;
  case 5:

      printf("nome do Fornecedor\n");
      fflush(stdin);
      printf("|--> ");
      fgets(nome,sizeof(nome),stdin);


      fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if(strcmp(alt.Nome,nome)== 0)//COMPARANDO AS DUAS STRINGS
      {
        a++;

          printf("+==================================================+\n");
          printf("| Alterar Endereço:");
          fflush(stdin);
          fgets(alt.Endereco,sizeof(alt.Endereco),stdin);
          printf("+==================================================+\n");

      }
      fwrite(&alt,sizeof(Fornecedor),1,Aux);// GRAVANDO NO ARQUIVO
      fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(a == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("Fornecedores.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin","Fornecedores.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;
  case 6:

      printf("nome do Fornecedor\n");
      printf("|--> ");
      fgets(nome,sizeof(nome),stdin);

      fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if(strcmp(alt.Nome,nome) == 0)//COMPARANDO AS DUAS STRINGS
      {
        a++;

      printf("+==============================================================+\n");
      fflush(stdin);
      printf("| Nome Completo:");
      fgets(alt.Nome,sizeof(alt.Nome),stdin);
      printf("+==============================================================+\n");
      printf("| CPF/CNPJ:");
      fflush(stdin);
      fgets(alt.CNPJ,sizeof(alt.CNPJ),stdin);
      printf("+==============================================================+\n");
      printf("| Telefone para Contato:");
      fflush(stdin);
      fgets(alt.Contato,sizeof(alt.Contato),stdin);
      printf("+==============================================================+\n");
      printf("| Email:");
      fflush(stdin);
      fgets(alt.Email,sizeof(alt.Email),stdin);
      printf("+==============================================================+\n");
      printf("| Endereço:");
      fflush(stdin);
      fgets(alt.Endereco,sizeof(alt.Endereco),stdin);
      printf("+==============================================================+\n");

      }
      fwrite(&alt,sizeof(Fornecedor),1,Aux);// GRAVANDO NO ARQUIVO
      fread(&alt,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
     if(a == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("Fornecedores.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin","Fornecedores.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;
      case 7: fornecedoresScreen();
              break;
    default:
        Limpar();
              clientesScreen();
          break;
  }
    Limpar();
    printf("\n+==============================================+");
    printf("\n [1] - Continuar                                ");
    printf("\n [2] - Voltar                                   ");
    printf("\n\n [0] - Menu Principal                            ");
    printf("\n+==============================================+\n\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
    switch (option)
    {
    case 1: Limpar();
            alterar_Fornecedor();
            break;
    case 2: fornecedoresScreen();
            break;
    default: clientesScreen();
            break;
    }
}

void listafornecedores(){
    Arq_princ = fopen("./database/Fornecedores.bin", "a+b");///ABERTURA DO ARQUIVO
    Fornecedor lista;
    int x=1;

    if(Arq_princ){
        while(!feof(Arq_princ)){//INDICADOR DO FINAL DO ARQUIVO
            if(fread(&lista,sizeof(Fornecedor),1,Arq_princ)){//LENDO OS BLOCOS DO ARQUIVO
                printf("\n+=======================================================+\n");
                printf("|              LISTA DE FORNECEDORES (%d)                |\n",x++);
                printf("+=======================================================+\n");
                printf("| Nome: %s",lista.Nome);
                printf("+=======================================================+\n");
                printf("| TIPO PESSOA/JURIDICA: %s",lista.tipo);
                printf("+=======================================================+\n");
                printf("| CPF/CNPJ: %s",lista.CNPJ);
                printf("+=======================================================+\n");
                printf("| Telefone: %s",lista.Contato);
                printf("+=======================================================+\n");
                printf("| Email: %s",lista.Email);
                printf("+=======================================================+\n");
                printf("| Endereço: %s",lista.Endereco);
                printf("+=======================================================+\n");
            }
        }fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    }else{
        printf("Erro ao abrir o Arquivo\n");
       Wait(1);///congelar a tela
       fornecedoresScreen();
    }
    pesquisafornecedor();
}

void pesquisafornecedor(){
   Arq_princ = fopen("./database/Fornecedores.bin", "a+b");///ABERTURA DO ARQUIVO

   Fornecedor pesquisa;
   char nome[40];
   int option;
   int x;
    printf("\n\n");
    printf("+==========================================================+");
    printf("\n [1] - Para Continuar                                       ");
    printf("\n [2] - Voltar                                               \n");
    printf("+==========================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: fornecedoresScreen();
            break;
  }
    printf("\n");
    printf("+==============================================================+\n");
    printf("|                    PESQUISA DE FORNECEDOR                    | \n");
    printf("+==============================================================+\n");
    printf("Digite o nome do Fornecedor\n");
    printf("|--> ");
    fflush(stdin);
    fgets(nome,sizeof(nome),stdin);
    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
    fread(&pesquisa,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while(!feof(Arq_princ)){//INDICADOR DO FINAL DO ARQUIVO
        if(strcmp(nome,pesquisa.Nome)== 0){//COMPARANDO AS DUAS STRINGS
            Limpar();
            x++;
            printf("+==============================================================+\n");
            printf("|  Nome: %s",pesquisa.Nome);
            printf("+==============================================================+\n");
            printf("|  TIPO PESSOA/JURIDICA: %s",pesquisa.tipo);
            printf("+==============================================================+\n");
            printf("|  CPF/CNPJ: %s",pesquisa.CNPJ);
            printf("+==============================================================+\n");
            printf("|  Telefone: %s",pesquisa.Contato);
            printf("+==============================================================+\n");
            printf("|  Email: %s",pesquisa.Email);
            printf("+==============================================================+\n");
            printf("|  Endereço: %s",pesquisa.Endereco);
            printf("+==============================================================+\n");
            getch();
        }
        fread(&pesquisa,sizeof(Fornecedor),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(x == 0) {printf("\nFornecedor não encotrado!");
      Wait(2);///congelar a tela
    }
    fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    Limpar();
    printf("\n+===================================================+");
    printf("\n [1] - Para Continuar                                ");
    printf("\n [2] - Voltar                                        ");
    printf("\n\n [0] - Menu Principal                                ");
    printf("\n+===================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
  case 1: Limpar();
          listafornecedores();
          break;
  case 2: fornecedoresScreen();
          break;
  default: clientesScreen();
          break;
  }
}

void oficinasScreen()
{
  int option;

        Limpar();
        printf("+==========================================================+\n");
        printf("|                       OFICINAS                            |");
        printf("\n+==========================================================+");
        printf("\n [1] - Novo Cadastro                                        ");
        printf("\n [2] - Alterar Cadastro                                     ");
        printf("\n [3] - Lista/Pesquisa Oficinas                              ");
        printf("\n [4] - Excluir Cadastro                                     ");
        printf("\n [5] - Alterar Cadastro                                     ");
        printf("\n\n [0] - Menu Principal                                       ");
        printf("\n+==========================================================+\n");
    printf("Digite uma das opções \n");
    printf("|--> ");
    scanf("%d",&option);
  switch(option)
  {
    case 1: cadastro_de_Oficinas();
            break;
    case 2: alterar_oficinas();
            break;
    case 3: Limpar();
            listaoficina();
            break;
    case 4: Excluir_oficinas();
            break;
    case 5: alterar_oficinas();
            break;
    default:
        Limpar();
        clientesScreen();
        break;
  }
}


void cadastro_de_Oficinas()
{
    Arq_princ = fopen("./database/Oficinas.bin","a+b");///ABERTURA DO ARQUIVO
  oficina f;
  int option;
    Limpar();
    printf("+==========================================================+");
    printf("\n [1] - Para Continuar                                       ");
    printf("\n [2] - Voltar                                               ");
    printf("\n+============================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: oficinasScreen();
            break;
  }
    if(Arq_princ)
    {       Limpar();
            printf("+==============================================================+\n");
            printf("|                 NOVO CADASTRO DE OFICINA                     |\n");
            printf("+==============================================================+\n");
            fflush(stdin);
            printf("| Nome da oficina:");
            fgets(f.nome,sizeof(f.nome),stdin);
            printf("+==============================================================+\n");
            printf("| CNPJ:");
            fflush(stdin);
            fgets(f.CNPJ,sizeof(f.CNPJ),stdin);
            printf("+==============================================================+\n");
            printf("| Contato:");
            fflush(stdin);
            fgets(f.contato,sizeof(f.contato),stdin);
            printf("+==============================================================+\n");
            printf("| Endereço:");
            fflush(stdin);
            fgets(f.endereco,sizeof(f.endereco),stdin);
            printf("+==============================================================+\n");

            fwrite(&f,sizeof(oficina),1,Arq_princ);// GRAVANDO NO ARQUIVO
            fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            getch();
    }
    else {
        printf("Erro ao abrir Arquivo!");
    Wait(1);///congelar a tela
    menuPrincipalScreen();
    }
    Limpar();
    printf("\n+===================================================+");
    printf("\n [1] - Para Continuar                                ");
    printf("\n [2] - Voltar                                        ");
    printf("\n\n [0] - Menu Principal                                ");
    printf("\n+===================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
  case 1: cadastro_de_Oficinas();
            break;
  case 2: oficinasScreen();
          break;
  default: clientesScreen();
          break;
  }
}

void alterar_oficinas()
{
  Arq_princ = fopen("./database/Oficinas.bin","a+b");///ABERTURA DO ARQUIVO
  Aux = fopen("./database/AUXILIAR.bin","wb");//ABERTURA DE UM ARQUIVO AUXILIAR///ABERTURA DO ARQUIVO AUXILIAR

  oficina alt;
  char Nome[20];
  int option,x;

    Limpar();
    printf("+==============================================================+");
    printf("\n|              ALTERAR CADASTRO DE OFICINA                   |\n");
    printf("+==============================================================+");
    printf("\n [1] - Alterar Nome                                           ");
    printf("\n [2] - Alterar CNPJ                                           ");
    printf("\n [3] - Alterar Contato                                        ");
    printf("\n [4] - Alterar Endereço                                       ");
    printf("\n [5] - Alterar Todas Aa Opções                                ");
    printf("\n [6] - Voltar                                                 ");
    printf("\n\n [0] - Menu Principal                                         \n");
    printf("+==============================================================+\n");
    printf("Digite uma das opçoes\n");
    printf("|--> ");
    scanf("%d",&option);
  switch(option)
  {
    case 1:

        printf("Digite o Nome da Oficina\n");
        printf("|--> ");
        fgets(Nome,sizeof(Nome),stdin);
      fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO

      while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
      {
        if(strcmp(alt.nome,Nome) == 0)//COMPARANDO AS DUAS STRINGS
        {x++;
            printf("+=============================================================+\n");
            printf("| Alterar Nome:");
            fflush(stdin);
            fgets(alt.nome,sizeof(alt.nome),stdin);
            printf("+=============================================================+\n");

        }
        fwrite(&alt,sizeof(oficina),1,Aux);// GRAVANDO NO ARQUIVO
        fread(&alt,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      }
     if(x == 0)printf("\nNome de cadastro não Encontrado!");
      else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("Oficinas.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","Oficinas.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
    case 2:

      printf("Digite o nome da oficina\n");
      printf("|--> ");
      fgets(Nome,sizeof(Nome),stdin);
      fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO

      while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
      {
        if(strcmp(alt.nome,Nome) == 0)//COMPARANDO AS DUAS STRINGS
        {x++;

          printf("+=============================================================+\n");
          printf("| Alterar CNPJ:");
          fflush(stdin);
          fgets(alt.CNPJ,sizeof(alt.CNPJ),stdin);
          printf("+=============================================================+\n");

        }
        fwrite(&alt,sizeof(oficina),1,Aux);// GRAVANDO NO ARQUIVO
        fread(&alt,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      }
       if(x == 0)printf("\nNome de cadastro não Encontrado!");
      else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("Oficinas.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","Oficinas.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
    case 3:

        printf("Digite o Nome da Oficina\n");
        printf("|--> ");
        fgets(Nome,sizeof(Nome),stdin);
      fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO

      while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
      {
        if(strcmp(alt.nome,Nome) == 0)//COMPARANDO AS DUAS STRINGS
        {
          x++;

            printf("+=============================================================+\n");
            printf("| Alterar Contato:");
            fflush(stdin);
            fgets(alt.endereco,sizeof(alt.endereco),stdin);
            printf("+=============================================================+\n");

        }
        fwrite(&alt,sizeof(oficina),1,Aux);// GRAVANDO NO ARQUIVO
        fread(&alt,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      }
       if(x == 0)printf("\nNome de cadastro não Encontrado!");
      else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("Oficinas.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","Oficinas.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
    case 4:

        printf("Digite o Nome da Oficina\n");
        printf("|--> ");
        fgets(Nome,sizeof(Nome),stdin);
      fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO

      while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
      {
        if(strcmp(alt.nome,Nome) == 0)//COMPARANDO AS DUAS STRINGS
        {
          x++;

            printf("+=============================================================+\n");
            printf("| Alterar Endereço:");
            fflush(stdin);
            fgets(alt.endereco,sizeof(alt.endereco),stdin);
            printf("+=============================================================+\n");

        }
        fwrite(&alt,sizeof(oficina),1,Aux);// GRAVANDO NO ARQUIVO
        fread(&alt,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      }
      if(x == 0)printf("\nNome de cadastro não Encontrado!");
      else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("Oficinas.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","Oficinas.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
    case 5:

        printf("Digite o Nome da Oficina\n");
        printf("|--> ");
        fgets(Nome,sizeof(Nome),stdin);
      fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
      fread(&alt,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO

      while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
      {
        if(strcmp(alt.nome, Nome) == 0)//COMPARANDO AS DUAS STRINGS
        {
          x++;

            printf("+==============================================================+\n");
            fflush(stdin);
            printf("| Nome da oficina:");
            fgets(alt.nome,sizeof(alt.nome),stdin);
            printf("+==============================================================+\n");
            printf("| CNPJ:");
            fflush(stdin);
            fgets(alt.CNPJ,sizeof(alt.CNPJ),stdin);
            printf("+==============================================================+\n");
            printf("| Contato:");
            fflush(stdin);
            fgets(alt.contato,sizeof(alt.contato),stdin);
            printf("+==============================================================+\n");
            printf("| Endereço:");
            fflush(stdin);
            fgets(alt.endereco,sizeof(alt.endereco),stdin);
            printf("+==============================================================+\n");

        }
        fwrite(&alt,sizeof(oficina),1,Aux);// GRAVANDO NO ARQUIVO
        fread(&alt,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      }
      if(x == 0)printf("\nNome de cadastro não Encontrado!");
      else printf("\nCadastro Alterado com Sucesso!\n");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
        remove("Oficinas.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","Oficinas.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
    break;
    case 6: oficinasScreen();
            break;
    default: clientesScreen();
              break;
  }
    Limpar();
   printf("\n+==============================================+");
    printf("\n [1] - Continuar                                ");
    printf("\n [2] - Voltar                                   ");
    printf("\n\n [0] - Menu Principal                            ");
    printf("\n+==============================================+\n\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
    switch (option)
    {
    case 1:
      Limpar();
      alterar_oficinas();
      break;
    case 2: oficinasScreen();
            break;
    default: clientesScreen();
            break;
    }
}

void Excluir_oficinas()
{
    Arq_princ = fopen("./database/Oficinas.bin","a+b");///ABERTURA DO ARQUIVO
    Aux = fopen("./database/AUXILIAR.bin","wb");//ABERTURA DE UM ARQUIVO AUXILIAR///ABERTURA DO ARQUIVO AUXILIAR

    char Nome[20];
    oficina del;
    int option,x;

    Limpar();
    printf("+==========================================================+");
    printf("\n[1] - Para Continuar                                       ");
    printf("\n[2] - Voltar                                               ");
    printf("\n+============================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: oficinasScreen();
            break;
  }
    Limpar();
    printf("+============================================================+\n");
    printf("|                    EXCLUIR CADASTRO                        |\n ");
    printf("+------------------------------------------------------------+\n");
    printf("Nome da Oficina\n");
    fflush(stdin);
    printf("|--> ");
    fgets(Nome,sizeof(Nome),stdin);
    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
    fread(&del,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
       if(strcmp(del.nome,Nome)== 0) x++;//COMPARANDO AS DUAS STRINGS
      else fwrite(&del,sizeof(oficina),1,Aux);// GRAVANDO NO ARQUIVO
      fread(&del,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(x == 0)printf("\nNumero de cadastro Não Encotrado!");
  else printf("\nCadastro Excluido com Sucesso!\n");
    fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    remove("Oficinas.bin");/// Excluindo o arquivo
    fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
    rename("AUXILIAR.bin","Oficinas.bin");///Renomendo o arquivo auxiliar
    remove("AUXILIAR.bin");/// Excluindo o arquivo
  getch();
    Limpar();
    printf("\n+===================================================+");
    printf("\n[1] - Para Continuar                                ");
    printf("\n[2] - Voltar                                        ");
    printf("\n\n[0] - Menu Principal                                ");
    printf("\n+===================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
  case 1: Excluir_oficinas();
        break;
  case 2: oficinasScreen();
          break;
  default: clientesScreen();
          break;
  }
}

void listaoficina()
{
    Arq_princ = fopen("./database/Oficinas.bin","a+b");///ABERTURA DO ARQUIVO

    oficina of;
    int a=1;
    int option;

    if(Arq_princ)
    {
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(fread(&of,sizeof(oficina),1,Arq_princ))//LENDO OS BLOCOS DO ARQUIVO
            {
                printf("+==============================================================+\n");
                printf("|                     LISTA DE OFICINAS (%d)                   |\n",a++);
                printf("+==============================================================+");
                printf("\n| Nome: %s",of.nome);
                printf("+==============================================================+");
                printf("\n| CNPJ: %s",of.CNPJ);
                printf("+==============================================================+");
                printf("\n| Contato: %s",of.contato);
                printf("+==============================================================+");
                printf("\n| Endereco: %s",of.endereco);
                printf("+==============================================================+\n");
            }
        }
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    }
    else
    {
        printf("\tErro ao Abrir Arquivo!\n");
        Wait(1);///congelar a tela
        menuPrincipalScreen();
    }
    printf("\n");
    printf("+==========================================================+");
    printf("\n[1] - Para Continuar                                       ");
    printf("\n[2] - Voltar                                               ");
    printf("\n+============================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: pesquisa_Oficinas();
            break;
    case 2: oficinasScreen();
            break;
  }
}

void pesquisa_Oficinas()
{
  Arq_princ = fopen("./database/Oficinas.bin","a+b");///ABERTURA DO ARQUIVO

  oficina of_Pesquisa;
  char nome[20];
  int option;
  int a=0;

    printf("\n");
    printf("+==============================================================+\n");
    printf("|                      PESQUISA DE OFICINA                     |\n");
    printf("+==============================================================+\n");
    printf("Digite o nome da oficina\n");
    fflush(stdin);
    printf("|--> ");
    fgets(nome,sizeof(nome),stdin);
    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
    fread(&of_Pesquisa,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if (strcmp(of_Pesquisa.nome,nome) == 0)//COMPARANDO AS DUAS STRINGS
      {a++;
        Limpar();
          printf("+==============================================================+");
          printf("\n| Nome: %s",of_Pesquisa.nome);
          printf("+==============================================================+");
          printf("\n| CNPJ: %s",of_Pesquisa.CNPJ);
          printf("+==============================================================+");
          printf("\n| Contato: %s",of_Pesquisa.contato);
          printf("+==============================================================+");
          printf("\n| Endereço: %s",of_Pesquisa.endereco);
          printf("+==============================================================+\n");

        getch();
      }
       fread(&of_Pesquisa,sizeof(oficina),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(a == 0){ printf("\noficina não encotrada!");
    Wait(2);///congelar a tela
    }
    fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    Limpar();
    printf("\n+===================================================+");
    printf("\n[1] - Para Continuar                                ");
    printf("\n[2] - Voltar                                        ");
    printf("\n\n[0] - Menu Principal                                ");
    printf("\n+===================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
  case 1: Limpar() ;
        listaoficina();
        break;
  case 2: oficinasScreen();
          break;
  default: clientesScreen();
          break;
  }
}

void usuarioScreen(){

    int option;

    Limpar();
   printf("\n+============================================================+");
   printf("\n|                        USUÁRIO                             |");
   printf("\n+============================================================+");
   printf("\n[1] - Alterar Usuário                                        ");
   printf("\n[2] - Lista De Usuários                                      ");
   printf("\n\n[0] - Menu Principal                                       ");
   printf("\n+============================================================+\n");
   printf("Digite uma das opções\n");
   printf("|--> ");
   scanf("%d", &option);
   do{
    switch(option){
      case 1: alterarUsuario();
              break;
      case 2: Limpar();
              listausuario();
              break;
      default: clientesScreen();
                break;
    }
   }while(option == 0);
}

void alterarUsuario(){

  Aux = fopen("./database/AUXILIAR.bin","wb");///ABERTURA DO ARQUIVO AUXILIAR
  arquivo = fopen("./database/usuarios.bin", "a+b");

  login user;
  char nome[20];
  int option, x;

    Limpar();
    printf("+============================================================+");
    printf("\n|                     ALTERAR USUÁRIO                      |\n");
    printf("+============================================================+");
    printf("\n [1] - Alterar Nome                                         ");
    printf("\n [2] - Alterar Senha                                        ");
    printf("\n [3] - Voltar                                               ");
    printf("\n\n[0] - Menu Principal                                    \n");
    printf("+============================================================+\n");
    printf("Digite uma das Opções\n");
    printf("|--> ");
    scanf("%d", &option);
    switch (option){

      case 1:
            printf("Nome Do Usuário:");
            fgets(nome,sizeof(nome),stdin);
            fseek(arquivo,SEEK_SET,1);//Movendo-se dentro do arquivo
            fread(&user,sizeof(login),1,arquivo);//LENDO OS BLOCOS DO ARQUIVO
          while (!feof(arquivo)){//INDICADOR DO FINAL DO ARQUIVO
            if(strcmp(user.usuario,nome)==0){//COMPARANDO AS DUAS STRINGS
              x++;
              printf("+==================================================+\n");
              printf("| Alterar Nome de Usuário:");
              fflush(stdin);
              fgets(user.usuario,sizeof(user.usuario),stdin);
              printf("+==================================================+\n");
            }
            fwrite(&user,sizeof(login),1,Aux);// GRAVANDO NO ARQUIVO
            fread(&user,sizeof(login),1,arquivo);//LENDO OS BLOCOS DO ARQUIVO
          }
          if (x == 0) printf("\nNome de cadastro não Encotrado!");
          else printf("\nCadastro Alterado com Sucesso!\n");
          fclose(arquivo);
        remove("usuarios.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","usuarios.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
          break;
      case 2:
          printf("Digite o Nome do Usuário:");
          fgets(nome,sizeof(nome),stdin);
          fseek(arquivo,SEEK_SET,1);//Movendo-se dentro do arquivo
          fread(&user,sizeof(login),1,arquivo);//LENDO OS BLOCOS DO ARQUIVO
          while (!feof(arquivo)){//INDICADOR DO FINAL DO ARQUIVO
            if(strcmp(user.usuario,nome)== 0){//COMPARANDO AS DUAS STRINGS
              x++;
              printf("+==================================================+\n");
              printf("| Alterar Senha:");
              fflush(stdin);
              fgets(user.senha1,sizeof(user.senha1),stdin);
              printf("+==================================================+\n");
            }
            fwrite(&user,sizeof(login),1,Aux);// GRAVANDO NO ARQUIVO
            fread(&user,sizeof(login),1,arquivo);//LENDO OS BLOCOS DO ARQUIVO
          }
            if (x == 0) printf("\nNome de cadastro não Encotrado!");
          else printf("\nCadastro Alterado com Sucesso!\n");
          fclose(arquivo);
        remove("usuarios.bin");/// Excluindo o arquivo
      fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
        rename("AUXILIAR.bin","usuarios.bin");///Renomendo o arquivo auxiliar
        remove("AUXILIAR.bin");/// Excluindo o arquivo
        getch();
          break;
      case 3: usuarioScreen();
              break;
      default: clientesScreen();
                break;
    }
    Limpar();
     printf("\n+==============================================+");
    printf("\n [1] - Continuar                                ");
    printf("\n [2] - Voltar                                   ");
    printf("\n\n [0] - Menu Principal                            ");
    printf("\n+==============================================+\n\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
    switch (option)
    {
    case 1:
      Limpar();
      alterarUsuario();
      break;
    case 2: usuarioScreen();
            break;
    default: clientesScreen();
            break;
    }
}

void listausuario(){
  arquivo = fopen("./database/usuarios.bin", "a+b");

  login lista;
  int x=1;
  if (arquivo){
    while(!feof(arquivo)){//INDICADOR DO FINAL DO ARQUIVO
      if (fread(&lista,sizeof(login),1,arquivo)){//LENDO OS BLOCOS DO ARQUIVO
        printf("+==================================================+\n");
        printf("|                 lista de Usuários (%d)           |\n",x++);
        printf("+==================================================+\n");
        printf("| Usuário:%s",lista.usuario);
        printf("+==================================================+\n");
      }
    }fclose(arquivo);
  }else{
    printf("\nErro ao abrir arquivo!");
    Wait(1);///congelar a tela
    usuarioScreen();
  }
  pesquisausuario();
}

void pesquisausuario(){
  arquivo = fopen("./database/usuarios.bin", "a+b");
  login lista;
  char nome[20];
  int option;
  int x;

  printf("\n");
  printf("+==================================================+\n");
  printf("|               PESQUISA DE USUÁRIO                |\n");
  printf("+==================================================+\n");
  printf("Digite o Nome do usuário\n");
  printf("|--> ");
  fflush(stdin);
  fgets(nome,sizeof(nome),stdin);
  fseek(arquivo,SEEK_SET,1);//Movendo-se dentro do arquivo
  fread(&lista,sizeof(login),1,arquivo);//LENDO OS BLOCOS DO ARQUIVO
  while (!feof(arquivo)){//INDICADOR DO FINAL DO ARQUIVO
    if (strcmp(lista.usuario,nome)==0){//COMPARANDO AS DUAS STRINGS
      x++;
      Limpar();
      printf("+==================================================+\n");
      printf("| Usuario:%s",lista.usuario);
      printf("+==================================================+\n");
      getch();
    }
    fread(&lista,sizeof(login),1,arquivo);//LENDO OS BLOCOS DO ARQUIVO
  }
  if(x==0){ printf("\nUsuário não encotrado!");
    Wait(2);///congelar a tela
  }
    fclose(arquivo);
    Limpar();
    printf("\n+===================================================+");
    printf("\n [1] - Para Continuar                                ");
    printf("\n [2] - Voltar                                        ");
    printf("\n\n[0] - Menu Principal                                ");
    printf("\n+===================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option){
  case 1: listausuario();
        break;
  case 2: usuarioScreen();
          break;
  default: clientesScreen();
          break;
  }
}

void atendimentoScreen(){
    int option;

    Limpar();
    printf("+========================================================+");
    printf("\n|                 MENU DE ATENDIMENTO                    |");
    printf("\n+========================================================+");
    printf("\n[1] - Consulta de Preço");
    printf("\n[2] - Ordem de Serviço");
    printf("\n[3] - Pesquisa de Placas");
    printf("\n\n[0] - Menu Principal");
    printf("\n+========================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d", &option);
    switch(option){
    case 1: Limpar();
            listavalor();
            break;
    case 2: ordemScreem();
            break;
    case 3: Limpar();
            listaplaca();
            break;
    default: menuPrincipalScreen();
            break;
    }
}

void listavalor(){

  Arq_princ = fopen("./database/ArqProdutoPneus.bin","a+b");///ABERTURA DO ARQUIVO
  estoque lista;
  int x=1;

  if (Arq_princ){
    while (!feof(Arq_princ)){//INDICADOR DO FINAL DO ARQUIVO
      if(fread(&lista,sizeof(estoque),1,Arq_princ)){//LENDO OS BLOCOS DO ARQUIVO

        printf("|                    PREÇOS DOS PNEUS (%d)                      |",x++);
        printf("\n+==============================================================+\n");
        printf("| Codigo do Produto: %d", lista.Codigo);
        printf("\n+==============================================================+\n");
        printf("| Nome do Produto: %s",lista.Nome);
        printf("+==============================================================+\n");
        printf("| Preço do Produto: R$%.2f",lista.Preco);
        printf("\n+==============================================================+\n");
        printf("\n");
      }
    }fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
  }else{
      printf("\nErro ao abrir Arquivo!\n");
      Wait(1);///congelar a tela
      atendimentoScreen();
    }
    Arq_princ = fopen("./database/ArqProdutoIluminacao.bin","a+b");///ABERTURA DO ARQUIVO
  estoque listaB;
  int b=1;

    if(Arq_princ){
      while(!feof(Arq_princ)){//INDICADOR DO FINAL DO ARQUIVO
        if(fread(&listaB,sizeof(estoque),1,Arq_princ)){//LENDO OS BLOCOS DO ARQUIVO

        printf("|                   PREÇOS DAS ILUMINAÇÕES (%d)                 |",b++);
        printf("\n+==============================================================+\n");
        printf("| Codigo do Produto: %d", listaB.Codigo);
        printf("\n+==============================================================+\n");
        printf("| Nome do Produto: %s",listaB.Nome);
        printf("+==============================================================+\n");
        printf("| Preço do Produto: R$%.2f",listaB.Preco);
        printf("\n+==============================================================+\n");
        printf("\n");
        }
      }
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    }else{
      printf("\tErro ao abrir Arquivo!\n");
      Wait(1);///congelar a tela
      atendimentoScreen();
    }
    pesquisarvalor();
}

void pesquisarvalor(){
  estoque pesquisar;
  int x=0;
  int option;
  int codigo;

  int opc;
        printf("\n\n");
        printf("+============================================================+");
        printf("\n|         QUAL O TIPO DE PRODUTO ESTA PESQUISANDO            |");
        printf("\n+============================================================+");
        printf("\n[1] - Pneu                                                 ");
        printf("\n[2] - Iluminacao                                           ");
        printf("\n+============================================================+\n");

    printf("Digite uma opção");
    printf("\n|--> ");
    scanf("%d", &opc);

  if(opc == 1)
    Arq_princ = fopen("./database/ArqProdutoPneus.bin","a+b");///ABERTURA DO ARQUIVO
  else
    Arq_princ = fopen("./database/ArqProdutoIluminacao.bin","a+b");///ABERTURA DO ARQUIVO



  printf("+==============================================================+\n");
  printf("|                  PESQUISA DE VALORES                         |\n");
  printf("+==============================================================+\n");
  printf("Digite o Codigo do Produto\n");
  printf("|--> ");
  scanf("%d",&codigo);
  fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
  fread(&pesquisar,sizeof(estoque),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO

  while (!feof(Arq_princ)){//INDICADOR DO FINAL DO ARQUIVO
    if(codigo == pesquisar.Codigo){
      x++;
      Limpar();
      printf("\n");
      printf("| Codigo Produto: %d",pesquisar.Codigo);
      printf("\n+==============================================================+\n");
      printf("| Marca: %s",pesquisar.Marca);
      printf("+==============================================================+\n");
      printf("| Nome do Produto: %s",pesquisar.Nome);
      printf("+==============================================================+\n");
      printf("| Preço do Produto: R$%.2f",pesquisar.Preco);
      printf("\n+==============================================================+\n");
      printf("| Cor: %s",pesquisar.Cor);
      printf("+==============================================================+\n");
      printf("| Quantidade em Estoque: %d",pesquisar.Num_em_estoque);
      printf("\n+==============================================================+\n");
      getch();
    }
    fread(&pesquisar,sizeof(estoque),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
  }
  if (x==0) printf("\nCodigo não encotrado!");
  fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    Wait(2);///congelar a tela
    Limpar();
    printf("\n+===================================================+");
    printf("\n [1] - Para Continuar                                ");
    printf("\n [2] - Voltar                                        ");
    printf("\n\n [0] - Menu Principal                                ");
    printf("\n+===================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
  case 1: Limpar();
          listavalor();
          break;
  case 2: atendimentoScreen();
          break;
  default: menuPrincipalScreen();
          break;
  }
}

void listaplaca(){

    Cadastro pesquisar;
    int a=1,option;
  Arq_princ = fopen("./database/cliente.bin","rb");///ABERTURA DO ARQUIVO

  if (Arq_princ)
  {
    while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if (fread(&pesquisar,sizeof(Cadastro),1,Arq_princ))//LENDO OS BLOCOS DO ARQUIVO
      {
          printf("+==============================================================+\n");
          printf("|                     LISTA DE CLIENTES (%d)                    |\n",a++);
          printf("+==============================================================+\n");
          printf("| Nome: %s| Placa: %s",pesquisar.Nome,pesquisar.placa);
          printf("+==============================================================+\n");
          printf("\n");

          fflush(stdin);
      }
    }
    fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
  }
  else{
  printf("\tErro ao abrir Arquivo!");
  Wait(1);///congelar a tela
    clientesScreen();
  }

  printf("\n\n");
  printf("+===========================================================+\n");
  printf("[1] - Para Continuar                                        |\n");
  printf("[2] - Voltar                                                |\n");
  printf("+===========================================================+\n");
  printf("|--> ");
  scanf("%d", &option);
  switch(option){
    case 1:
          break;
    default:atendimentoScreen();
          break;
  }
  pesquisarplaca();
}

void pesquisarplaca(){
    Cadastro pesquisar;
    int x;
    int option;
    char placa[10];
    Arq_princ = fopen("./database/cliente.bin","rb");///ABERTURA DO ARQUIVO

    printf("\n\n");
    printf("+==============================================================+");
    printf("\n|                   PESQUISA DE PLACAS                         |");
    printf("\n+==============================================================+\n");
    printf("Digite a placa \n");
    fflush(stdin);
    printf("|--> ");
    fgets(placa,sizeof(placa),stdin);
    fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
    fread(&pesquisar,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO

    while(!feof(Arq_princ)){//INDICADOR DO FINAL DO ARQUIVO
        if(strcmp(placa,pesquisar.placa) == 0){//COMPARANDO AS DUAS STRINGS
            x++;
            Limpar();
            printf("| Placa: %s",pesquisar.placa);
            printf("+==============================================================+");
            printf("\n| Modelo do Veiculo: %s",pesquisar.Modelo);
            printf("+==============================================================+");
            printf("\n| Proprietario: %s",pesquisar.Nome);
            printf("+==============================================================+");
            printf("\n| CPF: %s",pesquisar.CPF);
            printf("\n+==============================================================+");
            printf("\n| Contato: %s",pesquisar.Contato);
            printf("+==============================================================+\n");
            getch();
        }
        fread(&pesquisar,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(x == 0) printf("\n\nPlaca não encotrada");
     fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
     Wait(2);///congelar a tela
     Limpar();
    printf("\n+===================================================+");
    printf("\n [1] - Para Continuar                                ");
    printf("\n [2] - Voltar                                        ");
    printf("\n\n [0] - Menu Principal                                ");
    printf("\n+===================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
  case 1: Limpar();
          listaplaca();
          break;
  case 2: atendimentoScreen();
          break;
  default: menuPrincipalScreen();
          break;
  }
}

void ordemScreem(){
  int option;
  Limpar();
  printf("+============================================================+\n");
  printf("|                   ORDEM DE SERVIÇO                         |\n");
  printf("+============================================================+\n");
  printf("[1] - Criar Nova ordem de Serviço\n");
  printf("[2] - Pesquisar Ordem de Serviço\n");
  printf("[3] - Excluir Ordem de Serviço");
  printf("\n\n[0] - Menu Principal");
  printf("\n+============================================================+\n");
  printf("Digite uma das Opções\n");
  printf("|--> ");
  scanf("%d", &option);

  do{
    switch (option)
    {
      case 1: novaordem();
            break;
      case 2: pesquisarordem();
            break;
      case 3: excluirordem();
            break;
      default:atendimentoScreen();
            break;
    }
  }while (option == 0);
}

void novaordem(){

  arq_ordem = fopen("./database/ordemserv.txt","a+b");///ABERTURA DO ARQUIVO
  Cadastro pesq;
  int i=1;
  Arq_princ = fopen("./database/cliente.bin","a+b");//ABERTURA DE UM ARQUIVO///ABERTURA DO ARQUIVO
  int option;
  servico c;

  Limpar();
  printf("+===========================================================+\n");
  printf("[1] - Para Continuar                                        |\n");
  printf("[2] - Voltar                                                |\n");
  printf("+===========================================================+\n");
  printf("|--> ");
  scanf("%d", &option);
  switch(option){
    case 1:
          break;
    default:ordemScreem();
          break;
  }

  if(Arq_princ){
    while(!feof(Arq_princ)){//INDICADOR DO FINAL DO ARQUIVO
      if(fread(&pesq,sizeof(Cadastro),1,Arq_princ)){//LENDO OS BLOCOS DO ARQUIVO
          printf("+==============================================================+\n");
          printf("|                     LISTA DE CLIENTES (%d)                    |\n",i++);
          printf("+==============================================================+");
          printf("\n| Nome: %s",pesq.Nome);
          printf("\n+==============================================================+\n");
      }
    }fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
  }else{
    printf("\nErro ao abrir o Arquivo!!");
    Wait(1);///congelar a tela
    ordemScreem();
  }

  if(arq_ordem){

    printf("+===========================================================+\n");
    printf("|                 NOVA ORDEM DE SERVIÇO                     |\n");
    printf("+===========================================================+\n");
    fflush(stdin);
    printf("| Data de Inicio da Manutenção (dd/mm/aaaa): ");
    scanf("%d %d %d",&c.dia,&c.mes,&c.ano);
    printf("+===========================================================+\n");
    printf("| Nome do Mecanico: ");
    fflush(stdin);
    fgets(c.mecanico,sizeof(c.mecanico),stdin);
    printf("+===========================================================+\n");
    printf("| Itens Utilizados:");
    fflush(stdin);
    fgets(c.item,sizeof(c.item),stdin);
    printf("+===========================================================+\n");
    printf("| Nome do Cliente:");
    fflush(stdin);
    fgets(c.nome,sizeof(c.nome),stdin);
    printf("+===========================================================+\n");
    printf("| Valor Total:");
    fflush(stdin);
    scanf("%f",&c.valor);
    printf("+===========================================================+\n");
    fwrite(&c,sizeof(servico),1,arq_ordem);// GRAVANDO NO ARQUIVO
    fclose(arq_ordem);
    getch();
  }
  else{
    printf("\nErro ao abrir o Arquivo");
    Wait(1);///congelar a tela
    ordemScreem();
  }
  Limpar();
  printf("+============================================================+");
    printf("\n [1] - Para Continuar                                       ");
    printf("\n [2] - Voltar                                               ");
    printf("\n\n [0] - Menu Principal                                        ");
     printf("\n+=========================================================+\n");
    printf("Digite uma das opções\n");
    fflush(stdin);
  printf("|--> ");
  scanf("%d",&option);
  switch (option)
  {
    case 1: novaordem();
            Limpar();
            break;
    case 2: ordemScreem();
            break;
    default: atendimentoScreen();
            break;
  }
}

void pesquisarordem(){

  servico s;
  Cadastro c;

  Arq_princ = fopen("./database/Cliente.bin","r+b");///ABERTURA DO ARQUIVO
  arq_ordem = fopen("./database/ordemserv.txt","r+b");///ABERTURA DO ARQUIVO

  char placa[20],cliente[20];
  int option;
  int x=0;

    Limpar();
  printf("+===========================================================+\n");
  printf("[1] - Para Continuar                                        |\n");
  printf("[2] - Voltar                                                |\n");
  printf("+===========================================================+\n");
  printf("|--> ");
  scanf("%d", &option);
  switch(option){
    case 1:
          break;
    default:ordemScreem();
          break;
  }

  if (Arq_princ,arq_ordem == NULL){
    printf("\nNão foi possivel abrir este arquivo!");
  }else{
  Limpar();
  printf("|               PESQUISAR ORDEM DE SERVIÇO                |\n");
  printf("+=========================================================+\n");

  printf("Digite\n");

  printf("Placa do Veiculo:\n");
  fflush(stdin);
  printf("|--> ");
  fgets(placa,sizeof(placa),stdin);

  printf("Nome do cliente:\n");
  printf("|--> ");
  fflush(stdin);
  fgets(cliente,sizeof(cliente),stdin);

    do{
        if(strcmp(placa,c.placa)==0 ){//COMPARANDO AS DUAS STRINGS

        }else{
            fseek(Arq_princ,SEEK_SET,1);//Movendo-se dentro do arquivo
        fread(&c,sizeof(Cadastro),1,Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        continue;
        }
        if(strcmp(cliente,s.nome) ==0){//COMPARANDO AS DUAS STRINGS
                break;
        }else{
            fseek(arq_ordem,SEEK_SET,1);//Movendo-se dentro do arquivo
        fread(&s,sizeof(servico),1,arq_ordem);//LENDO OS BLOCOS DO ARQUIVO
        continue;
        }
    }while(placa == c.placa == 0 && cliente == s.nome == 0 );

    if(strcmp(placa,c.placa)== 0 && strcmp(cliente,s.nome)== 0 ){//COMPARANDO AS DUAS STRINGS
      x++;
        Limpar();
        printf("|                   ORDEM DE SERVIÇO                   |\n");
        printf("+======================================================+\n");
        printf("| Modelo do Veiculo: %s",c.Modelo);
        printf("+======================================================+\n");
        printf("| Marca: %s",c.marca);
        printf("+======================================================+\n");
        printf("| Nome do Cliente: %s",c.Nome);
        printf("+======================================================+\n");
        printf("| CPF: %s",c.CPF);
        printf("\n+======================================================+\n");
        printf("| Endereço: %s",c.Endereco);
        printf("+======================================================+\n\n");

      printf("| Data do inicio da Manutenção: %d/%d/%d",s.dia,s.mes,s.ano);
      printf("\n+======================================================+\n");
      printf("| Mecanico: %s",s.mecanico);
      printf("+======================================================+\n");
      printf("| Itens utilizados: %s",s.item);
      printf("+======================================================+\n");
      printf("| Valor Final: R$%.2f",s.valor);
      printf("\n+======================================================+\n");
    }
    if(x==0) printf("\n Ordem de Serviço não encontrada!");
  }
  fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
  fclose(arq_ordem);
  getch();

    Limpar();
   printf("\n+===================================================+");
    printf("\n [1] - Para Continuar                                ");
    printf("\n [2] - Voltar                                        ");
    printf("\n\n [0] - Menu Principal                                ");
    printf("\n+===================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
  case 1:Limpar() ;
        pesquisarordem();
        break;
  case 2: ordemScreem();
          break;
  default: atendimentoScreen();
          break;
  }
}

void excluirordem(){

  arq_ordem = fopen("./database/ordemserv.txt","r");///ABERTURA DO ARQUIVO
  Aux = fopen("./database/AUXILIAR.bin","wb");//ABERTURA DE UM ARQUIVO AUXILIAR///ABERTURA DO ARQUIVO AUXILIAR

  char nome[20];
  servico del;
  int option,x;

    Limpar();
    printf("+==========================================================+");
    printf("\n [1] - Para Continuar                                       ");
    printf("\n [2] - Voltar                                               ");
    printf("\n+============================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
    case 1: break;
    case 2: ordemScreem();
            break;
  }
  Limpar();
    printf("+============================================================+\n");
    printf("|                    EXCLUIR CADASTRO                        |\n ");
    printf("+------------------------------------------------------------+\n");
    printf("Digite o nome do Mecanico\n");
    fflush(stdin);
    printf("|--> ");
    fgets(nome,sizeof(nome),stdin);
    fseek(arq_ordem,SEEK_SET,1);//Movendo-se dentro do arquivo
    fread(&del,sizeof(servico),1,arq_ordem);//LENDO OS BLOCOS DO ARQUIVO
    while(!feof(arq_ordem)){//INDICADOR DO FINAL DO ARQUIVO
      if(strcmp(nome,del.mecanico)==0)x++;//COMPARANDO AS DUAS STRINGS
      else fwrite(&del,sizeof(servico),1,Aux);// GRAVANDO NO ARQUIVO
      fread(&del,sizeof(servico),1,arq_ordem);//LENDO OS BLOCOS DO ARQUIVO
    }
    if(x==0) printf("\nNome não encontrado!");
    else printf("\nCadastro Excluido com Sucesso!\n\n");
    fclose(arq_ordem);
    remove("ordemserv.txt");/// Excluindo o arquivo
    fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
    rename("AUXILIAR.bin","ordemserv.txt");///Renomendo o arquivo auxiliar
    remove("AUXILIAR.bin");/// Excluindo o arquivo
    getch();

    Limpar();
    printf("\n+===================================================+");
    printf("\n [1] - Para Continuar                                ");
    printf("\n [2] - Voltar                                        ");
    printf("\n\n [0] - Menu Principal                                ");
    printf("\n+===================================================+\n");
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d",&option);
  switch (option)
  {
  case 1: excluirordem();
        break;
  case 2: ordemScreem();
          break;
  default: menuPrincipalScreen();
          break;
  }
}


void Menu_de_Estoque(){

        Limpar();
        printf("+============================================================+");
        printf("\n|                      MENU DO ESTOQUE                       |");
        printf("\n+============================================================+");
        printf("\n[1] - Lista de produtos                                       ");
        printf("\n[2] - Cadastro de produtos                                    ");
        printf("\n\n[0] - Menu Principal                                         ");
        printf("\n+============================================================+\n");

  int opc;

  do{
    printf("Digite uma das opções\n");
    printf("|--> ");
    scanf("%d", &opc);

    switch(opc){
      case 1: Limpar();
              Menu_lista();
              break;
      case 2: Limpar();
              Menu_Cadastro_produto();
              break;
      default: menuPrincipalScreen();
              break;
    }
  } while(opc == 0);
}

void Menu_lista()
{
    Arq_princ = fopen("./database/ArqProdutoPneus.bin","a+b");///ABERTURA DO ARQUIVO

    estoque pneus;
    int a=1;

    if(Arq_princ)
    {
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(fread(&pneus, sizeof(estoque), 1, Arq_princ))//LENDO OS BLOCOS DO ARQUIVO
            {
                    printf("|                     CATEGORIA DE PNEUS (%d)                   |",a++);
                    printf("\n+==============================================================+\n");
                    printf("| Codigo do produto: %d", pneus.Codigo);
                    printf("\n+==============================================================+\n");
                    printf("| Nome do produto: %s", pneus.Nome);
                    printf("+==============================================================+\n");
                    printf("| Marca do produto: %s", pneus.Marca);
                    printf("+==============================================================+\n");
                    printf("| Preço do produto: R$%.2f", pneus.Preco);
                    printf("\n+==============================================================+\n");
                    printf("| Cor do produto: %s", pneus.Cor);
                    printf("+==============================================================+\n");
                    printf("| Quantidade em estoque: %d", pneus.Num_em_estoque);
                    printf("\n+==============================================================+\n");
                    printf("\n\n");
            }
        }fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    }
      else
    {
        printf("\tErro ao Abrir Arquivo!\n");
        Wait(1);///congelar a tela
        Menu_de_Estoque();
    }

    Arq_princ = fopen("./database/ArqProdutoIluminacao.bin","a+b");///ABERTURA DO ARQUIVO

    estoque iluminacao;
    int b=1;

    if(Arq_princ)
    {
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(fread(&iluminacao, sizeof(estoque), 1, Arq_princ))//LENDO OS BLOCOS DO ARQUIVO
            {
                    printf("|                   CATEGORIA DE ILUMINAÇÃO (%d)                |",b++);
                    printf("\n+==============================================================+\n");
                    printf("| Codigodo produto: %d", iluminacao.Codigo);
                    printf("\n+==============================================================+\n");
                    printf("| Nome do produto: %s", iluminacao.Nome);
                    printf("+==============================================================+\n");
                    printf("| Marca do produto: %s", iluminacao.Marca);
                    printf("+==============================================================+\n");
                    printf("| Preço do produto: R$%.2f", iluminacao.Preco);
                    printf("\n+==============================================================+\n");
                    printf("| Cor do produto: %s", iluminacao.Cor);
                    printf("+==============================================================+\n");
                    printf("| Quantidade em estoque: %d", iluminacao.Num_em_estoque);
                    printf("\n+==============================================================+\n");
                    printf("\n");
            }
        }fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    }
      else
    {
        printf("\tErro ao Abrir Arquivo!\n");
        Wait(1);///congelar a tela
        Menu_de_Estoque();
    }

    char pesquisa;
    printf("\n+==============================================================+\n");
    printf("\nDeseja pesquisar um produto especifico: (S/N)\n");
    printf("\n|--> ");
    scanf(" %c", &pesquisa);
    printf("\n+============================================================+\n");

    switch(pesquisa){
      case 'S':
        Limpar();
        pesquisa_estoque();
        break;

      case 's':
        Limpar();
        pesquisa_estoque();
        break;

      default:
        Limpar();
        Menu_de_Estoque();
        break;
    }

}

void pesquisa_estoque()
{
    estoque pro;

    int opc;
        Limpar();
        printf("+============================================================+");
        printf("\n|         QUAL O TIPO DE PRODUTO ESTA PESQUISANDO            |");
        printf("\n+============================================================+");
        printf("\n[1] - Pneu                                                 ");
        printf("\n[2] - Iluminação                                           ");
        printf("\n\n[0] - Voltar                                             ");
        printf("\n+============================================================+\n");

    printf("Digite uma opção");
    printf("\n|--> ");
    scanf("%d", &opc);

  if(opc == 1)
    Arq_princ = fopen("./database/ArqProdutoPneus.bin","a+b");///ABERTURA DO ARQUIVO
  else if(opc == 2)
    Arq_princ = fopen("./database/ArqProdutoIluminacao.bin","a+b");///ABERTURA DO ARQUIVO
  else
    Menu_de_Estoque();

    int codigo;
    int x;
        printf("\n");
        printf("\n+==============================================================+\n");
        printf("|                     PESQUISA DE PRODUTO                       |");
        printf("\n+==============================================================+\n");
        printf("\nPara pneus, digitar numeros entre 1 e 99");
        printf("\nPara iluminação, digitar numeros entre 100 e 199");
        printf("\nDigite o codigo do produto:");
        fflush(stdin);
        printf("\n|--> ");
        scanf("%d", &codigo);

        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&pro, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
        if(pro.Codigo == codigo)
        {x++;
                Limpar();
                printf("+==============================================================+");
                printf("\n| Codigo do produto: %d", pro.Codigo);
                printf("\n+==============================================================+");
                printf("\n| Nome do produto: %s",pro.Nome);
                printf("+==============================================================+");
                printf("\n| Marca do produto: %s", pro.Marca);
                printf("+==============================================================+");
                printf("\n| Preço do produto: R$%.2f", pro.Preco);
                printf("\n+==============================================================+");
                printf("\n| Cor do produto: %s", pro.Cor);
                printf("+==============================================================+");
                printf("\n| Quantidade em estoque: %d",pro.Num_em_estoque);
                printf("\n+==============================================================+\n");

            getch();
        }
        fread(&pro, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        if(x == 0)
        {
        printf("Produto nao localizado!\n");
        Wait(2);///congelar a tela
        }
    }
    fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
      Limpar();
      printf("+============================================================+");
      printf("\n[1] - Para Ver Lista Completa                                |");
      printf("\n[0] - Para Voltar Ao Menu Do Estoque                         |");
      printf("\n+============================================================+");

    printf("\n\nDigite uma opção\n");
    printf("|--> ");
    scanf("%d", &opc);

    switch (opc)
    {
    case 1:
      Limpar();
      Menu_lista();
      break;
    default:
      Limpar();
      Menu_de_Estoque();
      break;
    }

}

void Menu_Cadastro_produto()
{
    int opc;

        printf("\n\n");
        printf("+============================================================+");
        printf("\n|                    MENU DE CADASTRO                        |");
        printf("\n+============================================================+");
        printf("\n[1] - Novo Cadastro                                          ");
        printf("\n[2] - Alterar Cadastro                                       ");
        printf("\n[3] - Excluir Cadastro                                       ");
        printf("\n\n[0] - Menu principal                                        ");
        printf("\n+============================================================+\n");

    printf("Digite uma opção");
    printf("\n|--> ");
    scanf("%d", &opc);

switch(opc)
{
case 1:
    Limpar();
    estoque_novo();
    break;
case 2:
    Limpar();
    altera_estoque();
    break;
case 3:
    Limpar();
    Excluir_estoque();
    break;
default:
    Limpar();
    Menu_de_Estoque();
    break;
}

}

void estoque_novo()
{
  estoque P;

  int opc;

        printf("+============================================================+");
        printf("\n|         QUAL O TIPO DE PRODUTO SERA CADASTRADO             |");
        printf("\n+============================================================+");
        printf("\n[1] - Pneu                                                 ");
        printf("\n[2] - Iluminação                                           ");
        printf("\n\n[0] - Voltar");
        printf("\n+============================================================+\n");

    printf("Digite uma opção");
    printf("\n|--> ");
    scanf("%d", &opc);

  if(opc == 1)
    Arq_princ = fopen("./database/ArqProdutoPneus.bin","a+b");///ABERTURA DO ARQUIVO
  else if (opc == 2)
    Arq_princ = fopen("./database/ArqProdutoIluminacao.bin","a+b");///ABERTURA DO ARQUIVO
  else{
    Limpar();
    Menu_Cadastro_produto();
  }

  int opcao;

  if(Arq_princ)
  {
    Limpar();
    printf("+==============================================================+\n");
    printf("|                NOVO CADASTRO DE PRODUTOS                     |\n");

    printf("+==============================================================+\n");
    fflush(stdin);
    printf("| Codigo do produto: ");
    scanf("%d", &P.Codigo);
    printf("+==============================================================+\n");
    printf("| Nome do produto: ");
    fflush(stdin);
    fgets(P.Nome, sizeof(P.Nome), stdin);
    printf("+==============================================================+\n");
    printf("| Marca do produto: ");
    fflush(stdin);
    fgets(P.Marca, sizeof(P.Marca), stdin);
    printf("+==============================================================+\n");
    printf("| Preço do produto: ");
    scanf("%f", &P.Preco);
    printf("+==============================================================+\n");
    printf("| Cor do produto: ");
    fflush(stdin);
    fgets(P.Cor, sizeof(P.Cor), stdin);
    printf("+==============================================================+\n");
    printf("| Quantidade em estoque: ");
    fflush(stdin);
    scanf("%d", &P.Num_em_estoque);
    printf("+==============================================================+\n");

    fwrite(&P, sizeof(estoque), 1, Arq_princ);// GRAVANDO NO ARQUIVO
    fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
    getch();
  }
  else
  {
    printf("ERRO AO ABRIR ARQUIVO!");
    Wait(1);///congelar a tela
    Menu_de_Estoque();
  }
    printf("\n");
    printf("+============================================================+\n");
    printf("[1] - Para Cadastrar Novos Produtos                            |\n");
    printf("[0] - Para Voltar Ao Menu Estoque                              |");
    printf("\n+============================================================+");

    printf("\n|--> ");
    scanf("%d", &opcao);
  switch (opcao)
  {
  case 1:
    Limpar();
    estoque_novo();
    break;

  default:
    Limpar();
    Menu_de_Estoque();
    break;
  }
}

void altera_estoque()
{

    int tipo;

        printf("+============================================================+");
        printf("\n|      ESCOLHA UMA DAS SEGUINTES CATEGORIAS DE PRODUTO       |");
        printf("\n+============================================================+");
        printf("\n| [1] - Pneu                                                  |");
        printf("\n| [2] - Iluminação                                            |");
        printf("\n+============================================================+\n");

    printf("Digite uma opção");
    printf("\n|--> ");
    scanf("%d", &tipo);
    switch (tipo)
    {
    case 1:

    Arq_princ = fopen("./database/ArqProdutoPneus.bin", "a+b");///ABERTURA DO ARQUIVO
    Aux = fopen("./database/AUXILIAR.bin", "wb");///ABERTURA DO ARQUIVO AUXILIAR

    estoque alt;
    int codigo;
    int opc;
    int a;

        printf("\n");
        printf("+============================================================+");
        printf("\n|           ALTERAR CADASTRO DE PRODUTOS                     |");
        printf("\n+============================================================+");
        printf("\n[1] - Alterar Codigo do produto                            ");
        printf("\n[2] - Alterar Nome do produto                              ");
        printf("\n[3] - Alterar Marca do produto                             ");
        printf("\n[4] - Alterar Preço do produto                             ");
        printf("\n[5] - Alterar Cor do produto                               ");
        printf("\n[6] - Alterar Quantidade em estoque do Produto             ");
        printf("\n[7] - Alterar Todas As opções                              ");
        printf("\n\n[0] - Menu de Estoque                                      ");
        printf("\n+============================================================+");

        printf("\nDigite uma das opções");
        printf("\n|--> ");
        scanf("%d", &opc);
        Limpar();

    switch(opc)
    {
    case 1:

            printf("\nCodigo do produto a ser alterado");
            printf("\n|--> ");
            scanf("%d", &codigo);

        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt.Codigo == codigo)
            {a++;

                    printf("\n+============================================================+");
                    printf("\n| Novo codigo: ");
                    fflush(stdin);
                    scanf("%d", &alt.Codigo);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
     if(a == 0) printf("\nCadastro não o Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoPneus.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoPneus.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 2:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt.Codigo == codigo)
            {a++;

                    printf("\n+============================================================+");
                    printf("\n| Novo nome do produto: ");
                    fflush(stdin);
                    fgets(alt.Nome, sizeof(alt.Nome), stdin);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(a == 0) printf("\nCadastro não o Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoPneus.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoPneus.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 3:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt.Codigo == codigo)
            {a++;

                    printf("\n+============================================================+");
                    printf("\n| Nova marca do produto: ");
                    fflush(stdin);
                    fgets(alt.Marca, sizeof(alt.Marca), stdin);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(a == 0) printf("\nCadastro não o Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoPneus.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoPneus.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 4:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt.Codigo == codigo)
            {a++;

                    printf("\n+============================================================+");
                    printf("\n| Novo Preço do produto: ");
                    fflush(stdin);
                    scanf("%f",&alt.Preco);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(a == 0) printf("\nCadastro nÃ£o Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoPneus.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoPneus.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 5:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt.Codigo == codigo)
            {a++;

                    printf("\n+============================================================+");
                    printf("\n| Nova cor do produto: ");
                    fflush(stdin);
                    fgets(alt.Cor, sizeof(alt.Cor), stdin);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(a == 0) printf("\nCadastro nÃ£o Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoPneus.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoPneus.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 6:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt.Codigo == codigo)
            {a++;

                    printf("\n+============================================================+");
                    printf("\n| Nova quantidade em estoque do produto: ");
                    fflush(stdin);
                    scanf("%d",&alt.Num_em_estoque);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(a == 0) printf("\nCadastro nÃ£o Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoPneus.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoPneus.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 7:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt.Codigo == codigo)
            {a++;

                    printf("\n+============================================================+");
                    fflush(stdin);
                    printf("\n| Novo codigo: ");
                    scanf("%d", &alt.Codigo);
                    printf("\n+============================================================+");
                    printf("\n| Novo nome do produto: ");
                    fflush(stdin);
                    fgets(alt.Nome, sizeof(alt.Nome), stdin);
                    printf("\n+============================================================+");
                    printf("\n| Nova marca do produto: ");
                    fflush(stdin);
                    fgets(alt.Marca, sizeof(alt.Marca), stdin);
                    printf("\n+============================================================+");
                    printf("\n| Novo Preço do produto: ");
                    scanf("%f",&alt.Preco);
                    printf("\n+============================================================+");
                    printf("\n| Nova cor do produto: ");
                    fflush(stdin);
                    fgets(alt.Cor, sizeof(alt.Cor), stdin);
                    printf("\n+============================================================+");
                    fflush(stdin);
                    printf("\n| Nova quantidade em estoque do produto: ");
                    scanf("%d",&alt.Num_em_estoque);
                    printf("\n+============================================================+\n");
                    fflush(stdin);

            }
            fwrite(&alt, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(a == 0) printf("\nCadastro nÃ£o Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoPneus.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoPneus.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    default:
        Limpar();
        Menu_de_Estoque();
        break;
    }

    Limpar();
    Menu_Cadastro_produto();
    break;

    case 2:

    Arq_princ = fopen("./database/ArqProdutoIluminacao.bin","a+b");///ABERTURA DO ARQUIVO
    Aux = fopen("./database/AUXILIAR.bin","wb");//ABERTURA DE UM ARQUIVO AUXILIAR///ABERTURA DO ARQUIVO AUXILIAR

    estoque alt2;
    int codigo2;
    int opc2, b;

        printf("+============================================================+");
        printf("\n|             ALTERAR CADASTRO DE PRODUTOS                 |");
        printf("\n+============================================================+");
        printf("\n[1] - Alterar Codigo do produto                              ");
        printf("\n[2] - Alterar Nome do produto                                ");
        printf("\n[3] - Alterar Marca do produto                               ");
        printf("\n[4] - Alterar Preco do produto                               ");
        printf("\n[5] - Alterar Cor do produto                                 ");
        printf("\n[6] - Alterar Quantidade em estoque do Produto               ");
        printf("\n[7] - Alterar Todas As opções                               ");
        printf("\n\n[0] - Menu de Estoque                                        ");
        printf("\n+============================================================+");

        printf("\nDigite uma das opções");
        printf("\n|--> ");
        scanf("%d", &opc2);

    switch(opc2)
    {
    case 1:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo2);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt2.Codigo == codigo2)
            {b++;

                    printf("\n+============================================================+");
                    printf("\n| Novo codigo: ");
                    fflush(stdin);
                    scanf("%d", &alt2.Codigo);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt2, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
     if(b == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoIluminacao.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoIluminacao.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 2:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo2);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt2.Codigo == codigo2)
            {b++;

                    printf("\n+============================================================+");
                    printf("\n| Novo nome do produto: ");
                    fflush(stdin);
                    fgets(alt2.Nome, sizeof(alt2.Nome), stdin);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt2, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(b == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoIluminacao.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoIluminacao.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 3:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo2);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt2.Codigo == codigo2)
            {b++;

                    printf("\n+============================================================+");
                    printf("\n| Nova marca do produto: ");
                    fflush(stdin);
                    fgets(alt2.Marca, sizeof(alt2.Marca), stdin);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt2, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(b == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoIluminacao.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoIluminacao.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 4:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo2);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt2.Codigo == codigo2)
            {b++;

                    printf("\n+============================================================+");
                    printf("\n| Novo preço do produto: ");
                    scanf("%f",&alt2.Preco);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt2, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(b == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoIluminacao.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoIluminacao.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 5:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo2);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt2.Codigo == codigo2)
            {b++;

                    printf("\n+============================================================+");
                    printf("\n| Nova cor do produto: ");
                    fflush(stdin);
                    fgets(alt2.Cor, sizeof(alt2.Cor), stdin);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt2, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(b == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoIluminacao.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoIluminacao.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 6:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo2);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt2.Codigo == codigo2)
            {b++;

                    printf("\n+============================================================+");
                    printf("\n| Nova quantidade em estoque do produto: ");
                    scanf("%d",&alt2.Num_em_estoque);
                    printf("\n+============================================================+\n");

            }
            fwrite(&alt2, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(b == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoIluminacao.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoIluminacao.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    case 7:

            printf("\nCodigo do produto a ser alterado:");
            printf("\n|--> ");
            scanf("%d", &codigo2);


        fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
        fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        while(!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
        {
            if(alt2.Codigo == codigo2)
            {b++;

                    printf("\n+============================================================+");
                    fflush(stdin);
                    printf("\n| Novo codigo: ");
                    scanf("%d", &alt2.Codigo);
                    printf("\n+============================================================+");
                    fflush(stdin);
                    printf("\n| Novo nome do produto: ");
                    fgets(alt2.Nome, sizeof(alt2.Nome), stdin);
                    printf("\n+============================================================+");
                    fflush(stdin);
                    printf("\n| Nova marca do produto: ");
                    fgets(alt2.Marca, sizeof(alt2.Marca), stdin);
                    printf("\n+============================================================+");
                    printf("\n| Novo preco do produto: ");
                    scanf("%f",&alt2.Preco);
                    printf("\n+============================================================+");
                    fflush(stdin);
                    printf("\n| Nova cor do produto: ");
                    fgets(alt2.Cor, sizeof(alt2.Cor), stdin);
                    printf("\n+============================================================+");
                    fflush(stdin);
                    printf("\n| Nova quantidade em estoque do produto: ");
                    scanf("%d",&alt2.Num_em_estoque);
                    printf("\n+============================================================+\n");
                    fflush(stdin);

            }
            fwrite(&alt2, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
            fread(&alt2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
        if(b == 0) printf("\nCadastro não Encontrado!");
        else printf("\nCadastro Alterado com Sucesso!");
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoIluminacao.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin", "ArqProdutoIluminacao.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();
        break;

    default:
        Limpar();
        Menu_de_Estoque();
        break;
    }

    Limpar();
    Menu_Cadastro_produto();
    break;
  }
}

void Excluir_estoque()
{

  int tipo;

        Limpar();

        printf("+============================================================+");
        printf("\n|      ESCOLHA UMA DAS SEGUINTES CATEGORIAS DE PRODUTO       |");
        printf("\n+============================================================+");
        printf("\n[1] - Pneu                                                   |");
        printf("\n[2] - Iluminacao                                             |");
        printf("\n+============================================================+\n");

    printf("Digite uma opção");
    printf("\n|--> ");
    scanf("%d", &tipo);
    Limpar();

    switch (tipo)
    {
    case 1:

    Limpar();
    Arq_princ = fopen("./database/ArqProdutoPneus.bin", "a+b");///ABERTURA DO ARQUIVO
    Aux = fopen("./database/AUXILIAR.bin", "wb");///ABERTURA DO ARQUIVO AUXILIAR

    estoque del;
    int codigo;
    int x, opc;

    printf("+============================================================+");
    printf("\n|                   EXCLUIR CADASTRO                         |");
    printf("\n+============================================================+");
    printf("\n| Codigo do produto a ser excluido");
    fflush(stdin);
    printf("\n|--> ");
    scanf("%d",&codigo);

    fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
    fread(&del, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
    while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
    {
      if(del.Codigo == codigo)
        x++;
      else{
        fwrite(&del, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
        fread(&del, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      }
    }

    if(x == 0) printf("\nNumero de Cadastro nÃ£o encotrado!");
    else printf("\nCadastro Excluido com Sucesso!");
    fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
            remove("ArqProdutoPneus.bin");/// Excluindo o arquivo
        fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
            rename("AUXILIAR.bin","ArqProdutoPneus.bin");///Renomendo o arquivo auxiliar
            remove("AUXILIAR.bin");/// Excluindo o arquivo
            getch();

    Limpar();

        printf("+=============================================================+");
        printf("\n[1] - Para Continuar                                          |");
        printf("\n[0] - Para Voltar Ao Menu Estoque                             |");
        printf("\n+=============================================================+");
        printf("\n|--> ");
        scanf("%d", &opc);
        switch(opc)
        {
        case 1:
          Limpar();
          Excluir_estoque();
          break;
        default:
          Limpar();
          Menu_de_Estoque();
          break;
        }
        Limpar();
        break;

    case 2:

      Limpar();
      Arq_princ = fopen("./database/ArqProdutoIluminacao.bin", "a+b");///ABERTURA DO ARQUIVO
      Aux = fopen("./database/AUXILIAR.bin", "wb");///ABERTURA DO ARQUIVO AUXILIAR

      estoque del2;
      int codigo2;
      int y, opc2;

      printf("+============================================================+");
      printf("\n|                   EXCLUIR CADASTRO                         |");
      printf("\n+============================================================+");
      printf("\n| Codigo do produto a ser excluido:");
      fflush(stdin);
      printf("\n|--> ");
      scanf("%d", &codigo2);

      fseek(Arq_princ, SEEK_SET, 1);//Movendo-se dentro do arquivo
      fread(&del2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
      while (!feof(Arq_princ))//INDICADOR DO FINAL DO ARQUIVO
      {
        if(del2.Codigo == codigo2)
          y++;
        else{
          fwrite(&del2, sizeof(estoque), 1, Aux);// GRAVANDO NO ARQUIVO
          fread(&del2, sizeof(estoque), 1, Arq_princ);//LENDO OS BLOCOS DO ARQUIVO
        }
      }

      if(y == 0) printf("\nNumero de Cadastro não encotrado!");
      else printf("\nCadastro Excluido com Sucesso!");
      fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL
              remove("ArqProdutoIluminacao.bin");/// Excluindo o arquivo
          fclose(Aux);//FECHANDO O ARQUIVO AUXILIAR
              rename("AUXILIAR.bin","AArqProdutoIluminacao.bin");///Renomendo o arquivo auxiliar
              remove("AUXILIAR.bin");/// Excluindo o arquivo
              getch();

      Limpar();
          printf("+=============================================================+");
          printf("\n[1] - Para Continuar                                          |");
          printf("\n[0] - Para Voltar Ao Menu Estoque                             |");
          printf("\n+=============================================================+");
          printf("\n|--> ");
          scanf("%d", &opc2);
          switch(opc2)
          {
          case 1:
            Limpar();
            Excluir_estoque();
            break;
          default:
            Limpar();
            Menu_de_Estoque();
            break;
          }
          Limpar();
          break;

    default:

      Limpar();
      Menu_Cadastro_produto();
      break;
  }
}

void financeiroScreen(){
  int option;

  Limpar();
  printf("+=============================================================+\n");
  printf("|                        FINANCEIRO                           |\n");
  printf("+=============================================================+\n");
  printf("[1] - Caixa\n");
  printf("[2] - Relatorio Financeiro\n");
  printf("\n\n[0] - Menu Principal\n");
  printf("+=============================================================+\n");
  do{
    printf("Digite uma das Opções\n");
    printf("|--> ");
    scanf("%d", &option);

    switch (option){
      case 1: Limpar();
              caixasaldo();
              break;
      case 2:Relatorio();
              break;

      default: menuPrincipalScreen();
              break;
    }
  }while(option == 0);
}



void caixasaldo(){

  float valortotal = 0;
  int x = 0;
  servico saldo;
  arq_ordem = fopen("./database/ordemserv.txt","a+b");///ABERTURA DO ARQUIVO
        printf("|                VALORES RECEBIDOS                   |");
  if(arq_ordem){
    while(!feof(arq_ordem)){//INDICADOR DO FINAL DO ARQUIVO
      if(fread(&saldo,sizeof(servico),1,arq_ordem)){//LENDO OS BLOCOS DO ARQUIVO
        x++;
        printf("\n+====================================================+\n");
        printf("| Valor:R$%.2f",saldo.valor);
        printf("\n+====================================================+\n");
         valortotal += saldo.valor;
      }

    }fclose(arq_ordem);
  }else{
    printf("\nErro ao abrir o Arquivo");
    Wait(1);///congelar a tela
    financeiroScreen();
  }
  printf("\n");
  printf("\nValor total Recebido:R$ %.2f\n",valortotal);
  getch();
  financeiroScreen();
}

void Relatorio(){

    Limpar();
  int x = 0;
  servico saldo;
  estoque lista;

  float valorsaida1 = 0;
  float valorsaida2 = 0;
  float valortotal = 0;

  arq_ordem = fopen("./database/Relatorio.csv","w+");///ABERTURA DO ARQUIVO
  FILE *arq_serv = fopen("./database/ordemserv.txt","a+b");
  Arq_princ = fopen("./database/ArqProdutoPneus.bin","a+b");///ABERTURA DO ARQUIVO
  FILE *arq_princ = fopen("./database/ArqProdutoIluminacao.bin","a+b");///ABERTURA DO ARQUIVO

    if(arq_ordem){

          do{
          if(fread(&saldo,sizeof(servico),1,arq_serv) == 1){ //LENDO OS BLOCOS DO ARQUIVO
          fprintf(arq_ordem,"| Valor:R$%.2f\n\n",saldo.valor);
          valortotal += saldo.valor;
          }else{
            break;
          }
        }while(!feof(arq_serv) == 1);
          fclose(arq_serv);

        do{
          if(fread(&lista,sizeof(estoque),1,Arq_princ)){ //LENDO OS BLOCOS DO ARQUIVO
          fprintf(arq_ordem,"| Codigo do produto: %d\n",lista.Codigo);
          fprintf(arq_ordem,"| Nome: %s",lista.Nome);
          fprintf(arq_ordem,"| Preço: %.2f  Quantidade Comprada: %d\n\n",lista.Preco,lista.Num_em_estoque);
          valorsaida1 = lista.Preco * lista.Num_em_estoque;
          }else{
            break;
          }
        }while(!feof(Arq_princ) == 1);
        fclose(Arq_princ);//FECHANDO O ARQUIVO PRINCIPAL

        do{
          if(fread(&lista,sizeof(estoque),1,arq_princ)){ //LENDO OS BLOCOS DO ARQUIVO
          fprintf(arq_ordem,"| Codigo do produto: %d\n",lista.Codigo);
          fprintf(arq_ordem,"| Nome: %s",lista.Nome);
          fprintf(arq_ordem,"| Preço: %.2f  Quantidade Comprada: %d\n\n",lista.Preco,lista.Num_em_estoque);
          valorsaida2 = lista.Preco * lista.Num_em_estoque;
          }else{
            break;
          }
        }while(!feof(arq_princ) == 1);
        fclose(arq_princ);//FECHANDO O ARQUIVO PRINCIPAL

            fprintf(arq_ordem,"| Valor Total de Entrada: %.2f\n",valortotal);
            fprintf(arq_ordem,"| Valor de Saida: %.2f\n",valortotal - (valorsaida1+valorsaida2));
            fclose(arq_ordem);
    }else{
      printf("Erro ao abrir o arquivo!\n");
      Wait(1);//congelar a tela
      financeiroScreen();
    }
    printf("Relatorio criado com Sucesso");
    getch();
    system("start database/Relatorio.csv");
    financeiroScreen();
}
