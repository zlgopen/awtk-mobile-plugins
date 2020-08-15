import os
import scripts.app_helper as app

helper = app.Helper(ARGUMENTS);
APP_CCFLAGS=' -DWITHOUT_PLUGINS '
helper.add_libs(['plugins']).add_ccflags(APP_CCFLAGS).call(DefaultEnvironment)

SConscript(['demos/SConscript', 'src/SConscript'])
