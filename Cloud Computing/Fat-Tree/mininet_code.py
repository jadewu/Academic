#!/usr/bin/python

"""
This setup the topology in lab3-part1
"""

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

class Topology(Topo):
    
    
    def __init__(self):
        "Create Topology."
        
        # Initialize topology
        Topo.__init__(self)
        
        
        #### There is a rule of naming the hosts and switch, so please follow the rules like "h1", "h2" or "s1", "s2" for hosts and switches!!!!
      
        # Add hosts
        host1 = self.addHost('h1', ip='10.0.0.1/24')
        host2 = self.addHost('h2', ip='10.0.0.2/24')
        
        
        # Add switches
        swA = self.addSwitch('s1')
        swB = self.addSwitch('s2')
        swC = self.addSwitch('s3')
        swD = self.addSwitch('s4')
        swE = self.addSwitch('s5')
        

        # HOST1 --- SA
        self.addLink(host1, swA)        
        # SA P2 --- SB P1
        self.addLink(swA, swB, 2, 1)   
        # SA P3 --- SC P1
        self.addLink(swA, swC, 3, 1)
        # SB P2 --- SE P1
        self.addLink(swB, swE, 2, 1)
        # SB P3 --- SD P1
        self.addLink(swB, swD, 3, 1)
        # SE P3 --- SD P2
        self.addLink(swE, swD, 3, 2)
        # SE P2 --- SC P2
        self.addLink(swE, swC, 2, 2)
        # SC P3 --- SD P3
        self.addLink(swC, swD, 3, 3)
        # SD --- HOST2
        self.addLink(swD, host2)
        
        
        

# This is for "mn --custom"
topos = { 'mytopo': ( lambda: Topology() ) }

