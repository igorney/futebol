<h1 align="center"> ⚽ FUTEBOL ⚽</h1>

<h4 align="center"> 	
	Universidade Federal do ABC
</h4>
<h4 align="center"> 	
	Projeto da disciplina <b>Computação Gráfica</b>.
</h4>

--- 

## Descrição do Projeto
<p>Construção de um projeto 3D utilizando a biblioteca ABCg criada para o curso de Computação Gráfica  e as primitivas que são disponibilizadas pela biblioteca OpenGL. Trata-se de um cenário correspondente ao inicio do jogo, onde, na vida real, os jogadores se posicionam para cantar o hino. No centro, temos o juiz. De um lado, temos o time branco e do outro lado temos o time azul. A bola está localizada na frente do juiz. Também temos árvores ao redor dos times. </p>

--- 

## Construção do Projeto 
<p> O projeto está organizado nas seguintes classes: 
<li> openglwindow: classe que fará a chamada das funções membros das outras classes. </li>
<li> model: classe que é utilizada para a construção de todos os modelos.</li>
<li> ground: classe que é utilizada para a construção do chão do cenário.</li> 
<li> camera: classe que é utilizada para a definição da câmera.</li>

No arquivo <b>CMakeLists.txt</b> declara-se o nome do projeto e os executaveis (<b>.cpp</b>).

As classe <b>openglwindow.cpp</b> é onde ocorre a chamada para as funções membros que encontramos nas outras classes: initializeGL, paintGL e terminateGL. 

Na função <b>initializeGL</b> da classe <b>openglwindow.cpp</b> é onde é chamado o carregamento dos modelos.

Na função <b>paintGL</b> da classe <b>openglwindow.cpp</b> é onde todos os objetos são inseridos no cénario. 

Nas classes <b>.hpp</b> é onde ocorre a declaração das variáveis e atributos. 

A classe <b>main</b> é a responsável pela inicialização da aplicação. Também é nela onde incluimos os cabeçavelhos das bibliotecas que permite que utilizemos suas classes e funções e onde configuramos a janela da aplicação.

As classes <b>groud</b> são responsáveis pela definição dos métodos e atributos que são necessários para a construção do chão no cenário. Estes métodos são chamados na classe <b>openglwindow.cpp</b>.

As classes <b>model</b> são responsáveis pela definição dos métodos e atributos que são necessários para a construção dos objetos que são inseridos no cenário. Estes métodos são chamados na classe <b>openglwindow.cpp</b>.

As classes <b>camera</b> são responsáveis pela definição dos métodos e atributos que são necessários para a inicialização da câmera. Estes métodos são chamados na classe <b>openglwindow.cpp</b>.

Na pasta <b>assets</b>, temos os objetos (.obj) que são carregados no cenário. Além dos arquivos, lookat.frag e lookat.vert.

---

## Como interagir:

Utilize o teclado para mover-se no cénario:
<li>Tecla 'w' ou seta para cima: mover para frente. </li>
<li>Tecla 'a' ou seta esquerda: mover para esquerda. </li>
<li>Tecla 'd' ou seta direita: mover para direita.</li>
<li>Tecla 's' ou seta para baixo ou tecla espaço: mover para trás.

---

## Autores

<img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/48994130?v=4" width="100px;" alt=""/>
 <br />
 <b>Igor Santos Borges de Alencar RA: 11201811861 </b> 

---

 <img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/63355502?s=400&u=96d53188071a061d643b78620ba76d09c2e3bfb9&v=4" width="100px;" alt=""/>
 <br />
 <b>Jacqueline Marinho RA: 11201812013</b> 
 
 ---
 

### Professores

<i>Bruno Augusto Dorta Marques \
Harlen Costa Batagelo </i> 
</p>



