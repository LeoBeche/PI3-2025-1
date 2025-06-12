
## **Analise de consumo real da impressora em diferentes casos**

- **Adaptador**
  
Para a análise de energia, foi utilizado um adaptador da entrada de tensão da impressora Ender e conectado o nosso sistema de medição, foto do adaptador:

![image](https://github.com/LeoBeche/PI3-2025-1/blob/main/Entrega%2003/Adaptador.jpg)

Esse adptador é feito de maneira que a entrada de tensão da fonte passe pela circuito medidor de corrente aumentando a tensão de saída dele (isso é importante porque o código feito apenas detecta variação positiva de tensão a partir do offset de zero corrente)
  
---
As imagens asseguir mostram a média da corrente, tensão, potência de um período de 16 milissegundos e a energia consumida, com mensagens de período 1 segundo.
---

- **Mesa e Bico Esquentando**

![image][https://github.com/user-attachments/assets/878af08e-ccc4-40cf-91e7-01609b6b331a](https://github.com/LeoBeche/PI3-2025-1/blob/main/Entrega%2003/esquenta_bico_e_mesa_corrente.png)

Medição da Mesa e Bico Esquentando: https://youtube.com/shorts/ABSbG42qyyc

Com a medição foi percebido que esse é um período crítico de corrente no sistema, pois é o momento de maior consumo. A primeira mensagem do monitor serial foi feita momentos antes do bico e a mesa terem sido completamente esquentadas o que se evidencia pelas medições seguintes onde a impressora apenas mantém sua temperatura.

---

- **Nivelamento**

![Sem título](https://github.com/user-attachments/assets/c18f7853-6ea9-4e49-b850-8157645b7cae)

Medição Nivelamento: https://youtube.com/shorts/ZBV85Ab9q4k

Pode-se notar que esse é um período em que o consumo da impressora é baixo, pois só atua o seu sistema de comando e os motores, ficando com uma corrente média em torno de 500 miliampères e 2 ampères, o que está dentro do esperado pois cada motor da impressora tem um consumo nominal de 800 miliampères, o que se encaixa com a medida pois o sistema se move, para e volta a se mover, deixando essa característica no consumo. 

---

- **Impressão**

![Sem título](https://github.com/user-attachments/assets/c18f7853-6ea9-4e49-b850-8157645b7cae)

Medição Impressão: https://youtube.com/shorts/0EHOhPpAPSg?feature=share

Nesse caso o consumo flutua bastante, o que é esperado por causa da dinâmica do sistema.

---

- **Stand-by**

![Sem título](https://github.com/user-attachments/assets/c18f7853-6ea9-4e49-b850-8157645b7cae)

Medição Stand-by: https://youtube.com/shorts/RgSNFNDTvaA?feature=share

O estado dormente da impressora é bem caracterizado e assim é possível garantir que o sistema perceba essa mudança de comportamento e desligue conforme o esperado. 
Além disso, o consumo medido ficou dentro do esperado, como essa impressão

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

### **Projeto 3D da Case do Equipamento**

Para abrigar o **conjunto de baterias** e o **circuito de medição**, foi desenvolvido um projeto de case customizada. O processo iniciou com a tomada de medidas da parte traseira da impressora 3D, o que permitiu elaborar um design que se acopla de forma precisa e funcional ao equipamento.

![image](https://github.com/user-attachments/assets/afd3850e-05d8-490f-a7b4-fd1cd03f79de)

Considerando as dimensões do banco de baterias e das placas eletrônicas, foi possível elaborar um projeto de case robusto. O design visa não apenas proteger os componentes, mas também garantir durabilidade e praticidade, com uma estrutura firme e entradas e saídas de cabos bem definidas.

![image](https://github.com/user-attachments/assets/ca3ad015-c0cc-469f-a98d-0f863b47c642)

A case consiste em uma caixa com tampa parafusada para fácil acesso e manutenção. Possui aberturas para a passagem dos cabos que conectam o sistema à fonte de alimentação e à impressora. Além disso, conta com dois orifícios projetados para a instalação de interruptores de energia, garantindo o controle manual do sistema. O espaço interno foi otimizado para acomodar de forma segura e organizada o banco de baterias junto ao circuito de medição.

![image](https://github.com/user-attachments/assets/9d87047d-0cac-4c81-90de-278f5fdd2f7b)
