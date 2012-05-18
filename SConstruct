import SCons.Script as scons
import re
import os

obj_sources = Split(
"""
src/gsp_hashtree_class.cc
src/gsp_sequence_class.cc
src/gsp_itemsets_class.cc
""")

main_sources = Split(
"""
src/gsp.cc
"""
)

def MakeEnvironment():
    env = Environment()

    env.Append(CPPPATH=['src/',       
                        '/usr/include/c++/4.6.1/',
                        '/usr/include/c++/4.6.1/x86_64-linux-gnu'])
    env.Append(CPPFLAGS=['-Werror', '-Wall', '-pedantic', '-Wno-long-long'] )

    return env

env = MakeEnvironment()

object_list = env.Object(source = obj_sources)
main_list = env.Object(source = main_sources)
gsp = env.Program('gsp', main_list, object_list)