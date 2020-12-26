#!/usr/bin/python
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import Controller, RemoteController
from mininet.cli import CLI
from mininet.log import setLogLevel, info
from mininet.util import dumpNodeConnections
from mininet.link import Link, Intf, TCLink
import os
from time import sleep
import sys

k = 4

class Topology(Topo):
    def __init__(self):
        # Initialize topology
        Topo.__init__(self)

        core = int(pow((k/2),2))
        aggr = int(((k/2)*k))
        edge = int(((k/2)*k))
        host = int((k*pow((k/2),2)))

        cores = []
        aggrs = []
        edges = []
        hosts = []

        for i in range(core):
            cores.append(self.addSwitch('c'+str(i)))

        for pod in range(k): # construct each pod
            for i in range(aggr/k):
                switch = self.addSwitch('a'+str(i)+'_p'+str(pod))
                aggrs.append(switch)
                # for j in range(k*aggr//2, (k/2)*(aggr+1)):
                for j in range(k*i//2, (k/2)*(i+1)):
                    self.addLink(switch, cores[j])

            for i in range(edge/k):
                switch = self.addSwitch('e'+str(i)+'_p'+str(pod))
                edges.append(switch)
                for j in range((edge/k*pod), (edge/k)*(pod+1)):
                    self.addLink(switch, aggrs[j])

                for j in range(host/edge):
                    h = self.addHost('h'+str(j)+'_e'+str(i)+'_p'+str(pod))
                    hosts.append(h)
                    self.addLink(h, edges[i+pod*(k/2)])



# This is for "mn --custom"
topos = { 'mytopo': ( lambda: Topology() ) }

