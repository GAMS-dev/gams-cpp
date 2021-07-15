$tItLE 'Runs C++ API examples (except for the expert level examples)' (CPPEX3,SEQ=56)


$ontext
Contributor: Alexander Fust, July 2017
$offtext

$set VARS

$set cmakeFlags
$if %system.buildcode% == VS8 $set cmakeFlags '-G "Visual Studio 12 2013" -config Release -DVSVERSION:STRING=vs2013'
$if %system.buildcode% == WEI $set cmakeFlags '-G "Visual Studio 16 2019" -config Release -DVSVERSION:STRING=vs2019'

$if %system.buildcode% == VS8 $prefixPath %gams.sysdir%apifiles\C++\lib\vs2013
$if %system.buildcode% == WEI $prefixPath %gams.sysdir%apifiles\C++\lib\vs2017
$if %system.buildcode% == DEG $setenv DYLD_LIBRARY_PATH %gams.sysdir%apifiles/C++/lib:%sysenv.DYLD_LIBRARY_PATH%
$if %system.buildcode% == LEG $setenv LD_LIBRARY_PATH %gams.sysdir%apifiles/C++/lib:%sysenv.LD_LIBRARY_PATH%
$if %system.buildcode% == LEG $set VARS CC=gcc CXX=g++

* build all examples
$call %VARS% cmake %cmakeFlags% "%gams.sysdir%apifiles%system.dirsep%C++"
$ifE errorLevel<>0 $abort 'Problem prepering cmake build'

$ifthen %system.buildcode% == VS8 $call msbuild.exe examples.sln /p:Configuration=Release
$elseif %system.buildcode% == WEI $call msbuild.exe examples.sln /p:Configuration=Release
$else $call make
$endif
$ifE errorLevel<>0 $abort 'Problem compiling C++ API examples'

$set callDir
$if %system.buildcode% == VS8 $set callDir Release%system.dirsep%
$if %system.buildcode% == WEI $set callDir Release%system.dirsep%

* execute examples
$call =.%system.dirsep%transportGDX%system.dirsep%%callDir%transportGDX "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transportGDX'

$call =.%system.dirsep%transport1%system.dirsep%%callDir%transport1 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport1'

$call =.%system.dirsep%transport2%system.dirsep%%callDir%transport2 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport2'

$call =.%system.dirsep%transport3%system.dirsep%%callDir%transport3 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport3'

$call =.%system.dirsep%transport4%system.dirsep%%callDir%transport4 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport4'

$call =.%system.dirsep%transport5%system.dirsep%%callDir%transport5 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport5'

$call =.%system.dirsep%transport6%system.dirsep%%callDir%transport6 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport6'

$call =.%system.dirsep%transport7%system.dirsep%%callDir%transport7 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport7'

$call =.%system.dirsep%transport8%system.dirsep%%callDir%transport8 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport8'

* The following two require Qt to be installed
* $call =.%system.dirsep%transport9%system.dirsep%%callDir%transport9 "%gams.sysdir%."
* $ifE errorLevel<>0 $abort 'Problem running example transport9'

* $call =.%system.dirsep%transport10%system.dirsep%%callDir%transport10 "%gams.sysdir%."
* $ifE errorLevel<>0 $abort 'Problem running example transport10'

$call =.%system.dirsep%transport11%system.dirsep%%callDir%transport11 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport11'

$call =.%system.dirsep%transport12%system.dirsep%%callDir%transport12 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport12'

$call =.%system.dirsep%transport13%system.dirsep%%callDir%transport13 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport13'

$call =.%system.dirsep%transport14%system.dirsep%%callDir%transport14 "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example transport14'

$call =.%system.dirsep%domainchecking%system.dirsep%%callDir%domainchecking "%gams.sysdir%."
$ifE errorLevel<>0 $abort 'Problem running example domainchecking'

$if not set demosize $call =.%system.dirsep%warehouse%system.dirsep%%callDir%warehouse "%gams.sysdir%."
$if not set demosize $ifE errorLevel<>0 $abort 'Problem running example warehouse'

* In some cases we can't remove the direcory because the (terminated) example programs prevent deleting the directory.
$eval sleepSome sleep(5)
