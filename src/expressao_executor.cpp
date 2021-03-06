//Implentação dos métodos da classe expressao_validador
#include "expressao_executor.h"
#include  <cmath>

//Construtor Padrão
ExpressaoExecutor:: ExpressaoExecutor(){}

//Destrutor Padrão
ExpressaoExecutor:: ~ExpressaoExecutor(){}

// Receberá a expressão e chamará os métodos de conversão e execução da expressão em sua forma pós-fixa
double ExpressaoExecutor::resultado(string _expressao)
{
    expressao = _expressao;

    converterPosFixa();

    return executarPosFixa();
}

// Private

int ExpressaoExecutor::operPriori(string operador)
{
  if (operador == "(" || operador == ")") return 4;
  else if (operador == "^") return 3;
  else if (operador == "*" || operador == "/") return 2;
  else return 1;
}

// Método de converção da expressão para sua forma pós-fixa
// O for itera a expressão inteira
// Verifica primeiro se é um número, caso seja adiciona no buffer
// Caso não seja um número o buffer é adicionado na lista que representa a expressão pos-fixada
// Em seguida, verifica se é um parênteses, caso seja de abertura é colocado na pilha
// Caso o parenteses seja de fechamento são desempilhados todos os valores até o parênteses de abertura
// Caso seja um operador é verificado sua prioridade e empilhado
// Caso seja espaço vazio nenhuma ação é tomada
// Por fim, caso exista algo na pilha ela é desempilhada para a lista
void ExpressaoExecutor::converterPosFixa()
{
  pilha<string> operadores;
  string buffer = "";
  string oper_buffer = "";

  for(auto it = expressao.begin(); it != expressao.end(); ++it)
  {
    if (isdigit(*it) || *it == '.')
      buffer += *it;
    else
    {
      if (buffer != "") elementos.insere(buffer);
        buffer = "";
      
      oper_buffer = *it;

      if (*it == '(')
        operadores.insere(oper_buffer);
      
      else if (*it == ')')
      {
        while (operadores.topo() != "(")
        {
          elementos.insere(operadores.topo());
          operadores.tira();
        }

        operadores.tira(); // Remover o "(" da pilha
      }
      else
      {
        if (*it != ' ')
        {
          if (!operadores.vazia())
          {
            while (!operadores.vazia() && operadores.topo() != "(" && operPriori(operadores.topo()) >= operPriori(oper_buffer))
            {
              elementos.insere(operadores.topo());
              operadores.tira();
            }
          }

          operadores.insere(oper_buffer);
        }
      }
    }
  }

  if (buffer != "") elementos.insere(buffer);

  while(!operadores.vazia())
  {
    elementos.insere(operadores.topo());
    operadores.tira();
  }
}

// Verifica se uma string passada como argumento é um número
bool ExpressaoExecutor::ehNuemro(std::string num){
  try
    {
        std::stod(num);
    }
    catch(...)
    {
        return false;
    }
    return true;
}

// Verifica se uma string passada como argumento é um operador 
bool ExpressaoExecutor::ehOperador(std::string  op){
  return std::string("+-^/*").find(op[0]) != std::string::npos;
}

// Calcular uma operação binário passada com argumento, e dois valores para serem operados 
double ExpressaoExecutor::calcOperacao(std::string op, double valor_1, double valor_2){
  double resultado = 0;
  switch(op[0]){
    case '^' : 
      resultado = pow(valor_1, valor_2);
      break;
    case '*' : 
      resultado =  valor_1 * valor_2;
      break;
    case '/' :  
      if(valor_2 == 0){
        std::cerr << "Divisão por zero" << std::endl;
        exit(1);
      }
      resultado = valor_1/valor_2;
      break;

    case '+' : 
      resultado = valor_1 + valor_2;
      break;
    case '-' : 
      resultado =  valor_1 - valor_2;
      break;
  }
  
  return resultado;
}

// Método de execução da expressão em sua forma pós-fixa
double ExpressaoExecutor::executarPosFixa()
{
  minhasTads::pilha<double> pilha; 
  int n = elementos.tamanho();
  for(int i = 0; i < n ; i++){ // percorrendo toda a lista 

    if(ehNuemro(elementos.frente())){ // será verdade quando um ítem da lista for um número 
      double num = std::stod(elementos.frente()); 
      pilha.insere(num);
      elementos.tira();
      
    }else if(ehOperador(elementos.frente())){ // será verdade quando um ítem da lista for uma operação 
      double valor1 = pilha.topo(); // valor1 é o último elemento da pilha
      pilha.tira(); // remove o topo da pilha 
      double valor2 = pilha.topo(); // valor2 é agora é o novo último elemento da pilha
      pilha.tira();// remove o topo da pilha 
      pilha.insere(calcOperacao( elementos.frente(), valor2, valor1  ));
      elementos.tira();
    }
  }
  return pilha.topo();
}
