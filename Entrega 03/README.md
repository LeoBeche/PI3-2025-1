
## **Análise do consumo real da impressora em diferentes casos**

- **Adaptador**
  
<p align="justify"> Para obter os valores de tensão e corrente, possibilitando a análise do consumo da impressora, foi necessário desenvolver um adaptador, mostrado na imagem abaixo, que alimente a impressora e forneça saídas para que o nosso sistema possa realizar a medição de corrente e tensão. </p>

<p align="center"><b>FIGURA 01 - Adaptador</b></p>

![image](https://github.com/LeoBeche/PI3-2025-1/blob/main/Entrega%2003/Adaptador.jpg)

<p align="justify">Esse adptador foi desenvolvido de maneira que a entrada de tensão da fonte passe pelo circuito medidor de corrente, no sentido positivo de medição dele (isso é importante, porque o nosso código apenas detecta variação positiva de tensão sobre o offset de 2.5V, sendo que esse valor representa corrente nula). </p>
  
---

<p align="justify">A seguir, apresenta-se a média da corrente, tensão e potência, obtidas a partir de 16 amostras com período de 1 ms, e energia consumida pela impressora, com base no valor da potência média e no tempo de 16ms (tempo necessário para obter as 16 amostras), em diferentes modos de operação da impressora. </p>

### **Modo de operação 1: Mesa e Bico Esquentando**

```
Raw: 3161    Voltage: 843mV
Corrente: 10.041107 A
Tensão de entrada: 23.749924 V
Pontência instantânea: 238.475 W
Energia: 40613.877 J

Raw: 2199    Voltage: 609mV
Corrente: 0.210065 A
Tensão de entrada: 23.948787 V
Pontência instantânea: 5.030 W
Energia: 40618.907 J

Raw: 2199    Voltage: 609mV
Corrente: 0.210065 A
Tensão de entrada: 23.948787 V
Pontência instantânea: 5.030 W
Energia: 40623.937 J

Raw: 2198    Voltage: 609mV
Corrente: 0.210065 A
Tensão de entrada: 23.977196 V
Pontência instantânea: 5.036 W
Energia: 40628.973 J
``` 
<p align="justify">Com base no resultado da medição, para essa situação, percebe-se que o aquecimento da mesa e do bico representa um momento crítico de consumo, pois a corrente no sistema atinge, aproximadamente, 10 A, como pode-se observar na primeira medida apresentada (RAW 3161), realizada momentos antes do bico e da mesa terem sido completamente esquentados. Já nas outras medições, nota-se que o valor da corrente caiu drasticamente, sendo um consumo apenas para manter o bico e a mesa aquecidos. </p>

:clapper: Vídeo da medição do consumo da mesa e bico esquentando: https://youtube.com/shorts/ABSbG42qyyc


---

### **Modo de operação 2: Mapeamento de superfície**

```
Raw: 2254    Voltage: 622mV
Corrente: 0.756234 A
Tensão de entrada: 23.948787 V
Pontência instantânea: 18.110 W
Energia: 49133.514 J

Raw: 2252    Voltage: 622mV
Corrente: 0.756234 A
Tensão de entrada: 23.948787 V
Pontência instantânea: 18.110 W
Energia: 49151.624 J

Raw: 2409    Voltage: 660mV
Corrente: 2.352728 A
Tensão de entrada: 23.948787 V
Pontência instantânea: 56.344 W
Energia: 49207.968 J

Raw: 2420    Voltage: 663mV
Corrente: 2.478767 uA
Tensão de entrada: 23.863560 V
Pontência instantânea: 59.152 W
Energia: 49267.120 J
```
<p align="justify">Pode-se notar que, nesse modo de operação, o consumo da impressora é baixo, pois só atua o seu sistema de comando e os motores. A corrente média é, em torno de, 500 mA a 2 A, o que está dentro do esperado, pois cada motor da impressora tem um consumo nominal de 800 mA e o sistema se move, para e volta a se mover, o que justifica essa característica de consumo.</p>

:clapper: Vídeo da medição do consumo no nivelamento: https://youtube.com/shorts/ZBV85Ab9q4k

---

### **Modo de operação 3: Impressão**

```
Corrente: 10.923380 A
Tensão de entrada: 23.778333 V
Potência instantânea: 259.739000 W
Energia: 73.822919 Wh

Corrente: 1.134351 A
Tensão de entrada: 23.920378 V
Potência instantânea: 27.134000 W
Energia: 73.830456 Wh

Corrente: 1.134351 A
Tensão de entrada: 24.034014 V
Potência instantânea: 27.263000 W
Energia: 73.838029 Wh

Corrente: 2.730845 A
Tensão de entrada: 23.948787 V
Potência instantânea: 65.400000 W
Energia: 73.856196 Wh

Corrente: 2.604806 A
Tensão de entrada: 24.005605 V
Potência instantânea: 62.529000 W
Energia: 73.873565 Wh

Corrente: 6.427989 A
Tensao de entrada: 24.005605 V
Potência instantânea: 154.307000 W
Energia: 73.916428 Wh

Corrente: 10.965393 A
Tensão de entrada: 23.891969 V
Pontência instantânea: 261.984000 W
Energia: 73.989201 Wh

Corrente: 1.050325 A
Tensão de entrada: 23.806742 V
Potência instantânea: 25.004000 W
Energia: 73.996147 Wh

Corrente: 1.134351 A
Fensão de entrada: 23.948787 V
Potência instantânea: 27.166000 W
Energia: 74.003693 Wh

Corrente: 1.092338 A
tensão de entrada: 24.005605 V
Potência instantânea: 26.222000 W
Energia: 74.010977 Wh

Corrente: 2.646819 A
Tensão de entrada: 23.891969 V
Pontência instantânea: 63.237888 W
Energia: 74.028543 Wh
```

Na impressão, o consumo varia bastante, o que é esperado, por causa da dinâmica do sistema.

:clapper: Vídeo da medição do consumo na impressão: https://youtube.com/shorts/0EHOhPpAPSg?feature=share

---

### **Modo de operação 4: Stand-by**

```
Corrente: 0.546169 A
Tensão de entrada: 23.977196 V
Potência instantânea: 13.095000 W
Energia: 84.886653 Wh

Corrente: 0.882273 A
Tensao de entrada: 24.034014 V
Potência instantânea: 21.204000 W
Energia: 84.892543 Wh

Corrente: 0.882273 A
Tensão de entrada: 24.090832 V
Potência instantânea: 21.254000 W
Energia: 84.898447 Wh

Corrente: 0.882273 A
Tensao de entrada: 24.034014 V
Potência instantânea: 21.204000 W
Energia: 84.904337 Wh

Corrente: 0.882273 A
Tensão de entrada: 23.778333 V
Potência instantânea: 20.978080 W
Energia: 84.918164 Wh

Corrente: 0.168052 A
Tensão de entrada: 23.920378 V
Potência instantânea: 4.019000 W
Energia: 84.911281 Wh

Corrente: 0.168052 A
Tensão de entrada: 23.891969 V
Potência instantânea: 4.815000 W
Energia: 84.912396 Wh

Corrente: 0.168052 A
Tensão de entrada: 23.977196 V
Potência instantânea: 4.829000 W
Energia: 84.913515 Wh

Corrente: 0.210065 A
Tensão de entrada: 24.090832 V
Potência instantânea: 5.060000 W
Energia: 84.914921 Wh

Corrente: 0.126039 A
Tensao de entrada: 23.835151 V
Potência instantânea: 3.004000 W
Energia: 84.915755 Wh

Corrente: 0.168052 A
Tensao de entrada: 23.977196 V
Potência instantânea: 4.029000 W
Energia: 84.916874 Wh
```

<p align="justify">O estado de inatividade da impressora é bem caracterizado. Desse modo, é possível configurar o nosso sistema para perceber esse padrão de consumo e realizar o desligamento da impressora ao terminar o processo de impressão, sendo esta uma funcionalidade que queremos entregar com nosso projeto. </p>

:clapper: Vídeo da medição do consumo em Stand-by: https://youtube.com/shorts/RgSNFNDTvaA?feature=share

---

## **Teste de Carga do Banco de Baterias na Impressora 3D**

<p align="justify">Para validar o desempenho do banco de baterias, foram utilizados suportes impressos em 3D, que permitiram uma conexão rápida e segura das células ao Sistema de Gerenciamento de Bateria (BMS). </p>

<p align="center"><b>FIGURA 02 - Desenho 3D do suporte para conexão das baterias</b></p>

<div align="center">
  
![image](https://github.com/user-attachments/assets/1b2a5c9b-4e5d-4d52-980c-ada7ee057f2a)

</div>

### **Teste Inicial: Configuração 6S1P**

<p align="justify"> No primeiro teste, foi montada uma configuração <b>6S1P</b> (6 células em série). O banco de baterias conseguiu alimentar com sucesso tarefas de menor demanda, como o aquecimento do bico e a movimentação dos eixos.</p>

<p align="justify">Porém, ao tentar aquecer a mesa da impressora, a demanda de corrente inicial foi excessivamente alta, confirmando o que a análise de consumo já havia apontado como o <b>momento crítico do sistema</b>. Esse pico de corrente provocou uma queda de tensão abrupta na célula com maior resistência interna, fazendo com que o BMS, por segurança, desligasse todo o sistema após cerca de 15 segundos. </p>

<p align="center"><b>FIGURA 03 - montagem das baterias na configuração 6S1P</b></p>

<div align="center">
  
![image](https://github.com/user-attachments/assets/f3f601fc-c695-43ed-b47c-0ca1b96e327a)

</div>


### **Teste Final: Configuração 6S2P**

<p align="justify"> Para solucionar a limitação de corrente, a configuração foi alterada para <b>6S2P</b> (6 células em série, 2 em paralelo), dobrando a capacidade de descarga do conjunto. Com essa nova montagem, a impressora operou em todas as suas funções sem problemas. </p>

<p align="justify">O teste prático definitivo foi realizado iniciando uma impressão e, em seguida, desconectando a fonte da rede elétrica. A impressora manteve-se operante, com a energia das baterias, por <b>mais de 15 minutos</b>. Um resultado excelente que superou em muito a expectativa inicial do projeto, que era de resistir por, pelo menos, 1 minuto. Isso valida a eficácia do sistema como uma fonte de energia ininterrupta (No-break). </p>

<p align="center"><b>FIGURA 04 - Teste das baterias, na configuração 6S2P, alimentando a impressora </b></p>

<div align="center">

![image](https://github.com/user-attachments/assets/5412cf63-7a3b-4fc4-ac48-573c77f9e2e6)

</div>

:clapper: Vídeo de teste: https://photos.app.goo.gl/ZgK85YDPrmBBUdR36

## **Projeto do Compartimento Final das Baterias**

<p align="justify">Após o sucesso dos testes, foi projetado o compartimento definitivo que irá alocar todas as baterias de forma segura e organizada. O design consiste em um suporte modular: serão utilizados dois slots 3S2P idênticos que, quando associados em série, formarão o banco de baterias 6S2P final para a montagem dentro da case.</p>

<p align="center"><b>FIGURA 05 - Desenho 3D do slot de baterias</b></p>

<div align="center">

![image](https://github.com/user-attachments/assets/f3310ff4-c8e1-4a45-b330-47a2805cf3f0)

</div>


---

## **Projeto 3D da Case do Equipamento**

<p align="justify">Para abrigar o <b>conjunto de baterias</b> e o <b>circuito de medição</b>, foi desenvolvido um projeto de case customizada. O processo iniciou com a tomada de medidas da parte traseira da impressora 3D, o que permitiu elaborar um design que se acopla de forma precisa e funcional ao equipamento.</p>

<p align="center"><b>FIGURA 06 - Suporte onde a impressora será apoiada</b></p>

<div align="center">

![image](https://github.com/user-attachments/assets/afd3850e-05d8-490f-a7b4-fd1cd03f79de)

</div>


<p align="justify">Considerando as dimensões do banco de baterias e das placas eletrônicas, foi possível elaborar um projeto de case robusto. O design visa não apenas proteger os componentes, mas também garantir durabilidade e praticidade, com uma estrutura firme e entradas e saídas de cabos bem definidas.</p>

<p align="center"><b>FIGURA 07 - Posicionamento da case no suporte</b></p>

<div align="center">
  
![image](https://github.com/user-attachments/assets/ca3ad015-c0cc-469f-a98d-0f863b47c642)

</div>


<p align="justify">A case consiste em uma caixa com tampa parafusada para fácil acesso e manutenção. Possui aberturas para a passagem dos cabos que conectam o sistema à fonte de alimentação e à impressora. Além disso, conta com dois orifícios projetados para a instalação de interruptores de energia, garantindo o controle manual do sistema. O espaço interno foi otimizado para acomodar de forma segura e organizada o banco de baterias junto ao circuito de medição.</p>

<p align="center"><b>FIGURA 08 - Detalhamento da case</b></p>

<div align="center">
  
![image](https://github.com/user-attachments/assets/9d87047d-0cac-4c81-90de-278f5fdd2f7b)

</div>

