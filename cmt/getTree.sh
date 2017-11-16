grep Source seehad* | wc

if [ ! -e listofsubmerge.list ]
then
    find AnaOut_all_su* > listofsubmerge.list
fi

wc listofsubmerge.list

mydir=$(pwd)
echo $mydir
#source $(readlink -f /home/coplowe/GitHub/t2k/t2k-feb16freeze.sh)

echo "Combining trees to a single .root file"
for ii in default truth header config; do
echo "Producing $ii combined tree script"
rfile=$ii\Tree.root
cat > job/$ii\merge.sh <<EOF
source $(readlink -f /home/stewartt/t2ksoft.sh)
cd $mydir
echo "Merging $ii trees."
root -b -l <<EOF
#include "/home/coplowe/GitHub/transtools/NeutrinoTools.h"
ch=NeutrinoTools::InputFiles("listofsubmerge.list","$ii")
ch->Merge("$rfile")
EOF;
echo "Complete"
EOF
done

cd $mydir/job
for ii in default truth header config; do
qsub -l cput=02:00:00 -N $ii\merge.sh ./$ii\merge.sh
done

#root -b -l <<EOF
# #include "/home/coplowe/GitHub/transtools/NeutrinoTools.h"
#ch=NeutrinoTools::InputFiles("listofsubmerge.list","NRooTrackerVtx")
#ch->Merge("NRooTrackerVtxTree.root")
#EOF
#hadd allTree.root {default,truth,header,config,NRooTrackerVtx}Tree.root
#if [ -f all_systTree.root ]; then
#    echo "Combining trees (including all_syst)"
#    hadd allTree.root {default,truth,header,config,all_syst}Tree.root
#else
#    echo "Combining trees (excluding all_syst)"
#    hadd allTree.root {default,truth,header,config}Tree.root
#fi
#hadd allTree.root {default,truth,header}Tree.root
#echo "Done."
#echo done
