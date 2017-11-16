for jj in $(seq 0 9)
do
    for dd in $(seq 0 9); 
    do 
        ii=$jj$dd
        jobfile=m$ii\.log
        cat > $jobfile <<EOF
source /home/stewartt/t2ksoft_highland_v2r15.sh

cd $(pwd)

hadd AnaOut_all_sub$ii.root *AnaOut_sub*$ii\.root  > seehadd$ii.log
EOF
        
        chmod +x $jobfile
        
        qsub -l cput=01:30:00 -N $jobfile  ./$jobfile
    done
done

