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
 31. Write and quit to return to your blobtools directory ```Esc + :wq + Enter```
 32. Install blobtools ```./install``` 
 --------------------------------------------------------------------------------------------------------
##### Tar blobtools and python
 33. Return to your working directory ```cd ..```
 34. Run the following command to create your own tarball of the installations:
```
tar -czvf blobtools.tar.gz blobtools/
tar -czvf python-2.7.tar.gz python/
```

--------------------------------------------------------------------------------------------------------
##### Create a submit file
 35. Create and enter the file ```vim blobtools_test.sub``` and paste the following (Press i to edit)
```
universe                = vanilla
executable              = blobtools_script.sh

output                  = test_output.txt
#log                     = blobtools_test.log
#error                   = blobtools_test.err

request_memory          = 10 GB
request_disk            = 10 GB

request_cpus            = 1

transfer_input_files    = python-2.7.tar.gz, blobtools-1.5.tar.gz
should_transfer_files   = YES
when_to_transfer_output = ON_EXIT

#requirements           = (Targes.HasGuster == true)

Getenv                  =TRUE
queue
``` 
  36. Write and quit ```Esc + :wq + Enter```  
--------------------------------------------------------------------------------------------------------
 
 ##### Interactively test
 37. Submit the test ```condor_submit -i blobtools_test.sub```
 38. Once your job is processed, untar blobtools and python
```
tar xvzf blobtools-1.5.tar.gz
tar xvzf python-2.7.tar.gz
```
 39. Export the new path ```export PATH=$(pwd)/python/bin:$PATH```
 40. Test blobtools functions ```./blobtools -h```
 
You are successful if no errors are thrown and no interrupts occur. 

 41. Exit interactive mode ```exit```
 
--------------------------------------------------------------------------------------------------------
##### Retrieving bowtie
 42. Make a bowtie directory ```mkdir bowtie```
 43. Enter the new directory ```cd bowtie```
 44. Retrieve bowtie2-2.3.3.1 
```
wget https://sourceforge.net/projects/bowtie-bio/files/bowtie2/2.3.3.1/bowtie2-2.3.3.1-linux-x86_64.zip/download
```
 45. Unzip the installation ```unzip download```
 46. Return to your working directory and tar bowtie
```
cd ..
tar -czvf bowtie.tar.gz bowtie2/
```

--------------------------------------------------------------------------------------------------------
 ##### Create a submit file and Interactively test
  47. Create and enter the file ```vim bowtie_test.sub``` and paste the following (Press i to edit)
```
universe                = vanilla

output                  = test_output.txt

request_memory          = 10 GB
request_disk            = 10 GB

request_cpus            = 1

transfer_input_files    = bowtie.tar.gz
should_transfer_files   = YES
when_to_transfer_output = ON_EXIT

Getenv                  = TRUE
queue
 ``` 
 48. Write and quit ```Esc + :wq + Enter```  
 49. Submit the test ```condor_submit -i bowtie_test.sub```
 50. Once your job is processed, untar bowtie
```
tar xvzf bowtie.tar.gz
```
 51. Export the new path ```export BT2_HOME=$(pwd)/bowtie/bowtie2-2.3.3.1-linux-x86_64```
 52. Create a temporary directory in bowtie and enter it:
```
cd bowtie
mkdir temp
cd temp
``` 
 53. Run the following test commands
```
$BT2_HOME/bowtie2-build $BT2_HOME/example/reference/lambda_virus.fa lambda_virus
$BT2_HOME/bowtie2 -x lambda_virus -U $BT2_HOME/example/reads/reads_1.fq -S eg1.sam
```
You are successful if no errors are thrown, no interrupts occur, and the following is displayed.
```
10000 reads; of these:
  10000 (100.00%) were unpaired; of these:
    596 (5.96%) aligned 0 times
    9404 (94.04%) aligned exactly 1 time
    0 (0.00%) aligned >1 times
94.04% overall alignment rate
```
 54. To align paired-end reads included with Bowtie 2, stay in the same directory and run
```
$BT2_HOME/bowtie2 -x lambda_virus -1 $BT2_HOME/example/reads/reads_1.fq -2 $BT2_HOME/example/reads/reads_2.fq -S eg2.sam
```
 55. To use local alignment to align some longer reads included with Bowtie 2, stay in the same directory and run
```
$BT2_HOME/bowtie2 --local -x lambda_virus -U $BT2_HOME/example/reads/longreads.fq -S eg3.sam
```
 56. Run the paired-end example 
```
$BT2_HOME/bowtie2 -x $BT2_HOME/example/index/lambda_virus -1 $BT2_HOME/example/reads/reads_1.fq -2 $BT2_HOME/example/reads/reads_2.fq -S eg2.sam
```

This all went succesfully if there are no errors or interrupts. Fianl output looks similar to the output of (53)

 57. Exit interactive mode ```exit```
--------------------------------------------------------------------------------------------------------
##### Test bowtie with a submission script
 58. Create a test submission ```vim test_bowtie.sub``` and paste in it the following
```
universe                = vanilla
executable              = test_bowtie.sh

output                  = test_bowtie_output.txt

request_memory          = 10 GB
request_disk            = 10 GB

request_cpus            = 1

transfer_input_files    = bowtie.tar.gz
should_transfer_files   = YES
when_to_transfer_output = ON_EXIT

#requirements           = (Targes.HasGuster == true)

Getenv                  = TRUE
queue
```
 55. Create a script for execution by the submission ```vim test_bowtie.sh``` and paste in it
```
#!/bin/bash

# untar bowtie
tar xvzf bowtie.tar.gz

# test bowtie
export BT2_HOME=$(pwd)/bowtie/bowtie2-2.3.3.1-linux-x86_6

cd bowtie
mkdir temp
cd temp

echo 'inside temp'

$BT2_HOME/bowtie2-build $BT2_HOME/example/reference/lambda_virus.fa lambda_virus
$BT2_HOME/bowtie2 -x lambda_virus -U $BT2_HOME/example/reads/reads_1.fq -S eg1.sam
$BT2_HOME/bowtie2 -x lambda_virus -1 $BT2_HOME/example/reads/reads_1.fq -2 $BT2_HOME/example/reads/reads_2.fq -S eg2.sam
$BT2_HOME/bowtie2 --local -x lambda_virus -U $BT2_HOME/example/reads/longreads.fq -S eg3.sam
$BT2_HOME/bowtie2 -x $BT2_HOME/example/index/lambda_virus -1 $BT2_HOME/example/reads/reads_1.fq -2 $BT2_HOME/example/reads/reads_2.fq -S eg2.sam

echo 'now tarring temp directory'

#tar temp
cd ..
tar -czvf test_bowtie_temp.tar.gz temp/
cd ..
cp bowtie/test_bowtie_temp.tar.gz .

cp test_bowtie_temp.tar.gz /home/user # CHANGE TO YOUR HOME DIR
cp test_bowtie_output.txt /home/user # CHANGE TO YOUR HOME DIR
```
 59. Submit the test ```condor_submit test_bowtie.sub```
 60. When the job is completed, open the output file ```vim test_output.txt```
You are successfull if not errors or interrupts occur and the file contains lines similar to
```bowtie/bowtie2-2.3.3.1-linux-x86_64/scripts/``` for the bowtie tests.
 61. Untar the temp file ```tar xvzf test_bowtie_temp.tar.gz``` and use to your pleasure.

--------------------------------------------------------------------------------------------------------
 61. After testing, find proper command options for the installed programs using
```
./python -h
./bowtie2 -h
```
in their respective directories or bin directories. 
