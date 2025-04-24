# PI3-2025.1 - Analisador de energia inteligente para impressora 3D + powerbank

## **Problema e Motivação**
<p align="justify"> Durante o processo de impressão 3D, a falta de energia pela concessionária local, mesmo que seja rápida, pode provocar um mal funcionamento temporário na impressora, resultando  na fabricação incorreta da peça 3D. Sabendo-se que o tempo de impressão pode ser muito longo, dependendo do tamanho e da complexidade da peça, evitar esse mal funcionamento é imprenscível para que não se tenha perda de tempo e de material. Com o objetivo de atender essa necessidade, desenvolvemos uma interface, entre fonte de alimentação e a entrada de energia da impressora 3D, que servirá como um powerbank para quedas rápidas de energia e também terá a função de desligamento automático após o fim do processo de impressão.</p> 

## **Objetivos Gerais:**
<p align="justify">O objetivo geral é desenvolver um produto com determinadas funcionalidades que ajude o usuário de
impressora 3D a ter mais confiabilidade e economia na fabricação de suas peças.

## **Casos de Uso**

Imagine as seguintes situações:

> A rede elétrica da sua casa caiu, momentâneamente, e sua impressora estava imprimindo uma  
> peça que demora horas para ficar pronta. Por causa dessa queda rápida, acontece uma falha
> na sua querida impressão.


> Você está trazendo vida a uma estátua que um cliente seu pediu, mas não sabe 
> exatamente como cobrar, pois não entende o quanto gasta de energia com a impressora.


> Você coloca uma peça para ser impressa e não pode estar presente durante o processo de impressão,
> pois tem os seus compromissos. Quando a impressão acabar, não conseguirá desligar 
> a impressora e, portanto, não conseguirá evitar o gasto de energia durante esse tempo fora.


Se esses são alguns dos problemas que você se depara ao usar uma impressoras 3D, o nosso
produto é para você.</p>


## **Objetivos Específicos:**

- Prevenir a falha de impressão decorrente de quedas rápidas de energia;
- Evitar o gasto desnecessário de energia ao término de uma impressão;
- Informar ao usuário sobre o gasto energético da impressão;
- Informar ao usuário de quedas e retornos de energia.

## **Funcionalidades do produto:**

- **Aguenta quedas de energia de 1 minutos sem prejudicar a impressão.**

- **Desliga a impressora após a impressão estar concluída.**

- **Calcula o gasto de energia para a impressão.**

- **Envia notificação de quanta energia foi gasta em uma impressão.**

- **Envia notificação quando a energia cai e quando retorna.**  

## **Sistema**
Na Figura abaixo, apresenta-se o diagrama de blocos do sistema.

<p align="center"><b>Figura 01 - Diagrama de blocos do sistema</b></p>

