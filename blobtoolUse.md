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
 13. For each module needed by your code, run: ```./pip install module_name``` in the python/bin directory.
 
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

##### Install blobtools
 14. Return to your working directory, outside of python. ```ls``` should result in the following:
```
python  Python-2.7.14  Python-2.7.14.tgz
```
At this point we have installed all of the required dependencies for bloobtools, we just need to put them in the right place.
 
 15. Clone blobtools to your directory ```git clone https://github.com/DRL/blobtools.git```  
 16. ```ls``` should now result in: 
```
blobtools  python  Python-2.7.14  Python-2.7.14.tgz 
``` 

--------------------------------------------------------------------------------------------------------
   ###### Install samtools-1.5 in blobtools
 17. Import the tar file from Samtools to your blobtools directory.
   ```
   wget https://github.com/samtools/samtools/releases/download/1.5/samtools-1.5.tar.bz2
   ```
 18. Untar it ```tar -xvf samtools-1.5.tar.bz2```
 19. Enter the new directory ```cd samtools-1.5```
 20. Reset the path ```export PATH=$(pwd)/../samtools:$PATH``` 
 21. Configure with ```./configure --disable-lzma --prefix=$(pwd)/../samtools```
 
Caution: CRAM format may use LZMA2 compression, which was discluded from this configuration. 
 22. Make and install samtools.
   ```
   make
   make install
   ```
   ERROR OCCURS ON SECOND STEP ABOVE ^
--------------------------------------------------------------------------------------------------------
 23. Move up a directory ```cd ..```
 24. Reset your path to the correct location ```export PATH=$(pwd):$PATH```
 25. Install blobtools ```./install``` 
--------------------------------------------------------------------------------------------------------

##### Create a tarball of the python installation 
 26. Run the following command to create your own tarball of the installation: ```tar -czvf python.tar.gz python/```
 27. Exit the Interactive job and return to the submit server: ```exit```
 --------------------------------------------------------------------------------------------------------
##### Sample script:
We now have a python.tar.gz file that contains our entire Python installation. In order to use this installation in our HTCondor jobs, we will need to write a script that unpacks our Python installation and then runs our Python code. We will use this script as as the executable of our HTCondor submit file.

A sample script appears below. After the first line, the lines starting with hash marks are comments . You should replace "myscript.py" with the name of the script you would like to run.
```
#!/bin/bash

# untar your Python installation
tar -xzf python.tar.gz
# make sure the script will use your Python installation, 
# and the working directory as it's home location
export PATH=$(pwd)/python/bin:$PATH
mkdir home
export HOME=$(pwd)/home
# run your script
python my_script.py
```
Finally, give your script executable permissions by running: ```chmod +x run_python.sh```

-----------------------------------------------------------------------------------------------------------------------

### ERROR:
```
[bweibel@submit-3 samtools-1.5]$ make install
mkdir -p -m 755 /home/bweibel/python/bin/samtools-1.5/../../../blobtools/samtools/bin /home/bweibel/python/bin/samtools-1.5/../../../blobtools/samtools/bin /home/bweibel/python/bin/samtools-1.5/../../../blobtools/samtools/share/man/man1
install -p samtools /home/bweibel/python/bin/samtools-1.5/../../../blobtools/samtools/bin
[+] Checking dependencies...
    [+] [wget] /usr/bin/wget
    [+] [tar] /bin/tar
    [+] [pip] pip
    [+] [python2.7] /home/bweibel/python/bin/python
[+] Installing python dependencies...
/home/bweibel/python/bin/python: can't open file 'setup.py': [Errno 2] No such file or directory
FAIL.
[X] - Python dependencies could not be installed. Make sure you are using Python 2.7 and have a functional installation of pip.
install -p misc/ace2sam misc/maq2sam-long misc/maq2sam-short misc/md5fa misc/md5sum-lite misc/wgsim /home/bweibel/python/bin/samtools-1.5/../../../blobtools/samtools/bin
[+] Checking dependencies...
    [+] [wget] /usr/bin/wget
    [+] [tar] /bin/tar
    [+] [pip] pip
    [+] [python2.7] /home/bweibel/python/bin/python
[+] Installing python dependencies...
/home/bweibel/python/bin/python: can't open file 'setup.py': [Errno 2] No such file or directory
FAIL.
[X] - Python dependencies could not be installed. Make sure you are using Python 2.7 and have a functional installation of pip.
install -p misc/blast2sam.pl misc/bowtie2sam.pl misc/export2sam.pl misc/interpolate_sam.pl misc/novo2sam.pl misc/plot-bamstats misc/psl2sam.pl misc/sam2vcf.pl misc/samtools.pl misc/seq_cache_populate.pl misc/soap2sam.pl misc/varfilter.py misc/wgsim_eval.pl misc/zoom2sam.pl /home/bweibel/python/bin/samtools-1.5/../../../blobtools/samtools/bin
[+] Checking dependencies...
    [+] [wget] /usr/bin/wget
    [+] [tar] /bin/tar
    [+] [pip] pip
    [+] [python2.7] /home/bweibel/python/bin/python
[+] Installing python dependencies...
/home/bweibel/python/bin/python: can't open file 'setup.py': [Errno 2] No such file or directory
FAIL.
[X] - Python dependencies could not be installed. Make sure you are using Python 2.7 and have a functional installation of pip.
install -p -m 644 samtools.1 misc/wgsim.1 /home/bweibel/python/bin/samtools-1.5/../../../blobtools/samtools/share/man/man1
[+] Checking dependencies...
    [+] [wget] /usr/bin/wget
    [+] [tar] /bin/tar
    [+] [pip] pip
    [+] [python2.7] /home/bweibel/python/bin/python
[+] Installing python dependencies...
/home/bweibel/python/bin/python: can't open file 'setup.py': [Errno 2] No such file or directory
FAIL.
[X] - Python dependencies could not be installed. Make sure you are using Python 2.7 and have a functional installation of pip.

```
