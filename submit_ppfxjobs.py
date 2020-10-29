import os,random,shutil,tarfile,sys
import subprocess,string
from optparse import OptionParser
import random,time,platform


##################AUTHOR AMIT BASHYAL (amitbashyal@gmail.com) ##################
#USAGE
#THIS SCRIPT IS TO GENERATE THE PPFX FILES FROM ALREADY EXISTING DK2NU FILES
# ls path_to_dk2nufiles >file_list.txt
# python submit_ppfxjobs.py -t file_list.txt -n detector_index
#END USAGE#####################################################



def make_tarfile(output_filename, source_dir):
  tar = tarfile.open(output_filename, "w:gz")
  for i in os.listdir(source_dir):
    tar.add(i)
  tar.close()



CACHE_PNFS_AREA = "/pnfs/dune/scratch/users/{USER}/grid_cache/".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                               USER = os.getenv("USER"))
PWD = os.getenv("PWD")
tarfilename = "local_install.tar.gz"
#start by creating a scratch area to send file
cache_folder = CACHE_PNFS_AREA + str(random.randint(10000,99999)) + "/"
os.makedirs(cache_folder)

#write a wrapper file on the fly for the job submission.....
ppfx_dir = os.getcwd()
ppfx_executable = os.path.join(ppfx_dir,"bin/doReweight_dk2nu_numix")

if not os.path.exists(ppfx_executable):
    print "ERROR: I cannot find a ppfx executable at "+ppfx_executable
    sys.exit()


MODE="OPT" #since no one uses "REF"
ppfx_setup = "setup_for_dune_cvmfs.sh "+MODE
ppfx_xml = ppfx_xml =ppfx_dir+"/scripts/inputs_default.xml"

if not os.path.exists(ppfx_xml):
    print "ERROR: I cannot find the ppfx option file at "+ppfx_xml
    sys.exit()
    if not os.path.exists(ppfx_setup):
        print "ERROR: I cannot find ppfx setup script at "+ppfx_setup	
        sys.exit()


usage = "usage:%prog [options]"
user = os.getenv("USER")
parser = OptionParser(usage=usage)

parser.add_option("-o","--output_dir",dest="output_dir",
                  help="Output Directory ",default="/pnfs/dune/scratch/users/"+user+"/ppfxfiles/reweights")

parser.add_option("-t","--file_list",dest="file_list",
                  help = "Text file with a list of dk2nu input root files Should be in Same directory as teh job submission script (Only the name of the files) ",default="BLA")

parser.add_option("-i","--input_dir",dest="input_dir",
                  help="Input Directory where the dk2nu root files live ",default = "$PWD")
parser.add_option("-n","--detector_index",dest="detector_index",
                  help="Detector Index for Detector Location ",default = "0")

(options, args) = parser.parse_args()
print "Output files will be written in ",options.output_dir

if not os.path.exists(options.output_dir):
    os.makedirs(options.output_dir)


#first thing look at the list of inputfiles
#read the text file line by line
text_file =str(options.file_list)
_list = []
_textfile = open(text_file,'r')
for line in _textfile:
    _list.append(line)
#_list = os.listdir(options.input_dir)
_pre = []
index=[]
for i in range(0,len(_list)):
    if ".root" not in _list[i]:
        continue
    if "dk2nu.root" in _list[i]:
        _temp = _list[i].replace(".dk2nu.root","")
    else:
        _temp = _list[i].replace(".root","")
        
    temp = _temp.split("_")
    if len(_pre)!=0:
        #print temp,_pre
        for i in range(0,len(_pre)):
            if temp[i]!=_pre[i]:
                index.append(_pre[i])
    _pre = temp
    #print temp


__prefix = os.path.basename(_list[0])
_prefix = __prefix.replace("\n","")
if len(index)!=0:
    _temp_prefix = os.path.basename(_list[0])
    temp_prefix = _temp_prefix.replace(index[0],"")
    _prefix = temp_prefix.replace("\n","")
prefix = _prefix.replace(".root","")

print prefix,_list[0]
#sys.exit()
#sys.exit()
ntot = len(_list)
#now stuff to make the wrapper
wrapname = "wrapper_doReweight_dk2nu_"+str(int(time.time()))+".sh"
wrapdir  = CACHE_PNFS_AREA+"wrapdir/"
wrapfile = wrapdir+wrapname
temp_text = wrapname.replace(".sh",".txt")

if os.path.exists(wrapfile):
    os.remove(wrapfile)
if not os.path.exists(wrapdir):
    os.makedirs(wrapdir)

os.chmod(wrapdir,0777)
os.chmod(options.output_dir,0777)
os.chmod(cache_folder,0777)
print "\Tarring up the local area...."
make_tarfile(tarfilename,".")
shutil.move(tarfilename,    cache_folder) # temp file -> remove it from pwd
executable_file = cache_folder+tarfilename

temp_dir = "$_CONDOR_SCRATCH_DIR/"
local_tar_path = "$_CONDOR_SCRATCH_DIR/"+os.path.basename(executable_file)
print "name of executable file "+wrapfile
wrapper = open(wrapfile,'w')
wrapper.write("source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups \n\n")
wrapper.write("setup ifdhc \n\n")
wrapper.write("ifdh cp "+executable_file+" "+local_tar_path+"\n\n")
wrapper.write("tar -xzvf "+local_tar_path+" \n\n")
wrapper.write("source "+ppfx_setup+"\n\n")
#okay now we create
#wrapper.write("touch "+temp_text+"\n\n")
wrapper.write("export run_number=$((${PROCESS}+1)) \n\n")
wrapper.write('sed "${run_number}q;d" '+text_file+' >'+temp_text+'\n\n')
wrapper.write('file="$(cat '+temp_text+')"'+'\n\n')
wrapper.write("ifdh cp ${file} "+temp_dir+"\n\n")
wrapper.write("ls "+temp_dir+"*.root > file_list.txt \n\n")
output_filename = prefix+"_${run_number}.root"
print output_filename
command = "./bin/doReweight_dk2nu_numix file_list.txt "+temp_dir+output_filename+" scripts/inputs_default.xml "+str(options.detector_index)+"\n\n"
wrapper.write(command)

logfilename = output_filename.replace(".root",".log")
logfile = logfilename

#now copy the output file
wrapper.write("if [ -f "+temp_dir+output_filename+" ];\n")
wrapper.write("then\n")
wrapper.write("   ifdh cp -D "+ temp_dir+output_filename+" "+options.output_dir+"\n")
wrapper.write("fi \n")
#now copy the log file
#wrapper.write("if [ -f "+logfile+" ];\n")
#wrapper.write("then\n")
#wrapper.write("   ifdh cp -D "+logfile+" "+options.output_dir+"\n")
#wrapper.write("fi \n")
wrapper.write("echo 'DONE!!!'\n")
wrapper.close()
os.chmod(wrapfile,0777)

osver="SL%i"%int(float(platform.linux_distribution()[1]))
 
jobsub_command = "jobsub_submit.py -N "+str(ntot)+" --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime 4h --OS="+osver+" --group=dune -L "+logfile+" file://"+wrapfile;
print jobsub_command
os.system(jobsub_command)
print "Submitted ",ntot," jobs in the grid "
print "*******************************************************************************************"
print "END OF JOBSUB"
