
## **Detecção de queda de energia**

- **Circuito:**
  
Para a detecção da queda de energia, foi utilizado o circuito comparador, mostrado na imagem abaixo. Verifica-se que a saída do amplificador apresentará nível lógico alto quando ocorrer a queda de energia e 0V quando estiver operando normalmente. Como o ESP32 opera com 3V3, necessitou-se colocar um divisor de tensão na saída do amplificador para adequar a tensão.

![image](https://github.com/user-attachments/assets/7f016651-d7c7-4718-b856-b0c628e8db92)

Optou-se por utilizar o LM358 devido às seguintes características:
  -  Alimentação assimétrica.
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
![image](https://github.com/user-attachments/assets/66adb60d-f968-463e-83bf-26c08dd41e30)

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
**Equipamento recomendado:** Carregadores específicos para 18650, para os testes foram utilizados 3 carregadores individuais para baterias de lítio 18650.
![image](https://github.com/user-attachments/assets/48153725-988e-468a-b991-2bbf9bcd9e62)


**Parâmetros de carga:**
Tensão final: ~ 4,1V

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
**Equipamento necessário:** Carregadores com função de teste de capacidade.

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

**Resultados dos Testes:**
---

Foram inicialmente avaliadas 11 células 18650 recuperadas de baterias de notebooks antigos. Todas elas apresentaram tensão superior a 2V no momento da triagem inicial, o que indicava que estavam dentro de um intervalo aceitável para prosseguir com os testes seguintes.

Após a carga completa de todas as células até 4,1V, elas foram deixadas em repouso por um período de 24 horas, com o objetivo de verificar a estabilidade da tensão — uma etapa fundamental para identificar células com alta taxa de autodescarga. Esse comportamento geralmente está associado a danos internos ou degradação química que comprometem tanto a capacidade de retenção de carga quanto a segurança em aplicações práticas.

Ao final do período de repouso, duas células apresentaram queda significativa de tensão, ficando abaixo de 4,0V. Essa variação é um indicativo claro de que essas células não conseguem manter a carga adequadamente mesmo em repouso, e por isso foram consideradas inadequadas e descartadas, seguindo critérios de segurança e desempenho adotados em boas práticas de reaproveitamento de baterias.

As nove células restantes mantiveram tensões próximas a 4,1V, demonstrando boa estabilidade e comportamento confiável durante o teste. Essas células foram então aprovadas e consideradas aptas para utilização no projeto, seja em pacotes de bateria, sistemas de energia portátil ou aplicações de baixo consumo. A realização deste processo de avaliação garante que apenas as células em boas condições sejam reaproveitadas, contribuindo para a segurança, eficiência e durabilidade do projeto.
