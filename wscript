#!/usr/bin/python

VERSION='0.0.1'
APPNAME='Luppp'
top = './'
out = './.build'

def options(ctx):
  ctx.tool_options('compiler_cxx')
  
  ctx.add_option('--debug', action='store', default=True, help='Enable debuggable build')

def init(null):
  #print '\nInitializing Luppp...'
  pass

def configure(ctx):
  
  ctx.check_tool ('compiler_cxx')
  
  ctx.env.CXXFLAGS = ['-pg','-g','-Wall','-Wextra']
  
  #   Engine Depends
  ctx.check_cfg	(package='jack',at_least_version='0.118',args='--cflags --libs',uselib_store='JACK')
  ctx.check_cfg	(package='sndfile',args='--cflags --libs',uselib_store='SNDFILE')
  ctx.check_cfg	(package='gtkmm-2.4',at_least_version='2.0.0',args='--cflags --libs',uselib_store='GTKMM')
  
  # Check for headers:
  #ctx.check(header_name="ladspa.h",mandatory=False,uselib_store='LADSPA.H')
  

def build(ctx):
  print '\nBuilding the sources to objects...'
  
  #     ENGINE
  engineList=['src/top.cpp',
              'src/rtqueue.cpp',
              'src/ladspahost.cpp',
              'src/fileaudiosource.cpp',
              'src/mixer.cpp',
              'src/statestore.cpp',
              'src/engineevent.cpp',
              'src/audiotrack.cpp',
              'src/jackclient.cpp']
  
  guiList = [ 'src/g_window.cpp' ]
  
  engineDepends = 'JACK SNDFILE GTKMM'
  
  print 'Building ENGINE'
  buildList = engineList + guiList
  dependList = engineDepends
  
  print 'Depends list:',dependList
  
  lib	 = ctx.new_task_gen(
    features	= 'cxx cxxstlib',
    source		= buildList,
    target		= 'engine_gui',
    uselib		= engineDepends,
    export_includes = '.')
  
  
  main			= ctx.new_task_gen(
    features	= 'cxx cprogram',
    source		= 'src/main.cpp',
    use       = 'engine_gui',
    uselib    = engineDepends,
    target		= 'luppp' )
