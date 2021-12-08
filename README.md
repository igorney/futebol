<h1 align="center"> ⚽ FUTEBOL_v2 ⚽</h1>

<h4 align="center"> 	
	Universidade Federal do ABC
</h4>
<h4 align="center"> 	
	Projeto da disciplina <b>Computação Gráfica</b>.
</h4>

--- 

## Descrição do Projeto
<p>Este projeto é um aperfeiçoamento do projeto 3D entregue como atividade 2 na disciplina de Computação gráfica. Para este, ainda utilizamos a biblioteca ABCg criada para o curso e as primitivas que são disponibilizadas pela biblioteca OpenGL. Trata-se de um cenário correspondente ao inicio do jogo, onde, na vida real, os jogadores se posicionam para cantar o hino. No centro, temos o juiz. De um lado, temos o time amarelo e do outro lado temos o time azul. A bola está localizada na frente do juiz. Também temos árvores ao redor dos times. É possível interagir com o cénario. E, como aperfeiçoamento, os objetos foram construidos utilizando texturas e iluminações que alteram positivamente o cénario. </p>

--- 

## Construção do Projeto 
<p> O projeto está organizado nas seguintes classes: 
<li> openglwindow: classe que fará a chamada das funções membros das outras classes. </li>
<li> model: classe que é utilizada para a construção de todos os modelos.</li>
<li> ground: classe que é utilizada para a construção do chão do cenário.</li> 
<li> camera: classe que é utilizada para a definição da câmera.</li>

No arquivo <b>CMakeLists.txt</b> declara-se o nome do projeto e os executaveis (<b>.cpp</b>).

As classe <b>openglwindow.cpp</b> é onde ocorre a chamada para as funções membros que encontramos nas outras classes: initializeGL, paintGL e terminateGL. Também nesta classe foi definido funções para construção do chão, jogadores, árvores, juiz, bola e avião, separadas de acordo com o shader utilizado. 

Na função <b>initializeGL</b> da classe <b>openglwindow.cpp</b> é onde é chamado o carregamento dos modelos.

Na função <b>paintGL</b> da classe <b>openglwindow.cpp</b> é onde todos os objetos são inseridos no cénario. 

Nas classes <b>.hpp</b> é onde ocorre a declaração das variáveis e atributos. 

A classe <b>main</b> é a responsável pela inicialização da aplicação. Também é nela onde incluimos os cabeçavelhos das bibliotecas que permite que utilizemos suas classes e funções e onde configuramos a janela da aplicação.

As classes <b>groud</b> são responsáveis pela definição dos métodos e atributos que são necessários para a construção do chão no cenário. Estes métodos são chamados na classe <b>openglwindow.cpp</b>.

As classes <b>model</b> são responsáveis pela definição dos métodos e atributos que são necessários para a construção dos objetos que são inseridos no cenário. Estes métodos são chamados na classe <b>openglwindow.cpp</b>.

As classes <b>camera</b> são responsáveis pela definição dos métodos e atributos que são necessários para a inicialização da câmera. Estes métodos são chamados na classe <b>openglwindow.cpp</b>.

Na pasta <b>assets</b>, temos os objetos (.obj) que são carregados no cenário, bem como seus arquivos .mtl e imagens, quando utilizadas. Estes arquivos estão organizadas em subpastas de acordo com o objeto desejado. Também nessa pasta, temos os shaders que são utilizados na construção do cénario: lookat.frag e lookat.vert, texture.frag e texture.vert; e, phong.frag e phong.vert. E, por último, a subpasta 'sons' onde encontramos o arquivo de aúdio referente ao hino que é reproduzido no cénario. 

---

## Como interagir:

Utilize o teclado para mover-se no cénario:
<li>Tecla 'w' ou seta para cima: mover para frente. </li>
<li>Tecla 'a' ou seta esquerda: mover para esquerda. </li>
<li>Tecla 'd' ou seta direita: mover para direita.</li>
<li>Tecla 's' ou seta para baixo ou tecla espaço: mover para trás.

Além disso, você pode alterar os parâmetros que são apresentados nos combo box da lateral superior esquerda:
- Alterar o front face: escolha entre CCW e CW para notar as diferenças no cenário.
- Alterar o UV mapping: escolha entre triplanar, cylindrical, spherical e from mesh para notar as diferenças no cenário. A alteração será observada no juiz, na árvore, na bola e no avião.

Você também pode alterar os parâmetros de iluminação (Ia, Id e Is) para observar os efeitos na cena - lateral inferior esquerda.

E, por último, alternar para tela cheia, no botão "toggle fullscreen" na lateral inferior direita.

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



