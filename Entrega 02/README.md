
## **Detecção de queda de energia**

- **Circuito:**
  
Para a detecção da queda de energia, foi utilizado o circuito comparador, mostrado na imagem abaixo. Verifica-se que a saída do amplificador apresentará nível lógico alto quando ocorer a queda de energia e 0V quando estiver operando normalmente. Como o ESP32 opera com 3V3, necessitou-se colocar um divisor de tensão na saída do amplificador para adequar a tensão.

![image](https://github.com/user-attachments/assets/7f016651-d7c7-4718-b856-b0c628e8db92)

Optou-se por utilizar o LM358 devido às seguintes características:
  -  Alimentão assimétrica.
  - Valor mínimo de saída de 0V.
  - Não apresenta comportamento anormal quando uma das entradas tem 0V.
  
---

- **Montagem:**

Realizou-se a montagem do circuito em uma placa universal, apresentada na figura abaixo, para verificar o funcionamento do circuito e poder testar juntamente com o ESP32. Para simular a queda de energia utilizou-se um jumper, indicado com o quadrado vermelho.

![image](https://github.com/user-attachments/assets/878af08e-ccc4-40cf-91e7-01609b6b331a)

---

- **Resultado:**

A imagem abaixo apresenta o resultado com o código *"detecta_queda.c"* ao remover o Jumper do circuito.

![Sem título](https://github.com/user-attachments/assets/c18f7853-6ea9-4e49-b850-8157645b7cae)

---

## **Roteiro de Teste para Células 18650 Recuperadas**
1. **Preparação e Inspeção Visual**
---
**Remoção de resíduos:** Retire cuidadosamente fitas, adesivos e tiras de níquel das células. Utilize ferramentas apropriadas e, se necessário, um soprador de ar quente para facilitar a remoção de adesivos, evitando aquecer excessivamente as células.

**Inspeção visual:** Examine cada célula em busca de danos físicos, como inchaços, vazamentos, descoloração ou odores incomuns. Descarte imediatamente qualquer célula com esses sinais.


2. **Medição da Tensão Inicial**
---
**Instrumento necessário:** Multímetro digital.

**Procedimento:**
Meça a tensão de cada célula.
**Critérios de seleção:**
Células com tensão < 2,0V: Descarte, pois podem estar profundamente descarregadas e instáveis.
Células com tensão entre 2,0V e 4,2V: Prosseguir para o próximo passo.


3. **Carga Inicial**
---
**Equipamento recomendado:** Carregadores específicos para 18650, como LiitoKala Lii-500 ou Opus BT-C3100.

**Parâmetros de carga:**
Tensão final: 4,2V
Corrente de carga: 0,5C (exemplo: para uma célula de 2500mAh, utilize 1,25A)

**Monitoramento:**
Observe a temperatura durante a carga. Se a célula aquecer excessivamente (acima de 45°C), descarte-a.

4. **Teste de Autodescarga (24 horas)**
---
**Procedimento:**
Após a carga completa, deixe a célula em repouso por 24 horas em ambiente seguro.
Meça novamente a tensão.

**Critérios de avaliação:**
Se a tensão cair abaixo de 4,0V, a célula apresenta alta autodescarga e deve ser descartada.

5. **Teste de Capacidade**
---
**Equipamento necessário:** Carregadores com função de teste de capacidade, como LiitoKala Lii-500 ou Opus BT-C3100.

- **Procedimento:**

Realize ciclos de carga e descarga completos, medindo a capacidade real em mAh.

- **Critérios de seleção:**

Células com capacidade < 1000mAh: Descarte.

Células com capacidade entre 1000mAh e 1800mAh: Uso limitado.

Células com capacidade > 1800mAh: Adequadas para projetos.

**Resumo dos Critérios de Seleção:**
  
 - **Parâmetro	Critério:**

  Tensão inicial	≥ 2,0V

  Tensão após 24h	≥ 4,0V
  
  Capacidade	≥ 1800mAh
  
  Temperatura durante carga	≤ 45°C

---
**Segurança:**

Sempre utilize equipamentos adequados e siga as instruções dos fabricantes.

Realize os testes em ambientes ventilados e longe de materiais inflamáveis.

Descarte as células danificadas de acordo com as regulamentações locais.
