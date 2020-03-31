# Sistemas Embarcados - APS1

## Descrição
Projeto com o intuito de criar um sistema embarcado que toca músicas monofônicas.

## Video de demonstração
link para o vídeo de demonstração: 

## Materiais
- `1x Buzzer 5V`
- `2x Jumpers Fêmea-Fêmea`
- `1x Atmel SAME70 Xplained`
- `1x OLED1 Xplained`

## Instruções
1. Conectar os componentes, como explicado na parte das [Conexões](#Conexões) abaixo
2. Conectar a placa no computador
3. Executar os códigos no Atmel Studio
- Botões:
  - `Button 1` da placa `OLED1 Xplained` toca a música.
  - `Button 2` da placa `OLED1 Xplained` pausa a música.
  - `Button 3` da placa `OLED1 Xplained` escolhe a música.
- LED's:
  - `LED` da placa `Atmel SAME70 Xplained` pisca junto com a música que está sendo tocada.
  - `Display` da placa `OLED1 Xplained` indica a música que está tocando.

## Conexões
O `OLED1 Xplained` deve ser conectado à entrada `EXT1` da placa `Atmel SAME70 Xplained`, como na imagem a seguir:

![OLED1 conectado à placa Atmel SAME70 Xplained](./placa-com-oled1.jpeg "OLED1 conectado à placa Atmel SAME70 Xplained")


![OLED1 e Buzzer conectados à placa Atmel SAME70 Xplained](./placa-com-oled1-buzzer.jpeg "OLED1 e Buzzer conectados à placa Atmel SAME70 Xplained")

## Arquivos do projeto



## Implementação segundo a [rúbrica](https://insper.github.io/ComputacaoEmbarcada/APS-1-Musical/)
- A (1 item embarcado + 1 item C)
  - Embarcado
    - [ ] Utiliza dois falantes (para fazer uma música mais complexa);
    - [x] Utiliza interrupção nos botões;
  - C
    - [] Cria um arquivo .c .h com as funções de reprodução musical;
    - [x] Músicas organizadas em vetores de structs (ponteiros);

- B+
  - [x] Uma terceira música

- B
  - [x] [README.md](/README.md) explica o que é o projeto e como o software foi estruturado;
  - [x] Música separada em .h;
  - [x] Utiliza struct para organizar as músicas;
  - [x] Código organizado em funções e que recebem struct contendo música;
  - [x] Utiliza #define sempre que necessário;
  - [ ] Fecha todos os issues que forem abertos no repositório (pelo professor);

- C
  - [x] Repositório contém [README.md](/README.md) com ligações elétrica e passos de como usar o projeto, assim como link para o vídeo;
  - [x] Funcionalidade de PAUSE/ PLAY;
  - [x] Botão de seleção musical;
  - [x] Reproduz duas músicas (de forma inteligível);
  - [x] LED piscando de acordo com a música;
  - [x] Indicação visual de qual música foi selecionada;

- D
  - [ ] Faltando um item de C

- I
  - [ ] Mais que dois itens de C faltando
