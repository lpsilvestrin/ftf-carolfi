# -*- coding: utf-8 -*-

######################################################################
## Classes relacionadas às caches. Podem ser modificadas para alterar
## o tamanho (size), a associatividade (assoc), a latência no caso de
## hit (hit_latency) e a latência no caso de miss (response_latency) -
## ou seja, o tempo necessário para encaminhar a requisição ao proximo
## nível na hierarquia.
######################################################################

from m5.objects import BaseCache

## Não modificar esta classe. Modifique as classes L1ICache ou
## L1DCache. 
class L1Cache(BaseCache):
    """Cache L1 com parâmetros padrões"""

    def __init__(self):
        super(BaseCache, self).__init__()
        pass
    
    def connectCPU(self, cpu_port):
        """"Conecta a porta da cache à porta da CPU. Vai ser
            definida nas subclasses."""
        raise NotImplementedError

    def connectBus(self, bus):
        """"Conecta a porta da cache à porta do barramento abaixo na hierarquia"""
        self.mem_side = bus.slave

    mshrs = 4
    tgts_per_mshr = 20

    is_top_level = True

## Cache L1 de instruções
class L1ICache(L1Cache):
    """Cache L1 de instruções"""

    ## Tamanho da cache L1-I
    size = '8kB'
    ## Associatividade da cache L1-I
    assoc = 4
    ## Latência do hit da cache L1-I
    hit_latency = 2
    ## Latência do miss da cache L1-I
    response_latency = 2

    ## Para uso do simulador: não modificar.
    def connectCPU(self, cpu):
        """"Conecta a porta da cache à porta de instruções da CPU"""
        self.cpu_side = cpu.icache_port
    
    def __init__(self):
        super(L1ICache, self).__init__()

## Cache L1 de dados
class L1DCache(L1Cache):
    """Cache L1 de dados com parâmetros padrões"""

    ## Tamanho da cache L1-D
    size = '8kB'
    ## Associatividade da cache L1-D
    assoc = 4
    ## Latência do hit da cache L1-D
    hit_latency = 2
    ## Latência do miss da cache L1-D
    response_latency = 2

    ## Para uso do simulador: não modificar.
    def connectCPU(self, cpu):
        """"Conecta a porta da cache à porta de dados da CPU"""
        self.cpu_side = cpu.dcache_port
        
    def __init__(self):
        super(L1DCache, self).__init__()

## Cache L2. 
class L2Cache(BaseCache):
    """Cache L2 compartilhada com parâmetros padrões"""

    ## Tamanho da cache L2
    size = '256kB'
    ## Associatividade da cache L2
    assoc = 8
    ## Latência do hit da cache L2
    hit_latency = 4
    ## Latência do miss da cache L2
    response_latency = 6

    ## Para uso do simulador: não modificar.
    def __init__(self):
        super(L2Cache, self).__init__()

    def connectCPUSideBus(self, bus):
        """"Conecta a cache ao barramento acima na hierarquia."""
        self.cpu_side = bus.master

    def connectMemSideBus(self, bus):
        """"Conecta a cache ao barramento abaixo na hierarquia."""
        self.mem_side = bus.slave

    mshrs = 20
    tgts_per_mshr = 12
    
    is_top_level = False

## Cache L3    
class L3Cache(BaseCache):
    """Cache L3 compartilhada com parâmetros padrão"""

    ## Tamanho da cache L3
    size = '2MB'
    ## Associatividade da cache L3
    assoc = 16
    ## Latência de hit da cache L3
    hit_latency = 16
    ## Latência de miss da cache L3
    response_latency = 10
  
    ## Para uso do simulador: não modificar.
    def connectCPUSideBus(self, bus):
        """"Conecta a cache ao barramento acima na hierarquia."""
        self.cpu_side = bus.master

    def connectMemSideBus(self, bus):
        """"Conecta a cache ao barramento abaixo na hierarquia."""
        self.mem_side = bus.slave

    def __init__(self):
        super(L3Cache, self).__init__()

    mshrs = 20
    tgts_per_mshr = 12

    is_top_level = False
