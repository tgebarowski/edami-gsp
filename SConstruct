import SCons.Script as scons
import re
import os

obj_sources = Split(
"""
src/gsp_algorithm.cc
src/gsp_sequence_pool.cc
src/gsp_file_reader_class.cc
src/gsp_hashtree_class.cc
src/gsp_sequence_class.cc
src/gsp_itemset_class.cc
src/gsp_common.cc
""")

main_sources = Split(
"""
src/gsp.cc
"""
)

tortures = Split(
"""
torture/tortures.cc
""")

def RunProgramUnitTest(target, source, env):
    path, filename = os.path.split(source[0].abspath)
    if not env.Execute("cd %s; ./%s" % (path, filename)):
        env.Execute(scons.Touch(target))

def BoostUnitTest(env, name, source):
    try:
        libs = env["LIBS"] + ["boost_unit_test_framework"]
    except KeyError:
        libs = "boost_unit_test_framework"
    bin = env.Program(name, source, LIBS=libs)
    run = env.Command(".%s.succeeded" % str(name), name, RunProgramUnitTest)
    env.Depends(run, bin)
    return run

def MakeEnvironment():
    env = Environment()

    env.Append(CPPPATH=['src/',       
                        '/usr/include/c++/4.6.1/',
                        '/usr/include/c++/4.6.1/x86_64-linux-gnu'])
    env.Append(CPPFLAGS=['-Werror', '-Wall', '-pedantic', '-Wno-long-long' '-g'] )

    return env

def MakeTortureEnvironment(env):
    test_environment = env.Clone()
    torture_list = env.Object(source = tortures)
    return test_environment

env = MakeEnvironment()

object_list = env.Object(source = obj_sources)
main_list = env.Object(source = main_sources)
gsp = env.Program('gsp', source =main_list + object_list)

if env['PLATFORM'] != 'win32':
   BoostUnitTest(env, 'test', source=object_list + tortures)
