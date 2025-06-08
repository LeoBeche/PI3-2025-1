
## **Analise de consumo real da impressora em diferentes casos**

- **Adaptador**
  
Para a análise de energia, foi utilizado um adaptador da entrada de tensão da impressora Ender e conectado o nosso sistema de medição, foto do adaptador:

![image](https://github.com/LeoBeche/PI3-2025-1/blob/main/Entrega%2003/Adaptador.jpg)

Esse adptador é feito de maneira que a entrada de tensão da fonte passe pela circuito medidor de corrente aumentando a tensão de saída dele (isso é importante porque o código feito apenas detecta variação positiva de tensão a partir do offset de zero corrente)
  
---

- **Mesa e Bico Esquentando**

ESBOÇO Realizou-se a montagem do circuito em uma placa universal, apresentada na figura abaixo, para verificar o funcionamento do circuito e poder testar juntamente com o ESP32. Para simular a queda de energia utilizou-se um jumper, indicado com o quadrado vermelho.

![image](https://github.com/user-attachments/assets/878af08e-ccc4-40cf-91e7-01609b6b331a)

---

- **Nivelamento**

ESBOÇO A imagem abaixo apresenta o resultado com o código *"detecta_queda.c"* ao remover o Jumper do circuito.

![Sem título](https://github.com/user-attachments/assets/c18f7853-6ea9-4e49-b850-8157645b7cae)

---

- **Impressão**

ESBOÇO A imagem abaixo apresenta o resultado com o código *"detecta_queda.c"* ao remover o Jumper do circuito.

![Sem título](https://github.com/user-attachments/assets/c18f7853-6ea9-4e49-b850-8157645b7cae)

link: https://youtube.com/shorts/0EHOhPpAPSg?feature=share
---

- **Stand-by**

ESBOÇO A imagem abaixo apresenta o resultado com o código *"detecta_queda.c"* ao remover o Jumper do circuito.

![Sem título](https://github.com/user-attachments/assets/c18f7853-6ea9-4e49-b850-8157645b7cae)
link: https://youtube.com/shorts/RgSNFNDTvaA?feature=share
---

## **Teste de Carga do Conjunto de Baterias**

- **Circuito:**
  
ESBOÇO Para a medição de consumo de energia, foi utilizado o circuito de medição de tensão e corrente. Verifica-se a tensão e corrente, é feita a multiplicação, resultando em potência, assim, multiplicado pelo tempo que a task utiliza, fazendo com que o resultado seja o consumo de energia.


![image](https://github.com/user-attachments/assets/5106227e-a685-4b29-93f8-e59a52547741)

---


## **Desenho do Case**

- **Circuito:**
  
ESBOÇO Para a medição de consumo de energia, foi utilizado o circuito de medição de tensão e corrente. Verifica-se a tensão e corrente, é feita a multiplicação, resultando em potência, assim, multiplicado pelo tempo que a task utiliza, fazendo com que o resultado seja o consumo de energia.


![image](https://github.com/user-attachments/assets/5106227e-a685-4b29-93f8-e59a52547741)

---