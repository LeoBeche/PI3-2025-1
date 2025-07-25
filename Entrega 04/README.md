## **Interface com o usuário**
 
<p align="justify"> Uma das definições do nosso projeto está em informar o usuário sobre a queda de energia e, no término do processo de impressão, o valor da energia consumida, relacionando com o preço do kWh. Para isso, optou-se por fazer a interface com o usuário por meio de mensagens enviadas pelo whatsapp, através de um BOT da API <a href="https://www.callmebot.com/blog/free-api-whatsapp-messages/">CallMeBot</a>. É necessário que o usuário adicione o telefone do BOT e envie uma mensagem padrão para habilitar o serviço, como mostrado na imagem abaixo. </p>

<p align="center"><b>FIGURA 01 - Obter KEY da API callmebot </b></p>

<div align="center">

<img width="895" height="418" alt="image" src="https://github.com/user-attachments/assets/d9bc06b9-2e77-4884-a5d0-93e3996565f3" />

</div>


<p align="justify"> Para se comunicar com o sistema, o usuário precisará instalar no celular o aplicativo "Serial Bluetooth Terminal", disponível gratuitamente na playstore. Com o bluetooth habilitado e estando no aplicativo, na opção Devices, o usuário deve-se conectar ao POWER_BANK_SYSTEM, como mostrado abaixo. </p>
  
<p align="center"><b>FIGURA 02 - Conexão com Bluetooth</b></p>

<div align="center">
  
<img width="498" height="365" alt="image" src="https://github.com/user-attachments/assets/646ae9c9-166d-432b-9148-1b5b98843ee9" />

</div>

Para configurar o sistema, 4 informações precisarão ser enviadas: 

- **KEY:** Chave obtida do callmebot.
- **Phone:** O número de telefone. no seguinte formato: 55DDxxxxxxxx
- **SSID:** SSID da rede Wifi
- **PASS:** Senha da rede Wifi

<p align="center"><b>FIGURA 03 - Informações por Bluetooth</b></p>

<div align="center">
  
<img width="312" height="402" alt="image" src="https://github.com/user-attachments/assets/8aefad48-6a56-4aea-a861-5a136b022a39" />

</div>

 
<p align="justify"> Se todas as informações estiverem corretas, o usuário deverá receber no whatsapp a mensagem de configuração bem sucedida, conforme mostrado abaixo. </p>

<p align="center"><b>FIGURA 04 - Mensagem de configuração correta</b></p>

<div align="center">
  
<img width="858" height="516" alt="image" src="https://github.com/user-attachments/assets/41a3b873-22d4-4003-920f-d398ce9f82de" />

</div>

<p align="justify"> Caso não consiga se conectar ao WiFi ou o envio da mensagem não for bem sucedido, o sistema pedirá para informar novamente as credenciais do WiFi ou do CallMeBot, respectivamente. Nas imagens abaixo, apresenta-se o teste para cada situação. </p>

- Informou as credenciais do WiFi e do CallMeBot, mas as credenciais do WiFi estão incorretas.

<div align="center">
  
<img width="532" height="199" alt="image" src="https://github.com/user-attachments/assets/ad216905-8c43-4ff3-b350-242d338e4f2d" />

</div>

  
- Forneceu as credenciais corretas do WiFi, mas a do CallMeBot estão incorretas.

<div align="center">

  <img width="532" height="199" alt="image" src="https://github.com/user-attachments/assets/8048910d-d5f1-46cb-b333-aebc66513e22" />

</div>

- Forneceu as credenciais corretas do WiFi e do CallMeBot.

<div align="center">
  
<img width="532" height="199" alt="image" src="https://github.com/user-attachments/assets/95007322-801f-4422-ae08-4158939027a0" />

</div>

<p align="justify"> Após o sucesso na conexão com o WiFi e com o CallmeBot, o Bluetooth é desligado (por isso aparece "Connection lost") e as credenciais são salvas no NVS (Non-Volatile Storage), portanto, o usuário não precisará refazer esse processo sempre que religar o equipamento. </p>

### **Mensagens trocadas com o usuário**

- Fim do processo de impressão e gasto total para imprimir a peça:

<div align="center">

  <img width="851" height="510" alt="image" src="https://github.com/user-attachments/assets/b0d08d58-ec9f-4ba5-bd67-19ecc6c32618" />

</div>

- Queda energia:
  
<div align="center">
 
<img width="841" height="509" alt="image" src="https://github.com/user-attachments/assets/aa724df8-e966-4ce4-840c-8204ff261ff9" />

</div>
  
- Retorno da energia:

<div align="center">

<img width="845" height="504" alt="image" src="https://github.com/user-attachments/assets/b79aca12-9261-4cbe-a9d6-cc783dc5e4fd" />

</div>


---

<p align="justify">Realizamos um ensaio com o analizador de energia disponibilizado pelo professor e comparamos medindo na mesma condição com o nosso sistema. O resultado das medições com o analizador: </p>


<p align="center"><b>Impressão: Cubo de Calibração - Condição de medição: mesa fria</b></p>

-- imagem ainda a ser alterada
<div align="center">

![image](https://github.com/user-attachments/assets/f3310ff4-c8e1-4a45-b330-47a2805cf3f0)

</div>

<p align="center"><b>Impressão: Cubo de Calibração - Condição de medição: mesa fria</b></p>

'''
Corrente: 2.352728 A
Corrente: 660 A
tensao de entrada: 23.977196 V
Pontência instantânea: 56.411000 W
Energia: 52.138546 Wh

Corrente: 0.798247 A
Corrente: 623 A
tensao de entrada: 23.920378 V
Pontência instantânea: 19.094000 W
Energia: 52.143850 Wh

Corrente: 0.840260 A
Corrente: 624 A
tensao de entrada: 23.977196 V
Pontência instantânea: 20.147000 W
Energia: 52.149446 Wh

Corrente: 0.084026 A
Corrente: 606 A
tensao de entrada: 23.977196 V
Pontência instantânea: 2.014000 W
Energia: 52.150006 Wh

Corrente: 0.084026 A
Corrente: 606 A
tensao de entrada: 23.948787 V
Pontência instantânea: 2.012000 W
Energia: 52.150564 Wh

Corrente: 0.084026 A
Corrente: 606 A
tensao de entrada: 23.948787 V
Pontência instantânea: 2.012000 W
Energia: 52.151123 Wh

Corrente: 0.084026 A
Corrente: 606 A
tensao de entrada: 23.948787 V
Pontência instantânea: 2.012000 W
Energia: 52.151682 Wh

Corrente: 0.084026 A
Corrente: 606 A
tensao de entrada: 23.977196 V
Pontência instantânea: 2.014000 W
Energia: 52.152242 Wh

Corrente: 0.084026 A
Corrente: 606 A
tensao de entrada: 23.948787 V
Pontência instantânea: 2.012000 W
Energia: 52.152801 Wh

Corrente: 0.126039 A
Corrente: 607 A
tensao de entrada: 23.977196 V
Pontência instantânea: 3.022000 W
Energia: 52.153640 Wh

Corrente: 0.084026 A
Corrente: 606 A
tensao de entrada: 24.034014 V
Pontência instantânea: 2.019000 W
Energia: 52.154201 Wh

Corrente: 0.084026 A
Corrente: 606 A
tensao de entrada: 23.948787 V
Pontência instantânea: 2.012000 W
Energia: 52.154760 Wh

'''

<p align="justify">Medições coom o nosso sistema: </p>


---

<p align="justify">O vídeo de operação do sistema com queda de tesão se encontra em: https://photos.app.goo.gl/2cjk2MKaVvkJwBAs5 </p>
