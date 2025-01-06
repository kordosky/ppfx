# ppfx

This is new repository for official PPFX development intended to support experiments over the long term.

It is maintained by the original authors (Leo Aliaga, Mike Kordosky) with help from others.

It was cloned on 1/13/23 from the Fermilab repository at

ssh://p-ppfx@cdcvs.fnal.gov/cvs/projects/ppfx 

as originally referenced in the Fermilab redmine link

https://cdcvs.fnal.gov/redmine/projects/ppfx/wiki/PPFX



This branch is specifically for NOvA and DUNE with NA61 60 GeV Reweighter. We have removed MIPP and introduced SVD decomposition for NA61 data.
There is a new mode: scripts/inputa_na61.xml, this will allow to switch on NA61 reweighter.
You need to carefullt specify which detector you are woring with. If NOvA, use 1 in inputs_na61.xml or for DUNE use 2 in <Exp>
The NA49 simulations are also for the Geant4 v_10.4, earlier v9.2 was used.
