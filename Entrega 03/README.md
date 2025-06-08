
## **Analise de consumo real da impressora em diferentes casos**

- **Adaptador**
  
Para a an�lise de energia, foi utilizado um adaptador da entrada de tens�o da impressora Ender e conectado o nosso sistema de medi��o, foto do adaptador:

![image](https://github.com/LeoBeche/PI3-2025-1/blob/main/Entrega%2003/Adaptador.jpg)

Esse adptador � feito de maneira que a entrada de tens�o da fonte passe pela circuito medidor de corrente aumentando a tens�o de sa�da dele (isso � importante porque o c�digo feito apenas detecta varia��o positiva de tens�o a partir do offset de zero corrente)
  
---

- **Mesa e Bico Esquentando**

ESBO�O Realizou-se a montagem do circuito em uma placa universal, apresentada na figura abaixo, para verificar o funcionamento do circuito e poder testar juntamente com o ESP32. Para simular a queda de energia utilizou-se um jumper, indicado com o quadrado vermelho.

![image](https://github.com/user-attachments/assets/878af08e-ccc4-40cf-91e7-01609b6b331a)

---

- **Nivelamento**

ESBO�O A imagem abaixo apresenta o resultado com o c�digo *"detecta_queda.c"* ao remover o Jumper do circuito.

![Sem t�tulo](https://github.com/user-attachments/assets/c18f7853-6ea9-4e49-b850-8157645b7cae)

---

- **Impress�o**

ESBO�O A imagem abaixo apresenta o resultado com o c�digo *"detecta_queda.c"* ao remover o Jumper do circuito.

![Sem t�tulo](https://github.com/user-attachments/assets/c18f7853-6ea9-4e49-b850-8157645b7cae)

link: https://youtube.com/shorts/0EHOhPpAPSg?feature=share
---

- **Stand-by**

ESBO�O A imagem abaixo apresenta o resultado com o c�digo *"detecta_queda.c"* ao remover o Jumper do circuito.

![Sem t�tulo](https://github.com/user-attachments/assets/c18f7853-6ea9-4e49-b850-8157645b7cae)
link: https://youtube.com/shorts/RgSNFNDTvaA?feature=share
---

## **Teste de Carga do Conjunto de Baterias**

- **Circuito:**
  
ESBO�O Para a medi��o de consumo de energia, foi utilizado o circuito de medi��o de tens�o e corrente. Verifica-se a tens�o e corrente, � feita a multiplica��o, resultando em pot�ncia, assim, multiplicado pelo tempo que a task utiliza, fazendo com que o resultado seja o consumo de energia.


![image](https://github.com/user-attachments/assets/5106227e-a685-4b29-93f8-e59a52547741)

---


## **Desenho do Case**

- **Circuito:**
  
ESBO�O Para a medi��o de consumo de energia, foi utilizado o circuito de medi��o de tens�o e corrente. Verifica-se a tens�o e corrente, � feita a multiplica��o, resultando em pot�ncia, assim, multiplicado pelo tempo que a task utiliza, fazendo com que o resultado seja o consumo de energia.


![image](https://github.com/user-attachments/assets/5106227e-a685-4b29-93f8-e59a52547741)

---