import os
import sys
import platform

sys.path.insert(0, '../awtk/')
import awtk_config as awtk

APP_ROOT    = os.path.normpath(os.getcwd())
APP_BIN_DIR = os.path.join(APP_ROOT, 'bin')
APP_LIB_DIR = os.path.join(APP_ROOT, 'lib')
RES_ROOT    = awtk.TK_DEMO_ROOT.replace("\\", "\\\\")

os.environ['APP_ROOT'] = APP_ROOT;
os.environ['BIN_DIR'] = APP_BIN_DIR;
os.environ['LIB_DIR'] = APP_LIB_DIR;

APP_LINKFLAGS = '';
APP_LIBS = ['plugins']
APP_LIBPATH = [APP_LIB_DIR]
APP_CCFLAGS = ' -DWITHOUT_PLUGINS '
APP_CPPPATH = [os.path.join(APP_ROOT, 'src')]

DefaultEnvironment(
  CPPPATH   = APP_CPPPATH + awtk.CPPPATH,
  LINKFLAGS = APP_LINKFLAGS + awtk.LINKFLAGS,
  LIBS      = APP_LIBS + awtk.LIBS,
  LIBPATH   = APP_LIBPATH + awtk.LIBPATH,
  CCFLAGS   = APP_CCFLAGS + awtk.CCFLAGS, 
  OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS)

SConscript(['demos/SConscript', 'src/SConscript'])