![DiagramaBlocos](https://github.com/user-attachments/assets/25bebb09-30ef-4287-b567-e93bd9186dbc)


## **Seleção e justificativas na escolha do materiais:**
  
- **Microcontrolador: ESP32 com Wi-Fi**

<p align="center">
<img src="https://github.com/LeoBeche/PI3-2025-1/blob/main/Especifica%C3%A7%C3%B5es/Materiais/Microcontrolador.jpg" width="100" height="100" border="10"/>
</p> 

<p align="justify"><b>Motivo:</b> oferece dois ADCs de 12 bits para amostragem simultânea, além de comunicação sem-fio nativa (Wi-Fi/Bluetooth) para interface móvel e envio de dados em tempo real. Sua performance (dual-core, até 240 MHz) garante resposta rápida às interrupções e ao controle do sistema.</p>

<hr />


- **Sensor de tensão (divisor resistivo para até 25 V)**

<p align="center">
<img src="https://github.com/LeoBeche/PI3-2025-1/blob/main/Especifica%C3%A7%C3%B5es/Materiais/Sensor_tensao.jpg" width="100" height="100" border="10"/>
</p> 

<p align="justify"><b>Motivo:</b> Sensor realiza a medição de tensão de até 25 V, sendo que a impressora funciona com 24V, e fornece um sinal em um intervalo seguro (0–3,3 V) para os ADCs do ESP32, permitindo leitura direta e linear da tensão de alimentação da impressora. </p>

<hr />

- **Sensor de corrente: ACS712 20 A**

<p align="center">
<img src="https://github.com/LeoBeche/PI3-2025-1/blob/main/Especifica%C3%A7%C3%B5es/Materiais/Sensor_corrente.jpg" width="100" height="100" border="10"/>
</p> 

<p align="justify"><b>Motivo:</b> o ACS712 fornece isolamento galvânico e saída analógica proporcional à corrente (±20 A), compatível com o range esperado da impressora 3D. Conecta-se ao segundo ADC do ESP32, permitindo cálculo de potência instantânea sem circuitos de corrente contínua direta.</p>

<hr />

- **Sensor de queda de tensão: comparador (amplificador operacional)**

<p align="center">
<img src="https://github.com/LeoBeche/PI3-2025-1/blob/main/Especifica%C3%A7%C3%B5es/Materiais/Sensor_queda_tensao.jpg" width="100" height="100" border="10"/>
</p> 

<p align="justify"><b>Motivo:</b> um comparador baseado em amp-op gera um sinal digital ao detectar subtensão, por exemplo, abaixo de 20 V, acionando o sistema para envio da informação de que o houve queda de energia para o usuário.</p>

<hr />

- **Interruptor de energia: Relé SRA-05VDC-CL**

<p align="center">
<img src="https://github.com/LeoBeche/PI3-2025-1/blob/main/Especifica%C3%A7%C3%B5es/Materiais/Rele.jpg" width="100" height="100" border="10"/>
</p> 

<p align="justify"><b>Motivo:</b> relé de 5 VDC com contato para até 20A em 24V DC, suficiente para a corrente da impressora. Tem fácil acionamento, garantindo isolamento entre potência e lógica.</p>

<hr />

- **Bateria escolhida: Células 18650 Li-ion (reutilizadas de notebooks)**

<p align="center">
<img src="https://github.com/LeoBeche/PI3-2025-1/blob/main/Especifica%C3%A7%C3%B5es/Materiais/Bateria.jpg" width="100" height="100" border="10"/>
</p> 

<p align="justify"><b>Motivo:</b> Alta densidade de energia, longa vida útil (500~1000 ciclos), elevada eficiência (≈ 95 %) e tensão ideal (6 S: 22,2V nominal; 25,2V carga) para sistema 24 V. Reuso de células usadas reduz custo e impacto ambiental, promovendo economia circular.</p>

<hr />

- **BMS: Módulo S6 24 V 40 A para Li-íon**

<p align="center">
<img src="https://github.com/LeoBeche/PI3-2025-1/blob/main/Especifica%C3%A7%C3%B5es/Materiais/BMS.jpg" width="100" height="100" border="10"/>
</p> 

<p align="justify"><b>Motivo:</b> Oferece proteção contra sobrecarga, subtensão, sobrecorrente e balanceamento de 6S de células 18650, suportando até 40 A de descarga máxima. Garante segurança e prolonga a vida útil do banco de baterias.</p>

 <hr />
 
- **Conectores XT60**

<p align="center">
<img src="https://github.com/LeoBeche/PI3-2025-1/blob/main/Especifica%C3%A7%C3%B5es/Materiais/XT60.jpg" width="100" height="100" border="10"/>
</p> 

<p align="justify"><b>Motivo:</b> Padrão da Ender3 Creality e de muitas impressoras 3D, suportam correntes até 60 A e fornecem conexão robusta, polaridade garantida e baixa resistência de contato, facilitando integração direta com a impressora sem adaptadores.</p>


# Cabos de saída: cabo elétrico flexível 2.5 mm

<p align="center">
<img src="https://github.com/LeoBeche/PI3-2025-1/blob/main/Especifica%C3%A7%C3%B5es/Materiais/Cabos.jpg" width="100" height="100" border="10"/>
</p> 

> Motivo: Visto que a potência de saída da fonte da impressora Ender 3 Creality é de 270W a 24V, o cabo foi calculado com base na corrente máxima suportada e colocado um fator de segurança de 2. 

<p align="center">
<img src="https://github.com/LeoBeche/PI3-2025-1/blob/main/Especifica%C3%A7%C3%B5es/Materiais/Cabo.JPG" width="441" height="170" border="10"/>
</p> 