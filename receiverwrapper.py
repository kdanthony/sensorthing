# SensorThing Receiver Wrapper
# 
# Runs the receiver code and parses the output to graphite

import subprocess
import threading
import graphitesend

graphite = graphitesend.init(graphite_server='192.168.2.211', graphite_port=2003,prefix='Temperatures.House',system_name='',dryrun=False)

proc = subprocess.Popen(['bin/nodereceiver',''], shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

while True:
    lineinput = proc.stdout.readline()
    lineinput = lineinput.strip()
    if len(lineinput.split("|")) > 1:
        node,nodetimestamp,humidity,temperature,voltage = lineinput.split("|")
        print 'Node: ' + node
        print 'Humidity: ' + humidity
        print 'Temperature: ' + temperature
        print 'Voltage: ' + voltage
        print "==="
        if temperature != -50:
            graphite.send('node' + node + '.humidity', humidity)
            graphite.send('node' + node + '.temperature', temperature)
            graphite.send('node' + node + '.voltage', voltage)
    else:
        print lineinput
