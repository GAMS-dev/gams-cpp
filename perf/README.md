# GAMS Cpp Performance Tests

This folder contains all the performance test related files.


## Synopsis

'testrunner' to run all tests (auto mode).

'testrunner [testname]+' to run one or more tests, seperated by spaces (manual mode).

'testrunner help' to see a list of available tests.


## Usage

Navigate to the folder were "testrunner" is located. Execute 'testrunner' to run
all tests. Specifying no arguments uses QTest to execute, setup, and cleanup all 
tests found.
If you want to run one or more, but not all tests you can hand them over as arguments. Each 
test is then executed in the order specified. The function 'initTestCase()' will
be executed before running actual tests, if available.

## Important

Support for automatic execution of functions 'testname_data()', cleanupTestCase(),
init(), or cleanup() is currently not implemented for manual mode. Run auto mode
if these are needed.

