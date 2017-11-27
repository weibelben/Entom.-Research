### Instalation and uses of blobtools on Condor(CHTC):

--- A modular command-line solution for visualisation, quality control and taxonomic partitioning of genome datasets.

REQUIREMENTS:
 - Python 2.7
 - UNIX
 - pip
--------------------------------------------------------------------------------------------------------
##### Installing Python on Condor
 1. First create a directory in which you will install Python 2.7. ```mkdir python```
 2. Transfer the latest version of Python 2.7 to your current working directory.
````
wget https://www.python.org/ftp/python/2.7.14/Python-2.7.14.tgz
````
 - This link may need to be updated to the latest version.
Updated 11/22/2017

 3. Untar the source code that was transfered. ```tar -xzf Python-2.7.14.tgz```
 4. Change directory to the new Python file ```cd Python-2.7.14```
 5. Compile the file to the python install file you created ``./configure --prefix=$(pwd)/../python``
 6. Make and install Python 2.7.14
 ```
 make
 make install
 ```
--------------------------------------------------------------------------------------------------------
##### Ensure Proper Installation
 7. Move up a directory ```cd ..``` 
 8. Check contents of python directory, should look accordingly
```
[alice@build]$ ls python 
bin  include  lib  share
```
 9. Ensure you have a python executable ```ls python/bin```,
 the output should look similar to the following:
```
2to3 idle  pydoc  python  python2  python2.7  python2.7-config  python2-config  python-config  smtpd.py
```
--------------------------------------------------------------------------------------------------------
##### Install pip
 10. Set your PATH variable to include your Python installation: 
```export PATH=$(pwd)/python/bin:$PATH ```
 11. Transfer the latest verion of pip by executing the following in your python/bin directory:
 ```
 wget https://bootstrap.pypa.io/get-pip.py
 ``` 
 12. Install pip ```python get-pip.py```
 13. For each module needed by your code, run: ```pip install module_name``` in the python/bin directory.
 
 ```
 Zach, I'm unsure what modules we will need
 ```
--------------------------------------------------------------------------------------------------------
##### python/bin Directory
directory should look as follows:
```
2to3          easy_install-2.7  idle  pip2    pydoc   python2    python2.7-config  python-config  wheel
easy_install  get-pip.py        pip   pip2.7  python  python2.7  python2-config    smtpd.py
```
along with any additional modules that were installed in step (13) above. 
--------------------------------------------------------------------------------------------------------
##### Create a tarball of the installation 
 14. Run the following command to create your own tarball of the installation: ```tar -czvf python.tar.gz python/```
 15. Exit the Interactive job and return to the submit server: ```exit```
 --------------------------------------------------------------------------------------------------------
 
