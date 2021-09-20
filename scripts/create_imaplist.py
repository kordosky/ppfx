import os,sys
import ROOT


status=0
while status==0:
    status= ROOT.gSystem.Load("libdk.so")

print("STATUS ",status)

input_rfile = str(sys.argv[1])

dk2nu = ROOT.bsim.Dk2Nu

_file = ROOT.TFile(input_rfile)

_dk2nu = _file.Get("dk2nuTree")

nentries = _dk2nu.GetEntriesFast()
print("Total Entries ",nentries)


vol_list={}
for i in range(0,nentries):
    _dk2nu.GetEntry(i)
    anc_size = _dk2nu.dk2nu.ancestor.size()
    for j in range(0,anc_size):
        mat_name = _dk2nu.dk2nu.ancestor[j].imat
        vol_name = _dk2nu.dk2nu.ancestor[j].ivol
        vol_list[vol_name]=mat_name



#print(vol_list)
volume=[]
material=[]

for vol in vol_list:
    print(vol,vol_list[vol])
    material_name = vol_list[vol]
    volume_name = vol
    if material_name=="Target":
        material_name=="Carbon"
    if material_name=="TitaniumG5":
        material_name=="Titanium"
    #print (volume_name,"  ",material_name)
    volume.append(volume_name)
    material.append(material_name)

print (len(volume))

print(volume)

print("*********************************")

print(material)
