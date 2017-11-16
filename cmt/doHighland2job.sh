filelist=$(readlink -f $1)

#usage ./doHighland2job.sh [filelist] [exe] [id] 

#RunNumuCCAnalysis.exe
exe=$2

id=$3

echo filelist $filelist
echo exe $exe
echo id $id

###############################################

dir0=$id/job

mkdir -p $dir0

echo inputing...

nf=$(wc $filelist | awk '{print $1}')
echo nf $nf
#test
#nf=3

####################################################

cd $dir0

echo $filelist  > inlog

for ii in $(seq -w 1 $nf)
do
    ff=$(head -n $ii $filelist | tail -n 1)
    jobfile=$id\n$ii.sh

    cat > $jobfile <<EOF

source $(readlink -f /home/stewartt/t2ksoft_highland_v2r15.sh)
#Minimal setup is due to an issue that occurs where Psyche only reads in the first 50 PATHs
source $(readlink -f /data/t2k/stewartt/T2K/antiNumuCC1PiMinusAnalysis/v0r0/cmt/minimalsetup.sh)
cd $(pwd)

date

$exe -o ../AnaOut_sub$ii\.root $ff

date

EOF

    chmod +x $jobfile

    echo "Pre-submission: $jobfile"

    qsub -l cput=02:00:00 -N $jobfile  $jobfile
    qsubresult=$?
    echo "Sub result: $qsubresult"
    while ! [ $qsubresult  -eq 0 ];do
        sleep 4
        qsub -l cput=02:00:00 -N $jobfile  $jobfile
        qsubresult=$?
    done

    echo "Submitted: $jobfile"

    nq=$(qstat | grep $(whoami) | grep Q |wc | awk '{print $1}' )
    while [ $nq -gt 10 ]; do
        echo "Pausing for 10"
        sleep 10
        nq=$(qstat | grep $(whoami) | grep Q |wc | awk '{print $1}' )
    done

    ii=$( expr $ii + 1 )

done

echo done $ii

