#!/usr/bin/python

import atexit

import requests

import json

import datetime

import serial

import os

import signal

import sys

import time



class Daemon(object):

    """ Linux Daemon boilerplate. """

    def __init__(self, pid_file,

                 stdout='/var/log/smartbee/smartbee_out.log',

                 stderr='/var/log/smartbee/smartbee_err.log'):

        self.stdout = stdout

        self.stderr = stderr

        self.pid_file = pid_file



    def del_pid(self):

        """ Delete the pid file. """

        os.remove(self.pid_file)



    def daemonize(self):

        """ There shined a shiny daemon, In the middle, Of the road... """

        # fork 1 to spin off the child that will spawn the deamon.

        if os.fork():

            sys.exit()



        # This is the child.

        # 1. cd to root for a guarenteed working dir.

        # 2. clear the session id to clear the controlling TTY.

        # 3. set the umask so we have access to all files created by the daemon.

        os.chdir("/")

        os.setsid()

        os.umask(0)



        # fork 2 ensures we can't get a controlling ttd.

        if os.fork():

            sys.exit()



        # This is a child that can't ever have a controlling TTY.

        # Now we shut down stdin and point stdout/stderr at log files.



        # stdin

        with open('/dev/null', 'r') as dev_null:

            os.dup2(dev_null.fileno(), sys.stdin.fileno())



        # stderr - do this before stdout so that errors about setting stdout write to the log file.

        #

        # Exceptions raised after this point will be written to the log file.

        sys.stderr.flush()

        with open(self.stderr, 'a+', 0) as stderr:

            os.dup2(stderr.fileno(), sys.stderr.fileno())



        # stdout

        #

        # Print statements after this step will not work. Use sys.stdout

        # instead.

        sys.stdout.flush()

        with open(self.stdout, 'a+', 0) as stdout:

            os.dup2(stdout.fileno(), sys.stdout.fileno())



        # Write pid file

        # Before file creation, make sure we'll delete the pid file on exit!

        atexit.register(self.del_pid)

        pid = str(os.getpid())

        with open(self.pid_file, 'w+') as pid_file:

            pid_file.write('{0}'.format(pid))



    def get_pid_by_file(self):

        """ Return the pid read from the pid file. """

        try:

            with open(self.pid_file, 'r') as pid_file:

            	pid = int(pid_file.read().strip())

            return pid

        except IOError:

            return



    def start(self):

        """ Start the daemon. """

        print "Starting..."

        if self.get_pid_by_file():

            print 'PID file {0} existe. Ja existe um deamon executando?'.format(self.pid_file)

            sys.exit(1)



        self.daemonize()

        self.run()



    def stop(self):

        """ Stop the daemon. """

        print "Stopping..."

        pid = self.get_pid_by_file()

        if not pid:

            print "PID file {0} nao existe. O deamon nao esta rodando?".format(self.pid_file)

            return



        # Time to kill.

        try:

            while 1:

                os.kill(pid, signal.SIGTERM)

                time.sleep(0.1)

        except OSError as err:

            if 'No such process' in err.strerror and os.path.exists(self.pid_file):

                os.remove(self.pid_file)

            else:

                print err

                sys.exit(1)



    def restart(self):

        """ Restart the deamon. """

        self.stop()

        self.start()





    def run(self):

        """ The main loop of the daemon. """

	#Variaveis globais

	porta = "/dev/ttyACM0"

	velocidade = 57600
	COLMEIA1='ODQHRWU1WIPGH2CK'
	COLMEIA2='Z63X9L46JZFPDUWZ'

	urlFiware = "http://35.184.233.148:1026/v1/updateContext"

	

	try:

		#Conexao com porta serial
		conexao = serial.Serial(porta, velocidade);

		#Limpa o buffer da porta serial para nao ler valores errados
		conexao.flushInput()
		

		while 1:

			leitura = conexao.readline()

			valores = leitura.split(' ')

			if leitura != "" and len(valores) == 7:



				#Adiciona os cabecaalhos a requisicao

				headers = {

					'Content-Type':'application/json',

					'Fiware-Service': 'tcc',

					'Fiware-ServicePath': '/',

				}

				#Preparacao dos dados para enviar

				data1 = open('/etc/smartbee/json.json','r')

				data = data1.read()

				data1.close()

				

				#Obter data e hora

				a = datetime.datetime.now()

				data_atual = str(a.strftime('%d/%m/%y'))

				hora = str(a.strftime('%H:%M:%S'))

				

				#Alterar o atributos principais do JSON

				#IdColmeia

				data = data.replace('$1', valores[0])

				#Temperatura				

				data = data.replace('$2', valores[1])

				#Umidade

				data = data.replace('$3', valores[2])

				#Som

				data = data.replace('$4', valores[3])

				#Data

				data = data.replace('$5', data_atual)

				#Hora

				data = data.replace('$6', hora)

				#Co2

				data = data.replace('$7', valores[4])

				#TensaoColmeia

				data = data.replace('$8', valores[5])

				#TensaoRepetidor

				data = data.replace('$9', valores[6])

				

				#Consertando erros no arquivo JSON

				data = json.dumps(data)

				data = data.replace('\\n', '')

				data = data.replace('\\', '')

				data = data.replace('\"{', '{')
				data = data.replace('r\"}]}]','\"}]}]')

				data = data.replace('}\"', '}')

				status_requisicao = ''
				
				#ENVIANDO REQUISICAO PARA THINKSPEAK
				id_key='';
				if (valores[0] == 'Colmeia1'):
					id_key = COLMEIA1
				elif (valores[0] == 'Colmeia2'):			
					id_key = COLMEIA2
					
						
				with open('/etc/smartbee/leituras.txt', 'a') as arq:

					arq.write(leitura+'\n')
			
				try:
					r = requests.post(urlFiware, headers=headers, data=data)
					url="https://api.thingspeak.com/update?api_key=%s&field1=%s&field2=%s&field3=%s&field4=%s&field5=%s&field6=%s" %(id_key,valores[1], valores[2], valores[3], valores[4], valores[5], valores[6])
					r = requests.get(url)
				except urllib3.connection.HTTPConnection as err:
					with open('/var/log/smartbee/smartbee_err.log', 'a') as arq:
			                        arq.write(str(err)+'\n')
				except Exception as err:
                                        with open('/var/log/smartbee/smartbee_err.log', 'a') as arq:
						arq.write(str(err)+'\n')

	except Exception as err:

		with open('/var/log/smartbee/smartbee_err.log', 'a') as arq:

			arq.write(str(err)+'\n')

if __name__ == '__main__':

    if len(sys.argv) < 2:

        print "Use: {0} start|stop|restart".format(sys.argv[0])

        sys.exit(2)



    daemon = Daemon('/tmp/daemon_example.pid')

    if 'start' == sys.argv[1]:

        daemon.start()

    elif 'stop' == sys.argv[1]:

        daemon.stop()

    elif 'restart' == sys.argv[1]:

        daemon.restart()

    else:

        print "Comando desconhecido '{0}'".format(sys.argv[1])

        sys.exit(2)