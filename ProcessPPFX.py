#!/usr/bin/env python
##################################################
# ppfx grid job submitter
# 2019.07 -- L. Aliaga
##################################################
import os, optparse, random, shutil, tarfile, sys
import subprocess, string

CACHE_PNFS_AREA = "/pnfs/nova/scratch/users/{USER}/grid_cache/".format(USER = os.getenv("USER"))
PWD = os.getenv("PWD")

##################################################
# Job Defaults
##################################################
N_JOBS             = 1
OUTDIR             = "/pnfs/nova/persistent/users/{USER}/ppfx/test/".format(USER = os.getenv("USER"))
#INPUT_OPTIONS      = "{0}/scripts/inputs_default.xml".format(PWD)
INPUT_OPTIONS      = "scripts/inputs_default.xml"
IDET               = "3"
INDIR              = "/pnfs/nova/data/flux/g4numi/v6r1"
TARFILE_NAME       = "local_install.tar.gz"
BEAMCONFIG         = "me000z200i"

##################################################

def main():
  options = get_options()
  
  cache_folder = CACHE_PNFS_AREA + str(random.randint(10000,99999)) + "/"
  os.makedirs(cache_folder)
  
  os.makedirs(options.outdir)

  print "\nTarring up local area..."
  make_tarfile(TARFILE_NAME, ".")

  shutil.move(TARFILE_NAME,    cache_folder) 
  shutil.copy("ppfx_job.sh", cache_folder)
  
  print "\nTarball of local area:", cache_folder + TARFILE_NAME

  logfile = options.outdir + "/ppfx_{BEAMCONFIG}_\$PROCESS.log".format(BEAMCONFIG = options.beamconfig)                                                                               
  
  print "\nOutput logfile(s):",logfile

  submit_command = ("jobsub_submit {GRID} {MEMORY} -N {NJOBS} -dPPFX {OUTDIR} "
      "-G nova "
      "-e BEAMCONFIG={BEAMCONFIG} " 
      "-e IN_DIR={IN_DIR} " 
      "-e INPUT_OPTIONS={INPUT_OPTIONS} " 
      "-e IDET={IDET} " 
      "-f {TARFILE} "
      "-L {LOGFILE} "
      "file://{CACHE}/ppfx_job.sh".format(
      GRID       = ("--OS=SL7 -g "
                    "--resource-provides=usage_model=DEDICATED,OPPORTUNISTIC "
                    "--role=Analysis "),
      MEMORY     = "--memory 200MB ",
      NJOBS      = options.n_jobs,
      OUTDIR     = options.outdir,
      BEAMCONFIG = options.beamconfig,
      IN_DIR     = INDIR,
      INPUT_OPTIONS = options.input_options,
      IDET       =  options.idet,
      TARFILE    = cache_folder + TARFILE_NAME,
      LOGFILE    = logfile,
      CACHE      = cache_folder)
  )

  #Ship it
  print "\nSubmitting to grid:\n"+submit_command+"\n"
  status = subprocess.call(submit_command, shell=True)

def get_options():
  parser       = optparse.OptionParser(usage="usage: %prog [options]")
  grid_group   = optparse.OptionGroup(parser, "Grid Options")
  
  grid_group.add_option("--outdir",
                        default = OUTDIR,
                        help    = "Output flux histograms location. Default = %default.")
  
  grid_group.add_option("--n_jobs",
                        default = N_JOBS,
                        help = "Number of g4numi jobs. Default = %default.")
  
  beam_group   = optparse.OptionGroup(parser, "Beam Options")
  
  beam_group.add_option('--beamconfig', default = BEAMCONFIG, 
                        help="Select the neutrino beam: me000z200i, me000z-200i me000z000i.Default = %default.")
  
  run_group   = optparse.OptionGroup(parser, "Run Options")
  
  run_group.add_option("--input_options",
                       default = INPUT_OPTIONS,
                       help    = "PPFX input: number of universes, MIPP on/off, etc. Default = %default.")
  
  run_group.add_option("--idet",
                       default = IDET,
                       help    = "Index of the detector to be used (see at location.name in dkmetaTree->Show(0)). Default = %default.")
  
  parser.add_option_group(grid_group)
  parser.add_option_group(beam_group)
  parser.add_option_group(run_group)

  options, remainder = parser.parse_args()

  return options

def make_tarfile(output_filename, source_dir):
  tar = tarfile.open(output_filename, "w:gz")
  for i in os.listdir(source_dir):
    tar.add(i)
  tar.close()

if __name__ == "__main__":
  sys.exit(main())
