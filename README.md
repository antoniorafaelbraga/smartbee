# Sm@rtBee

The main objective of this project is to describe the behavior of bees in healthy hives and ready for pollination. Through real-time and minimally invasive monitoring, data from hives and colonies will be collected through heterogeneous sensors, connected to sensing devices with embedded radios. The data will be stored in a computational cloud and accessed remotely via the Web.



Se você desejar deixar o script do gateway executando no BeagleBone, não precisa fazer os passos abaixo, pule para a etapa 2

# Etapa 1.
Para executar os arquivos da instalação, é necessário pegar os arquivos mais recentes que estão no BeagleBone. Para isso é preciso plugar o BBB no computador e acessar por ssh o ip 192.168.7.2.

Apaga os arquivos antigos
#rm -rf /root/smartbee/gateway/final/*

Copiar os arquivo mais recentes
#cd /etc/smartbee/

#cp smartbeed.py start_daemon.sh json.json /root/smartbee/gateway/final/files

#cd /root/smartbee/gateway/final/

Zipar os arquivos
tar -zcvf smartbee.tar.gz *

Agora é necessário transferir esses arquivos para outra máquina, pode usar ferramentas como scp ou até o mRemoteNG

# Etapa 2.
Instalação do serviço. Esse serviço será responsável por ler a porta serial e enviar para o Fiware-Orion.
Após descompactar o arquivo zipado na etapa 1. Execute o script.

#./installer.sh

Serão criados os arquivos smartbee_err.log e smartbee_out.log na pasta /var/log/smartbee. Esses arquivos serão utilizados para ver a saída do programa. Qualquer erro que ocasionar no programa que faça o serviço parar, o correto é verificar o erro no arquivo smartbee_err.out.
Será criado uma pasta no /etc/smartbee, e serão copiados os seguintes arquivos smartbeed.py, json.json, start_daemon.py e leituras.txt.
smartbeed.py: Script responsável por iniciar o serviço.
json.json: Modelo de json usado pelo script para enviar os dados ao Fiware Orion.
leituras.txt: Arquivo que salva as leituras já enviadas para o servidor.Os dados são salvos no modelo [Conteúdo lido da porta serial >> status requisicao http]
O script start_daemon.py será tambem copiado para a pasta /etc/init.d. Esse script deveria iniciar junto com o sistema.
OBS: Se o sistema for ubuntu, a linha insserv -dv /etc/init.d/start_daemon.sh deve ser substituída pelo comando update-rc.d /etc/init.d/start_daemon.sh defaults

# Etapa 3. Execução do Script
Para iniciar o script:
	#/etc/smartbee/./smartbeed.py start
Para parar o script:
	#/etc/smartbee/./smartbeed.py stop
Para verificar se script está rodando:
	#ps aux | grep smartbee
Se o script por acaso não iniciar, a melhor forma é verificar o motivo do erro no arquivo de logs: /var/log/smartbee/smartbee_err.out


# Fazer o BeagleBone funcionar
As vezes quando o BBB é inicializado, não inicia a interface de rede, às vezes não inicia a USB. A dica que dou é, com o BBB desligado, conecte apenas a interface de rede e ligue o BBB pela usb B. Acesse o bbb pelo IP 192.168.7.2 e veja qual IP o BBB pegou(se pegar). Depois desligue o BBB e ligue já alimentado pela tomada. Acesse usando o IP que ele pegou pela conexão de rede e só assim conecte o arduino.
