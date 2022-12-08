#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

void Wait(int Seconds);
void textColor(int letras, int fundo);
void linha_superior();
void linha_inferior();
void item(int i, char str[]);
void gotoxy(int x , int y);

//LOGIN
void Menu_Login();
void Login_Entrar();
void Login_Cadastrar();
void Verificar_Cadastro(char txt[100]);
int Buscar_Usuario(int modo, char usuario[50], char senha[30]);
int Adicionar_Usuario(char usuario[50], char senha[30]);
int Msg_Pergunta(char txt[200]);

//TELAS DE MENU
void menuPrincipalScreen();
void clientesScreen();

//FUNÇÕES DE CADASTRO
void cadastroScreen();
void cadastro_de_cliente();
void listarclientes();
void Excluirclientes();
void alterar_cadastro();
void pesquisarclientes();

//FUNÇÕES DOS FORNECEDORES
void fornecedoresScreen();
void cadastro_de_Fornecedores();
void alterar_Fornecedor();
void Excluir_Fornecedor();
void listafornecedores();
void pesquisafornecedor();

//FUNÇOES DOS PRODUTOS
void produtosScreen();
void cadastro_de_produtos();
void altera_produto();
void lista_produto();
void pesquisa_produto();
void Excluir_produto();

//FUNÇAO DE VEICULOS
void veiculosScreen();
void cadastro_de_Veiculos();
void altera_Veiculo();
void Lista_Veiculos();
void pesquisa_Veiculo();
void Excluir_Veiculo();

//FUNÇÕES DA OFICINA
void oficinasScreen();
void cadastro_de_Oficinas();
void alterar_oficinas();
void listaoficina();
void pesquisa_Oficinas();
void Excluir_oficinas();

///FUNÇÕES DE USUARIO
void usuarioScreen();
void alterarUsuario();
void listausuario();
void pesquisausuario();

/// FUNÇÕES DO ESTOQUE
void Menu_de_Estoque();
void Menu_lista();
void pesquisa_estoque();
void Menu_Cadastro_produto();
void estoque_novo();
void altera_estoque();
void Excluir_estoque();

void atendimentoScreen();
void listavalor();
void pesquisarvalor();

//PLACA
void listarplaca();
void pesquisarplaca();

/// FUNÇOES DE ORDEM DE SERVIÇO
void ordemScreen();
void novaordem();
void pesquisarordem();
void excluirordem();

//FINANCEIRO
void financeiroScreen();
void caixasaldo();
void Relatorio();

#endif
