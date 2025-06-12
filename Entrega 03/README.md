
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

### **Teste de Carga do Banco de Baterias na Impressora 3D**

Para validar o desempenho do banco de baterias, foram utilizados suportes impressos em 3D, que permitiram uma conexão rápida e segura das células ao Sistema de Gerenciamento de Bateria (BMS).

![image](https://github.com/user-attachments/assets/1b2a5c9b-4e5d-4d52-980c-ada7ee057f2a)

**Teste Inicial: Configuração 6S1P**

No primeiro teste, foi montada uma configuração **6S1P** (6 células em série). O banco de baterias conseguiu alimentar com sucesso tarefas de menor demanda, como o aquecimento do bico e a movimentação dos eixos.

Porém, ao tentar aquecer a mesa da impressora, a demanda de corrente inicial foi excessivamente alta, confirmando o que a análise de consumo já havia apontado como o **período crítico do sistema**. Esse pico de corrente provocou uma queda de tensão abrupta na célula com maior resistência interna, fazendo com que o BMS, por segurança, desligasse todo o sistema após cerca de 15 segundos.

![image](https://github.com/user-attachments/assets/f3f601fc-c695-43ed-b47c-0ca1b96e327a)

**Teste Final: Configuração 6S2P**

Para solucionar a limitação de corrente, a configuração foi alterada para **6S2P** (6 células em série, 2 em paralelo), dobrando a capacidade de descarga do conjunto. Com essa nova montagem, a impressora operou em todas as suas funções sem problemas.

O teste prático definitivo foi realizado iniciando uma impressão e, em seguida, desconectando a fonte da rede elétrica. A impressora manteve-se operante com a energia das baterias por **mais de 15 minutos**, um resultado excelente que superou em muito a expectativa inicial do projeto, que era de resistir por pelo menos 1 minuto. Isso valida a eficácia do sistema como uma fonte de energia ininterrupta (No-break).

![image](https://github.com/user-attachments/assets/5412cf63-7a3b-4fc4-ac48-573c77f9e2e6)

https://photos.app.goo.gl/ZgK85YDPrmBBUdR36 (link para o video de teste)

**Projeto do Compartimento Final das Baterias**

Após o sucesso dos testes, foi projetado o compartimento definitivo que irá alocar todas as baterias de forma segura e organizada. O design consiste em um suporte modular: serão utilizados dois slots 3S2P idênticos que, quando associados em série, formarão o banco de baterias 6S2P final para a montagem dentro da case.

![image](https://github.com/user-attachments/assets/f3310ff4-c8e1-4a45-b330-47a2805cf3f0)

---

### **Projeto 3D da Case do Equipamento**

Para abrigar o **conjunto de baterias** e o **circuito de medição**, foi desenvolvido um projeto de case customizada. O processo iniciou com a tomada de medidas da parte traseira da impressora 3D, o que permitiu elaborar um design que se acopla de forma precisa e funcional ao equipamento.

![image](https://github.com/user-attachments/assets/afd3850e-05d8-490f-a7b4-fd1cd03f79de)

Considerando as dimensões do banco de baterias e das placas eletrônicas, foi possível elaborar um projeto de case robusto. O design visa não apenas proteger os componentes, mas também garantir durabilidade e praticidade, com uma estrutura firme e entradas e saídas de cabos bem definidas.

![image](https://github.com/user-attachments/assets/ca3ad015-c0cc-469f-a98d-0f863b47c642)

A case consiste em uma caixa com tampa parafusada para fácil acesso e manutenção. Possui aberturas para a passagem dos cabos que conectam o sistema à fonte de alimentação e à impressora. Além disso, conta com dois orifícios projetados para a instalação de interruptores de energia, garantindo o controle manual do sistema. O espaço interno foi otimizado para acomodar de forma segura e organizada o banco de baterias junto ao circuito de medição.

![image](https://github.com/user-attachments/assets/9d87047d-0cac-4c81-90de-278f5fdd2f7b)
