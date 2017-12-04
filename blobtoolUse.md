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
 5. Configure the file to the python install file you created ``./configure --prefix=$(pwd)/../python``
 6. Set path, make and install Python 2.7.14
 ```
 export PATH=$(pwd)/../python:$PATH
 make
 make install
 ```
--------------------------------------------------------------------------------------------------------
##### Ensure Proper Installation
 7. Move up a directory ```cd ..``` 
 8. Check contents of python directory, should look accordingly ```ls python```
```
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
 11. Enter python/bin directory ```cd python/bin```
 12. Transfer the latest verion of pip by executing the following in your python/bin directory:
 ```
 wget https://bootstrap.pypa.io/get-pip.py
 ``` 
 13. Install pip ```python get-pip.py```
--------------------------------------------------------------------------------------------------------
##### Install setup tools
 14. Transfer the tar file:
 ```
 wget https://pypi.python.org/packages/34/da/2a7bb4f6159fefb67a78591cbb2b0bf4e58fa84260a095969600bb53f4e2/bootstrap-py-0.6.0.tar.gz
 ```
 15. Untar the file ```tar xvzf bootstrap-py-0.6.0.tar.gz```
 16. Enter the new directory ```cd bootstrap-py-0.6.0```
 17. Install bootstrap-py ```python -m pip install bootstrap-py```
 18. Copy setup.py from bootstrap-py-0.6.0 to python's bin:
```
cd ..
cp bootstrap-py-0.6.0/setup.py .
```

--------------------------------------------------------------------------------------------------------
##### python/bin Directory
directory should look as follows:
```
2to3                       pip               python2-config  rst2latex.pyc           rst2s5.pyc            share
bin                        pip2              python-config   rst2man.py              rst2xetex.py          smtpd.py
bootstrap-py-0.6.0         pip2.7            python.tar.gz   rst2man.pyc             rst2xetex.pyc         sphinx-apidoc
bootstrap-py-0.6.0.tar.gz  pybabel           rst2html4.py    rst2odt_prepstyles.py   rst2xml.py            sphinx-autogen
chardetect                 pydoc             rst2html4.pyc   rst2odt_prepstyles.pyc  rst2xml.pyc           sphinx-build
easy_install               pygmentize        rst2html5.py    rst2odt.py              rstpep2html.py        sphinx-quickstart
easy_install-2.7           python            rst2html5.pyc   rst2odt.pyc             rstpep2html.pyc       wheel
f2py                       python2           rst2html.py     rst2pseudoxml.py        samtools-1.5
get-pip.py                 python2.7         rst2html.pyc    rst2pseudoxml.pyc       samtools-1.5.tar.bz2
idle                       python2.7-config  rst2latex.py    rst2s5.py               setup.py
```

--------------------------------------------------------------------------------------------------------

##### Install blobtools
 19. Return to your working directory, outside of python. ```ls``` should result in the following:
```
python  Python-2.7.14  Python-2.7.14.tgz
```
At this point we have installed all of the required dependencies for bloobtools, we just need to put them in the right place.
 
 20. Clone blobtools to your directory ```git clone https://github.com/DRL/blobtools.git```  
 21. ```ls``` should now result in: 
```
blobtools  python  Python-2.7.14  Python-2.7.14.tgz 
``` 

--------------------------------------------------------------------------------------------------------
   ###### Install samtools-1.5 in blobtools
 22. Import the tar file from Samtools to your blobtools directory.
```
cd blobtools
wget https://github.com/samtools/samtools/releases/download/1.5/samtools-1.5.tar.bz2
```
 23. Untar it ```tar -xvf samtools-1.5.tar.bz2```
 24. Enter the install directory ```cd samtools-1.5```
 25. Configure with ```./configure --disable-lzma --prefix=$(pwd)/../samtools```
 
Caution: CRAM format may use LZMA2 compression, which was discluded from this configuration. 

 26. Make and install samtools.
```
make
make install
```

--------------------------------------------------------------------------------------------------------
 27. Move up a directory ```cd ..```
 28. Enter the install executable ```vim install```
 29. Scroll down to line 127.
 30. Comment the lines that download and install samtools (press i to edit), the code should follow: 
```
# install samtools
samtools_tar=$DIR/samtools-1.5.tar.bz2
#if [ ! -f "$samtools_tar" ]; then
#    download_samtools
#fi
#install_samtools
```
 31. Write and quit to return to your blobtools directory ```Esc + wq + Enter```
 32. Install blobtools ```./install``` 
--------------------------------------------------------------------------------------------------------

##### Create a submit file
 33. Create the file ```blobtools_test.sub```
 34. Enter it ```vim blobtools_test.sub``` and paste the following
 ```
 universe                = vanilla
 executable              = blobtools_test.sh
 
 output                  = blobtools_test.out
 log                     = blobtools_test.log
 error                   = blobtools_test.err
 
 request_memory          = 10 GB
 request_disc            = 10 GB
 
 request_cpus            = 1
 
 transfer_input_files    = python-2.7.tar.gz, samtools-1.5.tar.gz
 should_transfer_files   = YES
 when_to_transfer_files  = ON_EXIT
 
 requirements            = (Targes.HasGuster == true)
 
 Getenv                  = TRUE
 queue
 ```


##### Create a tarball of the python installation 
 33. Run the following command to create your own tarball of the installation: ```tar -czvf python.tar.gz python/```
 34. Exit the Interactive job and return to the submit server: ```exit```
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
