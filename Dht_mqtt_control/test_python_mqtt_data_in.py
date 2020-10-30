import paho.mqtt.client as mqtt
import time
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("conectado ao broker:raspberry")
        global Connected                
        Connected = True                #estado inicial da conexão 
    else:
        print("Conexão Falhada")
Connected = False   #global variable for the state of the connection
broker_address= "192.168.1.100" #endereço do raspberry na rede local
port = 1883
user = "username"                                  #username da configuração do mqtt
password = "admin"                                 #password da configuração do mqtt 
topic = "esp8266" #topico definido para o mqtt, o MCU board contém o mesmo topico para envio de mensagens 
client = mqtt.Client("Python")               
client.username_pw_set(user, password=password)    #define username and password
client.on_connect= on_connect                      #attach function to callback
client.connect(broker_address, port=port)          #conecta ao MQTT broker
client.loop_start()                                #permite garantir a conexão
while Connected != True:                           #Aguarda a conexão
    time.sleep(0.1)
try:                                                #tenta recolher dados do teclado para o envio 
    while True:
        value = raw_input('Insira a mensagem que deseja enviar') #esta linha permite a introdução de dados diretamente para a variavel value 
        client.publish(topic,value)
except KeyboardInterrupt:                           #caso nao seja possivel provoca uma interrupção no keyboard proveniente de uma excepção
    client.disconnect()
    client.loop_stop()
    
    
    