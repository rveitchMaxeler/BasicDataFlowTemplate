#!/usr/bin/env python

import sys

try:
	from RuntimeBuilder import *
	from Sim import *
except ImportError, e:
	print "Couldn't find project-utils modules."
	sys.exit(1)

MAXFILES = ['PassThrough.max']
sources = ['myproject.c']
target = 'myproject'
includes = []
cflags = ['-Werror', '-O2', '-ggdb' ]
ldflags = []

simNetConfig = []

b = MaxRuntimeBuilder(maxfiles=MAXFILES)
s = MaxCompilerSim(dfeModel="MAIA")
e = Executor(logPrefix="[%s] " % (target))

def build():
	compile()
	link()

def compile():
	b.slicCompile()
	b.compile(sources, extra_cflags=cflags)

def link():
	b.link(sources, target, extra_ldflags=ldflags)

def clean():
	b.clean()

def start_sim():
	s.start(netConfig=simNetConfig)

def stop_sim():
	s.stop()

def restart_sim():
	stop_sim()
	start_sim()

def run_sim():
	build()
	restart_sim()
	e.execCommand([ "./" + target ])
	e.wait()
#	s.stop()
	

def maxdebug():
	s.maxdebug(MAXFILES)

if __name__ == '__main__':
	fabricate.main()
