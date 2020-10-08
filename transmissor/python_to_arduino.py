import serial
ser = serial.Serial('COM4', baudrate = 9600, timeout = 1)

def getValues(userInput):
	ser.write(userInput.encode('ascii'))
	
while 1:
	userInput = input('Digite os numeros: ')
	getValues(userInput)
